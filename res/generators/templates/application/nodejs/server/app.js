
var server_port = Rho.System.NodejsServerPort

var path = require('path');
var express = require('express');
var app = express();

app.use('/public', express.static(path.join(__dirname, 'public')));

app.get('/', function (req, res) {
  res.send('Hello World! (' + Date.now() + ")");
});

var server = app.listen(server_port, function () {
  Rho.Log.info("Express server is started. (port: "+server_port+")", "Node.js JS");
  // application must be inform RHomobile platform about starting of http server !
  Mobile.httpServerStarted();
});

