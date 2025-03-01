// masseSpring.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream> // console cin cout
#include <string> // string handling
#include <format> // num to string conversions
#include <cmath> // math function
#include "source.hpp" // secondary file
#include <vector> // For tables
#include <fstream> // for file read/write
#include <chrono>
#include "Solution.hpp"
#include "System.hpp"

//#include "matplotlibcpp.h"  //uncomment to plot
//namespace plt = matplotlibcpp; //uncomment to plot

using namespace std;

void stopTimer(chrono::high_resolution_clock::time_point& startTime, const int& n);

int main()
{
	//chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
	//startTime = chrono::high_resolution_clock::now();
	//solveSystem();
	//stopTimer(startTime, 1);
	//return 0;

	// System specs
	double m = 0.1;	// [kg]
	double k = 100;	// [N.m^-1]

	// Initial condition
	double x0 = 0.1;		// [m]

	System system(m, k, x0);

	// Time definition
	double tEnd = 1; //[s]
	double dt = 0.001; //[s]
	int n = tEnd / dt;

	// Programme frenqucy calculation

	chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();

	// Time variable initialisation initialisation

	const string resultPathRK4 = "../Results/resultRK4.txt";

	Solution solutionRk4(system, tEnd, dt);
	startTime = chrono::high_resolution_clock::now();
	solutionRk4.solveRK4();
	stopTimer(startTime, n);
	solutionRk4.calcEnergy();
	solutionRk4.save(resultPathRK4);

	const string resultPathEuler = "../Results/resultEuler.txt";
	Solution solutionEuler(system, tEnd, dt);
	startTime = chrono::high_resolution_clock::now();
	solutionEuler.solveEuler();
	stopTimer(startTime, n);
	solutionEuler.calcEnergy();
	solutionEuler.save(resultPathEuler);

	const string resultPathExact = "../Results/resultExact.txt";
	Solution solutionExact(system, tEnd, dt);
	startTime = chrono::high_resolution_clock::now();
	solutionExact.solveExact();
	stopTimer(startTime, n);
	solutionExact.calcEnergy();
	solutionExact.save(resultPathExact);

	bool plotActive = 1;

	/*if (plotActive)
	{
		plt::named_plot("Euler", solutionEuler.getT(), solutionEuler.getX(), "r");
		plt::named_plot("RK4", solutionRk4.getT(), solutionRk4.getX(), "g");
		plt::named_plot("Analytic", solutionExact.getT(), solutionExact.getX(), "k--");

		plt::grid(1);
		plt::legend();
		plt::show();
	}
	return 0;*/

}

void stopTimer(chrono::high_resolution_clock::time_point& startTime, const int& n)
{
	chrono::high_resolution_clock::time_point stopTime = chrono::high_resolution_clock::now();
	double timeNanoLoop = chrono::duration_cast<chrono::nanoseconds>(stopTime - startTime).count();
	double timeNanoStep = timeNanoLoop / n;
	double F = pow(10.0, 9) / timeNanoStep;

	// For debug : base display
	//cout << "Loop time : " << timeNanoLoop * pow(10.0, -9) << "s" << endl;
	//cout << "Loop time : " << timeNanoStep * pow(10.0, -9) << "s" << endl;
	//cout << "Frequency : " << F << " Hz" << endl;

	cout << endl << "# # # TIMER # # #" << endl;
	if (F < pow(10.0, 3))
	{
		cout << "Loop time : " << timeNanoLoop * pow(10.0, -6) << " ms" << endl;
		cout << "Step nb   : " << n << endl;
		cout << "Step time : " << timeNanoStep * pow(10.0, -6) << " ms" << endl;
		cout << "Step Freq : " << F << " Hz" << endl;
	}
	else if (F < pow(10.0, 6))
	{

		cout << "Loop time : " << timeNanoLoop * pow(10.0, -3) << " micro s" << endl;
		cout << "Step nb   : " << n << endl;
		cout << "Step time : " << timeNanoStep * pow(10.0, -3) << " micro s" << endl;
		cout << "Frequency : " << F / pow(10.0, 3) << " kHz" << endl;
	}
	else if (F < pow(10.0, 9))
	{

		cout << "Loop time : " << timeNanoLoop << " ns" << endl;
		cout << "Step nb   : " << n << endl;
		cout << "Step time : " << timeNanoStep << " ns" << endl;
		cout << "Frequency : " << F / pow(10.0, 6) << " MHz" << endl;
	}
	else if (F < pow(10.0, 12))
	{

		cout << "Loop time : " << timeNanoLoop * pow(10.0, 3) << " ps" << endl;
		cout << "Step nb   : " << n << endl;
		cout << "Step time : " << timeNanoStep * pow(10.0, 3) << " ps" << endl;
		cout << "Frequency : " << F / pow(10.0, 9) << " GHz" << endl;
	}
	cout << "# # # # # # # # # #" << endl << endl;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
