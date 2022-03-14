#include "TM1629C.h"


/* 定义芯片接口CLK,DIO,STB  */

#include "STB.h"
#include "DIO.h"
#include "CLK.h"


//unsigned char key_date[4];    //TM1629按键值
unsigned char LED_buf[16];     //TM1629LED显示位缓存,直接发送到TM1929C


//----------------------------------------------
//    TM1629C显示数据格式说明:  
//    1. 数据读写指令-写显示缓存指令
//    2. 设置地址指令+4位地址0000
//    3. 发送显示位数据(16byte)
//    4. 显示控制指令
//----------------------------------------------
//----------------------------------------------
//    TM1629C读取按键数据格式:  
//    1. 读键盘数据
//    2. 读取4字节键盘数据
//----------------------------------------------

//TM1629C数据读写指令

#define WRITE_DISPLAY_BUF_CMD   0x40      //写显示缓存指令
#define READ_KEY_DATE_CMD       0x42      //读键盘数据

//TM1629C显示控制指令,最后3位是亮度,默认是最暗

#define DISPLAY_MODE_ON     0x88      //开LED显示+3位亮度
#define DISPLAY_MODE_OFF    0x80      //关LED显示

//TM1629C地址设置指令
#define ADDRESS_SET     0xC0      //设置地址指令+4位地址


// 数码管字段ABCDEFGH, 置位点亮,
// 0,1,2,3,4,5,6,7,8,9,   abcdef  暗 -     
// [ 暗: 数值: 0,序号16; '-': 值: 0x02,序号17 ;  小数点:数值 0x01 ]
//const unsigned char Bar[]= { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0, 0x40};
  const unsigned char Bar[]= { 0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6, 0xee, 0x3e, 0x9c, 0x7a, 0x9e, 0x8e, 0, 0x02 };//3233,负号不是40,是02
     



/*---------------------------------------------
  从TM1629读取一个字节
---------------------------------------------*/

unsigned char TM1629_getc(void)
    {
    
    unsigned char c=0;
    unsigned char n;
    
    DIO_SetInput();     //DIO设置为输入状态，读取TM1629数据D0...D7      

    for( n=0;n<8;n++){
    
        c = c>>1;
    
        CLK_PutVal(0);    //CLK=0;
        
        if( DIO_GetVal() !=0 ) 
            c= c|0x80;    //数据从高位进入,顺序b0...b7
    
        CLK_PutVal(1);    //CLK=1;上升沿读数据            
        
        }
      
    return c;
    
    }



/*---------------------------------------------
  写一个字节到TM1629
---------------------------------------------*/
void TM1629_putc(unsigned char cc)
{
    unsigned char n,c;
 
    c =cc;              //输出数据
    
    DIO_SetOutput();    //DIO设置为输出状态，写入TM1629数据D0...D7    

    for( n=0; n<8; n++){
    
      CLK_PutVal(0);        //CLK=0;    
    
      DIO_PutVal( (c&0x01)==0? 0:1 );  //写DIO数据
    
      CLK_PutVal(1);        //CLK=1;上升沿触发写数据
    
      c = c>>1;
      }
    
    DIO_SetInput();   //DIO设置为输入状态 
     
    return;
  
}   




/*---------------------------------------------
  TM1629读取按键函数  
  unsigned char  TM1629_GetKey( void) 
  buf保存按键数值
---------------------------------------------*/
unsigned char TM1629_GetKey( void ) 
{

  //unsigned char buf[4];
  unsigned char c,d,k;
  
   STB_PutVal(0);               //STB=0

   TM1629_putc(READ_KEY_DATE_CMD);  //读键盘数据指令    

   for( k=0; k< 20; k++);    //delay 2us

   c = TM1629_getc();      //取数
   c = (c&0x80)|((c&0x08)<<3);
   d = c;                  //取b7,b3                
   
   c = TM1629_getc();      //取数
   c = (c&0x80)|((c&0x08)<<3);
   d = d | (c>>2);         //取(b7,b3)(b7,b3)

   c = TM1629_getc();      //取数
   c = (c&0x80)|((c&0x08)<<3);
   d = d | (c>>4);         //取(b7,b3)(b7,b3)
   
   c = TM1629_getc();      //取数
   c = (c&0x80)|((c&0x08)<<3);
   d = d | (c>>6);         //取(b7,b3)(b7,b3)       

   STB_PutVal(1);               //STB=1     
   
   return d;
}




