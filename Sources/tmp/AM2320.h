
#ifndef __AM2320__
#define __AM2320__



/*----------------------------------------
AM2320��������ȡ���ݺ���
unsigned char AM2320_GetData( signed int * wd, signed int * sd )
wd,sd�����ݽ��
��������1,���󷵻�0
----------------------------------------*/   
//unsigned char AM2320_GetData( signed int * wd, signed int * sd );
unsigned char AM2320GetChannel( unsigned char channel, signed int wd[], signed int  sd[] );


#endif





