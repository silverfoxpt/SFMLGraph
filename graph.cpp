#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "graph.h"

#pragma region mathStuff
const float radToDeg = 57.295779513;

float dist(float startX, float startY, float endX, float endY) {
    return std::sqrt((startX - endX) * (startX - endX) + (startY - endY) * (startY - endY));
}

float dotProduct(float x1, float y1, float x2, float y2) {
    return x1*x2 + y1*y2;
}

float angleBetweenTwoVector(float x1, float y1, float x2, float y2) {
    return std::acos(
        dotProduct(x1, y1, x2, y2)/
        (dist(x1, y1, 0, 0) * dist(x2, y2, 0, 0)) 
    ) * radToDeg;
}
#pragma endregion

Graph::Graph(sf::RenderWindow *window) {
    this->myWindow = window;
    this->windowWidth = this->myWindow->getSize().x;
    this->windowHeight = this->myWindow->getSize().y;
}

#pragma region line
void Graph::CreateLine(float startX, float startY, float endX, float endY, float lineThickness) {
    int width = dist(startX, startY, endX, endY);
    std::pair<float, float> toScreen = this->ConvertCoordsToScreen(startX, startY);

    sf::RectangleShape line(sf::Vector2f(width, lineThickness));
    line.setOrigin(sf::Vector2f(0, lineThickness/2.0));
    line.setPosition(sf::Vector2f(toScreen.first, toScreen.second));

    //rotation
    int x1 = endX - startX, y1 = endY - startY;
    float angle = angleBetweenTwoVector(x1, y1, 1, 0);
    line.setRotation(angle);

    this->axes.push_back(line);

}

void Graph::CreateAxis(float axisThickness) {
    float halfWidth = this->windowWidth/2.0;
    float halfHeight = this->windowHeight/2.0;

    this->CreateLine(0, halfHeight, 0, -halfHeight, axisThickness);
    this->CreateLine(-halfWidth, 0, halfWidth, 0, axisThickness);
}

void Graph::DrawAxis() {
    for (auto line: this->axes) {
        this->myWindow->draw(line);
    }
}
#pragma endregion

#pragma region pointConversion
std::pair<float, float> Graph::ConvertCoordsToDescartes(float x, float y) {
    float halfWidth = this->windowWidth/2.0;
    float halfHeight = this->windowHeight/2.0;

    std::pair<float, float> newPair;
    newPair.first = x - halfWidth;
    newPair.second = -(y - halfHeight);

    return newPair;
}

std::pair<float, float> Graph::ConvertCoordsToScreen(float x, float y) {
    float halfWidth = this->windowWidth/2.0;
    float halfHeight = this->windowHeight/2.0;

    std::pair<float, float> newPair;
    newPair.first = x + halfWidth;
    newPair.second = -y + halfHeight;

    return newPair;
}
#pragma endregion

#pragma region point
void Graph::CreatePoint(float x, float y, float rad) {
    std::pair<float, float> coords = ConvertCoordsToScreen(x, y);

    sf::CircleShape point = sf::CircleShape(rad);
    point.setOrigin(sf::Vector2f(rad, rad));
    point.setFillColor(sf::Color::Red);
    point.setPosition(sf::Vector2f(coords.first, coords.second));

    this->points.push_back(point);
}

void Graph::DrawPoint() {
    for (auto &point: this->points) {
        this->myWindow->draw(point);
    }
}

void Graph::ClearAllPoints() {
    this->points.clear();
}
#pragma endregion
