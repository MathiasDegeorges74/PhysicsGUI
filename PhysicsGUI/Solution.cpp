#include "Solution.hpp"

//#include <string>
//#include <vector>
//#include "System.hpp"

using namespace std;


#include "Gauss.hpp"

bool compareStrings(std::string& str1, std::string& str2)
{
	if (str1.length() != str2.length())
		return false;

	for (int i = 0; i < str1.length(); ++i) {
		if (tolower(str1[i]) != tolower(str2[i]))
			return false;
	}

	return true;
}

void solveSystem(double y[N], double const& k, double const& m, double const& x, double const& v)
{

	//	  M	  *	 dy/dt	 =	   F
	//	|1 0| *	|dv/dt|	 =	|-xk/m|
	//	|0 1|	|dx/dt|		|  v  |


	/* input matrix */
	double mat[N][N + 1] = { {1.0,0.0, -x * k / m},
							{0.0,1.0, v} };

	gaussianElimination(mat, y);

	//cout << y[0] << " " << y[1] << "\n";

	//cout << "system solved \n";
}

double calcNext_dv(const double& k, const double& m, const double& x)
{
	// dv/dt=f(x,k,m)=-x * k / m
	return -x * k / m;
}

double calcNext_dx(const double& v)
{
	// dx/dt=v
	return v;
}

Solution::Solution(System const& system, const double& t0, const double& tEnd, const double& dt) : m_system(system), m_t0(t0), m_tEnd(tEnd), m_dt(dt)
{

	// Time definition
	//m_tEnd = 1; //[s]
	//m_dt = 0.005; //[s]
	m_n = m_tEnd / m_dt;

	m_solverExact = false;
	m_solverRK4 = false;
	m_solverEuler = false;

	// Time variable initialisation initialisation
	m_t = vector <double>(m_n, 0);
	m_x = vector <double>(m_n, 0);
	m_v = vector <double>(m_n, 0);
	m_Ec = vector <double>(m_n, 0);
	m_Es = vector <double>(m_n, 0);
	m_Em = vector <double>(m_n, 0);

	vector <double>* tPtr = &m_t; m_Result.push_back(tPtr);
	vector <double>* xPtr = &m_x; m_Result.push_back(xPtr);
	vector <double>* dxPtr = &m_v; m_Result.push_back(dxPtr);
	vector <double>* EcPtr = &m_Ec; m_Result.push_back(EcPtr);
	vector <double>* EsPtr = &m_Es; m_Result.push_back(EsPtr);
	vector <double>* EmPtr = &m_Em; m_Result.push_back(EmPtr);

	m_header = "t	x	v	Ec	Es	Em";

	//cout << "Solution initialisation done" << endl;

	//for (int i = 0; i < m_n - 1;i++)
	//{
	//	m_t[i + 1] = m_t[i] + m_dt;
	//}

}

Solution::~Solution() {}

void Solution::forward(double tEnd)
{
	setTimeTarget(tEnd);
	updateInitialConditions();
	initTimeRT();

	if (m_solverExact)
	{
		solveExact();
	}
	else if (m_solverRK4)
	{
		solveRK4();
		//std::cout << "Solving RK4 \n";
	}
	else if (m_solverEuler)
	{
		solveEuler();
	}
	else
	{
		std::cout << "no solver chosen for " << m_name << "\n";
	}
	calcEnergy();
}

void Solution::setName(std::string name)
{
	m_name = name;
}

void Solution::setSolver(std::string solver)
{
	std::string flagExact = "EXACT";
	std::string flagRK4 = "RK4";
	std::string flagEuler = "EULER";

	m_solverExact = false;
	m_solverRK4 = false;
	m_solverEuler = false;

	if (compareStrings(solver, flagExact))
	{
		m_solverExact = true;
	}
	else if (compareStrings(solver, flagRK4))
	{
		m_solverRK4 = true;
	}
	else if (compareStrings(solver, flagEuler))
	{
		m_solverEuler = true;
	}
}

void Solution::setTimeBoundaries(double t0, double tEnd)
{
	m_t0 = t0;
	m_tEnd = tEnd;
}

void Solution::setTimeTarget(double tEnd)
{
	m_tEnd = tEnd;
}

void Solution::setStepTime(double dt)
{//Set step time
	m_dt = dt;
}

void Solution::setStepNumber(int n)
//Set step number and compute step time
{
	m_n = n;
}

void Solution::computeStepTime()
{//compute step time from t0, tEnd and step number
	// for n point -> n-1 steps
	m_dt = (m_tEnd - m_t0) / double(m_n - 1);
	//std::cout << m_name << " : dt = " << m_dt << "s\n";
}

void Solution::computeStepNumber()
{//compute step number from t0, tEnd and step time
	m_n = m_tEnd / m_dt;
}

void Solution::initPosition()
{
	m_x[0] = m_system.getInitialPosition();
	m_x[m_n - 1] = m_x[0];

	m_v[0] = 0;
	m_v[m_n - 1] = m_v[0];
}

