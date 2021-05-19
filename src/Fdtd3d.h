

#ifndef FDTD3D_H
#define FDTD3D_H


#include <math.h>

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
		
		Fdtd3d(int xsize, int ysize, int zsize){
			
			nx = xsize;
			ny = ysize;
			nz = zsize;
			pmlSize = 20;
			
			cdt = 0.299792458;   // m/s * 0.1*us
			deltat = cdt/c;
			cout<<"   Delta t = "<<deltat<<endl;
			t = 0;
			deltax = 1; // 0.1*um
			deltay = 1; // 0.1*um
			deltaz = 1; // 0.1*um
			grid = new cell ** [nx];
			
			for(int i = 0; i < nx; i++) {
				grid[i] = new cell*[ny];
				for(int j = 0; j < ny; j++){
					grid[i][j] = new cell[nz];
					for(int k = 0; k< nz; k++){
						for(int w = 0; w < 3; w++){
							grid[i][j][k].E[w] = 0;
							grid[i][j][k].H[w] = 0;
							grid[i][j][k].epsilon_r[w] = 1.0;
							grid[i][j][k].mu_r[w] = 1;
							
							grid[i][j][k].curlH[w] = grid[i][j][k].curlE[w] = 0;
							grid[i][j][k].intCE[w] = grid[i][j][k].intCH[w] = 0;
							
							grid[i][j][k].sigma[w] = 0;
							
							grid[i][j][k].intE[w] = 0;
							grid[i][j][k].intH[w] = 0;
							
						}
						double fullSigma = 100000000;
						if(i < pmlSize) grid[i][j][k].sigma[0] = fullSigma * (pmlSize - i) / pmlSize;
						if(i > nx - pmlSize) grid[i][j][k].sigma[0] = fullSigma * (i - nx + pmlSize) / pmlSize;
						
						if(j < pmlSize) grid[i][j][k].sigma[1] = fullSigma * (pmlSize - j) / pmlSize;
						if(j > ny - pmlSize) grid[i][j][k].sigma[1] = fullSigma *  (j - ny + pmlSize) / pmlSize;
						
						if(k < pmlSize) grid[i][j][k].sigma[2] = fullSigma * (pmlSize - k) / pmlSize;
						if(k > nz - pmlSize) grid[i][j][k].sigma[2] = fullSigma *  (k - nz + pmlSize) / pmlSize;
					}// next k
				}// next j
			}// next i
			
			
				
			circle(50,50, 30, 1.5);
			circle(50,50, 15, 4.5);
			
			
			for(int i = 0; i<nx; i++){
				for(int j = 0; j<ny; j++){
					for(int k = 0; k<nz; k++){
						if(k > 40 && k < 80 && i > 60) setIsotropic(i,j,k, 1);
						if(k > 50 && k < 70 && i > 50) setIsotropic(i,j,k, 1);
					}
				}
			}
			
			
		};
		
		bool circle( int x, int y, int r, double epsilon){
			for(int i = 0; i<nx; i++){
				for(int j = 0; j<ny; j++){
					for(int k = 0; k<nz; k++){
						if ( dist(i,j, x,y) < r){
							setIsotropic(i,j,k, epsilon);
						}
					}
				}
			}
			
		};
		
		double dist(int x, int y, int x0, int y0){
			return sqrt(pow((double) x - x0, 2) + pow((double) y - y0, 2));
		};
		
		void setIsotropic(int x, int y, int z, double epsilon_value){
			grid[x][y][z].epsilon_r[0] = grid[x][y][z].epsilon_r[1] = grid[x][y][z].epsilon_r[2] = epsilon_value;
		};
		
		double part_H_x(int j, int x, int y, int z){
			return (grid[x][y][z].H[j] - grid[x-1][y][z].H[j]) / deltax;
		}
		
		double part_H_y(int j, int x, int y, int z){
			return (grid[x][y][z].H[j] - grid[x][y-1][z].H[j]) / deltay;
		}
	
		double part_H_z(int j, int x, int y, int z){
			return (grid[x][y][z].H[j] - grid[x][y][z-1].H[j]) / deltaz;
		}
		
		double part_E_x(int j, int x, int y, int z){
			return (grid[x+1][y][z].E[j] - grid[x][y][z].E[j]) / deltax;
		}
		
		double part_E_y(int j, int x, int y, int z){
			return (grid[x][y+1][z].E[j] - grid[x][y][z].E[j]) / deltay;
		}
		
		double part_E_z(int j, int x, int y, int z){
			return (grid[x][y][z+1].E[j] - grid[x][y][z].E[j]) / deltaz;
		}
		
		#ifndef PML
		
		void updateE(int x, int y, int z){
			grid[x][y][z].E[0] = grid[x][y][z].E[0] + cdt / grid[x][y][z].epsilon_r[0] * ( + part_H_y(2,x,y,z) - part_H_z(1,x,y,z) );
			grid[x][y][z].E[1] = grid[x][y][z].E[1] + cdt / grid[x][y][z].epsilon_r[1] * ( - part_H_x(2,x,y,z) + part_H_z(0,x,y,z) );
			grid[x][y][z].E[2] = grid[x][y][z].E[2] + cdt / grid[x][y][z].epsilon_r[2] * ( + part_H_x(1,x,y,z) - part_H_y(0,x,y,z));
		};
		
		void updateH(int x, int y, int z){
			grid[x][y][z].H[0] = grid[x][y][z].H[0] - cdt / grid[x][y][z].mu_r[0] * ( + part_E_y(2,x,y,z) - part_E_z(1,x,y,z) );
			grid[x][y][z].H[1] = grid[x][y][z].H[1] - cdt / grid[x][y][z].mu_r[1] * ( - part_E_x(2,x,y,z) + part_E_z(0,x,y,z) );
			grid[x][y][z].H[2] = grid[x][y][z].H[2] - cdt / grid[x][y][z].mu_r[2] * ( + part_E_x(1,x,y,z) - part_E_y(0,x,y,z));
		};
		
		#else
		
		void updateE(int x, int y, int z){
			grid[x][y][z].curlH[0] = ( + part_H_y(2,x,y,z) - part_H_z(1,x,y,z));
			grid[x][y][z].curlH[1] = ( - part_H_x(2,x,y,z) + part_H_z(0,x,y,z));
			grid[x][y][z].curlH[2] = ( + part_H_x(1,x,y,z) - part_H_y(0,x,y,z));
			for(int k = 0; k < 3; k++) grid[x][y][z].intCH[k] = grid[x][y][z].intCH[k] + deltat * grid[x][y][z].curlH[k];
			for(int k = 0; k < 3; k++) grid[x][y][z].intE[k] += deltat * grid[x][y][z].E[k];
			
			for(int k = 0; k < 3; k++) 
				grid[x][y][z].E[k] = 
					(
					grid[x][y][z].E[k] 
					- ( grid[x][y][z].sigma[(k+1)%3] + grid[x][y][z].sigma[(k+2)%3] ) /2 * grid[x][y][z].E[k]  * deltat
					- ( grid[x][y][z].sigma[(k+1)%3] * grid[x][y][z].sigma[(k+2)%3] ) /4 * grid[x][y][z].E[k]  * deltat * deltat
					- ( grid[x][y][z].sigma[(k+1)%3] * grid[x][y][z].sigma[(k+2)%3] ) * grid[x][y][z].intE[k] * deltat
					+ cdt / grid[x][y][z].epsilon_r[k] * grid[x][y][z].curlH[k]
					+ cdt * grid[x][y][z].sigma[k] / grid[x][y][z].epsilon_r[k] * grid[x][y][z].intCH[k]
					) / (1 
								+ ( grid[x][y][z].sigma[(k+1)%3] + grid[x][y][z].sigma[(k+2)%3] ) /2 * deltat
								+ ( grid[x][y][z].sigma[(k+1)%3] * grid[x][y][z].sigma[(k+2)%3] ) /4 * deltat * deltat
					)
					;	
		};
		
		void updateH(int x, int y, int z){
			grid[x][y][z].curlE[0] = ( + part_E_y(2,x,y,z) - part_E_z(1,x,y,z) );
			grid[x][y][z].curlE[1] = ( - part_E_x(2,x,y,z) + part_E_z(0,x,y,z));
			grid[x][y][z].curlE[2] = ( + part_E_x(1,x,y,z) - part_E_y(0,x,y,z));
			for(int k = 0; k < 3; k++) grid[x][y][z].intCE[k] = grid[x][y][z].intCE[k] + deltat * grid[x][y][z].curlE[k];
			for(int k = 0; k < 3; k++) grid[x][y][z].intH[k] += deltat * grid[x][y][z].H[k];
			
			for(int k = 0; k < 3; k++) 
				grid[x][y][z].H[k] = 
					(
					grid[x][y][z].H[k] 
					- ( grid[x][y][z].sigma[(k+1)%3] + grid[x][y][z].sigma[(k+2)%3] ) /2 * grid[x][y][z].H[k]  * deltat
					- ( grid[x][y][z].sigma[(k+1)%3] * grid[x][y][z].sigma[(k+2)%3] ) /4 * grid[x][y][z].H[k]  * deltat * deltat
					- ( grid[x][y][z].sigma[(k+1)%3] * grid[x][y][z].sigma[(k+2)%3] ) * grid[x][y][z].intH[k] * deltat
					- cdt / grid[x][y][z].mu_r[k] * grid[x][y][z].curlE[k]
					- cdt * grid[x][y][z].sigma[k] / grid[x][y][z].mu_r[k] * grid[x][y][z].intCE[k]
					) / (1 
								+ ( grid[x][y][z].sigma[(k+1)%3] + grid[x][y][z].sigma[(k+2)%3] ) /2 * deltat
								+ ( grid[x][y][z].sigma[(k+1)%3] * grid[x][y][z].sigma[(k+2)%3] ) /4 * deltat * deltat
					)
					;
					
			
			
		};
		
		#endif
		
		
		void algorithmstep(){
			
			for(int i = 1; i < nx-1; i++)
				for(int j = 1; j < ny-1; j++)
					for(int k = 1; k < nz-1; k++)
						updateE(i,j,k);
					
			for(int i = 1; i < nx-1; i++)
				for(int j = 1; j < ny-1; j++)
					for(int k = 1; k < nz-1; k++)
						updateH(i,j,k);
					
			t = t + 1;
			
		}
		
};

#endif

