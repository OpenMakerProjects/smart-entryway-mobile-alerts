#include "../firmware/alerts.h"
#include <cassert>
#include <iostream>
int main(){
 Alerts a;a.armed=true;assert(!a.tick(0,true,true,true));assert(a.tick(100,true,true,true));assert(a.active);
 assert(!a.tick(200,true,true,true));a.tick(5100,true,false,true);assert(!a.active);
 assert(!a.tick(10000,true,true,true));assert(a.tick(10100,true,true,true));
 a.tick(10200,false,false,true);assert(!a.active&&!a.armed);
 a.armed=true;a.tick(11000,true,false,false);assert(!a.armed&&!a.active);
 a.armed=true;a.active=true;a.stop();assert(!a.armed&&!a.active);
 Alerts w;w.seen=true;w.lastEvent=0xfffffff0u;w.confirmations=1;assert(w.tick(12000,true,true,true));
 std::cout<<"confirmation, cooldown, five-second output, invalid/offline stop, explicit STOP and wrap passed\n";
}
