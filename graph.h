#ifndef GRAPH_H
#define GRAPH_H

#include <SFML/Graphics.hpp>

class Graph {
    public:
        sf::RenderWindow *myWindow;

        Graph(sf::RenderWindow *window) {
            this->myWindow = window;
            this->windowWidth = this->myWindow->getSize().x;
            this->windowHeight = this->myWindow->getSize().y;
        }

        void DrawLine(float startX, float startY, float endX, float endY) {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(startX, startY)),
                sf::Vertex(sf::Vector2f(endX, endY))
            };

            this->myWindow->draw(line, 2, sf::Lines);
        }

        void DrawAxis() {
            float halfWidth = this->windowWidth/2.0;
            float halfHeight = this->windowHeight/2.0;

            DrawLine(0, halfHeight, this->windowWidth, halfHeight);
            DrawLine(halfWidth, 0, halfWidth, this->windowHeight);
        }

        std::pair<float, float> ConvertCoordsToScreen(float x, float y) {
            float halfWidth = this->windowWidth/2.0;
            float halfHeight = this->windowHeight/2.0;

            std::pair<float, float> newPair;
            newPair.first = x + halfWidth;
            newPair.second = -y + halfHeight;

            return newPair;
        }

        void PlotPoint(float x, float y, float rad) {
            std::pair<float, float> coords = ConvertCoordsToScreen(x, y);
            sf::CircleShape point(rad);
            point.setOrigin(sf::Vector2f(rad, rad));
            point.setFillColor(sf::Color::Red);
            point.setPosition(sf::Vector2f(coords.first, coords.second));

            this->myWindow->draw(point);
        }

    private:
        float windowWidth;
        float windowHeight;
};

#endif