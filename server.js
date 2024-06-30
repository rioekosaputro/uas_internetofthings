const express = require('express');

const WebSocket = require('ws');

const mqtt = require('mqtt');


// Set up Express

const app = express();

const port = 9777;


app.use(express.static('public'));


const server = app.listen(port, () => {

console.log(`Server running at http://localhost:${port}`);

});


// Set up WebSocket

const wss = new WebSocket.Server({ server });


wss.on('connection', (ws) => {

console.log('WebSocket connected');


const mqttClient = mqtt.connect('mqtt://usm.revolusi-it.com', {

username: 'usm',

password: 'usmjaya24'

});


mqttClient.on('connect', () => {

mqttClient.subscribe('test/test');

});


mqttClient.on('message', (topic, message) => {

if (topic === 'test/test') {

ws.send(message.toString());

}

});


ws.on('message', (data) => {

const msg = JSON.parse(data);

mqttClient.publish('test/test', JSON.stringify(msg));

});


ws.on('close', () => {

mqttClient.end();

});

});