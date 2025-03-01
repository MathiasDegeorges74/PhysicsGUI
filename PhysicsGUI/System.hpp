#pragma once
#pragma once
#ifndef DEF_SYSTEM
#define DEF_SYSTEM


class System {
public:
	System(double m= 0.1, double k=100, double x0=0.1);
	~System();
	double getMasse() const;
	double getSpringStifness() const;
	double getInitialPosition() const;

private:
	double m_m;		// [kg]
	double m_k;		// [N.m^-1]
	double m_x0;	// [m]
};

#endif // !DEF_SYSTEM