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

	window.setFramerateLimit(62);

	sf::CircleShape circleExact(20.f);
	sf::CircleShape circleEuler(20.f);
	sf::CircleShape circleRK4(20.f);

	circleExact.setFillColor(sf::Color(255, 0, 0, 255));
	circleEuler.setFillColor(sf::Color(0, 255, 0, 255));
	circleRK4.setFillColor(sf::Color(0, 0, 255, 255));

	sf::Font font("arial.ttf"); // Throws sf::Exception if an error occurs

	sf::Text text(font);
	text.setString("hello");

	sf::Text textExact(font);
	textExact.setString("Exact");

	sf::Text textEuler(font);
	textEuler.setString("Euler");

	sf::Text textRK4(font);
	textRK4.setString("RK4");

	sf::Clock clockGeneral;
	sf::Clock clockFrame;

	float yExact(0.0), yEuler(0.0), yRK4(0.0);

	while (window.isOpen())
	{
		sf::Time timeSinceLastFrame = clockFrame.restart();
		sf::Time elapsedTime = clockGeneral.getElapsedTime();
		std::cout << std::format("time error (micros s) : {0:.5f}", ((1.0/62.0)-timeSinceLastFrame.asSeconds())*1000.0) << "\n";

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		//    //text.setString(std::format("Time since a little butt is watching this : {0:.5f}", elapsedTime.asSeconds()));
		text.setString(std::format("FPS : {0:.1f}", 1.0 / timeSinceLastFrame.asSeconds()));
		text.setPosition(sf::Vector2f(100, 100));

		yExact = sin(0.3 * elapsedTime.asSeconds() * 2 * std::numbers::pi+0.5);
		yEuler = sin(0.4 * elapsedTime.asSeconds() * 2 * std::numbers::pi+0.5);
		yRK4 = sin(0.5 * elapsedTime.asSeconds() * 2 * std::numbers::pi+0.5);


		circleExact.setPosition(sf::Vector2f(200.0 + 100.0 * yExact, 250.0));
		circleEuler.setPosition(sf::Vector2f(200.0 + 100.0 * yEuler, 350.0));
		circleRK4.setPosition(sf::Vector2f(200.0 + 100.0 * yRK4, 450.0));


		textExact.setPosition(sf::Vector2f(200.0 + 100.0 * yExact, 200.0));
		textEuler.setPosition(sf::Vector2f(200.0 + 100.0 * yEuler, 300.0));
		textRK4.setPosition(sf::Vector2f(200.0 + 100.0 * yRK4, 400.0));


		window.clear();
		window.draw(text);

		window.draw(circleExact);
		window.draw(circleEuler);
		window.draw(circleRK4);

		window.draw(textExact);
		window.draw(textEuler);
		window.draw(textRK4);

		window.display();
	}
}