
#include "GlobalVar.h"


unsigned int SysTick=0;     //100ms tick



//�û����ò�����
int SetupVal[SETUP_ROW];   

//setup[]@FLASH,�����û����õĲ���ֵ  
volatile int SetupOnFlash[ SETUP_ROW ]  @SETUP_ON_FLASH_ADDR;  

//setup[]@ROM, ���ݽṹ,�����Ƿ���ʾ,�Ƿ�ɵ���, ���ֵ,��Сֵ
#pragma INTO_ROM
const struct SetupStruct SetupOnROM[ SETUP_ROW ]=  
 
    //   ��ʾ  �Ƿ���ʾ   �ɵ���
    // prompt  show       adjust  max   min
    {    
    {  0xA1,    1,        1,      1,    0    },   // mode;ģʽ
       
    {  0xB1,    1,        1,      199,  -99  },   // WenduSet; �¶ȿ���ֵ
    {  0xB2,    1,        1,      199,  -99  },   // WenduSet; �¶Ȼָ�ֵ        
    
    {  0xC1,    1,        1,      99,   0    },     // ShiduSet; ʪ�ȿ���ֵ
    {  0xC2,    1,        1,      99,   0    },     // ShiduSet; ʪ�ȿ���ֵ
    
    {  0xD1,    1,        1,      1,    0    },     // �����ļ�ؿ���; 1: ��Ч; Ver3232
    
    {  0xE1,    1,        1,      99,   -99    },     // WenduGap; �¶�ƫ��+-9.9��C,��λ0.1
    {  0xE2,    1,        1,      90,   -90    },     // ShiduGap; ʪ��ƫ��+-90%����λ1%
    
    };    
   
                         
                         
 //4·����AD���ֵ   
 unsigned int AD[4];   
 unsigned int LoadVal[4];     //���ؼ����˲�ֵ
 
 
 //2·��ʪ��ֵ������״̬,�����ڽṹ����,�ֱ����¶�1,ʪ��1,�¶�2,ʪ��2
 struct ParameterStruct Parameter[4];
 
 
 unsigned char ManualON =0;   //�ֶ�
 

 




