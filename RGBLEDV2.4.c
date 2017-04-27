#include "STC12C5A60S2.h"
#include <string.h>
//#include <stdio.h>
//#include <intrins.h>
/*****************************************************
 * 版本：	V2.1
 * NAME_CN:	LED红绿双色点阵模块
 * 控制IO：	P15-电源595-14
 * 		P16-电源595-11
 * 		P17-电源595-12
 * 		——————————————
 * 		P05-绿色595-11
 * 		P06-绿色595-12
 * 		P07-绿色595-14
 * 		——————————————
 * 		P27-红色595-11
 * 		P26-红色595-12
 * 		P25-红色595-14
 * 晶振：	22.1184MHz
 * 指令：	[0][S][G]——默认主程序[十位][个位]		[a]——Ascii测试
 * 		[w][Ascii]——自定义显示字符			[f]——闪灯
 * 		[l]——左转转向灯					[r]——右转转向灯
 * 最后修改：	2015年12月7日02时45分43秒|	2015年12月22日20时48分53秒
 *****************************************************/
#define CSLED P1		//模块片选GR
#define uchar unsigned char
#define uint unsigned int

void Delay(unsigned int t);
void dly(unsigned int t);
uchar __code vdata[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};	//行选，电源扫描数据
uchar tmp[8];	//显示缓冲数组
__xdata uchar wd[252];	//自定义显示字符缓存数组

uchar __code hzsrkl[4][8]=	//生日快乐
{
	{0x50,0x7e,0x90,0x10,0x3c,0x10,0x10,0xff},
	{0x7e,0x42,0x42,0x7e,0x42,0x42,0x42,0x7e},
	{0x08,0x48,0xde,0xea,0xdf,0x4c,0x54,0x63},
	{0x1c,0x60,0x48,0x7e,0x2c,0x4a,0x1b,0x08}
};

uchar __code shuzi[10][8]=	//定义0-9共10个数字的点阵数据
{
	{0x00,0x3C,0x24,0x24,0x24,0x24,0x24,0x3C},	//0
	{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04},	//1
	{0x00,0x3C,0x04,0x04,0x3C,0x20,0x20,0x3C},	//2
	{0x00,0x3C,0x04,0x04,0x3C,0x04,0x04,0x3C},	//3
	{0x00,0x24,0x24,0x24,0x3C,0x04,0x04,0x04},	//4
	{0x00,0x3C,0x20,0x20,0x3C,0x04,0x04,0x3C},	//5
	{0x00,0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C},	//6
	{0x00,0x3C,0x04,0x04,0x04,0x04,0x04,0x04},	//7
	{0x00,0x3C,0x24,0x24,0x3C,0x24,0x24,0x3C},	//8
	{0x00,0x3C,0x24,0x24,0x3C,0x04,0x04,0x3C}	//9
};

