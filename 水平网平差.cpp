// 水平网平差.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "PlaneNetAdjust.h"


#include "public.h"


//////////////////////////////////////////////////////////////////////////
//   最小二乘平差算例函数
void main1()
{
	char *datafile="算例\\最小二乘平差\\data.txt";
	char *resultfile="算例\\最小二乘平差\\result.txt";

	CPlaneNetAdjust djst;
	djst.resultfp=fopen(resultfile,"w");

	if(djst.resultfp == NULL)
	{
		MyBreak("无法创建(打开)计算结果文件：%s",resultfile);
		return;
	}
	djst.LS_Adjust(datafile);
	fclose(djst.resultfp);

	printf("\nOK. 最小二乘平差结束");
	printf("\n   数据文件：%s",datafile);
	printf("\n   结果文件：%s\n\n",resultfile);
	
}

//////////////////////////////////////////////////////////////////////////
// 自由网平差算例
void main2()
{
	char *datafile="算例\\自由网平差\\data.txt";
	char *resultfile="算例\\自由网平差\\result.txt";

	CPlaneNetAdjust djst;
	djst.resultfp=fopen(resultfile,"w");

	if(djst.resultfp == NULL)
	{
		MyBreak("无法创建(打开)计算结果文件：%s",resultfile);
		return;
	}

	djst.InputData(datafile);
	djst.PrintData();
	djst.Free();

	fclose(djst.resultfp);	
	
	printf("\nOK. 自由网平差结束");
	printf("\n   数据文件：%s",datafile);
	printf("\n   结果文件：%s\n\n",resultfile);
	
}


//////////////////////////////////////////////////////////////////////////
// 拟稳平差算例
void main3()
{
	char *datafile="算例\\拟稳平差\\data.txt";
	char *datafile2="算例\\拟稳平差\\拟稳点.txt";
	char *resultfile="算例\\拟稳平差\\result.txt";
	
	CPlaneNetAdjust adjust;
	adjust.resultfp=fopen(resultfile,"w");
	
	if(adjust.resultfp == NULL)
	{
		MyBreak("无法创建(打开)计算结果文件：%s",resultfile);
		return;
	}
	
	adjust.InputData(datafile);
	adjust.PrintData();
	adjust.Quasi_Stable(datafile2);
	
	fclose(adjust.resultfp);		
	
	printf("\nOK. 拟稳平差平差结束");
	printf("\n   数据文件：%s",datafile);
	printf("\n   结果文件：%s\n\n",resultfile);
	
}



//////////////////////////////////////////////////////////////////////////
// 粗差探测算例函数
void main4()
{
	char *datafile="算例\\粗差探测\\data.txt";
	char *resultfile="算例\\粗差探测\\result.txt";
	
	CPlaneNetAdjust adjust;
	adjust.resultfp=fopen(resultfile,"w");
	
	if(adjust.resultfp == NULL)
	{
		MyBreak("无法创建(打开)计算结果文件：%s",resultfile);
		return;
	}
	
	adjust.InputData(datafile);
	// adjust.PrintData();
	adjust.Snooping(0.003);
	
	fclose(adjust.resultfp);		
	
	printf("\nOK. 粗差探测结束");
	printf("\n   数据文件：%s",datafile);
	printf("\n   结果文件：%s\n\n",resultfile);
	
}


int main()
{

	//ErrorEllipse_exmple();

	printf("\n  ==== 第6章 水平网平差 ====\n"
		   "	1.最小二乘平差\n"
		   "	2.自由网平差\n"
		   "	3.拟稳平差\n"
		   "	4.粗差探测\n");

	
	int sw=getch()-'0';

	switch(sw) {
	case 1: 
		{
			main1();  //最小二乘平差
		}
		break;
	case 2:
		{
			main2(); //自由网平差
		}
		break;
	case 3:
		{
			main3();  //拟稳平差
		}
		break;
	case 4:
		{
			main4();  //粗差探测
		}
		break;
		
	default:
		break;
	}

	return 0;
}


