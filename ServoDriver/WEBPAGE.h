const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>

<head>
    <title>Servo Driver with ESP32</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link rel="icon" href="data:,">
    <style>
        html {
        font-family: Arial;
        display: inline-block;
        background: #000000;
        color: #efefef;
        text-align: center;
    }

    h2 {
        font-size: 3.0rem;
    }

    p {
        font-size: 1.0rem;
    }

    body {
        max-width: 600px;
        margin: 0px auto;
        padding-bottom: 25px;
    }

    button {
        display: inline-block;
        margin: 5px;
        padding: 10px 10px;
        border: 0;
        line-height: 21px;
        cursor: pointer;
        color: #fff;
        background: #4247b7;
        border-radius: 5px;
        font-size: 21px;
        outline: 0;
        width: 100px

        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;

        user-select: none;
    }

    button:hover {
        background: #ff494d
    }

    button:active {
        background: #f21c21
    }

    </style>
</head>

<body>
    <h3>SERVO DRIVER with ESP32</h3>
    <p>
    <span id="IDValue">Click this button to start searching servos.</span>
    <p>
    <label align="center"><button class="button" onclick="toggleCheckbox(9, 0, 0, 0);">Start Searching</button></label>
    <p>
    <span id="STSValue">Single servo infomation.</span>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(0, 1, 0, 0);">ID Select+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(0, -1, 0, 0);">ID Select-</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 1, 0, 0);">Middle</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 2, 0, 0);">Stop</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 3, 0, 0);">Release</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 4, 0, 0);">Torque</button></label>
    <p>
        <label align="center"><button class="button" onmousedown="toggleCheckbox(1, 5, 0, 0);" ontouchstart="toggleCheckbox(1, 5, 0, 0);" onmouseup="toggleCheckbox(1, 2, 0, 0);" ontouchend="toggleCheckbox(1, 2, 0, 0);">Position+</button></label>
        <label align="center"><button class="button" onmousedown="toggleCheckbox(1, 6, 0, 0);" ontouchstart="toggleCheckbox(1, 6, 0, 0);" onmouseup="toggleCheckbox(1, 2, 0, 0);" ontouchend="toggleCheckbox(1, 2, 0, 0);">Position-</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 7, 0, 0);">Speed+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 8, 0, 0);">Speed-</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 9, 0, 0);">ID to Set+</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 10, 0, 0);">ID to Set-</button></label>
    <p>
        <label align="center"><button class="button" onclick="setMiddle();">Set Middle Position</button></label>
        <label align="center"><button class="button" onclick="setNewID();">Set New ID</button></label>
    <p>
        <label align="center"><button class="button" onclick="setServoMode();">Set Servo Mode</button></label>
        <label align="center"><button class="button" onclick="setStepperMode();">Set Motor Mode</button></label>
    <p>
        <label align="center"><button class="button" id="serialForwarding" onclick="serialForwarding();">Start Serial Forwarding</button></label>
    <p>
        <label align="center"><button class="button" onclick="setRole(0);">Normal</button></label>
        <label align="center"><button class="button" onclick="setRole(1);">Leader</button></label>
        <label align="center"><button class="button" onclick="setRole(2);">Follower</button></label>
    <p>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 20, 0, 0);">RainbowON</button></label>
        <label align="center"><button class="button" onclick="toggleCheckbox(1, 21, 0, 0);">RainbowOFF</button></label>
    <script>
        serialForwardStatus = false;

        function toggleCheckbox(inputT, inputI, inputA, inputB) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", "cmd?inputT="+inputT+"&inputI="+inputI+"&inputA="+inputA+"&inputB="+inputB, true);
            xhr.send();
        }

        function ctrlMode() {
            xhr.open("GET", "ctrl", true);
            xhr.send();
        }

        setInterval(function() {
          getData();
        }, 300);

        setInterval(function() {
          getServoID();
        }, 1500);

        function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("STSValue").innerHTML =
                  this.responseText;
                }
            };
            xhttp.open("GET", "readSTS", true);
            xhttp.send();
        }

        function getServoID() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                  document.getElementById("IDValue").innerHTML =
                  this.responseText;
                }
            };
            xhttp.open("GET", "readID", true);
            xhttp.send();
        }

        function setRole(modeNum){
            if(modeNum == 0){
                var r=confirm("Set the role as Normal. Dev won't send or receive data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 17, 0, 0);
                }
            }
            if(modeNum == 1){
                var r=confirm("Set the role as Leader. Dev will send data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 18, 0, 0);
                }
            }
            if(modeNum == 2){
                var r=confirm("Set the role as Follower. Dev will receive data via ESP-NOW.");
                if(r==true){
                    toggleCheckbox(1, 19, 0, 0);
                }
            }
        }

        function setMiddle(){
            var r=confirm("The middle position of the active servo will be set.");
            if(r==true){
                toggleCheckbox(1, 11, 0, 0);
            }
        }

        function setServoMode(){
            var r=confirm("The active servo will be set as servoMode.");
            if(r==true){
                toggleCheckbox(1, 12, 0, 0);
            }
        }

        function setStepperMode(){
            var r=confirm("The active servo will be set as motorMode.");
            if(r==true){
                toggleCheckbox(1, 13, 0, 0);
            }
        }

        function setNewID(){
            var r=confirm("A new ID of the active servo will be set.");
            if(r==true){
                toggleCheckbox(1, 16, 0, 0);
            }
        }

        function serialForwarding(){
            if(!serialForwardStatus){
                var r=confirm("Do you want to start serial forwarding?");
                if(r){
                    toggleCheckbox(1, 14, 0, 0);
                    serialForwardStatus = true;
                    document.getElementById("serialForwarding").innerHTML = "Stop Serial Forwarding";
                }
            }
            else{
                var r=confirm("Do you want to stop serial forwarding?");
                if(r){
                    toggleCheckbox(1, 15, 0, 0);
                    serialForwardStatus = false;
                    document.getElementById("serialForwarding").innerHTML = "Start Serial Forwarding";
                }
            }
        }

    </script>
</body>
</html>
)rawliteral";