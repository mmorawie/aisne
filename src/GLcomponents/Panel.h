


#ifndef PANEL_H
#define PANEL_H

#include <stdio.h>
#include <math.h>
#include <fstream>

#include "Font.h"
#include "GLComponent.h"
#include "../Fdtd.h"


class Panel : public GLComponent {
	
	char text[250];
	int cellSize;
	
	int px, py; bool print;
	
	double time;
	
	int margin, topmargin;
	int w, h;
	int markLength, smallMarkLength;
	
	public: 
		QGridLayout *layout;
		Fdtd * fdtd;
		
		Panel() : GLComponent(NULL, NULL){
						
			setFixedSize(782, 416);
			
			
		};
		
		Panel(QWidget * parent, Font * fnt, Fdtd * nfdtd) : GLComponent(parent, fnt){
			
			layout = new QGridLayout(this);
			margin = 25; 
			topmargin = 50;
			w = 782;
			h = 416;
			markLength = 12;
			smallMarkLength = 8;
			
			setFixedSize(w + 2*margin, h + margin + topmargin);
			
			setMouseTracking(true);
			this->installEventFilter(this);
			
			
			fdtd = nfdtd; 
			
			cellSize = 2;
			
			px = -5; py = -5; 
			print = false;
			
			save_pattern("pattern.dat", 1.9);
			
		};
		
		void line(int x1, int y1, int x2, int y2){
			glBegin(GL_LINE_STRIP);
				glVertex2f(margin + x1, topmargin + y1);
				glVertex2f(margin + x2, topmargin + y2);
			glEnd();
		};
		
		void hmark(int x, int nbr){
			line(x, h, x, h - markLength);
			line(x, 0, x, markLength);
			font->print(margin + x, topmargin + h + 16, nbr);
			glColor4f(0,0,0,1);
		};
		
		void shmark(int x){
			line(x, h, x, h - smallMarkLength);
			line(x, 0, x, smallMarkLength);
		};
		
		void vmark(int y, int nbr){
			line(0, y, markLength, y);
			line(w, y, w - markLength, y);
			font->print(margin - 8, topmargin + y + 4, nbr);
			glColor4f(0,0,0,1);
		};
		
		void svmark(int y){
			line(0, y, smallMarkLength, y);
			line(w, y, w - smallMarkLength, y);
		};
		
		void frame(){
			glLineWidth(1);
			glColor4f(0,0,0,1);
					
			
			line(0,0,w,0);
			line(0,0,0,h);
			line(0,h,w,h);
			line(w,0,w,h);
			
			glLineWidth(1);
			for(int i = 0; i < 6; i++){
				hmark(80 + i*100, i*5);
				for(int j = 1; j < 5; j++) shmark(80 + i*100 + j * 20);
			}
			hmark(80 + 6*100, 6*5);
			
			vmark(h/2, 0);
			for(int i = 1; i < 2; i++){
				vmark(h/2 + i*100,  i*5);
				vmark(h/2 - i*100,  i*5);
			}
			
			font->print( margin + w, topmargin + h+16, font->replace("um", 0, 181) );
			time = fdtd->t * fdtd->deltat * pow(10.0,8);
			sprintf(text, "t = %.2f fs", time);
			font->print( margin + w - 120, topmargin - 10, text );
		};
		
	protected: void paintGL(){
			glClearColor(0.95, 0.95, 0.95, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			for(int i = 0; i < fdtd->nx; i++) {
				for(int j = 0; j < fdtd->ny; j++){
					double b = pow( (fdtd->grid[i][j].E[1]), 2) ;
					double c = pow( (fdtd->grid[i][j].H[2]), 2) ;
					
					glColor3f( b/2, c/2, 0);
					glRecti(margin + cellSize*i, topmargin + cellSize*j, 
							margin + cellSize*(i+1), topmargin + cellSize*(j+1));
				}
			}
			
			frame();
			
			#ifndef TEST
				
			if( fdtd->t%10 == 0 ) {
				//sprintf(text, "../gif/p%03d.jpg", fdtd->t/10 );
				sprintf(text, "./gif/p%d.dat", fdtd->t/10 );
				write_to_file(text);
			};
			
			#endif
				
		};
		
	void write_to_file(char * filename){
			ofstream file (filename);
			double * E;
			double * H; double poynting[3];
			
			
			for(int j = 0; j < 200; j++){
				for(int i = 0; i < 390; i++) {
					E = fdtd->grid[i][j].E;
					H = fdtd->grid[i][j].H;
					double length = ( sqrt( pow(E[0], 2) + pow(E[1],2) + pow(E[2], 2) ) );
					file<<length<<endl;
				}
				file<<endl;
			}
			
			file.close();
		};
	
	void save_pattern(char * filename, double treshold){
		ofstream file (filename);
			double epsilon;
			
			for(int j = 0; j < fdtd->ny; j++){
				for(int i = 0; i < fdtd->nx; i++) {
					if (fdtd->grid[i][j].epsilon_r[0] > treshold) epsilon = 1000;
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
					E = fdtd->grid[i][j].E;
					H = fdtd->grid[i][j].H;
					
					poynting[0] = (E[1]*H[2] - E[2]*H[1]);
					poynting[1] = (E[2]*H[0] - E[0]*H[2]);
					poynting[2] = (E[0]*H[1] - E[1]*H[0]);
					double length = sqrt( pow(poynting[0], 2) + pow(poynting[1],2) );
					data<<i<<" "<<j<<" "<<poynting[0]<<" "<<poynting[1]<<endl;
				}
			}
			for(int j = 0; j < fdtd->ny; j++){
				for(int i = 0; i < fdtd->nx; i++) {
					E = fdtd->grid[i][j].E;
					H = fdtd->grid[i][j].H;
					double length = ( sqrt( pow(E[0], 2) + pow(E[1],2) + pow(E[2], 2) ) );
					file<<length<<endl;
					
				}
				file<<endl;
			}
			
			
			data.close();
			file.close();
		};
		
	void leaveEvent(QEvent * event){
			print = false;
		};
};


#endif