uchar __code dxzm[96][8]=	//大写英文字母D-Z
{
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	// 0x20
	{0x18,0x3c,0x3c,0x3c,0x18,0x00,0x18,0x18},	//0x21!
	{0x18,0x3c,0x7e,0xff,0x18,0x18,0x18,0x18},//"
	{0x24,0x24,0x7e,0x24,0x24,0x7e,0x24,0x24},	//#
	{0x18,0x3c,0x7e,0xff,0x18,0x18,0x18,0x18},//$
	{0x00,0x42,0xa4,0x48,0x12,0x25,0x42,0x00},//%
	{0x18,0x24,0x24,0x18,0x29,0x46,0x46,0x39},//&
	{0x18,0x18,0x18,0x18,0xff,0x7e,0x3c,0x18},//'
	{0x10,0x30,0x70,0xff,0xff,0x70,0x30,0x10},//(左键头
	{0x08,0x0c,0x0e,0xff,0xff,0x0e,0x0c,0x08},//)右箭头
	{0x00,0x24,0x18,0x7e,0x18,0x24,0x00,0x00},	//*	0x2a
	{0x00,0x10,0x10,0x7c,0x10,0x10,0x00,0x00},	//+
	{0x00,0x00,0x00,0x00,0x30,0x30,0x10,0x20},	//,
	{0x00,0x00,0x00,0x7c,0x00,0x00,0x00,0x00},	//-
	{0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60},	//.
	{0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80},	//
	{0x00,0x3C,0x24,0x24,0x24,0x24,0x24,0x3C},	//0
	{0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04},	//1
	{0x00,0x3C,0x04,0x04,0x3C,0x20,0x20,0x3C},	//2
	{0x00,0x3C,0x04,0x04,0x3C,0x04,0x04,0x3C},	//3
	{0x00,0x24,0x24,0x24,0x3C,0x04,0x04,0x04},	//4
	{0x00,0x3C,0x20,0x20,0x3C,0x04,0x04,0x3C},	//5
	{0x00,0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C},	//6
	{0x00,0x3C,0x04,0x04,0x04,0x04,0x04,0x04},	//7
	{0x00,0x3C,0x24,0x24,0x3C,0x24,0x24,0x3C},	//8
	{0x00,0x3C,0x24,0x24,0x3C,0x04,0x04,0x3C},	//9
	{0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00},//:
	{0x00,0x30,0x30,0x00,0x30,0x30,0x10,0x20},//,
	{0x07,0x18,0x60,0x80,0x60,0x18,0x07,0x00},//<
	{0x00,0x00,0x7e,0x00,0x00,0x7e,0x00,0x00},	//=
	{0xe0,0x18,0x06,0x01,0x06,0x18,0xe0,0x00},//>
	{0x3c,0x42,0x42,0x24,0x08,0x08,0x00,0x08},//?
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},	//@
	{0x18,0x24,0x24,0x42,0x7e,0x81,0x81,0x81},	//A	34
	{0x78,0x44,0x44,0x78,0x44,0x42,0x42,0x7c},	//B1	35
	{0x3c,0x42,0x80,0x80,0x80,0x80,0x42,0x3c},	//C2	36
	{0x78,0x44,0x42,0x42,0x42,0x42,0x44,0x78},	//D3	37
	{0x7c,0x40,0x40,0x78,0x40,0x40,0x40,0x7c},	//E4
	{0x7c,0x40,0x40,0x78,0x40,0x40,0x40,0x40},	//F5
	{0x38,0x44,0x80,0x80,0x8e,0x86,0x46,0x3a},	//G6	40
	{0x42,0x42,0x42,0x7e,0x42,0x42,0x42,0x42},	//H7
	{0x38,0x10,0x10,0x10,0x10,0x10,0x10,0x38},	//I8
	{0x1c,0x08,0x08,0x08,0x08,0x08,0x48,0x30},	//J9
	{0x44,0x48,0x50,0x60,0x60,0x50,0x48,0x44},	//K10
	{0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3c},	//L11
	{0x44,0x6c,0x6c,0x6c,0x92,0x92,0x92,0x92},	//M12
	{0x44,0x64,0x74,0x74,0x5c,0x5c,0x4c,0x44},	//N13
	{0x38,0x44,0x82,0x82,0x82,0x82,0x44,0x38},	//O14
	{0x7c,0x42,0x42,0x7c,0x40,0x40,0x40,0x40},	//P15
	{0x38,0x44,0x82,0x82,0x82,0x8a,0x44,0x3a},	//Q16	50
	{0x7c,0x42,0x42,0x7c,0x44,0x44,0x42,0x42},	//R17
	{0x3c,0x42,0x40,0x30,0x0c,0x02,0x42,0x3c},	//S18
	{0xfe,0x10,0x10,0x10,0x10,0x10,0x10,0x10},	//T19
	{0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3c},	//U20
	{0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},	//V21
	{0x81,0x81,0x81,0x99,0x5a,0x5a,0x24,0x24},	//W22
	{0x42,0x42,0x24,0x18,0x18,0x24,0x42,0x42},	//X23
	{0x82,0x82,0x44,0x28,0x10,0x10,0x10,0x10},	//Y24
	{0x7e,0x02,0x04,0x08,0x10,0x20,0x40,0x7e},	//Z25
	{0x0e,0x08,0x08,0x08,0x08,0x08,0x08,0x0e},	//方括号左[
	{0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01},	//反斜杠\\.
	{0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x70},	//]
	{0x10,0x28,0x44,0x00,0x00,0x00,0x00,0x00},	//^
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e},	//_
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},	//`
	
	{0x00,0x00,0x00,0x38,0x48,0x48,0x48,0x34},	//a	
	{0x00,0x40,0x40,0x70,0x48,0x48,0x48,0xb0},	//b1	
	{0x00,0x00,0x00,0x30,0x40,0x40,0x40,0x30},	//c2	
	{0x08,0x08,0x08,0x38,0x48,0x48,0x48,0x34},	//d3	
	{0x00,0x00,0x00,0x30,0x48,0x78,0x40,0x38},	//e4
	{0x08,0x14,0x10,0x7c,0x10,0x10,0x50,0x20},	//f5
	{0x00,0x18,0x24,0x24,0x1c,0x04,0x24,0x18},	//g6	40
	{0x00,0x20,0x20,0x20,0x38,0x24,0x24,0x24},	//h7
	{0x00,0x10,0x00,0x30,0x10,0x10,0x10,0x18},	//i8
	{0x00,0x08,0x00,0x18,0x08,0x08,0x28,0x10},	//j9
	{0x00,0x20,0x20,0x28,0x30,0x30,0x28,0x24},	//k10
	{0x00,0x10,0x10,0x10,0x10,0x10,0x14,0x18},	//l11
	{0x00,0x00,0x00,0x00,0x28,0x54,0x54,0x54},	//m12
	{0x00,0x00,0x00,0x00,0x38,0x24,0x24,0x24},	//n13
	{0x00,0x00,0x00,0x00,0x18,0x24,0x24,0x18},	//o14
	{0x00,0x00,0x00,0x38,0x24,0x38,0x20,0x20},	//p15
	{0x00,0x00,0x00,0x1c,0x24,0x1c,0x04,0x04},	//q16	50
	{0x00,0x00,0x00,0x00,0x2c,0x30,0x20,0x20},	//r17
	{0x00,0x00,0x18,0x24,0x10,0x08,0x24,0x18},	//s18
	{0x00,0x00,0x00,0x10,0x38,0x10,0x10,0x18},	//t19
	{0x00,0x00,0x00,0x00,0x24,0x24,0x24,0x1a},	//u20
	{0x00,0x00,0x00,0x00,0x44,0x44,0x28,0x10},	//v21
	{0x00,0x00,0x00,0x00,0x44,0x54,0x54,0x28},	//w22
	{0x00,0x00,0x00,0x44,0x28,0x10,0x28,0x44},	//x23
	{0x00,0x00,0x24,0x24,0x1c,0x04,0x24,0x18},	//y24
	{0x00,0x00,0x00,0x00,0x3c,0x08,0x10,0x3c},	//z25
	{0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff},	//{

	{0x50,0x7e,0x90,0x10,0x3c,0x10,0x10,0xff},	//|生
	{0x7e,0x42,0x42,0x7e,0x42,0x42,0x42,0x7e},	//}日
	{0x08,0x48,0xde,0xea,0xdf,0x4c,0x54,0x63},	//~快
	{0x1c,0x60,0x48,0x7e,0x2c,0x4a,0x1b,0x08}	//Del乐
};

