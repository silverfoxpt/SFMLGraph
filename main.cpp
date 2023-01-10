#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "graph.h"
#include "RPN.h"
#include "IMGui Stuffs/imgui.h"
#include "IMGui Stuffs/imgui-SFML.h"


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

void GraphManipulation(Graph &newGraph) {
    newGraph.ClearDrawBuffer();

    newGraph.SetPixelEquivalent(20); //1 unit = ? pixel
    newGraph.SetSpacing(0.01);

    newGraph.SetExpression("tan(exp(sin(tan(abs(tan(sin(x)*2)/2)+2)-1)))");
    newGraph.CreateAxis(1.0);
    newGraph.CreateGraph();

    newGraph.DisplayDrawBuffer();
    newGraph.Debug();
}

int main()
{
    //test();
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML project");
    ImGui::SFML::Init(window);

    Graph newGraph(&window);
    GraphManipulation(newGraph);

    sf::Clock deltaTime;
    while (window.isOpen())
    {
        std::cout << "FPS: " << 1.0/deltaTime.getElapsedTime().asSeconds() << '\n';

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);    
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaTime.restart());
        window.clear();

        newGraph.DrawGraph();

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    
    return 0;
}