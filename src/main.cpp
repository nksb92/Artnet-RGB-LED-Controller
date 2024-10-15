#include "main.h"

#if !defined(ARDUINO_ESP32S3_DEV)

#error "Unknown Controller. Controller used to create Project: ESP32-S3 Devkit C1."

#endif // ARDUINO_ESP32S3_DEV

void setup()
{
    artnet_mutex = xSemaphoreCreateMutex();

    memset(data, 128, sizeof(data));
    Serial.begin(115200);
    Serial.println("Init Start");

    delay(2000);
    display.init();
    display.main_menu();

    delay(1000);
    display.clear_display();

    artnet.init();
    artnet.check_cable_status();
    Serial.println("Init Done");

    xTaskCreatePinnedToCore(
        loop_0,  // Function to implement the task
        "loop2", // Name of the task
        2000, // Stack size in bytes
        NULL,    // Task input parameter
        0,       // Priority of the task
        NULL,    // Task handle.
        0        // Core where the task should run
    );
}

// runs on core 1
void loop()
{
    loop_1();
}

// runs on core 0
void loop_0(void *parameters)
{
    Serial.println("Core 0 running.");
    while (1)
    {
        // take the mutex object if available
        // ensures that the method is not called by both cores at the same time
        if (xSemaphoreTake(artnet_mutex, portTICK_PERIOD_MS * 2))
        {
            switch (artnet.get_connection_status())
            {
            case CONNECTED:
                display.main_menu();
                break;

            case DISCONNECTED:
                // display.main_menu();
                break;

            default:
                break;
            }

            // release mutex object to release resource
            xSemaphoreGive(artnet_mutex);
        }
    }
}

void loop_1()
{
    artnet.check_cable_status();

    // take the mutex object if available
    // ensures that the method is not called by both cores at the same time
    if (xSemaphoreTake(artnet_mutex, portTICK_PERIOD_MS))
    {
        switch (artnet.get_connection_status())
        {
        case CONNECTED:
            if (millis() - last_update >= update_time)
            {
                hsv_value.hue++;
                temp_val = hsv_value;
                dmx_address = 0;
                for (int j = 0; j < 2; j++)
                {
                    for (int i = 0; i < segments; i++)
                    {
                        hsv2rgb_rainbow(temp_val, rgb_val);
                        data[dmx_address] = rgb_val.r;
                        data[dmx_address + 1] = rgb_val.g;
                        data[dmx_address + 2] = rgb_val.b;
                        temp_val.hue += STEP;
                        dmx_address += 3;
                    }
                    data[dmx_address] = 255;
                    dmx_address += 1;
                    temp_val.h += 127;
                }
                artnet.set_data(data);
                last_update = millis();
            }
            artnet.stream_universe(universe);
            break;
        case DISCONNECTED:

            artnet.connect();
            break;

        default:
            break;
        }

        // release mutex object to release resource
        xSemaphoreGive(artnet_mutex);
    }
}

// // Please include ArtnetEther.h to use Artnet on the platform
// // which can use both WiFi and Ethernet
// #include <ArtnetEther.h>
// #include "utility/w5100.h"
// // #include <ArtnetNativeEther.h>  // only for Teensy 4.1
// // this is also valid for other platforms which can use only Ethernet
// // #include <Artnet.h>

// // Ethernet stuff
// const IPAddress ip(192, 168, 0, 2);
// uint8_t mac[6]; // = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};

// ArtnetSender artnet;
// // Subenet Mask: 255.255.255.0
// const String target_ip = "192.168.0.255";

// const uint16_t size = 512;
// uint8_t data[size];
// uint8_t value = 0;
// uint8_t count = 0;

// unsigned long last_update = 0;
// uint16_t update_time = 25;
// int16_t factor = 1;
// IPAddress ip_;
// IPAddress dhcp_ip;

// void setup()
// {
//     memset(data, 0, sizeof(data));
//     Serial.begin(115200);
//     delay(5000);

//     esp_read_mac(mac, ESP_MAC_ETH);
//     Serial.print("Ethernet MAC: ");
//     for (int i = 0; i < 5; i++)
//     {
//         Serial.printf("%02X:", mac[i]);
//     }
//     Serial.printf("%02X\n", mac[5]);

//     // W5100.init();
//     Ethernet.setMACAddress(mac);

//     delay(1000);

//     // Ethernet.begin(mac, ip);
//     while (1)
//     {
//         if (Ethernet.begin(mac) == 1)
//         {
//             ip_ = Ethernet.localIP();
//             Serial.println(ip_.toString());
//             if (ip_.toString() != "0.0.0.0")
//             {
//                 break;
//             }
//         }

//         count++;
//         Serial.print("Count: ");
//         Serial.println(count);

//         if (count > 5)
//         {
//             Ethernet.begin(mac, ip);
//             break;
//         }
//     }

//     delay(1000);
//     // Ethernet.begin(mac, ip_);
//     artnet.begin();

//     Serial.println("start");
// }

// void loop()
// {
//     if (millis() - last_update >= update_time)
//     {
//         // value += factor;
//         // if (value >= 255 || value <= 0)
//         // {
//         //     factor *= -1;
//         // }
//         // memset(data, value, sizeof(data));
//         // Serial.println(value);
//         /* code */

//         hsv_value.hue++;
//         // put your main code here, to run repeatedly:
//         temp_val = hsv_value;
//         dmx_address = 0;
//         for (int i = 0; i < segments; i++)
//         {
//             hsv2rgb_rainbow(temp_val, rgb_val);
//             data[dmx_address] = rgb_val.r;
//             data[dmx_address + 1] = rgb_val.g;
//             data[dmx_address + 2] = rgb_val.b;
//             temp_val.hue += 10;
//             dmx_address += 3;
//         }
//         // Serial.println(dmx_address);
//         ip_ = Ethernet.localIP();
//         Serial.println(ip_.toString());
//         data[dmx_address] = 255;
//         // // artnet.streamArtDmxTo()

//         artnet.setArtDmxData(data, size);

//         last_update = millis();
//     }

//     artnet.streamArtDmxTo(target_ip, universe); // automatically send set data in 40fps
// }
