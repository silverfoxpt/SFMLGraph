#include <SFML/Graphics.hpp>
#include <iostream>
#include "graph.h"
#include "RPN.h"
#include <vector>
#include <string>

void test() {
    //test 1
    std::vector<std::string> a = RPN::infixToRPN("(3+7/7-2/5)*(9+6/(3-7))");
    for (int i = 0; i < a.size(); i++) {
        std::cout << a[i] << " ";
    }
    std::cout << '\n';

    //test 2;
    std::cout << RPN::RPNToValue(a, 1.0);
}

int main()
{
    //test();
    sf::RenderWindow window(sf::VideoMode(640, 640), "SFML project");

    Graph newGraph(&window);
    newGraph.SetExpression("tan(abs(tan(sin(x)*2)/2)+2)-1");
    newGraph.CreateAxis(1.0);
    newGraph.SetPixelEquivalent(20); //1 unit = ? pixel
    newGraph.SetSpacing(0.02);
    newGraph.CreateGraph();

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
        newGraph.DrawGraph();
        newGraph.DrawPoint();

        window.display();
    }

    return 0;
}