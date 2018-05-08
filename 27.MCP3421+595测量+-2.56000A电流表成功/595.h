#ifndef __595_H__
#define __595_H__
#define uint unsigned int
#define uchar unsigned char
/*
unsigned char xdata segmcode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8, 
0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8};
unsigned char xdata bitcode[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; */

unsigned char xdata segmcode[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
0x77,0x7C,0x39,0x5E,0x79,0x71};
unsigned char xdata bitcode[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

sbit ser  = P0^7; //74HC595������������
sbit oe   = P0^6;	//74HC595ʹ�ܣ�Ϊ0ʱ������ݣ�Ϊ1ʱ�ʸ���̬
sbit rclk = P0^5; //74HC595��������
sbit srclk= P0^4;// 74HC595����ʱ��

void Delay_ms(unsigned int t)
{
	 unsigned int x,y;
	 for(x=t;x>0;x--)
	 for(y=80;y>0;y--);//��ʱ��Լ1����
}
void DTDisplayChar(unsigned char segmd,unsigned char bitd )//����ܶ���������λ��
{
	 unsigned char i;
	 unsigned int dat;
	 dat=bitd; 
	 dat=dat<<8|segmd; 				//λ�����ϲ�Ϊһ��int������
	 for(i=0;i<16;i++)				//16λ���ݴӸ�λ��������74HC595
	 {
		 ser=(dat&0x8000)?1:0; 		//�ж����λ��Ϊ��ȡ1��Ϊ��ȡ0
		 srclk=1;					//������������
		 srclk=0;
		 dat<<=1;					//����ȡ��һλ
	 }
rclk=1; 								//74HC595��������
rclk=0; 
}
void LED_595_display()
{
			
			 
			 DTDisplayChar(segmcode[AD_temp%1000000/100000],bitcode[7]);oe=0;Delay_ms(20);oe=1;		
			 DTDisplayChar(segmcode[AD_temp%100000/10000],bitcode[6]);oe=0;Delay_ms(20);oe=1;
			 DTDisplayChar(segmcode[AD_temp%10000/1000],bitcode[5]);oe=0;Delay_ms(20);oe=1;
			 DTDisplayChar(segmcode[AD_temp%1000/100],bitcode[4]);oe=0;Delay_ms(20);oe=1;
			 DTDisplayChar(segmcode[AD_temp%100/10],bitcode[3]);oe=0;Delay_ms(20);oe=1;
			 DTDisplayChar(segmcode[AD_temp%10],bitcode[2]);oe=0;Delay_ms(20);oe=1;

			 //DTDisplayChar(segmcode[miao%100/10],bitcode[1]);oe=0;Delay_ms(20);oe=1;
			// DTDisplayChar(segmcode[miao%10],bitcode[0]);oe=0;Delay_ms(20);oe=1;	 
		
}

#endif

