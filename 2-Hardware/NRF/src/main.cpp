#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <bsp_nrf.hpp>

uchar nrf_sta;

int CE = 22;
int CSN = 21;
int IRQ = 4;
// int SCK = 13;
// int MOSI = 11;
// int MISO = 12;  这几个引脚是硬件固定的，无法更改

// declare address and transmited data
uchar TX_Addr[] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9E};
uchar RX_Addr[] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9E};
uchar TX_Buffer[32];
uchar RX_Buffer[RX_DATA_WITDH];
char flag = 0;
int data_x;
unsigned long df = 0;
unsigned long t = 0;

/* Parameters for the Joystick */
// Potentiometer is connected to GPIO 34
const int joy_left_x_Pin = 34;
const int joy_left_y_Pin = 35;

// variable for storing the potentiometer value
int16_t joy_left_x_value = 0;
int16_t joy_left_y_value = 0;

void NRF24L01_Check(void)
{
    uint8_t buf[5];
    uint8_t i;

    SPI_W_DBuffer(W_REGISTER + TX_ADDR, TX_Addr, 5); // Write 5 Bytes data to NRF
    SPI_R_DBuffer(R_REGISTER + TX_ADDR, buf, 5);     // Read 5 Bytes data from NRF

    // Compare sent and received data
    for (i = 0; i < 5; i++)
    {
        if (buf[i] != TX_Addr[i])
        {
            break;
        }
    }
    delay(5);
    if (i == 5)
        Serial.println("NRF24L01 	Initialisation Successful ...");
    else
        Serial.println("WARNING:  	NRF24L01 Initialisation Failed !");
}

void ReadJoystickValue(void)
{
    // Reading potentiometer value
    joy_left_x_value = analogRead(joy_left_x_Pin);
    joy_left_y_value = analogRead(joy_left_y_Pin);

    TX_Buffer[0] = '$';
    TX_Buffer[1] = 'M';
    TX_Buffer[2] = '<';
    TX_Buffer[3] = 'A';
    TX_Buffer[5] = joy_left_x_value & 0xFF;
    TX_Buffer[6] = (joy_left_x_value >> 8) & 0xFF;
    TX_Buffer[7] = joy_left_y_value & 0xFF;
    TX_Buffer[8] = (joy_left_y_value >> 8) & 0xFF;

    // Print the value to serial monitor
    // Serial.print("Potentiometer value: ");
    // Serial.print(joy_left_x_value);
    // Serial.println(joy_left_x_value);

    // uint16_t test = 0;
    // test = dataToSend[5] | (dataToSend[6] << 8);
    // Serial.println(test);
}

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(CE, OUTPUT);
    pinMode(CSN, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);

    NRF24L01_Check();
    // // check the register value of NRF24L01
    // NRF24L01_print_reg();
    nRF24L01_Init();
}

void loop()
{
    // put your main code here, to run repeatedly:
    // Serial.println(10);
    unsigned long lastTime = millis();
    delay(1);
    while (1)
    {
        nRF24L01_Set_RX_Mode();
        // delay(4);
        // delayMicroseconds(4);
        if (nRF24L01_RX_Data()) // 当接收到数据时
        {
            for (int i = 0; i < RX_DATA_WITDH; i++)
            {
                Serial.print(RX_Buffer[i]);
                Serial.println(" ");
            }
            ReadJoystickValue();                 // read joystick value
            nRF24L01_Set_TX_Mode(&TX_Buffer[0]); // transmit data
            nRF24L01_Set_RX_Mode();              // change to transmit mode
            Serial.println("send data ...");
            nRF24L01_RX_Data(); // clear RX_DR or TX_DS or MAX_RT interrupt flag
            lastTime = millis();
        }

        // if during 1s no data received, then send data
        if (millis() - lastTime > 5000)
        {
            // Serial.println("already 5s ...");
            lastTime = millis();
            uchar sta_op = SPI_R_byte(R_REGISTER + STATUS);
            if (sta_op == 0x0e)
                Serial.println("Have not received information for 5s ...");
        }
        delayMicroseconds(100);
    }
}
