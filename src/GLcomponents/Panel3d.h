


#ifndef PANEL3D_H
#define PANEL3D_H

#include <QObject>
#include <QWidget>
#include <QGLWidget>
#include <QtGui>

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
		
		Panel3d(QWidget * parent, Font * fnt, Fdtd3d * nfdtd);
	
	protected: void paintGL();
		void write_to_file(char * filename);
		void write_to_file2(char * filename, int level);
		void save_pattern(char * filename, double treshold);
		void mousePressEvent(QMouseEvent * event);	
};


#endif

