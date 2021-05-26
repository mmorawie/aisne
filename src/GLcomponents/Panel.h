


#ifndef PANEL_H
#define PANEL_H

#include "Font.h"
#include "GLComponent.h"
#include "../Fdtd.h"

#include <QObject>
#include <QWidget>
#include <QGLWidget>
#include <QtGui>


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
		
		Panel(QWidget * parent, Font * fnt, Fdtd * nfdtd);
		
		void line(int x1, int y1, int x2, int y2);
		void hmark(int x, int nbr);
		void shmark(int x);
		void vmark(int y, int nbr);
		void svmark(int y);		
		void frame();
		
	protected: void paintGL();
	
	void write_to_file(char * filename);
	void save_pattern(char * filename, double treshold);
	void mousePressEvent(QMouseEvent * event);
};


#endif

