const dgram = require('dgram');

// Configurações
const serverAddress = '192.168.137.255'; // Endereço de broadcast (ajuste para sua rede)
const serverPort = 5555; // Porta para enviar dados em broadcast

// Criar um cliente UDP
const client = dgram.createSocket('udp4');

// Dados para enviar
const setPoint = 25; // Exemplo de setpoint
const temperature = 22.5; // Exemplo de temperatura

// Formatar a mensagem
const message = `${setPoint},${temperature}`;

// Enviar mensagem
client.send(message, 0, message.length, serverPort, serverAddress, (err) => {
    if (err) {
        console.error('Erro ao enviar mensagem:', err);
    } else {
        console.log(`Mensagem enviada: ${message}`);
    }

    // Fechar o cliente UDP
    client.close();
});
