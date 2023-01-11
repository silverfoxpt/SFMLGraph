#ifndef GRAPH_H
#define GRAPH_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

#include "RPN.h"

class Graph {
    public:
        Graph(int width, int height, sf::RenderWindow *myWindow, float originPercentWidth, float originPercentHeight);

        sf::RectangleShape CreateLine(float startX, float startY, float endX, float endY, float lineThickness, sf::Color col);
        void CreateSingleLine(float startX, float startY, float endX, float endY, sf::Color col);
        void CreateAxis(float axisThickness);
        void CreateMarker(float markerThickness);

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
        void SetBufferPosition(float x, float y);

        void SetBackgroundColor(sf::Color col);
        void SetLineGraphColor(sf::Color col);

    private:
        float windowWidth;
        float windowHeight;

        sf::RenderWindow *myWindow;
        sf::RenderTexture *myBuffer;

        float spacing;
        float pixelEquivalent; //1 unit = ? pixel

        float originPercentWidth;
        float originPercentHeight;
        float originX;
        float originY;

        std::string expression;
        std::vector<std::string> myRPN;

        sf::Color backgroundColor;
        sf::Color lineColor;

        std::pair<float, float> bufferPos;
};

#endif