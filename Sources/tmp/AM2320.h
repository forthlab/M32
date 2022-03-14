
#ifndef __AM2320__
#define __AM2320__



/*----------------------------------------
AM2320传感器读取数据函数
unsigned char AM2320_GetData( signed int * wd, signed int * sd )
wd,sd是数据结果
正常返回1,错误返回0
----------------------------------------*/   
//unsigned char AM2320_GetData( signed int * wd, signed int * sd );
unsigned char AM2320GetChannel( unsigned char channel, signed int wd[], signed int  sd[] );


#endif





