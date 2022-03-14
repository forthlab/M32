#include "TM1629C.h"


/* ����оƬ�ӿ�CLK,DIO,STB  */

#include "STB.h"
#include "DIO.h"
#include "CLK.h"


//unsigned char key_date[4];    //TM1629����ֵ
unsigned char LED_buf[16];     //TM1629LED��ʾλ����,ֱ�ӷ��͵�TM1929C


//----------------------------------------------
//    TM1629C��ʾ���ݸ�ʽ˵��:  
//    1. ���ݶ�дָ��-д��ʾ����ָ��
//    2. ���õ�ַָ��+4λ��ַ0000
//    3. ������ʾλ����(16byte)
//    4. ��ʾ����ָ��
//----------------------------------------------
//----------------------------------------------
//    TM1629C��ȡ�������ݸ�ʽ:  
//    1. ����������
//    2. ��ȡ4�ֽڼ�������
//----------------------------------------------

//TM1629C���ݶ�дָ��

#define WRITE_DISPLAY_BUF_CMD   0x40      //д��ʾ����ָ��
#define READ_KEY_DATE_CMD       0x42      //����������

//TM1629C��ʾ����ָ��,���3λ������,Ĭ�����

#define DISPLAY_MODE_ON     0x88      //��LED��ʾ+3λ����
#define DISPLAY_MODE_OFF    0x80      //��LED��ʾ

//TM1629C��ַ����ָ��
#define ADDRESS_SET     0xC0      //���õ�ַָ��+4λ��ַ


// ������ֶ�ABCDEFGH, ��λ����,
// 0,1,2,3,4,5,6,7,8,9,   abcdef  �� -     
// [ ��: ��ֵ: 0,���16; '-': ֵ: 0x02,���17 ;  С����:��ֵ 0x01 ]
//const unsigned char Bar[]= { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0, 0x40};
  const unsigned char Bar[]= { 0xfc, 0x60, 0xda, 0xf2, 0x66, 0xb6, 0xbe, 0xe0, 0xfe, 0xf6, 0xee, 0x3e, 0x9c, 0x7a, 0x9e, 0x8e, 0, 0x02 };//3233,���Ų���40,��02
     



/*---------------------------------------------
  ��TM1629��ȡһ���ֽ�
---------------------------------------------*/

unsigned char TM1629_getc(void)
    {
    
    unsigned char c=0;
    unsigned char n;
    
    DIO_SetInput();     //DIO����Ϊ����״̬����ȡTM1629����D0...D7      

    for( n=0;n<8;n++){
    
        c = c>>1;
    
        CLK_PutVal(0);    //CLK=0;
        
        if( DIO_GetVal() !=0 ) 
            c= c|0x80;    //���ݴӸ�λ����,˳��b0...b7
    
        CLK_PutVal(1);    //CLK=1;�����ض�����            
        
        }
      
    return c;
    
    }



/*---------------------------------------------
  дһ���ֽڵ�TM1629
---------------------------------------------*/
void TM1629_putc(unsigned char cc)
{
    unsigned char n,c;
 
    c =cc;              //�������
    
    DIO_SetOutput();    //DIO����Ϊ���״̬��д��TM1629����D0...D7    

    for( n=0; n<8; n++){
    
      CLK_PutVal(0);        //CLK=0;    
    
      DIO_PutVal( (c&0x01)==0? 0:1 );  //дDIO����
    
      CLK_PutVal(1);        //CLK=1;�����ش���д����
    
      c = c>>1;
      }
    
    DIO_SetInput();   //DIO����Ϊ����״̬ 
     
    return;
  
}   




/*---------------------------------------------
  TM1629��ȡ��������  
  unsigned char  TM1629_GetKey( void) 
  buf���水����ֵ
---------------------------------------------*/
unsigned char TM1629_GetKey( void ) 
{

  //unsigned char buf[4];
  unsigned char c,d,k;
  
   STB_PutVal(0);               //STB=0

   TM1629_putc(READ_KEY_DATE_CMD);  //����������ָ��    

   for( k=0; k< 20; k++);    //delay 2us

   c = TM1629_getc();      //ȡ��
   c = (c&0x80)|((c&0x08)<<3);
   d = c;                  //ȡb7,b3                
   
   c = TM1629_getc();      //ȡ��
   c = (c&0x80)|((c&0x08)<<3);
   d = d | (c>>2);         //ȡ(b7,b3)(b7,b3)

   c = TM1629_getc();      //ȡ��
   c = (c&0x80)|((c&0x08)<<3);
   d = d | (c>>4);         //ȡ(b7,b3)(b7,b3)
   
   c = TM1629_getc();      //ȡ��
   c = (c&0x80)|((c&0x08)<<3);
   d = d | (c>>6);         //ȡ(b7,b3)(b7,b3)       

   STB_PutVal(1);               //STB=1     
   
   return d;
}




