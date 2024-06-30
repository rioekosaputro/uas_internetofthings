const ws = new WebSocket('ws://usm.revolusi-it.com:9777');

let tempData = [];

let humData = [];


const tempChart = new Chart(document.getElementById('tempChart').getContext('2d'), {

type: 'line',

data: {

labels: [],

datasets: [{

label: 'Suhu',

data: tempData,

borderColor: 'rgba(255, 99, 132, 1)',

borderWidth: 1

}]

},

options: {

scales: {

x: { beginAtZero: true },

y: { beginAtZero: true }

}

}

});


const humChart = new Chart(document.getElementById('humChart').getContext('2d'), {

type: 'line',

data: {

labels: [],

datasets: [{

label: 'Kelembaban',

data: humData,

borderColor: 'rgba(54, 162, 235, 1)',

borderWidth: 1

}]

},

options: {

scales: {

x: { beginAtZero: true },

y: { beginAtZero: true }

}

}

});


ws.onmessage = (event) => {

const message = JSON.parse(event.data);

const time = new Date().toLocaleTimeString();


tempData.push(message.suhu);

humData.push(message.kelembaban);


tempChart.data.labels.push(time);

humChart.data.labels.push(time);


tempChart.update();

humChart.update();

};


const sendMQTTMessage = (buttonId, state) => {

const message = {

nim: 'G.231.21.0206',

[buttonId]: state

};

ws.send(JSON.stringify(message));

};


document.getElementById('D3on').addEventListener('click', () => {

sendMQTTMessage('D3', 1);

});


document.getElementById('D4on').addEventListener('click', () => {

sendMQTTMessage('D4', 1);

});


document.getElementById('D5on').addEventListener('click', () => {

sendMQTTMessage('D5', 1);

});


document.getElementById('D3off').addEventListener('click', () => {

sendMQTTMessage('D3', 0);

});


document.getElementById('D4off').addEventListener('click', () => {

sendMQTTMessage('D4', 0);

});


document.getElementById('D5off').addEventListener('click', () => {

sendMQTTMessage('D5', 0);

});