
#ifndef SOURCE_H
#define SOURCE_H


#include "Fdtd.h"
#include "Fdtd3d.h"


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
		GaussianBeam::GaussianBeam(Fdtd::point p1, int w, double amp, double wavelength, Fdtd * fz);
		void inject();
	private:
		double gaussianProfile(int y);
};


class PlaneWave : public Source {
	
	double lambda;
	Fdtd::point a, b;
	int width;
	double amplitude;
	
	public:
		PlaneWave(Fdtd::point p1, Fdtd::point p2, double amp, double wavelemgth, Fdtd * fz);		
		void inject();
};


class GaussianBeamMM : public Source {
	
	double lambda;
	Fdtd::point location;
	int width;
	double amplitude;
	int mode;
	
	public:
		GaussianBeamMM(Fdtd::point p1, int w, double amp, double wavelength, int nmode, Fdtd * fz);
		void inject();
	private:
		double gaussianProfile(int y);
};



class GaussianBeam3d : public Source {
	
	double lambda;
	Fdtd3d::point location;
	int width;
	double amplitude;
	
	public:
		GaussianBeam3d(Fdtd3d::point p1, int w, double amp, double wavelength, Fdtd3d * fz);
		void inject();
	private:
		double gaussianProfile(double rad);
};



#endif

