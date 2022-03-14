
#ifndef __GOLBALVAR__
#define __GOLBALVAR__


#define  TPS   50    //50��/��,20ms���  

#define VERSION 0x3234   //����汾,��16������ʾģʽ: 32- 34-    


 extern unsigned char ManualON;   //�ֶ�

//ϵͳ����

extern unsigned int SysTick;     //20ms tick

#define SETUP_ON_FLASH_ADDR   0xe000    //Setup��flash��ӳ���ַ



//���ÿ��Ʋ���

#define SETUP_ROW   8           //�û����ò�����StructSetup�Ĳ�������

//��������ʾ�������Ʊ�ṹ����
struct SetupStruct{

  //signed   int  Val;              //����ֵ,V3210�������SetupVal[]��,��ͬ����FLASH��
  unsigned int  Prompt;           //��ʾ��
  unsigned char Show;             //��ʾ��ʾ��Ϣ,0:����ʾ��ֵ,������ʾval; showΪ��ʾ��Ϣ
  unsigned char Adjust;           //�Ƿ���Ե�������
  signed int  Max;                //������ֵ
  signed int  Min;                //��С����ֵ
  
};


 //��������ʾ�������Ʊ�@ROM
#pragma INTO_ROM

extern const struct SetupStruct SetupOnROM[ SETUP_ROW ];

//���Ʋ���ֵ
extern int SetupVal[SETUP_ROW];
extern int SetupOnFlash[ SETUP_ROW ] ;

#define SetupVal_D1  SetupVal[5]    //V3232: 	SGCC ������ؿ���, 1:��Ч, 0: ��Ч

#define WenduGap  SetupVal[6]       //V3233: 	SGCC ������ؿ���, 1:��Ч, 0: ��Ч
#define ShiduGap  SetupVal[7]       //V3233: 	SGCC ������ؿ���, 1:��Ч, 0: ��Ч


//4·����AD���ֵ   

extern unsigned int AD[4];          //AD��ȡ��ԭֵ 
extern unsigned int LoadVal[4];     //���ؼ����˲�ֵ

//���������ķ�ֵ
#define LoadOKVal   0x9000    //�и���ʱ,�ź�С,û�и��ص���ʱ,�źŴ�  

  
  
//2·��,ʪ��ֵ������״̬,�����ڽṹ����;
struct ParameterStruct{
  signed int Val;           //�¶Ȼ���ʪ�ȵĲ���ֵ,��λ0.1��    
  
  signed int SetVal;        //����ֵ(�趨ֵ)
  signed int ResetVal;      //�ָ�ֵ

  signed int WorkingTick;   //��������״̬��ʱ(=�̵�������)(����++/������--)

  unsigned int InvalidCnt;  //�������쳣����,ֻ���¶�������,ʪ�ȵĺ���     
 
  unsigned char Mode:1;       //0: ����ģʽ; 1: ����ģʽ;  ��ʪ�൱��ɢ��ģʽ
  unsigned char Working:1;    //��������״̬(����)��־,����/ֹͣ
  unsigned char Alarm:1;      //�澯״̬��־,û��ʹ��!
  unsigned char LoadON:1;     //����״̬(1:�и���/0:�޸���)
  unsigned char LoadAlarm:1;  //�����쳣(0:����/1:�쳣)
  unsigned char SGCC:   1;    //���ҵ����Ŀ���״̬����ɹ�
  };
  
  
/*	SGCC ����Ҫ���������������ʱ,ͬʱ�����¶Ⱥ�ʪ�ȼ̵���:
		(1) �¶�<5������
		(2) �¶���5-25��,ʪ��>=65%,����; ����50%�ָ�����
		(3) �¶���25-38��,ʪ��>=75%,����; ����50%�ָ�����  */
  
  
extern struct ParameterStruct  Parameter[4];

#define SensorInvalidCount  10   //��������ȡ�����������,�����򴫸�������

#define WENDU1  Parameter[0]
#define SHIDU1  Parameter[1]
#define WENDU2  Parameter[2]
#define SHIDU2  Parameter[3]

#define WARM_MODE   1   //����ģʽ
#define FAN_MODE    0   //��ȴģʽ





//LEDָʾ�Ƶ���Ϩ��ĺ궨��

#define LED1_ON   LED_buf[1] |= 0x10;    //LED1 �¶�1
#define LED1_OFF  LED_buf[1] &= 0xef;

#define LED2_ON   LED_buf[3] |= 0x10;     //LED2 ʪ��1    
#define LED2_OFF  LED_buf[3] &= 0xef;

#define LED3_ON   LED_buf[5] |= 0x10;     //LED3 �¶�2
#define LED3_OFF  LED_buf[5] &= 0xef;

#define LED4_ON   LED_buf[7] |= 0x10;     //LED4 ʪ��2 
#define LED4_OFF  LED_buf[7] &= 0xef;


#define MANUAL_LED_ON   LED_buf[9] |= 0x10;     //LED4 �ֶ� 
#define MANUAL_LED_OFF  LED_buf[9] &= 0xef;


//����
#define KEY_OK    0x20
#define KEY_UP    0x10
#define KEY_DOWN  0x80
#define KEY_MANUAL 0x04    //�ֶ�,��4·





#endif





