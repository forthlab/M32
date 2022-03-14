
#include "IFsh1.h"

#include "GlobalVar.h"
#include "AM2320.h"
#include "key.h"
#include "WDog.h"
#include "Relay1.h"
#include "Relay2.h"
#include "Relay3.h"
#include "Relay4.h"
#include "Alarm.h"

#include "TM1629C.h"

#include "IncDec.h"
#include "math.h"


enum { Start =0, Control, Manual } MainStatus =0;


/*******************************

    控制策略
    controlling()
    
    struct ParameterStruct Parameter[4]
      这里保存着测量参数,控制参数等
    根据这些参数,生成控制结果

*******************************/

void controlling( void )
{

unsigned char n;    //4个温湿度参数序号


//按下手动按钮,或者WOWN按钮3秒钟,切换手动状态
if( ( ( new_key() == KEY_DOWN ) || ( ( new_key() == KEY_MANUAL )) ) 
    && ( ( get_key_tick() > (TPS*3) ) ))
{
    get_key();  
    if( ManualON ){
        ManualON =0;
        MANUAL_LED_OFF;
        }
    else{
        ManualON=1;
        MANUAL_LED_ON;
        }
}                


//== 根据4路温湿度值,生成状态控制结果Working, 并计时WorkingTick

for( n=0; n< 4; n++){  
  
      /*	V3231
          国网要求满足下面的条件时,同时启动温度和湿度继电器:
		  (1) 温度<5度启动
		  (2) 温度在5-25度,湿度>=65%,启动; 低于50%恢复正常
		  (3) 温度在25-38度,湿度>=75%,启动; 低于50%恢复正常  */
		
      if( ( n== 0 ) || ( n == 2) )    //温度1和温度2时,检测
      {             
          if( Parameter[n].Val <= 50 )                            //(1) 温度<=5度,满足激活条件
              { 
              Parameter[n].SGCC = Parameter[n+1].SGCC =1;         //    温度,湿度struct的国网状态=1
              }
              
              
          else if( Parameter[n].Val <= 250 )                        //(2)温度<=25度
              {
              
               if(  Parameter[n+1].Val >= 650 )                    //(2a)如果湿度>=65%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =1;       //    温度,湿度struct的国网状态=1
                  }
              
               if(  Parameter[n+1].Val <= 500 )                    //(2b) 如果湿度<=50%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =0;       //     温度,湿度struct的国网状态=0
                  }  
              }
              
          else if( Parameter[n].Val <= 380 )                         //(3) 温度<=38度
              {
              
               if(  Parameter[n+1].Val >= 750 )                      //(3a)如果湿度>=75%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =1;        //    温度,湿度struct的国网状态=1
                  }
              
               if(  Parameter[n+1].Val <= 500 )                      //(3b) 湿度<=50%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =0;        //    温度,湿度struct的国网状态=0
                  } 
               } 
                  
           else { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =0;         //(4) >38度, 温度,湿度struct的国网状态=0
                  }       
                  
          }                    
  
  
  
    //--  根据测量值, 手动或者国网触发状态, 控制working状态并计数
      if(   ( (Parameter[n].Mode == WARM_MODE ) && ( Parameter[n].Val <= Parameter[n].SetVal ) )     //加热模式,测量值<= 设定值,working=1
        ||  ( (Parameter[n].Mode != WARM_MODE ) && ( Parameter[n].Val >= Parameter[n].SetVal ) )     //散热模式,测量值>= 设定值,working=1

        ||  ( ( Parameter[n].SGCC== 1 ) && (SetupVal_D1 !=0))   //V3232 SGCC 国网条件激活,配置参数D1=1
        ||  ( ManualON ) )  //手动有效

          {
          Parameter[n].Working = 1;           //控制状态被激活  
          INCINT( Parameter[n].WorkingTick ); //激活状态计时++ 
          }
          
      else if(  ( (Parameter[n].Mode == WARM_MODE ) && ( Parameter[n].Val >= Parameter[n].ResetVal ) )  //加热模式,测量值>= 恢复值,working=0
        ||    ( (Parameter[n].Mode != WARM_MODE ) && ( Parameter[n].Val <= Parameter[n].ResetVal ) ) )  //散热模式,测量值<= 恢复值,working=0
          {
          Parameter[n].Working = 0;  
          DECINT( Parameter[n].WorkingTick );    
          }

     
     //-- 负载检测:LoadON,有负载时,信号为低电平,没有负载时为高电平
     if( LoadVal[n] <= LoadOKVal )    
          Parameter[n].LoadON =1;
     
     else
          Parameter[n].LoadON =0;    
     
        
     
     //--负载异常告警: LoadAlarm
     
      if( (Parameter[n].WorkingTick > (TPS*3) ) && ( Parameter[n].LoadON != 1 ) )     //工作时间3秒后,有负载,正常
          {
          Parameter[n].LoadAlarm = 1;   
          } 
      
      else                                  //工作时间3秒后,没有负载,报警
          {
          Parameter[n].LoadAlarm = 0;   
          }
 
  }//for 4个温湿度值
                                             
  
  
  
  //== 继电器控制
  
  // 温度1
  // 继电器和LED控制
  // 传感器有效 && 工作状态,或者手动 -> 继电器1吸合,LED亮
  if( (( WENDU1.InvalidCnt <= SensorInvalidCount )  && ( Parameter[0].Working == 1 ) )  || ManualON )
      {
      Relay1_PutVal(1);   //继电器1吸合   
      LED1_ON;            //LED亮                                                    
      
      if( Parameter[0].LoadAlarm == 1 )  
          { if( (SysTick & 0x10 ) == 0) {LED1_ON;} 
            else {LED1_OFF;}; } //负载告警,LED闪
      }
  
  else{
      Relay1_PutVal(0);   //继电器释放
      LED1_OFF;
      }
      
      
  
    //湿度1,继电器和LED控制
    
  if( ((WENDU1.InvalidCnt <= SensorInvalidCount )  && ( Parameter[1].Working == 1 ) )  || ManualON )
      {
                          //传感器2正常,湿度进入状态
      Relay2_PutVal(1);   //继电器吸合   
      LED2_ON;            //LED亮 
                                                         
      if( Parameter[1].LoadAlarm == 1 )  
          { 
          //湿度负载异常
          if( (SysTick & 0x10 ) == 0) 
          {
              LED2_ON;//LED2亮
              }
          else
          { 
              LED2_OFF;} } //负载告警,LED按
      }
  
  else{
                          //传感器1异常,或者湿度不工作
      Relay2_PutVal(0);   //继电器释放
      LED2_OFF;           //LED2灭
      }
      
      

    //温度2,继电器和LED控制
  if( ( (WENDU2.InvalidCnt <= SensorInvalidCount )  && ( Parameter[2].Working == 1 ) )  || ManualON )
      {
      Relay3_PutVal(1);   //继电器吸合   
      LED3_ON;            //LED亮                                                    
      if( Parameter[2].LoadAlarm == 1 )  { if( (SysTick & 0x10 ) == 0){ LED3_ON;}else{LED3_OFF; }} //负载告警,LED闪
      }
  
  else{
      Relay3_PutVal(0);   //继电器释放
      LED3_OFF;
      }
      


    //湿度2,继电器和LED控制
  if( ((WENDU2.InvalidCnt <= SensorInvalidCount )  && ( Parameter[3].Working == 1 )  )  || ManualON )
      {
      Relay4_PutVal(1);   //继电器吸合   
      LED4_ON;            //LED亮                                                    
      if( Parameter[3].LoadAlarm == 1 ) { if( (SysTick & 0x10 ) == 0) {LED4_ON;}else{ LED4_OFF; }} //负载告警,LED闪
      }
  
  else{
      Relay4_PutVal(0);   //继电器释放
      LED4_OFF;
      }
        
        
        
  //温度传感器异常,或者温度负载告警, 或者湿度负载告警 => Alarm1告警信号输出H  
  if(                                       ( WENDU1.InvalidCnt > 10 ) || ( WENDU1.LoadAlarm == 1) || ( SHIDU1.LoadAlarm == 1) 
      || ( ((key.value2 & 0x02) == 0)  && ( ( WENDU2.InvalidCnt > 10 ) || ( WENDU2.LoadAlarm == 1) || ( SHIDU2.LoadAlarm == 1) ) )  )       //key.value2 b1位是S8短接,第二路负载报警被屏蔽!
      {
      Alarm_PutVal(1);   //输出高
      }
  else{
      Alarm_PutVal(0); 
      }

        
        
}  //控制策略 controlling()





 /********************************
    用户调节Setup参数
    0: 没有执行完毕
    1: 执行完毕，生成新设置参数
    2：执行完毕，没有生成新参数
********************************/

