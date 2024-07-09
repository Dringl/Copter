#include "Fire_Gimbal.h"
Gimbal_str Gimbal;

void Fire_Gimbal::inc_multiple()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x20, 0x00, 0x00, 0x21};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::red_multiple()
{ 
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x40, 0x00, 0x00, 0x41};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::inc_zoom()
{ 
    uint8_t data_to_send[7] = {0xff, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::red_zoom()
{ 
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x80, 0x00, 0x00, 0x81};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::inc_aperture()
{ 
    uint8_t data_to_send[7] = {0xff, 0x01, 0x02, 0x00, 0x00, 0x00, 0x03};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::red_aperture()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x04, 0x00, 0x00, 0x00, 0x05};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::stop()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::open_LED()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x09, 0x00, 0x02, 0x0c};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::close_LED()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x0b, 0x00, 0x02, 0x0e};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::open_windscreen_wiper()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x09, 0x00, 0x01, 0x0b};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::close_windscreen_wiper()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x0b, 0x00, 0x01, 0x0d};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::turn_to_save_loc()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x07, 0x00, 0x01, 0x09};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::save_loc()
{
    uint8_t data_to_send[7] = {0xff, 0x01, 0x00, 0x03, 0x00, 0x01, 0x05};
    uint8_t cnt = 7;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::control_pitch(int16_t pitch)
{

    uint8_t data_to_send[7] = {0xFF, 0x01, 0x00, 0x4D, 0x00, 0x00, 0x00};
    uint8_t cnt = 7;
    uint16_t output_angle = 0;
    uint8_t sum = 0;
    (pitch >= 0)?(output_angle=360-pitch):(output_angle = -pitch);
    data_to_send[4] = BYTE1(output_angle);
    data_to_send[5] = BYTE0(output_angle);
    for (uint8_t i = 0; i < 6; i++)
    {
        sum += data_to_send[i];
        /* code */
    }
    data_to_send[6] = sum;
    hal.serial(4)->write(data_to_send, cnt);
}

void Fire_Gimbal::control_roll(int16_t roll)
{
    uint8_t data_to_send[7] = {0xFF, 0x01, 0x00, 0x4B, 0x00, 0x00, 0x00};
    uint8_t cnt = 7;
    uint16_t output_angle = roll;
    uint8_t sum = 0;
    data_to_send[4] = BYTE1(output_angle);
    data_to_send[5] = BYTE0(output_angle);
    for (uint8_t i = 0; i < 6; i++)
    {
        sum += data_to_send[i];
        /* code */
    }
    data_to_send[6] = sum;
    hal.serial(4)->write(data_to_send, cnt);
}
void Fire_Gimbal::Data_Receive_Anl_Task(uint8_t *data_buf, uint16_t num)
{
    uint16_t temp;
    switch (data_buf[3])
    {
    case 0x59 /* constant-expression */:
        /* code */
        Gimbal.roll = (data_buf[4] << 8) + data_buf[5];
        break;
    case 0x5B:
        temp = (data_buf[4] << 8) + data_buf[5];
        (temp >= 180)?(Gimbal.pitch = 360 -temp):(Gimbal.pitch = - temp);
    default:
        break;
    }
    
}
void Fire_Gimbal::Data_Receive_Prepare()
{
    uint8_t num = hal.serial(4)->available(); // 读取串口有多少个数据
    // gcs().send_text(MAV_SEVERITY_CRITICAL, "num:%d", num);
    uint8_t c;
    uint8_t sum = 0;
    static uint8_t stat = 0, rece_len = 0, data_len = 0;
    static uint8_t data_buff[255];
    while (num > 0)
    {
        c = hal.serial(4)->read();
        num--;
        if (stat == 0)
        {
            if (c == 0xFF) // 如果等于发射器发送的帧头0xff
            {
                data_buff[rece_len++] = c;
                stat++;
            }
        }
        else if (stat == 1) // 接收功能码
        {
            if (c == 0x01)
            {
                data_buff[rece_len++] = c;
                stat++;
                data_len = 5;
                /* code */
            }
            else
            {
                rece_len = 0;
                stat = 0;
            }
        }
        else if (stat == 2 && data_len > 0) // 记录寄存器地址
        {
            data_len--;
            data_buff[rece_len++] = c; // 直接接收起始地址与寄存器数量
            if (data_len == 0)
            {
                stat++;
            }
        }
        else if (stat == 3) // 根据上面的功能码进行接下来的数据接收
        {
            data_buff[rece_len++] = c; // 这里已经接收完一个，故下面的需要减掉1
            stat++;
            for (uint8_t i = 0; i < 6; i++)
            {
                sum += data_buff[i];
                /* code */
            }
            if (sum == data_buff[6])
            {
                Data_Receive_Anl_Task(data_buff, rece_len);
                /* code */
            }  
        }
        else
        {
            rece_len = 0;
            stat = 0;
        }
    }
}

void Fire_Gimbal::control_by_RC()
{
    static uint8_t i = 0;
    static float pit = 0;
    static float roll = 0;
    if (abs(Rc_In[5] - 1500) > 100 )
    {
        pit += (Rc_In[5] - 1500)*0.005f;
    }

    if (abs(Rc_In[4] - 1500) > 100)
    {
        roll += (Rc_In[4] - 1500) * 0.005f;
    }

    if (abs(Rc_In[9] - 1500) > 100)
    {
        ((Rc_In[9] - 1500) > 0) ? inc_multiple() : red_multiple();
    }

    if (abs(Rc_In[8] - 1500) > 100)
    {
        ((Rc_In[9] - 1500) > 0) ? inc_zoom() : red_zoom();
    }

    if ((abs(Rc_In[5] - 1500) < 100))
    {
        /* code */
    }
    
    // if( i == 0)
    control_pitch((int16_t)pit);
        // i++;
    // else
    control_roll((int16_t)roll);
        // i = 0;
    
    
}