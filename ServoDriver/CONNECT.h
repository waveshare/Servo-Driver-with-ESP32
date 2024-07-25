// https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
#include <esp_now.h>
#include <WiFi.h>
#include <WebServer.h>
#include "WEBPAGE.h"


// Create AsyncWebServer object on port 80
WebServer server(80);


// select the ID of active servo.
void activeID(int cmdInput){
  activeNumInList += cmdInput;
  if(activeNumInList >= searchNum){
    activeNumInList = 0;
  }
  else if(activeNumInList < 0){
    activeNumInList = searchNum;
  }
}


void activeSpeed(int cmdInput){
  activeServoSpeed += cmdInput;
  if (activeServoSpeed > ServoMaxSpeed){
    activeServoSpeed = ServoMaxSpeed;
  }
  else if(activeServoSpeed < 0){
    activeServoSpeed = 0;
  }
}


int rangeCtrl(int rawInput, int minInput, int maxInput){
  if(rawInput > maxInput){
    return maxInput;
  }
  else if(rawInput < minInput){
    return minInput;
  }
  else{
    return rawInput;
  }
}


void activeCtrl(int cmdInput){
  switch(cmdInput){
    // Middle
    case 1:st.WritePosEx(listID[activeNumInList], ServoDigitalMiddle, activeServoSpeed, ServoInitACC);break;
    // Stop
    case 2:
      if(modeRead[listID[activeNumInList]] == 0) {
        servoStop(listID[activeNumInList]);
      }
      else if(modeRead[listID[activeNumInList]] == 3){
        st.WritePosEx(listID[activeNumInList], 0, 0, 0);
      }
      break;
    // Release
    case 3:servoTorque(listID[activeNumInList],0);Torque_List[activeNumInList] = false;break;
    // Torque
    case 4:servoTorque(listID[activeNumInList],1);Torque_List[activeNumInList] = true;break;
    // Position +
    case 5:
      if(modeRead[listID[activeNumInList]] == 0){
#ifdef CTRL_ST_SERVO
          st.WritePosEx(listID[activeNumInList], ServoDigitalRange - 1, activeServoSpeed, ServoInitACC);
#endif
#ifdef CTRL_SC_SERVO
          st.WritePosEx(listID[activeNumInList], ServoDigitalRange - MAX_MIN_OFFSET, activeServoSpeed, ServoInitACC);
#endif
      }


      else if(modeRead[listID[activeNumInList]] == 3){
#ifdef CTRL_ST_SERVO
        st.WritePosEx(listID[activeNumInList], 10000, activeServoSpeed, ServoInitACC);
#endif
#ifdef CTRL_SC_SERVO
          st.WritePos(listID[activeNumInList], 0, rangeCtrl(activeServoSpeed,200,999), 0);
#endif
      }
      break;
    // Position -
    case 6:
      if(modeRead[listID[activeNumInList]] == 0){
#ifdef CTRL_ST_SERVO
          st.WritePosEx(listID[activeNumInList], 0, activeServoSpeed, ServoInitACC);
#endif
#ifdef CTRL_SC_SERVO
          st.WritePosEx(listID[activeNumInList], MAX_MIN_OFFSET, activeServoSpeed, ServoInitACC);
#endif
      }

      else if(modeRead[listID[activeNumInList]] == 3){
#ifdef CTRL_ST_SERVO
          st.WritePosEx(listID[activeNumInList], -10000, activeServoSpeed, ServoInitACC);
#endif
#ifdef CTRL_SC_SERVO
          st.WritePos(listID[activeNumInList], 0, rangeCtrl(activeServoSpeed,200,999)+1024, 0);
#endif
      }
      break;
    // Speed +
    case 7:activeSpeed(100);break;
    // Speed -
    case 8:activeSpeed(-100);break;
    // ID to set +
    case 9:servotoSet += 1;if(servotoSet > 250){servotoSet = 0;}break;
    // ID to set -
    case 10:servotoSet -= 1;if(servotoSet < 0){servotoSet = 0;}break;
    // Set Middle position
    case 11:setMiddle(listID[activeNumInList]);break;
    // Set Servo Mode
    case 12:setMode(listID[activeNumInList], 0);break;
    // Set Motor mode
    case 13:setMode(listID[activeNumInList], 3);break;
    // Start Serial Forwarding
    case 14:SERIAL_FORWARDING = true;break;
    // End Serial Forwarding
    case 15:SERIAL_FORWARDING = false;break;
    // Set New ID
    case 16:setID(listID[activeNumInList], servotoSet);break;

    // Set role as Normal
    case 17:DEV_ROLE = 0;break;
    // Set Role as Leader
    case 18:DEV_ROLE = 1;break;
    // Set Role as Follower
    case 19:DEV_ROLE = 2;break;

    // Rainbow On
    case 20:RAINBOW_STATUS = 1;break;
    // Rainbow OFF
    case 21:RAINBOW_STATUS = 0;break;
  }
}


