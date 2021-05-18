

#ifndef FDTD_H
#define FDTD_H


#include <math.h>

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
		
		Fdtd(int xsize, int ysize){
			
			nx = xsize;
			ny = ysize;
			pmlSize = 30;
			
			cdt = 0.299792458;   // m/s * 0.1*us
			deltat = cdt/c;
			cout<<"   Delta t = "<<deltat<<endl;
			t = 0;
			deltax = 1; // 0.1*um
			deltay = 1; // 0.1*um
			
			grid = new cell * [nx];
			
			for(int i = 0; i < nx; i++) {
				grid[i] = new cell[ny];
				
				for(int j = 0; j < ny; j++){
					for(int k = 0; k < 3; k++){
						grid[i][j].E[k] = 0;
						grid[i][j].H[k] = 0;
						grid[i][j].epsilon_r[k] = 1.45;
						grid[i][j].mu_r[k] = 1;
						
						grid[i][j].curlH[k] = grid[i][j].curlE[k] = 0;
						grid[i][j].intCE[k] = grid[i][j].intCH[k] = 0;
						
						grid[i][j].sigma[k] = 0;
						
						grid[i][j].intE[k] = 0;
						grid[i][j].intH[k] = 0;
						
					}
					double fullSigma = 100000000;
					if(i < pmlSize) grid[i][j].sigma[0] = fullSigma * (pmlSize - i) / pmlSize;
					if(i > nx - pmlSize) grid[i][j].sigma[0] = fullSigma * (i - nx + pmlSize) / pmlSize;
					
					if(j < pmlSize) grid[i][j].sigma[1] = fullSigma * (pmlSize - j) / pmlSize;
					if(j > ny - pmlSize) grid[i][j].sigma[1] = fullSigma *  (j - ny + pmlSize) / pmlSize;
				}
			}
			
			
			
			QImage picture("patterns/splitter.jpg");
			
			for(int i = 0; i<nx; i++){
				for(int j = 0; j<ny; j++){
					if( qBlue( picture.pixel(i,j) ) < 128 ){
						setIsotropic(i,j, 2.25, 1.4);
					}
				}
			
			}
			
		};
		
		void setIsotropic(int x, int y, double epsilon_value, double mu_value){
			grid[x][y].epsilon_r[0] = grid[x][y].epsilon_r[1] = grid[x][y].epsilon_r[2] = epsilon_value;
			//grid[x][y].mu_r[0] = grid[x][y].mu_r[1] = grid[x][y].mu_r[2] = mu_value;
		};
		
		void rectangle(int x1, int x2, int y1, int y2, double epsilon, double mu){
			for(int x = x1; x < x2; x++){
				for(int y = y1; y < y2; y++){
					grid[x][y].epsilon_r[0] = grid[x][y].epsilon_r[1] = grid[x][y].epsilon_r[2] = epsilon;
					grid[x][y].mu_r[0] = grid[x][y].mu_r[1] = grid[x][y].mu_r[2] = mu;
				}
			}
			
		};
		
		void setPML(int border){
			double tangential_epsilon = 5;
			double tangential_mu = 5;
			for(int i = 0; i < border; i++) {
				for(int j = i; j < ny - i; j++){
					setIsotropic(i,j, 1 + i*0.2, 1 + i*0.05);
					setIsotropic(nx-i-1,j, 1 + i*0.2, 1 + i*0.05);
				}
			}
			
			for(int j = 0; j < border; j++) {
				for(int i = j; i < nx - j; i++){
					setIsotropic(i,j, 1 + j*0.2, 1 + j*0.05);
					setIsotropic(i,ny-j-1, 1 + j*0.2, 1 + j*0.05);
				}
			}
		};
		
		double part_H_x(int j, int x, int y){
			return (grid[x][y].H[j] - grid[x-1][y].H[j]) / deltax;
		}
		
		double part_H_y(int j, int x, int y){
			return (grid[x][y].H[j] - grid[x][y-1].H[j]) / deltay;
		}
		
		double part_E_x(int j, int x, int y){
			return (grid[x+1][y].E[j] - grid[x][y].E[j]) / deltax;
		}
		
		double part_E_y(int j, int x, int y){
			return (grid[x][y+1].E[j] - grid[x][y].E[j]) / deltay;
		}
		
	//	#define PML
		#ifndef PML
		
		void updateE(int x, int y){
			grid[x][y].E[0] = grid[x][y].E[0] + cdt / grid[x][y].epsilon_r[0] * ( + part_H_y(2,x,y) );
			grid[x][y].E[1] = grid[x][y].E[1] + cdt / grid[x][y].epsilon_r[1] * ( - part_H_x(2,x,y) );
			grid[x][y].E[2] = grid[x][y].E[2] + cdt / grid[x][y].epsilon_r[2] * ( + part_H_x(1,x,y) - part_H_y(0,x,y));
		};
		
		void updateH(int x, int y){
			grid[x][y].H[0] = grid[x][y].H[0] - cdt / grid[x][y].mu_r[0] * ( + part_E_y(2,x,y) );
			grid[x][y].H[1] = grid[x][y].H[1] - cdt / grid[x][y].mu_r[1] * ( - part_E_x(2,x,y) );
			grid[x][y].H[2] = grid[x][y].H[2] - cdt / grid[x][y].mu_r[2] * ( + part_E_x(1,x,y) - part_E_y(0,x,y));
		};
		
		#else
		
		void updateE(int x, int y){
			grid[x][y].curlH[0] = ( + part_H_y(2,x,y) );
			grid[x][y].curlH[1] = ( - part_H_x(2,x,y) );
			grid[x][y].curlH[2] = ( + part_H_x(1,x,y) - part_H_y(0,x,y));
			for(int k = 0; k < 3; k++) grid[x][y].intCH[k] = grid[x][y].intCH[k] + deltat * grid[x][y].curlH[k];
			for(int k = 0; k < 3; k++) grid[x][y].intE[k] += deltat * grid[x][y].E[k];
			
			for(int k = 0; k < 3; k++) 
				grid[x][y].E[k] = 
					(
					grid[x][y].E[k] 
					- ( grid[x][y].sigma[(k+1)%3] + grid[x][y].sigma[(k+2)%3] ) /2 * grid[x][y].E[k]  * deltat
					- ( grid[x][y].sigma[(k+1)%3] * grid[x][y].sigma[(k+2)%3] ) /4 * grid[x][y].E[k]  * deltat * deltat
					- ( grid[x][y].sigma[(k+1)%3] * grid[x][y].sigma[(k+2)%3] ) * grid[x][y].intE[k] * deltat
					+ cdt / grid[x][y].epsilon_r[k] * grid[x][y].curlH[k]
					+ cdt * grid[x][y].sigma[k] / grid[x][y].epsilon_r[k] * grid[x][y].intCH[k]
					) / (1 
								+ ( grid[x][y].sigma[(k+1)%3] + grid[x][y].sigma[(k+2)%3] ) /2 * deltat
								+ ( grid[x][y].sigma[(k+1)%3] * grid[x][y].sigma[(k+2)%3] ) /4 * deltat * deltat
					)
					;
				
			//grid[x][y].E[1] = grid[x][y].E[1] + cdt / grid[x][y].epsilon_r[1] * curlH[1];
			//grid[x][y].E[2] = grid[x][y].E[2] + cdt / grid[x][y].epsilon_r[2] * curlH[2];
			
		};
		
		void updateH(int x, int y){
			grid[x][y].curlE[0] = ( + part_E_y(2,x,y) );
			grid[x][y].curlE[1] = ( - part_E_x(2,x,y) );
			grid[x][y].curlE[2] = ( + part_E_x(1,x,y) - part_E_y(0,x,y));
			for(int k = 0; k < 3; k++) grid[x][y].intCE[k] = grid[x][y].intCE[k] + deltat * grid[x][y].curlE[k];
			for(int k = 0; k < 3; k++) grid[x][y].intH[k] += deltat * grid[x][y].H[k];
			
			for(int k = 0; k < 3; k++) 
				grid[x][y].H[k] = 
					(
					grid[x][y].H[k] 
					- ( grid[x][y].sigma[(k+1)%3] + grid[x][y].sigma[(k+2)%3] ) /2 * grid[x][y].H[k]  * deltat
					- ( grid[x][y].sigma[(k+1)%3] * grid[x][y].sigma[(k+2)%3] ) /4 * grid[x][y].H[k]  * deltat * deltat
					- ( grid[x][y].sigma[(k+1)%3] * grid[x][y].sigma[(k+2)%3] ) * grid[x][y].intH[k] * deltat
					- cdt / grid[x][y].mu_r[k] * grid[x][y].curlE[k]
					- cdt * grid[x][y].sigma[k] / grid[x][y].mu_r[k] * grid[x][y].intCE[k]
					) / (1 
								+ ( grid[x][y].sigma[(k+1)%3] + grid[x][y].sigma[(k+2)%3] ) /2 * deltat
								+ ( grid[x][y].sigma[(k+1)%3] * grid[x][y].sigma[(k+2)%3] ) /4 * deltat * deltat
					)
					;
					
			
			
		};
		
		#endif
		void algorithmstep(){
			
			for(int i = 1; i < nx-1; i++)
				for(int j = 1; j < ny-1; j++)
					updateE(i,j);
					
			for(int i = 1; i < nx-1; i++)
				for(int j = 1; j < ny-1; j++)
					updateH(i,j);
					
			t = t + 1;
			
		}
		
		void inject(){
			//double nu = pow(10,12); 
			for(int i = 80; i < 120; i++){
				grid[40][i].E[1] = 10* sin( 2*M_PI*t*0.01) ;
				grid[40][i].H[2] = 10* cos( 2*M_PI*t*0.01) ;
				grid[39][i].H[2] = 0 ;
			}
		}
	
};

#endif

