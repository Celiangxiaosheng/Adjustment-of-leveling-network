// 第五章 边角网平差.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "NetworkAdjust.h"

#include "public.h"


//////////////////////////////////////////////////////////////////////////
//   最小二乘平差算例函数
void main1()
{
	char *datafile="算例\\导线网最小二乘平差\\data.txt";
	char *resultfile="算例\\导线网最小二乘平差\\result.txt";

	CPlaneNetAdjust djst;
	djst.resultfp=fopen(resultfile,"w");

	if(djst.resultfp == NULL)
	{
		MyBreak("无法创建(打开)计算结果文件：%s",resultfile);
		return;
	}
	djst.LS_Adjust(datafile);
	fclose(djst.resultfp);
		
}

//////////////////////////////////////////////////////////////////////////
// 自由网平差算例
void main2()
{
	char *datafile="算例\\自由网\\data.txt";
	char *resultfile="算例\\自由网\\result.txt";

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
}


//////////////////////////////////////////////////////////////////////////
// 拟稳平差算例
void main3()
{
	char *datafile="算例\\自由网拟稳平差\\data.txt";
	char *datafile2="算例\\自由网拟稳平差\\拟稳点.txt";
	char *resultfile="算例\\自由网拟稳平差\\result.txt";
	
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
}



//////////////////////////////////////////////////////////////////////////
// 粗差探测算例函数
void main4()
{
	char *datafile="算例\\导线网粗差探测\\data.txt";
	char *resultfile="算例\\导线网粗差探测\\result.txt";
	
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
}


int main(int argc, char* argv[])
{

	//ErrorEllipse_exmple();

	printf("\n  ==== 水平网平差计算 ===="
		   "\n  1.最小二乘平差\n  2.自由网平差\n  3.拟稳平差\n  4.粗差探测\n");

	
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