unsigned char AdjustVal( void )
{

  static unsigned char row =0;   //当前调整的参数表行
  
  unsigned char k;                //按键值


  if( row >= SETUP_ROW )row =0;

  //20秒没有按键,不保存参数退出
  if( get_key_tick() < (-TPS*20)  )
      {
      row = 0;
      return 2;
      }
    
  //row到下一个有效的(show=1)的行    
  while( SetupOnROM[ row ].Show == 0 )  
      {
      row ++;      
  
      if( row >= SETUP_ROW ){
          row = 0;
          return 2;
      }
      }
   
   
  //按键处理
             
  
  k = key_pressed();
      
  switch( k ){      
        
      //按OK按键,下一个参数
      case KEY_OK:
        
                        //数码管显示参数提示符和参数值
            DisplayHEX_( 0, SetupOnROM[row].Prompt );   //数码管组1,显示参数值
            DisplayInt( 1, SetupVal[row], 0 );          //数码管组1,显示参数值            
        
            if( get_key() == 0 ) break;            

            row ++;   //下一行   
            
            //到达最后一个参数,保存退出 
            if( row >= SETUP_ROW )
                {
                row = 0;
                return 1;
                }        
                            
            break;           
  
      //按+按键,val++
      case KEY_UP:
            
            if( SetupOnROM[ row ].Adjust  == 0 ){
                get_key();
                break;  //不可以调整
                }
            
            if( SetupVal[row] <  SetupOnROM[row].Max ) 
                {            
                if(   ( get_key() != 0 )                
                    ||( (get_key_tick() < ( TPS*2 ) ) && ( (get_key_tick() & 0x0f)== 0 ) ) 
                    ||( (get_key_tick() >=( TPS*2 ) ) && ( (get_key_tick() & 0x01)== 0 ) ))
                      {
                        SetupVal[row]++ ;
                      } 
                }
                        
            //数码管显示参数提示符和参数值
            DisplayHEX_( 0, SetupOnROM[row].Prompt );   //数码管组1,显示参数值
            DisplayInt( 1, SetupVal[row], 0 );   //数码管组1,显示参数值
            
            break;
      
      //按-按键,val--             
      case KEY_DOWN:
                  
            if( SetupOnROM[ row ].Adjust  == 0 ){
                get_key();
                break;  //不可以调整
            }
            
            if( SetupVal[row] >  SetupOnROM[row].Min ) 
                {
              
                if(   ( get_key() != 0 )                
                    ||( (get_key_tick() < ( TPS*2 ) ) && ( (get_key_tick() & 0x0f)== 0 ) ) 
                    ||( (get_key_tick() >=( TPS*2 ) ) && ( (get_key_tick() & 0x01)== 0 ) ))
                      {
                        SetupVal[row]-- ;
                      } 
                }

            //数码管显示参数提示符和参数值
            DisplayHEX_( 0, SetupOnROM[row].Prompt );   //数码管组1,显示参数值
            DisplayInt( 1, SetupVal[row], 0 );   //数码管组1,显示参数值
            
            break;      
      
      
      //其他:        
      default :
            
            break;
            
      }//switch

  return 0;   
      
  }








