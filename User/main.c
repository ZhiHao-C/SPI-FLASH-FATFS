#include "stm32f10x.h"                  // Device header
#include "bps_led.h"
#include "bps_usart.h"
#include <stdio.h>
#include "bps_flash.h"
#include "ff.h"	

uint16_t i;
FATFS FSobjec;//����һ���������f_mountʹ��
FIL fp;//����һ���ļ����

uint8_t WriteData[]="�ҵ�δ��������";
UINT bw;
UINT br;
char ReadData[4096]="";

void delay_ms(int n)//��ʱ����
{
	int i;
	while (n--)
	{
		for(i=0;i<5520;i++);
	}
}



int main()
{
	FRESULT res=FR_DISK_ERR;
	USART_Config();
	
//	
//	SPI_FLASH_Init();
//	for(i=0;i<2;i++)
//	{
//		Sector_erase(i*4096);
//		WaitnoBUSY();
//	}
	
	
	printf("����һ��SPI����\n");
	//�����ļ�ϵͳ
	res=f_mount(&FSobjec,"1:",1);//���������һ������ֵ FRESULT ͨ���жϷ���ֵ����֪����û�гɹ��������д���һ������Ϊ1������Ӳ����SPI_FLASH
	//f_mount(NULL,"1:",1);//ȡ������
	printf("first_f_mount=%d\n",res);
	if(res==FR_NO_FILESYSTEM)
	{
		//��һ���������豸�ţ��ڶ���Ϊѡ��FDISK��֧��usb��sd����flash��SFD��֧�����̣�������ѡ���Զ�����
		res=f_mkfs ("1:", 0, 0);
		printf("f_mkfs=%d\n",res);
	}
	f_mount(NULL,"1:",1);//ȡ������
	res=f_mount(&FSobjec,"1:",1);//���¹���
	printf("second_f_mount=%d\n",res);
	
	/*&fp��Ϊģ���ļ����
	���ļ�����������Ϊ��abc.txt���ļ�
	FA_OPEN_ALWAYS|FA_READ|FA_WRITE��ִ������Ϊ��һ���ļ����û�н�����һ�����ɶ���д
	*/
	res=f_open(&fp,"1:abc.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
	printf("fopen=%d\n",res);
	
	if(res==FR_OK)//�ж����������Ƿ�ɹ�
	{
		/*
		WriteData���Ѹ������ֵд��flash��
		sizeof(WriteData)��Ҫд�����ݵ��ֽ���Ŀ
		&bw����ʾ�ж����ֽڱ�д��
		*/
		res=f_write(&fp,WriteData,sizeof(WriteData),&bw);
		printf("bw= %d\n",bw);
		if(res==FR_OK)//�ж����������Ƿ�ɹ�
		{
			//���ù��λ�ù�����ľ����ĸ�λ�������д������ǰ��д���Ժ���λ����д������һ���ֽں�
			//��������ѹ��λ�������Ƶ��ļ���ͷ����ʼ��
			//�ڶ�����������ƫ����
			f_lseek(&fp,0);
			
			/*
			ReadData�����������ݴ浽��������
			f_size�����ڶ�������ΪҪ��ȡ�����ֽڣ����������ļ��Ĵ�С��ȡ�����ļ�
			&br���ж����ֽڱ�����
			*/
			res=f_read(&fp,ReadData,f_size(&fp),&br);
			
			if(res==FR_OK)
			{	printf("br= %d\n",br);
				printf("�ļ�����:%s\n",ReadData);
			}
		}
	}
	f_close(&fp);

	
//	USART_Sendbyte(USART1,0x64);
//	USART_Send_twobyte(USART1,0xff56);
//	USART_Sendarr(USART1,a,10);
//	USART_Sendstr(DEBUG_USARTx,"�Һ�������");
//	printf("���ڲ���");
//	putchar('p');
	
	
	
}


////���մ����жϺ���
//void DEBUG_USART_IRQHandler(void)
//{
//	uint8_t temp;
//	if((USART1->SR&0x20)==0x20)
//	{
//		temp=USART_ReceiveData(USART1);
//		USART_SendData(USART1,temp);
//		while (!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
//	}
//}	



