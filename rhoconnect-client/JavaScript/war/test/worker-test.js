onmessage = function(e){
  if ( e.data === "start" ) {
    // Do some computation
    done()
  }
};
function done(){
  // Send back the results to the parent page
  var async = null;
  var sync = null;
  try { async = openDatabase } catch (ex) { async = ex };
  try { sync = openDatabaseSync } catch (ex) { sync = ex };
  postMessage('In the Worker:');
  postMessage('&nbsp;&nbsp;openDatabase is: ' +async);
  postMessage('&nbsp;&nbsp;openDatabaseSync is: ' +sync);
}