/*---------------------------------------------
 ������buf[]��n������,д��TM1629оƬ�ĵ�ַaddr(LED��ʾ���ݻ���)
   
 void TM1629_WriteDateBuf( unsigned char * buf, unsigned char n, unsigned char brightness ) 
    buf:  TM1629λ��ʾ����
    addr: ��ʾ�����ַ
    n:    ����
    brightness: ����(0..7)
---------------------------------------------*/
void TM1629_WriteDateBuf( unsigned char * buf, unsigned char addr, unsigned char n, unsigned char brightness ) 
{
   unsigned char c;
   
   STB_PutVal(0);                                 //STB=0  
   TM1629_putc( WRITE_DISPLAY_BUF_CMD );          //������ʾ����ָ��
   STB_PutVal(1);                                 //STB=1
   
   
   STB_PutVal(0);                                 //STB=0   
   TM1629_putc( ADDRESS_SET | (addr & 0x07 ) );   //��ʾ�����ַ   
                                 
   for(c=0; c<n; c++){                            //����n����ʾ����
      TM1629_putc( buf[c] );
   }
   
   STB_PutVal(1);                  //STB=1
   
   
   STB_PutVal(0);                   //STB=0
   TM1629_putc( DISPLAY_MODE_ON | (brightness &0x0f) ); //��������
   
   STB_PutVal(1);                                 //STB=1
   
}



/*-----------------------------------------------------------------------
        void Swap2buf( unsigned char dat, unsigned char *buf, unsigned char n )

            ������c��8����������,���Ŵ���buf��n��0..15���У�ע�⣬һ��16bit��2�ֽڣ�
            ����
            c=1011 0011����buf[0..7]�ĵ�bit5��,���£�
            
                    (n)
      �����         |
      ��             V                     
      a     buf[0] XX1X XXXX    buf[1] XXXXXXXX  
      b     buf[2] XX0X XXXX    buf[3] XXXXXXXX
      c     buf[4] XX1X XXXX    buf[5] XXXXXXXX
      d     buf[6] XX1X XXXX    buf[7] XXXXXXXX
            
      e     buf[8] XX0X XXXX    buf[ ] XXXXXXXX
      f     buf[10]XX0X XXXX    buf[ ] XXXXXXXX
      g     buf[12]XX1X XXXX    buf[ ] XXXXXXXX
      h     buf[14]XX1X XXXX    buf[ ] XXXXXXXX   
            
            ���n>7,��������ֽ�����
------------------------------------------------------------------------*/

