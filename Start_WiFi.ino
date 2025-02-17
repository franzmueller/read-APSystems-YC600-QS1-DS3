// ************************************************************************************
// *                        START wifi
// ************************************************************************************
void start_wifi() {
 WiFi.softAPdisconnect(true);
 WiFi.mode(WIFI_STA);
 WiFi.hostname("ECU");

 WiFi.mode(WIFI_STA); // geen ap op dit moment 

// we gaan 10 pogingen doen om te verbinden
// met de laatst gebruikte credentials
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print("*");
     event+=1;
     if (event==30) {break;}
  }
// als het verbinden is mislukt gaan we naar het configportal  
  if (event>29) {
     event=0;
     Serial.print("start portal...");  //Serial.println(event);
     start_portal(); // stel het portal in
     }
Serial.println(F("# connection attempts = "));  //Serial.println(event);
event=0; // we kunnen door naar de rest
checkFixed();
  
  start_asyserver();
}
//// *************************************************************************
////                      SERVER STARTEN


 // ********************************************************************
//             check if there must come a static ip
// ********************************************************************
void checkFixed() {
  // we comer here only when wifi connected
    char GATE_WAY[16]="";
    IPAddress gat=WiFi.gatewayIP();
    sprintf(GATE_WAY, "%d.%d.%d.%d", gat[0], gat[1], gat[2], gat[3]);
    Serial.print("GATE_WAY in checkFixed = nu: "); Serial.println(String(GATE_WAY));
    Serial.print("static_ip in checkFixed = nu: "); Serial.println(String(static_ip));

    if (static_ip[0] != '\0' && static_ip[0] != '0') {
      Serial.println("we moeten een static ip hebben Custom STA IP/GW/Subnet");
      IPAddress _ip,_gw,_sn(255,255,255,0); // declaratie 
      _ip.fromString(static_ip);
      _gw.fromString(GATE_WAY);//  if (ssid != "") {
      //WiFi.config(_ip, _gw, _sn);
      Serial.println(WiFi.localIP());
  } else {
      Serial.println("trying to get rid of wificonfig");
      //WiFi.config(0u, 0u, 0u);     
  }
}

void loginBoth(AsyncWebServerRequest *request, String who) {
  //String authFailResponse = "<h2>login failed <a href='/'>click here</a></h2>";
  if (who == "admin" ){
  const char* www_realm = "login as administrator."; 
    if (!request->authenticate("admin", pswd)) return request->requestAuthentication();
  }
  if (who == "both" ){
  const char* www_realm = "login as administrator or user."; 
    if (!request->authenticate("admin", pswd) && !request->authenticate("user", userPwd)) return request->requestAuthentication();
  }
}
