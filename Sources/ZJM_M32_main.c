
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

    ���Ʋ���
    controlling()
    
    struct ParameterStruct Parameter[4]
      ���ﱣ���Ų�������,���Ʋ�����
    ������Щ����,���ɿ��ƽ��

*******************************/

void controlling( void )
{

unsigned char n;    //4����ʪ�Ȳ������


//�����ֶ���ť,����WOWN��ť3����,�л��ֶ�״̬
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


//== ����4·��ʪ��ֵ,����״̬���ƽ��Working, ����ʱWorkingTick

for( n=0; n< 4; n++){  
  
      /*	V3231
          ����Ҫ���������������ʱ,ͬʱ�����¶Ⱥ�ʪ�ȼ̵���:
		  (1) �¶�<5������
		  (2) �¶���5-25��,ʪ��>=65%,����; ����50%�ָ�����
		  (3) �¶���25-38��,ʪ��>=75%,����; ����50%�ָ�����  */
		
      if( ( n== 0 ) || ( n == 2) )    //�¶�1���¶�2ʱ,���
      {             
          if( Parameter[n].Val <= 50 )                            //(1) �¶�<=5��,���㼤������
              { 
              Parameter[n].SGCC = Parameter[n+1].SGCC =1;         //    �¶�,ʪ��struct�Ĺ���״̬=1
              }
              
              
          else if( Parameter[n].Val <= 250 )                        //(2)�¶�<=25��
              {
              
               if(  Parameter[n+1].Val >= 650 )                    //(2a)���ʪ��>=65%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =1;       //    �¶�,ʪ��struct�Ĺ���״̬=1
                  }
              
               if(  Parameter[n+1].Val <= 500 )                    //(2b) ���ʪ��<=50%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =0;       //     �¶�,ʪ��struct�Ĺ���״̬=0
                  }  
              }
              
          else if( Parameter[n].Val <= 380 )                         //(3) �¶�<=38��
              {
              
               if(  Parameter[n+1].Val >= 750 )                      //(3a)���ʪ��>=75%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =1;        //    �¶�,ʪ��struct�Ĺ���״̬=1
                  }
              
               if(  Parameter[n+1].Val <= 500 )                      //(3b) ʪ��<=50%
                  { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =0;        //    �¶�,ʪ��struct�Ĺ���״̬=0
                  } 
               } 
                  
           else { 
                  Parameter[n].SGCC = Parameter[n+1].SGCC =0;         //(4) >38��, �¶�,ʪ��struct�Ĺ���״̬=0
                  }       
                  
          }                    
  
  
  
    //--  ���ݲ���ֵ, �ֶ����߹�������״̬, ����working״̬������
      if(   ( (Parameter[n].Mode == WARM_MODE ) && ( Parameter[n].Val <= Parameter[n].SetVal ) )     //����ģʽ,����ֵ<= �趨ֵ,working=1
        ||  ( (Parameter[n].Mode != WARM_MODE ) && ( Parameter[n].Val >= Parameter[n].SetVal ) )     //ɢ��ģʽ,����ֵ>= �趨ֵ,working=1

        ||  ( ( Parameter[n].SGCC== 1 ) && (SetupVal_D1 !=0))   //V3232 SGCC ������������,���ò���D1=1
        ||  ( ManualON ) )  //�ֶ���Ч

          {
          Parameter[n].Working = 1;           //����״̬������  
          INCINT( Parameter[n].WorkingTick ); //����״̬��ʱ++ 
          }
          
      else if(  ( (Parameter[n].Mode == WARM_MODE ) && ( Parameter[n].Val >= Parameter[n].ResetVal ) )  //����ģʽ,����ֵ>= �ָ�ֵ,working=0
        ||    ( (Parameter[n].Mode != WARM_MODE ) && ( Parameter[n].Val <= Parameter[n].ResetVal ) ) )  //ɢ��ģʽ,����ֵ<= �ָ�ֵ,working=0
          {
          Parameter[n].Working = 0;  
          DECINT( Parameter[n].WorkingTick );    
          }

     
     //-- ���ؼ��:LoadON,�и���ʱ,�ź�Ϊ�͵�ƽ,û�и���ʱΪ�ߵ�ƽ
     if( LoadVal[n] <= LoadOKVal )    
          Parameter[n].LoadON =1;
     
     else
          Parameter[n].LoadON =0;    
     
        
     
     //--�����쳣�澯: LoadAlarm
     
      if( (Parameter[n].WorkingTick > (TPS*3) ) && ( Parameter[n].LoadON != 1 ) )     //����ʱ��3���,�и���,����
          {
          Parameter[n].LoadAlarm = 1;   
          } 
      
      else                                  //����ʱ��3���,û�и���,����
          {
          Parameter[n].LoadAlarm = 0;   
          }
 
  }//for 4����ʪ��ֵ
                                             
  
  
  
  //== �̵�������
  
  // �¶�1
  // �̵�����LED����
  // ��������Ч && ����״̬,�����ֶ� -> �̵���1����,LED��
  if( (( WENDU1.InvalidCnt <= SensorInvalidCount )  && ( Parameter[0].Working == 1 ) )  || ManualON )
      {
      Relay1_PutVal(1);   //�̵���1����   
      LED1_ON;            //LED��                                                    
      
      if( Parameter[0].LoadAlarm == 1 )  
          { if( (SysTick & 0x10 ) == 0) {LED1_ON;} 
            else {LED1_OFF;}; } //���ظ澯,LED��
      }
  
  else{
      Relay1_PutVal(0);   //�̵����ͷ�
      LED1_OFF;
      }
      
      
  
    //ʪ��1,�̵�����LED����
    
  if( ((WENDU1.InvalidCnt <= SensorInvalidCount )  && ( Parameter[1].Working == 1 ) )  || ManualON )
      {
                          //������2����,ʪ�Ƚ���״̬
      Relay2_PutVal(1);   //�̵�������   
      LED2_ON;            //LED�� 
                                                         
      if( Parameter[1].LoadAlarm == 1 )  
          { 
          //ʪ�ȸ����쳣
          if( (SysTick & 0x10 ) == 0) 
          {
              LED2_ON;//LED2��
              }
          else
          { 
              LED2_OFF;} } //���ظ澯,LED��
      }
  
  else{
                          //������1�쳣,����ʪ�Ȳ�����
      Relay2_PutVal(0);   //�̵����ͷ�
      LED2_OFF;           //LED2��
      }
      
      

    //�¶�2,�̵�����LED����
  if( ( (WENDU2.InvalidCnt <= SensorInvalidCount )  && ( Parameter[2].Working == 1 ) )  || ManualON )
      {
      Relay3_PutVal(1);   //�̵�������   
      LED3_ON;            //LED��                                                    
      if( Parameter[2].LoadAlarm == 1 )  { if( (SysTick & 0x10 ) == 0){ LED3_ON;}else{LED3_OFF; }} //���ظ澯,LED��
      }
  
  else{
      Relay3_PutVal(0);   //�̵����ͷ�
      LED3_OFF;
      }
      


    //ʪ��2,�̵�����LED����
  if( ((WENDU2.InvalidCnt <= SensorInvalidCount )  && ( Parameter[3].Working == 1 )  )  || ManualON )
      {
      Relay4_PutVal(1);   //�̵�������   
      LED4_ON;            //LED��                                                    
      if( Parameter[3].LoadAlarm == 1 ) { if( (SysTick & 0x10 ) == 0) {LED4_ON;}else{ LED4_OFF; }} //���ظ澯,LED��
      }
  
  else{
      Relay4_PutVal(0);   //�̵����ͷ�
      LED4_OFF;
      }
        
        
        
  //�¶ȴ������쳣,�����¶ȸ��ظ澯, ����ʪ�ȸ��ظ澯 => Alarm1�澯�ź����H  
  if(                                       ( WENDU1.InvalidCnt > 10 ) || ( WENDU1.LoadAlarm == 1) || ( SHIDU1.LoadAlarm == 1) 
      || ( ((key.value2 & 0x02) == 0)  && ( ( WENDU2.InvalidCnt > 10 ) || ( WENDU2.LoadAlarm == 1) || ( SHIDU2.LoadAlarm == 1) ) )  )       //key.value2 b1λ��S8�̽�,�ڶ�·���ر���������!
      {
      Alarm_PutVal(1);   //�����
      }
  else{
      Alarm_PutVal(0); 
      }

        
        
}  //���Ʋ��� controlling()





 /********************************
    �û�����Setup����
    0: û��ִ�����
    1: ִ����ϣ����������ò���
    2��ִ����ϣ�û�������²���
********************************/

