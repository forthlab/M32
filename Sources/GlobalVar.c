
#include "GlobalVar.h"


unsigned int SysTick=0;     //100ms tick



//用户设置参数表
int SetupVal[SETUP_ROW];   

//setup[]@FLASH,保存用户设置的参数值  
volatile int SetupOnFlash[ SETUP_ROW ]  @SETUP_ON_FLASH_ADDR;  

//setup[]@ROM, 数据结构,包括是否显示,是否可调整, 最大值,最小值
#pragma INTO_ROM
const struct SetupStruct SetupOnROM[ SETUP_ROW ]=  
 
    //   提示  是否显示   可调整
    // prompt  show       adjust  max   min
    {    
    {  0xA1,    1,        1,      1,    0    },   // mode;模式
       
    {  0xB1,    1,        1,      199,  -99  },   // WenduSet; 温度控制值
    {  0xB2,    1,        1,      199,  -99  },   // WenduSet; 温度恢复值        
    
    {  0xC1,    1,        1,      99,   0    },     // ShiduSet; 湿度控制值
    {  0xC2,    1,        1,      99,   0    },     // ShiduSet; 湿度控制值
    
    {  0xD1,    1,        1,      1,    0    },     // 国网的监控控制; 1: 有效; Ver3232
    
    {  0xE1,    1,        1,      99,   -99    },     // WenduGap0; 温度偏差+-9.9°C,单位0.1
    {  0xE2,    1,        1,      90,   -90    },     // ShiduGap0; 湿度偏差+-90%，单位1%
    
    {  0xE3,    1,        1,      99,   -99    },     // WenduGap1; 温度偏差+-9.9°C,单位0.1
    {  0xE4,    1,        1,      90,   -90    },     // ShiduGap1; 湿度偏差+-90%，单位1%

    
    };    
   
                         
                         
 //4路负载AD检测值   
 unsigned int AD[4];   
 unsigned int LoadVal[4];     //负载检测的滤波值
 
 
 //2路温湿度值及控制状态,保存在结构体中,分别是温度1,湿度1,温度2,湿度2
 struct ParameterStruct Parameter[4];
 
 
 unsigned char ManualON =0;   //手动
 

 




