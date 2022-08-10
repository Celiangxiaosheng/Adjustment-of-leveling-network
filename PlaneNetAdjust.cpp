// NetworkAdjust.cpp: implementation of the CNetworkAdjustment class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlaneNetAdjust.h"

#include "public.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlaneNetAdjust::CPlaneNetAdjust()
{
	m_Lnumber=0;
	m_Snumber=0;
	m_Tnumber=0;
	m_Pnumber=0;

}

CPlaneNetAdjust::~CPlaneNetAdjust()
{
	if(m_Lnumber>0) //����۲�ֵ�����ʼ��
	{
		delete []dir0;
		delete []dir1;    
		delete []dir2;
		delete []L;
		delete []V; 
	}
	if(m_Snumber>0)//�߳��۲�ֵ�����ʼ��
	{
		delete []S_dir1;
		delete []S_dir2;
		delete []S_L;
		delete []S_V;
	}
	
	if(m_Tnumber>0)//��λ�ǹ۲�ֵ�����ʼ��
	{
		delete []T_dir1;
		delete []T_dir2;
		delete []T_L;
		delete []T_V;
	}
	if(m_Pnumber>0)//��λ�ǹ۲�ֵ�����ʼ��
	{
		delete []XY;
		for(int i=0;i<m_Pnumber;i++)
		{
			delete []Pname[i];
		}
		delete []Pname;
	}		
}



//////////////////////////////////////////////////////////////////////////
//	   ��Ŵ��������ݵ���������
int  CPlaneNetAdjust::GetStationNumber(char *name)
{
	for(int i=0;i<m_Pnumber;i++)
	{
		if(Pname[i]==NULL) break;
		if(strcmp(name,Pname[i])==0)return i;
	}

	if( i < m_Pnumber )//�Ѿ������ŵĵ���С���ܵ���
	{
        Pname[i]=new char[strlen(name)+1];
        strcpy(Pname[i],name);
        return i;
	}
	else 
	{
		fprintf(resultfp,"�������󣬳����ж�ִ�У�");
	}
	return -1;
}



//////////////////////////////////////////////////////////////////////////
//   ��ȡԭʼ�����ļ�
void CPlaneNetAdjust::InputData(char *file)
{
	FILE *fp;
    if((fp=fopen(file,"r"))==NULL)
	{    
		MyBreak("�޷���ԭʼ�����ļ���"); 
		exit(0);
	}

    fscanf(fp,"%d%d%d%d",&m_Pnumber, // �ܵ���
			      &m_knPnumber,       // ��֪����
			      &m_Lnumber,         // ����ֵ����
			      &m_Nnumber          // ����ֵ����
			      );   

    fscanf(fp,"%d%d",&m_Snumber,  // �߳�����
			      &m_Tnumber      // ��λ����
				  );   
 
	XY=new double   [2*m_Pnumber]; 

    Pname=new char* [m_Pnumber];
	for(int i=0;i<m_Pnumber;i++)Pname[i]=NULL;
	 
	if(m_Lnumber>0) // Ϊ����۲�ֵ���������ڴ�
	{
		 dir0=new int [m_Lnumber+1]; //�����׷���۲�ֵ�����
		 dir1=new int [m_Lnumber];   //��վ���
		 dir2=new int [m_Nnumber];   //��׼���
		 L=new double [m_Nnumber];   //����ֵ
		 V=new double [m_Nnumber];   //�ֲV֮ǰ����������l
	}
	if(m_Snumber>0)//Ϊ�߳��۲�ֵ���������ڴ�
	{
		 S_dir1=new int[m_Snumber];  //��վ���
		 S_dir2=new int[m_Snumber];  //��׼���
		 S_L=new double[m_Snumber];  //�߳��۲�ֵ
		 S_V=new double[m_Snumber];  //�߳��в�
	}

	if(m_Tnumber>0)//Ϊ��λ�ǹ۲�ֵ���������ڴ�
	{
		 T_dir1=new int[m_Snumber];  //��վ���
		 T_dir2=new int[m_Snumber];  //��׼���
		 T_L=new double[m_Snumber];  //��λ�ǹ۲�ֵ
		 T_V=new double[m_Snumber];  //��λ�ǲв�
	}

	// �۲�ֵ���ñ�־����
	int n=m_Nnumber+m_Snumber+m_Tnumber;
	Usable=new bool[n];
	for(i=0;i<n; i++)Usable[i]=true;

    int t=2*m_Pnumber+m_Lnumber; // δ֪��������
    int tt=t*(t+1)/2;

    ATPL=new double [t];   //������������
    ATPA=new double [tt];  //ϵ������
    dX=new double [t];     //δ֪������
	 
    int unPnumber=m_Pnumber-m_knPnumber;
	 
	// ��ȡ�۲�ֵ�����
	fscanf(fp,"%lf%lf%lf%lf",&ma,&mS1,&mS2,&mT);

	char name[100];
	for(i=0;i<m_knPnumber;i++)//��ȡ��֪������
    {
	     fscanf(fp,"%s",name);
         int k=GetStationNumber(name);
	     fscanf(fp,"%lf%lf",XY+2*k, XY+2*k+1);
    }

	if(m_Lnumber>0) //��ȡ����ֵ
	{
		dir0[0]=0;
		for(i=0;i<=m_Lnumber-1;i++) 
		{
			int ni; // ni: ��վ������
			fscanf(fp,"%s%d",name,&ni);   
			if(ni<1) 
			{
				MyBreak("\n�����ļ����󣺷�����С��1!"); 
				exit(0);
			}
			 
			dir1[i]=GetStationNumber(name);
			dir0[i+1]=dir0[i]+ni;
			for(int j=dir0[i];j<dir0[i+1];j++)
			{
				fscanf(fp,"%s%lf",name,L+j);
				dir2[j]=GetStationNumber(name); //��׼���
				L[j]=dms_rad(L[j]); // dms_arc: �ȷ��뻯����
			}
		} 
	}

    for(i=0;i<=m_Snumber-1;i++)  //��ȡ�߳�
	{
		fscanf(fp,"%s",name);   
		S_dir1[i]=GetStationNumber(name);

		fscanf(fp,"%s",name);   
		S_dir2[i]=GetStationNumber(name);

		fscanf(fp,"%lf",&S_L[i]);
	} 
	 
    for(i=0;i<=m_Tnumber-1;i++)  //��ȡ��λ��
    {
		fscanf(fp,"%s",name);   
		T_dir1[i]=GetStationNumber(name);

		fscanf(fp,"%s",name);   
		T_dir2[i]=GetStationNumber(name);

		fscanf(fp,"%lf",&(T_L[i]));
		T_L[i]=dms_rad(T_L[i]);
    }

    fclose(fp);
}

