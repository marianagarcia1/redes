const dgram = require('dgram');
const WebSocket = require('ws');

// Configurações
const udpReceivePort = 5555; // Porta para receber dados UDP
const udpSendPort = 5555;    // Porta para enviar dados em broadcast
const wsPort = 8080;         // Porta do servidor WebSocket
const broadcastAddress = '192.168.137.255'; // Endereço de broadcast (ajuste para sua rede)

// Variáveis globais para armazenar os últimos valores válidos
let globalSetpoint = 50;
let lastValidTemperature = null;

// Criar um servidor UDP para receber dados
const udpServer = dgram.createSocket('udp4');

udpServer.on('listening', () => {
    console.log(`Servidor UDP rodando na porta ${udpReceivePort}`);
    udpServer.setBroadcast(true);
});

udpServer.on('message', (message, rinfo) => {
    console.log(`Mensagem recebida: ${message} de ${rinfo.address}:${rinfo.port}`);

    // Converter a mensagem para string e separar os valores
    const data = message.toString().split(',');

    // Convertendo os valores
    const set_point = parseInt(data[0], 10);
    let temperature = parseFloat(data[1]);

    if (isNaN(temperature)) {
        temperature = lastValidTemperature;
        console.log('Temperatura recebida é nula, usando último valor válido:', temperature);
    } else {
        lastValidTemperature = temperature;
    }

    console.log(`Setpoint: ${set_point}°C`);
    console.log(`Temperatura: ${temperature}°C`);

    // Atualizar o valor global do setpoint
    globalSetpoint = set_point;

    // Enviar para os clientes WebSocket conectados
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify({ setpoint: set_point, temperature }));
        }
    });
});

// Iniciar o servidor UDP para receber dados
udpServer.bind(udpReceivePort);

// Criar um socket UDP cliente para enviar dados e configurar broadcast
const udpClient = dgram.createSocket('udp4');
udpClient.on('listening', () => {
    udpClient.setBroadcast(true);
    console.log('Cliente UDP configurado para broadcast');
});
udpClient.bind(); // Bind sem porta específica para configurar broadcast

function sendUDPMessage(message) {
    const broadcastMessage = Buffer.from(message.toString());
    udpClient.send(broadcastMessage, 0, broadcastMessage.length, udpSendPort, broadcastAddress, (err) => {
        if (err) {
            console.error('Erro ao enviar mensagem em broadcast:', err);
        } else {
            console.log('Mensagem enviada em broadcast:', broadcastMessage.toString());
        }
    });
}

// Função para monitorar e enviar mudanças no setpoint
function monitorSetpoint() {
    setInterval(() => {
        sendUDPMessage(globalSetpoint);
    }, 1000); // Verificar a cada segundo
}

// Iniciar monitoramento do setpoint
monitorSetpoint();

// Criar um servidor WebSocket
const wss = new WebSocket.Server({ port: wsPort });

wss.on('connection', ws => {
    console.log('Cliente WebSocket conectado');

    ws.on('message', message => {
        const data = JSON.parse(message);
        console.log('Mensagem recebida do cliente WebSocket:', data);

        // Atualizar o valor global do setpoint
        globalSetpoint = parseInt(data.setpoint, 10);

        // Enviar o novo setpoint via UDP imediatamente
        sendUDPMessage(globalSetpoint);
    });
});

console.log(`Servidor WebSocket rodando na porta ${wsPort}`);
