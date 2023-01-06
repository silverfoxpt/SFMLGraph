#ifndef GRAPH_H
#define GRAPH_H

#include <SFML/Graphics.hpp>
#include <vector>

class Graph {
    public:
        Graph(sf::RenderWindow *window);

        void CreateLine(float startX, float startY, float endX, float endY, float lineThickness);
        void CreateAxis(float axisThickness);
        void DrawAxis();

        void CreatePoint(float x, float y, float rad);
        void DrawPoint();
        void ClearAllPoints();

        std::pair<float, float> ConvertCoordsToScreen(float x, float y);     
        std::pair<float, float> ConvertCoordsToDescartes(float x, float y);

    private:
        float windowWidth;
        float windowHeight;

        sf::RenderWindow *myWindow;

        std::vector<sf::CircleShape> points;
        std::vector<sf::RectangleShape> axes;
};

#endif