uchar __code tabs[4][8]=	//箭头组 上下左右
{
	{0x18,0x3c,0x7e,0xff,0x18,0x18,0x18,0x18},
	{0x18,0x18,0x18,0x18,0xff,0x7e,0x3c,0x18},
	{0x10,0x30,0x70,0xff,0xff,0x70,0x30,0x10},
	{0x08,0x0c,0x0e,0xff,0xff,0x0e,0x0c,0x08}
};
uchar __code testTAB4[8][8]={	//箭头动画
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
	{0x00,0x00,0x10,0x3c,0x3c,0x10,0x00,0x00},
	{0x00,0x00,0x30,0x7e,0x7e,0x30,0x00,0x00},
	{0x00,0x30,0x70,0xff,0xff,0x70,0x30,0x00},
	{0x10,0x30,0x70,0xff,0xff,0x70,0x30,0x10},
	{0x10,0x30,0x70,0xff,0xff,0x70,0x30,0x10},
	{0x10,0x30,0x70,0xff,0xff,0x70,0x30,0x10}
};

uchar __code xl[]={0x3C,0x42,0xA5,0x81,0xA5,0x99,0x42,0x3C};
//0x90 P1 | 0xA0 P2 | 0xB0 P3 | 0xC0 P4 | 0xC8 P5
//电源595扫描接口
__sbit __at (0xB0^3) SER;	//14
__sbit __at (0xB0^4) RCLK;	//12
__sbit __at (0xB0^5) SCLK;	//11

__sbit __at (0x80^7) GSDI;	//14
__sbit __at (0x80^5) GSCLK;	//11
__sbit __at (0x80^6) GRCLK;	//12

__sbit __at (0xa0^5) RSDI;	//14
__sbit __at (0xa0^7) RSCLK;	//11
__sbit __at (0xa0^6) RRCLK;	//12
/*红绿使能单独定义
__sbit __at (0x90^0) GLED1;
__sbit __at (0x90^1) GLED2;
__sbit __at (0x90^2) GLED3;
__sbit __at (0x90^3) GLED4;

__sbit __at (0x90^4) RLED1;
__sbit __at (0x90^5) RLED2;
__sbit __at (0x90^6) RLED3;
__sbit __at (0x90^7) RLED4;
*/
__sbit __at (0x80^4) BEEP;
__sbit __at (0xC0^0) KC1;
__sbit __at (0xa0^0) KC2;
__sbit __at (0xa0^1) KC3;
__sbit __at (0xa0^2) KC4;
__sbit __at (0xa0^3) KC5;
__sbit __at (0xa0^4) KC6;

__xdata uchar array[252];	//串口缓存数据
__bit begin=0, flags=0,ISPMODE=0;	//串口接收开始和完毕标志位
uchar count=0,sj=0;	//串口数据长度变量
/*
void putchar (char c) {
	
	while (!TI);
	TI = 0;
	SBUF = c;
}*/


