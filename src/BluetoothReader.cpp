#include "BluetoothReader.h"
#include <Settings.h>

BluetoothReader::BluetoothReader(BLEUUID t_uid, std::string t_adress):
    m_client(BLEDevice::createClient())
{
    m_serviceUID = t_uid;
    m_clientAdress = std::move(t_adress);
}

void BluetoothReader::SetServiceUUID(BLEUUID t_uuid)
{
    m_serviceUID = t_uuid;
}

void BluetoothReader::SetAddress(std::string t_adress)
{
    m_clientAdress = t_adress;
};

bool BluetoothReader::Connect()
{
    if (m_client && m_clientAdress != "")
    {
        return m_client->connect(BLEAddress(m_clientAdress));
    }
    return false;
}

void BluetoothReader::SetCallback(BLEClientCallbacks *t_callback)
{
    m_client->setClientCallbacks(t_callback);
}

bool BluetoothReader::IsConnected()
{
    return m_client->isConnected();
}

void BluetoothReader::RegisterNotifications()
{
    const auto service = m_client->getService(m_serviceUID);

    if (service)
    {
        const auto characteristics = service->getCharacteristics();
        for (auto characteristicIt = characteristics->begin();
             characteristicIt != characteristics->end(); characteristicIt++)
        {
            if (characteristicIt->second->canNotify() && characteristicIt->second->canRead())
            {
                characteristicIt->second->registerForNotify(BluetoothReader::NofificationsCallback);
            }
        }
    }
}

void BluetoothReader::NofificationsCallback(BLERemoteCharacteristic *pBLERemoteCharacteristic,
                                            uint8_t *pData,
                                            size_t length,
                                            bool isNotify)
{
    float temp;
    float humi;
    temp = (pData[0] | (pData[1] << 8)) * 0.01;
    humi = pData[2];
    Serial.printf("temp = %.2f : humidity = %.2f \n", temp, humi);
    if(temp > Settings::GetInstance()->GetHighTemp())
    {
        digitalWrite(23,LOW);
    }
        if(temp < Settings::GetInstance()->GetLowTemp())
    {
        digitalWrite(23,HIGH);
    }
    Settings::GetInstance()->SetCurrTemp(temp);
    pBLERemoteCharacteristic->getRemoteService()->getClient()->disconnect();
}