//////////////////////////////////////////////////////////////////////////
//  ԭʼ�������
void CPlaneNetAdjust::PrintData()
{
     fprintf(resultfp," �ܵ���: %d\n ����������: %d\n ����ֵ����: %d\n",
					 m_Pnumber,m_Lnumber,m_Nnumber);
     fprintf(resultfp," �߳�����: %d\n ��λ������: %d  \n",
						m_Snumber,m_Tnumber);
	 
     fprintf(resultfp,"\n   ��֪������:\n");
     for(int i=0;i<m_knPnumber;i++)
     {
	     fprintf(resultfp,"\n%8s  ",Pname[i]);
	     fprintf(resultfp,"%12.3lf%14.3lf",XY[2*i],XY[2*i+1]);
     }

     if(m_Lnumber>0)
	 {
		 fprintf(resultfp,"\n\n   ����۲�ֵ:\n");
		 for(i=0;i<=m_Lnumber-1;i++)
		 {
     		  int k1=dir1[i];
			  for(int j=dir0[i];j<dir0[i+1];j++)
			  {
				if(j==dir0[i])
				{
					fprintf(resultfp,"%10s  %d \n",
						Pname[k1],dir0[i+1]-dir0[i]);
				}
				fprintf(resultfp,"%15s %15.3lf\n",
							Pname[dir2[j]],rad_dms(L[j]));
			  }
		 }
	 }

	 if(m_Snumber>0)
	 {
		 fprintf(resultfp,"\n   �߳��۲�ֵ:\n");
		 for(i=0;i<m_Snumber;i++)
		 {
			 int k1=S_dir1[i];
			 int k2=S_dir2[i];
			 fprintf(resultfp,"%10s %10s  %11.4lf\n",
				            Pname[k1],Pname[k2],S_L[i]);
		 }
	 }
	 
	 if(m_Tnumber>0)
	 {
		 fprintf(resultfp,"\n\n ��λ�ǹ۲�ֵ:\n");
		 for(i=0;i<=m_Tnumber-1;i++)
		 {
			 int k1=T_dir1[i];
			 int k2=T_dir2[i];
			 fprintf(resultfp,"%10s %10s  %15.3lf\n",
					 Pname[k1],Pname[k2],rad_dms(T_L[i]));
		 }
	 } 
}


//////////////////////////////////////////////////////////////////////////
//   ��������ŷ�����pΪ�����һ���н�
double CPlaneNetAdjust::Get_Angle(int p,int p1,int p2)
{
	double PAI=3.14159265358979312;
	
      int i,j,k,i1,i2;
      double A;

      for(i=0;i<=m_Lnumber-1;i++)
      {
	     i1=i2=-1;
	     k=dir1[i];
	     for(j=dir0[i];j<dir0[i+1];j++)
	     {
		      if(k!=p)break;
		      if(p1==dir2[j])i1=j;
		      if(p2==dir2[j])i2=j;
	     }
	     if(i1>=0&&i2>=0)
	     {
		      A=L[i2]-L[i1];
		      if(A<0.0)A+=2.0*PAI;
		      return A;
	     }
      }
      return -1.0;    //�Ҳ������������Ľǣ����ظ�ֵ
}

