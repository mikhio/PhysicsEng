#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define GRAVITY 2
#define PI      3.14159265


struct Line {
    float k;
    float c;
    sf::Vector2f p1;
    sf::Vector2f p2;
};

Line createLine(sf::Vector2f p1, sf::Vector2f p2) {
    Line resLine;

    resLine.k = (p2.y - p1.y) / (p2.x - p1.x);
    resLine.c = p1.y - resLine.k * p1.x;
    resLine.p1 = p1;
    resLine.p2 = p2;

    return resLine;
}

bool getCollison(sf::Vector2f *point, sf::Vector2f c, sf::Vector2f p1, sf::Vector2f p2, float rad) {
    float len = sqrt( pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
    float dot = ( (p2.x-p1.x)*(c.x-p1.x) + (p2.y-p1.y)*(c.y-p1.y) ) / pow(len, 2);
    float closeX = p1.x + dot * (p2.x - p1.x);
    float closeY = p1.y + dot * (p2.y - p1.y);

    if (pow((c.x - closeX), 2) + pow((c.y - closeY), 2) > pow(rad, 2)) return false;
    
    *point = sf::Vector2f(closeX, closeY);

    return true;
}

int main() {
    // Create window and set FPS
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "Physics");
    window.setFramerateLimit(60);

    // Circle 
    sf::CircleShape circle(100.f);
    circle.setPosition(sf::Vector2f(700.f, 1170.f));
    circle.setOrigin(100.f, 100.f);

    // Line
    Line lineData = createLine(sf::Vector2f(500.f, 900.f), sf::Vector2f(1000.f, 1000.f));
    sf::Vertex lineShape[] = {
        sf::Vertex(lineData.p1),
        sf::Vertex(lineData.p2)
    };

    // Circle and line collison
    sf::Vector2f point;
    bool isCollide = getCollison(&point, circle.getPosition(), lineData.p1, lineData.p2, circle.getRadius());

    sf::CircleShape pointShape1(10.f);
    pointShape1.setFillColor(sf::Color::Red);
    pointShape1.setOrigin(10.f, 10.f);

    while (window.isOpen()) {
        // Check event
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        circle.move(0.f, -1.f);

        isCollide = getCollison(&point, circle.getPosition(), lineData.p1, lineData.p2, circle.getRadius());

        pointShape1.setPosition(point);

        // Clearing, updating and drawing
        window.clear(sf::Color(0, 0, 0));
        
        window.draw(circle);
        window.draw(lineShape, 2, sf::Lines);

        if (isCollide) {
            window.draw(pointShape1);
        }

        window.display();
    }

    return 0;
}
