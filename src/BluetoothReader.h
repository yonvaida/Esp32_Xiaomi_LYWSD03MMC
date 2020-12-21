#include <Arduino.h>
#include <BLEDevice.h>
#include <memory>

class BluetoothReader
{
public:
    BluetoothReader(BLEUUID, std::string);
    void SetServiceUUID(BLEUUID);
    void SetAddress(std::string);
    bool Connect();
    void SetCallback(BLEClientCallbacks *);
    bool IsConnected();
    void RegisterNotifications();

private:
    BLEUUID m_serviceUID = {};
    std::unique_ptr<BLEClient> m_client = {};
    std::string m_clientAdress;
    static void NofificationsCallback(BLERemoteCharacteristic *,
                                      uint8_t *,
                                      size_t,
                                      bool);
};