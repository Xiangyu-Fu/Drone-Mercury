#include "bsp_nrf.hpp"

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
    nrf_sta = SPI_R_byte(R_REGISTER + STATUS);
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
    nrf_sta = SPI_R_byte(R_REGISTER + STATUS);
    if (RX_DR)
    {
        digitalWrite(CE, LOW);
        SPI_R_DBuffer(R_RX_PLOAD, RX_Buffer, RX_DATA_WITDH);
        status = 1;
    }
    SPI_W_Reg(W_REGISTER + STATUS, 0xff);
    return status;
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
    nrf_sta = SPI_R_byte(R_REGISTER + STATUS);
    if (RX_DR)
    {
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
