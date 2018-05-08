#ifndef __MCP3421_H__
#define __MCP3421_H__
#define uint unsigned int
#define uchar unsigned char

#define SlaveADDR  0xD0	  // �ӻ���ַ
#define tt  26			        // ��ʱ5uSʱ�䳣����֤1.8V������ѹ������д��

sbit SCL=P2^7; //��MCP3421��ʱ�ӡ����ݡ�
sbit SDA=P2^6;

void delay (unsigned char t);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void  IIC_NoAck(void);
bit IIC_GetACK() ;
unsigned char IIC_SendByte(unsigned char Data);
unsigned char IIC_RecByte(void) ;   



void IIC_Start(void)     // ��ʼλ    
{
		SDA=1;
		SCL=1;
		delay(tt);		 // ����4.7uS���ϣ�TSU.STA��  
		SDA=0;
		delay(tt);		 // ����4uS���ϣ�THD.STA��
		SCL = 0;		 // ������һ��ʱ�Ӵӵ͵��߱仯�Ĳ�����
}
void IIC_Stop(void)      // ֹͣλ
{
		SDA=0;
		SCL=1;        	 
		delay(tt);		 // ����4.7uS���ϣ�TSU.STO��
		SDA=1;        	  
		delay(tt);		 //	����4.7uS���ϣ�TBUF��
}
void IIC_Ack(void)       // ����Ӧ��λ 
{
		SDA=0;        
		delay(tt);		 // ����0.2uS���ϣ�TSU.DAT)
		SCL=1;        
		delay(tt);		 // ����4.0uS����(THIGH)
		SCL=0;
		delay(tt);		 // ����4.7uS����(TLOW),�������Բ�Ҫ			
}
void  IIC_NoAck(void)    // ��������Ӧ��λ 
{
        SDA=1;        
		delay(tt);		   
        SCL=1;        
		delay(tt);		   
        SCL=0;
		delay(tt);		 		
}
bit IIC_GetACK()       // ��ȡ�ӻ�Ӧ���źţ�����Ϊ0ʱ�յ�ACK,����Ϊ1ʱû�յ�ACK 
{
		bit ErrorBit;
		SDA = 1; 
	    delay(tt);		// ����0.2uS���ϣ�TSU.DAT)
		SCL = 1;
		delay(tt);		// ����4.0uS����(THIGH)
		ErrorBit=SDA;
		SCL = 0;         
		delay(tt);		// ����4.7uS����(TLOW),�������Բ�Ҫ
		return ErrorBit;
} 
//*********************************************************************
// �������������豸����豸���͸�һ�ֽ�
// ����ֵ�� 0->�ɹ�  1->ʧ��
//*********************************************************************/
unsigned char IIC_SendByte(unsigned char Data)    
{
	unsigned char i;               // λ������ 
	for (i=0;i<8;i++)	   // д��ʱ����ʱ���½���ͬ������
	{
		if (Data & 0x80) 
			SDA = 1;
        else 
			SDA = 0;
        delay(tt); 		  // ����0.2uS���ϣ�TSU.DAT)
        SCL = 1;
		delay(tt); 		  // ����4.0uS����(THIGH)
		SCL = 0;
		delay(tt);		  // ����4.7uS����(TLOW)
        Data <<= 1;
	 }	
	 return IIC_GetACK();  
}

//*********************************************************************
// �������������豸����豸��ȡһ���ֽ�
// ����ֵ��  �������ֽ�  
// *********************************************************************/
unsigned char IIC_RecByte(void)       // ���յ��ֽڵ�����,�����ظ��ֽ�ֵ
{
	unsigned char i,rbyte=0;
	SDA=1;
	for(i=0;i<8;i++)		  // ����ʱ����ʱ��������ͬ������
	{
		SCL=0;
		delay(tt);			  // ����4.7uS����(TLOW)
		SCL=1;
		delay(tt);			  // ����4.0uS����(THIGH)
		if(SDA) rbyte|=(0x80>>i); 
	}
	SCL=0;
	return rbyte; 
} 

bit  WrToMCP3421(unsigned char SlaveAddress, unsigned char aa);
bit  RdFromMCP3421(unsigned char SlaveAddress, unsigned char *pbuf,unsigned char Len);

void delay (unsigned char t)	// IIC������ʱ���� ������Ҫ���ܸ�
{
	while(--t);
}
//******************************************************************
// ������������MCP3421д��һ���ֽ����ݣ������޸�MCP3421�ڲ�����           
// ����˵����SlaveAddress  Ҫд��Ĵ�����Ӳ����ַ
//           1101 A2 A1 A0 R/W [A2:A0]��MCP3421��оƬ�ڲ���ַ��1101 000*=0xD0��
//	         R/W�Ƕ�/дѡ��λ��0Ϊд������1Ϊ������
//			 ���ﺯ���ڲ��Ѷ�R/W���˴����ⲿ�̶�Ϊ0��1������ 
//			 aa  д�������              
// ����˵����0->�ɹ�    1->ʧ��
//****************************************************************** 
bit  WrToMCP3421(unsigned char SlaveAddress, unsigned char aa)
{
	unsigned char i=0;
	IIC_Start();		// ��������
	if(IIC_SendByte(SlaveAddress&0xfe)==1) return 1;  // д����Ѱ�����Ӧ����
	if(IIC_SendByte(aa)==1) return 1;        // ����1���ֽ�����
	IIC_Stop();			// ��������
	return 0;			// д�뵥�ֽڳɹ�
} 

//******************************************************************
// ����������������������������ȡLen���ֽڵ�pbuf��
// ����˵����SlaveAddress  Ҫ��ȡ�Ĵ�����Ӳ����ַ
//           1101 A2 A1 A0 R/W [A2:A0]��MCP3421��оƬ�ڲ���ַ��1101 000*=0xD0��
//	                       R/W�Ƕ�/дѡ��λ��0Ϊд������1Ϊ������
//						   ���ﺯ���ڲ��Ѷ�R/W���˴����ⲿ�̶�Ϊ0��1������ 
//			 pbuf          ָ�����ݻ�������ָ��
//			 Len           ��ȡ���ݳ���              
// ����˵����0->�ɹ�    1->ʧ��
// ******************************************************************
bit  RdFromMCP3421(unsigned char SlaveAddress, unsigned char *pbuf,unsigned char Len)
{
	unsigned char i=0;                                                  
	IIC_Start();				                      // ������������
	if(IIC_SendByte(SlaveAddress|0x01)==1) return 1;  // ������
	for(i=0;i<Len-1;i++)
	{
		*pbuf++=IIC_RecByte();	   // ����1���ֽ�����
		IIC_Ack();				   // Ӧ��0������������Ҫ����һ�ֽ�����
	}	
	*pbuf=IIC_RecByte();		   // �������1���ֽ�����
    IIC_NoAck();				   // Ӧ��1�������������ٶ�ȡ����
    IIC_Stop();					   // ��������
	return 0;					   // ��ȡ���ֽڳɹ�
} 


#endif

