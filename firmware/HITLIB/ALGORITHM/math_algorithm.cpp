/*******************************************************************
版权声明：HITCRT(哈工大竞技机器人队)
文件名：Common_Algorithm.c
最近修改日期：2015.11.6
版本：第一版
作者：李禄平
**********************************************************************/

#include "math_algorithm.h"

/*******************************************************************
函数名称：Round()
函数功能：将浮点数四舍五入，返回32位整型数
输入：    fpValue
输出：    四舍五入后返回的整型数
备注：
********************************************************************/
int32_t Round(fp32 fpValue)
{
    if (fpValue >= 0)
    {
        return (int32_t)(fpValue + 0.5f);
    }
    else
    {
        return (int32_t)(fpValue - 0.5f);
    }
}
/*******************************************************************
函数名称：CalRadialProjection()
函数功能：计算一向量在基准向量方向投影
输入：    stAim:目标向量，即需要投影的向量
		  stBase:基准向量
输出：    目标向量在基准向量上的投影
备注：    当目标点在基准直线之间时，投影值为正，反之为负
********************************************************************/
fp32 CalRadialProjection(ST_VECTOR stAim, ST_VECTOR stBase)
{
    return (fp32)(stBase.fpVx * stAim.fpVx + stBase.fpVy * stAim.fpVy)/sqrtf((fp32)(stBase.fpVy * stBase.fpVy + stBase.fpVx * stBase.fpVx)) ;
}

