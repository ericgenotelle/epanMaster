#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

struct DataPacket
{
    uint8_t bidonId;
    uint8_t data1;
    uint8_t data2;
};

uint8_t clientAddress[] =
{
     // B8:D6:1A:47:68:34
    0xB8, 0xD6, 0x1A, 0x47, 0x68, 0x34
};

uint8_t clientAddress2[] =
{
     //70:B8:F6:5B:93:D4
    0x70, 0xB8, 0xF6, 0x5B, 0x93, 0xD4
};

uint8_t clientAddress3[] =
{
     //A0:85:E3:E3:4C:04
    //  A0:85:E3:E3:4C:04
    0xA0, 0x85, 0xE3, 0xE3, 0x4C, 0x04
};


uint8_t notes[8] = { 45,47, 48, 50, 52, 53, 55, 57 };
uint8_t leds[3] = { 1 ,2, 0};

void OnDataRecv(const uint8_t *recv_info,
                const uint8_t *incomingData,
                int len)
{
    DataPacket packet;
    static uint8_t compteur = 0;

    static uint8_t lastNote = 0;
    static uint8_t lastLed = 0;


    memcpy(&packet, incomingData, sizeof(packet));

    Serial.printf(
        "Recu de %02X:%02X:%02X:%02X:%02X:%02X -> %d %d %d \n",
        recv_info[0],
        recv_info[1],
        recv_info[2],
        recv_info[3],
        recv_info[4],
        recv_info[5],
        packet.bidonId,
        packet.data1,
        packet.data2);


//        packet.data1 = notes[lastNote];
        packet.data2 = leds[lastLed];
        
        lastLed++;
        if (lastLed>=3) {
            lastLed = 0;
            // lastNote++;
            // if (lastNote>=8) {
            //     lastNote = 0;
            // }
        }

        esp_now_send(recv_info,
                (uint8_t *)&packet,
                sizeof(packet));

 }

void setup()
{
    Serial.begin(115200);

    // WiFi.mode(WIFI_STA);
     WiFi.mode(WIFI_AP_STA);

    Serial.print("MAC : ");
    Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Erreur ESP-NOW");
        return;
    }

    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, clientAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    esp_now_add_peer(&peerInfo);

    esp_now_peer_info_t peerInfo2 = {};
    memcpy(peerInfo2.peer_addr, clientAddress2, 6);
    peerInfo2.channel = 0;
    peerInfo2.encrypt = false;
    esp_now_add_peer(&peerInfo2);

    esp_now_peer_info_t peerInfo3 = {};
    memcpy(peerInfo3.peer_addr, clientAddress3, 6);
    peerInfo3.channel = 0;
    peerInfo3.encrypt = false;
    esp_now_add_peer(&peerInfo3);


    esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
}