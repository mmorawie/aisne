

#ifndef GLCOMPONENT_H
#define GLCOMPONENT_H

#include "Font.h"


class GLComponent : public QGLWidget{
	
	public: 
		Font * font;
		
		GLComponent(QWidget * parent, Font * fnt) : 
			QGLWidget(QGLFormat(QGL::SampleBuffers), parent, fnt)
			{
			font = fnt;
		};
		
		void refresh(){
			update();
		};
		
	protected: void initializeGL(){
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glEnable(GL_LINE_SMOOTH);
			glEnable( GL_POLYGON_SMOOTH );
			glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			
			glLoadIdentity();
			glOrtho( 0.f, width(), height(), 0.f, 0.f, 1.f );
			
		};
	
};

#endif