/*---------------------------------------------
 将缓存buf[]的n个数据,写入TM1629芯片的地址addr(LED显示数据缓存)
   
 void TM1629_WriteDateBuf( unsigned char * buf, unsigned char n, unsigned char brightness ) 
    buf:  TM1629位显示缓存
    addr: 显示缓存地址
    n:    数量
    brightness: 亮度(0..7)
---------------------------------------------*/
void TM1629_WriteDateBuf( unsigned char * buf, unsigned char addr, unsigned char n, unsigned char brightness ) 
{
   unsigned char c;
   
   STB_PutVal(0);                                 //STB=0  
   TM1629_putc( WRITE_DISPLAY_BUF_CMD );          //发送显示数据指令
   STB_PutVal(1);                                 //STB=1
   
   
   STB_PutVal(0);                                 //STB=0   
   TM1629_putc( ADDRESS_SET | (addr & 0x07 ) );   //显示缓存地址   
                                 
   for(c=0; c<n; c++){                            //发送n个显示数据
      TM1629_putc( buf[c] );
   }
   
   STB_PutVal(1);                  //STB=1
   
   
   STB_PutVal(0);                   //STB=0
   TM1629_putc( DISPLAY_MODE_ON | (brightness &0x0f) ); //发送亮度
   
   STB_PutVal(1);                                 //STB=1
   
}



/*-----------------------------------------------------------------------
        void Swap2buf( unsigned char dat, unsigned char *buf, unsigned char n )

            将数据c的8个比特数据,竖着存入buf第n（0..15）列，注意，一行16bit（2字节）
            例如
            c=1011 0011存入buf[0..7]的第bit5列,如下：
            
                    (n)
      数码管         |
      段             V                     
      a     buf[0] XX1X XXXX    buf[1] XXXXXXXX  
      b     buf[2] XX0X XXXX    buf[3] XXXXXXXX
      c     buf[4] XX1X XXXX    buf[5] XXXXXXXX
      d     buf[6] XX1X XXXX    buf[7] XXXXXXXX
            
      e     buf[8] XX0X XXXX    buf[ ] XXXXXXXX
      f     buf[10]XX0X XXXX    buf[ ] XXXXXXXX
      g     buf[12]XX1X XXXX    buf[ ] XXXXXXXX
      h     buf[14]XX1X XXXX    buf[ ] XXXXXXXX   
            
            如果n>7,则向后续字节延续
------------------------------------------------------------------------*/

void Swap2buf( unsigned char dat, unsigned char *buf, unsigned char n )
{

unsigned char i;
unsigned char c;      //原始数据
unsigned char mask;   //移位,如0010 0000

      c = dat;
      mask = 0x01 << (n&0x07);   //从最低位向最高位的第n位！！！

      for( i = 0; i<8; i++) {
      
          if( c & 0x80 )          //测试数据位
              {
              buf[ i*2 + (n>>3)] |= mask;   //修改数据位
              }
            else
              {
              buf[ i*2 + (n>>3)] &= mask^0xff;
              }
         
          c = c<<1;
        
      }//for

} 




void Swap2buf_Incert( unsigned char dat, unsigned char *buf, unsigned char n )
{

unsigned char i;
unsigned char c;      //原始数据
unsigned char mask;   //移位,如0010 0000

      c = dat;
      mask = 0x01 << (n&0x07);   //从最高位向最低位的第n位

      for( i = 0; i<8; i++) {
      
         if( c & 0x01 ) {
              buf[ i*2 + (n>>3)] |= mask;
         }
         // else{
         //     buf[ i*2 + (n>>3)] &= mask^0xff;
         //}
         
         c = c>>1;
        
      }//for

} 



