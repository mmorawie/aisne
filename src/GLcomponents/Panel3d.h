


#ifndef PANEL3D_H
#define PANEL3D_H

#include <stdio.h>
#include <math.h>
#include <fstream>

#include "Font.h"
#include "GLComponent.h"
#include "../Fdtd3d.h"


class Panel3d : public GLComponent {
	
	char text[250];
	int cellSize;
	
	double time;
	
	int margin, topmargin;
	int w, h;
	int nx, ny;
	
	int z0;
	
	public: 
		QGridLayout *layout;
		Fdtd3d * fdtd;
		
		Panel3d() : GLComponent(NULL, NULL){
			
		};
		
		Panel3d(QWidget * parent, Font * fnt, Fdtd3d * nfdtd) : GLComponent(parent, fnt){
			
			fdtd = nfdtd; 
			z0 = 45;
			
			cellSize = 2;
			
			nx = fdtd->nx;
			ny = fdtd->nz;
			
			layout = new QGridLayout(this);
			margin = 25; 
			topmargin = 25;
			w = nx*cellSize;
			h = ny*cellSize;
			
			setMouseTracking(true);
			this->installEventFilter(this);
			
			setFixedSize(w + 2*margin, h + margin + topmargin);
				
			save_pattern("pattern.dat", 1.9);
			save_pattern("pattern2.dat", 1.0);
			
		};
		
	protected: void paintGL(){
			glClearColor(0.95, 0.95, 0.95, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			for(int i = 0; i < nx; i++) {
				for(int j = 0; j < ny; j++){
			//		double b = pow( (fdtd->grid[i][j][z0].E[0]), 2) ;
			//		double c = pow( (fdtd->grid[i][j][z0].H[1]), 2) ;
				
					double b = pow( (fdtd->grid[i][50][j].E[0]), 2) ;
					double c = pow( (fdtd->grid[i][50][j].H[1]), 2) ;
				
					glColor3f( b/2, c/2, 0);
					glRecti(margin + cellSize*i, topmargin + cellSize*j, 
							margin + cellSize*(i+1), topmargin + cellSize*(j+1));
				}
			}
			
			#ifndef TEST
				
			if( fdtd->t%10 == 0 ) {
				sprintf(text, "p%d.dat", fdtd->t/10 );
				write_to_file(text);
				sprintf(text, "q%d.dat", fdtd->t/10 );
				write_to_file2(text);
			};
			
			#endif
				
			time = fdtd->t * 0.1;
			sprintf(text, "t = %.2f fs", time);
			font->print( margin, topmargin - 2, text );
			
		};
		
	void write_to_file(char * filename){ // xz cross section
			ofstream file (filename);
			double * E;
			double * H; 
			
			for(int i = 0; i < fdtd->nx; i++) {
				for(int j = 0; j < fdtd->nz; j++){
					E = fdtd->grid[i][50][j].E;
					H = fdtd->grid[i][50][j].H;
					double length = ( sqrt( pow(E[0], 2) + pow(E[1],2) + pow(E[2], 2) ) );
					file<<length<<endl;
				}
				file<<endl;
			}
			cout<< "  saved to: "<<filename<<endl;
			file.close();
		};
	
	void write_to_file2(char * filename){ // xy cross section
			ofstream file (filename);
			double * E;
			double * H; 
			
			
			for(int j = 0; j < fdtd->ny; j++){
				for(int i = 0; i < fdtd->nx; i++) {
					E = fdtd->grid[i][j][60].E;
					H = fdtd->grid[i][j][60].H;
					double length = ( sqrt( pow(E[0], 2) + pow(E[1],2) + pow(E[2], 2) ) );
					file<<length<<endl;
				}
				file<<endl;
			}
			cout<< "  saved to: "<<filename<<endl;
			file.close();
		};
	
	
	void save_pattern(char * filename, double treshold){
		ofstream file (filename);
			double epsilon;
			
			for(int i = 0; i < fdtd->nx; i++) {
				for(int j = 0; j < fdtd->nz; j++){
					if (fdtd->grid[i][50][j].epsilon_r[0] > treshold) epsilon = 1000;
					else epsilon = 0;
					file<<epsilon<<endl;
				}
				file<<endl;
			}
			
			file.close();
	};
	
	
	
	void mousePressEvent(QMouseEvent * event){
			cout<<"saving to file"<<endl;
			ofstream data ("dat.f4");
			ofstream file ("int.f4");
			double poynting[3];
			double * E;
			double * H;
			int step = 5;
			
			for(int i = 0; i < fdtd->nx; i = i + step) {
				for(int j = 0; j < fdtd->ny; j = j + step){
					E = fdtd->grid[i][j][z0].E;
					H = fdtd->grid[i][j][z0].H;
					
					poynting[0] = (E[1]*H[2] - E[2]*H[1]);
					poynting[1] = (E[2]*H[0] - E[0]*H[2]);
					poynting[2] = (E[0]*H[1] - E[1]*H[0]);
					double length = sqrt( pow(poynting[0], 2) + pow(poynting[1],2) );
					data<<i<<" "<<j<<" "<<poynting[0]<<" "<<poynting[1]<<endl;
				}
			}
			for(int j = 0; j < fdtd->ny; j++){
				for(int i = 0; i < fdtd->nx; i++) {
					E = fdtd->grid[i][j][z0].E;
					H = fdtd->grid[i][j][z0].H;
					double length = ( sqrt( pow(E[0], 2) + pow(E[1],2) + pow(E[2], 2) ) );
					file<<length<<endl;
					
				}
				file<<endl;
			}
			
			
			data.close();
			file.close();
		};
		
};


#endif