//////////////////////////////////////////////////////////////////////////
//        �����������(������)
void CPlaneNetAdjust::ca_xy0()
{
     int i,j,jj,k,k1,k2,s,f;
     double A,B,C,xA,yA,xB,yB;
     double sinA,sinB,cosA,cosB,sinC,xk,yk;
	 int unPnumber=m_Pnumber-m_knPnumber;

     for(i=0;i<m_knPnumber;i++)XY[2*i]=1.0e30;  //��������δ֪���־

      s=0;
      while(1)
      {
        s++;
	     for(i=0;i<=m_Lnumber-1;i++)
	     {
	       k=dir1[i];
	       if(XY[2*k]<1.0e20)continue;
	       for(j=dir0[i];j<dir0[i+1];j++)
	       {
	          if(XY[2*k]<1.0e20)break;
	          k2=dir2[j];
	          xB=XY[2*k2];
	          yB=XY[2*k2+1];
	          if(xB>1.0e29)continue;

	          for(jj=j+1;jj<dir0[i+1];jj++)
	          {
		           k1=dir2[jj];
		           xA=XY[2*k1];
		           yA=XY[2*k1+1];
		           if(xA>1.0e29)continue;

				   A=Get_Angle(k1,k,k2);
		           B=Get_Angle(k2,k1,k);
		           C=L[jj]-L[j];
		           if(A<0.0||B<0.0)continue;

				   double PAI=3.14159265358979312;
				   
		           sinA=sin(A);		  sinB=sin(B);		  sinC=sin(C);
         		   if(C>PAI)
		           {
		              cosB=cos(B);
		              xk=xB+((xA-xB)*cosB-(yA-yB)*sinB)*sinA/sinC;
		              yk=yB+((xA-xB)*sinB+(yA-yB)*cosB)*sinA/sinC;
		           }
		           else
		           {
		                cosA=cos(A);
		                xk=xA+((xB-xA)*cosA+(yB-yA)*sinA)*sinB/sinC;
		                yk=yA+((yB-yA)*cosA-(xB-xA)*sinA)*sinB/sinC;
		           }
					  
				   XY[2*k]=xk;
				   XY[2*k+1]=yk;
	               break;
	          } //jj
	       } //j
	     }
	     f=1;
	     for(i=0;i<m_Pnumber;i++)if(XY[2*i]>1.0e29){ f=0; break; }
	     if(f)break;
	     if(s>unPnumber && !f)
	     {
	        printf("\n�����е��޷����������:\n");
	        for(i=0;i<m_Pnumber;i++)
		            if(XY[2*i]>1.0e29)printf("\n%s",Pname[i]);
	        exit(0);
			getch();
	     }
      }

	   fprintf(resultfp,"\n��������\n");
		
	   for(i=0;i<=m_Pnumber-1;i++)
	   { 
			 double xi=XY[2*i];
			 double yi=XY[2*i+1];
			 fprintf(resultfp,"\n%2d %3s ",i+1,Pname[i]);
			 fprintf(resultfp,"%14.3lf%12.3lf", xi,yi+500000.0);
	   } 
	   fflush(resultfp);		
}


//////////////////////////////////////////////////////////////////////////
//        ���㷽������ʽabϵ��
double CPlaneNetAdjust::ca_ab(int k1,int k2,double A[],int Ain[])
{
	const double ROU=2.062648062470963630e+05;
	const double PAI=3.14159265358979312;
	
	
	double dx=XY[2*k2]-XY[2*k1];
	double dy=XY[2*k2+1]-XY[2*k1+1];
	
    double s2=dx*dx+dy*dy;

    A[0]= dy/s2*ROU;     Ain[0]=2*k1;
    A[1]=-dx/s2*ROU;     Ain[1]=2*k1+1;
    A[2]=-dy/s2*ROU;     Ain[2]=2*k2;
    A[3]= dx/s2*ROU;     Ain[3]=2*k2+1;

    double T=atan2(dy,dx);
    if(T<0.0)T=T+2.0*PAI;
    return T;
}

//////////////////////////////////////////////////////////////////////////
//       ����߳�����ϵ��
double CPlaneNetAdjust::ca_cd(int k1,int k2,double A[],int Ain[])
{
	double dx=XY[2*k2]-XY[2*k1];
	double dy=XY[2*k2+1]-XY[2*k1+1];
    double s = sqrt(dx*dx+dy*dy);

    A[0]=-dx/s;     Ain[0]=2*k1;
    A[1]=-dy/s;     Ain[1]=2*k1+1;
    A[2]= dx/s;     Ain[2]=2*k2;
    A[3]= dy/s;     Ain[3]=2*k2+1;
    return s;
}

//////////////////////////////////////////////////////////////////////////
//	һ������ʽ��ɵķ�����
void CPlaneNetAdjust::ca_ATPAi(double B[],int Bin[],double p,double Li,int m)
{
    int k,s,i,j;
    double ai,aj;

    for(k=0;k<m;k++)
    {
		i=Bin[k];
	    ai=B[k];
	    ATPL[i]-=p*ai*Li;
	    for(s=0;s<m;s++)
	    {
			j=Bin[s];
	        if(i>j)continue;
	        aj=B[s];
	        ATPA[ij(i,j)]+=p*ai*aj;
		} 
	} 
} 

//////////////////////////////////////////////////////////////////////////
//         �� �� �� �� �� ʽ
void CPlaneNetAdjust::ca_ATPA()
{
	const double ROU=2.062648062470963630e+05;
	const double PAI=3.14159265358979312;	
	
	double B[5];
    int Bin[5];

	int t=2*m_Pnumber+m_Lnumber;
    int tt=t*(t+1)/2;
    for(int i=0;i<=tt-1;i++) ATPA[i]=0.0;
    for(i=0;i<=t-1;i++) ATPL[i]=0.0;

	//---------------------------------------
	//  ����ֵ��ɷ�����
	double Pi=1.0/(ma*ma);
	B[4]=-1.0;
    for( i=0;i<=m_Lnumber-1;i++)
	{
		int k1=dir1[i];
		Bin[4]=2*m_Pnumber+i; //����Ǹ�������δ֪�����
		
		double z;     //����ǽ���ֵ
		for(int j=dir0[i];j<dir0[i+1];j++)
		{
			if(!Usable[j])continue;
			int k2=dir2[j];
			double T=ca_T12(k1,k2); //����ֵ����λ��
			z=T-L[j];
			if(z<0.0) z+=2.0*PAI;
			break;
		}
		
		for(j=dir0[i];j<dir0[i+1];j++)
		{
			int k2=dir2[j];
			double T12=ca_ab(k1,k2,B,Bin);
			double Lj=T12-L[j];
			if(Lj<0.0)Lj+=2.0*PAI;
			Lj=(Lj-z)*ROU;
			V[j]=Lj; //���������V���������в�ʱʹ��
			if(Usable[j]) 
			{
				ca_ATPAi(B,Bin,Pi,Lj,5);
			}
		}    
	} 
	
	//---------------------------------------
	//  �߳���ɷ�����
	for(i=0;i<=m_Snumber-1;i++)
	{
		if(!Usable[i+m_Nnumber])continue;
		int k1=S_dir1[i];
		int k2=S_dir2[i];

		double m=mS1+mS2*S_L[i];
		double Pi=1.0/(m*m+1.0e-15); // �߳���Ȩ
		  
		double S12=ca_cd(k1,k2,B,Bin);
		double Li=S12-S_L[i];  
		ca_ATPAi(B,Bin,Pi,Li,4);
	} 
	
	//---------------------------------------
	//  ��λ����ɷ�����
	Pi=1.0/(mT*mT+1.0e-15); //��λ�ǵ�Ȩ
    for(i=0;i<=m_Tnumber-1;i++)
	{
		if(!Usable[i+m_Nnumber+m_Snumber])continue;
		int k1=T_dir1[i];
		int k2=T_dir2[i];
		  
		double T12=ca_ab(k1,k2,B,Bin);
		double Li=(T12-T_L[i])*ROU;  
		ca_ATPAi(B,Bin,Pi,Li,4);
	} 
}


