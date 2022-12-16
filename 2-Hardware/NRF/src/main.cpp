// SimpleTx - the master or the transmitter
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 22
#define CSN_PIN 21

void send(void);
void ReadJoystickValue(void);

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

uint8_t dataToSend[9];
char txNum = '0';

/* Parameters for the time */
unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second

/* Parameters for the Joystick */
// Potentiometer is connected to GPIO 34
const int joy_left_x_Pin = 34;
const int joy_left_y_Pin = 35;

// variable for storing the potentiometer value
int16_t joy_left_x_value = 0;
int16_t joy_left_y_value = 0;

void setup()
{

    Serial.begin(115200);
    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.setRetries(3, 5); // delay, count
    radio.openWritingPipe(slaveAddress);
}

//====================

void loop()
{
    currentMillis = millis();
    ReadJoystickValue();
    if (currentMillis - prevMillis >= txIntervalMillis)
    {
        send();
        prevMillis = millis();
    }
    delay(100);
}

//====================

void send()
{

    bool rslt;
    rslt = radio.write(&dataToSend, sizeof(dataToSend));
    // Always use sizeof() as it gives the size as the number of bytes.
    // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    for (int i = 0; i < sizeof(dataToSend); i++)
    {
        Serial.print(dataToSend[i]);
        Serial.print(" ");
    }

    if (rslt)
    {
        Serial.println("  Acknowledge received");
    }
    else
    {
        Serial.println("  Tx failed");
    }
}

void ReadJoystickValue(void)
{
    // Reading potentiometer value
    joy_left_x_value = analogRead(joy_left_x_Pin);
    joy_left_y_value = analogRead(joy_left_y_Pin);

    dataToSend[5] = joy_left_x_value & 0xFF;
    dataToSend[6] = (joy_left_x_value >> 8) & 0xFF;
    dataToSend[7] = joy_left_y_value & 0xFF;
    dataToSend[8] = (joy_left_y_value >> 8) & 0xFF;

    // Print the value to serial monitor
    // Serial.print("Potentiometer value: ");
    // Serial.print(joy_left_x_value);
    // Serial.println(joy_left_x_value);

    // uint16_t test = 0;
    // test = dataToSend[5] | (dataToSend[6] << 8);
    // Serial.println(test);
}

#if 0
// SimpleRx - the slave or the receiver
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

void getData(void);
void showData(void);
void ReadJoystickValue(void);

#define CE_PIN 22
#define CSN_PIN 21

const byte thisSlaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;


//===========

void setup()
{

    Serial.begin(115200);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.startListening();
}

//=============

void loop()
{
    getData();
    showData();
    ReadJoystickValue();
    delay(50);
}

//==============

void getData()
{
    if (radio.available())
    {
        radio.read(&dataReceived, sizeof(dataReceived));
        newData = true;
    }
}

void showData()
{
    if (newData == true)
    {
        Serial.print("Data received ");
        Serial.println(dataReceived);
        newData = false;
    }
}

void ReadJoystickValue(void)
{
    // Reading potentiometer value
    potValue = analogRead(joy_left_x_Pin);
    potValue2 = analogRead(joy_left_y_Pin);

    // Print the value to serial monitor
    Serial.print("Potentiometer value: ");
    Serial.print(potValue);
    Serial.println(potValue2);
}
#endif

#if 0
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
// String joy_right_x;
// String joy_right_y;
const int ktest = 100;

// parameters for the Joystick
// Potentiometer is connected to GPIO 34
const int joy_left_x_Pin = 34;
const int joy_left_y_Pin = 35;

// variable for storing the potentiometer value
int16_t potValue = 0;
int16_t potValue2 = 0;

void ReadData(void)
{
    if (Serial.available())
    {
        received_data = Serial.readString();
    }
    else
    {
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

void ReadJoystickValue(void)
{
    // Reading potentiometer value
    potValue = analogRead(joy_left_x_Pin);
    potValue2 = analogRead(joy_left_y_Pin);

    // Print the value to serial monitor
    Serial.print("Potentiometer value: ");
    Serial.print(potValue);
    Serial.println(potValue2);
}

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    ReadData();
    ProcessData();
    ShowData();
    delay(50);
}

#endif