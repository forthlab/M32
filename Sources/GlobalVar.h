
#ifndef __GOLBALVAR__
#define __GOLBALVAR__


#define  TPS   50    //50次/秒,20ms间隔  

#define VERSION 0x325A   //软件版本,用16进制显示模式: 32- 34-    


 extern unsigned char ManualON;   //手动

//系统参数

extern unsigned int SysTick;     //20ms tick

#define SETUP_ON_FLASH_ADDR   0xe000    //Setup在flash的映像地址



//设置控制参数

#define SETUP_ROW   10           //用户设置参数表StructSetup的参数数量

//参数表显示调整控制表结构定义
struct SetupStruct{

  //signed   int  Val;              //参数值,V3210将其放在SetupVal[]中,并同步到FLASH中
  unsigned int  Prompt;           //提示符
  unsigned char Show;             //显示提示信息,0:不显示此值,其他显示val; show为提示信息
  unsigned char Adjust;           //是否可以调整参数
  signed int  Max;                //最大调整值
  signed int  Min;                //最小调整值
  
};


 //参数表显示调整控制表@ROM
#pragma INTO_ROM

extern const struct SetupStruct SetupOnROM[ SETUP_ROW ];

//控制参数值
extern int SetupVal[SETUP_ROW];
extern int SetupOnFlash[ SETUP_ROW ] ;

#define SetupVal_D1  SetupVal[5]    //V3232: 	SGCC 国网监控控制, 1:有效, 0: 无效

#define WenduGap0  SetupVal[6]       //V3235: 温度0校准，单位+-0.1°
#define ShiduGap0  SetupVal[7]       //V3235: 湿度0校准，单位+-1%
#define WenduGap1  SetupVal[8]       //V3235: 温度1校准
#define ShiduGap1  SetupVal[9]       //V3235: 湿度1校准，单位+-1%

//4路负载AD检测值   

extern unsigned int AD[4];          //AD读取的原值 
extern unsigned int LoadVal[4];     //负载检测的滤波值

//负载正常的阀值
#define LoadOKVal   0x9000    //有负载时,信号小,没有负载电流时,信号大  

  
  
//2路温,湿度值及控制状态,保存在结构体中;
struct ParameterStruct{
  signed int Val;           //温度或者湿度的测量值,单位0.1度    
  
  signed int SetVal;        //触发值(设定值)
  signed int ResetVal;      //恢复值

  signed int WorkingTick;   //触发进入状态计时(=继电器吸合)(吸合++/不吸合--)

  unsigned int InvalidCnt;  //传感器异常计数,只在温度上设置,湿度的忽略     
 
  unsigned char Mode:1;       //0: 降温模式; 1: 升温模式;  除湿相当于散热模式
  unsigned char Working:1;    //触发进入状态(工作)标志,启动/停止
  unsigned char Alarm:1;      //告警状态标志,没有使用!
  unsigned char LoadON:1;     //负载状态(1:有负载/0:无负载)
  unsigned char LoadAlarm:1;  //负载异常(0:正常/1:异常)
  unsigned char SGCC:   1;    //国家电网的控制状态激活成功
  };
  
  
/*	SGCC 国网要求满足下面的条件时,同时启动温度和湿度继电器:
		(1) 温度<5度启动
		(2) 温度在5-25度,湿度>=65%,启动; 低于50%恢复正常
		(3) 温度在25-38度,湿度>=75%,启动; 低于50%恢复正常  */
  
  
extern struct ParameterStruct  Parameter[4];

#define SensorInvalidCount  10   //传感器读取错误允许次数,超过则传感器故障

#define WENDU1  Parameter[0]
#define SHIDU1  Parameter[1]
#define WENDU2  Parameter[2]
#define SHIDU2  Parameter[3]

#define WARM_MODE   1   //加热模式
#define FAN_MODE    0   //冷却模式





//LED指示灯点亮熄灭的宏定义

#define LED1_ON   LED_buf[1] |= 0x10;    //LED1 温度1
#define LED1_OFF  LED_buf[1] &= 0xef;

#define LED2_ON   LED_buf[3] |= 0x10;     //LED2 湿度1    
#define LED2_OFF  LED_buf[3] &= 0xef;

#define LED3_ON   LED_buf[5] |= 0x10;     //LED3 温度2
#define LED3_OFF  LED_buf[5] &= 0xef;

#define LED4_ON   LED_buf[7] |= 0x10;     //LED4 湿度2 
#define LED4_OFF  LED_buf[7] &= 0xef;


#define MANUAL_LED_ON   LED_buf[9] |= 0x10;     //LED4 手动 
#define MANUAL_LED_OFF  LED_buf[9] &= 0xef;


//按键
#define KEY_OK    0x20
#define KEY_UP    0x10
#define KEY_DOWN  0x80
#define KEY_MANUAL 0x04    //手动,打开4路





#endif





