#include <SFML/Graphics.hpp>
#include <format>
#include <cmath>
#include <numbers>
#include <iostream>
#include "Solution.hpp"
#include "System.hpp"


int main()
{

	//window.setFramerateLimit(62);

	sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!");

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
	textOF.setPosition(sf::Vector2f(500, 100));
	textOFMax.setPosition(sf::Vector2f(500, 200));

	// Body legends
	sf::Text textExact(font);
	sf::Text textRK4(font);
	sf::Text textEuler(font);

	textExact.setString("Exact");
	textRK4.setString("RK4");
	textEuler.setString("Euler");

	textExact.setPosition(sf::Vector2f(200.0 + 100.0, 200.0));
	textRK4.setPosition(sf::Vector2f(200.0 + 100.0, 300.0));
	textEuler.setPosition(sf::Vector2f(200.0 + 100.0, 400.0));

	textExact.setFillColor(sf::Color(255, 255, 255, 255));
	textRK4.setFillColor(sf::Color(0, 255, 0, 255));
	textEuler.setFillColor(sf::Color(255, 255, 0, 255));

	float yExact(0.0), yEuler(0.0), yRK4(0.0);

	// Timing features 
	sf::Clock clockGeneral; // General time clock
	sf::Time elapsedTime = clockGeneral.getElapsedTime();

	sf::Clock clockFrame; // Frame time clock
	sf::Time timeSinceLastFrame = clockFrame.restart();
	sf::Time frameElapsedTime = clockFrame.getElapsedTime();

	double fpsTarget = 62.0;
	double frameTime = 1.0 / fpsTarget;
	double fps=0;

	double frameOverFlow = 0; // Track frame time filling compared to target FPS
	double frameOverFlowMax = 0; // History of peak frame time filling compared to target FPS



	while (window.isOpen())
	{
		elapsedTime = clockGeneral.getElapsedTime();
		// Check time as the begginig of the frame computing 
		timeSinceLastFrame = clockFrame.restart();
		fps = 1.0 / timeSinceLastFrame.asSeconds();
		textFPS.setString(std::format("FPS : {0:.5f}", fps));

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		yExact = sin(0.3 * elapsedTime.asSeconds() * 2 * std::numbers::pi + 0.5);
		yRK4 = sin(0.5 * elapsedTime.asSeconds() * 2 * std::numbers::pi + 0.5);
		yEuler = sin(0.4 * elapsedTime.asSeconds() * 2 * std::numbers::pi + 0.5);

		circleExact.setPosition(sf::Vector2f(200.0 + 100.0 * yExact, 250.0));
		circleRK4.setPosition(sf::Vector2f(200.0 + 100.0 * yRK4, 350.0));
		circleEuler.setPosition(sf::Vector2f(200.0 + 100.0 * yEuler, 450.0));


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

		if ((frameOverFlow > frameOverFlowMax) && (elapsedTime.asSeconds() > 2))
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

	}
}