let express = require('./conf_express');
let routes = require('./conf_routes')(express);
let socket = require('./conf_socket');
//let net = require('./conf_net');

let server_http = express.listen(8080, function(){
	console.log("Servidor online.");
});

let socket_io = socket(server_http);


