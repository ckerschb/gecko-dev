/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsAboutBlank.h"
#include "nsStringStream.h"
#include "nsNetUtil.h"

NS_IMPL_ISUPPORTS(nsAboutBlank, nsIAboutModule)

NS_IMETHODIMP
nsAboutBlank::NewChannel(nsIURI *aURI, nsIChannel **result)
{
    NS_ASSERTION(false, "Deprecated, you should use NewChannel2");
    // ckerschb: commenting rest of function to get merge conflicts
    // when merging with master
    return NS_ERROR_NOT_IMPLEMENTED;

    // NS_ENSURE_ARG_POINTER(aURI);

    // nsCOMPtr<nsIInputStream> in;
    // nsresult rv = NS_NewCStringInputStream(getter_AddRefs(in), EmptyCString());
    // if (NS_FAILED(rv)) return rv;

    // nsCOMPtr<nsIChannel> channel;
    // rv = NS_NewInputStreamChannel(getter_AddRefs(channel), aURI, in,
    //                               NS_LITERAL_CSTRING("text/html"),
    //                               NS_LITERAL_CSTRING("utf-8"));
    // if (NS_FAILED(rv)) return rv;

    // channel.forget(result);
    // return rv;
}

NS_IMETHODIMP
nsAboutBlank::NewChannel2(nsIURI* aURI,
                          nsIPrincipal* aRequestingPrincipal,
                          /* nsINode* */ nsISupports* aRequestingNode,
                          uint32_t aSecurityFlags,
                          nsContentPolicyType aContentPolicyType,
                          uint32_t aLoadFlags,
                          nsIChannel** outChannel)
{
    NS_ASSERTION(aRequestingPrincipal, "Can not create channel without aRequestingPrincipal");

    // NewChannel() calls NS_NewInputStreamChannel().  We need to pass the load info
    // to the input stream channel, so we can't call NewChannel() directly.  Implementing it here inline
    NS_ENSURE_ARG_POINTER(aURI);

    nsCOMPtr<nsIInputStream> in;
    nsresult rv = NS_NewCStringInputStream(getter_AddRefs(in), EmptyCString());
    if (NS_FAILED(rv)) return rv;

    nsCOMPtr<nsIChannel> channel;
    rv = NS_NewInputStreamChannel2(getter_AddRefs(channel), aURI, in,
                                   NS_LITERAL_CSTRING("text/html"),
                                   NS_LITERAL_CSTRING("utf-8"),
                                   aRequestingPrincipal,
                                   aRequestingNode,
                                   aContentPolicyType);
    if (NS_FAILED(rv)) return rv;

    channel.forget(outChannel);

    // info gets set in NS_NewInputStreamChannel2
    // (*outChannel)->SetContentPolicyType(aContentPolicyType);
    // (*outChannel)->SetRequestingContext(aRequestingNode);
    // (*outChannel)->SetRequestingPrincipal(aRequestingPrincipal);
    return rv;
}

NS_IMETHODIMP
nsAboutBlank::GetURIFlags(nsIURI *aURI, uint32_t *result)
{
    *result = nsIAboutModule::URI_SAFE_FOR_UNTRUSTED_CONTENT |
              nsIAboutModule::HIDE_FROM_ABOUTABOUT;
    return NS_OK;
}

nsresult
nsAboutBlank::Create(nsISupports *aOuter, REFNSIID aIID, void **aResult)
{
    nsAboutBlank* about = new nsAboutBlank();
    if (about == nullptr)
        return NS_ERROR_OUT_OF_MEMORY;
    NS_ADDREF(about);
    nsresult rv = about->QueryInterface(aIID, aResult);
    NS_RELEASE(about);
    return rv;
}

////////////////////////////////////////////////////////////////////////////////