void Solution::nextStep()
{
	m_x[0] = m_x[m_n - 1];
	m_v[0] = m_v[m_n - 1];
	m_t[0] = m_x[m_n - 1];
	m_t0 = m_t[0];
	//std::cout << m_name << " : x0 = " << m_x[0] << "m\n";
}

void Solution::updateInitialConditions()
{
	m_x[0] = m_x[m_n - 1];
	m_v[0] = m_v[m_n - 1];

	m_t[0] = m_t[m_n - 1];
	m_t0 = m_t[0];
	//std::cout << m_name << " : x0 = " << m_x[0] << "m\n";
}


void Solution::initTime()
{
	computeStepNumber();
	m_t[0] = m_t0;
	for (int i = 0; i < m_n - 1;i++)
	{
		m_t[i + 1] = m_t[i] + m_dt;
	}
}

void Solution::initTimeRT()
{
	computeStepTime();
	m_t[0] = m_t0;
	for (int i = 0; i < m_n - 1;i++)
	{
		m_t[i + 1] = m_t[i] + m_dt;
	}
}

void Solution::solveEuler()
{
	double y[N];
	double m = m_system.getMasse();
	double k = m_system.getSpringStifness();

	for (int i = 0; i < m_n - 1;i++)
	{
		solveSystem(y, k, m, m_x[i], m_v[i]);

		m_v[i + 1] = m_v[i] + y[0] * m_dt;
		m_x[i + 1] = m_x[i] + y[1] * m_dt; //+ 0.5 * ddx * dt * dt;
	}

}

void Solution::solveRK4()
{
	double y1[N], y2[N], y3[N], y4[N];
	double m = m_system.getMasse();
	double k = m_system.getSpringStifness();

	for (int i = 0; i < m_n - 1;i++)
	{
		solveSystem(y1, k, m, m_x[i], m_v[i]);

		solveSystem(y2, k, m, m_x[i] + y1[1] * m_dt / 2., m_v[i] + y1[0] * m_dt / 2.);
		solveSystem(y3, k, m, m_x[i] + y2[1] * m_dt / 2., m_v[i] + y2[0] * m_dt / 2.);
		solveSystem(y4, k, m, m_x[i] + y3[1] * m_dt, m_v[i] + y3[0] * m_dt);

		m_x[i + 1] = m_x[i] + (m_dt / 6.) * (y1[1] + 2 * y2[1] + 2 * y3[1] + y4[1]);
		m_v[i + 1] = m_v[i] + (m_dt / 6.) * (y1[0] + 2 * y2[0] + 2 * y3[0] + y4[0]);
	}
}

void Solution::solveExact() {
	double m = m_system.getMasse();
	double k = m_system.getSpringStifness();
	double x0 = m_system.getInitialPosition();

	for (int i = 0; i < m_n;i++)
	{
		m_x[i] = x0 * cos(sqrt(k / m) * m_t[i]);
		m_v[i] = -sqrt(k / m) * x0 * sin(sqrt(k / m) * m_t[i]);

	}
}

void Solution::calcEnergy()
{

	double m = m_system.getMasse();
	double k = m_system.getSpringStifness();

	for (int i = 0; i < m_t.size();i++)
	{
		m_Ec[i] = 0.5 * m * m_v[i] * m_v[i];
		m_Es[i] = 0.5 * k * m_x[i] * m_x[i];
		m_Em[i] = m_Ec[i] + m_Es[i];
	}
}

void Solution::save(string const& filePath) const
{
	//const string filePath = "../Results/resultRK4.txt";

	ofstream myFlux(filePath.c_str());

	if (myFlux)
	{
		string line = "";
		string cell = "";
		myFlux << m_header << endl;

		cout << "Size :" << m_Result.size() << endl;
		cout << "Size line :" << (*m_Result[0]).size() << endl;

		for (int i = 0;i < (*m_Result[0]).size();i++)
		{
			line = "";
			for (int j = 0;j < m_Result.size();j++) {
				cell = std::format("{0:.5f}", (*m_Result[j])[i]);
				line = line + cell + "	";
				cell = "";
			}
			myFlux << line << endl;
		}
		cout << "result saved to : " << filePath << endl;
	}
	else
	{
		cout << "Unable to open file!" << endl;
	}

}

double Solution::getPosition()const
{
	return m_x[m_n - 1];
}

double Solution::getEm() const
{
	return m_Em[m_n - 1];
}

double Solution::getFrequency() const
{
	return 1.0 / m_dt;
}

double Solution::getTime() const
{
	return m_t[m_n - 1];
}

std::string Solution::getDetails() const
{
	std::string solutionDetails;
	solutionDetails = std::format("Solution energy [J]	: {0:.3f}	", m_Em[m_n - 1]);
	solutionDetails += format("Solution time [s]	: {0:.3f}	", m_t[m_n - 1]);
	solutionDetails += format("Solution frequency [/s]	: {0:.3f}	", 1 / m_dt);

	return solutionDetails;
}

vector <double> Solution::getX()
{
	return m_x;
}

vector <double> Solution::getT()
{
	return m_t;
}