void S595(__bit type,uchar rdat1,uchar rdat2,uchar rdat3,uchar rdat4){
	uchar aa,bb;
	uchar ltmp[4];
	ltmp[0]=rdat1;ltmp[1]=rdat2;ltmp[2]=rdat3;ltmp[3]=rdat4;
	if (type == 0){
		RSCLK=RRCLK=1;
		for(bb=0;bb<4;bb++){
			for(aa=0;aa<8;aa++){
				RSDI=ltmp[bb]>>7;
				ltmp[bb]<<=1;
				RSCLK=0;
				__asm
				nop
				__endasm;
				RSCLK=1;
			}
		}
	}
	else {
		GSCLK=GRCLK=1;
		for(bb=0;bb<4;bb++){
			for(aa=0;aa<8;aa++){
				GSDI=ltmp[bb]>>7;
				ltmp[bb]<<=1;
				GSCLK=0;
				__asm
				nop
				__endasm;
				GSCLK=1;
			}
		}
	}
}

void HC595send(unsigned char dat){	//电源扫描数据发送并显示
	uchar aa;
	SCLK=1;
	RCLK=1;
	for(aa=0;aa<8;aa++)
	{
		SER=dat>>7;
		dat<<=1;
		SCLK=0;
		__asm
		nop
		__endasm;
		SCLK=1;
	}
	RCLK=0;
	__asm
	nop
	__endasm;
	RCLK=1;
}

void show(void){	//显示图像数据
	RRCLK=0;GRCLK=0;
	__asm
	nop
	__endasm;
	RRCLK=1;GRCLK=1;
}

void cominit(unsigned char brts){	//串口初始化函数
	PCON |= 0x80;		//使能波特率倍速位SMOD
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x04;		//独立波特率发生器时钟为Fosc,即1T
	BRT = brts;		/*设定独立波特率发生器重装值	FF=1382400bps	FE=691200bps
				FD=460800bps	FC=345600bps	FB=276480bps	FA=230400bps
				F4=115200bps	E8=57600bps	DC=38400bps	B8=19200bps	70=9600bps*/
	AUXR |= 0x01;		//串口1选择独立波特率发生器为波特率发生器
	AUXR |= 0x10;		//启动独立波特率发生器
	//TI=0;//RI=1;	//初始化标志位
	ES = 1;	//开串口中断.
	EA = 1;
}

void keyinit(void){
	KC1=KC2=KC3=KC4=KC5=KC6=0;
	if(KC4==0 && KC5==0 && KC6==0)
		cominit(0xf4);	//115200bps
	else if(KC4==0 && KC5==0 && KC6==1)
		cominit(0x70);	//9600bps
	else if(KC4==0 && KC5==1 && KC6==0)
		cominit(0xB8);	//19200bps
	else if(KC4==0 && KC5==1 && KC6==1)
		cominit(0xDC);	//38400bps
	else if(KC4==1 && KC5==0 && KC6==0)
		cominit(0xE8);	//57600bps
	else if(KC4==1 && KC5==0 && KC6==1)
		cominit(0xFA);	//230400bps
	else if(KC4==1 && KC5==1 && KC6==0)
		cominit(0xFB);	//276480bps
	else if(KC4==1 && KC5==1 && KC6==1)
		cominit(0xFC);	//345600bps
	else cominit(0xf4);	//115200bps
}

