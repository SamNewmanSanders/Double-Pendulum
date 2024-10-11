#pragma once //This works in visual studio, but maybe not other compilers. Does the same job as an include guard

#include <SFML\Graphics.hpp>
#include <iostream>
#include <deque> //for the trail container

class DoublePendulum {
public:

	// Pendulum parameters
	double L1 = 150 , L2 = 150;      // Length of pendulums
	double m1 = 1, m2 = 1;      // Masses of pendulums
	double theta1, theta2;  // Angles from vertical
	double omega1, omega2;  // Angular velocities
	double g = 150;           // Gravitational constant

  DoublePendulum(sf::RenderWindow& window, double theta1, double theta2, double omega1, double omega2);

	sf::Vector2f convert_to_cartesian(double theta, double length); //helper function

	void update(double dt);

	void draw();

	sf::Vector2f currentPosition() {
		return origin.getPosition() + convert_to_cartesian(theta1, L1) + convert_to_cartesian(theta2, L2);
	}
	
private:
	sf::CircleShape origin;
	sf::CircleShape bob1, bob2;
	sf::CircleShape pin1, pin2;
	sf::RectangleShape rod1, rod2;

	sf::RenderWindow& window;
};