/************** 从flash读取Setup数据  ****************/

byte ReadSetupFromFlash( void ) 
{
 unsigned char n;
 
 
 //检测SetupOnFlash 前面8个数据被擦除（都是0xFF）？
 for( n=0; n< 6; n++){
  
    if( ((unsigned char *)SetupOnFlash)[n] != 0xff)
        break;
 }
 
 //SetupOnFlash被擦除，重置数据
 if( n>=6 )
    {
    SetupVal[0] = 0;        //模式
    SetupVal[1] = 30;       //温度1
    SetupVal[2] = 70;
    SetupVal[3] = 40;       //湿度
    SetupVal[4] = 80;
    SetupVal[5] = 0;        //国网模式
    SetupVal[6] = 0;        //温度偏差
    SetupVal[7] = 0;        //湿度偏差

    IFsh1_SetBlockFlash( (IFsh1_TAddress)(&SetupVal), (IFsh1_TAddress)(SetupOnFlash), sizeof(int) * SETUP_ROW );
    }
    
else{ 
    //从FLASH读取setup数据表到RAM
    n = IFsh1_GetBlockFlash( (IFsh1_TAddress)&SetupOnFlash,  &SetupVal, sizeof(int) * SETUP_ROW );
    }

return n;

}



    
    
    
    
    
      
/************** 将Setup数据写入flash  ****************/

