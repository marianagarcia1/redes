const dgram = require('dgram');
const WebSocket = require('ws');

// Configurações
const udpPort = 12345; // Porta para receber dados UDP
const wsPort = 8080;   // Porta do servidor WebSocket

// Criar um servidor UDP
const udpServer = dgram.createSocket('udp4');

udpServer.on('listening', () => {
    console.log(`Servidor UDP rodando na porta ${udpPort}`);
});

udpServer.on('message', (message, rinfo) => {
    console.log(`Mensagem recebida: ${message} de ${rinfo.address}:${rinfo.port}`);

    // Converter a mensagem para string e separar os valores
    const data = message.toString().split(',');

    // Convertendo os valores
    const set_point = parseInt(data[0], 10);
    const temperature = parseFloat(data[1]);

    console.log(`Setpoint: ${set_point}°C`);
    console.log(`Temperatura: ${temperature}°C`);

    // Enviar para os clientes WebSocket conectados
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify({ setpoint: set_point, temperature }));
        }
    });
});

// Iniciar o servidor UDP
udpServer.bind(udpPort);

// Criar um servidor WebSocket
const wss = new WebSocket.Server({ port: wsPort });

wss.on('connection', ws => {
    console.log('Cliente WebSocket conectado');
});

console.log(`Servidor WebSocket rodando na porta ${wsPort}`);
