#include "motor_drive.h"

/*************************************************************************
�� �� ����GetEncoderNumber
�������ܣ�����6623��C620/C610���صĻ�е�Ƕ�ֵ������ʽ������ֵ��
��    ע����е�Ƕ�ֵ��Χ��0~8191��0x1FFF��
*************************************************************************/
//int32_t Get_Encoder_Number(CanRxMsg* rx_message)
//{
//    int32_t encoder_temp;
//	encoder_temp = rx_message->Data[0]<<8 | rx_message->Data[1];
//	return encoder_temp;
//}

/************************************************************************
�� �� ����Get_Speed
�������ܣ�����C620/C610���ص�ת�٣���λ��r/min
��    ע��RM3508������ٱ�Ϊ1��19��M2006������ٱ�Ϊ1��36
*************************************************************************/
//int32_t Get_Speed(CanRxMsg* rx_message)
//{
//    int32_t speed_temp;
//	if(rx_message->Data[2] & 0x01<<7)
//    {	
//        speed_temp = (0xFFFF<<16 | rx_message->Data[2]<<8 | rx_message->Data[3]);
//    }
//    else
//        speed_temp = (rx_message->Data[2]<<8 | rx_message->Data[3]);//rpm
//	return speed_temp;
//}

/*************************************************************************
�� �� ����Encoder_Process
�������ܣ���ˢ�������ʽ��������RM3510�������ʽ���������ݴ������õ�ת��
��    ע��type:0--����ʽ��������1--����ʽ������
*************************************************************************/
void cEncoder::Encoder_Process(int32_t value,uint8_t type)
{
	static fp32 fpVeltCoff;
	this->siPreRawValue = this->siRawValue;
	this->siRawValue = value;
	this->siDiff = this->siRawValue - this->siPreRawValue;		
	
	if(this->siDiff < -this->siNumber/2)//���α������ķ���ֵ���̫��,��ʾ����ʽ������Ȧ�������˸ı������ʽ�������Ķ�ʱ���������������
	{
		(type == 0)? (this->siDiff += (this->siNumber+1)):(this->siDiff += 65536);//��ʱ��16λ������������Χ0-65535��65536����
	}
	else if(this->siDiff > this->siNumber/2)//���α������ķ���ֵ���̫��,��ʾ����ʽ������Ȧ�������˸ı������ʽ�������Ķ�ʱ���������������
	{
		(type == 0)? (this->siDiff -= (this->siNumber+1)):(this->siDiff -= 65536);
	}

	if(type == 0)
	{
	    fpVeltCoff = 60.0f / this->siGearRatio / this->siNumber / 0.001f;//0.001��ָ���β������1ms
	}
	else
	{
	    fpVeltCoff = 60.0f / this->siGearRatio / this->siNumber / 0.001f / 4.0f;//0.001��ָ���β������1ms,4��ָ��ʱ��������ģʽ��4��Ƶ
	}		
	this->fpSpeed = fpVeltCoff*this->siDiff;//��λ��r/min
	this->siSumValue += this->siDiff;//��¼��������������λ�ñջ���	   
}

/*************************************************************************
�� �� ����GetPosition
�������ܣ���ô��ı������ۼ�ֵ
��    ע���˺���ֻ���������360���Է���ֵ���ۼ�ֵ����û�з���ת�ٵȲ�������˽������ڴ���������ɵ������ת��ֵ�����
*************************************************************************/
void cEncoder::GetPosition(void)
{
	this->siDiff = this->siRawValue - this->siPreRawValue;
	if(this->siDiff > 300)
	{
		this->siDiff -= 360;
	}
	else if(this->siDiff < -300)
	{
		this->siDiff += 360;
	}
	this->siSumValue += this->siDiff;
	this->siPreRawValue = this->siRawValue;
}

//void can_send_data(CAN_TypeDef* CANx, uint32_t StdID, int16_t ssMotor1, int16_t ssMotor2, 
//															int16_t ssMotor3, int16_t ssMotor4)
//{
//	CanTxMsg tx_message;
//	
//	tx_message.StdId = StdID;
//	tx_message.IDE = CAN_Id_Standard;
//	tx_message.RTR = CAN_RTR_Data;
//	tx_message.DLC = 0x08;
//	tx_message.Data[0] = ssMotor1>>8; 	             	
//	tx_message.Data[1] = ssMotor1;
//	tx_message.Data[2] = ssMotor2>>8;               
//	tx_message.Data[3] = ssMotor2;        
//	
//	tx_message.Data[4] = ssMotor3>>8;               
//	tx_message.Data[5] = ssMotor3; 
//	tx_message.Data[6] = ssMotor4>>8;               
//	tx_message.Data[7] = ssMotor4; 
//	
//	CAN_Transmit(CANx, &tx_message);

//}