main(){
	uchar i,Bgw,t,Bsw,Bbeep,w,tm,SJ;	//i常规扫描计数变量；t延时；Bgw计时个位变量；Bsw计时十位变量；Bbeep;w ascii显示计数变量；tm串口数字修改交换数据
	uchar hc[4];
	__bit ISPS=0;	//烧写模式标志位
	keyinit();
	CSLED=0x0f;BEEP=1;	//默认红色使能，关闭蜂鸣器
	w=4;Bsw=Bgw=Bbeep=t=i=0;
	
	for(t=0;t<250;t++){
		for(i=0;i<8;i++){
			S595(1,~dxzm['V'-0x20][i],~shuzi[2][i],~dxzm['.'-0x20][i],~shuzi[6][i]);
			HC595send(vdata[i]);	//发送电源扫描数据
			show();
			dly(353);
			S595(1,0xff,0xff,0xff,0xff);show();
		}
	}
	wl_main:	//goto断点0，默认主程序###################################################
	w=4;Bsw=Bgw=Bbeep=t=i=0;
	CSLED=0xf0;BEEP=1;	//默认红色使能，关闭蜂鸣器
	//EA = 0;
	//printf("love");
	//EA = 1;
	//SBUF=0x30;TI=0;
	while(1){
		CSLED=0x69;
		for(Bsw=0;Bsw<10;Bsw++)	//数字十位计数器循环
		{
			for(Bgw=0;Bgw<10;Bgw++)	//数字个位计数器循环
			{
				for(Bbeep=0;Bbeep<2;Bbeep++)	//蜂鸣器翻转循环
				{
					for(t=0;t<200;t++)	//显示延迟循环
					{
						for (i=0;i<8;i++)	//行扫描循环
						{
							if(ISPMODE==1){		//烧写模式，倒数5秒
								Bsw=9;Bgw=4;
								ISPMODE=0;ISPS=1;
							}
							if(ISPS==1){	//烧写模式显示
								S595(1,~tabs[3][i],~shuzi[9-Bsw][i],~shuzi[9-Bgw][i],~tabs[2][i]);dly(14);
								CSLED=0x0f;
							}
							else	//普通模式显示
							{
								S595(0,~tabs[3][i],~shuzi[9-Bsw][i],~shuzi[9-Bgw][i],~tabs[2][i]);
								S595(1,~tabs[3][i],~shuzi[9-Bsw][i],~shuzi[9-Bgw][i],~tabs[2][i]);
								CSLED=0x69;
							}
							HC595send(vdata[i]);	//发送电源扫描数据
							show();dly(14);
							S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
							
							if(flags==1){
								switch(array[0])
								{
									case '0':
										if(sj>=2){
											tm=array[1]-0x30;
											Bsw=9-tm;
											if(sj==3){
												tm=array[2]-0x30;
												Bgw=9-tm;
											}
										}
										break;
									case 'a':
										flags=0;
										goto wl_ascii;
										break;
									case 'w':
										goto wl_word;
										break;
									case 'f':
										flags=0;
										goto wl_flash;
										break;
									case 'l':
										flags=0;
										goto wl_left;
										break;
									case 'r':
										flags=0;
										goto wl_right;
										break;
									case 'b':
										if (array[1] == '1')
											BEEP=1;
										else if (array[1] == '0')
											BEEP=0;
										break;
									case 'E':
										ISPMODE=0;
										ISPS=0;
										break;
								}
								flags=0;
							}
						}
					}
				}
			}
		}
		if(ISPMODE==1 || ISPS==1){
			IAP_CONTR = 0x60;
		}
		CSLED=0xf0;
		for(Bbeep=0;Bbeep<4;Bbeep++)
		{
			for(t=0;t<255;t++)
			{
				for (i=0;i<8;i++)
				{
					HC595send(vdata[i]);	//发送电源扫描数据
					S595(0,~dxzm['E'-0x20][i],~dxzm['L'-0x20][i],~dxzm['S'-0x20][i],~dxzm['C'-0x20][i]);
					S595(1,~dxzm['E'-0x20][i],~dxzm['L'-0x20][i],~dxzm['S'-0x20][i],~dxzm['C'-0x20][i]);
					show();
					dly(53);
					S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
				}
			}
		}
		for(Bbeep=0;Bbeep<4;Bbeep++)
		{
			for(t=0;t<255;t++)
			{
				for (i=0;i<8;i++)
				{
					HC595send(vdata[i]);	//发送电源扫描数据
					S595(0,~dxzm['E'-0x20][i],~dxzm['T'-0x20][i],~dxzm['C'-0x20][i],~xl[i]);
					S595(1,~dxzm['E'-0x20][i],~dxzm['T'-0x20][i],~dxzm['C'-0x20][i],~xl[i]);
					show();
					dly(53);
					S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
				}
			}
		}
	}
	
	wl_ascii:	//goto断点1，ascii字符显示测试############################################
	CSLED=0xf0;BEEP=1;
	w=Bsw=Bgw=Bbeep=t=i=0;
	//puts("ascii");
	while(1){
		CSLED=0x96;
		for(w=0;w<91;w++){
			for(Bgw=0;Bgw<8;Bgw++){
				for(t=0;t<55;t++){	//显示延迟循环
					for (i=0;i<8;i++){	//行扫描循环
						HC595send(vdata[i]);	//发送电源扫描数据
						hc[0]=(dxzm[w][i]<<Bgw)|(dxzm[w+1][i]>>(8-Bgw));
						hc[1]=(dxzm[w+1][i]<<Bgw)|(dxzm[w+2][i]>>(8-Bgw));
						S595(0,~tabs[3][i],~hc[0],~hc[1],~tabs[2][i]);
						S595(1,~tabs[3][i],~hc[0],~hc[1],~tabs[2][i]);	//发送每个G模块的显示数据
						show();S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
						if(ISPMODE==1)
							goto wl_main;
						if(flags==1){
							switch(array[0])
							{
								case '0':
									goto wl_main;
									break;
								case 'a':
									flags=0;
									goto wl_ascii;
									break;
								case 'w':
									goto wl_word;
									break;
								case 'f':
									flags=0;
									goto wl_flash;
									break;
								case 'l':
									flags=0;
									goto wl_left;
									break;
								case 'r':
									flags=0;
									goto wl_right;
									break;
								case 't':
									flags=0;
									goto wl_test;
									break;
								case 'b':
									if (array[1] == '1')
										BEEP=1;
									else if (array[1] == '0')
										BEEP=0;
									break;
							}
							flags=0;
						}
					}
				}
			}
		}
	}
	
	wl_word:	//goto断点2，自定义显示字符###############################################
	CSLED=0xf0;BEEP=1;
	//puts("word");
	SJ=sj;
	while(1){
		if(SJ>6){
			for(w=0;w<SJ-2;w++){
				for(Bgw=0;Bgw<8;Bgw++){
					for(t=0;t<25;t++){	//显示延迟循环
						for (i=0;i<8;i++){	//行扫描循环
							HC595send(vdata[i]);	//发送电源扫描数据
							hc[0]=(dxzm[wd[w]][i]<<Bgw)|(dxzm[wd[w+1]][i]>>(8-Bgw));
							hc[1]=(dxzm[wd[(w >= SJ ? 0 : w+1)]][i]<<Bgw)|(dxzm[wd[(w >= SJ ? 0 : w+2)]][i]>>(8-Bgw));
							/*if((w+4)>=SJ){	//(q == 4 ? 0 : q + 1)
								hc[3]=(dxzm[0][i]<<Bgw)|(dxzm[0][i]>>(8-Bgw));
								hc[2]=(dxzm[0][i]<<Bgw)|(dxzm[0][i]>>(8-Bgw));
							}
							else
							{
								hc[3]=(dxzm[wd[w+3]][i]<<Bgw)|(dxzm[0][i]>>(8-Bgw));
								if((w+3)>=SJ)
									hc[2]=(dxzm[0][i]<<Bgw)|(dxzm[0][i]>>(8-Bgw));
								else{
									hc[2]=(dxzm[wd[w+2]][i]<<Bgw)|(dxzm[wd[w+3]][i]>>(8-Bgw));
									hc[3]=(dxzm[wd[w+3]][i]<<Bgw)|(dxzm[wd[w+4]][i]>>(8-Bgw));
								}
							}*/
							hc[2]=(dxzm[wd[(w >= SJ ? 0 : w+2)]][i]<<Bgw)|(dxzm[wd[(w >= SJ ? 0 : w+3)]][i]>>(8-Bgw));
							hc[3]=(dxzm[wd[(w >= SJ ? 0 : w+3)]][i]<<Bgw)|(dxzm[wd[(w >= SJ ? 0 : w+4)]][i]>>(8-Bgw));
							S595(0,~hc[0],~hc[1],~hc[2],~hc[3]);
							S595(1,~hc[0],~hc[1],~hc[2],~hc[3]);	//发送每个G模块的显示数据
							show();S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
							if(ISPMODE==1)
								goto wl_main;
							if(flags==1){
								switch(array[0])
								{
									case '0':
										goto wl_main;
										break;
									case 'a':
										flags=0;
										goto wl_ascii;
										break;
									case 'w':
										if(array[1]=='g')
											CSLED=0x0f;
										else if(array[1]=='o')
											CSLED=0x00;
										else
											CSLED=0xf0;
										memset(wd,0,sizeof(char)*250);
										for(Bsw=0;Bsw<sj;Bsw++){	//把串口收到的数据放入显示临时缓存
											wd[Bsw]=array[Bsw+2]-0x20;	//把收到的ascii串口数据转换
											wd[Bsw+1]=0x00;wd[Bsw+2]=0x00;wd[Bsw+3]=0x00;wd[Bsw+4]=0x00;
										}
								//串口数据第一个是识别码所以要从下一个数据读取
								//因为写的字库从空格0x20开始所以要-0x20
								//puts(array);
										SJ=sj;
										flags=0;
										w=SJ;Bgw=255;t=255;
										//goto wl_word;
										break;
									case 'f':
										flags=0;
										goto wl_flash;
										break;
									case 'l':
										flags=0;
										goto wl_left;
										break;
									case 'r':
										flags=0;
										goto wl_right;
										break;
									case 't':
										flags=0;
										goto wl_test;
										break;
									case 'b':
										if (array[1] == '1')
											BEEP=1;
										else if (array[1] == '0')
											BEEP=0;
										break;
								}
								flags=0;memset(array,0,sizeof(char)*250);
							}
						}
					}
				}
			}
		}
		else{
		for (i=0;i<8;i++)	//行扫描循环
		{
			HC595send(vdata[i]);	//发送电源扫描数据
			S595(0,~dxzm[wd[0]][i],~dxzm[wd[1]][i],~dxzm[wd[2]][i],~dxzm[wd[3]][i]);
			S595(1,~dxzm[wd[0]][i],~dxzm[wd[1]][i],~dxzm[wd[2]][i],~dxzm[wd[3]][i]);
			show();
			S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
			if(ISPMODE==1)
				goto wl_main;
			if(flags==1){
				switch(array[0])
				{
					case '0':
						goto wl_main;
						break;
					case 'a':
						flags=0;
						goto wl_ascii;
						break;
					case 'w':
						if(array[1]=='g')
							CSLED=0x0f;
						else if(array[1]=='o')
							CSLED=0x00;
						else
							CSLED=0xf0;
						//memset(wd,0,sizeof(char)*250);
						for(Bsw=0;Bsw<sj;Bsw++){	//把串口收到的数据放入显示临时缓存
							wd[Bsw]=array[Bsw+2]-0x20;	//把收到的ascii串口数据转换
							wd[Bsw+1]=0x00;wd[Bsw+2]=0x00;wd[Bsw+3]=0x00;wd[Bsw+4]=0x00;
						}
						//串口数据第一个是识别码所以要从下一个数据读取
						//因为写的字库从空格0x20开始所以要-0x20
						SJ=sj;
						flags=0;
						break;
					case 'f':
						flags=0;
						goto wl_flash;
						break;
					case 'l':
						flags=0;
						goto wl_left;
						break;
					case 'r':
						flags=0;
						goto wl_right;
						break;
					case 't':
						flags=0;
						goto wl_test;
						break;
					case 'b':
						if (array[1] == '1')
							BEEP=1;
						else if (array[1] == '0')
							BEEP=0;
						break;
				}
				flags=0;memset(array,0,sizeof(char)*250);
			}
		}
		}
	}
	
	wl_flash:	//goto断点3，全屏闪烁#####################################################
	w=4;Bsw=Bgw=Bbeep=t=i=0;
	CSLED=0xf0;BEEP=1;	//默认红色使能，关闭蜂鸣器
	SJ=1;
	while(1){
		if(SJ<=1)
			CSLED=0xFE;	//开红色全部
		else if(SJ==2)
			CSLED=0xf7;
		else if(SJ==3)
			CSLED=0xEF;
		else if(SJ==4)
			CSLED=0x7f;
		else if(SJ==5)
			CSLED=0x99;
		else if(SJ>=6){
			SJ=0;
			CSLED=0xff;
		}
		for(t=0;t<25;t++)
		{
			for (i=0;i<8;i++)
			{
				HC595send(vdata[i]);	//发送电源扫描数据
				S595(0,0x00,0x00,0x00,0x00);S595(1,0x00,0x00,0x00,0x00);show();
				S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
			}
		}
		SJ++;
		if(ISPMODE==1)
			goto wl_main;
		if(flags==1){
			switch(array[0])
			{
				case '0':
					goto wl_main;
					break;
				case 'a':
					flags=0;
					goto wl_ascii;
					break;
				case 'w':
					goto wl_word;
					break;
				case 'f':
					flags=0;
					goto wl_flash;
					break;
				case 'l':
					flags=0;
					goto wl_left;
					break;
				case 'r':
					flags=0;
					goto wl_right;
					break;
				case 't':
					flags=0;
					goto wl_test;
					break;
				case 'b':
					if (array[1] == '1')
						BEEP=1;
					else if (array[1] == '0')
						BEEP=0;
					break;
			}
			flags=0;
		}
	}
	
	wl_left:	//goto断点4，左转转向灯###################################################
	CSLED=0xf7;BEEP=1;
	while(1){
		for(Bgw=0;Bgw<8;Bgw++){
			for(t=0;t<65;t++)
			{
				for (i=0;i<8;i++)
				{
					HC595send(vdata[i]);	//发送电源扫描数据~(tabs[2][i]<<Bgw | tabs[2][i]>>(8-Bgw))
					S595(0,~tabs[2][i],(~tabs[2][i]<<Bgw | ~tabs[2][i]>>(8-Bgw)),~(tabs[2][i]<<Bgw | tabs[2][i]>>(8-Bgw)),~(tabs[2][i]<<Bgw | tabs[2][i]>>(8-Bgw)));
					S595(1,~tabs[2][i],0xff,0xff,0xff);
					show();S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
					if(ISPMODE==1)
						goto wl_main;
					if(flags==1){
						switch(array[0])
						{
							case '0':
								goto wl_main;
								break;
							case 'a':
								flags=0;
								goto wl_ascii;
								break;
							case 'w':
								goto wl_word;
								break;
							case 'f':
								flags=0;
								goto wl_flash;
								break;
							case 'l':
								flags=0;
								goto wl_left;
								break;
							case 'r':
								flags=0;
								goto wl_right;
								break;
							case 't':
								flags=0;
								goto wl_test;
								break;
							case 'b':
								if (array[1] == '1')
									BEEP=1;
								else if (array[1] == '0')
									BEEP=0;
								break;
						}
						flags=0;
					}
				}
			}
			if((Bgw % 2)==0)
				CSLED=0xf1;
			else	CSLED=0x00;
		}
	}

	wl_right:	//goto断点5，右转转向灯###################################################
	CSLED=0xfe;BEEP=1;
	while(1){
		for(Bgw=0;Bgw<8;Bgw++){
			for(t=0;t<65;t++)
			{
				for (i=0;i<8;i++)
				{
					HC595send(vdata[i]);	//发送电源扫描数据
					S595(0,~(tabs[3][i]<<(8-Bgw) | tabs[3][i]>>Bgw),~(tabs[3][i]<<(8-Bgw) | tabs[3][i]>>Bgw),~tabs[3][i]<<(8-Bgw) | 0xff>>Bgw,~tabs[3][i]);
					S595(1,0xff,0xff,0xff,~tabs[3][i]);
					show();S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
					if(ISPMODE==1)
						goto wl_main;
					if(flags==1){
						switch(array[0])
						{
							case '0':
								goto wl_main;
								break;
							case 'a':
								flags=0;
								goto wl_ascii;
								break;
							case 'w':
								goto wl_word;
								break;
							case 'f':
								flags=0;
								goto wl_flash;
								break;
							case 'l':
								flags=0;
								goto wl_left;
								break;
							case 'r':
								flags=0;
								goto wl_right;
								break;
							case 't':
								flags=0;
								goto wl_test;
								break;
							case 'b':
								if (array[1] == '1')
									BEEP=1;
								else if (array[1] == '0')
									BEEP=0;
								break;
						}
						flags=0;
					}
				}
			}
			if((Bgw % 2)==0)
				CSLED=0xf8;
			else	CSLED=0x00;
		}
	}
	wl_test:	//goto断点6，程序模块测试###################################################
	while(1){
		CSLED=0x00;
		for(Bgw=0;Bgw<8;Bgw++){
			for(t=0;t<35;t++)
			{
				for(i=0;i<8;i++){
					HC595send(vdata[i]);	//发送电源扫描数据
					hc[0]=(dxzm['\\'-0x20][i]<<(8-Bgw)) | (dxzm['/'-0x20][i]>>Bgw);
					hc[1]=(dxzm['/'-0x20][i]<<(8-Bgw)) | (dxzm['\\'-0x20][i]>>Bgw);
					S595(0,~hc[0],~hc[0],~hc[0],~hc[0]);
					S595(1,~hc[1],~hc[1],~hc[1],~hc[1]);
					show();S595(0,0xff,0xff,0xff,0xff);S595(1,0xff,0xff,0xff,0xff);show();
					if(ISPMODE==1)
						goto wl_main;
					if(flags==1){
						switch(array[0])
						{
							case '0':
								goto wl_main;
								break;
							case 'a':
								flags=0;
								goto wl_ascii;
								break;
							case 'w':
								goto wl_word;
								break;
							case 'f':
								flags=0;
								goto wl_flash;
								break;
							case 'l':
								flags=0;
								goto wl_left;
								break;
							case 'r':
								flags=0;
								goto wl_right;
								break;
							case 'b':
								if (array[1] == '1')
									BEEP=1;
								else if (array[1] == '0')
									BEEP=0;
								break;
						}
						flags=0;
					}
				}
			}
		}
	}
}
void dly(unsigned int t)
{
	while(--t != 0){
		__asm
		nop
		nop
		__endasm;
	}
}

