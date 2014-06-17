/* verify that certain invalid URIs are not parsed by the resource
   protocol handler */

const specs = [
  "resource:////",
  "resource:///http://www.mozilla.org/",
  "resource:///file:///",
  "resource:///..\\",
  "resource:///..\\..\\",
  "resource:///..%5C",
  "resource:///..%5c"
];

function check_for_exception(spec)
{
  var ios =
    Cc["@mozilla.org/network/io-service;1"].
    getService(Ci.nsIIOService);

  try {
    var channel = ios.newChannel2(spec,
                                  null,
                                  null,
                                  Services.scriptSecurityManager.getSystemPrincipal(),
                                  null,      // requestingNode
                                  0,         // securityFlags
                                  Components.interfaces.nsIContentPolicy.TYPE_OTHER,
                                  0);        // loadFlags
  }
  catch (e) {
    return;
  }

  do_throw("Successfully opened invalid URI: '" + spec + "'");
}

function run_test() {
  for each (spec in specs) {
    check_for_exception(spec);
  }
}
