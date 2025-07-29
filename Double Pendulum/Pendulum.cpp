#include "Pendulum.h"

// Internal struct to hold the state of the pendulum
struct State {
    double theta1, theta2;
    double omega1, omega2;
};

// Helper function to compute derivatives (dtheta/dt and domega/dt)
static State compute_derivatives(const DoublePendulum& pend, const State& s) {
    double delta = s.theta1 - s.theta2;
    double m1 = pend.m1;
    double m2 = pend.m2;
    double L1 = pend.L1;
    double L2 = pend.L2;
    double g = pend.g;

    double denom1 = L1 * (2 * m1 + m2 - m2 * cos(2 * delta));
    double denom2 = L2 * (2 * m1 + m2 - m2 * cos(2 * delta));

    double a1 = (-g * (2 * m1 + m2) * sin(s.theta1)
        - m2 * g * sin(s.theta1 - 2 * s.theta2)
        - 2 * sin(delta) * m2 * (s.omega2 * s.omega2 * L2 + s.omega1 * s.omega1 * L1 * cos(delta)))
        / denom1;

    double a2 = (2 * sin(delta) * (s.omega1 * s.omega1 * L1 * (m1 + m2)
        + g * (m1 + m2) * cos(s.theta1)
        + s.omega2 * s.omega2 * L2 * m2 * cos(delta)))
        / denom2;

    return { s.omega1, s.omega2, a1, a2 };
}

DoublePendulum::DoublePendulum(sf::RenderWindow& window,
    double theta1, double theta2,
    double omega1, double omega2)
    : window(window)
    , theta1(theta1)
    , theta2(theta2)
    , omega1(omega1)
    , omega2(omega2)
{
    // Set up shapes
    origin.setRadius(5);
    origin.setFillColor(sf::Color::White);
    origin.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
    origin.setOrigin(5.f, 5.f);

    bob1.setRadius(10);
    bob1.setFillColor(sf::Color::Red);
    bob1.setOrigin(10.f, 10.f);

    bob2.setRadius(10);
    bob2.setFillColor(sf::Color::Blue);
    bob2.setOrigin(10.f, 10.f);

    pin1.setRadius(3);
    pin1.setFillColor(sf::Color::White);
    pin1.setOrigin(3.f, 3.f);

    pin2.setRadius(3);
    pin2.setFillColor(sf::Color::White);
    pin2.setOrigin(3.f, 3.f);

    rod1.setSize({ static_cast<float>(L1), 2.f });
    rod1.setFillColor(sf::Color::White);
    rod1.setOrigin(L1 / 2.f, 1.f);

    rod2.setSize({ static_cast<float>(L2), 2.f });
    rod2.setFillColor(sf::Color::White);
    rod2.setOrigin(L2 / 2.f, 1.f);
}

void DoublePendulum::update(double dt) {
    // Current state
    State s{ theta1, theta2, omega1, omega2 };

    // RK4 integration
    State k1 = compute_derivatives(*this, s);
    State s2{ s.theta1 + k1.theta1 * dt / 2, s.theta2 + k1.theta2 * dt / 2,
              s.omega1 + k1.omega1 * dt / 2, s.omega2 + k1.omega2 * dt / 2 };
    State k2 = compute_derivatives(*this, s2);

    State s3{ s.theta1 + k2.theta1 * dt / 2, s.theta2 + k2.theta2 * dt / 2,
              s.omega1 + k2.omega1 * dt / 2, s.omega2 + k2.omega2 * dt / 2 };
    State k3 = compute_derivatives(*this, s3);

    State s4{ s.theta1 + k3.theta1 * dt, s.theta2 + k3.theta2 * dt,
              s.omega1 + k3.omega1 * dt, s.omega2 + k3.omega2 * dt };
    State k4 = compute_derivatives(*this, s4);

    // Combine
    theta1 += dt * (k1.theta1 + 2 * k2.theta1 + 2 * k3.theta1 + k4.theta1) / 6.0;
    theta2 += dt * (k1.theta2 + 2 * k2.theta2 + 2 * k3.theta2 + k4.theta2) / 6.0;
    omega1 += dt * (k1.omega1 + 2 * k2.omega1 + 2 * k3.omega1 + k4.omega1) / 6.0;
    omega2 += dt * (k1.omega2 + 2 * k2.omega2 + 2 * k3.omega2 + k4.omega2) / 6.0;

    // Update shapes
    sf::Vector2f pos1 = origin.getPosition() + convert_to_cartesian(theta1, L1);
    sf::Vector2f pos2 = pos1 + convert_to_cartesian(theta2, L2);

    bob1.setPosition(pos1);
    pin1.setPosition(pos1);
    bob2.setPosition(pos2);
    pin2.setPosition(pos2);

    // Update rods
    sf::Vector2f mid1{ (origin.getPosition().x + pos1.x) / 2, (origin.getPosition().y + pos1.y) / 2 };
    sf::Vector2f mid2{ (pos1.x + pos2.x) / 2, (pos1.y + pos2.y) / 2 };
    rod1.setPosition(mid1);
    rod2.setPosition(mid2);

    const double PI = 3.14159265358979323846;
    rod1.setRotation(90.f - static_cast<float>(theta1 * 180.0 / PI));
    rod2.setRotation(90.f - static_cast<float>((theta1 + theta2) * 180.0 / PI));
}

sf::Vector2f DoublePendulum::convert_to_cartesian(double theta, double length) {
    return { static_cast<float>(length * sin(theta)),
             static_cast<float>(length * cos(theta)) };
}

void DoublePendulum::draw() {
    window.draw(origin);
    //window.draw(rod1);
    //window.draw(rod2);
    //window.draw(bob1);
    //window.draw(bob2);
    //window.draw(pin1);
    //window.draw(pin2);
}