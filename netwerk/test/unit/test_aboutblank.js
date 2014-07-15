function run_test() {
  var ioServ = Components.classes["@mozilla.org/network/io-service;1"]
                         .getService(Components.interfaces.nsIIOService);

  var base = ioServ.newURI("http://www.example.com", null, null);

  var about1 = ioServ.newURI("about:blank", null, null);
  var about2 = ioServ.newURI("about:blank", null, base);

  var chan1 = ioServ.newChannelFromURI2(about1,
                                       Services.scriptSecurityManager.getSystemPrincipal(),
                                       null, //requestingNode
                                       0,       //securityFlags
                                       Components.interfaces.nsIContentPolicy.TYPE_OTHER,
                                       0)      //loadFlags
                    .QueryInterface(Components.interfaces.nsIPropertyBag2);
  var chan2 = ioServ.newChannelFromURI2(about2,
                                       Services.scriptSecurityManager.getSystemPrincipal(),
                                       null, //requestingNode
                                       0,       //securityFlags
                                       Components.interfaces.nsIContentPolicy.TYPE_OTHER,
                                       0)      //loadFlags
                    .QueryInterface(Components.interfaces.nsIPropertyBag2);

  var haveProp = false;
  var propVal = null;
  try {
    propVal = chan1.getPropertyAsInterface("baseURI",
                                           Components.interfaces.nsIURI);
    haveProp = true;
  } catch (e if e.result == Components.results.NS_ERROR_NOT_AVAILABLE) {
    // Property shouldn't be there.
  }
  do_check_eq(propVal, null);
  do_check_eq(haveProp, false);
  do_check_eq(chan2.getPropertyAsInterface("baseURI",
                                           Components.interfaces.nsIURI),
              base);
}
