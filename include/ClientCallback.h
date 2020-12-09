#include <BLEDevice.h>

class ClientCallback : public BLEClientCallbacks
{
public:
    void onConnect(BLEClient *pclient);
    void onDisconnect(BLEClient *pclient);
};