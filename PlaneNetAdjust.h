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
	FILE   *resultfp;   //结果文件指针
	int    m_Pnumber;   //总点数
	int    m_Lnumber;   //方向组总数
	int    m_Nnumber;   //方向值总数
	int    m_knPnumber; //已知点总数
	int    m_StableNumber; //拟稳点数
	
	int    m_Tnumber;   //方位角观测值个数
	int    m_Snumber;   //边长观测值个数
	
	int    *dir1;   //测站点号
	int    *dir2;   //观测方向的点号
	int    *dir0;   //测站零方向在方向值数组中的位置

	char   **Pname;     //点名地址数组
	
	double *L,*V; //方向观测值及其残差数组
	double *XY;   //坐标数组
	double *ATPA,*ATPL; //法方程系数阵、法方程自由项
	double *dX;   //未知参数数组（坐标改正数、定向角改正数）
	double m_pvv; // [pvv]
	double m_mu;  //验后单位权中误差
	
	bool *IsStable;  //拟稳点标志数组

	double ma,mT; //方向值中误差、方位角中误差
	double mS1,mS2; //边长固定误差、比例误差
	
	int     *T_dir1; //方位角测站点点号数组
	int     *T_dir2; //方位角照准点点号数组
	double  *T_L;    //方位角观测值数组
	double  *T_V;    //方位角残差数组
	
	int     *S_dir1; //边长观测值测站点号数组
	int     *S_dir2; //边长观测值照准点号数组
	double  *S_L;    //边长观测值数组
	double  *S_V;    //边长残差数组	

	bool    *Usable;  //观测值可用标志数组
	
	CPlaneNetAdjust();
	virtual ~CPlaneNetAdjust();	
	
	void   InputData(char *DATAFILE);//输入原始数据
	void   PrintData();              //输出原始数据
	int    GetStationNumber(char *name); //保存点名，返回点号
	
	double Get_S12(int k1,int k2);  //查找边长观测值
	double Get_T12(int k1,int k2);  //查找方位角观测值
	double Get_Angle(int p,int p1,int p2);//查找角度值
	void   ca_xy0(); //近似坐标计算(三角网)
	void   ca_x0y0();//近似坐标计算（导线网）
		
	double ca_T12(int k1,int k2);  //用坐标计算方位角，返回值为弧度值,
	double ca_ab(int k1,int k2,double B[],int Bin[]); // ab系数计算
	double ca_cd(int k1,int k2,double B[],int Bin[]); // cd系数计算
	void   ca_ATPA(); //法方程组成
	void   ca_ATPAi(double B[],int Bint[],double p,
									double Li,int m);//法方程累加项计算
	double ca_dX();  //参数平差值计算
	double ca_V();   //残差计算
	double qq(double B[],int Bin[]); //权倒数计算
	void   PrintResult(); //输出平差成果

	void LS_Adjust(char *data_file); //最小二乘平差
	void Free(); //自由网平差
	int  Quasi_Stable(char *QuasiStablefile); //拟稳平差
	double *ca_GT(char *file);//自由网平差用的G矩阵

	void ErrorEllipse(); //误差椭圆计算

	double  Snooping(double arfa);
		

};

#endif // !defined(AFX_NETWORKADJUST_H__029E7110_362B_45E9_BCA6_4D307D91111B__INCLUDED_)