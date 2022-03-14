
#include "GlobalVar.h"
#include "TM1629C.h"

//int key_scan( void );//用户定义函数,函数返回键值

#define  key_scan( ) TM1629_GetKey()

extern struct {
    unsigned char value;    //value2&0xf 
    unsigned char value2;   //TM1629_GetKey取的键值
    unsigned char unread;    //按键是否被读取过?1: 还没有读取; 0: 已经读取
    signed int tick;        //健计时
} key;   

#define KEY_VALID_TICK ((unsigned int)(1))   //判断按键有效的最短时间(连续几次同样键值按键确认有效),如20次


    /*************************
    key_proc()    
    按键处理定时程序
    定时取按键状态并放入key键值中
    计算key_tick,key_valid等变量
    *************************/
void key_proc(void); 



    /*************************
    get_key()    
    有新按键,读取按键并返回按键值,(只能读取一次,下次返回0)
    没有按键或者已经读取,返回0
    *************************/
unsigned char get_key(void); 



    /*************************
    key_pressed()    
    返回当前按下的按键的值
    没有按键返回0
    *************************/
unsigned char key_pressed(void);




 /*************************
    get_key_tick()    
    >0: 有按键按下,返回按键按下的时间tick
    <0: 没有按键按下,返回按键空闲时间tick
    *************************/
signed int get_key_tick(void);




    /*************************
    new_key()  
    有新的按键,返回新键值
    没有新按键,返回0
    *************************/
unsigned char new_key(void); 

