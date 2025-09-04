// ESP32 Robot Remote Control - JavaScript
// WebSocket communication and UI controls

let socket;
let isConnected = false;
let statusUpdateInterval = null;
let masterSpeed = 2.0; // Master speed for all movements (like toy car throttle)

// Replace with your ESP32's IP address
const ESP32_IP = '192.168.1.100'; // Change this to your ESP32's IP

function connectWebSocket() {
    socket = new WebSocket(`ws://${ESP32_IP}:81`);
    
    socket.onopen = function(event) {
        console.log('✅ Connected to ESP32');
        isConnected = true;
        updateConnectionStatus();
    };
    
    socket.onmessage = function(event) {
        const data = JSON.parse(event.data);
        console.log('📡 Received from ESP32:', data);
        
        if (data.type === 'status') {
            // Update all 4 motor displays
            updateMotorDisplay(1, data.motor1_speed, data.motor1_target, data.motor1_pwm);
            updateMotorDisplay(2, data.motor2_speed, data.motor2_target, data.motor2_pwm);
            updateMotorDisplay(3, data.motor3_speed, data.motor3_target, data.motor3_pwm);
            updateMotorDisplay(4, data.motor4_speed, data.motor4_target, data.motor4_pwm);
        }
    };
    
    socket.onclose = function(event) {
        console.log('❌ Disconnected from ESP32');
        isConnected = false;
        updateConnectionStatus();
        // Try to reconnect after 3 seconds
        setTimeout(connectWebSocket, 3000);
    };
    
    socket.onerror = function(error) {
        console.log('❌ WebSocket error:', error);
        isConnected = false;
        updateConnectionStatus();
    };
}

function updateConnectionStatus() {
    const statusElement = document.getElementById('connectionStatus');
    if (isConnected) {
        statusElement.textContent = 'Connected to ESP32 ✅';
        statusElement.className = 'connection-status connected';
        
        // Start requesting status updates from ESP32
        if (!statusUpdateInterval) {
            statusUpdateInterval = setInterval(requestStatusUpdate, 100);
        }
    } else {
        statusElement.textContent = 'Disconnected - Attempting to reconnect... ❌';
        statusElement.className = 'connection-status disconnected';
        
        // Stop status updates and simulate local status
        if (statusUpdateInterval) {
            clearInterval(statusUpdateInterval);
            statusUpdateInterval = null;
        }
        simulateMotorStatus();
    }
}

// Simulate motor status when not connected
function simulateMotorStatus() {
    // This simulates the motor response based on target speeds
    for (let i = 1; i <= 4; i++) {
        const targetSpeed = parseFloat(document.getElementById(`motor${i}Target`).textContent);
        
        // Simulate realistic motor response (with some lag and noise)
        const currentSpeed = targetSpeed * (0.85 + Math.random() * 0.3); // 85-115% of target
        const pwmValue = targetSpeed * 42.5; // Approximate PWM conversion (255/6 ≈ 42.5)
        
        // Update with simulated values
        document.getElementById(`motor${i}Speed`).textContent = currentSpeed.toFixed(2);
        document.getElementById(`motor${i}PWM`).textContent = Math.round(pwmValue).toFixed(0);
    }
}

// Master speed control function (like toy car throttle)
function updateMasterSpeed(speed) {
    masterSpeed = parseFloat(speed);
    document.getElementById('masterSpeedValue').textContent = masterSpeed.toFixed(1);
    
    // Show verification popup
    let verificationMsg = `🎚️ MASTER SPEED SET\n` +
                         `New Speed: ${masterSpeed.toFixed(1)} rad/s\n\n` +
                         `💡 This speed will be used for:\n` +
                         `• Forward/Backward: ±${masterSpeed.toFixed(1)} rad/s\n` +
                         `• Turn Left/Right: ${(masterSpeed * 0.5).toFixed(1)} / ${masterSpeed.toFixed(1)} rad/s\n` +
                         `• Spin: ±${masterSpeed.toFixed(1)} rad/s\n\n` +
                         `🎮 Press movement buttons to apply this speed!\n` +
                         `⚠️ Motors won't move until you press a button!`;
    
    showVerificationPopup(verificationMsg);
    
    console.log(`🎚️ Master speed set to: ${masterSpeed} rad/s`);
}

