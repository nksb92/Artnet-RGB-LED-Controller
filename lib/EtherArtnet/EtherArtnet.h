#if !defined(ETHER_ARTNET_H)
#define ETHER_ARTNET_H

#include <Arduino.h>
#include <ArtnetEther.h>
#include <SPI.h>

// https://github.com/khoih-prog/ESP32_Ethernet_Manager/blob/main/examples/ConfigOnDoubleReset/ConfigOnDoubleReset.ino
// consider rebuilding ArtnetEther lib to use DHCP and configureable SPI pins
// and support for W5500

// -------------------------------------------------------------
// ------------------------ETHERNET-----------------------------
// -------------------------------------------------------------
// Pins for the communication via Ethernet
// Type: W5500 with SPI
// Note: Using the standard SPI pins,
// therefore only CS is defined
#define ETH_CHIP_SELECT 1U
#define ETH_MOSI 9U
#define ETH_MISO 8U
#define ETH_SCK 7U
#define ETH_RST 16U

#define UNIVERSE_SIZE 512

#define PRINT_ETHERNET_INFO

enum RST_STATES
{
    ACTIVE = LOW,
    INACTIVE = HIGH,
};

enum IP_CHECK
{
    INVALID = 0,
    VALID,
};

enum CON_STATUS
{
    DISCONNECTED = 0,
    CONNECTED,
};

class EtherArtnet
{
private:
    // Data
    uint8_t mac[6];
    CON_STATUS connection_status = DISCONNECTED;
    CON_STATUS cable_connection = DISCONNECTED;

    ArtnetSender artnet = ArtnetSender();

    // String broadcast_ip = "192.168.0.255";
    IPAddress broadcast_ip = IPAddress(255, 255, 255, 255);
    IPAddress device_ip;
    IPAddress gateway_ip;

    // cooldown to not spam request to W5500 about cable connection
    // 5s seems to be ok for stable connection
    long unsigned last_cable_check = 0;
    uint16_t cable_check_time = 5000;

public:
    EtherArtnet(/* args */);
    ~EtherArtnet();
    void get_ips();
    void init();
    void re_init();
    void reset();
    void set_mac_adr();
    void connect();
    void validate_connection();
    void check_cable_status();
    void set_data(uint8_t *data);
    void stream_universe(uint8_t universe);
    CON_STATUS get_connection_status();
};

#endif // ETHER_ARTNET_H