//////////////////////////////////////////////////////////////////////////
//  ����ƽ��ֵ����,����ֵ����������������ֵ
double CPlaneNetAdjust::ca_dX()
{
     int t=2*m_Pnumber+m_Lnumber; //δ֪������
     if(!inverse(ATPA,t)) //������ϵ����������
	 {
		 MyBreak("����ca_dX��������������ϵ�������ȣ�");
		 fclose(resultfp);
		 exit(0);
	 }

	 double max=0.0; //��������������ֵ
     for(int i=0;i<t;i++)
     {
		  double xi=0.0;
		  for(int j=0;j<t;j++)
		  {
			  xi+=ATPA[ij(i,j)]*ATPL[j];
		  }
		  dX[i]=xi;
		  if(i<2*m_Pnumber)
		  {
			  XY[i]+=xi;
			  if(fabs(xi)>max)max=fabs(xi);
		  }
     }
	 return max;
}

//////////////////////////////////////////////////////////////////////////
//     ������С���˲в[pvv]
double  CPlaneNetAdjust::ca_V()
{
	m_pvv=0.0;

	double A[5];
	int Ain[5];

	//---------------------------------------
	//  ����ֵ�в����
	A[4]=-1.0;

	double Pi=1.0/(ma*ma); //����ֵ��Ȩ
	for(int i=0;i<=m_Lnumber-1;i++)
	{
		int k1=dir1[i];
		Ain[4]=2*m_Pnumber+i;

		for(int j=dir0[i];j<dir0[i+1];j++)
		{
			int k2=dir2[j];
			
			double T=ca_ab(k1,k2,A,Ain);

			double vj=V[j];
			for(int s=0;s<5;s++)
			{
				int k=Ain[s]; 
				vj+=A[s]*dX[k];
			}
			V[j]=vj;
			if(Usable[j])
				m_pvv += vj*vj*Pi;
		}
	}

	//---------------------------------------
	//  �߳��в����
	for(i=0;i<=m_Snumber-1;i++)
	{
		int k1=S_dir1[i];
		int k2=S_dir2[i];

		double S12=ca_cd(k1,k2,A,Ain);

		double vi=S12-S_L[i];
		S_V[i] = vi;
		double m=mS1+mS2*S_L[i];
		Pi=1.0/(m*m);

		if(Usable[m_Nnumber+i])
			m_pvv += Pi*vi*vi;
	}
	  
	//---------------------------------------
	//  ��λ�ǲв����
	Pi=1.0/(mT*mT);
	for(i=0;i<=m_Tnumber-1;i++)
	{
		int k1=T_dir1[i];
		int k2=T_dir2[i];

		double T12=ca_T12(k1,k2);

		double vi=(T12-T_L[i])*206264.806247;  
		T_V[i] = vi;
		if(Usable[m_Nnumber+m_Snumber+i])
			m_pvv += Pi*vi*vi;
	}
	return m_pvv;
}


#include "Probability.h"

