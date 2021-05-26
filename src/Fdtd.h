

#ifndef FDTD_H
#define FDTD_H


//#include <QtGui>


//#include <math.h>
//#include <iostream>

using namespace std;

class Fdtd {

	struct cell{
		double E[3];
		double H[3];
		double epsilon_r[3];
		double mu_r[3];
		
		double sigma[3];
		double curlE[3];
		double curlH[3];
		double intCE[3];
		double intCH[3];
		
		double intH[3];
		double intE[3];
	};
	
	private:
		
		void setIsotropic(int x, int y, double epsilon_value);
		void rectangle(int x1, int x2, int y1, int y2, double epsilon, double mu);
		double part_H_x(int j, int x, int y);
		double part_H_y(int j, int x, int y);
		double part_E_x(int j, int x, int y);
		double part_E_y(int j, int x, int y);
		
		void updateE(int x, int y);
		void updateH(int x, int y);
		
	public: 
		
		struct point{
			int x, y;
		};
		
		int t; // time index
		double deltat;
		double deltax;
		double deltay;
		
		
		int nx;
		int ny;
		int pmlSize;
		
		cell ** grid;
		
		static const double c = 299792458; // m/s
		double cdt;
		
		Fdtd(int xsize, int ysize);
		void algorithmstep();
		
};

#endif

