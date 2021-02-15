#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#define GRAVITY 1
#define PI      3.14159265


struct Line {
    sf::Vector2f p1;
    sf::Vector2f p2;
    sf::Vertex lineShape[2];

    Line (sf::Vector2f p1, sf::Vector2f p2) {
        this->p1 = p1;
        this->p2 = p2;
        this->lineShape[0] = sf::Vertex(p1);
        this->lineShape[1] = sf::Vertex(p2);
    }
};

struct Ball {
    float rad;
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f acc;
    sf::CircleShape shape;

    Ball (sf::Vector2f pos, float rad) {
        this->rad = rad;
        this->pos = pos;

        this->updateShape();
    }
    
    void updateShape() {
        this->shape.setRadius(this->rad);
        this->shape.setPosition(this->pos);
        this->shape.setOrigin(this->rad, this->rad);
    }

    void move(sf::Vector2f vm) {
        this->pos += vm;
        updateShape();
    }
};


float getLength(sf::Vector2f p1, sf::Vector2f p2) { return sqrt( pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2)); }

bool getCollison(sf::Vector2f *point, sf::Vector2f c, sf::Vector2f p1, sf::Vector2f p2, float rad) {
    float len = getLength(p1, p2);
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
    Ball ball(sf::Vector2f(700.f, 270.f), 100.f);

    // Line
    Line lineData(sf::Vector2f(500.f, 900.f), sf::Vector2f(1000.f, 1000.f));

    // Circle and line collison
    sf::Vector2f point;
    bool isCollide;
    sf::CircleShape pointShape1(10.f);
    pointShape1.setFillColor(sf::Color::Red);
    pointShape1.setOrigin(10.f, 10.f);

    while (window.isOpen()) {
        // Check event
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        ball.acc = sf::Vector2f(0.f, GRAVITY);

        isCollide = getCollison(&point, ball.pos, lineData.p1, lineData.p2, ball.rad);

        if (isCollide) {
            sf::Vector2f bounce(ball.pos.x - point.x, ball.pos.y - point.y);
            bounce /= getLength(ball.pos, point);
            bounce *= ball.rad - getLength(ball.pos, point);

            ball.acc += bounce;
        }

        pointShape1.setPosition(point);

        ball.vel += ball.acc;
        ball.move(ball.vel);

        // Clearing, updating and drawing
        window.clear(sf::Color(0, 0, 0));
        
        window.draw(ball.shape);
        window.draw(lineData.lineShape, 2, sf::Lines);

        if (isCollide) {
            window.draw(pointShape1);
        }

        window.display();
    }

    return 0;
}
