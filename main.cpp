#include <SFML/Graphics.hpp>
#include "graph.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML project");

    Graph newGraph(&window);
    newGraph.CreateAxis(1.0);
    newGraph.CreatePoint(20, 20, 3);
    newGraph.CreatePoint(0, 0, 3);
    newGraph.CreatePoint(-20, -20, 3);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        
        newGraph.DrawAxis();
        newGraph.DrawPoint();

        window.display();
    }

    return 0;
}