#include <stdio.h>
#include <math.h>

int main(){

	//time
	double t=0.0;
	const double Ts=1e-4;

	//state on Cartesian coordinate
	double xcmd[2]={0.0};
	double x[3]={0.0};	//x,y,theta
	double dx[3]={0.0};	//dx,dy,dtheta

	//internal variable	
	double zeta=0.0;
	double dzeta=0.0;

	//input on Cartesian coordinate and joint space
	double uc[2]={0.0};	// ux,uy
	double uj[2]={0.0}; // uv, uw

	//counter
	int i=0,j=0, k=0;
	const double sim_sec=2.0;
	const int sloop=static_cast<int>(sim_sec/Ts);
	const int ploop=500;

	FILE *fp=fopen("state.dat","w");

	for(i=0;i<sloop;i++){

		//reference
		uc[0]=1.0;
		uc[1]=1.0;
		
		//internal variable
		zeta+=dzeta*Ts;
		dzeta=uc[0]*cos(x[2])+uc[1]*sin(x[2]);
		
		//transformation
		uj[0]=zeta;
		uj[1]=(uj[0]==0)? 0.0:(cos(x[2])*uc[1]-sin(x[2])*uc[0])/uj[0];
		/* cross product: uj[0]=0 <-> uc[0]=0 & uc[1]=0; */
		
		for(j=0;j<ploop;j++){
			for(k=0;k<3;k++) x[k]+=dx[k]*(Ts/ploop);
			dx[0]=uj[0]*cos(x[2]);
			dx[1]=uj[0]*sin(x[2]);
			dx[2]=uj[1];
		};

		//logging
		fprintf(fp,"%lf %lf %lf %lf %lf %lf\n",
			t,xcmd[0],xcmd[1],x[0],x[1],x[2]*180/M_PI
		);

		//update
		t+=Ts;
	};
		
	return 0;
}
