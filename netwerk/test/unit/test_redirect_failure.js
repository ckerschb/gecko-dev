Cu.import("resource://testing-common/httpd.js");

XPCOMUtils.defineLazyGetter(this, "URL", function() {
  return "http://localhost:" + httpServer.identity.primaryPort;
});

var httpServer = null;
// Need to randomize, because apparently no one clears our cache
var randomPath = "/redirect/" + Math.random();

XPCOMUtils.defineLazyGetter(this, "randomURI", function() {
  return URL + randomPath;
});

function make_channel(url, callback, ctx) {
  var ios = Cc["@mozilla.org/network/io-service;1"].
            getService(Ci.nsIIOService);
  return ios.newChannel2(url,
                         "",
                         null,
                         Services.scriptSecurityManager.getSystemPrincipal(),
                         null,      // requestingNode
                         0,         // securityFlags
                         Components.interfaces.nsIContentPolicy.TYPE_OTHER,
                         0);        // loadFlags
}

function redirectHandler(metadata, response)
{
  response.setStatusLine(metadata.httpVersion, 301, "Moved");
  response.setHeader("Location", "httpx://localhost:" +
                     httpServer.identity.primaryPort + "/content", false);
  response.setHeader("Cache-Control", "no-cache", false);
}

function finish_test(request, buffer)
{
  do_check_eq(request.status, Components.results.NS_ERROR_UNKNOWN_PROTOCOL);

  do_check_eq(buffer, "");
  httpServer.stop(do_test_finished);
}

function run_test()
{
  httpServer = new HttpServer();
  httpServer.registerPathHandler(randomPath, redirectHandler);
  httpServer.start(-1);

  var chan = make_channel(randomURI);
  chan.asyncOpen2(new ChannelListener(finish_test, null, CL_EXPECT_FAILURE), null);
  do_test_pending();
}
