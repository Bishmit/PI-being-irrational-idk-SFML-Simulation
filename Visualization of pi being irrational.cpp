#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

#define PI 3.14159

class Pendulum {
    sf::Vector2f piv;           // Pivot point
    sf::Vector2f bob;           // Bob position
    sf::Vector2f bob2;          // Second bob position
    sf::RectangleShape pivot;   // Rectangle representing pivot 
    sf::CircleShape circle;     // First bob
    sf::CircleShape circle2;    // Second bob
    sf::Vertex line1[2];        // Line from pivot to first bob
    sf::Vertex line2[2];        // Line from first bob to second bob
    std::vector<sf::Vector2f> path;    // Path points for tracing
    sf::VertexArray pathLine;  // Path line
    float windowWidth;          // Width of the window
    float windowHeight;         // Height of the window
    float gravity;            // Acceleration due to gravity
    float bobRadius;          // Radius of circular motion for first bob
    float bob2Radius;         // Radius of circular motion for second bob
    float bobSpeed;           // Angular speed for first bob
    float bob2Speed;          // Angular speed for second bob
    float time;               // Elapsed time for simulation
    float bob2Angle;          // Angle for second bob's circular motion

public:
    Pendulum(float windowWidth, float windowHeight) {
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
        piv = sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f);
        bob = sf::Vector2f(piv.x, piv.y); // Move bob slightly down
        bob2 = sf::Vector2f(bob.x , bob.y); // Place bob2 to the right of bob
        circle = sf::CircleShape(10);
        circle2 = sf::CircleShape(10);
        bobRadius = 100.0f; // Adjust the radius as needed
        bob2Radius = 100.0f; // Adjust the radius as needed
        bobSpeed = 0.999f; // Adjust the speed as needed
        bob2Speed = 0.099f; // Adjust the speed as needed
        time = 0.0f;

        circle.setFillColor(sf::Color::Green);
        circle2.setFillColor(sf::Color::Green);
        //pivot.setSize(sf::Vector2f(10.f, 10.f));
       // pivot.setFillColor(sf::Color::Yellow);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle2.setOrigin(circle2.getRadius(), circle2.getRadius());
        circle.setPosition(bob);
        circle2.setPosition(bob2);
        pivot.setOrigin(pivot.getSize().x / 2.f, pivot.getSize().y / 2.f);
        pivot.setPosition(piv);

        line1[0] = sf::Vertex(piv, sf::Color::Yellow);
        line1[1] = sf::Vertex(bob, sf::Color::Yellow);

        line2[0] = sf::Vertex(bob, sf::Color::White);
        line2[1] = sf::Vertex(bob2, sf::Color::White);
        
        pathLine.setPrimitiveType(sf::LineStrip);
    }

    void update() {
        // Circular motion for bob
        bob.x = piv.x + bobRadius * cos(bobSpeed * time);
        bob.y = piv.y + bobRadius * sin(bobSpeed * time);

        // Circular motion for bob2 independent of the pivot
        bob2.x = bob.x + bob2Radius * cos(bob2Angle);
        bob2.y = bob.y + bob2Radius * sin(bob2Angle);
        bob2Angle = bob2Angle + bob2Speed; // Increment the angle for bob2

        // Update time
        time += 0.02;// change this angle to see different patterns :p

        circle.setPosition(bob);
        circle2.setPosition(bob2);
        line1[0] = sf::Vertex(piv, sf::Color::White);
        line1[1] = sf::Vertex(bob, sf::Color::White);

        line2[0] = sf::Vertex(bob, sf::Color::White);
        line2[1] = sf::Vertex(bob2, sf::Color::White);

        path.push_back(bob2);
        pathLine.append(sf::Vertex(bob2, sf::Color::Yellow));
    }

    void render(sf::RenderWindow& window) {
        float centerX = windowWidth / 2.0f;
        float centerY = windowHeight / 2.0f;

        pivot.setPosition(centerX, centerY);
        circle.setPosition(bob.x, bob.y);
        circle2.setPosition(bob2.x, bob2.y);

        window.draw(pathLine);
        window.draw(line1, 2, sf::Lines);
        window.draw(circle);
        window.draw(circle2);
        window.draw(pivot);
        window.draw(line2, 2, sf::Lines);
    }
};

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    const unsigned int windowWidth = desktop.width;
    const unsigned int windowHeight = desktop.height;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "PI being irrational", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    Pendulum pend(windowWidth, windowHeight);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }

        window.clear();
        pend.update();
        pend.render(window);
        window.display();
    }

    return 0;
}
