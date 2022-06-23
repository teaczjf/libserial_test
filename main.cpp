#include <stdio.h>
#include <iostream>
#include "my_easylogging.h"
#include "SerialPort.h"

#define BUFF_LENTH 8000
#define BUFF_LENTH_serial 66
#define TIMEOUT_MS 100 //

using namespace LibSerial;
using namespace std;

SerialPort *m_serial;

typedef struct modbus_rtu_param
{
    string com_name;
    UINT32 com_baud;
    UINT8 com_data_bit;
    UINT8 com_stop_bit;
    char com_parity;

    modbus_rtu_param()
    {
        com_baud = 115200;
        com_data_bit = 8;
        com_stop_bit = 1;
        com_parity = 'N';
    }
} modbus_rtu_param_t;

int main()
{
    easylogginginit(); //日志初始化
                       // log_test();        //日志自测函数   里面可以看细节的用法
    m_serial = new SerialPort();
    modbus_rtu_param_t m_modbus_param_rtu;
    DataBuffer serialbuffer;
    UINT8 tmpdatabuffer[100] = {0};
    string message;
    try
    {
        m_serial->Open("/dev/ttymxc2");

        m_serial->SetBaudRate(BaudRate::BAUD_9600);
        // Set the number of data bits.
        m_serial->SetCharacterSize(CharacterSize::CHAR_SIZE_8);
        // Set the hardware flow control.
        m_serial->SetFlowControl(FlowControl::FLOW_CONTROL_NONE);
        // Set the parity.
        m_serial->SetParity(Parity::PARITY_NONE);
        // Set the number of stop bits.
        m_serial->SetStopBits(StopBits::STOP_BITS_1);
    }
    catch (...)
    {
        LOG(ERROR) << "OdlxCarModbus::Init>>Serial open error!" << m_modbus_param_rtu.com_name.data();
    }

    while (1)
    {
        try
        {
            // LOG(ERROR) << "################################# start Read #####################";
            // 有两种取值的方式
            // m_serial->Read(serialbuffer, BUFF_LENTH_serial, TIMEOUT_MS); //读取66个  超时等待时间100ms

            m_serial->Read(message, BUFF_LENTH_serial, TIMEOUT_MS); //读取66个  超时等待时间100ms

            // LOG(ERROR) << "################################# END Read #####################";
        }
        catch (...)
        {
            //到这就已经是触发了超时100ms的异常了
            usleep(10 * 1000);
            LOG(ERROR) << "到这就已经是触发了超时100ms的异常了";
        }
        memset(tmpdatabuffer, 0, 100); //及时清空缓存

        for (int i = 0; i < serialbuffer.size(); i++) //单次循环取11个字节出来
        {
            tmpdatabuffer[i] = serialbuffer[i];
        }
        LogInfoDump("得到信息：：", tmpdatabuffer, serialbuffer.size());

        LOG(INFO) << "#########message == " << message;

        m_serial->Write("AT+1231");
        usleep(500 * 1000);
    }
    printf("Hello world\n\r");

    return 0;
}
