// ˮƽ��ƽ��.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "PlaneNetAdjust.h"


#include "public.h"


//////////////////////////////////////////////////////////////////////////
//   ��С����ƽ����������
void main1()
{
	char *datafile="����\\��С����ƽ��\\data.txt";
	char *resultfile="����\\��С����ƽ��\\result.txt";

	CPlaneNetAdjust djst;
	djst.resultfp=fopen(resultfile,"w");

	if(djst.resultfp == NULL)
	{
		MyBreak("�޷�����(��)�������ļ���%s",resultfile);
		return;
	}
	djst.LS_Adjust(datafile);
	fclose(djst.resultfp);

	printf("\nOK. ��С����ƽ�����");
	printf("\n   �����ļ���%s",datafile);
	printf("\n   ����ļ���%s\n\n",resultfile);
	
}

//////////////////////////////////////////////////////////////////////////
// ������ƽ������
void main2()
{
	char *datafile="����\\������ƽ��\\data.txt";
	char *resultfile="����\\������ƽ��\\result.txt";

	CPlaneNetAdjust djst;
	djst.resultfp=fopen(resultfile,"w");

	if(djst.resultfp == NULL)
	{
		MyBreak("�޷�����(��)�������ļ���%s",resultfile);
		return;
	}

	djst.InputData(datafile);
	djst.PrintData();
	djst.Free();

	fclose(djst.resultfp);	
	
	printf("\nOK. ������ƽ�����");
	printf("\n   �����ļ���%s",datafile);
	printf("\n   ����ļ���%s\n\n",resultfile);
	
}


//////////////////////////////////////////////////////////////////////////
// ����ƽ������
void main3()
{
	char *datafile="����\\����ƽ��\\data.txt";
	char *datafile2="����\\����ƽ��\\���ȵ�.txt";
	char *resultfile="����\\����ƽ��\\result.txt";
	
	CPlaneNetAdjust adjust;
	adjust.resultfp=fopen(resultfile,"w");
	
	if(adjust.resultfp == NULL)
	{
		MyBreak("�޷�����(��)�������ļ���%s",resultfile);
		return;
	}
	
	adjust.InputData(datafile);
	adjust.PrintData();
	adjust.Quasi_Stable(datafile2);
	
	fclose(adjust.resultfp);		
	
	printf("\nOK. ����ƽ��ƽ�����");
	printf("\n   �����ļ���%s",datafile);
	printf("\n   ����ļ���%s\n\n",resultfile);
	
}



//////////////////////////////////////////////////////////////////////////
// �ֲ�̽����������
void main4()
{
	char *datafile="����\\�ֲ�̽��\\data.txt";
	char *resultfile="����\\�ֲ�̽��\\result.txt";
	
	CPlaneNetAdjust adjust;
	adjust.resultfp=fopen(resultfile,"w");
	
	if(adjust.resultfp == NULL)
	{
		MyBreak("�޷�����(��)�������ļ���%s",resultfile);
		return;
	}
	
	adjust.InputData(datafile);
	// adjust.PrintData();
	adjust.Snooping(0.003);
	
	fclose(adjust.resultfp);		
	
	printf("\nOK. �ֲ�̽�����");
	printf("\n   �����ļ���%s",datafile);
	printf("\n   ����ļ���%s\n\n",resultfile);
	
}


int main()
{

	//ErrorEllipse_exmple();

	printf("\n  ==== ��6�� ˮƽ��ƽ�� ====\n"
		   "	1.��С����ƽ��\n"
		   "	2.������ƽ��\n"
		   "	3.����ƽ��\n"
		   "	4.�ֲ�̽��\n");

	
	int sw=getch()-'0';

	switch(sw) {
	case 1: 
		{
			main1();  //��С����ƽ��
		}
		break;
	case 2:
		{
			main2(); //������ƽ��
		}
		break;
	case 3:
		{
			main3();  //����ƽ��
		}
		break;
	case 4:
		{
			main4();  //�ֲ�̽��
		}
		break;
		
	default:
		break;
	}

	return 0;
}


