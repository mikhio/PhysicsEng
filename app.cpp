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
float dot(sf::Vector2f v1, sf::Vector2f v2) { return v1.x * v2.x + v1.y * v2.y; }
sf::Vector2f pow(sf::Vector2f v, int n) { return sf::Vector2f(pow(v.x, n), pow(v.y, n)); }

void getCollison(sf::Vector2f *points, bool *collFlags, sf::Vector2f c, Line *lines, float rad, int linesSize) {
    for (int i = 0; i < linesSize; ++i) {
        bool isCollide = true;

        sf::Vector2f p1 = lines[i].p1;
        sf::Vector2f p2 = lines[i].p2;

        float len = getLength(p1, p2);
        float dot = ( (p2.x-p1.x)*(c.x-p1.x) + (p2.y-p1.y)*(c.y-p1.y) ) / pow(len, 2);
        float closeX = p1.x + dot * (p2.x - p1.x);
        float closeY = p1.y + dot * (p2.y - p1.y);

        if (!((closeX >= p1.x && closeX <= p2.x) || (closeX >= p2.x && closeX <= p1.x))) isCollide = false;
        if (pow((c.x - closeX), 2) + pow((c.y - closeY), 2) > pow(rad, 2)) isCollide = false;

        points[i] = sf::Vector2f(closeX, closeY);

        if (!isCollide) {
            if (getLength(c, p1) <= rad) {
                isCollide = true;
                points[i] = p1;
            } else if (getLength(c, p2) <= rad) {
                isCollide = true;
                points[i] = p2;
            }
        }

        collFlags[i] = isCollide;
    }
}



int main() {
    // Create window and set FPS
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "Physics");
    window.setFramerateLimit(60);

    // Circle 
    Ball ball(sf::Vector2f(500.f, 270.f), 100.f);

    // Line
    Line lines[] = {
        Line(sf::Vector2f(0.f, 0.f), sf::Vector2f(1500.f, 0.f)),
        Line(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.f, 1500.f)),
        Line(sf::Vector2f(1500.f, 0.f), sf::Vector2f(1500.f, 1500.f)),
        Line(sf::Vector2f(0.f, 1500.f), sf::Vector2f(1500.f, 1500.f)),

        Line(sf::Vector2f(200.f, 800.f), sf::Vector2f(600.f, 1000.f)),
        Line(sf::Vector2f(1300.f, 800.f), sf::Vector2f(700.f, 1000.f)),
    };

    // Circle and line collison
    sf::Vector2f points[sizeof(lines)/sizeof(lines[0])];
    bool collFlags[sizeof(lines)/sizeof(lines[0])];

    while (window.isOpen()) {
        // Check event
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }
        ball.acc = sf::Vector2f(0.f, GRAVITY);

        getCollison(points, collFlags, ball.pos, lines, ball.rad, sizeof(lines)/sizeof(lines[0]));

        for (int i = 0; i < sizeof(lines)/sizeof(lines[0]); ++i) {
            if (collFlags[i]) {
                sf::Vector2f point = points[i];

                sf::Vector2f bounce(ball.pos.x - point.x, ball.pos.y - point.y);

                bounce /= getLength(ball.pos, point);
                ball.vel = 0.8f * dot(-bounce, ball.vel) * -bounce + ball.vel - dot(-bounce, ball.vel) * -bounce;
                bounce *= (ball.rad - getLength(ball.pos, point));

                ball.acc += bounce;
            }

        }


        sf::Vector2f envResist = pow(ball.vel, 2) * 0.01f;

        //ball.acc -= envResist;

        ball.vel += ball.acc;
        //ball.vel = (getLength(sf::Vector2f(0.f, 0.f), ball.vel) <= 10f) ? sf::Vector2f(0.f, 0.f) : ball.vel;
        ball.move(ball.vel);


        // Clearing, updating and drawing
        window.clear(sf::Color(0, 0, 0));
        
        window.draw(ball.shape);
        for (int i = 0; i < sizeof(lines)/sizeof(lines[0]); ++i) window.draw(lines[i].lineShape, 2, sf::Lines);

        window.display();
    }

    return 0;
}