byte WriteSetupToFlash( void ) 
{
 unsigned char c;
 
 
 //上次的程序,这里忘记擦除了!!!!,所以在PE里面设置为先擦除在写入
c = IFsh1_SetBlockFlash(  (IFsh1_TAddress)(&SetupVal), (IFsh1_TAddress)(&SetupOnFlash), 2 * SETUP_ROW );

return c;

}











/**************  根据Setup参数,修改控制参数 *******************/
//下次应该用Setup[][]2020-1-12

#define Val_A1  SetupVal[0]
#define Val_B1  SetupVal[1]
#define Val_B2  SetupVal[2]
#define Val_C1  SetupVal[3]
#define Val_C2  SetupVal[4]


void ModifyParameterBySetup( void )
{
          unsigned char c;
          signed int hi,low;
  
          //== A1:控制模式; 1: 加热模式, 0: 散热模式
          WENDU1.Mode   = WENDU2.Mode   = Val_A1;        
  
          //== B1,B2: 温度启动控制参数,计算高温值和低温值
          if( Val_B1 > Val_B2 ) {
              hi =  Val_B1;
              low = Val_B2;
              }
          else if( Val_B1 == Val_B2 ) {
              hi =  Val_B1;
              low = Val_B1-1;
          }    
          else          
          {
              hi =  Val_B2;
              low = Val_B1;
          }    
          
                
          if(  WENDU1.Mode ){       
              //加热模式
              WENDU1.SetVal = WENDU2.SetVal = low *10;      //温度触发值,setupVal单位是度,WENDU.SetVal单位是0.1度   
              WENDU1.ResetVal = WENDU2.ResetVal = hi *10;
              }
                   
          else{
              //散热(降温)模式 
              WENDU1.SetVal = WENDU2.SetVal = hi *10;       //温度触发值,setupVal单位是度,WENDU.SetVal单位是0.1度   
              WENDU1.ResetVal = WENDU2.ResetVal = low*10;
          }       
                  
       
          /*
          WENDU1.SetVal = WENDU2.SetVal = SetupVal[1] *10;    //温度触发值,setupVal单位是度,WENDU.SetVal单位是0.1度          
          
          // 计算温度回差
          c = abs(SetupVal[1]/8)*10;      
          if( c < 50) c=50;
          
          // 计算温度恢复值
          WENDU1.ResetVal = WENDU2.ResetVal = WENDU1.Mode ? (WENDU1.SetVal+c):(WENDU1.SetVal-c);     //温度控制释放值: 1:加热模式
          */
          
          //== C1: 湿度启动参数 
          SHIDU1.SetVal = SHIDU2.SetVal = ((Val_C1>Val_C2)?Val_C1:Val_C2) *10;       //湿度触发值(高值)
          SHIDU1.Mode   = SHIDU2.Mode   = 0;                //控制模式,1: 加热模式, 0: 散热模式(除湿)
          
          SHIDU1.ResetVal = SHIDU2.ResetVal =  ((Val_C1>Val_C2)?Val_C2:Val_C1) *10;   //温度恢复(3221)
          /***计算湿度恢复值
          c = abs(Val_C1 /8) *10; 
          if( c < 50) c=50;           
          SHIDU1.ResetVal = SHIDU2.ResetVal = SHIDU1.Mode ? (SHIDU1.SetVal+c):(SHIDU1.SetVal-c);     //温度控制释放值: 1:加热模式
          */
          
}












/*****************************    

          主程序 

*****************************/

