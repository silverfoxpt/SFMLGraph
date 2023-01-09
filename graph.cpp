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
sf::RectangleShape Graph::CreateLine(float startX, float startY, float endX, float endY, float lineThickness, sf::Color col = sf::Color::White) {
    int width = dist(startX, startY, endX, endY);
    std::pair<float, float> toScreen = this->ConvertCoordsToScreen(startX, startY);

    sf::RectangleShape line(sf::Vector2f(width, lineThickness));
    line.setOrigin(sf::Vector2f(0, lineThickness/2.0));
    line.setPosition(sf::Vector2f(toScreen.first, toScreen.second));
    line.setFillColor(col);

    //rotation
    int x1 = endX - startX, y1 = endY - startY;
    float angle = angleBetweenTwoVector(x1, y1, 1, 0);
    line.setRotation((endY < startY) ? angle : 360-angle);

    return line;
}

void Graph::CreateAxis(float axisThickness) {
    float halfWidth = this->windowWidth/2.0;
    float halfHeight = this->windowHeight/2.0;

    this->axes.push_back(this->CreateLine(0, halfHeight, 0, -halfHeight, axisThickness, sf::Color::Red));
    this->axes.push_back(this->CreateLine(-halfWidth, 0, halfWidth, 0, axisThickness, sf::Color::Red));
}

void Graph::DrawAxis() {
    for (auto line: this->axes) {
        this->myWindow->draw(line);std::vector<std::string> rpn = RPN::infixToRPN(expression);
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

#pragma region graph
void Graph::SetPixelEquivalent(float eqi) {
    this->pixelEquivalent = eqi;
}

void Graph::SetSpacing(float spacing) {
    this->spacing = spacing;
}

void Graph::CreateGraph() {
    float x = -this->windowWidth/2.0;
    std::vector<std::pair<float, float>> po;

    while(x <= this->windowWidth/2.0) {
        float plotX = x * this->pixelEquivalent;
        float plotY = this->CalculateGraph(x) * this->pixelEquivalent;
        //CreatePoint(plotX, plotY, 2);

        std::pair<float, float> newPoint(plotX, plotY);
        po.push_back(newPoint);

        x += this->spacing;
    }

    //create lines
    int n = po.size();
    for (int i = 0; i < n-1; i++) {
        int x1 = po[i].first, y1 = po[i].second;
        int x2 = po[i+1].first, y2 = po[i+1].second;

        sf::RectangleShape newLineConnection = CreateLine(x1, y1, x2, y2, 1.0);
        this->graphLine.push_back(newLineConnection);
    }
}

void Graph::DrawGraph() {
    for (auto line: this->graphLine) {
        this->myWindow->draw(line);
    }
}

float Graph::CalculateGraph(float x) {
    return RPN::RPNToValue(this->myRPN, x);
}

void Graph::SetExpression(std::string ex) {
    this->expression = ex;
    this->myRPN = RPN::infixToRPN(ex);
}
#pragma endregion