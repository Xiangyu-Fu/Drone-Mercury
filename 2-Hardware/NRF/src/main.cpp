#if 0
// SimpleTx - the master or the transmitter
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define CE_PIN 22
#define CSN_PIN 21

void send(void);
void ReadJoystickValue(void);

const byte slaveAddress[5] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9E};
const byte masterAddress[5] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9F};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

uint8_t dataToSend[32];
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

char dataReceived[32]; // this must match dataToSend in the TX
bool newData = false;

//====================
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

void setup()
{

    Serial.begin(115200);
    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate(RF24_2MBPS);
    radio.setRetries(3, 5); // delay, count
    // radio.openWritingPipe(slaveAddress);
    radio.setChannel(40);
    radio.openReadingPipe(1, masterAddress);
}

//====================

void loop()
{
    currentMillis = millis();
    ReadJoystickValue();
    if (currentMillis - prevMillis >= txIntervalMillis)
    {
        getData();
        showData();
        prevMillis = millis();
    }
    delay(100);
}

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

    dataToSend[0] = '$';
    dataToSend[1] = 'M';
    dataToSend[2] = '<';
    dataToSend[3] = 'A';
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
#endif

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

#if 1
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

#define uint unsigned int
#define uchar unsigned char

#define TX_ADDR_WITDH 5 // 发送地址宽度设置为5个字节
#define RX_ADDR_WITDH 5
#define TX_DATA_WITDH 32
#define RX_DATA_WITDH 32
#define R_REGISTER 0x00  // 读寄存器
#define W_REGISTER 0x20  // 写寄存器
#define R_RX_PLOAD 0x61  // 读RX FIFO有效数据，1-32字节，当读数据完成后，数据被清除，应用于接收模式
#define W_TX_PLOAD 0xA0  // 写TX FIFO有效数据，1-32字节，写操作从字节0开始，应用于发射模式
#define FLUSH_TX 0xE1    // 清除TX FIFO寄存器，应用于发射模式
#define FLUSH_RX 0xE2    // 清除RX FIFO寄存器，应用于接收模式
#define REUSE_TX_PL 0xE3 // 重新使用上一包有效数据，当CE为高过程中，数据包被不断的重新发射
#define NOP 0xFF         // 空操作，可以用来读状态寄存器
/******************************************************************
// nRF24L01寄存器地址
*******************************************************************/
#define CONFIG 0x00      // 配置寄存器
#define EN_AA 0x01       // “自动应答”功能寄存器
#define EN_RX_ADDR 0x02  // 接收通道使能寄存器
#define SETUP_AW 0x03    // 地址宽度设置寄存器
#define SETUP_RETR 0x04  // 自动重发设置寄存器
#define RF_CH 0x05       // 射频通道频率设置寄存器
#define RF_SETUP 0x06    // 射频设置寄存器
#define STATUS 0x07      // 状态寄存器
#define OBSERVE_TX 0x08  // 发送检测寄存器
#define CD 0x09          // 载波检测寄存器
#define RX_ADDR_P0 0x0A  // 数据通道0接收地址寄存器
#define RX_ADDR_P1 0x0B  // 数据通道1接收地址寄存器
#define RX_ADDR_P2 0x0C  // 数据通道2接收地址寄存器
#define RX_ADDR_P3 0x0D  // 数据通道3接收地址寄存器
#define RX_ADDR_P4 0x0E  // 数据通道4接收地址寄存器
#define RX_ADDR_P5 0x0F  // 数据通道5接收地址寄存器
#define TX_ADDR 0x10     // 发送地址寄存器
#define RX_PW_P0 0x11    // 数据通道0有效数据宽度设置寄存器
#define RX_PW_P1 0x12    // 数据通道1有效数据宽度设置寄存器
#define RX_PW_P2 0x13    // 数据通道2有效数据宽度设置寄存器
#define RX_PW_P3 0x14    // 数据通道3有效数据宽度设置寄存器
#define RX_PW_P4 0x15    // 数据通道4有效数据宽度设置寄存器
#define RX_PW_P5 0x16    // 数据通道5有效数据宽度设置寄存器
#define FIFO_STATUS 0x17 // FIFO状态寄存器
//*********************************************************************************
uchar sta;                  // 状态变量
#define RX_DR (sta & 0x40)  // 接收成功中断标志
#define TX_DS (sta & 0x20)  // 发射成功中断标志
#define MAX_RT (sta & 0x10) // 重发溢出中断标志
#define ADC_Mid 2100
int CE = 22;
int CSN = 21;
int IRQ = 4;
// int SCK = 13;
// int MOSI = 11;
// int MISO = 12;  这几个引脚是硬件固定的，无法更改
uchar TX_Addr[] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9E};
uchar RX_Addr[] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9E};
uchar TX_Buffer[] = {0xfe, 100, 0xfb, 0xf7, 0xef, 0xdf, 0xbf, 0x7f};
uchar RX_Buffer[RX_DATA_WITDH];
char flag = 0;
int data_x;
unsigned long df = 0;
unsigned long t = 0;
void nRF24L01_Init(void)
{
    delay(2);
    digitalWrite(CE, LOW);
    digitalWrite(CSN, HIGH);
    digitalWrite(SCK, LOW);
    //    digitalWrite(MOSI,LOW);
    //    digitalWrite(MISO,LOW);
    digitalWrite(IRQ, HIGH);
}

