// ESP32 Robot Remote Control - Clean JavaScript
// Designed specifically for web_interface.html

let socket;
let isConnected = false;
let masterSpeed = 75; // Master speed for all movements

// Replace with your ESP32's IP address
const ESP32_IP = '192.168.1.100'; // Change this to your ESP32's IP

function connectWebSocket() {
    socket = new WebSocket(`ws://${ESP32_IP}:81`);
    
    socket.onopen = function(event) {
        console.log('Connected to ESP32');
        isConnected = true;
        updateConnectionStatus();
    };
    
    socket.onmessage = function(event) {
        const data = JSON.parse(event.data);
        console.log('Received from ESP32:', data);
        
        if (data.type === 'status') {
            // Update motor displays
            updateMotorDisplay(1, data.motor1_speed, data.motor1_target, data.motor1_pwm);
            updateMotorDisplay(2, data.motor2_speed, data.motor2_target, data.motor2_pwm);
            updateMotorDisplay(3, data.motor3_speed, data.motor3_target, data.motor3_pwm);
            updateMotorDisplay(4, data.motor4_speed, data.motor4_target, data.motor4_pwm);
        }
    };
    
    socket.onclose = function(event) {
        console.log('Disconnected from ESP32');
        isConnected = false;
        updateConnectionStatus();
        setTimeout(connectWebSocket, 3000);
    };
    
    socket.onerror = function(error) {
        console.log('WebSocket error:', error);
        isConnected = false;
        updateConnectionStatus();
    };
}

function updateConnectionStatus() {
    const statusElement = document.getElementById('connectionStatus');
    if (statusElement) {
        if (isConnected) {
            statusElement.textContent = 'Connected';
            statusElement.className = 'connection-status connected';
        } else {
            statusElement.textContent = 'Disconnected';
            statusElement.className = 'connection-status disconnected';
        }
    }
}

function updateMotorDisplay(motorNum, speed, target, pwm) {
    const speedElement = document.getElementById(`motor${motorNum}Speed`);
    const targetElement = document.getElementById(`motor${motorNum}Target`);
    const pwmElement = document.getElementById(`motor${motorNum}PWM`);
    
    if (speedElement) speedElement.textContent = speed.toFixed(2);
    if (targetElement) targetElement.textContent = target.toFixed(2);
    if (pwmElement) pwmElement.textContent = pwm.toFixed(2);
}

function updateMasterSpeed(speed) {
    masterSpeed = parseFloat(speed);
    document.getElementById('masterSpeedValue').textContent = masterSpeed;
    console.log('Master speed updated to:', masterSpeed);
}

function sendMovementCommand(command) {
    if (isConnected && socket.readyState === WebSocket.OPEN) {
        const message = {
            command: command,
            speed: masterSpeed
        };
        socket.send(JSON.stringify(message));
        console.log('Sent movement command:', message);
    } else {
        alert('Not connected to ESP32. Check connection.');
    }
}

// Connect when page loads
window.onload = function() {
    connectWebSocket();
};
