#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <WiFiUdp.h>
#include <WakeOnLan.h>

#include "config.h"

WiFiUDP UDP;
WakeOnLan WOL(UDP);

const int ip_count = sizeof(ip_list) / sizeof(ip_list[0]);
int ping_fails[ip_count];

void setup()
{
    Serial.begin(115200);
    Serial.println("esp32-wol-watchdog starting...");
}

void checkAndConnectWifi(){
    // check if WiFi is connected
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        WiFi.begin(wifi_ssid, wifi_password);
        Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.print("Connected to WiFi, IP Address: ");
        Serial.println(WiFi.localIP());
        WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());
        Serial.println("Starting continuous ping test...");
    }
}

void checkAndWakeDevices() {
    // for each IP in the list, check if it responds to ping 
    // and if it does not 3 times in a row, send a magic packet to wake up the device
    for (int i = 0; i < ip_count; i++) {
        Serial.print("Pinging ");
        Serial.print(ip_list[i]);
        Serial.print("... ");

        if (Ping.ping(ip_list[i], 1)) {
            Serial.println("online");
            ping_fails[i] = 0; // reset fail count
        } else {
            ping_fails[i]++;
            Serial.println("offline (" + String(ping_fails[i]) + " / " + String(ping_retry_count) + ")");
            if (ping_fails[i] >= ping_retry_count) {
                ping_fails[i] = 0; // reset fail count
                Serial.print("Sending magic packet to ");
                Serial.println(mac_list[i]);
                WOL.sendMagicPacket(mac_list[i]);
            }
        }
    }
}

void sleepForSeconds(int seconds) {
    // sleep for const wait_time_sec
    Serial.print("Waiting for ");
    Serial.print(seconds);
    Serial.print(" seconds");

    for (int i = 0; i < seconds; i++) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
}

void loop(){
    // check if WiFi is connected
    checkAndConnectWifi();
    // wait for const wait_time_sec
    sleepForSeconds(wait_time_sec);
    // perform ping check and send wol if needed
    checkAndWakeDevices();
}

