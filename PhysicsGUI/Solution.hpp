#pragma once
#ifndef DEF_SOLUTION
#define DEF_SOLUTION

#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <vector>
#include <cmath>
#include "System.hpp"

#define N 2        // Number of unknowns

class Solution {
public:
	Solution(System const& system, const double& t0 = 0.0, const double& tEnd = 1.0, const double& dt = 0.001);
	~Solution();
	void solveEuler();
	void solveRK4();
	void solveExact();
	void calcEnergy();
	void setName(std::string name);
	void setTimeBoundaries(double t0, double tEnd);
	void setStepTime(double dt);
	void setStepNumber(int n);
	void computeStepTime();
	void computeStepNumber();
	void nextStep();
	void initTime();
	void initPosition();
	void initTimeRT();
	void save(std::string const& filePath) const;
	double getPosition() const;
	double getEm() const;
	std::vector <double> getX();
	std::vector <double> getT();

protected:
	System m_system;

	double m_t0;		//[s]
	double m_tEnd;		//[s]
	double m_dt;		//[s]
	int m_n;			//[-]

	std::vector <double> m_t;
	std::vector <double> m_x;
	std::vector <double> m_v;
	std::vector <double> m_Ec;
	std::vector <double> m_Es;
	std::vector <double> m_Em;
	std::vector<std::vector <double>*> m_Result;
	std::string m_header;
	std::string m_name;
};

double calcNext_dv(const double& k, const double& m, const double& x);

double calcNext_dx(const double& v);

void solveSystem(double y[N], double const& k, double const& m, double const& x, double const& v);


#endif // DEF_SOLUTION