/*---------------------------------------------
     第n个数码管显示数字i
     i的范围是0...9
    void DisplayDigit( unsigned char i, unsigned n)
        n     数码管序号0..15
        i     数值， 范围是0.9 
---------------------------------------------*/ 

void DisplayDigit( unsigned int i, unsigned n)
{
           
    Swap2buf( Bar[i], LED_buf, n );   
                      
}





/*---------------------------------------------
     第n组数码管显示数字i
     i的范围是-99...999
    void DisplayInt( signed int i, unsigned n)
        group     数码管序号0..3
        i     数值， 范围是-999...999 
        dot:  小数点， 0：没有； 1：第一位。。。
---------------------------------------------*/ 

void DisplayInt2( unsigned char group, signed int i, unsigned char dot )
{
    
    
    if( i> 999) i = 999;
    if( i< -99) i = -99;
     
              //显示正数
    if( i>=0){  
          
        if( i>99 )Swap2buf( Bar[i/100],     LED_buf, group*3 ); 
            else  Swap2buf( 0,              LED_buf, group*3 );       //显示百位
            
        //if( i>9  )
            Swap2buf( Bar[(i/10)%10], LED_buf, group*3+1 ); 
        //    else  Swap2buf( 0,              LED_buf, group*3+1 );   //显示十位
            
        Swap2buf( Bar[(i)%10],    LED_buf, group*3+2 );               //显示个位         
       
        }
        
              //显示负数-99..-10
    else if( i<-9 )
        {
    
        Swap2buf( 0x02,         LED_buf, group*3);               //显示负号（0x02）
            
        Swap2buf( Bar[(-i)/10], LED_buf, group*3+1);             //显示十位
            
        Swap2buf( Bar[(-i)%10], LED_buf, group*3+2);              //显示个位
            
        }
        
        /*最少显示2位
        
              //显示负数-9..-1
    else{
    
        Swap2buf( 0,            LED_buf, group*3);         //第一个数码管暗
        Swap2buf( 0x40,         LED_buf, group*3+1);         //显示负号（0x40）
        Swap2buf( Bar[(-i)%10], LED_buf, group*3+2);         //显示个位
        } 
          */
   
   //显示小数点
   if( (dot >0 ) && (dot<=3) ){ 
        Swap2buf_Incert( 0x80,   LED_buf, group*3+(3-dot) );            //插入小数点显示数据 
   }
   
                      
}






/*---------------------------------------------
     第n组数码管显示数字i
     i的范围是-99...999
    void DisplayInt( signed int i, unsigned n)
        group     数码管序号0..3
        i     数值， 范围是-999...999 
        dot:  小数点位数， 0：没有； 1：一位小数。。。
---------------------------------------------*/ 

void DisplayInt( unsigned char group, signed int i, unsigned char dot )
{    
    
    if( i> 999) i = 999;
    if( i< -99) i = -99;
     
              //显示正数和零
    if( i>=100 ){
        DisplayDigit(  i/100,   group*3   );    //XXX,显示百位数
        DisplayDigit( (i/10)%10,group*3+1 );    //显示十位
        DisplayDigit( (i)%10,   group*3+2 );    //显示个位 
        }
          
    else if( i>=0 ){
        DisplayDigit( 16,       group*3);       // 0XX, 不显示百位,'16#'是暗        
        DisplayDigit( (i/10)%10,group*3+1 );    //显示十位
        DisplayDigit( (i)%10,   group*3+2);     //显示个位          
        }
        
              //显示负数-1..-9
    else if( i>= -9 )
        {        
        DisplayDigit(16, group*3+0);            //百位暗(16#)
        DisplayDigit(17, group*3+1);            //十显示负号(17#) 
        DisplayDigit( (-i)%10, group*3+2 );     //个位显示数字         
        }
        
             //显示负数-99..-10
    else 
        {         
        DisplayDigit(17,          group*3+0);      //百位显示负号(17#) 
        DisplayDigit((-i/10)%10,  group*3+1);      //十显示数字 
        DisplayDigit( (-i)%10,    group*3+2 );     //个位显示数字           
        }      
    
    //显示小数点
    if( (dot >0 ) && (dot<=3) ){ 
        Swap2buf_Incert( 0x80,   LED_buf, group*3+(3-dot) );            //插入小数点显示数据 
    
    }
                          
}



