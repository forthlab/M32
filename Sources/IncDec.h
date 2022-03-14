
#ifndef __INCDEC__
#define __INCDEC__




//数据加减宏定义
#define INCUINT(a)      {if(a<0xffff) a++;}
#define INCUCHAR(a)     {if(a<0xff) a++;}

#define INCINT(a)       {if(a<0) a=0; else if(a<0x7fff) a++;}
#define DECINT(a)       {if(a>0) a=0; else if(a>(-0x7fff))a--;}





#endif