uchar SPI_RW(uchar byte)
{
    uchar i;
    for (i = 0; i < 8; i++)
    {
        if (byte & 0x80)
            digitalWrite(MOSI, HIGH);
        else
            digitalWrite(MOSI, LOW);
        byte <<= 1;
        digitalWrite(SCK, HIGH);
        if (digitalRead(MISO))
            //     Serial.println(10);
            byte |= 0x01;
        digitalWrite(SCK, LOW);
    }
    return byte;
}

uchar SPI_W_Reg(uchar reg, uchar value)
{
    uchar status;
    digitalWrite(CSN, LOW);
    status = SPI_RW(reg);
    SPI_RW(value);
    digitalWrite(CSN, HIGH);
    return status;
}

uchar SPI_R_byte(uchar reg)
{
    uchar status;
    digitalWrite(CSN, LOW);
    SPI_RW(reg);
    status = SPI_RW(0);
    digitalWrite(CSN, HIGH);
    return status;
}

uchar SPI_R_DBuffer(uchar reg, uchar *Dat_Buffer, uchar Dlen)
{
    uchar reg_value, i;
    digitalWrite(CSN, LOW);
    reg_value = SPI_RW(reg);
    for (i = 0; i < Dlen; i++)
    {
        Dat_Buffer[i] = SPI_RW(0);
    }
    digitalWrite(CSN, HIGH);
    return reg_value;
}

uchar SPI_W_DBuffer(uchar reg, uchar *TX_Dat_Buffer, uchar Dlen)
{
    uchar reg_value, i;
    digitalWrite(CSN, LOW);
    reg_value = SPI_RW(reg);
    for (i = 0; i < Dlen; i++)
    {
        SPI_RW(TX_Dat_Buffer[i]);
    }
    digitalWrite(CSN, HIGH);
    return reg_value;
}

void nRF24L01_Set_TX_Mode(uchar *TX_Data)
{
    digitalWrite(CE, LOW);                                          // 待机（写寄存器之前一定要进入待机模式或掉电模式）
    SPI_W_DBuffer(W_REGISTER + TX_ADDR, TX_Addr, TX_ADDR_WITDH);    /*写寄存器指令+接收节点地址+地址宽度*/
    SPI_W_DBuffer(W_REGISTER + RX_ADDR_P0, TX_Addr, TX_ADDR_WITDH); /*为了接收设备应答信号，接收通道0地址与发送地址相同*/
    SPI_W_DBuffer(W_TX_PLOAD, TX_Data, TX_DATA_WITDH);              /*写有效数据地址+有效数据+有效数据宽度*/
    SPI_W_Reg(W_REGISTER + EN_AA, 0x01);                            /*接收通道0自动应答*/
    SPI_W_Reg(W_REGISTER + EN_RX_ADDR, 0x01);                       /*使能接收通道0*/
    SPI_W_Reg(W_REGISTER + SETUP_RETR, 0x1a);                       /*自动重发延时250US+86US，重发10次*/
    SPI_W_Reg(W_REGISTER + RF_CH, 40);                              /*(2400)MHZ射频通道* 40是信道*/
    SPI_W_Reg(W_REGISTER + RF_SETUP, 0x0f);                         /*1Mbps速率,发射功率:0DBM,低噪声放大器增益*/
    SPI_W_Reg(W_REGISTER + CONFIG, 0x0e);                           /*发送模式,上电,16位CRC校验,CRC使能*/
    digitalWrite(CE, HIGH);
    delay(5);
}

uchar Check_Ack(void)
{
    sta = SPI_R_byte(R_REGISTER + STATUS);
    if (TX_DS || MAX_RT)
    {
        SPI_W_Reg(W_REGISTER + STATUS, 0xff);
        digitalWrite(CSN, LOW);
        SPI_RW(FLUSH_TX);
        digitalWrite(CSN, HIGH);
        return 0;
    }
    else
        return 1;
}