function updateMotorDisplay(motorNum, speed, target, pwm) {
    document.getElementById(`motor${motorNum}Speed`).textContent = speed.toFixed(2);
    document.getElementById(`motor${motorNum}Target`).textContent = target.toFixed(2);
    document.getElementById(`motor${motorNum}PWM`).textContent = pwm.toFixed(2);
}

function updateTargetSpeeds(motorActions) {
    document.getElementById('motor1Target').textContent = motorActions.motor1.toFixed(2);
    document.getElementById('motor2Target').textContent = motorActions.motor2.toFixed(2);
    document.getElementById('motor3Target').textContent = motorActions.motor3.toFixed(2);
    document.getElementById('motor4Target').textContent = motorActions.motor4.toFixed(2);
    
    // Update slider values (legacy compatibility)
    document.getElementById('motor1Slider').value = motorActions.motor1;
    document.getElementById('motor2Slider').value = motorActions.motor2;
    document.getElementById('motor3Slider').value = motorActions.motor3;
    document.getElementById('motor4Slider').value = motorActions.motor4;
    
    // Update slider display values
    document.getElementById('motor1Value').textContent = motorActions.motor1.toFixed(1);
    document.getElementById('motor2Value').textContent = motorActions.motor2.toFixed(1);
    document.getElementById('motor3Value').textContent = motorActions.motor3.toFixed(1);
    document.getElementById('motor4Value').textContent = motorActions.motor4.toFixed(1);
}

// Master speed control - all movements use this speed (toy car style)
function sendMovementCommand(command) {
    // Use the current master speed for all movement commands
    let speed = masterSpeed;
    
    // Create verification message based on command type
    let motorActions = getMotorActionsForCommand(command, speed);
    let verificationMsg = createVerificationMessage(command, speed, motorActions);
    
    // Update target speeds in status display immediately
    updateTargetSpeeds(motorActions);
    
    // Show verification popup
    showVerificationPopup(verificationMsg);
    
    if (isConnected && socket.readyState === WebSocket.OPEN) {
        const message = {
            command: command,
            speed: speed
        };
        socket.send(JSON.stringify(message));
        console.log('✅ SENT TO ESP32:', message);
        console.log('📋 Motor Actions:', motorActions);
    } else {
        alert('❌ Not connected to ESP32. Check connection.');
        console.log('❌ FAILED: WebSocket not connected');
    }
}

// Individual motor control
function updateMotorSpeed(motorNum, speed) {
    document.getElementById(`motor${motorNum}Value`).textContent = parseFloat(speed).toFixed(1);
    
    // Update target speed in status display immediately
    document.getElementById(`motor${motorNum}Target`).textContent = parseFloat(speed).toFixed(2);
    
    // Create verification message for individual motor
    let verificationMsg = `🎚️ SLIDER CONTROL\n` +
                         `Motor ${motorNum}: ${parseFloat(speed).toFixed(1)} rad/s\n` +
                         `Command: {"command":"motor${motorNum}","speed":${parseFloat(speed)}}\n` +
                         `Action: ${speed > 0 ? 'Forward' : speed < 0 ? 'Backward' : 'Stop'} at ${Math.abs(speed).toFixed(1)} rad/s`;
    
    // Show verification popup
    showVerificationPopup(verificationMsg);
    
    if (isConnected && socket.readyState === WebSocket.OPEN) {
        const message = {
            command: `motor${motorNum}`,
            speed: parseFloat(speed)
        };
        socket.send(JSON.stringify(message));
        console.log(`✅ SENT TO ESP32 - Motor ${motorNum}:`, message);
    } else {
        console.log(`❌ FAILED - Motor ${motorNum}: WebSocket not connected`);
    }
}