//////////////////////////////////////////////////////////////////////////
//     �ֲ�̽����㺯��
double  CPlaneNetAdjust::Snooping(double arfa)
{
	//caxy0();
	ca_x0y0();
	CProbability prty;
	double U=prty.re_norm(1.0-arfa/2.0);

	int no=0; //�ֲ���������
	while(1)
	{
		ca_ATPA();
		for(int i=0;i<m_knPnumber;i++) 
		{
			ATPA[ij(2*i,2*i)]+=1.0e20;
			ATPA[ij(2*i+1,2*i+1)]+=1.0e20;
		}
		double max=ca_dX();
		double m_pvv=ca_V();
		
		double F[5];
		int Fin[5];
		
		double max_v=0.0;
		int    max_i;

		F[4]=-1.0;
		double m2 = ma*ma;  //����۲�ֵ�������ƽ��
		for( i=0;i<=m_Lnumber-1;i++)
		{
			int k1=dir1[i];
			Fin[4]=2*m_Pnumber+i;
			
			for(int j=dir0[i];j<dir0[i+1];j++)
			{
				if(!Usable[j])continue;
				int k2=dir2[j];
				
				ca_ab(k1,k2,F,Fin);
				
				double q=Calculate_q(ATPA,F,Fin,5);
				double mv = sqrt(m2-q);
				double vj=V[j]/(mv+1.0e-12);
				if(fabs(vj)>max_v)
				{
					max_v=fabs(vj);
					max_i=j;
				}
			}
		}
		
		for(i=0;i<=m_Snumber-1;i++)
		{
			if(!Usable[i+m_Nnumber])continue;
			int k1=S_dir1[i];
			int k2=S_dir2[i];
			
			double S12=ca_cd(k1,k2,F,Fin);
			
			double m=mS1+mS2*S_L[i];
			
			double q=Calculate_q(ATPA,F,Fin,4);
			double mv = sqrt(m*m-q);
			double v=S_V[i]/(mv+1.0e-12);
			if(fabs(v)>max_v)
			{
				max_v=fabs(v);
				max_i=m_Nnumber+i;
			}
		}
		
		m2 = mT*mT;
		for(i=0;i<=m_Tnumber-1;i++)
		{
			if(!Usable[i+m_Nnumber+m_Snumber])continue;
			int k1=T_dir1[i];
			int k2=T_dir2[i];
			
			double T12=ca_T12(k1,k2);
			
			double q=Calculate_q(ATPA,F,Fin,4);
			double mv = sqrt(m2-q);
			double v=T_V[i]/(mv+1.0e-12);
			if(fabs(v)>max_v)
			{
				max_v=fabs(v);
				max_i=m_Nnumber+m_Snumber+i;
			}
		}

		if(max_v>U)
		{
			Usable[max_i]=false;
			no++;  //�ֲ����
			continue;
		}

		int n=m_Nnumber+m_Snumber+m_Tnumber-no;
		int t=2*(m_Pnumber-m_knPnumber)+m_Lnumber;
		m_mu=sqrt(m_pvv/(n-no-t));

		fprintf(resultfp,"\n  �ֲ�̽��: ��=��%.3lf",m_mu);
		PrintResult();
		ErrorEllipse();

		if(no>0)fprintf(resultfp,"\n�ֲ������%d",no);
		else    fprintf(resultfp,"\n�޴ֲ�");

		break;
	}
		
	return m_pvv;
}




//////////////////////////////////////////////////////////////////////////
//   ƽ��ɹ����
void  CPlaneNetAdjust::PrintResult()
{
	int i,j,k1,k2;
	double m1,xi,yi,dxi,dyi,m2;

	fprintf(resultfp,"\n\n            ====  ����ƽ��ֵ���侫�� ====\n");
	fprintf(resultfp,"\nNo.   P        X            Y      "
		"   mx     my       M\n");

	for(i=0;i<=m_Pnumber-1;i++)
	{
		xi=XY[2*i];
		yi=XY[2*i+1];
		dxi=dX[2*i];
		dyi=dX[2*i+1];
		fprintf(resultfp,"\n%2d %3s ",i+1,Pname[i]);
		fprintf(resultfp,"%14.3lf %12.3lf", xi,yi);

		m1=sqrt(ATPA[ij(2*i,2*i)])*m_mu;
		fprintf(resultfp,"%7.3lf",m1);
		m2=sqrt(ATPA[ij(2*i+1,2*i+1)])*m_mu;
		fprintf(resultfp," %7.3lf",m2);
		fprintf(resultfp," %7.3lf",sqrt(m1*m1+m2*m2));
	}

	if(m_Lnumber>0)
	{
		fprintf(resultfp,"\n\n%20s","");
		fprintf(resultfp,"====   ����۲�ֵƽ��ɹ�  ====\n");
		fprintf(resultfp,"\n    P1     P2        L        V"
					"        T         mT        S       ms");
		for(i=0;i<=m_Lnumber-1;i++)
		{
			k1=dir1[i];
			for(j=dir0[i];j<dir0[i+1];j++)
			{
				double T,S,q,A[5];
				int Ain[5];
				k2=dir2[j];
				if(j==dir0[i])fprintf(resultfp,"\n\n%6s",Pname[k1]);
				else fprintf(resultfp,"\n      ");
				fprintf(resultfp,"%7s",Pname[k2]);
				fprintf(resultfp,"%13.2lf%7.2lf",rad_dms(L[j]),V[j]);

				T=ca_ab(k1,k2,A,Ain);
				q=sqrt(qq(A,Ain))*m_mu;
				fprintf(resultfp,"%13.2lf %5.2lf",rad_dms(T),q);

				S=ca_cd(k1,k2,A,Ain);
				q=sqrt(qq(A,Ain))*m_mu;
				fprintf(resultfp,"%12.3lf %6.3lf",S,q);
				if(!Usable[j])fprintf(resultfp," �дֲ�");
			}
		} 
	} 

	if(m_Snumber>0)
	{
		fprintf(resultfp,"\n\n%25s"," ");
		fprintf(resultfp,"====   �߳��۲�ֵƽ��ɹ�  ====\n");
		fprintf(resultfp,"\n    P1     P2         S        Vs"
		"          T         mT        S+Vs     ms");

		double A[5];
		int Ain[5];

		for(i=0;i<=m_Snumber-1;i++)
		{
			int k1=S_dir1[i];
			int k2=S_dir2[i];

			fprintf(resultfp,"\n%6s",Pname[k1]);
			fprintf(resultfp,"%7s",Pname[k2]);
			fprintf(resultfp,"%13.4lf %8.4lf ",S_L[i],S_V[i]);


			double T=ca_ab(k1,k2,A,Ain);
			double mT=sqrt(qq(A,Ain))*m_mu;
			fprintf(resultfp,"%13.3lf %6.3lf",rad_dms(T),mT);
			double S=ca_cd(k1,k2,A,Ain);
			double mS=sqrt(qq(A,Ain))*m_mu;
			fprintf(resultfp,"%12.3lf  %6.3lf",S,mS);

			if(!Usable[m_Nnumber+i])fprintf(resultfp," �дֲ�");
		} 
	}

	if(m_Tnumber>0)
	{
		fprintf(resultfp,"\n\n%20s","");
		fprintf(resultfp,"====   ��λ�ǹ۲�ֵƽ��ɹ�  ====\n");
		fprintf(resultfp,"\n    P1     P2        T          V"
		"         T+V         mT        S        ms");

		double A[5];
		int Ain[5];

		for(i=0;i<=m_Tnumber-1;i++)
		{
			int k1=T_dir1[i];
			int k2=T_dir2[i];

			fprintf(resultfp,"\n%6s",Pname[k1]);
			fprintf(resultfp,"%7s",Pname[k2]);
			fprintf(resultfp,"%14.3lf %8.4lf ",	rad_dms(T_L[i]),T_V[i]);

			double T=ca_ab(k1,k2,A,Ain);
			double mT=sqrt(qq(A,Ain))*m_mu;
			fprintf(resultfp,"%13.3lf %6.3lf",rad_dms(T),mT);
			double S=ca_cd(k1,k2,A,Ain);
			double mS=sqrt(qq(A,Ain))*m_mu;
			fprintf(resultfp,"%12.3lf  %6.3lf",S,mS);
			if(!Usable[m_Nnumber+m_Snumber+i])
				fprintf(resultfp," �дֲ�");
		}
	} 
}