uchar Check_Rec(void)
{
    uchar status;
    sta = SPI_R_byte(R_REGISTER + STATUS);
    if (RX_DR)
    {
        digitalWrite(CE, LOW);
        SPI_R_DBuffer(R_RX_PLOAD, RX_Buffer, RX_DATA_WITDH);
        status = 1;
    }
    SPI_W_Reg(W_REGISTER + STATUS, 0xff);
    return status;
}

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

void nRF24L01_Set_RX_Mode(void)
{
    digitalWrite(CE, LOW); // 待机
    // SPI_W_DBuffer(W_REGISTER+TX_ADDR,TX_Addr,TX_ADDR_WITDH);
    SPI_W_DBuffer(W_REGISTER + RX_ADDR_P0, RX_Addr, RX_ADDR_WITDH);
    SPI_W_Reg(W_REGISTER + EN_AA, 0x01); // auot ack
    SPI_W_Reg(W_REGISTER + EN_RX_ADDR, 0x01);
    // SPI_W_Reg(W_REGISTER+SETUP_RETR,0x0a);
    SPI_W_Reg(W_REGISTER + RX_PW_P0, RX_DATA_WITDH);
    SPI_W_Reg(W_REGISTER + RF_CH, 40);
    SPI_W_Reg(W_REGISTER + RF_SETUP, 0x0f); // 0db,lna
    SPI_W_Reg(W_REGISTER + CONFIG, 0x0f);

    digitalWrite(CE, HIGH);
    delay(5);
}

uchar nRF24L01_RX_Data(void)
{
    //  uchar i,status;
    sta = SPI_R_byte(R_REGISTER + STATUS);
    if (RX_DR)
    {
        Serial.println("RX_DR");
        digitalWrite(CE, LOW);
        SPI_R_DBuffer(R_RX_PLOAD, RX_Buffer, RX_DATA_WITDH);
        SPI_W_Reg(W_REGISTER + STATUS, 0xff);
        digitalWrite(CSN, LOW);
        SPI_RW(FLUSH_RX);
        digitalWrite(CSN, HIGH);
        return 1;
    }
    else
        return 0;
}

void NRF24L01_print_reg(void)
{
    uint8_t buf[8];
    for (int i = 0; i < 29; i++)
    {
        SPI_R_DBuffer(R_REGISTER + i, buf, 1); // Read 5 Bytes data from NRF
        printf("Buffer %x:", i);
        printf("%x \n", buf[0]);
    }
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
    NRF24L01_print_reg();
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

            nRF24L01_Set_TX_Mode(&TX_Buffer[1]); // 发送数据
            nRF24L01_Set_RX_Mode();
            Serial.println("send data ...");
        }
        // if during 1s no data received, then send data
        if (millis() - lastTime > 5000)
        {
            // Serial.println("already 5s ...");
            lastTime = millis();
            sta = SPI_R_byte(R_REGISTER + STATUS);
            Serial.println(sta);
        }
    }
}
#endif

#if 0
// SimpleRx - the slave or the receiver
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

void getData(void);
void showData(void);

#define CE_PIN 22
#define CSN_PIN 21

const byte thisSlaveAddress[5] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9E};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[10]; // this must match dataToSend in the TX
bool newData = false;

//===========

void setup()
{

    Serial.begin(115200);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate(RF24_2MBPS);
    radio.openReadingPipe(1, thisSlaveAddress);
    radio.setChannel(40);
    radio.startListening();
}

//=============

void loop()
{
    getData();
    showData();
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

#endif

#if 0
// SimpleTx - the master or the transmitter
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 22
#define CSN_PIN 21

void updateMessage(void);
void send(void);

const byte slaveAddress[5] = {0x1A, 0x3B, 0x5C, 0x7D, 0x9E};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char dataToSend[10] = "Message 0";
char txNum = '0';

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second

void setup()
{

    Serial.begin(115200);

    Serial.println("SimpleTx Starting");

    radio.begin();
    radio.setDataRate(RF24_2MBPS);
    radio.setRetries(3, 5); // delay, count
    radio.setChannel(40);
    radio.openWritingPipe(slaveAddress);
}

//====================

void loop()
{
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis)
    {
        send();
        prevMillis = millis();
    }
}

//====================

void send()
{

    bool rslt;
    rslt = radio.write(&dataToSend, sizeof(dataToSend));
    // Always use sizeof() as it gives the size as the number of bytes.
    // For example if dataToSend was an int sizeof() would correctly return 2

    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    if (rslt)
    {
        Serial.println("  Acknowledge received");
        updateMessage();
    }
    else
    {
        Serial.println("  Tx failed");
    }
}

//================

void updateMessage()
{
    // so you can see that new data is being sent
    txNum += 1;
    if (txNum > '9')
    {
        txNum = '0';
    }
    dataToSend[8] = txNum;
}
#endif
