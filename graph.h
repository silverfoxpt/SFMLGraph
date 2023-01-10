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

        void CreatePoint(float x, float y, float rad);

        std::pair<float, float> ConvertCoordsToScreen(float x, float y);     
        std::pair<float, float> ConvertCoordsToDescartes(float x, float y);

        void DrawGraph();
        void CreateGraph();
        float CalculateGraph(float x);

        void SetPixelEquivalent(float eqi);
        void SetSpacing(float spacing);

        void SetExpression(std::string ex);
        void Debug();

        void ClearDrawBuffer();
        void DisplayDrawBuffer();

    private:
        float windowWidth;
        float windowHeight;

        sf::RenderWindow *myWindow;
        sf::RenderTexture *myBuffer;

        float spacing;
        float pixelEquivalent; //1 unit = ? pixel

        std::string expression;
        std::vector<std::string> myRPN;
};

#endif