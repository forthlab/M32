
#include "AM2320.h"

#include "SDA1.h"
#include "SDA2.h"
#include "us.h"



/*
us.h ��1us�ļ�ʱ��
ver1��  ����ʹ�õ���0.1us�ļ�ʱ��
ver2��  ʹ��1us��ʱ��
Ver2.1  �޸ĺ���
*/


void SDALow( unsigned char channel ){
 
   switch( channel ) {
      case 0: SDA1_ClrVal();  break;
      case 1: SDA2_ClrVal();  break;
      default: break;
      break;   
   }

}



void SDAHigh( unsigned char channel ){   
  
    switch( channel ) {
      case 0: SDA1_SetVal();   break;
      case 1: SDA2_SetVal();  break;
      default: break;
      break;  
    }
 
}
 
void SDAOutput( unsigned char channel){
   
   switch( channel ) {
      case 0: SDA1_SetDir(1);  break;
      case 1: SDA2_SetDir(1);  break;
      default: break;
      break; 
   }
   
}


void SDAInput(unsigned char channel){
  
    switch( channel ) {
      case 0: SDA1_SetDir(0);   break;
      case 1: SDA2_SetDir(0);  break;
      default: break;
      break;  
    }  
  
}

unsigned char  SDAGetc( unsigned char channel){

    switch( channel ) {
      case 0: return SDA1_GetVal();
      case 1: return SDA2_GetVal();
      default: break;
      break;  
    }  

}

/***********************************
    1. ���崫�������ݽӿ�: SDA
    1.2 ����us��ʱ��: us
    2. �����������е�us��ʱ��
    3. ��ȡ���ݴ��󷵻�0,��ȡ������ȷ,����1
    4. ���ݽ����date
***********************************/


unsigned char get_8bit( unsigned char channel, unsigned char *date )
    {
    unsigned char n;        //8bit����
    unsigned char c;        //���ݽ��
    unsigned long t1,t2;    //�ߵ�ƽ�ź������غ��½��ؼ�ʱ
    unsigned long w;        //��ʱ����
    
    c=0;
    
    for(n=0;n<8;n++)
        {
        c=c<<1; 
        
        //���������50us�͵�ƽ,CPU�ȴ��ߵ�ƽ 
        for(w=0;;w++)
            {                             //�ȴ��ߵ�ƽ
            if( w >= 0x0ff0 ) return 0;      //��ʱ��û���ź�,��ʱ����,����0
            if( SDAGetc(channel) != 0 )break;  //���ִ�����SDA�źű���ˣ��˳�
            }  
            
        //����������ߵ�ƽ�����ʱ�䳤�ȴ�������1����0                             
        
        us_GetCounterValue( &t1 );      //�����ߵ�ƽ��������ʱ��
        
        for(w=0;;w++)
            {                             //�ȴ��͵�ƽ
            if( w >= 0x0ff0 ) return 0;   //��ʱ��û���ź�,��ʱ����,����0
            if( SDAGetc(channel) == 0 )break;  //�����ź��½��� 
            }             
                                              
        us_GetCounterValue( &t2 );        //�����ź��½���ʱ��
        
        if( (t2 - t1)>50) c |=1;         //�����źŸߵ�ƽ��ʱ����,ʱ��>50.usΪ1,����Ϊ0
      
        }
        
    *date = c;
    
    return 1;    
    
    }




/*----------------------------------------
AM2320��������ȡ���ݺ���
unsigned char AM2320GetChannel( unsignd channel, signed int wd[], signed int  sd[] )

channel �Ǵ��������

wd[], sd[]������ݽ��,��λ0.1

��������1,���󷵻�0
----------------------------------------*/   
unsigned char AM2320GetChannel( unsigned char channel, signed int *wd, signed int  *sd )
{
unsigned int t1,t2;
unsigned int w;
unsigned char c,c1,chk;
signed int _wd, _sd;

//-----CPU������ʼ�����ź�,���800us�͵�ƽ

us_GetCounterValue( &t1 );

SDALow( channel );    //SDA_ClrVal();     //SDA=0

SDAOutput(channel);   //SDA_SetDir(1);    //SDAΪ���״̬

//SDA���1ms�͵�ƽ
do {
    us_GetCounterValue( &t2 );
    }while((t2-t1)<1200);    

                
// ����SDAΪ����
SDAInput( channel );   //SDA_SetDir(0);  


//����SDA�ǵ͵�ƽ
//CPU�ȴ�SDA������(�ߵ�ƽ)�����������ӹ�SDA����
for(w=0;;w++)                   
    {                           
    if( SDAGetc(channel) != 0 )      //SDA����������(�ߵ�ƽ),����ѭ��
        break; 
    if( w >= 0x0ff0 ) {
        SDAInput( channel );
        return 0; //��ʱ��û���ź�,��ʱ����,����0    
        }
    }
   
//����SDA�Ǹߵ�ƽ   
//�ȴ�SDA�½���(�͵�ƽ)    
for(w=0;;w++)                   
    {                           
    
    if( SDAGetc(channel) == 0 )
        break;                  //SDA�͵�ƽ,��ʾ�������ӹ�SDA���� 
    
    if( w >= 0x0ff0 ) {
        SDAInput( channel );
        return 0; //��ʱ��û���ź�
        }
    
    }        


//-----��������Ӧ
chk=0;

//���������80us�͵�ƽ
for(w=0;;w++)                   //����SDA�ź��ǵ͵�ƽ
    {                           
    if( w >= 0xfff0 ) {
        SDAInput( channel );
        return 0; 
        }
    if( SDAGetc(channel) != 0 )break;//SDA����������(�ߵ�ƽ),��һ��
    }               

//���������80us�ߵ�ƽ
for(w=0;;w++)                   
    {                           
    if( w >= 0xfff0 ) {
        SDAInput( channel );
        return 0; 
        }
    if( SDAGetc(channel) == 0 )break;//SDA�����½���(�͵�ƽ),��һ��
    }      

//-----��ʼ��������

get_8bit( channel, &c );     //ʪ�ȸ�
chk += c;

get_8bit( channel, &c1 );     //ʪ�ȵ�
chk += c1;

if( c>0x7f) _sd = -( (c& 0x7f)*256 + c1); //ʪ�ȵ�
    else _sd = (c& 0x7f)*256 + c1; 
     
get_8bit( channel, &c );     //�¶ȸ�
chk += c;

get_8bit( channel, &c1 );   //�¶ȵ�
chk += c1;

if( c&0x80) _wd = -( (c& 0x7f)*256 + c1); //ʪ�ȵ�
    else _wd = ( (c& 0x7f)*256 + c1); 

get_8bit( channel, &c );     //У��
if ( c != chk ) {
    SDAInput( channel );
    return 0; //У���
    }

*wd = _wd;
*sd = _sd;

SDAInput( channel );
return 1;
}





