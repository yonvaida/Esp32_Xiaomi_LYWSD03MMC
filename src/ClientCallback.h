#include <BLEDevice.h>

class ClientCallback : public BLEClientCallbacks
{
public:
    ClientCallback(bool* t_reading){ m_reading = t_reading;};
    void onConnect(BLEClient *pclient);
    void onDisconnect(BLEClient *pclient);
    private:
    bool* m_reading;
};