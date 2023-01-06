#include <SFML/Graphics.hpp>
#include "graph.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML project");
    Graph newGraph(&window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        //DrawAxis(window);
        newGraph.DrawAxis();
        newGraph.PlotPoint(20, 20, 3);
        newGraph.PlotPoint(0, 0, 3);
        newGraph.PlotPoint(-20, -20, 3);

        window.display();
    }

    return 0;
}