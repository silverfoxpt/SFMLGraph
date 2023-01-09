#ifndef GRAPH_H
#define GRAPH_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

#include "RPN.h"

class Graph {
    public:
        Graph(sf::RenderWindow *window);

        sf::RectangleShape CreateLine(float startX, float startY, float endX, float endY, float lineThickness, sf::Color col);
        void CreateAxis(float axisThickness);
        void DrawAxis();

        void CreatePoint(float x, float y, float rad);
        void DrawPoint();
        void ClearAllPoints();

        std::pair<float, float> ConvertCoordsToScreen(float x, float y);     
        std::pair<float, float> ConvertCoordsToDescartes(float x, float y);

        void DrawGraph();
        void CreateGraph();
        float CalculateGraph(float x);

        void SetPixelEquivalent(float eqi);
        void SetSpacing(float spacing);

        void SetExpression(std::string ex);

    private:
        float windowWidth;
        float windowHeight;

        sf::RenderWindow *myWindow;

        std::vector<sf::CircleShape> points;
        std::vector<sf::RectangleShape> axes;
        std::vector<sf::RectangleShape> graphLine;

        float spacing;
        float pixelEquivalent; //1 unit = ? pixel

        std::string expression;
        std::vector<std::string> myRPN;
};

#endif