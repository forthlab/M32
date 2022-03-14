/* ###################################################################
     Filename    : key_func.c
     键盘处理程序
     
     使用方法：
     1.  编写 int key_scan(void)函数,返回键值
     2. 设置KEY_VALID_TICK 判断按键有效的最短时间(按下多少时间按键确认有效),如20MS     
     2. 定时调用key_proc()  键盘处理后台程序(定时中断)
     3. 可使用的函数:     
        GetKey()    取键盘值,按下后可以读取,再调用返回0   
        Key_Pressed()  有键按下
        CheckKey()    返回现在按下的键
      
      内部变量:
       int key     按键值,程序内部变量
       key_tick     按键计时,按下++,放开--      
         		

** ###################################################################*/


#include"key.h"
#include"IncDec.h"



struct {
    unsigned char value;    //value2&0xf 
    unsigned char value2;   //TM1629_GetKey取的键值
    unsigned char unread;    //按键是否被读取过?1: 还没有读取; 0: 已经读取
    signed int tick;        //健计时
} key = {0, 0, 0, -30000 };







    /*************************
    key_proc()   按键处理定时程序
    定时扫描按键key_scan(),如果没有按键,返回0,否则返回键值val
    状态并放入key键值中
    计算key.tick,key.valid等变量
    *************************/
void key_proc(void) 
{         
        
    key.value2 = key_scan(  );        //调用按键扫描程序,取原始值,高4位是按键,最低位是1/2路控制
    key.value =  key.value2 & 0xfc;    //取按键值
     
    //计算按键tick
    if( key.value != 0 )   //有按键++
        {
        
        INCINT( key.tick );
        
        if( key.tick == KEY_VALID_TICK)  
            key.unread =1;  //按键有效
        
    }
    else //没有按键--    
        {

        key.unread =0;  //按键无效

        DECINT( key.tick ); 

    }                         
}



    /*************************
    get_key()    
    取按键值,返回按键值
    只能读取一次,下次返回0
    *************************/
unsigned char get_key(void) 
    {      
    if( ! key.unread ) 
        return 0;
    
    key.unread =0;  //清除标志
    
    return key.value;   
    }

    /*************************
    new_key()    
    测试是否有新键值(unused),
    如果有新按键,返回键值
    没有新按键,返回0
    *************************/
unsigned char new_key(void) 
    {
          
    if( ! key.unread ) return 0;
    
    return key.value;   
    }


    /*************************
    key_pressed()    
    当前按下键的键值
    *************************/
unsigned char key_pressed(void) 
    { 
    
    //按键时间不够,返回0   
    if( key.tick < KEY_VALID_TICK ) 
        return 0;   
    
    //按键时间够了,返回当前按住的键值
    return key.value;   
    }              


    /*************************
    get_key_tick()    
    >0: 有按键按下,返回按键按下的时间tick
    <0: 没有按键按下,返回按键空闲时间tick
    *************************/
signed int get_key_tick(void) 
  {    
   return key.tick;   
  }