void my_main( void )
{
unsigned char c,r,k;

static unsigned char tick;
signed int t, h;    //温度,湿度临时值

WDog_Clear();

//等待新的tick
while( tick == (SysTick&0x0ff) );
tick = SysTick;




//主状态机
switch( MainStatus )
{

  //启动
  case Start:


      //从FLASH读取setup数据表
      ReadSetupFromFlash();
      
      //根据参数表,调整控制参数
      ModifyParameterBySetup(); 


      //开机显示版本号
      DisplayHEX_( 0, VERSION>>8 );      //显示版本高位
      DisplayHEX_( 1, VERSION&0xff);     //显示版本低位
      
      //开机显示控制参数
      DisplayInt( 2, SetupVal[1]*10, 2 );      //显示温度控制值
      DisplayInt( 3, SetupVal[2]*10, 2 );      //显示湿度控制值      
      
      
       //每2秒读取传感器数据      
       if( SysTick > ( TPS *2) ){
      
          if( (SysTick & 0x7F) == 0 )     
              //AM2320GetChannel(0, &WENDU1.Val + WenduGap, &SHIDU1.Val + ShiduGap);         //M3233,读取的温湿度+GAP
              AM2320GetChannel(0, &t, &h );         
              WENDU1.Val  = t + WenduGap;           //M3233,读取的温湿度+GAP  ,GAP单位0.1度
              SHIDU1.Val  = h + ShiduGap*10;        //湿度 ShiduGap 的单位是1%
          
          if( (SysTick & 0x7F) == 0x40 )  
              //AM2320GetChannel(1, &WENDU2.Val + WenduGap, &SHIDU2.Val + ShiduGap);
              AM2320GetChannel(1, &t, &h );         
              WENDU2.Val  = t + WenduGap;           //M3233,读取的温湿度+GAP,GAP单位0.1度
              SHIDU2.Val  = h + ShiduGap*10;
        }   



      
      //开机等待5秒钟；
      if( SysTick > (TPS*5))          
          {
          MainStatus = Control;
          }
      break;




  //控制策略
  case Control:
  
      //每2秒读取传感器数据, SysTick(20ms)
      
      if( (SysTick & 0x7f) == 0 ) {
            
              //c = AM2320GetChannel(0, &WENDU1.Val, &SHIDU1.Val);
              c = AM2320GetChannel(0, &t, &h );         
              WENDU1.Val  = t + WenduGap;           //M3233,读取的温湿度+GAP  ,GAP单位0.1度
              SHIDU1.Val  = h + ShiduGap*10;        //湿度 ShiduGap 的单位是1%
              
              //传感器故障
              if( c == 0 ){
                  INCUINT( WENDU1.InvalidCnt);
                  INCUINT( SHIDU1.InvalidCnt);
                  }
                   
               else {
                  WENDU1.InvalidCnt =0;
                  SHIDU1.InvalidCnt =0;}
      }
          
      if( (SysTick & 0x7f) == 0x40 ) {
           
              //c = AM2320GetChannel(1, &WENDU2.Val, &SHIDU2.Val);
              c = AM2320GetChannel(1, &t, &h );         
              WENDU2.Val  = t + WenduGap;           //M3233,读取的温湿度+GAP,GAP单位0.1度
              SHIDU2.Val  = h + ShiduGap*10;
          
              //传感器故障
              if( c == 0 ){
                  //传感器异常,计数++
                  INCUINT( WENDU2.InvalidCnt);  
                  INCUINT( SHIDU2.InvalidCnt);  
                  } 
                  
               else {
                  WENDU2.InvalidCnt =0;
                  SHIDU2.InvalidCnt =0;
                  }
       }
       
          
      
      //显示当前温湿度值
      
      if( WENDU1.InvalidCnt <= SensorInvalidCount )
          {
          //传感器异常次数<=3次,显示温湿度      
          DisplayInt( 0, WENDU1.Val, 2 );
          DisplayInt( 1, SHIDU1.Val, 2 );
          }
      else{
          //传感器异常次数>3次,显示"---" 
          Display___(0);
          Display___(1);
          }
          
      if( WENDU2.InvalidCnt <= SensorInvalidCount ){    
          DisplayInt( 2, WENDU2.Val, 2 );
          DisplayInt( 3, SHIDU2.Val, 2 );                           
          }
      else{
          Display___(2);
          Display___(3);      
          }
                        
      
      
      //如果按下OK按键,adjust()
      if( new_key() == KEY_OK ) {
          
          get_key();
          
          //数码管3，4不显示
          DisplayClr( 2 );  
          DisplayClr( 3 ); 
          
          MainStatus = Manual;
      }
      
      
      //10秒钟后,启动控制策略                                                          
      if( SysTick > ( TPS *10) )  
          controlling();    
      
      break;
  
  //手动调节参数    
  case Manual:
  
      c = AdjustVal();
            
      if( c == 1 )        //修改Setup参数成功
          {
          
          //setup[ ]保存到FLASH
          r = WriteSetupToFlash(); 
          
          //根据参数表,调整控制参数
          if( r == 0 ) 
              ModifyParameterBySetup();           
           

          //跳转到自动控制          
          //MainStatus = Start;                
          MainStatus = Control;
          }
      
      else if( c == 2 ){
                          
          //从FLASH恢复setup数据表
          ReadSetupFromFlash();
          
          ModifyParameterBySetup();    
                          
          MainStatus = Start;                   
          }    
          
      break;
        
     
     
     
  //其他按键   
  default:
      break;
      
  }//switch    
      



}