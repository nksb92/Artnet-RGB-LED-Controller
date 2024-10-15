#include "EtherArtnet.h"

EtherArtnet ::EtherArtnet(/* args */)
{
}
EtherArtnet::~EtherArtnet()
{
}

void EtherArtnet::get_ips()
{
    device_ip = Ethernet.localIP();
    broadcast_ip = IPAddress(device_ip[0], device_ip[1], device_ip[2], 255);
    // broadcast_ip = IPAddress(255, 255, 255, 255);
    gateway_ip = Ethernet.gatewayIP();

#if defined(PRINT_ETHERNET_INFO)
    Serial.printf("Device IP: %s\n", device_ip.toString());
    Serial.printf("Broadcast IP: %s\n", broadcast_ip.toString());
    Serial.printf("Gateway IP: %s\n", gateway_ip.toString());
#endif // PRINT_ETHERNET_INFO
}

void EtherArtnet::init()
{
    // define Pins for Reset and set pin state high
    pinMode(ETH_RST, OUTPUT);

    re_init();
}

void EtherArtnet::re_init()
{
    reset();
    set_mac_adr();
    connection_status = DISCONNECTED;
}

void EtherArtnet::reset()
{
#if defined(PRINT_ETHERNET_INFO)

    Serial.printf("Reset of W5500\n");

#endif // PRINT_ETHERNET_INFO

    digitalWrite(ETH_RST, ACTIVE);
    // 500 us min Delay for reset (+10%)
    // delayMicroseconds(550);
    delay(10);
    digitalWrite(ETH_RST, INACTIVE);
}

void EtherArtnet::set_mac_adr()
{
    // Read MAC-Address of ESP-Device
    esp_read_mac(mac, ESP_MAC_ETH);

#if defined(PRINT_ETHERNET_INFO)

    Serial.print("Ethernet MAC: ");
    for (int i = 0; i < 5; i++)
    {
        Serial.printf("%02X:", mac[i]);
    }
    Serial.printf("%02X\n", mac[5]);

#endif // PRINT_ETHERNET_INFO

    // Set the MAC-Address of the Ethernet-Module
    // wait half a seccond to ensure that w5500 module has the registers set
    Ethernet.setMACAddress(mac);
    delay(500);
}

void EtherArtnet::connect()
{
    // Function has a timeout time,
    // therefore only call it when a cable is connected
    // Without a cable no connection to DHCP-Server
    if (cable_connection == CONNECTED)
    {
        // Start Connection and
        // Check if connection is established
        if (Ethernet.begin(mac) == 1)
        {
            validate_connection();
        }
    }
}

void EtherArtnet::validate_connection()
{
    IPAddress ip = Ethernet.localIP();

    if (ip.toString() != "0.0.0.0")
    {
        get_ips();

        // Start Artnet
        artnet.begin();

        // Update Status
        connection_status = CONNECTED;

#if defined(PRINT_ETHERNET_INFO)

        Serial.println("Connected");

#endif // PRINT_ETHERNET_INFO
    }
    else
    {
        connection_status = DISCONNECTED;
    }
}

void EtherArtnet::check_cable_status()
{
    // bool reset_required = false;

    // Checking if the cable is disconnected
    // react as fast as possible
    EthernetLinkStatus status = Ethernet.linkStatus();

#if defined(PRINT_ETHERNET_INFO)

#endif // PRINT_ETHERNET_INFO

    switch (status)
    {
    case LinkOFF:
        if (cable_connection == CONNECTED)
        {
            cable_connection = DISCONNECTED;
            connection_status = DISCONNECTED;
            // reset_required = true;

#if defined(PRINT_ETHERNET_INFO)

            Serial.println("Cable Disconnected");

#endif // PRINT_ETHERNET_INFO
        }

        break;
    case LinkON:
        if (cable_connection == DISCONNECTED)
        {
            cable_connection = CONNECTED;

#if defined(PRINT_ETHERNET_INFO)

            Serial.println("Cable Connected");

#endif // PRINT_ETHERNET_INFO
        }

        break;
    default:
        break;
    }

    if ((millis() - last_cable_check >= cable_check_time) && cable_connection)
    {
        Serial.printf("Checking connection ... \n");
        // ip-adr change without a call
        if (device_ip != Ethernet.localIP())
        {
            Serial.printf("IP Adress Change -> Current IP: %s; New IP: %s \n", device_ip.toString(), Ethernet.localIP().toString());

            connection_status = DISCONNECTED;

            re_init();
            // reset_required = true;
        }

        // if (reset_required)
        // {
        //     re_init();
        // }

        last_cable_check = millis();
    }
}

void EtherArtnet::set_data(uint8_t *data)
{

    // Serial.println("Setting Data");

    artnet.setArtDmxData(data, UNIVERSE_SIZE);
}

void EtherArtnet::stream_universe(uint8_t universe)

{
    // Serial.print("Sending Data");

    artnet.streamArtDmxTo(broadcast_ip.toString(), universe);

    // Serial.println(" ... Done");
}

CON_STATUS EtherArtnet::get_connection_status()
{
    return connection_status;
}
