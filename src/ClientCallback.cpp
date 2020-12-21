#include <ClientCallback.h>
#include <Arduino.h>

void ClientCallback::onConnect(BLEClient *pclient)
{
    Serial.println("Client connected \n");
}

 void ClientCallback::onDisconnect(BLEClient *pclient) 
 {
     Serial.println("Client disconnected \n");
 };