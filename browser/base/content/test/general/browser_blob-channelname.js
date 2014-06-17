/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
Cu.import("resource://gre/modules/NetUtil.jsm");

function test() {
    var file = new File(new Blob(['test'], {type: 'text/plain'}), {name: 'test-name'});
    var url = URL.createObjectURL(file);
    var channel = NetUtil.newChannel2(url,
                                      Services.scriptSecurityManager.getSystemPrincipal(),
                                      null,      // requestingNode
                                      0,         // securityFlags
                                      Components.interfaces.nsIContentPolicy.TYPE_OTHER,
                                      0);        // loadFlags

    is(channel.contentDispositionFilename, 'test-name', "filename matches");
}
