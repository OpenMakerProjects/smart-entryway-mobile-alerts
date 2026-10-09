#pragma once
#include <stdint.h>
struct Alerts{
 bool armed=false,active=false,seen=false;uint8_t confirmations=0;uint32_t lastEvent=0,activeAt=0;
 bool tick(uint32_t now,bool valid,bool loud,bool online){
  if(!online||!valid){armed=false;active=false;}
  if(active&&uint32_t(now-activeAt)>=5000)active=false;
  confirmations=valid&&loud?(confirmations<2?confirmations+1:2):0;
  bool event=confirmations>=2&&(!seen||uint32_t(now-lastEvent)>=10000);
  if(event){seen=true;lastEvent=now;if(armed&&online){active=true;activeAt=now;}}
  return event;
 }
 void stop(){armed=false;active=false;}
};
