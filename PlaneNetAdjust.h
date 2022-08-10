// NetworkAdjust.h: interface for the CNetworkAdjustment class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKADJUST_H__029E7110_362B_45E9_BCA6_4D307D91111B__INCLUDED_)
#define AFX_NETWORKADJUST_H__029E7110_362B_45E9_BCA6_4D307D91111B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>



class CPlaneNetAdjust  
{
public:
	FILE   *resultfp;   //����ļ�ָ��
	int    m_Pnumber;   //�ܵ���
	int    m_Lnumber;   //����������
	int    m_Nnumber;   //����ֵ����
	int    m_knPnumber; //��֪������
	int    m_StableNumber; //���ȵ���
	
	int    m_Tnumber;   //��λ�ǹ۲�ֵ����
	int    m_Snumber;   //�߳��۲�ֵ����
	
	int    *dir1;   //��վ���
	int    *dir2;   //�۲ⷽ��ĵ��
	int    *dir0;   //��վ�㷽���ڷ���ֵ�����е�λ��

	char   **Pname;     //������ַ����
	
	double *L,*V; //����۲�ֵ����в�����
	double *XY;   //��������
	double *ATPA,*ATPL; //������ϵ���󡢷�����������
	double *dX;   //δ֪�������飨���������������Ǹ�������
	double m_pvv; // [pvv]
	double m_mu;  //���λȨ�����
	
	bool *IsStable;  //���ȵ��־����

	double ma,mT; //����ֵ������λ�������
	double mS1,mS2; //�߳��̶����������
	
	int     *T_dir1; //��λ�ǲ�վ��������
	int     *T_dir2; //��λ����׼��������
	double  *T_L;    //��λ�ǹ۲�ֵ����
	double  *T_V;    //��λ�ǲв�����
	
	int     *S_dir1; //�߳��۲�ֵ��վ�������
	int     *S_dir2; //�߳��۲�ֵ��׼�������
	double  *S_L;    //�߳��۲�ֵ����
	double  *S_V;    //�߳��в�����	

	bool    *Usable;  //�۲�ֵ���ñ�־����
	
	CPlaneNetAdjust();
	virtual ~CPlaneNetAdjust();	
	
	void   InputData(char *DATAFILE);//����ԭʼ����
	void   PrintData();              //���ԭʼ����
	int    GetStationNumber(char *name); //������������ص��
	
	double Get_S12(int k1,int k2);  //���ұ߳��۲�ֵ
	double Get_T12(int k1,int k2);  //���ҷ�λ�ǹ۲�ֵ
	double Get_Angle(int p,int p1,int p2);//���ҽǶ�ֵ
	void   ca_xy0(); //�����������(������)
	void   ca_x0y0();//����������㣨��������
		
	double ca_T12(int k1,int k2);  //��������㷽λ�ǣ�����ֵΪ����ֵ,
	double ca_ab(int k1,int k2,double B[],int Bin[]); // abϵ������
	double ca_cd(int k1,int k2,double B[],int Bin[]); // cdϵ������
	void   ca_ATPA(); //���������
	void   ca_ATPAi(double B[],int Bint[],double p,
									double Li,int m);//�������ۼ������
	double ca_dX();  //����ƽ��ֵ����
	double ca_V();   //�в����
	double qq(double B[],int Bin[]); //Ȩ��������
	void   PrintResult(); //���ƽ��ɹ�

	void LS_Adjust(char *data_file); //��С����ƽ��
	void Free(); //������ƽ��
	int  Quasi_Stable(char *QuasiStablefile); //����ƽ��
	double *ca_GT(char *file);//������ƽ���õ�G����

	void ErrorEllipse(); //�����Բ����

	double  Snooping(double arfa);
		

};

#endif // !defined(AFX_NETWORKADJUST_H__029E7110_362B_45E9_BCA6_4D307D91111B__INCLUDED_)