//////////////////////////////////////////////////////////////////////////
//            ƽ�淽λ�Ǽ���
double CPlaneNetAdjust::ca_T12(int k1,int k2)  // ����ֵΪ����ֵ,
{
	const double PAI=3.14159265358979312;

	double dx=XY[2*k2]-XY[2*k1];
	double dy=XY[2*k2+1]-XY[2*k1+1];

	double T=atan2(dy,dx);
	if(T<0.0)T=T+2.0*PAI;

	return T;
}



//////////////////////////////////////////////////////////////////////////
//            Ȩ �� �� �� ��
double CPlaneNetAdjust::qq(double B[],int Bin[])
{
      int i,j,k1,k2;
      double q=0.0;

      for(i=0;i<4;i++)
      {
	      k1=Bin[i];
	      for(j=0;j<4;j++)
		  {
		       k2=Bin[j];
		       q+=ATPA[ij(k1,k2)]*B[i]*B[j];
		  }
      }

	  if(fabs(q)<1.0e-8) q=0.0; // ��Ϊ�������q�ӽ�0ʱ����Ϊ����
      return q;
}



//////////////////////////////////////////////////////////////////////////
//       �߽�������ƽ��
int CPlaneNetAdjust::Quasi_Stable(char *file)
{
	ca_ATPA(); //��ɷ�����

	double *GT = ca_GT(file); 
	
	int kn=4;
	if(m_Snumber>0)kn--;
	if(m_Tnumber>0)kn--;
	
	int t=2*m_Pnumber+m_Lnumber;
	int *GT_in=new int[t];
	for(int i=0;i<t;i++)GT_in[i]=i;

	double *GT2=new double[4*t];
	for( i=0;i<4*t;i++)GT2[i]=0.0;
	
	for(i=0;i<kn; i++)
	{
		for(int j=0;j<m_Pnumber;j++)
		{
			if(IsStable[j])
			{
				GT2[i*t+2*j]=GT[i*t+2*j];
				GT2[i*t+2*j+1]=GT[i*t+2*j+1];
			}
		}
	}
	
	for(int k=0; k<kn; k++)
	{
		ca_ATPAi(GT2+k*t,GT_in,1.0,0.0,t);
	}
	
	ca_dX();

	for( k=0; k<kn; k++)
	{
		ca_ATPAi(GT+k*t,GT_in,-1.0,0.0,t);
	}

	m_pvv=ca_V();
	int n=m_Nnumber+m_Snumber+m_Tnumber;
	m_mu=sqrt(m_pvv/(n-2.0*(m_Pnumber-2)-m_Lnumber));

	fprintf(resultfp,"\n\n  ˮƽ������ƽ��: ��=��%lf",m_mu);
	PrintResult();
	
	PrintM2(resultfp, ATPA, 2*m_Pnumber,5,"%13.4e ","����ƽ��ֵ֮Ȩ����");
	
	return 1;
}	




