#ifndef GRAPH_H
#define GRAPH_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
#include <iostream>

#include "RPN.h"

class Graph {
    public:
        sf::RenderTexture *myBuffer;

        Graph();
        Graph(int width, int height, float originPercentWidth, float originPercentHeight, float pixelEquivalent);

        void CreateRectLine(float startX, float startY, float endX, float endY, float lineThickness, sf::Color col);
        void CreateSingleLine(float startX, float startY, float endX, float endY, sf::Color col);
        void CreateAxis();
        void CreateMarker(float markerThickness);

        void CreatePoint(float x, float y, float rad);

        std::pair<float, float> ConvertCoordsToScreen(float x, float y);     
        std::pair<float, float> ConvertCoordsToDescartes(float x, float y);
        
        void SetQuarters(bool first, bool second, bool third, bool fourth);
        void CreateGraph();
        float CalculateGraph(float x);
        void SetGraphLineThickness(float thick);

        void SetPixelEquivalent(float eqi);
        void SetSpacing(float spacing);

        void SetExpression(std::string ex);
        void Debug();

        void ClearDrawBuffer();
        void DisplayDrawBuffer();
        void SetBufferPosition(float x, float y);

        void SetBackgroundColor(sf::Color col);
        void SetLineGraphColor(sf::Color col);

        bool QuarterCheck(float x, float y, int quart);

        void SetFont(sf::Font& font);
        void CreateText(float x, float y, std::string text, float size, std::pair<float, float> spacing, std::pair<float, float> originPer);
        void CreateSpaceText(float spacing, float size);

    private:
        float windowWidth;
        float windowHeight;

        float spacing;
        float pixelEquivalent; //1 unit = ? pixel
        float lineSize;

        float originPercentWidth;
        float originPercentHeight;
        float originX;
        float originY;

        float leftDescartesBound;
        float rightDescartesBound;
        float topDescartesBound;
        float bottomDescartesBound;

        std::string expression;
        std::vector<std::string> myRPN;

        sf::Color backgroundColor;
        sf::Color lineColor;

        std::pair<float, float> bufferPos;
        bool quarters[4];

        sf::Font spaceFont;
};

#endif