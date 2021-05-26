
#include "Font.h"


#include <QtOpenGL> 

#include <GL/gl.h>
#include <GL/glu.h>


#include <iostream>

using namespace std;

unsigned long Font::createRGBA(int r, int g, int b, int a){   
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
};


Font::Font(const char * filename, int size, long color, QWidget * parent) 
		: QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
	
	resize(0,0);
	load_font_from_file(filename);
	
	
	main_color = color;
	main_size = size;
	
	FT_Set_Pixel_Sizes(face, 0, main_size);  
	load_characters(characters, main_color);
		
	FT_Done_Face(face);
	FT_Done_FreeType(ft); 
	
	initialized = false;
};
	
void Font::load_font_from_file(const char * filename){
	FT_Init_FreeType(&ft);

	FT_New_Face(ft, filename, 0, &face);
}

void Font::init(){
	if(initialized) return;
	
	
	glEnable(GL_TEXTURE_2D);
	
	generate_set(characters, main_color);
	
	glDisable(GL_TEXTURE_2D);
	
	initialized = true;
	
};

void Font::load_characters(character table[], long color){
	FT_UInt glyph_index;
	unsigned char r, g, b, a;

	r = color >> 24;
	g = color >> 16;
	b = color >> 8;
	a = color >> 0;
	
	
	for (int i = 0; i < number_of_characters; i++) 
		{
		
		glyph_index = FT_Get_Char_Index( face, i );

		if( FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT ) ) cout<<"error"<<endl;
		
		FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
		table[i].x_pos = face->glyph->bitmap_left;
		table[i].y_pos = face->glyph->bitmap_top - face->glyph->bitmap.rows;;
		table[i].width = face->glyph->bitmap.width;
		table[i].height = face->glyph->bitmap.rows;
		table[i].advance = face->glyph->advance.x * 1/64.0;
		
		int length = table[i].width * table[i].height *4;
		table[i].bitmap = new unsigned char[length];
				
		for (int j = 0; j < table[i].height; j++) 
			{
			for (int k = 0; k < table[i].width; k++)
				{
				int value = face->glyph->bitmap.buffer[(table[i].height - j - 1) * table[i].width  + k];
				value = (int)(value * a/255.0);
				table[i].bitmap[4 * j * table[i].width  + 4 * k + 0] = r ;//- value/255.0 * 127;
				table[i].bitmap[4 * j * table[i].width  + 4 * k + 1] = g ;//- value/255.0 * 127;
				table[i].bitmap[4 * j * table[i].width  + 4 * k + 2] = b ;//- value/255.0 * 127;
				table[i].bitmap[4 * j * table[i].width  + 4 * k + 3] = value;
				
				}
			}
		}
};

void Font::generate_set(character table[], long color){

	for(int i = 0; i < number_of_characters; i++) 
		{
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
		glEnable(GL_TEXTURE_2D);
		
		glGenTextures(1, &(table[i].texture));
		glBindTexture(GL_TEXTURE_2D, table[i].texture);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, table[i].width);
		
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			 GL_RGBA8,
			table[i].width,
			table[i].height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			table[i].bitmap
		); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		}
		
};

int Font::putchar(int x, int y, char c){
	
	glDisable(GL_TEXTURE_2D);

	glColor4f(1.0,0.0,0.0, 1.0);
	glRecti(x, y, x+100, y-100);
	
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_TEXTURE_2D);
	
	
	glBindTexture(GL_TEXTURE_2D, characters[c].texture);
	
	glColor4f(1.0,1.0,1.0, 1.0);
	
	glBegin(GL_QUADS);
	  glTexCoord2f(0,0);	glVertex3f(x,y,0); 							// Bottom Left
	  glTexCoord2f(1,0);	glVertex3f(x+characters[c].width,y,0); 		// Bottom Right
	  glTexCoord2f(1,1);	glVertex3f(x+characters[c].width,y-characters[c].height,0);// Top Right
	  glTexCoord2f(0,1);	glVertex3f(x,y-characters[c].height,0); // Top Left
	glEnd();
	
	
	};

	
float Font::print(int x, int y, int number, char filter){
	glEnable(GL_TEXTURE_2D);
	float x1 = x - characters[48].advance;
	if ( number == 0 ) printchar(x1, y, 48, filter);
	while(number > 0)
		{
			printchar(x1, y, (number%10)+48, filter);
			x1 = x1 - characters[(number%10)+48].advance;
			number = number / 10;
		}
	glDisable(GL_TEXTURE_2D);
	return x1;
};
	
float Font::print(int x, int y, const char * cc, char filter){
	glEnable(GL_TEXTURE_2D);
	float x1 = x;
	for(int i = 0; cc[i] != '\0'; i++)
		{
			x1 = printchar(x1, y, cc[i], filter);
		}
	glDisable(GL_TEXTURE_2D);
	return x1;
};

inline float Font::printchar(float x, float y, unsigned char c, char filter){
	return printchar(x,y,c,characters, filter);
};

inline float Font::printchar(float x, float y, unsigned char c, character set[], char filter){
	x = x;
	y = y;
	glBindTexture(GL_TEXTURE_2D, set[c].texture);
	glColor4ub(255,255,255, filter);
	glBegin(GL_QUADS);
	  glTexCoord2f(0,0);	
		glVertex3f(x + set[c].x_pos, y - set[c].y_pos,  0); // Bottom Left
	  glTexCoord2f(1,0);	
		glVertex3f(x + set[c].x_pos + set[c].width, y - set[c].y_pos,0); 	// Bottom Right
	  glTexCoord2f(1,1);	
		glVertex3f(x + set[c].x_pos + set[c].width, y - set[c].y_pos - set[c].height,0);
		 // Top Right
	  glTexCoord2f(0,1);	
		glVertex3f(x + set[c].x_pos, y - set[c].y_pos - set[c].height,0); // Top Left
	glEnd();
	return x + set[c].advance;
};
	
float Font::printchar2(float x, float y, char c){
	printchar( x - characters[c].width, y, c); 
	return x - characters[c].width;
};

int Font::unsgnd_strlen(const unsigned char *text) {
	int len = 0;
	while (text[len] != '\0') len++;
	return len;
}

char * Font::replace(const char * text, int i, char c){
	int len = strlen(text);
	char * retrurn_text = new char[len+1];
	for(int j = 0; j < len+1; j++) retrurn_text[j] = text[j];
	retrurn_text[i] = c;
	return retrurn_text;
};













