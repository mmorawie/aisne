

#ifndef FONT_H
#define FONT_H


#include <ft2build.h>
#include FT_FREETYPE_H

#include <QObject>
#include <QWidget>
#include <QGLWidget>


class Font : public QGLWidget{
	
	static const int number_of_characters = 255;//128;
	
	struct character 
		{
		int width; 
		int height;
		int x_pos;
		int y_pos;
		float advance;
		unsigned char *bitmap;
		unsigned int texture;
		};
	
	
	
	FT_Face face;
	FT_Library ft;
			
	long main_color;
	bool initialized;
	
	public: 
		struct color
		{
		int r, g, b;
		int a;
		};
		
		int main_size;
		character characters[number_of_characters];		
		
		static unsigned long createRGBA(int r, int g, int b, int a);
		
		Font(const char * filename, int size, long color, QWidget * parent) ;
			
		void load_font_from_file(const char * filename);
		
		void init();
		
		void load_characters(character table[], long color);
		void generate_set(character table[], long color);
		
		protected: void initializeGL(){
			init();
		};
		
		
		public: int putchar(int x, int y, char c);
			
		float print(int x, int y, int number, char filter = 255);	
		float print(int x, int y, const char * cc, char filter = 255);
		inline float printchar(float x, float y, unsigned char c, char filter = 255);
		
		inline float printchar(float x, float y, unsigned char c, character set[], char filter=255);
		float printchar2(float x, float y, char c);
		int unsgnd_strlen(const unsigned char *text);
		static char * replace(const char * text, int i, char c);		
		
};



#endif
