
#include "GlobalVar.h"
#include "TM1629C.h"

//int key_scan( void );//�û����庯��,�������ؼ�ֵ

#define  key_scan( ) TM1629_GetKey()

extern struct {
    unsigned char value;    //value2&0xf 
    unsigned char value2;   //TM1629_GetKeyȡ�ļ�ֵ
    unsigned char unread;    //�����Ƿ񱻶�ȡ��?1: ��û�ж�ȡ; 0: �Ѿ���ȡ
    signed int tick;        //����ʱ
} key;   

#define KEY_VALID_TICK ((unsigned int)(1))   //�жϰ�����Ч�����ʱ��(��������ͬ����ֵ����ȷ����Ч),��20��


    /*************************
    key_proc()    
    ��������ʱ����
    ��ʱȡ����״̬������key��ֵ��
    ����key_tick,key_valid�ȱ���
    *************************/
void key_proc(void); 



    /*************************
    get_key()    
    ���°���,��ȡ���������ذ���ֵ,(ֻ�ܶ�ȡһ��,�´η���0)
    û�а��������Ѿ���ȡ,����0
    *************************/
unsigned char get_key(void); 



    /*************************
    key_pressed()    
    ���ص�ǰ���µİ�����ֵ
    û�а�������0
    *************************/
unsigned char key_pressed(void);




 /*************************
    get_key_tick()    
    >0: �а�������,���ذ������µ�ʱ��tick
    <0: û�а�������,���ذ�������ʱ��tick
    *************************/
signed int get_key_tick(void);




    /*************************
    new_key()  
    ���µİ���,�����¼�ֵ
    û���°���,����0
    *************************/
unsigned char new_key(void); 

