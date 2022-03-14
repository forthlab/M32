#ifndef __TM1629C__
#define __TM1629C__

/* ����оƬ�ӿ�CLK,DIO,STB  */

#include "STB.h"
#include "DIO.h"
#include "CLK.h"

extern unsigned char LED_buf[16];     //TM1629LED��ʾλ����,ֱ�ӷ��͵�TM1929C



/*---------------------------------------------
  TM1629��ȡ��������  
  unsigned char  TM1629_GetKey( void) 
  buf���水����ֵ
---------------------------------------------*/
unsigned char TM1629_GetKey( void );


/*---------------------------------------------
 ������buf[]��n������,д��TM1629оƬ�ĵ�ַaddr(LED��ʾ���ݻ���)
   
 void TM1629_WriteDateBuf( unsigned char * buf, unsigned char n, unsigned char brightness ) 
    buf:  TM1629λ��ʾ����
    addr: ��ʾ�����ַ
    n:    ����
    brightness: ����(0..7)
---------------------------------------------*/
void TM1629_WriteDateBuf( unsigned char * buf, unsigned char addr, unsigned char n, unsigned char brightness ) ;



/*---------------------------------------------
     ��n���������ʾ����i
     i�ķ�Χ��-99...999
    void DisplayInt( signed int i, unsigned n)
        group     ��������0..3
        i     ��ֵ�� ��Χ��-999...999 
        dot:  С��λ���� 0��û�У� 1��һλС��������
---------------------------------------------*/ 

void DisplayInt( unsigned char group, signed int i, unsigned char dot );


/*---------------------------------------------
     �������ʾ16�������֣���Χ��000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     ��������0..3
        i     ��ֵ�� ��Χ��-999...999 
---------------------------------------------*/ 

//void DisplayHEX( unsigned char group, signed int i );



/*---------------------------------------------
    ��n���������ʾ16�������ֵ����i: "A1-"
    
    ÿ�������3��
    i�ķ�Χ��000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     ��������0..3
        i     ��ֵ�� ��Χ��-999...999 
---------------------------------------------*/ 

void DisplayHEX_( unsigned char group, unsigned int i);


/*---------------------------------------------
    ��n���������ʾ "---"
    
    ÿ�������3��
    
---------------------------------------------*/ 

void Display___( unsigned char group );



/*---------------------------------------------

          ����������ʾ
          
 -------------------------------------------*/

 void DisplayClr( unsigned char n);




//  test
void TestTM1629( void );


#endif





