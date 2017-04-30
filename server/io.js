var mqtt = require('mqtt');
var mqttClient = mqtt.connect('mqtt://192.168.1.104');
var httpServer = require('http').createServer();
io = require('socket.io')(httpServer, {origins: '*:*'});

io.on('connection', function(client){
    client.on('mqtt', function(msg){
        console.log("ws", msg);
        mqttClient.publish(msg.topic, msg.payload.toString());
    })
});

mqttClient.on('connect', function () {
    mqttClient.subscribe('sensors/#');
});

mqttClient.on('message', function (topic, message) {
    console.log("mqtt", topic, message.toString());
    io.sockets.emit('mqtt', {
        topic: topic,
        payload: message.toString()
    });
});

httpServer.listen(3000, '0.0.0.0');