
#ifndef SOURCE_H
#define SOURCE_H


#include "Fdtd.h"


class Source {
	
	
	public:
		int duration;
		Fdtd * fdtd;
		Fdtd3d* fdtd3d;
		int t;
	
		
		Source(Fdtd * fz, int d){
			duration = d;
			fdtd = fz;
			t = 0;
		};
		
		Source(Fdtd3d * fz, int d){
			duration = d;
			fdtd3d = fz;
			t = 0;
		};
		
	virtual void inject(){
			t = t + 1;
		};
};


class GaussianBeam : public Source {
	
	double lambda;
	Fdtd::point location;
	int width;
	double amplitude;
	
	public:
		GaussianBeam(Fdtd::point p1, int w, double amp, double wavelength, Fdtd * fz) : Source(fz, INT_MAX) {
			location = p1;
			width = w;
			amplitude = amp;
			lambda = wavelength; 
		};
		
		void inject(){
			Source::inject();
			
			for( int i = location.y - width; i < location.y + width; i++ ) {
				fdtd->grid[location.x][i].E[1] = gaussianProfile(i - location.y)*
					amplitude * sin( 2*M_PI*t * fdtd->cdt / lambda ) ;
				fdtd->grid[location.x][i].H[2] = gaussianProfile(i - location.y)*
					amplitude * cos( 2*M_PI*t * fdtd->cdt / lambda ) ;
				fdtd->grid[location.x - 1][i].H[2] = 0 ;
			}
			
		};
		
		double gaussianProfile(int y){
			return 1.0 / width * exp( - pow(3.0*y / width , 2) * 1.0/2 );
		};
	
};


class PlaneWave : public Source {
	
	double lambda;
	Fdtd::point a, b;
	int width;
	double amplitude;
	
	public:
		PlaneWave(Fdtd::point p1, Fdtd::point p2, double amp, double wavelemgth, Fdtd * fz) : Source(fz, INT_MAX) {
			a = p1;
			b = p2;
			amplitude = amp;
			lambda = wavelemgth;
		};
		
		void inject(){
			Source::inject();
			
			int length = (int)sqrt( pow((double) a.x - b.x, 2) + pow((double)a.y - b.y, 2) ) ;
			
			for( int i = 0 ; i < length; i++ ) {
				int x = (int)( a.x + 1.0*(b.x - a.x )/length * i ) ; 
				int y = (int)( a.y + 1.0*(b.y - a.y )/length * i ) ; 
				
				fdtd->grid[x][y].E[1] = 
					amplitude * sin( 2*M_PI*t * fdtd->cdt/lambda ) ;
				fdtd->grid[x][y].H[2] = 
					amplitude * cos( 2*M_PI*t * fdtd->cdt/lambda ) ; 
			}
			
		};	
};


class GaussianBeamMM : public Source {
	
	double lambda;
	Fdtd::point location;
	int width;
	double amplitude;
	int mode;
	
	public:
		GaussianBeamMM(Fdtd::point p1, int w, double amp, double wavelength, int nmode, Fdtd * fz) : Source(fz, INT_MAX) {
			location = p1;
			width = w;
			amplitude = amp;
			lambda = wavelength; 
			mode = nmode;
		};
		
		void inject(){
			Source::inject();
			
			int x = 0;
			for( int i = location.y - width; i < location.y + width; i++ ) {
				fdtd->grid[location.x][i].E[1] += gaussianProfile(i - location.y)*
					amplitude * sin( 2*M_PI*t * fdtd->cdt / lambda ) * sin( mode / 2.0 * M_PI* x / width );
				fdtd->grid[location.x][i].H[2] += gaussianProfile(i - location.y)*
					amplitude * cos( 2*M_PI*t * fdtd->cdt / lambda ) * sin( mode / 2.0 * M_PI* x / width );
				fdtd->grid[location.x - 1][i].H[2] = 0 ;
				x++;
			}
			
		};
		
		double gaussianProfile(int y){
			return 1.0 / width * exp( - pow(3.0*y / width , 2) * 1.0/2 );
		};
	
};



class GaussianBeam3d : public Source {
	
	double lambda;
	Fdtd3d::point location;
	int width;
	double amplitude;
	
	public:
		GaussianBeam3d(Fdtd3d::point p1, int w, double amp, double wavelength, Fdtd3d * fz) : Source(fz, INT_MAX) {
			location = p1;
			width = w;
			amplitude = amp;
			lambda = wavelength; 
		};
		
		void inject(){
			Source::inject();
			int y = 50;
			for( int x = 1; x < fdtd3d->nx-1; x++ ) {
				for( int y = 1; y < fdtd3d->ny-1; y++ ) {
					double rad = sqrt( pow((double)x - location.x, 2) + pow((double)y - location.y, 2) ); 
					fdtd3d->grid[x][y][location.z].E[0] = gaussianProfile(rad)*
						amplitude * sin( 2*M_PI*t * fdtd3d->cdt / lambda ) ;
					fdtd3d->grid[x][y][location.z].H[1] = gaussianProfile(rad)*
						amplitude * cos( 2*M_PI*t * fdtd3d->cdt / lambda ) ;
					fdtd3d->grid[x][y][location.z-1].H[1] = 0 ;
				}
			}
			//cout<< "    ## "<< 2 *t * fdtd3d->cdt / lambda;
		};
		
		double gaussianProfile(double rad){
			return 1.0 / width * exp( - pow(3.0*rad / width , 2) * 1.0/2 );
		};
	
};



#endif

