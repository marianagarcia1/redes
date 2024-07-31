const dgram = require('dgram');
const WebSocket = require('ws');

const udpPort = 12345; // Porta para receber dados UDP
const wsPort = 8080;   // Porta do WebSocket

// Criar um servidor UDP
const udpServer = dgram.createSocket('udp4');

udpServer.on('listening', () => {
    console.log(`Servidor UDP rodando na porta ${udpPort}`);
});

udpServer.on('message', (message, rinfo) => {
    console.log(`Mensagem recebida: ${message} de ${rinfo.address}:${rinfo.port}`);
    // Enviar a mensagem para todos os clientes WebSocket conectados
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(message.toString());
        }
    });
});

udpServer.bind(udpPort);

// Criar um servidor WebSocket
const wss = new WebSocket.Server({ port: wsPort });

wss.on('connection', ws => {
    console.log('Cliente WebSocket conectado');
});

console.log(`Servidor WebSocket rodando na porta ${wsPort}`);

