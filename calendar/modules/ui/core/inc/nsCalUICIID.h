/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#ifndef nsCalUICIID_h__
#define nsCalUICIID_h__

#include "nsISupports.h"
#include "nsIFactory.h"
#include "nsRepository.h"

//29bd8f10-e9e6-11d1-9244-00805f8a7ab6
#define NS_CAL_CONTEXT_CONTROLLER_CID \
{ 0x29bd8f10, 0xe9e6, 0x11d1, \
{ 0x92, 0x44, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//36658790-2032-11d2-9246-00805f8a7ab6
#define NS_CAL_TIMEBAR_CONTEXT_CONTROLLER_CID \
{ 0x36658790, 0x2032, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//07fdbd10-21bb-11d2-9246-00805f8a7ab6
#define NS_CAL_MONTH_CONTEXT_CONTROLLER_CID \
{ 0x07fdbd10, 0x21bb, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//b0d4fa90-e9e6-11d1-9244-00805f8a7ab6
#define NS_CAL_TIME_CONTEXT_CID \
{ 0xb0d4fa90, 0xe9e6, 0x11d1, \
{ 0x92, 0x44, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//7013faa0-1f39-11d2-bed9-00805f8a8dbd
#define NS_CAL_COMPONENT_CID \
{ 0x7013faa0, 0x1f39, 0x11d2, \
{ 0xbe, 0xd9, 0x00, 0x80, 0x5f, 0x8a, 0x8d, 0xbd } }

//8bf4b740-200d-11d2-9246-00805f8a7ab6
#define NS_CAL_TOOLKIT_CID \
{ 0x8bf4b740, 0x200d, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//757fbd20-2039-11d2-9246-00805f8a7ab6
#define NS_CAL_DURATION_COMMAND_CID \
{ 0x757fbd20, 0x2039, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//489a6750-224d-11d2-9246-00805f8a7ab6
#define NS_CAL_DAYLIST_COMMAND_CID \
{ 0x489a6750, 0x224d, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//6e645390-edba-11d1-bec6-00805f8a8dbd
#define NS_CAL_TIMEBARCANVAS_CID \
{ 0x6e645390, 0xedba, 0x11d1, \
{ 0xbe, 0xc6, 0x00, 0x80, 0x5f, 0x8a, 0x8d, 0xbd } }

//025d7730-edbc-11d1-bec6-00805f8a8dbd
#define NS_CAL_TIMEBARCOMPONENTCANVAS_CID \
{ 0x025d7730, 0xedbc, 0x11d1, \
{ 0xbe, 0xc6, 0x00, 0x80, 0x5f, 0x8a, 0x8d, 0xbd } }

//65620940-1b64-11d2-9246-00805f8a7ab6
#define NS_CAL_TODOCOMPONENTCANVAS_CID \
{ 0x65620940, 0x1b64, 0x11d2, \
{ 0x92, 0x64, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//d6da5f20-1b61-11d2-9246-00805f8a7ab6
#define NS_CAL_TIMEBARHEADING_CID \
{ 0xd6da5f20, 0x1b61, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//994b4280-1db7-11d2-bed9-00805f8a8dbd
#define NS_CAL_TIMEBARUSERHEADING_CID \
{ 0x994b4280, 0x1db7, 0x11d2, \
{ 0xbe, 0xd9, 0x00, 0x80, 0x5f, 0x8a, 0x8d, 0xbd } }

//560bd2a0-1db6-11d2-bed9-00805f8a8dbd
#define NS_CAL_TIMEBARTIMEHEADING_CID \
{ 0x560bd2a0, 0x1db6, 0x11d2, \
{ 0xbe, 0xd9, 0x00, 0x80, 0x5f, 0x8a, 0x8d, 0xbd } }

//940fda80-1b66-11d2-9246-00805f8a7ab6
#define NS_CAL_TIMEBARSCALE_CID \
{ 0x940fda80, 0x1b66, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//5b6775e0-1c39-11d2-9246-00805f8a7ab6
#define NS_CAL_MULTIDAYVIEWCANVAS_CID \
{ 0x5b6775e0, 0x1c39, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//879f3230-1da4-11d2-bed9-00805f8a8dbd
#define NS_CAL_DAYVIEWCANVAS_CID \
{ 0x879f3230, 0x1da4, 0x11d2, \
{ 0xbe, 0xd9, 0x00, 0x80, 0x5f, 0x8a, 0x8d, 0xbd } }

//f5b41150-218e-11d2-9246-00805f8a7ab6
#define NS_CAL_STATUSCANVAS_CID \
{ 0xf5b41150, 0x218e, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

//107f9a00-218f-11d2-9246-00805f8a7ab6
#define NS_CAL_COMMANDCANVAS_CID \
{ 0x107f9a00, 0x218f, 0x11d2, \
{ 0x92, 0x46, 0x00, 0x80, 0x5f, 0x8a, 0x7a, 0xb6 } }

#endif