void handleRoot() {
 server.send(200, "text/html", index_html); //Send web page
}


void handleID() {
  if(!searchedStatus && searchFinished){
    String IDmessage = "ID:";
    for(int i = 0; i< searchNum; i++){
      IDmessage += String(listID[i]) + " ";
    }
    server.send(200, "text/plain", IDmessage);
  }
  else if(searchedStatus){
    String IDmessage = "Searching...";
    server.send(200, "text/plain", IDmessage);
  }
}


void handleSTS() {
  String stsValue = "Active ID:" + String(listID[activeNumInList]);
  if(voltageRead[listID[activeNumInList]] != -1){
    stsValue += "  Position:" + String(posRead[listID[activeNumInList]]);
    if(DEV_ROLE == 0){
      stsValue += "<p>Device Mode: Normal";
    }
    else if(DEV_ROLE == 1){
      stsValue += "<p>Device Mode: Leader";
    }
    else if(DEV_ROLE == 2){
      stsValue += "<p>Device Mode: Follower";
    }
    stsValue += "<p>Voltage:" + String(float(voltageRead[listID[activeNumInList]])/10);
    stsValue += "  Load:" + String(loadRead[listID[activeNumInList]]);
    stsValue += "<p>Speed:" + String(speedRead[listID[activeNumInList]]);

    stsValue += "  Temper:" + String(temperRead[listID[activeNumInList]]);
    stsValue += "<p>Speed Set:" + String(activeServoSpeed);
    stsValue += "<p>ID to Set:" + String(servotoSet);
    stsValue += "<p>Mode:";
    if(modeRead[listID[activeNumInList]] == 0){
      stsValue += "Servo Mode";
    }
    else if(modeRead[listID[activeNumInList]] == 3){
      stsValue += "Motor Mode";
    }

    if(Torque_List[activeNumInList]){
      stsValue += "<p>Torque On";
    }
    else{
      stsValue += "<p>Torque Off";
    }
  }
  else{
    stsValue += " FeedBack err";
  }
  server.send(200, "text/plain", stsValue); //Send ADC value only to client ajax request
}


void webCtrlServer(){
    server.on("/", handleRoot);
    server.on("/readID", handleID);
    server.on("/readSTS", handleSTS);

    server.on("/cmd", [](){
    int cmdT = server.arg(0).toInt();
    int cmdI = server.arg(1).toInt();
    int cmdA = server.arg(2).toInt();
    int cmdB = server.arg(3).toInt();

    switch(cmdT){
      case 0:activeID(cmdI);break;
      case 1:activeCtrl(cmdI);break;
      case 9:searchCmd = true;break;
    }
    server.send(204);
  });

  // Start server
  server.begin();
  Serial.println("Server Starts.");
}


void webServerSetup(){
  webCtrlServer();
}


void getMAC(){
  WiFi.mode(WIFI_AP_STA);
  MAC_ADDRESS = WiFi.macAddress();
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());
}


void getIP(){
  IP_ADDRESS = WiFi.localIP();
}


void setAP(){
  WiFi.softAP(AP_SSID, AP_PWD);
  IPAddress myIP = WiFi.softAPIP();
  IP_ADDRESS = myIP;
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  WIFI_MODE = 1;
}


void setSTA(){
  WIFI_MODE = 3;
  WiFi.begin(STA_SSID, STA_PWD);
}


void getWifiStatus(){
  if(WiFi.status() == WL_CONNECTED){
    WIFI_MODE = 2;
    getIP();
    WIFI_RSSI = WiFi.RSSI();
  }
  else if(WiFi.status() == WL_CONNECTION_LOST && DEFAULT_WIFI_MODE == 2){
    WIFI_MODE = 3;
    // WiFi.disconnect();
    WiFi.reconnect();
  }
}


void wifiInit(){
  DEV_ROLE  = DEFAULT_ROLE;
  WIFI_MODE = DEFAULT_WIFI_MODE;
  if(WIFI_MODE == 1){setAP();}
  else if(WIFI_MODE == 2){setSTA();}
}


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void OnDataRecv(const esp_now_recv_info * mac, const uint8_t *incomingData, int len) {
  if(DEV_ROLE == 2){
    memcpy(&myData, incomingData, sizeof(myData));
    myData.Spd_send = abs(myData.Spd_send);
    if(myData.Spd_send < 50){
      myData.Spd_send = 200;
    }
    st.WritePosEx(myData.ID_send, myData.POS_send, abs(myData.Spd_send), 0);

    Serial.print("Bytes received: ");
    Serial.println(len);
    Serial.print("POS: ");
    Serial.println(myData.POS_send);
    Serial.print("SPEED: ");
    Serial.println(abs(myData.Spd_send));
  }
}


void espNowInit(){
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  MAC_ADDRESS = WiFi.macAddress();
  Serial.print("MAC:");
  Serial.println(WiFi.macAddress());
}
