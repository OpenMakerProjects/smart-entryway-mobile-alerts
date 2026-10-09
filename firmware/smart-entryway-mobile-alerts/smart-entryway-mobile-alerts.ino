#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "../alerts.h"
#include "../config.h"
WiFiClient wifi;PubSubClient mqtt(wifi);Servo pointer;Alerts policy;
uint32_t sampled=0,reported=0,retry=0,seq=0;bool lastValid=false;
void command(char*,byte* bytes,unsigned length){
 if(length==3&&!memcmp(bytes,"ARM",3)&&lastValid&&mqtt.connected())policy.armed=true;
 if(length==4&&!memcmp(bytes,"STOP",4))policy.stop();
}
void setup(){
 pinMode(5,OUTPUT);digitalWrite(5,LOW);pointer.attach(4);pointer.write(0);Serial.begin(115200);
 mqtt.setServer(MQTT_HOST,MQTT_PORT);mqtt.setCallback(command);mqtt.setSocketTimeout(1);
 if(strlen(WIFI_SSID)){WiFi.mode(WIFI_STA);WiFi.begin(WIFI_SSID,WIFI_PASSWORD);}
}
void loop(){
 uint32_t now=millis();
 if(strlen(MQTT_HOST)&&WiFi.status()==WL_CONNECTED&&!mqtt.connected()&&uint32_t(now-retry)>=10000){
  retry=now;String client="entry12-"+String(ESP.getChipId(),HEX);
  if(mqtt.connect(client.c_str(),MQTT_USER,MQTT_PASSWORD,"entry12/availability",0,true,"offline")){
   mqtt.publish("entry12/availability","online",true);mqtt.subscribe("entry12/command");
  }
 }
 mqtt.loop();
 if(uint32_t(now-sampled)<100){delay(1);return;}sampled=now;
 int lo=1023,hi=0;bool valid=now>=2000;
 for(int i=0;i<64;i++){int v=analogRead(A0);lo=min(lo,v);hi=max(hi,v);if(v<2||v>1021)valid=false;delayMicroseconds(100);}
 lastValid=valid;bool event=policy.tick(now,valid,hi-lo>=180,mqtt.connected());
 digitalWrite(5,policy.active?HIGH:LOW);pointer.write(policy.active?90:0);
 if(event){char e[100];snprintf(e,sizeof(e),"{\"id\":12,\"seq\":%lu,\"sound_pp\":%d}",(unsigned long)++seq,hi-lo);Serial.println(e);if(mqtt.connected())mqtt.publish("entry12/event",e,false);}
 if(uint32_t(now-reported)>=1000){reported=now;char s[160];snprintf(s,sizeof(s),"{\"id\":12,\"sound_pp\":%d,\"valid\":%s,\"armed\":%s,\"relay\":%s,\"servo_deg\":%d}",hi-lo,valid?"true":"false",policy.armed?"true":"false",policy.active?"true":"false",policy.active?90:0);Serial.println(s);if(mqtt.connected())mqtt.publish("entry12/state",s,true);}
}
