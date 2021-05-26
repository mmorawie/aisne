

#ifndef FDTD3D_H
#define FDTD3D_H


class Fdtd3d {

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
	
		bool circle( int x, int y, int r, double epsilon);
		double dist(int x, int y, int x0, int y0);
		
		void setIsotropic(int x, int y, int z, double epsilon_value);
		
		double part_H_x(int j, int x, int y, int z);
		double part_H_y(int j, int x, int y, int z);
		double part_H_z(int j, int x, int y, int z);
		
		double part_E_x(int j, int x, int y, int z);
		double part_E_y(int j, int x, int y, int z);
		double part_E_z(int j, int x, int y, int z);
		
		
		void updateE(int x, int y, int z);		
		void updateH(int x, int y, int z);
		
		
	public: 
		
		struct point{
			int x, y, z;
		};
		
		int t; // time index
		double deltat;
		double deltax;
		double deltay;
		double deltaz;
		
		
		int nx;
		int ny;
		int nz;
		int pmlSize;
		
		cell *** grid;
		
		static const double c = 299792458; // m/s
		double cdt;
		
		Fdtd3d(int xsize, int ysize, int zsize);
		void algorithmstep();
};

#endif