void Swap2buf( unsigned char dat, unsigned char *buf, unsigned char n )
{

unsigned char i;
unsigned char c;      //ԭʼ����
unsigned char mask;   //��λ,��0010 0000

      c = dat;
      mask = 0x01 << (n&0x07);   //�����λ�����λ�ĵ�nλ������

      for( i = 0; i<8; i++) {
      
          if( c & 0x80 )          //��������λ
              {
              buf[ i*2 + (n>>3)] |= mask;   //�޸�����λ
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
unsigned char c;      //ԭʼ����
unsigned char mask;   //��λ,��0010 0000

      c = dat;
      mask = 0x01 << (n&0x07);   //�����λ�����λ�ĵ�nλ

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
     ��n���������ʾ����i
     i�ķ�Χ��0...9
    void DisplayDigit( unsigned char i, unsigned n)
        n     ��������0..15
        i     ��ֵ�� ��Χ��0.9 
---------------------------------------------*/ 

void DisplayDigit( unsigned int i, unsigned n)
{
           
    Swap2buf( Bar[i], LED_buf, n );   
                      
}





/*---------------------------------------------
     ��n���������ʾ����i
     i�ķ�Χ��-99...999
    void DisplayInt( signed int i, unsigned n)
        group     ��������0..3
        i     ��ֵ�� ��Χ��-999...999 
        dot:  С���㣬 0��û�У� 1����һλ������
---------------------------------------------*/ 

void DisplayInt2( unsigned char group, signed int i, unsigned char dot )
{
    
    
    if( i> 999) i = 999;
    if( i< -99) i = -99;
     
              //��ʾ����
    if( i>=0){  
          
        if( i>99 )Swap2buf( Bar[i/100],     LED_buf, group*3 ); 
            else  Swap2buf( 0,              LED_buf, group*3 );       //��ʾ��λ
            
        //if( i>9  )
            Swap2buf( Bar[(i/10)%10], LED_buf, group*3+1 ); 
        //    else  Swap2buf( 0,              LED_buf, group*3+1 );   //��ʾʮλ
            
        Swap2buf( Bar[(i)%10],    LED_buf, group*3+2 );               //��ʾ��λ         
       
        }
        
              //��ʾ����-99..-10
    else if( i<-9 )
        {
    
        Swap2buf( 0x02,         LED_buf, group*3);               //��ʾ���ţ�0x02��
            
        Swap2buf( Bar[(-i)/10], LED_buf, group*3+1);             //��ʾʮλ
            
        Swap2buf( Bar[(-i)%10], LED_buf, group*3+2);              //��ʾ��λ
            
        }
        
        /*������ʾ2λ
        
              //��ʾ����-9..-1
    else{
    
        Swap2buf( 0,            LED_buf, group*3);         //��һ������ܰ�
        Swap2buf( 0x40,         LED_buf, group*3+1);         //��ʾ���ţ�0x40��
        Swap2buf( Bar[(-i)%10], LED_buf, group*3+2);         //��ʾ��λ
        } 
          */
   
   //��ʾС����
   if( (dot >0 ) && (dot<=3) ){ 
        Swap2buf_Incert( 0x80,   LED_buf, group*3+(3-dot) );            //����С������ʾ���� 
   }
   
                      
}






/*---------------------------------------------
     ��n���������ʾ����i
     i�ķ�Χ��-99...999
    void DisplayInt( signed int i, unsigned n)
        group     ��������0..3
        i     ��ֵ�� ��Χ��-999...999 
        dot:  С����λ���� 0��û�У� 1��һλС��������
---------------------------------------------*/ 

void DisplayInt( unsigned char group, signed int i, unsigned char dot )
{    
    
    if( i> 999) i = 999;
    if( i< -99) i = -99;
     
              //��ʾ��������
    if( i>=100 ){
        DisplayDigit(  i/100,   group*3   );    //XXX,��ʾ��λ��
        DisplayDigit( (i/10)%10,group*3+1 );    //��ʾʮλ
        DisplayDigit( (i)%10,   group*3+2 );    //��ʾ��λ 
        }
          
    else if( i>=0 ){
        DisplayDigit( 16,       group*3);       // 0XX, ����ʾ��λ,'16#'�ǰ�        
        DisplayDigit( (i/10)%10,group*3+1 );    //��ʾʮλ
        DisplayDigit( (i)%10,   group*3+2);     //��ʾ��λ          
        }
        
              //��ʾ����-1..-9
    else if( i>= -9 )
        {        
        DisplayDigit(16, group*3+0);            //��λ��(16#)
        DisplayDigit(17, group*3+1);            //ʮ��ʾ����(17#) 
        DisplayDigit( (-i)%10, group*3+2 );     //��λ��ʾ����         
        }
        
             //��ʾ����-99..-10
    else 
        {         
        DisplayDigit(17,          group*3+0);      //��λ��ʾ����(17#) 
        DisplayDigit((-i/10)%10,  group*3+1);      //ʮ��ʾ���� 
        DisplayDigit( (-i)%10,    group*3+2 );     //��λ��ʾ����           
        }      
    
    //��ʾС����
    if( (dot >0 ) && (dot<=3) ){ 
        Swap2buf_Incert( 0x80,   LED_buf, group*3+(3-dot) );            //����С������ʾ���� 
    
    }
                          
}



/*---------------------------------------------
    ��n���������ʾ16��������i��
    ÿ�������3��
    i�ķ�Χ��000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     ��������0..3
        i     ��ֵ�� ��Χ��-999...999 
---------------------------------------------*/ 

void DisplayHEX( unsigned char group, unsigned int i)
{
    if( i> 0xfff) i = 0xfff;
      
    Swap2buf( Bar[i>>8],        LED_buf, group*3);     //��ʾ��λ
    Swap2buf( Bar[(i>>4)&0x0f], LED_buf, group*3+1);     //��ʾʮλ
    Swap2buf( Bar[(i>>0)&0x0f], LED_buf, group*3+2);     //��ʾ��λ                 
}




/*---------------------------------------------
    ��n���������ʾ16�������ֵ����i: "A1-"
    
    ÿ�������3��
    i�ķ�Χ��000...fff
    void DisplayHEX( unsigned n, unsigned int i)
        n     ��������0..3
        i     ��ֵ�� ��Χ��-999...999 
---------------------------------------------*/ 

void DisplayHEX_( unsigned char group, unsigned int i)
{
    if( i> 0x0ff) i = 0x0ff;
      
    Swap2buf( Bar[i>>4],      LED_buf, group*3);       //��ʾ��λ
    Swap2buf( Bar[(i)&0x0f],  LED_buf, group*3+1);     //��ʾ��λ
    Swap2buf( 0x02,           LED_buf, group*3+2);     //��ʾ -                 
}




/*---------------------------------------------
    ��n���������ʾ "---"
    
    ÿ�������3��
    
---------------------------------------------*/ 

void Display___( unsigned char group )
{
   
      
    Swap2buf( 0x02,  LED_buf, group*3);       //��ʾ-
    Swap2buf( 0x02,  LED_buf, group*3+1);     //��ʾ-
    Swap2buf( 0x02,  LED_buf, group*3+2);     //��ʾ-                 
}


/*---------------------------------------------

          ����������ʾ
          
 -------------------------------------------*/

 void DisplayClr( unsigned char n)
{
    //tick_DisableEvent();  //�رն�ʱ�жϣ���ֹͬʱ����TM1629
  
     Swap2buf( 0 , LED_buf, n*3);       //��λ��
     Swap2buf( 0 , LED_buf, n*3+1);     //ʮλ��
     Swap2buf( 0 , LED_buf, n*3+2);     //��λ��
    
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
    
    TM1629_WriteDateBuf( LED_buf, 0, 16, 7 );  //LED��ʱˢ��,ԭʼ����LED_buf���͵�TM1629�ĵ�ַ0��ʼ��16������,����7 
   
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
