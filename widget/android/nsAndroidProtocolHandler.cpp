/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/* vim:set ts=4 sw=4 sts=4 et cin: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsAndroidProtocolHandler.h"
#include "nsCOMPtr.h"
#include "nsIChannel.h"
#include "nsIIOService.h"
#include "nsNetUtil.h"
#include "android/log.h"
#include "nsBaseChannel.h"
#include "AndroidBridge.h"
#include "GeneratedJNIWrappers.h"

using namespace mozilla;
using namespace mozilla::widget::android;

class AndroidInputStream : public nsIInputStream
{
public:
    AndroidInputStream(jobject connection) {
        JNIEnv *env = GetJNIForThread();
        mBridgeInputStream = env->NewGlobalRef(GeckoAppShell::CreateInputStream(connection));
        mBridgeChannel = env->NewGlobalRef(AndroidBridge::ChannelCreate(mBridgeInputStream));
    }
    virtual ~AndroidInputStream() {
        JNIEnv *env = GetJNIForThread();
        env->DeleteGlobalRef(mBridgeInputStream);
        env->DeleteGlobalRef(mBridgeChannel);
    }
    NS_DECL_THREADSAFE_ISUPPORTS
    NS_DECL_NSIINPUTSTREAM

    private:
    jobject mBridgeInputStream;
    jobject mBridgeChannel;
};

NS_IMPL_ISUPPORTS(AndroidInputStream, nsIInputStream)

NS_IMETHODIMP AndroidInputStream::Close(void) {
    mozilla::AndroidBridge::InputStreamClose(mBridgeInputStream);
    return NS_OK;
}

NS_IMETHODIMP AndroidInputStream::Available(uint64_t *_retval) {
    *_retval = mozilla::AndroidBridge::InputStreamAvailable(mBridgeInputStream);
    return NS_OK;
}

NS_IMETHODIMP AndroidInputStream::Read(char *aBuf, uint32_t aCount, uint32_t *_retval) {
    return  mozilla::AndroidBridge::InputStreamRead(mBridgeChannel, aBuf, aCount, _retval);
}

NS_IMETHODIMP AndroidInputStream::ReadSegments(nsWriteSegmentFun aWriter, void *aClosure, uint32_t aCount, uint32_t *_retval) {
    return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP AndroidInputStream::IsNonBlocking(bool *_retval) {
    *_retval = false;
    return NS_OK;
}


class AndroidChannel : public nsBaseChannel
{
private:
    AndroidChannel(nsIURI *aURI, jobject aConnection) {
        JNIEnv *env = GetJNIForThread();
        mConnection = env->NewGlobalRef(aConnection);
        mURI = aURI;
        nsCString type;
        jstring jtype = GeckoAppShell::ConnectionGetMimeType(mConnection);
        if (jtype)
            SetContentType(nsJNICString(jtype, env));
    }
public:
    static AndroidChannel* CreateChannel(nsIURI *aURI)  {
        nsCString spec;
        aURI->GetSpec(spec);
        jobject connection = GeckoAppShell::GetConnection(spec);
        if (!connection)
            return NULL;
        return new AndroidChannel(aURI, connection);
    }
    ~AndroidChannel() {
        JNIEnv *env = GetJNIForThread();
        env->DeleteGlobalRef(mConnection);
    }

    virtual nsresult OpenContentStream(bool async, nsIInputStream **result,
                                       nsIChannel** channel) {
        nsCOMPtr<nsIInputStream> stream = new AndroidInputStream(mConnection);
        NS_ADDREF(*result = stream);
        return NS_OK;
    }
private:
    jobject mConnection;
};

NS_IMPL_ISUPPORTS(nsAndroidProtocolHandler,
                  nsIProtocolHandler,
                  nsISupportsWeakReference)


NS_IMETHODIMP
nsAndroidProtocolHandler::GetScheme(nsACString &result)
{
    result.AssignLiteral("android");
    return NS_OK;
}

NS_IMETHODIMP
nsAndroidProtocolHandler::GetDefaultPort(int32_t *result)
{
    *result = -1;        // no port for android: URLs
    return NS_OK;
}

NS_IMETHODIMP
nsAndroidProtocolHandler::AllowPort(int32_t port, const char *scheme, bool *_retval)
{
    // don't override anything.
    *_retval = false;
    return NS_OK;
}

NS_IMETHODIMP
nsAndroidProtocolHandler::GetProtocolFlags(uint32_t *result)
{
    *result = URI_STD | URI_IS_UI_RESOURCE | URI_IS_LOCAL_RESOURCE | URI_NORELATIVE | URI_DANGEROUS_TO_LOAD;
    return NS_OK;
}

NS_IMETHODIMP
nsAndroidProtocolHandler::NewURI(const nsACString &aSpec,
                                 const char *aCharset,
                                 nsIURI *aBaseURI,
                                 nsIURI **result)
{
    nsresult rv;

    nsCOMPtr<nsIStandardURL> surl(do_CreateInstance(NS_STANDARDURL_CONTRACTID, &rv));
    NS_ENSURE_SUCCESS(rv, rv);

    rv = surl->Init(nsIStandardURL::URLTYPE_STANDARD, -1, aSpec, aCharset, aBaseURI);
    if (NS_FAILED(rv))
        return rv;

    nsCOMPtr<nsIURL> url(do_QueryInterface(surl, &rv));
    NS_ENSURE_SUCCESS(rv, rv);

    surl->SetMutable(false);

    NS_ADDREF(*result = url);
    return NS_OK;
}

NS_IMETHODIMP
nsAndroidProtocolHandler::NewChannel(nsIURI* aURI,
                                     nsIChannel* *aResult)
{
    nsCOMPtr<nsIChannel> channel = AndroidChannel::CreateChannel(aURI);
    if (!channel)
        return NS_ERROR_FAILURE;
    NS_ADDREF(*aResult = channel);
    return NS_OK;
}


/* There appear to be no callers to NewChannel().  After updating all callsites from
 * NewChannel() to NewChannel2(), the browser still runs - the absence of
 * nsAndroidProtocolHandler:NewChannel2() causes no issues, and hence the need for it
 * and NewChannel in nsAndroidProtocolHandler may only be for addons.  Including it here
 * for completeness, and we can followup on this later. */

NS_IMETHODIMP
nsAndroidProtocolHandler::NewChannel2(nsIURI* aURI,
                                      nsIPrincipal* aRequestingPrincipal,
                                      /* nsINode* */ nsISupports* aRequestingNode,
                                      uint32_t aSecurityFlags,
                                      nsContentPolicyType aContentPolicyType,
                                      uint32_t aLoadFlags,
                                      nsIChannel** outChannel)
{
  NS_ASSERTION(aRequestingPrincipal, "Can not create channel without aRequestingPrincipal");
  nsresult rv = NewChannel(aURI, outChannel);
  NS_ENSURE_SUCCESS(rv, rv);
  (*outChannel)->SetContentPolicyType(aContentPolicyType);
  (*outChannel)->SetRequestingContext(aRequestingNode);
  (*outChannel)->SetRequestingPrincipal(aRequestingPrincipal);
  return NS_OK;
}
