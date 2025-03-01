#include <SFML/Graphics.hpp>
#include <format>
#include <cmath>
#include <numbers>
#include <iostream>
#include "Solution.hpp"
#include "System.hpp"


int main()
{


	sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!");

	//window.setFramerateLimit(62);

	// Create simulation system

	// Systeme parameters
	double m = 1;	// [kg]
	double k = 20;	// [N.m^-1]

	// Initial condition
	double x0 = 1;		// [m]

	System system(m, k, x0);

	Solution solutionExact(system);
	Solution solutionEuler(system);
	Solution solutionRK4(system);

	solutionExact.setStepNumber(4);
	solutionExact.setName("Exact");
	solutionExact.initPosition();

	solutionEuler.setStepNumber(200);
	solutionEuler.setName("Euler");
	solutionEuler.initPosition();

	solutionRK4.setStepNumber(200);
	solutionRK4.setName("RK4");
	solutionRK4.initPosition();

	// Created simulation body objects

	sf::CircleShape circleExact(20.f);
	sf::CircleShape circleRK4(20.f);
	sf::CircleShape circleEuler(20.f);

	circleExact.setFillColor(sf::Color(255, 255, 255, 255));
	circleRK4.setFillColor(sf::Color(0, 255, 0, 255));
	circleEuler.setFillColor(sf::Color(255, 255, 0, 255));


	sf::Font font("arial.ttf"); // Throws sf::Exception if an error occurs

	// HUD text
	sf::Text textFPS(font);
	sf::Text textOF(font);
	sf::Text textOFMax(font);

	textFPS.setString("FPS");
	textOF.setString("OF");
	textOFMax.setString("OFMax");

	textFPS.setPosition(sf::Vector2f(100, 100));
	textOF.setPosition(sf::Vector2f(100, 200));
	textOFMax.setPosition(sf::Vector2f(100, 300));

	// Body legends
	sf::Text textExact(font);
	sf::Text textRK4(font);
	sf::Text textEuler(font);

	textExact.setString("Exact");
	textRK4.setString("RK4");
	textEuler.setString("Euler");

	textExact.setPosition(sf::Vector2f(540, 200.0));
	textRK4.setPosition(sf::Vector2f(540, 300.0));
	textEuler.setPosition(sf::Vector2f(540, 400.0));

	textExact.setFillColor(sf::Color(255, 255, 255, 255));
	textRK4.setFillColor(sf::Color(0, 255, 0, 255));
	textEuler.setFillColor(sf::Color(255, 255, 0, 255));

	double yExact(0.0), yEuler(0.0), yRK4(0.0);
	double emExact(0.0), emEuler(0.0), emRK4(0.0);
	double fExact(0.0), fEuler(0.0), fRK4(0.0);

	// Timing features 
	sf::Clock clockGeneral; // General time clock
	sf::Time elapsedTime = clockGeneral.getElapsedTime();

	sf::Clock clockFrame; // Frame time clock
	sf::Time timeSinceLastFrame = clockFrame.restart();
	sf::Time frameElapsedTime = clockFrame.getElapsedTime();

	double fpsTarget = 60.0;
	double frameTime = 1.0 / fpsTarget;
	double fps = 0;

	double frameOverFlow = 0; // Track frame time filling compared to target FPS
	double frameOverFlowMax = 0; // History of peak frame time filling compared to target FPS

	while (window.isOpen())
	{

		elapsedTime = clockGeneral.getElapsedTime();
		//std::cout << std::format("new frame at {0:.5f} s\n", elapsedTime.asSeconds());
		// 
		// Check time as the begginig of the frame computing 
		timeSinceLastFrame = clockFrame.restart();
		fps = 1.0 / timeSinceLastFrame.asSeconds();
		textFPS.setString(std::format("FPS : {0:.5f}", fps));

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		solutionExact.setTimeBoundaries(elapsedTime.asSeconds(), elapsedTime.asSeconds() + frameTime);
		solutionExact.initTimeRT();
		solutionExact.solveExact();
		solutionExact.calcEnergy();

		solutionRK4.setTimeBoundaries(elapsedTime.asSeconds(), elapsedTime.asSeconds() + frameTime);
		solutionRK4.initTimeRT();
		solutionRK4.solveRK4();
		solutionRK4.calcEnergy();
		solutionRK4.nextStep();

		solutionEuler.setTimeBoundaries(elapsedTime.asSeconds(), elapsedTime.asSeconds() + frameTime);
		solutionEuler.initTimeRT();
		solutionEuler.solveEuler();
		solutionEuler.calcEnergy();
		solutionEuler.nextStep();

		yExact = solutionExact.getPosition();
		yRK4 = solutionRK4.getPosition();
		yEuler = solutionEuler.getPosition();

		circleExact.setPosition(sf::Vector2f(540 + 100.0 * yExact, 250.0));
		circleRK4.setPosition(sf::Vector2f(540 + 100.0 * yRK4, 350.0));
		circleEuler.setPosition(sf::Vector2f(540 + 100.0 * yEuler, 450.0));


		emExact = solutionExact.getEm();
		emRK4 = solutionRK4.getEm();
		emEuler = solutionEuler.getEm();

		fExact = solutionExact.getFrequency();
		fRK4 = solutionRK4.getFrequency();
		fEuler = solutionEuler.getFrequency();

		textExact.setString(std::format("Exact	: {0:.3f}", emExact));
		textRK4.setString(std::format("RK4	: {0:.3f}", emRK4));
		textEuler.setString(std::format("Euler	: {0:.3f}", emEuler));

		//textExact.setString(std::format("Exact {0:.2f}Hz : {0:.3f}", fExact, emExact));
		//textRK4.setString(std::format("RK4 {0:.2f}Hz : {0:.3f}", fRK4, emRK4));
		//textEuler.setString(std::format("Euler {0:.2f}Hz : {0:.3f}", fEuler, emEuler));


		// Draw all elements
		window.clear();
		window.draw(textFPS);
		window.draw(textOF);
		window.draw(textOFMax);

		window.draw(circleExact);
		window.draw(circleRK4);
		window.draw(circleEuler);

		window.draw(textExact);
		window.draw(textEuler);
		window.draw(textRK4);

		window.display();


		// Check 
		frameElapsedTime = clockFrame.getElapsedTime();
		frameOverFlow = frameElapsedTime.asSeconds() / frameTime * 100.0;

		textOF.setString(std::format("Frame filling [%] : {:07.2f}", frameOverFlow));

		if ((frameOverFlow > frameOverFlowMax))
		{
			frameOverFlowMax = frameOverFlow;
			textOFMax.setString(std::format("Frame filling [%] : {:07.2f}", frameOverFlowMax));
		}

		// Stabilise the FPS by waiting before starting the next frame computing
		frameElapsedTime = clockFrame.getElapsedTime();
		while (frameElapsedTime.asSeconds() < frameTime)
		{
			frameElapsedTime = clockFrame.getElapsedTime();
			//std::cout << std::format("FET (micros s) : {0:.5f} \n", frameElapsedTime.asSeconds() * 1000.0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
		{
			solutionExact.initPosition();
			solutionRK4.initPosition();
			solutionEuler.initPosition();
			frameOverFlowMax = 0;
			clockGeneral.restart();
		}
	}
}