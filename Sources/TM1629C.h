#ifndef __TM1629C__
#define __TM1629C__

/* 定义芯片接口CLK,DIO,STB  */

#include "STB.h"
#include "DIO.h"
#include "CLK.h"

extern unsigned char LED_buf[16];     //TM1629LED显示位缓存,直接发送到TM1929C



/*---------------------------------------------
  TM1629读取按键函数  
  unsigned char  TM1629_GetKey( void) 
  buf保存按键数值
---------------------------------------------*/
unsigned char TM1629_GetKey( void );


/*---------------------------------------------
 将缓存buf[]的n个数据,写入TM1629芯片的地址addr(LED显示数据缓存)
   
 void TM1629_WriteDateBuf( unsigned char * buf, unsigned char n, unsigned char brightness ) 
    buf:  TM1629位显示缓存
    addr: 显示缓存地址
    n:    数量
    brightness: 亮度(0..7)
---------------------------------------------*/
void TM1629_WriteDateBuf( unsigned char * buf, unsigned char addr, unsigned char n, unsigned char brightness ) ;



/*---------------------------------------------
     第n组数码管显示数字i
     i的范围是-99...999
    void DisplayInt( signed int i, unsigned n)
        group     数码管序号0..3
        i     数值， 范围是-999...999 
        dot:  小数位数， 0：没有； 1：一位小数。。。
---------------------------------------------*/ 

void DisplayInt( unsigned char group, signed int i, unsigned char dot );


/*---------------------------------------------
     数码管显示16进制数字，范围是000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     数码管序号0..3
        i     数值， 范围是-999...999 
---------------------------------------------*/ 

//void DisplayHEX( unsigned char group, signed int i );



/*---------------------------------------------
    第n组数码管显示16进制数字的序号i: "A1-"
    
    每组数码管3个
    i的范围是000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     数码管序号0..3
        i     数值， 范围是-999...999 
---------------------------------------------*/ 

void DisplayHEX_( unsigned char group, unsigned int i);


/*---------------------------------------------
    第n组数码管显示 "---"
    
    每组数码管3个
    
---------------------------------------------*/ 

void Display___( unsigned char group );



/*---------------------------------------------

          数码管清除显示
          
 -------------------------------------------*/

 void DisplayClr( unsigned char n);




//  test
void TestTM1629( void );


#endif





