#ifndef __MCP3421_H__
#define __MCP3421_H__
#define uint unsigned int
#define uchar unsigned char

#define SlaveADDR  0xD0	  // 从机地址
#define tt  26			        // 延时5uS时间常数保证1.8V工作电压正常读写。

sbit SCL=P2^7; //置MCP3421的时钟、数据。
sbit SDA=P2^6;

void delay (unsigned char t);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void  IIC_NoAck(void);
bit IIC_GetACK() ;
unsigned char IIC_SendByte(unsigned char Data);
unsigned char IIC_RecByte(void) ;   



void IIC_Start(void)     // 起始位    
{
		SDA=1;
		SCL=1;
		delay(tt);		 // 保持4.7uS以上（TSU.STA）  
		SDA=0;
		delay(tt);		 // 保持4uS以上（THD.STA）
		SCL = 0;		 // 方便下一次时钟从低到高变化的操作。
}
void IIC_Stop(void)      // 停止位
{
		SDA=0;
		SCL=1;        	 
		delay(tt);		 // 保持4.7uS以上（TSU.STO）
		SDA=1;        	  
		delay(tt);		 //	保持4.7uS以上（TBUF）
}
void IIC_Ack(void)       // 主机应答位 
{
		SDA=0;        
		delay(tt);		 // 保持0.2uS以上（TSU.DAT)
		SCL=1;        
		delay(tt);		 // 保持4.0uS以上(THIGH)
		SCL=0;
		delay(tt);		 // 保持4.7uS以上(TLOW),此语句可以不要			
}
void  IIC_NoAck(void)    // 主机反向应答位 
{
        SDA=1;        
		delay(tt);		   
        SCL=1;        
		delay(tt);		   
        SCL=0;
		delay(tt);		 		
}
bit IIC_GetACK()       // 获取从机应答信号，返回为0时收到ACK,返回为1时没收到ACK 
{
		bit ErrorBit;
		SDA = 1; 
	    delay(tt);		// 保持0.2uS以上（TSU.DAT)
		SCL = 1;
		delay(tt);		// 保持4.0uS以上(THIGH)
		ErrorBit=SDA;
		SCL = 0;         
		delay(tt);		// 保持4.7uS以上(TLOW),此语句可以不要
		return ErrorBit;
} 
//*********************************************************************
// 功能描述：主设备向从设备发送个一字节
// 返回值： 0->成功  1->失败
//*********************************************************************/
unsigned char IIC_SendByte(unsigned char Data)    
{
	unsigned char i;               // 位数控制 
	for (i=0;i<8;i++)	   // 写入时是用时钟下降沿同步数据
	{
		if (Data & 0x80) 
			SDA = 1;
        else 
			SDA = 0;
        delay(tt); 		  // 保持0.2uS以上（TSU.DAT)
        SCL = 1;
		delay(tt); 		  // 保持4.0uS以上(THIGH)
		SCL = 0;
		delay(tt);		  // 保持4.7uS以上(TLOW)
        Data <<= 1;
	 }	
	 return IIC_GetACK();  
}

//*********************************************************************
// 功能描述：主设备向从设备读取一个字节
// 返回值：  读到的字节  
// *********************************************************************/
unsigned char IIC_RecByte(void)       // 接收单字节的数据,并返回该字节值
{
	unsigned char i,rbyte=0;
	SDA=1;
	for(i=0;i<8;i++)		  // 读出时是用时钟上升沿同步数据
	{
		SCL=0;
		delay(tt);			  // 保持4.7uS以上(TLOW)
		SCL=1;
		delay(tt);			  // 保持4.0uS以上(THIGH)
		if(SDA) rbyte|=(0x80>>i); 
	}
	SCL=0;
	return rbyte; 
} 

bit  WrToMCP3421(unsigned char SlaveAddress, unsigned char aa);
bit  RdFromMCP3421(unsigned char SlaveAddress, unsigned char *pbuf,unsigned char Len);

void delay (unsigned char t)	// IIC传输延时函数 ，很重要不能改
{
	while(--t);
}
//******************************************************************
// 功能描述：向MCP3421写入一个字节数据，用于修改MCP3421内部配置           
// 参数说明：SlaveAddress  要写入的从器件硬件地址
//           1101 A2 A1 A0 R/W [A2:A0]是MCP3421的芯片内部地址（1101 000*=0xD0）
//	         R/W是读/写选择位，0为写操作，1为读操作
//			 这里函数内部已对R/W作了处理，外部固定为0或1都可以 
//			 aa  写入的数据              
// 返回说明：0->成功    1->失败
//****************************************************************** 
bit  WrToMCP3421(unsigned char SlaveAddress, unsigned char aa)
{
	unsigned char i=0;
	IIC_Start();		// 启动总线
	if(IIC_SendByte(SlaveAddress&0xfe)==1) return 1;  // 写命令，已包含有应答函数
	if(IIC_SendByte(aa)==1) return 1;        // 接收1个字节数据
	IIC_Stop();			// 结束总线
	return 0;			// 写入单字节成功
} 

//******************************************************************
// 功能描述：连续读操作，连续读取Len个字节到pbuf中
// 参数说明：SlaveAddress  要读取的从器件硬件地址
//           1101 A2 A1 A0 R/W [A2:A0]是MCP3421的芯片内部地址（1101 000*=0xD0）
//	                       R/W是读/写选择位，0为写操作，1为读操作
//						   这里函数内部已对R/W作了处理，外部固定为0或1都可以 
//			 pbuf          指向数据缓冲区的指针
//			 Len           读取数据长度              
// 返回说明：0->成功    1->失败
// ******************************************************************
bit  RdFromMCP3421(unsigned char SlaveAddress, unsigned char *pbuf,unsigned char Len)
{
	unsigned char i=0;                                                  
	IIC_Start();				                      // 从新启动总线
	if(IIC_SendByte(SlaveAddress|0x01)==1) return 1;  // 读命令
	for(i=0;i<Len-1;i++)
	{
		*pbuf++=IIC_RecByte();	   // 接收1个字节数据
		IIC_Ack();				   // 应答0，告诉器件还要读下一字节数据
	}	
	*pbuf=IIC_RecByte();		   // 接收最后1个字节数据
    IIC_NoAck();				   // 应答1，告诉器件不再读取数据
    IIC_Stop();					   // 结束总线
	return 0;					   // 读取多字节成功
} 


#endif

