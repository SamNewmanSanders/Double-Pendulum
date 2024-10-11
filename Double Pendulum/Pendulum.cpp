#include "Pendulum.h"

DoublePendulum::DoublePendulum(sf::RenderWindow& window, double theta1, double theta2, double omega1, double omega2)
	:window(window), theta1(theta1), theta2(theta2), omega1(omega1), omega2(omega2) {

  //Set up appropriate shapes
  origin.setRadius(5);
  origin.setFillColor(sf::Color::White);
  origin.setPosition(static_cast<float>(window.getSize().x/2), static_cast<float>(window.getSize().y/2)); //find centre of the screen
  origin.setOrigin(5, 5);

  bob1.setRadius(10);
  bob1.setFillColor(sf::Color::Red);
  bob1.setOrigin(10, 10);

  bob2.setRadius(10);
  bob2.setFillColor(sf::Color::Blue);
  bob2.setOrigin(10, 10);

  pin1.setRadius(3);
  pin1.setFillColor(sf::Color::White);
  pin1.setOrigin(3, 3);

  pin2.setRadius(3);
  pin2.setFillColor(sf::Color::White);
  pin2.setOrigin(3, 3);

  rod1.setSize(sf::Vector2f(L1, 2));
  rod1.setFillColor(sf::Color::White);
  rod1.setOrigin(L1 / 2, 1); // Set origin to shape centre so easier to manipulate

  rod2.setSize(sf::Vector2f(L1, 2));
  rod2.setFillColor(sf::Color::White);
  rod2.setOrigin(L1 / 2, 1); // Set origin to shape centre so easier to manipulate
}

void DoublePendulum::update(double dt)
// DO NOT UNDERSTAND THIS AT ALL, GUESSING WILL LEARN THIS THIS YEAR
{
  // Step 1: Calculate angular accelerations using the equations
  double delta_theta = theta1 - theta2;
  double denominator1 = L1 * (2 * m1 + m2 - m2 * cos(2 * delta_theta));
  double denominator2 = L2 * (2 * m1 + m2 - m2 * cos(2 * delta_theta));

  double a1 = (-g * (2 * m1 + m2) * sin(theta1) -
    m2 * g * sin(theta1 - 2 * theta2) -
    2 * sin(delta_theta) * m2 * (omega2 * omega2 * L2 + omega1 * omega1 * L1 * cos(delta_theta))) / denominator1;

  double a2 = (2 * sin(delta_theta) * (omega1 * omega1 * L1 * (m1 + m2) +
    g * (m1 + m2) * cos(theta1) +
    omega2 * omega2 * L2 * m2 * cos(delta_theta))) / denominator2;

  // Step 2: Update the angular velocities
  omega1 += a1 * dt;
  omega2 += a2 * dt;

  // Step 3: Update the angles
  theta1 += omega1 * dt;
  theta2 += omega2 * dt;

  // Update shape positions
  sf::Vector2f bob1_pos = origin.getPosition() + convert_to_cartesian(theta1, L1);
  sf::Vector2f bob2_pos = origin.getPosition() + convert_to_cartesian(theta1, L1) + convert_to_cartesian(theta2, L2);

  bob1.setPosition(bob1_pos);
  bob2.setPosition(bob2_pos);

  pin1.setPosition(bob1_pos);
  pin2.setPosition(bob2_pos);


  #define PI 3.14159265358979323846

  sf::Vector2f rod1_pos = sf::Vector2f((bob1_pos.x + origin.getPosition().x)/ 2, (bob1_pos.y + origin.getPosition().y) / 2);
  sf::Vector2f rod2_pos = sf::Vector2f((bob1_pos.x + bob2_pos.x) / 2, (bob1_pos.y + bob2_pos.y) / 2);

  rod1.setPosition(rod1_pos);
  rod2.setPosition(rod2_pos);

  rod1.setRotation(90 - theta1 * 180 / PI ); //90 accounts for offset
  rod2.setRotation(90 - (theta2) * 180 / PI); //90 accounts for offset

}

sf::Vector2f DoublePendulum::convert_to_cartesian(double theta, double length) {
  // Convert polar to Cartesian coordinates
  float x = static_cast<float>(length * std::sin(theta));
  float y = static_cast<float>(length * std::cos(theta));

  // Return the Cartesian coordinates as an sf::Vector2f
  return sf::Vector2f(x, y);
}

void DoublePendulum::draw() {
  window.draw(origin);

  window.draw(bob1);
  window.draw(bob2);

  window.draw(pin1);
  window.draw(pin2);

  window.draw(rod1);
  window.draw(rod2);
 
}