void Delay(unsigned int t)
{		
	unsigned char i, j, k;
	while( --t != 0){
		__asm
		nop
		nop
		__endasm;
		i = 5;
		j = 52;
		k = 195;
		do
		{
			do
			{
				while (--k);
			} while (--j);
		} while (--i);
	}
}
void Serial() __interrupt 4
{
	uchar temp;	//收到的串口数据存放
	RI = 0;	//清除接收标志位
	temp=SBUF;	//把收到的数据放入临时变量
	if(temp=='`'||begin==1)//如果是@，则接下来的数据都是要接收的，如果begin是1也表示是要接收的
	{
		begin=1;	//开始接收标志位
		if(temp!='`')//因为@是标致位，所以我们是要丢弃的
		{
			if(temp!=0x0A)	//如果收到的数据不是#
			{
				array[count]=temp;	//把数据放入临时数组内
				count++;
			}
			else if(temp==0x0A)//停止位
			{
				//array[count+1]='#';
				sj=count;	//数据长度
				begin=0;	//清除接收标志位，结束接收
				count=0;	//清除累加器
				flags=1;	//标志数据接收完毕信号
			}

		}
	}
	else if (temp==0x8f){	//ISP烧写模式
		ISPMODE=1;
		flags=0;
	}
		//goto wl_main;
		//IAP_CONTR = 0x60;
}