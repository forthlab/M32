
#include "AM2320.h"

#include "SDA.h"
#include "us.h"



/***********************************
    1. 定义传感器数据接口: SDA
    1.2 定义us定时器: us
    2. 定义自由运行的us定时器
    3. 读取数据错误返回0,读取数据正确,返回1
    4. 数据结果在date
***********************************/


unsigned char get_8bit( unsigned char *date )
    {
    unsigned char n;        //8bit计数
    unsigned char c;        //数据结果
    unsigned long t1,t2;    //高电平信号上升沿和下降沿计时
    unsigned long w;        //临时变量
    
    c=0;
    
    for(n=0;n<8;n++)
        {
        c=c<<1; 
        
        //传感器响应50us低电平,等待高电平 
        for(w=0;;w++)
            {                            //等待高电平
            if( w >= 0xfff0 ) return 0;  //长时间没有信号,超时错误,返回0
            if(SDA_GetVal() != 0 )break; //出现信号上升沿
            }  
            
        //传感器输出高电平，这段时间长度代表数据1或者0                             
        
        us_GetCounterValue( &t1 );      //测量高电平的上升沿时刻
        
        for(w=0;;w++)
            {                             //等待低电平
            if( w >= 0xfff0 ) return 0;   //长时间没有信号,超时错误,返回0
            if(SDA_GetVal() == 0 )break;  //出现信号下降沿 
            }             
                                              
        us_GetCounterValue( &t2 );        //测量信号下降沿时刻
        
        if( (t2 - t1)>50) c |=1;         //计算信号高电平的时间间隔,时间>50.us为1,否则为0
      
        }
        
    *date = c;
    
    return 1;    
    
    }


/*----------------------------------------
AM2320传感器读取数据函数
unsigned char AM2320_GetData( signed int * wd, signed int * sd )
wd,sd是数据结果
正常返回1,错误返回0
----------------------------------------*/   
unsigned char AM2320_GetData( signed int * wd, signed int * sd )
{
unsigned int t1,t2;
unsigned int w;
unsigned char c,c1,chk;

//-----CPU发送起始触发信号,输出800us低电平

us_GetCounterValue( &t1 );

SDA_ClrVal();     //SDA=0

SDA_SetDir(1);    //SDA为输出状态

//SDA输出8000us低电平
do {
    us_GetCounterValue( &t2 );
    }while((t2-t1)<1000);    

                
// 设置SDA为输入
SDA_SetDir(0);  


//return 0;  ////////////////////////


//现在SDA信号是低电平
//等待SDA上升沿(高电平)
for(w=0;;w++)                   
    {                           
    if(SDA_GetVal() != 0 )      //SDA出现上升沿(高电平),跳出循环
        break; 
    if( w >= 200 )  return 0; //长时间没有信号,超时错误,返回0    
    }
   
//现在SDA是高电平   
//等待SDA下降沿(低电平)    
for(w=0;;w++)                   
    {                           
    
    if(SDA_GetVal() == 0 )
        break;                  //SDA低电平,表示传感器接管SDA总线 
    
    if( w >= 200 ) return 0; //长时间没有信号
    
    }        


//-----传感器响应
chk=0;

//传感器响应,SDA有80us低电平
for(w=0;;w++)                   //现在SDA信号是低电平
    {                           
    if( w >= 0xfff0 ) return 0; 
    if(SDA_GetVal() != 0 )break;//SDA出现上升沿(高电平),下一步
    }               

//传感器响应80us高电平
for(w=0;;w++)                   
    {                           
    if( w >= 0xfff0 ) return 0; 
    if(SDA_GetVal() == 0 )break;//SDA出现下降沿(低电平),下一步
    }      

//-----开始接收数据

get_8bit( &c );     //湿度高
chk += c;

get_8bit( &c );     //湿度低
chk += c1;

if( c>0x7f) *sd = -( (c& 0x7f)*256 + c1); //湿度低
    else *sd = (c& 0x7f)*256 + c1; 
     
get_8bit( &c );     //温度高
chk += c;

get_8bit( &c );   //温度低
chk += c1;

if( c&0x80) *wd = -( (c& 0x7f)*256 + c1); //湿度低
    else *wd = ( (c& 0x7f)*256 + c1); 

get_8bit( &c );     //校验
if ( c != chk ) return 0; //校验错

return 1;
}







