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

using namespace std;


String received_data;
string received_data_string;
string output_data;
char *p_data;

String default_data = "100100000100100000";
String test_data = "100";

String joy_left_x;
String joy_left_y;
String joy_right_x;
String joy_right_y;
const int ktest= 100;


void ReadData(void)
{
    if ( Serial.available() ) {
        received_data = Serial.readString();
    }
    else {
        received_data = default_data;
    }
    delay(20);
}

void ProcessData(void)
{
    if (received_data.length() > 17)
    {
        joy_left_x = received_data.substring(0, 3);
        int left_x = joy_left_x.toInt();
        joy_left_y = received_data.substring(3, 6);
        int left_y = joy_left_y.toInt();
        joy_right_x = received_data.substring(6, 9);
        int right_x = joy_right_x.toInt();
        joy_right_y = received_data.substring(9, 12);
        int right_y = joy_right_y.toInt();
        delay(20);
    }
}

void ShowData(void)
{
    // Serial.print("Data received: \n");
    // Serial.println(received_data);
    // Serial.print("Joy Left X: \n");
    Serial.println(joy_left_x);
    Serial.println(joy_left_y);
    // Serial.println(joy_right_x);
    // Serial.println(joy_right_y);
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    ReadData();
    ProcessData();
    ShowData();
    delay(20);
}
