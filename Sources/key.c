/* ###################################################################
     Filename    : key_func.c
     ���̴������
     
     ʹ�÷�����
     1.  ��д int key_scan(void)����,���ؼ�ֵ
     2. ����KEY_VALID_TICK �жϰ�����Ч�����ʱ��(���¶���ʱ�䰴��ȷ����Ч),��20MS     
     2. ��ʱ����key_proc()  ���̴����̨����(��ʱ�ж�)
     3. ��ʹ�õĺ���:     
        GetKey()    ȡ����ֵ,���º���Զ�ȡ,�ٵ��÷���0   
        Key_Pressed()  �м�����
        CheckKey()    �������ڰ��µļ�
      
      �ڲ�����:
       int key     ����ֵ,�����ڲ�����
       key_tick     ������ʱ,����++,�ſ�--      
         		

** ###################################################################*/


#include"key.h"
#include"IncDec.h"



struct {
    unsigned char value;    //value2&0xf 
    unsigned char value2;   //TM1629_GetKeyȡ�ļ�ֵ
    unsigned char unread;    //�����Ƿ񱻶�ȡ��?1: ��û�ж�ȡ; 0: �Ѿ���ȡ
    signed int tick;        //����ʱ
} key = {0, 0, 0, -30000 };







    /*************************
    key_proc()   ��������ʱ����
    ��ʱɨ�谴��key_scan(),���û�а���,����0,���򷵻ؼ�ֵval
    ״̬������key��ֵ��
    ����key.tick,key.valid�ȱ���
    *************************/
void key_proc(void) 
{         
        
    key.value2 = key_scan(  );        //���ð���ɨ�����,ȡԭʼֵ,��4λ�ǰ���,���λ��1/2·����
    key.value =  key.value2 & 0xfc;    //ȡ����ֵ
     
    //���㰴��tick
    if( key.value != 0 )   //�а���++
        {
        
        INCINT( key.tick );
        
        if( key.tick == KEY_VALID_TICK)  
            key.unread =1;  //������Ч
        
    }
    else //û�а���--    
        {

        key.unread =0;  //������Ч

        DECINT( key.tick ); 

    }                         
}



    /*************************
    get_key()    
    ȡ����ֵ,���ذ���ֵ
    ֻ�ܶ�ȡһ��,�´η���0
    *************************/
unsigned char get_key(void) 
    {      
    if( ! key.unread ) 
        return 0;
    
    key.unread =0;  //�����־
    
    return key.value;   
    }

    /*************************
    new_key()    
    �����Ƿ����¼�ֵ(unused),
    ������°���,���ؼ�ֵ
    û���°���,����0
    *************************/
unsigned char new_key(void) 
    {
          
    if( ! key.unread ) return 0;
    
    return key.value;   
    }


    /*************************
    key_pressed()    
    ��ǰ���¼��ļ�ֵ
    *************************/
unsigned char key_pressed(void) 
    { 
    
    //����ʱ�䲻��,����0   
    if( key.tick < KEY_VALID_TICK ) 
        return 0;   
    
    //����ʱ�乻��,���ص�ǰ��ס�ļ�ֵ
    return key.value;   
    }              


    /*************************
    get_key_tick()    
    >0: �а�������,���ذ������µ�ʱ��tick
    <0: û�а�������,���ذ�������ʱ��tick
    *************************/
signed int get_key_tick(void) 
  {    
   return key.tick;   
  }


