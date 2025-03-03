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

	//Test parameters
	sf::Font font("arial.ttf"); // Throws sf::Exception if an error occurs

	int characterSizeHUD = 15;
	int characterSizeLegend = 30;
	int spacementCounterHUD = 0;

	// Create simulation system

	// Systeme parameters
	double m = 1;	// [kg]
	double k = 20;	// [N.m^-1]

	// Initial condition
	double x0 = 1;		// [m]

	// Solver condition
	int n = 40; // Make it work with 10000

	System system(m, k, x0);

	Solution solutionExact(system);
	Solution solutionEuler(system);
	Solution solutionRK4(system);

	solutionExact.setStepNumber(n);
	solutionExact.setName("Exact");
	solutionExact.setSolver("EXACT");
	solutionExact.initPosition();

	solutionEuler.setStepNumber(n);
	solutionEuler.setName("Euler");
	solutionEuler.setSolver("EULER");
	solutionEuler.initPosition();

	solutionRK4.setStepNumber(n);
	solutionRK4.setName("RK4");
	solutionRK4.setSolver("RK4");
	solutionRK4.initPosition();

	// Created simulation body objects

	sf::CircleShape circleExact(20.f);
	sf::CircleShape circleRK4(20.f);
	sf::CircleShape circleEuler(20.f);

	circleExact.setFillColor(sf::Color(255, 255, 255, 255));
	circleRK4.setFillColor(sf::Color(0, 255, 0, 255));
	circleEuler.setFillColor(sf::Color(255, 255, 0, 255));


	// HUD text
	sf::Text textFramePerSecond(font);
	sf::Text textFrameFilling(font);
	sf::Text textFrameFillingMax(font);

	textFramePerSecond.setString("FPS");
	textFrameFilling.setString("OF");
	textFrameFillingMax.setString("OFMax");

	textFramePerSecond.setPosition(sf::Vector2f(0, spacementCounterHUD += characterSizeHUD));
	textFrameFilling.setPosition(sf::Vector2f(0, spacementCounterHUD += characterSizeHUD));
	textFrameFillingMax.setPosition(sf::Vector2f(0, spacementCounterHUD += characterSizeHUD));


	textFramePerSecond.setCharacterSize(characterSizeHUD);
	textFrameFilling.setCharacterSize(characterSizeHUD);
	textFrameFillingMax.setCharacterSize(characterSizeHUD);

	// Solution details texts
	sf::Text textDetailsExact(font);
	sf::Text textDetailsRK4(font);
	sf::Text textDetailsEuler(font);

	textDetailsExact.setString("FPS");
	textDetailsRK4.setString("OF");
	textDetailsEuler.setString("OFMax");

	textDetailsExact.setPosition(sf::Vector2f(0, spacementCounterHUD += characterSizeHUD));
	textDetailsRK4.setPosition(sf::Vector2f(0, spacementCounterHUD += characterSizeHUD));
	textDetailsEuler.setPosition(sf::Vector2f(0, spacementCounterHUD += characterSizeHUD));

	textDetailsExact.setFillColor(sf::Color(255, 255, 255, 255));
	textDetailsRK4.setFillColor(sf::Color(0, 255, 0, 255));
	textDetailsEuler.setFillColor(sf::Color(255, 255, 0, 255));

	textDetailsExact.setCharacterSize(characterSizeHUD);
	textDetailsRK4.setCharacterSize(characterSizeHUD);
	textDetailsEuler.setCharacterSize(characterSizeHUD);

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
		textFramePerSecond.setString(std::format("FPS : {0:.5f}", fps));

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		solutionExact.forward(elapsedTime.asSeconds() + frameTime);
		//solutionExact.setTimeBoundaries(elapsedTime.asSeconds(), elapsedTime.asSeconds() + frameTime);
		//solutionExact.initTimeRT();
		//solutionExact.solveExact();
		//solutionExact.calcEnergy();

		solutionRK4.forward(elapsedTime.asSeconds() + frameTime);
		//solutionRK4.setTimeBoundaries(elapsedTime.asSeconds(), elapsedTime.asSeconds() + frameTime);
		//solutionRK4.initTimeRT();
		//solutionRK4.solveRK4();
		//solutionRK4.calcEnergy();
		//solutionRK4.nextStep();


		solutionEuler.forward(elapsedTime.asSeconds() + frameTime);
		//solutionEuler.setTimeBoundaries(elapsedTime.asSeconds(), elapsedTime.asSeconds() + frameTime);
		//solutionEuler.initTimeRT();
		//solutionEuler.solveEuler();
		//solutionEuler.calcEnergy();
		//solutionEuler.nextStep();

		yExact = solutionExact.getPosition();
		yRK4 = solutionRK4.getPosition();
		yEuler = solutionEuler.getPosition();

		circleExact.setPosition(sf::Vector2f(540 + 100.0 * yExact, 250.0));
		circleRK4.setPosition(sf::Vector2f(540 + 100.0 * yRK4, 350.0));
		circleEuler.setPosition(sf::Vector2f(540 + 100.0 * yEuler, 450.0));


		textDetailsExact.setString(solutionExact.getDetails());
		textDetailsRK4.setString(solutionRK4.getDetails());
		textDetailsEuler.setString(solutionEuler.getDetails());

		// Draw all elements
		window.clear();


		window.draw(circleExact);
		window.draw(circleRK4);
		window.draw(circleEuler);

		window.draw(textExact);
		window.draw(textEuler);
		window.draw(textRK4);


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3))
		{
			window.draw(textFramePerSecond);
			window.draw(textFrameFilling);
			window.draw(textFrameFillingMax);

			window.draw(textDetailsExact);
			window.draw(textDetailsEuler);
			window.draw(textDetailsRK4);


		}

		window.display();


		// Check 
		frameElapsedTime = clockFrame.getElapsedTime();
		frameOverFlow = frameElapsedTime.asSeconds() / frameTime * 100.0;

		textFrameFilling.setString(std::format("Frame filling [%] : {:07.2f}", frameOverFlow));

		if ((frameOverFlow > frameOverFlowMax))
		{
			frameOverFlowMax = frameOverFlow;
			textFrameFillingMax.setString(std::format("Frame filling max [%] : {:07.2f}", frameOverFlowMax));
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