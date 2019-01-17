module.exports = function socket(server){
	let net = require('net');
	let socketIO = require('socket.io').listen(server);			
	var serialport = require('serialport');
	const parsers = serialport.parsers;
	const Readline = require('@serialport/parser-readline')
	
	const parser = new parsers.Readline({
		delimiter: '\n'
	})
	// var serialPort = new serialport('/dev/ttyUSB3',{
	// 	baudRate: 115200,
	// 	stopBits: 1,
	// 	parity: 'none',
	// 	dataBits: 8,
	// 	lock: false,
	// 	xon: false,
  	// 	xoff: false,
  	// 	xany: false,
  	// 	rtscts: false,
  	// 	hupcl: true,
	// 	autoOpen: true,
	// 	flowControl: false,
	// 	parser: parsers.raw,
	// 	//platformOptions: SerialPortBinding.platformOptions,
	// 	bufferSize: 64 * 1024,
	// });
	function delay(ms) {
		ms += new Date().getTime();
		while (new Date() < ms){}
	 }

	// serialPort.pipe(parser);
	// serialPort.on("open", function () {
	// 	console.log('Comunicação serial aberta');
	// 	//let texto = '1234567890111';
	// 	const texto = 'GET=1;IP';
	// 	console.log("Palavra enviada:"+ texto);
	// 	for(let i =0; i < texto.length; i++){
	// 		serialPort.write(texto[i],function(err){
	// 			console.log(err);
				
	// 		});
	// 		delay(2000);
	// 		console.log(texto[i]);
	// 	}
	// 	serialPort.write("1",function(err){
	// 		console.log(err);
	// 	});
		
	// 	serialPort.write("GET=0");
		
	//  });
	// parser.on('data', function(data) {
	// 	console.log(data);
	// });
	// serialPort.on("close", function(err){
	// 	console.log(err);
	// 	console.log("Comunicação serial finalizada");
	//   })
	let netServer = net.createServer(function(res){  		
		//console.log('Socket servidor conectado');
	
		socketIO.on('connection', function(client){
			
			console.log('Usuário conectou no Websocket');
			
			client.on('join', function(name){
				console.log('Usuário '+ client +'conectou no socket!');
				client.emit("update", "Você conectou no socket servidor.");
			});

			client.on('send', function(msg){
				console.log("Message "+ msg);
			});

			client.on('disconnect', function(){
				console.log("Usuário desconectou do Websocket.");
			});

			client.on('teste',function(botao){
				console.log('botão '+ botao+ ' pressionado');
				res.write(botao + '\n', function(teste){
					console.log("resultado" + teste);
				});

			})
		});

	});	

	netServer.listen(9000, '0.0.0.0', function(teste){
		console.log('Socket conectado na porta 9000.');
	});


	netServer.on('end', function(){
		console.log('Servidor socket desconectado.');
	});

	netServer.on("connection", function(teste){
		console.log("Cliente socket conectado com sucesso");

	});

}




  