#if 0
// SimpleRx - the slave or the receiver
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

void getData(void);
void showData(void);

#define CE_PIN  22
#define CSN_PIN 21

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;

//===========

void setup() {

    Serial.begin(9600);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

//=============

void loop() {
    getData();
    showData();
}

//==============

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
        Serial.print("Data received ");
        Serial.println(dataReceived);
        newData = false;
    }
}
#endif

#include <Arduino.h>

String received_data;

void ReadData(void)
{
    if ( Serial.available() ) {
        received_data = Serial.readString();
    }
}
void setup() {
    Serial.begin(9600);
}

void loop() {
    ReadData();
    Serial.println(received_data);
    delay(200);
}
