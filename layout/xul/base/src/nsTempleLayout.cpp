/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is Netscape Communications
 * Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 */

//
// Eric Vaughan
// Netscape Communications
//
// See documentation in associated header file
//

#include "nsTempleLayout.h"
#include "nsIBox.h"
#include "nsCOMPtr.h"
#include "nsIScrollableFrame.h"

nsresult
NS_NewTempleLayout( nsIPresShell* aPresShell, nsCOMPtr<nsIBoxLayout>& aNewLayout)
{
  aNewLayout = new nsTempleLayout(aPresShell);

  return NS_OK;
  
} 

nsTempleLayout::nsTempleLayout(nsIPresShell* aPresShell):nsMonumentLayout(aPresShell), mMonuments(nsnull)
{
}

NS_IMETHODIMP
nsTempleLayout::CastToTemple(nsTempleLayout** aTemple)
{
  *aTemple = this;
  return NS_OK;
}

NS_IMETHODIMP
nsTempleLayout::EnscriptionChanged(nsBoxLayoutState& aState, PRInt32 aIndex)
{
  // if a cell changes size. 
  if (mMonuments) {
     nsBoxSizeList* size = mMonuments->GetAt(aIndex);
     if (size)
        size->Desecrate(aState);
  }

  return NS_OK;
}

NS_IMETHODIMP
nsTempleLayout::GetMonumentList(nsIBox* aBox, nsBoxLayoutState& aState, nsBoxSizeList** aList)
{
  if (mMonuments) { 
     *aList = mMonuments;
     return NS_OK;
  }

  *aList = nsnull;

  // run through our children.
  // ask each child for its monument list
  // append the list to our list
  nsIBox* box = nsnull;
  aBox->GetChildBox(&box);
  nsBoxSizeList* current = nsnull;
  nsCOMPtr<nsIMonument> monument;

  nsMonumentIterator it(box);

  while(it.GetNextMonument(getter_AddRefs(monument))) {

      it.GetBox(&box);

      if (!mMonuments) {
          mMonuments = new nsBoxSizeListImpl(box);
          mMonuments->AddRef();
        }

      current = mMonuments;
      nsBoxSizeList* node = nsnull;
      monument->GetMonumentList(box, aState, &node);

      if (node)
          node->AddRef();

      while(node)
      {
        current->Append(aState, node);
        node->Release(aState);

        nsBoxSizeList* tmp = node->GetNext();
        if (tmp)
          tmp->AddRef();

        node->SetNext(aState, nsnull);
        node = tmp;

        if (node && !current->GetNext()) {
          nsBoxSizeList* newOne = new nsBoxSizeListImpl(box);
          current->SetNext(aState, newOne);
          current = newOne;
        } else {
          current = current->GetNext();
        }
      }    
  }

  *aList = mMonuments;
  return NS_OK;
}

NS_IMETHODIMP
nsTempleLayout::BuildBoxSizeList(nsIBox* aBox, nsBoxLayoutState& aState, nsBoxSize*& aFirst, nsBoxSize*& aLast, PRBool aIsHorizontal)
{
  // ok we need to build a nsBoxSize for each obelisk in this temple. We will then return the list of them.
  // We are just returning a flattened list that we will use to layout our each cell.
  nsIBox* box = nsnull;
  aBox->GetChildBox(&box);

  aFirst = nsnull;
  aLast = nsnull;

  nsBoxSize* first;
  nsBoxSize* last;
  PRInt32 count = 0;

  nsCOMPtr<nsIBoxLayout> layout;

  nsLayoutIterator it(box);

  while(it.GetNextLayout(getter_AddRefs(layout))) {

    it.GetBox(&box);

    if (layout) {
      nsresult rv = NS_OK;
      nsCOMPtr<nsIMonument> monument = do_QueryInterface(layout, &rv);

      if (monument) 
           monument->BuildBoxSizeList(box, aState, first, last, aIsHorizontal);
      else {
           nsMonumentLayout::BuildBoxSizeList(box, aState, first, last, aIsHorizontal);
           first->bogus = PR_TRUE;
      }

      if (count == 0)
        aFirst = first;
      else  if (aLast)
        (aLast)->next = first;
      aLast = last;
    }
    
    count++;
  }

  // ok now we might have a margin or border. If we do then we need to take that into account. One example 
  // might be if we are oriented vertically making us a "columns" and we contain a horizontal obelisk "row".
  // Now say we have a left border of 10px. Well if we just layed things out then the whole row would be pushed over and
  // the columns would not not line up. So we must take the space from somewhere. So if its on the left we take from the first 
  // child (which is leftmost) and if its on the right we take from the last child (which is rightmost). So for our example we
  // need to subtract 10px from the first child.
  
  // so get the border and padding and add them up.
  nsMargin borderPadding(0,0,0,0);
  aBox->GetBorderAndPadding(borderPadding);
  nsMargin margin(0,0,0,0);
  aBox->GetMargin(margin);

  // add the margins up
  nsMargin leftMargin(borderPadding.left + margin.left, borderPadding.top + margin.top, 0, 0);
  nsMargin rightMargin(0,0, borderPadding.right + margin.right, borderPadding.bottom + margin.bottom);

  // Subtract them out.
  PRBool isHorizontal = PR_FALSE;
  aBox->GetOrientation(isHorizontal);

  if (aFirst)
    aFirst->Add(leftMargin,isHorizontal);
  
  if (aLast)
    aLast->Add(rightMargin,isHorizontal);

  return NS_OK;
}

NS_IMETHODIMP
nsTempleLayout::ChildrenInserted(nsIBox* aBox, nsBoxLayoutState& aState, nsIBox* aPrevBox, nsIBox* aChildList)
{
  DesecrateMonuments(aBox, aState);
  return NS_OK;
}

NS_IMETHODIMP
nsTempleLayout::ChildrenAppended(nsIBox* aBox, nsBoxLayoutState& aState, nsIBox* aChildList)
{
  DesecrateMonuments(aBox, aState);
  return NS_OK;
}

NS_IMETHODIMP
nsTempleLayout::ChildrenRemoved(nsIBox* aBox, nsBoxLayoutState& aState, nsIBox* aChildList)
{
  DesecrateMonuments(aBox, aState);
  return NS_OK;
}

NS_IMETHODIMP
nsTempleLayout::DesecrateMonuments(nsIBox* aBox, nsBoxLayoutState& aState)
{
  if (mMonuments) {
    nsBoxSizeList* tmp = mMonuments;
    mMonuments = nsnull;
    tmp->Release(aState);
  }

  return NS_OK;
}

