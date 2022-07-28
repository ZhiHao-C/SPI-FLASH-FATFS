#include "stm32f10x.h"                  // Device header
#include "bps_led.h"
#include "bps_usart.h"
#include <stdio.h>
#include "bps_flash.h"
#include "ff.h"	

uint16_t i;
FATFS FSobjec;//定义一个变量配合f_mount使用
FIL fp;//创建一个文件句柄

uint8_t WriteData[]="我的未来不是梦";
UINT bw;
UINT br;
char ReadData[4096]="";

void delay_ms(int n)//延时函数
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
	
	
	printf("这是一个SPI测试\n");
	//挂载文件系统
	res=f_mount(&FSobjec,"1:",1);//这个函数有一个返回值 FRESULT 通过判断返回值可以知道有没有成功或哪里有错。第一个参数为1：代表硬件是SPI_FLASH
	//f_mount(NULL,"1:",1);//取消挂载
	printf("first_f_mount=%d\n",res);
	if(res==FR_NO_FILESYSTEM)
	{
		//第一个参数是设备号，第二个为选择FDISK：支持usb，sd卡，flash。SFD：支持软盘，第三个选择自动配置
		res=f_mkfs ("1:", 0, 0);
		printf("f_mkfs=%d\n",res);
	}
	f_mount(NULL,"1:",1);//取消挂载
	res=f_mount(&FSobjec,"1:",1);//重新挂载
	printf("second_f_mount=%d\n",res);
	
	/*&fp：为模板文件句柄
	打开文件，创建名字为：abc.txt的文件
	FA_OPEN_ALWAYS|FA_READ|FA_WRITE：执行命令为打开一个文件如果没有将创建一个。可读可写
	*/
	res=f_open(&fp,"1:abc.txt",FA_OPEN_ALWAYS|FA_READ|FA_WRITE);
	printf("fopen=%d\n",res);
	
	if(res==FR_OK)//判断上条命令是否成功
	{
		/*
		WriteData：把该数组的值写到flash中
		sizeof(WriteData)：要写入数据的字节数目
		&bw：表示有多少字节被写入
		*/
		res=f_write(&fp,WriteData,sizeof(WriteData),&bw);
		printf("bw= %d\n",bw);
		if(res==FR_OK)//判断上条命令是否成功
		{
			//配置光标位置光标在哪就在哪个位置往后读写，由于前面写入以后光标位置在写入的最后一个字节后
			//所以这里把光标位置重新移到文件开头往后开始读
			//第二个参数设置偏移量
			f_lseek(&fp,0);
			
			/*
			ReadData：读到的数据存到该数组中
			f_size：（第二个参数为要读取多少字节）计算整个文件的大小读取整个文件
			&br：有多少字节被读到
			*/
			res=f_read(&fp,ReadData,f_size(&fp),&br);
			
			if(res==FR_OK)
			{	printf("br= %d\n",br);
				printf("文件内容:%s\n",ReadData);
			}
		}
	}
	f_close(&fp);

	
//	USART_Sendbyte(USART1,0x64);
//	USART_Send_twobyte(USART1,0xff56);
//	USART_Sendarr(USART1,a,10);
//	USART_Sendstr(DEBUG_USARTx,"家和万事兴");
//	printf("串口测试");
//	putchar('p');
	
	
	
}


////接收触发中断函数
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



