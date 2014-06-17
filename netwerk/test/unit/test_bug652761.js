// This is just a crashtest for a url that is rejected at parse time (port 80,000)

function completeTest(request, data, ctx)
{
    do_test_finished();
}

function run_test()
{
    var ios = Components.classes["@mozilla.org/network/io-service;1"].
                         getService(Components.interfaces.nsIIOService);
    var chan = ios.newChannel2("http://localhost:80000/",
                               "",
                               null,
                               Services.scriptSecurityManager.getSystemPrincipal(),
                               null,      // requestingNode
                               0,         // securityFlags
                               Components.interfaces.nsIContentPolicy.TYPE_OTHER,
                               0);        // loadFlags
    var httpChan = chan.QueryInterface(Components.interfaces.nsIHttpChannel);
    httpChan.asyncOpen(new ChannelListener(completeTest,
                                           httpChan, CL_EXPECT_FAILURE), null);
    do_test_pending();
}

