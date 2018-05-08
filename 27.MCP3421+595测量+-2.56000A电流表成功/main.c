#include <STC12C5204AD.H>
#include <intrins.h>
void init_timer1();	
unsigned int num=0;
unsigned char xdata shi=0,fen=0;
unsigned int xdata miao=0;
long AD_temp; 
#include "mcp3421.H"
#include "595.h"

void main(void)
{			      
		unsigned char	test_data[3]={0x00,0x00,0x00};
		long aa;
		init_timer1();
		WrToMCP3421(SlaveADDR, 0x9f);	   // 1001 1100
while (1)
		{
		LED_595_display();	
		RdFromMCP3421(SlaveADDR, test_data,3);  // 连续读取3个字节数据
		aa=test_data[0]<<8;
		aa=aa+test_data[1];
		aa=aa<<8;
		aa=aa+	test_data[2]; 
		//AD_temp=aa;
		aa+=131072;
		if(aa<=131072)aa=131072-aa;  //大于中间值
		else if(aa>131072)aa-=131072;
		AD_temp =aa*1.945;

	}

}

void init_timer1()//定时器初始化函数
	{
	TMOD = 0x01;
	TH0=(65535-50000)/256;
    TL0=(65535-50000)%256;
    EA = 1;   //开总中断	  
    ET0 = 1;  //定时器0中断打开
    TR0 = 1;  //打开定时器 开始计时

	}
void Timer0Interrupt(void) interrupt 1
{	
	TH0=(65535-50000)/256;
    TL0=(65535-50000)%256;
    num++; 
		if(num==10)
		{	
		 num=0;
		 //kkk=MCPadread();
			miao++;
			if(miao==9999)
			miao=0;
		}
	
}			 