/*---------------------------------------------
    第n组数码管显示16进制数字i，
    每组数码管3个
    i的范围是000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     数码管序号0..3
        i     数值， 范围是-999...999 
---------------------------------------------*/ 

void DisplayHEX( unsigned char group, unsigned int i)
{
    if( i> 0xfff) i = 0xfff;
      
    Swap2buf( Bar[i>>8],        LED_buf, group*3);     //显示百位
    Swap2buf( Bar[(i>>4)&0x0f], LED_buf, group*3+1);     //显示十位
    Swap2buf( Bar[(i>>0)&0x0f], LED_buf, group*3+2);     //显示个位                 
}




/*---------------------------------------------
    第n组数码管显示16进制数字的序号i: "A1-"
    
    每组数码管3个
    i的范围是000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     数码管序号0..3
        i     数值， 范围是-999...999 
---------------------------------------------*/ 

void DisplayHEX_( unsigned char group, unsigned int i)
{
    if( i> 0x0ff) i = 0x0ff;
      
    Swap2buf( Bar[i>>4],      LED_buf, group*3);       //显示高位
    Swap2buf( Bar[(i)&0x0f],  LED_buf, group*3+1);     //显示低位
    Swap2buf( 0x02,           LED_buf, group*3+2);     //显示 -                 
}




/*---------------------------------------------
    第n组数码管显示 "---"
    
    每组数码管3个
    
---------------------------------------------*/ 

void Display___( unsigned char group )
{
   
      
    Swap2buf( 0x02,  LED_buf, group*3);       //显示-
    Swap2buf( 0x02,  LED_buf, group*3+1);     //显示-
    Swap2buf( 0x02,  LED_buf, group*3+2);     //显示-                 
}


/*---------------------------------------------

          数码管清除显示
          
 -------------------------------------------*/

 void DisplayClr( unsigned char n)
{
    //tick_DisableEvent();  //关闭定时中断，防止同时访问TM1629
  
     Swap2buf( 0 , LED_buf, n*3);       //百位暗
     Swap2buf( 0 , LED_buf, n*3+1);     //十位暗
     Swap2buf( 0 , LED_buf, n*3+2);     //个位暗
    
    //tick_EnableEvent();                      
}


/*---------------------------------------------

          test
          
 -------------------------------------------*/


//delay
void delay(unsigned int t) {
  
  unsigned int i,j;
  
  for(j=0; j< t; j++)
  for( i=0; i< 65500; i++);   //delay
  
}


//  test
void TestTM1629( void )
{


volatile signed char i,n;

for( ; ; ) {
    //i = TM1629_GetKey();
    
    Swap2buf( i,    LED_buf, n ); 
    
    TM1629_WriteDateBuf( LED_buf, 0, 16, 7 );  //LED定时刷新,原始数据LED_buf发送到TM1629的地址0开始的16个数据,亮度7 
   
     delay(3);
    
   // DisplayInt( i, n,0 ); delay(3); 
   // DisplayInt( i, 1 ); delay(1); 
   // DisplayInt( i, 2 ); delay(1); 
   // DisplayInt( i, 3 ); delay(1); 
    }    

              /*
for( i=0; i<100; i++) {
    DisplayHEX( i, 0 ); delay(1); 
    DisplayHEX( i, 1 ); delay(1); 
    DisplayHEX( i, 2 ); delay(1); 
    DisplayHEX( i, 3 ); delay(1); 
   
    }  
        */

}
