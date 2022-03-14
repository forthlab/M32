
#include "AM2320.h"

#include "SDA.h"
#include "us.h"



/***********************************
    1. ���崫�������ݽӿ�: SDA
    1.2 ����us��ʱ��: us
    2. �����������е�us��ʱ��
    3. ��ȡ���ݴ��󷵻�0,��ȡ������ȷ,����1
    4. ���ݽ����date
***********************************/


unsigned char get_8bit( unsigned char *date )
    {
    unsigned char n;        //8bit����
    unsigned char c;        //���ݽ��
    unsigned long t1,t2;    //�ߵ�ƽ�ź������غ��½��ؼ�ʱ
    unsigned long w;        //��ʱ����
    
    c=0;
    
    for(n=0;n<8;n++)
        {
        c=c<<1; 
        
        //��������Ӧ50us�͵�ƽ,�ȴ��ߵ�ƽ 
        for(w=0;;w++)
            {                            //�ȴ��ߵ�ƽ
            if( w >= 0xfff0 ) return 0;  //��ʱ��û���ź�,��ʱ����,����0
            if(SDA_GetVal() != 0 )break; //�����ź�������
            }  
            
        //����������ߵ�ƽ�����ʱ�䳤�ȴ�������1����0                             
        
        us_GetCounterValue( &t1 );      //�����ߵ�ƽ��������ʱ��
        
        for(w=0;;w++)
            {                             //�ȴ��͵�ƽ
            if( w >= 0xfff0 ) return 0;   //��ʱ��û���ź�,��ʱ����,����0
            if(SDA_GetVal() == 0 )break;  //�����ź��½��� 
            }             
                                              
        us_GetCounterValue( &t2 );        //�����ź��½���ʱ��
        
        if( (t2 - t1)>50) c |=1;         //�����źŸߵ�ƽ��ʱ����,ʱ��>50.usΪ1,����Ϊ0
      
        }
        
    *date = c;
    
    return 1;    
    
    }


/*----------------------------------------
AM2320��������ȡ���ݺ���
unsigned char AM2320_GetData( signed int * wd, signed int * sd )
wd,sd�����ݽ��
��������1,���󷵻�0
----------------------------------------*/   
unsigned char AM2320_GetData( signed int * wd, signed int * sd )
{
unsigned int t1,t2;
unsigned int w;
unsigned char c,c1,chk;

//-----CPU������ʼ�����ź�,���800us�͵�ƽ

us_GetCounterValue( &t1 );

SDA_ClrVal();     //SDA=0

SDA_SetDir(1);    //SDAΪ���״̬

//SDA���8000us�͵�ƽ
do {
    us_GetCounterValue( &t2 );
    }while((t2-t1)<1000);    

                
// ����SDAΪ����
SDA_SetDir(0);  


//return 0;  ////////////////////////


//����SDA�ź��ǵ͵�ƽ
//�ȴ�SDA������(�ߵ�ƽ)
for(w=0;;w++)                   
    {                           
    if(SDA_GetVal() != 0 )      //SDA����������(�ߵ�ƽ),����ѭ��
        break; 
    if( w >= 200 )  return 0; //��ʱ��û���ź�,��ʱ����,����0    
    }
   
//����SDA�Ǹߵ�ƽ   
//�ȴ�SDA�½���(�͵�ƽ)    
for(w=0;;w++)                   
    {                           
    
    if(SDA_GetVal() == 0 )
        break;                  //SDA�͵�ƽ,��ʾ�������ӹ�SDA���� 
    
    if( w >= 200 ) return 0; //��ʱ��û���ź�
    
    }        


//-----��������Ӧ
chk=0;

//��������Ӧ,SDA��80us�͵�ƽ
for(w=0;;w++)                   //����SDA�ź��ǵ͵�ƽ
    {                           
    if( w >= 0xfff0 ) return 0; 
    if(SDA_GetVal() != 0 )break;//SDA����������(�ߵ�ƽ),��һ��
    }               

//��������Ӧ80us�ߵ�ƽ
for(w=0;;w++)                   
    {                           
    if( w >= 0xfff0 ) return 0; 
    if(SDA_GetVal() == 0 )break;//SDA�����½���(�͵�ƽ),��һ��
    }      

//-----��ʼ��������

get_8bit( &c );     //ʪ�ȸ�
chk += c;

get_8bit( &c );     //ʪ�ȵ�
chk += c1;

if( c>0x7f) *sd = -( (c& 0x7f)*256 + c1); //ʪ�ȵ�
    else *sd = (c& 0x7f)*256 + c1; 
     
get_8bit( &c );     //�¶ȸ�
chk += c;

get_8bit( &c );   //�¶ȵ�
chk += c1;

if( c&0x80) *wd = -( (c& 0x7f)*256 + c1); //ʪ�ȵ�
    else *wd = ( (c& 0x7f)*256 + c1); 

get_8bit( &c );     //У��
if ( c != chk ) return 0; //У���

return 1;
}







