var Cc = Components.classes;
var Ci = Components.interfaces;

// Regression test for bug 370103 - crash when passing a null listener to
// nsIChannel.asyncOpen
function run_test() {
  // Compose the jar: url
  var ioService = Cc["@mozilla.org/network/io-service;1"].
                    getService(Ci.nsIIOService);
  var file = do_get_file("data/test_bug370103.jar");
  var url = ioService.newFileURI(file).spec;
  url = "jar:" + url + "!/test_bug370103";

  // Try opening channel with null listener
  var channel = ioService.newChannel2(url,
                                      null,
                                      null,
                                      Services.scriptSecurityManager.getSystemPrincipal(),
                                      null,      // requestingNode
                                      0,         // securityFlags
                                      Components.interfaces.nsIContentPolicy.TYPE_OTHER,
                                      0);        // loadFlags

  var exception = false;
  try {
    channel.asyncOpen2(null, null);
  }
  catch(e) {
    exception = true;
  }

  do_check_true(exception); // should throw exception instead of crashing
}