/*******************************************************************
函数名称：CalNormalProjection()
函数功能：计算一向量在基准向量法向方向投影
输入：    stAim:目标向量，即需要投影的向量
		  stBase:基准向量
输出：    目标向量在基准向量法向方向上的投影
备注：    基准向量的法向向量为基准向量顺时针旋转90°得到
		  当目标点在基准直线左侧时（以基准向量方向为正方向），投影值为正，反之为负
********************************************************************/
fp32 CalNormalProjection(ST_VECTOR stAim,ST_VECTOR stBase)
{
    return (fp32)(stBase.fpVy * stAim.fpVx - stBase.fpVx * stAim.fpVy)/sqrtf((fp32)(stBase.fpVy * stBase.fpVy + stBase.fpVx * stBase.fpVx));
}
/*******************************************************************
函数名称：CalAngle()
函数功能：计算一向量与ssPosY轴正半轴夹角
输入：    stAim:目标向量
输出：    目标向量与ssPosY轴正半轴夹角(RADIAN)
备注：    逆时针为正，顺时针为负，计算夹角范围为：[-PI,PI)
********************************************************************/
fp32 CalAngle(ST_VECTOR stAim)
{
    fp32 fpAngAim;

    if(stAim.fpVy == 0)//分母为0
    {
        if(stAim.fpVx > 0)
        {
            return -PI_2;
        }
        else if(stAim.fpVx < 0)
        {
            return PI_2;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(stAim.fpVx == 0)
        {
            if(stAim.fpVy > 0)
            {
                return 0;
            }
            else if(stAim.fpVy < 0)
            {
                return -PI;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            fpAngAim = -atanf((fp32)(stAim.fpVx / (fp32)stAim.fpVy));
            if (stAim.fpVy < 0)//3、4象限
            {
                if (fpAngAim >= 0)//4象限
                {
                    fpAngAim -= PI;
                }
                else if (fpAngAim < 0)//3
                {
                    fpAngAim += PI;
                }
            }
            return fpAngAim;
        }
    }
}
/*******************************************************************
函数名称：ConvertAngle()
函数功能：将角度转换为全局坐标系的航向角范围[-PI,PI)
输入：    ang：目标角度(RADIAN)
输出：    转换后的角度(RADIAN)
备注：    逆时针为正，顺时针为负，不适合对角度值较大的值做转换
********************************************************************/
fp32 ConvertAngle(fp32 fpAngA)
{
    do
    {
        if (fpAngA >= PI)
        {
            fpAngA -= PI2;
        }
        else if (fpAngA < -PI)
        {
            fpAngA += PI2;
        }
    } while (fpAngA >= PI || fpAngA < -PI);
    return fpAngA;
}
/*******************************************************************
函数名称：ConvertDeg()
函数功能：将角度转换为全局坐标系的航向角范围[-1800,1800)(单位：0.1°)
输入：    ang：目标角度(单位：0.1°)
输出：    转换后的角度(单位：0.1°)
备注：    逆时针为正，顺时针为负，不适合对角度值较大的值做转换
********************************************************************/
int16_t ConvertDeg(int16_t fpDegA)
{
    do
    {
        if (fpDegA >= 1800)
        {
            fpDegA -= 3600;
        }
        else if (fpDegA < -1800)
        {
            fpDegA += 3600;
        }
    } while (fpDegA >= 1800 || fpDegA < -1800);
    return fpDegA;
}
/*******************************************************************
函数名称：Clip()
函数功能：削波函数，去除超出最大值与最小值之间的值，代之以最大或最小值
输入：    siValue:实际值
		  siMin:下限值
		  siMax:上限值
输出：    siValue：削波后的值
备注：	  适用于整型变量的消波
********************************************************************/
int32_t Clip(int32_t siValue, int32_t siMin, int32_t siMax)
{
    if(siValue < siMin)
    {
        return siMin;
    }
    else if(siValue > siMax)
    {
        return siMax;
    }
    else
    {
        return siValue;
    }
}

/*******************************************************************
函数名称：ClipChar()
函数功能：削波函数，去除超出最大值与最小值之间的值，代之以最大或最小值
输入：    ucValue:实际值
		  ucMin:下限值
		  ucMax:上限值
输出：    ucValue：削波后的值
备注：	  适用于无符号字符型削波
********************************************************************/
uint8_t ClipChar(uint8_t ucValue, uint8_t ucMin, uint8_t ucMax)
{
    if(ucValue < ucMin)
    {
        return ucMin;
    }
    else if(ucValue > ucMax)
    {
        return ucMax;
    }
    else
    {
        return ucValue;
    }
}

/*******************************************************************
函数名称：ClipFloat()
函数功能：削波函数，去除超出最大值与最小值之间的值，代之以最大或最小值
输入：    fpValue:实际值
		  fpMin:下限值
		  fpMax:上限值
输出：    fpValue：削波后的值
备注：	  适用于浮点数变量的消波
********************************************************************/
fp32 ClipFloat(fp32 fpValue, fp32 fpMin, fp32 fpMax)
{
    if(fpValue < fpMin)
    {
        return fpMin;
    }
    else if(fpValue > fpMax)
    {
        return fpMax;
    }
    else
    {
        return fpValue;
    }
}
/*******************************************************************
函数名称：Fabs()
函数功能：计算绝对值函数
输入：    fpNum: 实际值
输出：    fpNum：削波后的值
备注：
********************************************************************/
fp32 Fabs(fp32 fpNum)
{
    if(fpNum < 0)
    {
        return -fpNum;
    }

    return fpNum;
}

// CRC高位字节值表
const uint8_t aucCRCHighTable[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};
// CRC低位字节值表
const uint8_t aucCRCLowTable[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};
/***********************************************************************************
函数名称：crc16()
函数功能：对固定长度的数组生成CRC校验码
入口参数：要进行校验的数组的首地址，数组的长度
返回值：  校验的结果为16位 uint16_t(无符号16位整型变量)
************************************************************************************/
uint16_t crc16(uint8_t *pucData,uint8_t ucLength)
{
    uint8_t ucCRCHi = 0xFF; // 高CRC字节初始化
    uint8_t ucCRCLo = 0xFF; // 低CRC字节初始化
    uint32_t uiIndex;            // CRC循环中的索引

    while (ucLength--)
    {
        // 计算CRC
        uiIndex = ucCRCLo ^ *pucData++ ;
        ucCRCLo = ucCRCHi ^ aucCRCHighTable[uiIndex];
        ucCRCHi = aucCRCLowTable[uiIndex] ;
    }
    return ((ucCRCHi << 8) | ucCRCLo) ;
}

/****************************************************************************************************
函数名称：us_delay()
函数功能：延时us
入口参数：需要延时的时间
****************************************************************************************************/
void us_delay(uint32_t uiTime)
{
    uint32_t uiDelay;
    while(uiTime -- )
    {
        uiDelay = 25;
        while(uiDelay -- );
    }
}
/***********************************************************************************
函数名称：sum8()
函数功能：对固定长度的数组生成SUM校验码
入口参数：要进行校验的数组的首地址，数组的长度
返回值：  校验的结果为8位 uint16_t(无符号8位字符型)
************************************************************************************/
uint8_t sum8(uint8_t *data,uint8_t length)
{
    uint8_t chSUM=0x00;
    while(length--)
        //计算sum
    {
        chSUM += *data ;
        data++;
    }
    return (chSUM);
}


/*-------------------------------------------------------------------------------------------------
函 数 名：tangentdot
函数功能：求过圆外一点的直线与圆的切点
备    注：ptCenter圆心坐标，ptOutside圆外点，Radious圆半径，mode模式，返回切点坐标
-------------------------------------------------------------------------------------------------*/
stPoint tangentdot(stPoint ptCenter, stPoint ptOutside, fp32 Radious, int mode)
{ 
	 stPoint E,F,G,H;
	 fp32 r=Radious;
	 fp32 a;
	
	 E.x= ptOutside.x-ptCenter.x;
	 E.y= ptOutside.y-ptCenter.y; 
	 

	 fp32 t= r / sqrt (E.x * E.x + E.y * E.y);  
	 F.x= E.x * t;   F.y= E.y * t;   //计算结果与导入结果的倍数补偿，一方面补偿单位，一方面补偿计算结果的偏差
	 

	 if(mode)//mode=1,切点在圆心指向圆外一点的向量的逆时针方向一侧
		a=acos(t); 
	 else
		a=-acos(t);
	 G.x=F.x*cos(a) -F.y*sin(a);
	 G.y=F.x*sin(a) +F.y*cos(a);   
	 

	 H.x=G.x+ptCenter.x;
	 H.y=G.y+ptCenter.y;            
		

	 return H;
}

/*******************************************************************
函数名称：CalAngle()
函数功能：计算一向量与ssPosX轴正半轴夹角
输入：    stAim:目标向量
输出：    目标向量与ssPosX轴正半轴夹角(RADIAN)
备注：    逆时针为正，顺时针为负，计算夹角范围为：[-PI,PI)
********************************************************************/
fp32 Angle(stPoint p0,stPoint p1)
{
    stPoint p;
	fp32 alpha;
	p.x=p1.x-p0.x;
	p.y=p1.y-p0.y;

    if(p.y == 0)//分母为0
    {
        if(p.x > 0)
        {
            return 0;
        }
        else if(p.x < 0)
        {
            return PI;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if(p.x == 0)
        {
            if(p.y > 0)
            {
                return PI_2;
            }
            else if(p.y < 0)
            {
                return -PI_2;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            alpha = atanf((fp32)p.y / (fp32)p.x);
            if (p.x < 0)//3、4象限
            {
                if (alpha >= 0)//4象限
                {
                    alpha -= PI;
                }
                else if (alpha < 0)//3
                {
                    alpha += PI;
                }
            }
            return alpha;
        }
    }
}

//计算长度
fp32 SLine(stPoint p1,stPoint p2)
{
	return (fp32)sqrtf((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}
//计算圆弧长度
fp32 SCircle(stPoint p1,stPoint p2,stPoint p0)
{
	fp32 theta,theta1,theta2,Radius;
	Radius = SLine(p1,p0);
	theta1 = Angle(p0,p1);
	theta2 = Angle(p0,p2);
	
	if((theta2-theta1)>PI)
		theta2 -= PI2;
	else if((theta2-theta1)<-PI)
		theta2 += PI2;
	theta = fabs(theta2 - theta1);
	return theta*Radius;
}
/*******************************************************************
函数名称：Cirpoint()
函数功能：计算圆心坐标
输入：    p1、p2、p3为圆上3点，Radius为半径大小
输出：    圆心坐标
备注：    
********************************************************************/
stPoint Cirpoint(stPoint p1,stPoint p2,stPoint p3,fp32 Radius)
{
    //ax+by=m
    //cx+dy+n
    fp32 a,b,c,d;
    fp32 m[4],n[4];
    stPoint p;
    int i;
    fp32 A,B,C;
    a=p2.y-p1.y;
    b=p1.x-p2.x;
    c=p2.y-p3.y;
    d=p3.x-p2.x;


    m[0]=(p2.y-p1.y)*p2.x-(p2.x-p1.x)*p2.y+SLine(p1,p2)*Radius;
    n[0]=(p2.y-p3.y)*p2.x-(p2.x-p3.x)*p2.y+SLine(p3,p2)*Radius;


    m[1]=(p2.y-p1.y)*p2.x-(p2.x-p1.x)*p2.y+SLine(p1,p2)*Radius;
    n[1]=(p2.y-p3.y)*p2.x-(p2.x-p3.x)*p2.y-SLine(p3,p2)*Radius;


    m[2]=(p2.y-p1.y)*p2.x-(p2.x-p1.x)*p2.y-SLine(p1,p2)*Radius;
    n[2]=(p2.y-p3.y)*p2.x-(p2.x-p3.x)*p2.y-SLine(p3,p2)*Radius;

    m[3]=(p2.y-p1.y)*p2.x-(p2.x-p1.x)*p2.y-SLine(p1,p2)*Radius;
    n[3]=(p2.y-p3.y)*p2.x-(p2.x-p3.x)*p2.y+SLine(p3,p2)*Radius;

    for(i = 0; i <= 3; ++i)
    {
        p.x = (m[i]*d-b*n[i])/(a*d-b*c);
        p.y = (a*n[i]-m[i]*c)/(a*d-b*c);
        A = Angle(p2,p1);
        B = Angle(p2,p);
        C = Angle(p2,p3);
        if(fabs(A-C) < PI)
        {
            if(fabs(B - (A + C) / 2.0f) < 0.0001f)
                return p;
        }
        else
        {
            if(A < 0)
                A = A + 2.0f * PI;
            if(B < 0)
                B = B + 2.0f * PI;
            if(C < 0)
                C = C + 2.0f * PI;
            if(fabs(B - (A + C) / 2.0f) < 0.0001f)
                return p;
        }
    }
    return p;
}


/*******************************************************************
函数名称：Bezier_Nav()
函数功能：根据点的数目给出n阶贝塞尔曲线
输入：    stNav的时间、目标点们、贝塞尔曲线需要用到的结构体、速度目标值
输出：    随时间变化的位置目标值与速度目标值
备注：    时间有归一化处理，总时间为t_sum（ms）
		  在比赛中时间不紧张的时候使用
********************************************************************/
//uint8_t Bezier_Nav(ST_NAV *pstNav, ST_POT *Des, stBezier_Para *pstBezier, ST_VELT * pstVel)
//{
//	fp32 	t;
//	fp32 	_1_t[3],_t_n[3];
//	fp32	tmp1=1,tmp2=1;
//	uint8_t 	cnt;
//	
//	//时间归一化
//	if(pstNav->time < pstBezier->t_sum)
//		t = pstNav->time*1.0/pstBezier->t_sum;
//	else
//	{
//		pstNav->enNavState = NAV_LOCK;
//		return 1;
//	}
//	
//	//算1-t的n次方和t的n次方
//	for(cnt=0; cnt<pstBezier->n; cnt++)
//	{
//		tmp1 *= (1-t);
//		tmp2 *= t;
//		_1_t[cnt] = tmp1;
//		_t_n[cnt] = tmp2;
//	}
//	
//	switch(pstBezier->n)
//	{
//		case 1:
//			Des->fpPosX = pstBezier->Pi[0].x*_1_t[0] + pstBezier->Pi[1].x*_t_n[0];
//			Des->fpPosY = pstBezier->Pi[0].y*_1_t[0] + pstBezier->Pi[1].y*_t_n[0];
//			pstVel->fpVx = (pstBezier->Pi[1].x - pstBezier->Pi[0].x)/pstBezier->t_sum*1000;
//			pstVel->fpVy = (pstBezier->Pi[1].y - pstBezier->Pi[0].y)/pstBezier->t_sum*1000;
//			break;
//		case 2:
//			Des->fpPosX = pstBezier->Pi[0].x*_1_t[1] + 2*pstBezier->Pi[1].x*_t_n[0]*_1_t[0] + pstBezier->Pi[2].x*_t_n[1];
//			Des->fpPosY = pstBezier->Pi[0].y*_1_t[1] + 2*pstBezier->Pi[1].y*_t_n[0]*_1_t[0] + pstBezier->Pi[2].y*_t_n[1];
//			pstVel->fpVx = 2*( (pstBezier->Pi[0].x + pstBezier->Pi[1].x)*_1_t[0] + (pstBezier->Pi[2].x - pstBezier->Pi[1].x)*_t_n[0] )/pstBezier->t_sum*1000;
//			pstVel->fpVy = 2*( (pstBezier->Pi[0].y + pstBezier->Pi[1].y)*_1_t[0] + (pstBezier->Pi[2].y - pstBezier->Pi[1].y)*_t_n[0] )/pstBezier->t_sum*1000;
//			break;
//		case 3:
//			Des->fpPosX = pstBezier->Pi[0].x*_1_t[2] + 3*pstBezier->Pi[1].x*_t_n[0]*_1_t[1] + 3*pstBezier->Pi[2].x*_t_n[1]*_1_t[0] + pstBezier->Pi[3].x*_t_n[2];
//			Des->fpPosY = pstBezier->Pi[0].y*_1_t[2] + 3*pstBezier->Pi[1].y*_t_n[0]*_1_t[1] + 3*pstBezier->Pi[2].y*_t_n[1]*_1_t[0] + pstBezier->Pi[3].y*_t_n[2];
//			pstVel->fpVx = 3*( (pstBezier->Pi[1].x - pstBezier->Pi[0].x)*_1_t[1] + 2*(pstBezier->Pi[2].x - pstBezier->Pi[1].x)*_t_n[0]*_1_t[0] + (pstBezier->Pi[3].x - pstBezier->Pi[2].x)*_t_n[1] )/pstBezier->t_sum*1000;
//			pstVel->fpVy = 3*( (pstBezier->Pi[1].y - pstBezier->Pi[0].y)*_1_t[1] + 2*(pstBezier->Pi[2].y - pstBezier->Pi[1].y)*_t_n[0]*_1_t[0] + (pstBezier->Pi[3].y - pstBezier->Pi[2].y)*_t_n[1] )/pstBezier->t_sum*1000;
//			break;
//		default:
//			Des->fpPosX = 0;
//			Des->fpPosY = 0;
//			pstVel->fpVx = 0;
//			pstVel->fpVy = 0;
//			break;
//	}
//	
//	return 0;
//	
//}

/*******************************************************************
函数名称：CalTime_min()
函数功能：计算以某一速度到达某一点的最短加速时间和减速时间
输入：    A1最大加速度，A2最大减速度，S距离，V目标速度
输出：    最短总时间T_min
备注：    从0开始加速
********************************************************************/
fp32 CalTime_min(fp32 A1,fp32 A2,fp32 S,fp32 V)
{
	fp32 T_min;
	T_min = (-V*A1+sqrt(V*V*A1*A1+A1*A2*V*V+2*A1*A2*S*(A1+A2)))/(A1*A2);
	return T_min;
}

/*******************************************************************
函数名称：CalTime_mid()
函数功能：计算以某一速度到达某一点的最短加速时间和减速时间
输入：    A1最大加速度，A2最大减速度，S距离，V目标速度
输出：    T_min中的加速时间
备注：    从0开始加速
********************************************************************/
fp32 CalTime_mid(fp32 A1,fp32 A2,fp32 S,fp32 V)
{
	fp32 T_min;
	fp32 T_mid;
	T_min = CalTime_min(A1,A2,S,V);
	T_mid = (A2*T_min+V)/(A1+A2);
	return T_mid;
}

/*******************************************************************
函数名称：CalLineTime()
函数功能：以初速为0，加速度A1加速、A2减速，停止距出发点S位置处所需要的加速时间
输入：    A1加速度，A2减速度，S距离
输出：    总用时T1、加速时间T11
备注：    
********************************************************************/
void CalLineTime(fp32 A1,fp32 A2,fp32 S,fp32 *T1,fp32 *T11)
{
	*T1 = sqrt(2.0f*(A1+A2)*S/(A1*A2));
	*T11 = *T1*A2/(A1+A2);
}

/*******************************************************************
函数名称：Trapezoid_Speed()
函数功能：以0初速的，在最大速度限制下的梯形速度规划，若未达到限制则采用三角形速度规划
输入：    A加速度，S距离，Vmax最大速度限制
输出：    T1总时间、T11加速时间、T12加速时间加上匀速时间
备注：    A不能为0
********************************************************************/
void Trapezoid_Speed(fp32 A,fp32 Vmax,fp32 S,fp32 *T1,fp32 *T11,fp32 *T12)
{
	fp32 V_expect;//全程加减速时，运动过程中的最大速度
	fp32 S_acc;//加减速路程
	V_expect = sqrt(A*S);
	if(V_expect < Vmax)
	{
		CalLineTime(A,A,S,T1,T11);
		*T12 = *T11;
	}
	else
	{
		*T11 = Vmax/A;
		S_acc = Vmax*Vmax/A;
		*T12 = *T11 + (S-S_acc)/Vmax;
		*T1 = *T12 + *T11;
	}
}
/*******************************************************************
函数名称：CalRushTime()
函数功能：计算以某一初速度加速到达某一点的冲刺时间
输入：    A加速度，S距离，V初速度
输出：    加速时间T_Rush
备注：    A不能为0
********************************************************************/
fp32 CalRushTime(fp32 A,fp32 S,fp32 V)
{
	fp32 T_Rush;
	T_Rush = (sqrt(V*V+2.0f*A*S)-V)/A;
	return T_Rush;
}


/*******************************************************************
函数名称：CalT_dST()
函数功能：以某一初速度以加速度A加速再减速最后在END处停下，会对减速结构体的加减速时间、矢量化初速度、加减速的加速度赋值
输入：    T_dST减速结构体、Start出发点、End终点、Vin初速度、A加速度
输出：    
备注：    会对Vin的直线方向做投影再计算，若方向不对则会产生实际上的折线
********************************************************************/
void CalT_dST(T_fasedown * T_dST,stPoint Start,stPoint End,ST_VELT Vin,fp32 A)
{
	fp32 S;
	static ST_VELT S_vector;
	static fp32 Vin_real;//实际初速度在起点在路径方向上的投影
	static fp32 T11;
	S = sqrt(pow(End.x-Start.x,2) + pow(End.y-Start.y,2));
	S_vector.fpVx = End.x-Start.x;//将路程向量化
	S_vector.fpVy = End.y-Start.y;
	Vin_real = fabs((Vin.fpVx*S_vector.fpVx + Vin.fpVy*S_vector.fpVy)/S);
	T_dST->VIN.fpVx = Vin_real*S_vector.fpVx/S;
	T_dST->VIN.fpVy = Vin_real*S_vector.fpVy/S;
	T_dST->t1 = CalTime_min(A,A,S,Vin_real);//计算总减速时间
	T11 = CalTime_mid(A,A,S,Vin_real);
	T_dST->t11 = T_dST->t1 - T11;//计算加速时间
	T_dST->Accup.fpVx = (2*(End.x-Start.x)-T_dST->VIN.fpVx*(T_dST->t1 + T_dST->t11))/(T_dST->t11*T_dST->t1);
	T_dST->Accup.fpVy = (2*(End.y-Start.y)-T_dST->VIN.fpVy*(T_dST->t1 + T_dST->t11))/(T_dST->t11*T_dST->t1);
	T_dST->Accdown.fpVx = (T_dST->VIN.fpVx+T_dST->Accup.fpVx*T_dST->t11)/(T_dST->t1 - T_dST->t11);
	T_dST->Accdown.fpVy = (T_dST->VIN.fpVy+T_dST->Accup.fpVy*T_dST->t11)/(T_dST->t1 - T_dST->t11);
	if(T_dST->t11<0)//加速再减速在A的加速度下无法实现，进行强减速，突破加速度限制
	{
		T_dST->t1 = 2.0f*S/Vin_real;
		T_dST->Accdown.fpVx = 2.0f*S_vector.fpVx/(T_dST->t1*T_dST->t1);
		T_dST->Accdown.fpVy = 2.0f*S_vector.fpVy/(T_dST->t1*T_dST->t1);
		T_dST->Accup.fpVx = 0;
		T_dST->Accup.fpVy = 0;
	}
}

int32_t my_intabs(int32_t num)
{
	if(num >= 0)
	{
		return num;
	}
	else 
	{
		return -num;
	}
}

/*-------------------------------------------------------------------
函数功能：判断正负
备    注：返回值为1和-1，来改变数的符号
-------------------------------------------------------------------*/
int8_t sign_judge(float fp_Judge_Number)
{
    return fp_Judge_Number >= 0 ? 1:-1;
}

/*输入的des范围为0到360度，返回距离fdb最近的角度值*/
fp32 cal_min_angle(fp32 des, fp32 fdb)
{
    fp32 abs_fdb;

    abs_fdb = ((int32_t)fdb + 720000) % 360 + fdb - (int32_t)fdb;

    if(fabs(des - abs_fdb) > 180)
    {
        if(des > abs_fdb)
        {
            des -= 360;
        }
        else
        {
            des += 360;
        }
    }
    return (fdb + des - abs_fdb);
}

//将全局坐标下的速度转化为局部坐标下的速度
void Concert_coorindnate(ST_VECTOR *global,ST_VECTOR *local,fp32 fpQ)
{
	Covert_coordinate(global,CARTESIAN);
	Covert_coordinate(local,CARTESIAN);
	local->fpVx =global->fpVx * cosf(fpQ) + 
											global->fpVy * sinf(fpQ);
	local->fpVy = -global->fpVx * sinf(fpQ) + 
												global->fpVy * cosf(fpQ);
	local->fpLength = sqrt(pow(local->fpVx, 2) + 
															pow(local->fpVx,2));
	local->fpW = global->fpW;
}

fp32 Geometric_mean(fp32 a,fp32 b)
{
	return sqrt(pow(a, 2) + pow(b, 2));
}

void Vector_minus(ST_VECTOR *a,ST_VECTOR *b,ST_VECTOR *c)
{
	Covert_coordinate(a,CARTESIAN);
	Covert_coordinate(b,CARTESIAN);
	Covert_coordinate(c,CARTESIAN);

	c->fpVx = a->fpVx - b->fpVx;
	c->fpVy = a->fpVy - b->fpVy;
	
	c->fpLength = Geometric_mean(c->fpVx,c->fpVy);
	
}
void Vector_Plus(ST_VECTOR *a,ST_VECTOR *b,ST_VECTOR *c)
{
	Covert_coordinate(a,CARTESIAN);
	Covert_coordinate(b,CARTESIAN);
	Covert_coordinate(c,CARTESIAN);

	c->fpVx = a->fpVx + b->fpVx;
	c->fpVy = a->fpVy + b->fpVy;
	
	c->fpLength = Geometric_mean(c->fpVx,c->fpVy);
	
}
/****************************************************************************************************
函数名称: Vector_cross(ST_VECTOR *r,fp32 w,ST_VECTOR *c)
函数功能: 外积，
输入参数: 
返回参数: 
备    注:
****************************************************************************************************/
void Vector_cross(ST_VECTOR *r,fp32 w,ST_VECTOR *c)
{
	Covert_coordinate(r,CARTESIAN);
	Covert_coordinate(c,CARTESIAN);
	c->fpVx = -w* r->fpVy;
	c->fpVy = w*r->fpVx;
	c->fpLength = Geometric_mean(c->fpVx,c->fpVy);
}

//就相当于解出极坐标下的点在笛卡尔坐标系下的投影，解出笛卡尔坐标系下的点在极坐标系下的投影
void Covert_coordinate(ST_VECTOR *a,COORDINATE _type)
{
    if(a->type == POLAR)
    {
    a->fpVx = a->fpLength*cosf(a->fpthetha*RADIAN);
    a->fpVy = a->fpLength*sinf(a->fpthetha*RADIAN);
    }
    else if(a->type == CARTESIAN)
    {
        a->fpLength = Geometric_mean(a->fpVx,a->fpVy);
        if(fabs(a->fpVy)<EPS&&fabs(a->fpVx)<EPS)
        {
                a->fpthetha = 0;
        }
        else
        a->fpthetha = atan2(a->fpVy,a->fpVx)/RADIAN;
    }
    a->type = _type;
}
//向量A在B上的投影
void Vector_Projection_BonA(ST_VECTOR *a,ST_VECTOR *b,ST_VECTOR *c)
{
	Covert_coordinate(a,CARTESIAN);
	Covert_coordinate(b,CARTESIAN);
	Covert_coordinate(c,CARTESIAN);
  
	fp32 theta,L;//L是有正负的

	theta = a->fpthetha - b->fpthetha;
  L = b->fpLength * cos(theta*RADIAN);
	c->fpVx = L * cos(a->fpthetha*RADIAN);
	c->fpVy = L * sin(a->fpthetha*RADIAN);
	Covert_coordinate(c,CARTESIAN);
}
//向量B在A上的投影，c是一个有正负的数。
void Vector_Projection_BonA_2(ST_VECTOR *a,ST_VECTOR *b,fp32 *c)
{
	Covert_coordinate(a,CARTESIAN);
	Covert_coordinate(b,CARTESIAN);
  
	fp32 theta;

	theta = a->fpthetha - b->fpthetha;
  *c = b->fpLength * cos(theta*RADIAN);
}
//向量A与B的点积
void Vector_Product(ST_VECTOR *a,ST_VECTOR *b,fp32 *c)
{
	Covert_coordinate(a,CARTESIAN);
	Covert_coordinate(b,CARTESIAN);

	*c = a->fpVx * b->fpVx + a->fpVy * b->fpVy;
}
//向量旋转
void Vector_Turn(ST_VECTOR *a,ST_VECTOR *b,fp32 c)
{
	Covert_coordinate(a,CARTESIAN);
	Covert_coordinate(b,CARTESIAN);
  
	b->fpthetha = a->fpthetha + c;
	b->fpLength = a->fpLength;
	b->fpVx = b->fpLength * cos(b->fpthetha*RADIAN);
	b->fpVy = b->fpLength * sin(b->fpthetha*RADIAN);
	b->fpW = a->fpW;

}