//////////////////////////////////////////////////////////////////////////
//   ����G����(ת��)
double * CPlaneNetAdjust::ca_GT(char *file)
{
	IsStable = new bool[m_Pnumber]; // ���ȵ��־
	
	if(file==NULL) // ������ƽ�����Ҫ���ȵ����ļ�
	{
		m_StableNumber=m_Pnumber;  //���ȵ����������ܵ���
		for(int i=0;i<m_Pnumber;i++)
		{
			IsStable[i]=true;
		}
	}
	else
	{
		FILE *fp=fopen(file,"r");
		if(fp==NULL)
		{
			MyBreak("���ȵ����ļ��򲻿�:%s",file);
			fclose(resultfp);
			exit(0);
		}
		
		for(int i=0;i<m_Pnumber;i++)
		{
			IsStable[i]=false;
		}
		
		fscanf(fp,"%d",	&m_StableNumber);//���ȵ�����
		if(m_StableNumber<2)
		{
			MyBreak(" ���ȵ�����С��2��");
			fclose(resultfp);
			exit(0);
		}
		
		for(i=0;i<m_StableNumber;i++)
		{
			char buf[50];
			fscanf(fp,"%s",buf);
			int k=GetStationNumber(buf);
			if(k<0)
			{
				MyBreak("���ȵ������ļ��д��ڴ������!");
				fclose(resultfp);
				exit(0);			
			}
			IsStable[k]=true;
		}
		fclose(fp);
	}

	int t=m_Lnumber+2*m_Pnumber;

	double *GT= new double[4*t];//G�����ת��
	for(int i=0; i<4*t; i++) GT[i]=0.0;

	//--------------------------------------
	//   ��������ľ�ֵ
	double xm=0.0;
	double ym=0.0;
	for( i=0;i<m_Pnumber;i++)
	{
		if(IsStable[i])
		{
			xm+=XY[2*i];
			ym+=XY[2*i+1];
		}
	}
	xm/=m_StableNumber;
	ym/=m_StableNumber;
	
	double H=0.0;
	for(i=0;i<m_Pnumber;i++)
	{
		if(!IsStable[i])continue;
		
		double dx=XY[2*i]-xm;
		double dy=XY[2*i+1]-ym;
		H+=dx*dx+dy*dy;
	}
	H=sqrt(H);	
	double m=1.0/sqrt(m_StableNumber+0.0);
	 
	// ---------------------------------------------
	//  ���GT
	for( int k=0; k<m_Pnumber;k++)
	{
		GT[2*k]=m;      //��һ��
		GT[t+2*k+1]=m;  //�ڶ���
		
		double dx=XY[2*k]-xm;
		double dy=XY[2*k+1]-ym;
		
		//��3��
		GT[2*t+2*k]=-dy/H;
		GT[2*t+2*k+1]=dx/H;
		
		//��4��
		GT[3*t+2*k]  =dx/H;
		GT[3*t+2*k+1]=dy/H;
	}

	// ��3�� ����Ƕ�Ӧ��
	for(k=2*m_Pnumber; k<t; k++)
	{
		GT[2*t+k]=206264.806247096/H;
	}

	int kn=4;
	if(m_Snumber>0) kn=3; //�б߳��۲�ֵʱ,��������һ�в�Ҫ��
	if(m_Tnumber>0) //�з�λ�ǹ۲�ֵʱ,����ĵ����в�Ҫ��
	{
		kn--;
		for(int i=0;i<t;i++)
			GT[2*t+i]=GT[3*t+i];
	}

	fprintf(resultfp,"\n    G����ת�ã�: ");
	for( k=0; k<kn; k++)
	{
		fprintf(resultfp,"\n%2d: ",k);
		for(int i=0;i<t;i++)
		{
			fprintf(resultfp," %7.3lf",GT[k*t+i]);
		}
	}	

/*
	// --------------------------------------
	// ��֤������ϵ��������G�ĳ˻��Ƿ�Ϊ��
	double max_b=0.0; 
	for( k=0; k<kn; k++)
	{
		for(int i=0;i<t;i++)
		{
			double b=0.0;
			for(int j=0;j<t;j++)
				b+=ATPA[ij(i,j)]*GT[k*t+j]; 
			if(fabs(b)>max_b)max_b=fabs(b);
		}
	}	
	fprintf(resultfp,"\n    (N��G')=0  ���պϲ� = %e",max_b);
*/

	return GT;
}


//////////////////////////////////////////////////////////////////////////
//    ������ƽ����㺯��
void CPlaneNetAdjust::Free()
{
	ca_ATPA();

	double *GT = ca_GT(NULL); //NULL: ������ƽ��ı�־
	
	int kn=4;
	if(m_Snumber>0)kn--;
	if(m_Tnumber>0)kn--;

	int t=2*m_Pnumber+m_Lnumber;
	int *GT_in=new int[t];

	for(int i=0;i<t;i++)GT_in[i]=i;
		
	for(int k=0; k<kn; k++)
	{
		ca_ATPAi(GT+k*t,GT_in,1.0,0.0,t);
	}

	ca_dX();

	for( k=0;k<kn;k++)
	{
		ca_ATPAi(GT+k*t,GT_in,-1.0,0.0,t);
	}
	delete []GT;
	delete []GT_in;
	
	m_pvv=ca_V();
	
	int n=m_Nnumber+m_Snumber+m_Tnumber;
	m_mu=sqrt(m_pvv/(n-2.0*(m_Pnumber-2)-m_Lnumber));
	
	fprintf(resultfp,"\n  ������ƽ��: ��=��%lf",m_mu);
	
	PrintM2(resultfp, ATPA, 2*m_Pnumber,5,"%13.4e ","����ƽ��ֵ֮Ȩ����");
	PrintResult();

}



//////////////////////////////////////////////////////////////////////////
//    ��С����ƽ��
void CPlaneNetAdjust::LS_Adjust(char *data_file)
{	
	InputData(data_file);  // ����ԭʼ����
	PrintData();           // ���ԭʼ����
	
	//caxy0();
	ca_x0y0();   //�����������
	
	double max=1.0;
	while(max>0.01)
	{
		ca_ATPA();
		for(int i=0;i<m_knPnumber;i++) 
		{
			ATPA[ij(2*i,2*i)]+=1.0e20;
			ATPA[ij(2*i+1,2*i+1)]+=1.0e20;
		}
		max=ca_dX();
	}
	
	m_pvv=ca_V();
	
	int n=m_Nnumber+m_Snumber+m_Tnumber;
	int t=2*(m_Pnumber-m_knPnumber)+m_Lnumber;
	m_mu=sqrt(m_pvv/(n-t));
	
	fprintf(resultfp,"\n��С����ƽ��:��=��%.3lf\n",m_mu);
	PrintResult();
	ErrorEllipse();
		
}



