
#include "Fdtd.h"
#include <QtGui>
#include <iostream>

#define PML

using namespace std;


Fdtd::Fdtd(int xsize, int ysize){
	
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
				setIsotropic(i,j, 2.25);			}
		}
	
	}
	
};

void Fdtd::setIsotropic(int x, int y, double epsilon_value){
	grid[x][y].epsilon_r[0] = grid[x][y].epsilon_r[1] = grid[x][y].epsilon_r[2] = epsilon_value;
};

void Fdtd::rectangle(int x1, int x2, int y1, int y2, double epsilon, double mu){
	for(int x = x1; x < x2; x++){
		for(int y = y1; y < y2; y++){
			grid[x][y].epsilon_r[0] = grid[x][y].epsilon_r[1] = grid[x][y].epsilon_r[2] = epsilon;
			grid[x][y].mu_r[0] = grid[x][y].mu_r[1] = grid[x][y].mu_r[2] = mu;
		}
	}
	
};

double Fdtd::part_H_x(int j, int x, int y){
	return (grid[x][y].H[j] - grid[x-1][y].H[j]) / deltax;
}

double Fdtd::part_H_y(int j, int x, int y){
	return (grid[x][y].H[j] - grid[x][y-1].H[j]) / deltay;
}

double Fdtd::part_E_x(int j, int x, int y){
	return (grid[x+1][y].E[j] - grid[x][y].E[j]) / deltax;
}

double Fdtd::part_E_y(int j, int x, int y){
	return (grid[x][y+1].E[j] - grid[x][y].E[j]) / deltay;
}

#ifndef PML

void Fdtd::updateE(int x, int y){
	grid[x][y].E[0] = grid[x][y].E[0] + cdt / grid[x][y].epsilon_r[0] * ( + part_H_y(2,x,y) );
	grid[x][y].E[1] = grid[x][y].E[1] + cdt / grid[x][y].epsilon_r[1] * ( - part_H_x(2,x,y) );
	grid[x][y].E[2] = grid[x][y].E[2] + cdt / grid[x][y].epsilon_r[2] * ( + part_H_x(1,x,y) - part_H_y(0,x,y));
};

void Fdtd::updateH(int x, int y){
	grid[x][y].H[0] = grid[x][y].H[0] - cdt / grid[x][y].mu_r[0] * ( + part_E_y(2,x,y) );
	grid[x][y].H[1] = grid[x][y].H[1] - cdt / grid[x][y].mu_r[1] * ( - part_E_x(2,x,y) );
	grid[x][y].H[2] = grid[x][y].H[2] - cdt / grid[x][y].mu_r[2] * ( + part_E_x(1,x,y) - part_E_y(0,x,y));
};

#else

void Fdtd::updateE(int x, int y){
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
};

void Fdtd::updateH(int x, int y){
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

void Fdtd::algorithmstep(){
	
	for(int i = 1; i < nx-1; i++)
		for(int j = 1; j < ny-1; j++)
			updateE(i,j);
			
	for(int i = 1; i < nx-1; i++)
		for(int j = 1; j < ny-1; j++)
			updateH(i,j);
			
	t = t + 1;
	
};




