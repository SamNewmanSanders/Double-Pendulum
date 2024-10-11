#include "Pendulum.h"
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;


int main() {

    sf::RenderWindow window(sf::VideoMode(1800, 1000), "Mouse Trail Example");
    window.setFramerateLimit(0);

    int frameCount = 0; // To keep track of the number of frames captured for the video file


    int numPendulums = 1;
    std::vector<DoublePendulum> pendulumList;

    for (int i = 0; i < numPendulums; i++) {

      DoublePendulum doublependulum(window, 2.2, 2.6+ static_cast<float>(i)/6000, 0, 0);

      pendulumList.push_back(doublependulum);
    }

    sf::Clock clock;

    std::deque<std::vector<sf::Vector2f>> trails;

    int maxTrailLength = 130;

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        bool video = false;
        float deltaTime;

        if (video) {
          deltaTime = 0.01;
        }
        else {
          deltaTime = clock.restart().asSeconds();
          //deltaTime = 0.1;
        }

        // Render
        window.clear(sf::Color::Black);

        std::vector<sf::Vector2f> trails_for_one_frame;
        for (DoublePendulum pendulum : pendulumList) {
          trails_for_one_frame.push_back(pendulum.currentPosition());
        }

        trails.push_back(trails_for_one_frame);
        if (trails.size() > maxTrailLength) {
          trails.pop_front(); // Remove the oldest position
        }

        for (int i = 0; i < trails.size(); ++i) {
            
           for (int j = 0; j < trails[i].size(); ++j){

            // Calculate alpha: newer points are more opaque (255), older points are more transparent (near 0)
            // we want alpha to be lowest for front of the list

            float k = 4.0f; // Adjust this value to control the rate of decay (EXPONENTIAL EXPRESSION)
            float alpha = 255 * std::exp(-k * (static_cast<float>(trails.size() - 1 - i) / maxTrailLength));


            sf::Color colour = sf::Color(255 - j * 255 / numPendulums, 10, j * 255 / numPendulums, static_cast<sf::Uint8>(alpha)); //colour needs to be in the inner loop

            sf::RectangleShape trailPoint; // Size of the trail point
            trailPoint.setSize(sf::Vector2f(5, 5));
            trailPoint.setOrigin(5, 5);
            trailPoint.setPosition(trails[i][j]); //where this is the position vector
            trailPoint.setFillColor(colour);

            window.draw(trailPoint); // Draw the trail point
          }
        }

        for (DoublePendulum& doublependulum : pendulumList) { //draw pendulum elements AFTER AS ORIGIN SHOULD BE VISIBLE

          doublependulum.update(deltaTime);
          doublependulum.draw();
        }

        window.display();


        if (video) {
          // Capture the current frame
          sf::Texture texture;
          texture.create(window.getSize().x, window.getSize().y);
          texture.update(window);

          sf::Image image = texture.copyToImage();

          // Create a filename using the frame count
          std::ostringstream filename;
          filename << "frame_" << std::setw(4) << std::setfill('0') << frameCount << ".png";

          // Save the image to a file
          image.saveToFile(filename.str());
          frameCount++; // Increment frame count
        }
    }

    return 0;
}