unsigned char AdjustVal( void )
{

  static unsigned char row =0;   //��ǰ�����Ĳ�������
  
  unsigned char k;                //����ֵ


  if( row >= SETUP_ROW )row =0;

  //20��û�а���,����������˳�
  if( get_key_tick() < (-TPS*20)  )
      {
      row = 0;
      return 2;
      }
    
  //row����һ����Ч��(show=1)����    
  while( SetupOnROM[ row ].Show == 0 )  
      {
      row ++;      
  
      if( row >= SETUP_ROW ){
          row = 0;
          return 2;
      }
      }
   
   
  //��������
             
  
  k = key_pressed();
      
  switch( k ){      
        
      //��OK����,��һ������
      case KEY_OK:
        
                        //�������ʾ������ʾ���Ͳ���ֵ
            DisplayHEX_( 0, SetupOnROM[row].Prompt );   //�������1,��ʾ����ֵ
            DisplayInt( 1, SetupVal[row], 0 );          //�������1,��ʾ����ֵ            
        
            if( get_key() == 0 ) break;            

            row ++;   //��һ��   
            
            //�������һ������,�����˳� 
            if( row >= SETUP_ROW )
                {
                row = 0;
                return 1;
                }        
                            
            break;           
  
      //��+����,val++
      case KEY_UP:
            
            if( SetupOnROM[ row ].Adjust  == 0 ){
                get_key();
                break;  //�����Ե���
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
                        
            //�������ʾ������ʾ���Ͳ���ֵ
            DisplayHEX_( 0, SetupOnROM[row].Prompt );   //�������1,��ʾ����ֵ
            DisplayInt( 1, SetupVal[row], 0 );   //�������1,��ʾ����ֵ
            
            break;
      
      //��-����,val--             
      case KEY_DOWN:
                  
            if( SetupOnROM[ row ].Adjust  == 0 ){
                get_key();
                break;  //�����Ե���
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

            //�������ʾ������ʾ���Ͳ���ֵ
            DisplayHEX_( 0, SetupOnROM[row].Prompt );   //�������1,��ʾ����ֵ
            DisplayInt( 1, SetupVal[row], 0 );   //�������1,��ʾ����ֵ
            
            break;      
      
      
      //����:        
      default :
            
            break;
            
      }//switch

  return 0;   
      
  }








/************** ��flash��ȡSetup����  ****************/

byte ReadSetupFromFlash( void ) 
{
 unsigned char n;
 
 
 //���SetupOnFlash ǰ��8�����ݱ�����������0xFF����
 for( n=0; n< 6; n++){
  
    if( ((unsigned char *)SetupOnFlash)[n] != 0xff)
        break;
 }
 
 //SetupOnFlash����������������
 if( n>=6 )
    {
    SetupVal[0] = 0;        //ģʽ
    SetupVal[1] = 30;       //�¶�1
    SetupVal[2] = 70;
    SetupVal[3] = 40;       //ʪ��
    SetupVal[4] = 80;
    SetupVal[5] = 0;        //����ģʽ
    SetupVal[6] = 0;        //�¶�0ƫ��
    SetupVal[7] = 0;        //ʪ��0ƫ��
    SetupVal[8] = 0;        //�¶�1ƫ��
    SetupVal[9] = 0;        //ʪ��1ƫ��


    IFsh1_SetBlockFlash( (IFsh1_TAddress)(&SetupVal), (IFsh1_TAddress)(SetupOnFlash), sizeof(int) * SETUP_ROW );
    }
    
else{ 
    //��FLASH��ȡsetup���ݱ�RAM
    n = IFsh1_GetBlockFlash( (IFsh1_TAddress)&SetupOnFlash,  &SetupVal, sizeof(int) * SETUP_ROW );
    }

return n;

}



    
    
    
    
    
      
/************** ��Setup����д��flash  ****************/

byte WriteSetupToFlash( void ) 
{
 unsigned char c;
 
 
 //�ϴεĳ���,�������ǲ�����!!!!,������PE��������Ϊ�Ȳ�����д��
c = IFsh1_SetBlockFlash(  (IFsh1_TAddress)(&SetupVal), (IFsh1_TAddress)(&SetupOnFlash), 2 * SETUP_ROW );

return c;

}











/**************  ����Setup����,�޸Ŀ��Ʋ��� *******************/
//�´�Ӧ����Setup[][]2020-1-12

#define Val_A1  SetupVal[0]
#define Val_B1  SetupVal[1]
#define Val_B2  SetupVal[2]
#define Val_C1  SetupVal[3]
#define Val_C2  SetupVal[4]


void ModifyParameterBySetup( void )
{
          unsigned char c;
          signed int hi,low;
  
          //== A1:����ģʽ; 1: ����ģʽ, 0: ɢ��ģʽ
          WENDU1.Mode   = WENDU2.Mode   = Val_A1;        
  
          //== B1,B2: �¶��������Ʋ���,�������ֵ�͵���ֵ
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
              //����ģʽ
              WENDU1.SetVal = WENDU2.SetVal = low *10;      //�¶ȴ���ֵ,setupVal��λ�Ƕ�,WENDU.SetVal��λ��0.1��   
              WENDU1.ResetVal = WENDU2.ResetVal = hi *10;
              }
                   
          else{
              //ɢ��(����)ģʽ 
              WENDU1.SetVal = WENDU2.SetVal = hi *10;       //�¶ȴ���ֵ,setupVal��λ�Ƕ�,WENDU.SetVal��λ��0.1��   
              WENDU1.ResetVal = WENDU2.ResetVal = low*10;
          }       
                  
          //== C1: ʪ���������� 
          SHIDU1.SetVal = SHIDU2.SetVal = ((Val_C1>Val_C2)?Val_C1:Val_C2) *10;       //ʪ�ȴ���ֵ(��ֵ)
          SHIDU1.Mode   = SHIDU2.Mode   = 0;                //����ģʽ,1: ����ģʽ, 0: ɢ��ģʽ(��ʪ)
          
          SHIDU1.ResetVal = SHIDU2.ResetVal =  ((Val_C1>Val_C2)?Val_C2:Val_C1) *10;   //�¶Ȼָ�(3221)
          
}












/*****************************    

          ������ 

*****************************/

void my_main( void )
{
unsigned char c,r,k;

static unsigned char tick;
signed int t, h;    //�¶�,ʪ����ʱֵ

WDog_Clear();

//�ȴ��µ�tick
while( tick == (SysTick&0x0ff) );
tick = SysTick;




//��״̬��
switch( MainStatus )
{

  //����
  case Start:


      //��FLASH��ȡsetup���ݱ�
      ReadSetupFromFlash();
      
      //���ݲ�����,�������Ʋ���
      ModifyParameterBySetup(); 


      //������ʾ�汾��
      DisplayHEX_( 0, VERSION>>8 );      //��ʾ�汾��λ
      DisplayHEX_( 1, VERSION&0xff);     //��ʾ�汾��λ
      
      //������ʾ���Ʋ���
      DisplayInt( 2, SetupVal[1]*10, 2 );      //��ʾ�¶ȿ���ֵ
      DisplayInt( 3, SetupVal[2]*10, 2 );      //��ʾʪ�ȿ���ֵ      
      
      
       //ÿ2���ȡ����������      
       if( SysTick > ( TPS *2) ){
      
          if( (SysTick & 0x7F) == 0 )     
              //AM2320GetChannel(0, &WENDU1.Val + WenduGap, &SHIDU1.Val + ShiduGap);         //M3233,��ȡ����ʪ��+GAP
              AM2320GetChannel(0, &t, &h );         
              WENDU1.Val  = t + WenduGap0;           //M3233,��ȡ����ʪ��+GAP  ,GAP��λ0.1��
              SHIDU1.Val  = h + ShiduGap0*10;        //ʪ�� ShiduGap �ĵ�λ��1%
          
          if( (SysTick & 0x7F) == 0x40 )  
              //AM2320GetChannel(1, &WENDU2.Val + WenduGap, &SHIDU2.Val + ShiduGap);
              AM2320GetChannel(1, &t, &h );         
              WENDU2.Val  = t + WenduGap1;           //M3233,��ȡ����ʪ��+GAP,GAP��λ0.1��
              SHIDU2.Val  = h + ShiduGap1*10;
        }   



      
      //�����ȴ�5���ӣ�
      if( SysTick > (TPS*5))          
          {
          MainStatus = Control;
          }
      break;




  //���Ʋ���
  case Control:
  
      //ÿ2���ȡ����������, SysTick(20ms)
      
      if( (SysTick & 0x7f) == 0 ) {
            
              //c = AM2320GetChannel(0, &WENDU1.Val, &SHIDU1.Val);
              c = AM2320GetChannel(0, &t, &h );         
              WENDU1.Val  = t + WenduGap0;           //M3233,��ȡ����ʪ��+GAP  ,GAP��λ0.1��
              SHIDU1.Val  = h + ShiduGap0*10;        //ʪ�� ShiduGap �ĵ�λ��1%
              
              //����������
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
              WENDU2.Val  = t + WenduGap1;           //M3233,��ȡ����ʪ��+GAP,GAP��λ0.1��
              SHIDU2.Val  = h + ShiduGap1*10;
          
              //����������
              if( c == 0 ){
                  //�������쳣,����++
                  INCUINT( WENDU2.InvalidCnt);  
                  INCUINT( SHIDU2.InvalidCnt);  
                  } 
                  
               else {
                  WENDU2.InvalidCnt =0;
                  SHIDU2.InvalidCnt =0;
                  }
       }
       
          
      
      //��ʾ��ǰ��ʪ��ֵ
      
      if( WENDU1.InvalidCnt <= SensorInvalidCount )
          {
          //�������쳣����<=3��,��ʾ��ʪ��      
          DisplayInt( 0, WENDU1.Val, 2 );
          DisplayInt( 1, SHIDU1.Val, 2 );
          }
      else{
          //�������쳣����>3��,��ʾ"---" 
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
                        
      
      
      //�������OK����,adjust()
      if( new_key() == KEY_OK ) {
          
          get_key();
          
          //�����3��4����ʾ
          DisplayClr( 2 );  
          DisplayClr( 3 ); 
          
          MainStatus = Manual;
      }
      
      
      //10���Ӻ�,�������Ʋ���                                                          
      if( SysTick > ( TPS *10) )  
          controlling();    
      
      break;
  
  //�ֶ����ڲ���    
  case Manual:
  
      c = AdjustVal();
            
      if( c == 1 )        //�޸�Setup�����ɹ�
          {
          
          //setup[ ]���浽FLASH
          r = WriteSetupToFlash(); 
          
          //���ݲ�����,�������Ʋ���
          if( r == 0 ) 
              ModifyParameterBySetup();           
           

          //��ת���Զ�����          
          //MainStatus = Start;                
          MainStatus = Control;
          }
      
      else if( c == 2 ){
                          
          //��FLASH�ָ�setup���ݱ�
          ReadSetupFromFlash();
          
          ModifyParameterBySetup();    
                          
          MainStatus = Start;                   
          }    
          
      break;
        
     
     
     
  //��������   
  default:
      break;
      
  }//switch    
      



}