let http = require('http');

let server = http.createServer(function(req, res){
	let caminho = req.url;
	res.end("<html><body>Hello World!</body></html>");
});

server.listen(3000);