// Helper function to determine motor actions for movement commands
function getMotorActionsForCommand(command, speed) {
    switch(command) {
        case 'forward':
        case 'moveForward':
            return {
                motor1: speed, motor2: speed, motor3: speed, motor4: speed,
                description: 'All motors forward'
            };
        case 'backward':
        case 'moveBackward':
            return {
                motor1: -speed, motor2: -speed, motor3: -speed, motor4: -speed,
                description: 'All motors backward'
            };
        case 'turnLeft':
            return {
                motor1: speed * 0.5, motor2: speed, motor3: speed * 0.5, motor4: speed,
                description: 'Left motors slower, right motors faster'
            };
        case 'turnRight':
            return {
                motor1: speed, motor2: speed * 0.5, motor3: speed, motor4: speed * 0.5,
                description: 'Right motors slower, left motors faster'
            };
        case 'spinLeft':
            return {
                motor1: -speed, motor2: speed, motor3: -speed, motor4: speed,
                description: 'Left motors reverse, right motors forward'
            };
        case 'spinRight':
            return {
                motor1: speed, motor2: -speed, motor3: speed, motor4: -speed,
                description: 'Right motors reverse, left motors forward'
            };
        case 'stop':
            return {
                motor1: 0, motor2: 0, motor3: 0, motor4: 0,
                description: 'All motors stopped'
            };
        default:
            return {
                motor1: 0, motor2: 0, motor3: 0, motor4: 0,
                description: 'Unknown command - motors stopped'
            };
    }
}

// Helper function to create verification message
function createVerificationMessage(command, speed, motorActions) {
    let message = `🎮 MOVEMENT COMMAND\n`;
    message += `Command: ${command.toUpperCase()}\n`;
    message += `Speed: ${speed} rad/s\n\n`;
    message += `📡 JSON to ESP32:\n`;
    message += `{"command":"${command}","speed":${speed}}\n\n`;
    message += `🔧 Motor Actions:\n`;
    message += `${motorActions.description}\n`;
    message += `• Motor 1 (FL): ${motorActions.motor1.toFixed(1)} rad/s\n`;
    message += `• Motor 2 (FR): ${motorActions.motor2.toFixed(1)} rad/s\n`;
    message += `• Motor 3 (RL): ${motorActions.motor3.toFixed(1)} rad/s\n`;
    message += `• Motor 4 (RR): ${motorActions.motor4.toFixed(1)} rad/s`;
    
    return message;
}

// Function to show verification popup
function showVerificationPopup(message) {
    // Create popup element if it doesn't exist
    let popup = document.getElementById('verificationPopup');
    if (!popup) {
        popup = document.createElement('div');
        popup.id = 'verificationPopup';
        popup.className = 'verification-popup';
        document.body.appendChild(popup);
    }
    
    // Set message and show popup
    popup.textContent = message;
    popup.style.display = 'block';
    popup.style.opacity = '1';
    
    // Auto-hide after 3 seconds
    setTimeout(() => {
        popup.style.opacity = '0';
        setTimeout(() => {
            popup.style.display = 'none';
        }, 300);
    }, 3000);
}

function resetAllMotorStatus() {
    // Reset motor status displays to zero (master speed control - no individual sliders)
    for (let i = 1; i <= 4; i++) {
        document.getElementById(`motor${i}Target`).textContent = '0.00';
        
        // If not connected, also reset current speed and PWM
        if (!isConnected) {
            document.getElementById(`motor${i}Speed`).textContent = '0.00';
            document.getElementById(`motor${i}PWM`).textContent = '0';
        }
    }
}

// Legacy function for compatibility (removed individual motor control)
// Individual motor control replaced with master speed system

// Initialize when page loads
window.onload = function() {
    // Update current IP display
    document.getElementById('currentIP').textContent = ESP32_IP;
    
    // Connect to WebSocket
    connectWebSocket();
    
    // Start simulation updates when not connected
    setInterval(() => {
        if (!isConnected) {
            simulateMotorStatus();
        }
    }, 200); // Update every 200ms when simulating
};
