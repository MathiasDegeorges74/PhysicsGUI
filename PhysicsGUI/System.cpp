#include "System.hpp"

using namespace std;

System::System(double m, double k, double x0) : m_m(m), m_k(k), m_x0(x0) {}

System::~System() {}

double System::getMasse() const
{
	return m_m;
}

double System::getSpringStifness() const
{
	return m_k;
}

double System::getInitialPosition() const
{
	return m_x0;
}