//////////////////////////////////////////////////////////////////////////
//    ����������������㺯��
void CPlaneNetAdjust::ca_x0y0()
{
	int unknow=m_Pnumber-m_knPnumber; //δ֪����

	//����δ֪���־,δ֪��ĵ�Ŵ�m_knPnumber��ʼ
    for(int i=m_knPnumber;i<m_Pnumber;i++) XY[2*i]=1.0e30;
	
	for(int No=1; ; No++)
	{
		if(unknow==0) return ;
		 
        if(No>(m_Pnumber-m_knPnumber))
		{
			 fprintf(resultfp,"\n\n���ֵ���㲻����������:");
			 for(int k=0;k<m_Pnumber;k++)
			 {
				 if(XY[2*k]>1.0e29)
				 {
					 fprintf(resultfp,"\n%s",Pname[k]);
				 }
			 }
			 fclose(resultfp);

			 MyBreak("���ֵ���㲻���������꣡");
			 exit(0);
		} 

	    for( i=0;i<=m_Lnumber-1;i++) // ��������ѭ������������۲�ֵ
		{ 
			int k1=dir1[i];  //��վ���

			double x1=XY[2*k1];   //��վ���x����
			double y1=XY[2*k1+1]; //��վ���y����
			if(x1>1.0e29) continue; //��վ����δ֪�㣬ת��һ������

			int j0=dir0[i]; //���������׷���۲�ֵ�����
			for(int j=j0; j<dir0[i+1];j++)
			{
				int k2=dir2[j]; //��׼���
			    double T12;
			    if(XY[2*k1]<1.0e29 && XY[2*k2]<1.0e29) //k1��k2������֪��
				{ 
			 		T12=ca_T12(k1,k2); //��������㷽λ��
				}
			    else T12=Get_T12(k1,k2);  //�ڷ�λ�������в�����ʼ��λ��
			    if(T12>1.0e29) continue;  //����ʼ��λ��

			    for(int k=j0;k<dir0[i+1];k++)
				{ 
					int k3=dir2[k];
					double x3=XY[2*k3];
					if(x3<1.0e29)continue; //k3����֪��

					double S13=Get_S12(k1,k3); //�ڱ߳������в��ұ߳�
					if( S13 >1.0e29 ) continue; //�ޱ߳��۲�ֵ
					
					double T13=T12+L[k]-L[j];
					x3=S13*cos(T13);
					double y3=S13*sin(T13);

					XY[2*k3]  =x1+x3;
					XY[2*k3+1]=y1+y3;
					unknow--;
				}  
			} 
		} 
	} 
}


//////////////////////////////////////////////////////////////////////////
//    ���ұ߳��۲�ֵ
double CPlaneNetAdjust::Get_S12(int k1,int k2)
{	
	for(int i=0; i<m_Snumber; i++)
	{
		if(k1==S_dir1[i] && k2==S_dir2[i])
		{
			return S_L[i];
		}
		if(k1==S_dir2[i] && k2==S_dir1[i])
		{
			return S_L[i];
		}
	}
	
	return 1.0e30;	
}

//////////////////////////////////////////////////////////////////////////
//      �ڷ�λ�ǹ۲�ֵ�в���
double CPlaneNetAdjust::Get_T12(int k1,int k2)
{
	const double PAI=3.14159265358979312;
	
	for(int i=0; i<m_Tnumber; i++)
	{
		if(k1 == T_dir1[i] && k2 == T_dir2[i])
		{
			return T_L[i];
		}
		if(k1==T_dir2[i] && k2==T_dir1[i])
		{
			double T=T_L[i]+PAI;
			if(T>2.0*PAI)T=T-2.0*PAI;
			return T;
		}
	}

	return 1.0e30;
}



//////////////////////////////////////////////////////////////////////////
//         ���������Բ
//   2007-03-20, Ϊ����̲ı�ĺ���.
void CPlaneNetAdjust::ErrorEllipse( )
{
	const double PAI=3.14159265358979312;
	
	double m2=m_mu*m_mu;
	fprintf(resultfp,"\n\n            ==== ��λ�����Բ ====\n\n");
	fprintf(resultfp,"   ����   ��Բ������  ��Բ�̰���  ���᷽λ��\n");
	for(int i=0;i<m_Pnumber; i++)
	{
		double mx2=ATPA[ij(2*i,2*i)]*m2;  //x����������ƽ��
		double my2=ATPA[ij(2*i+1,2*i+1)]*m2; //y����������ƽ��
		double mxy=ATPA[ij(2*i,2*i+1)]*m2; //xy�����Э����

		if(sqrt(mx2+my2)<0.0001) continue;

		double K=sqrt((mx2-my2)*(mx2-my2)+4.0*mxy*mxy);
		
		double E=sqrt(0.5*(mx2+my2+K));  //����
		double F=sqrt(0.5*(mx2+my2-K));  //����
		
		double A; //�����Բ����ķ�λ��
		if(fabs(mxy)<1.0e-14)  // mxy = 0
		{
			if(mx2>my2) A=0.0;
			else A=0.5*PAI;
		}
		else   // mxy �� 0
		{
			A=atan( (E*E-mx2)/mxy);
		}
		if(A<0.0)A+=PAI;

		fprintf(resultfp,"\n %6s  %8.3lf  %10.3lf %13.2lf",
							Pname[i],E,F,rad_dms(A));
	}
}

