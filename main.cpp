#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "graph.h"
#include "RPN.h"
#include "IMGui Stuffs/imgui.h"
#include "IMGui Stuffs/imgui-SFML.h"

//var
char buf[64]; 

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

void GraphManipulation(Graph &newGraph, std::string express) {
    newGraph.SetPixelEquivalent(30); //1 unit = ? pixel
    newGraph.SetSpacing(0.01);
    newGraph.SetQuarters(true, true, true, true);
      
    newGraph.SetBackgroundColor(sf::Color(255, 255, 255, 255));
    newGraph.SetLineGraphColor(sf::Color(0, 0, 0, 150));

    newGraph.SetBufferPosition(50, 50);

    newGraph.ClearDrawBuffer();

    newGraph.SetExpression(express);
    newGraph.CreateAxis();
    newGraph.CreateMarker(2.5);
    newGraph.CreateGraph();
    newGraph.CreateSpaceText(4, 10);

    newGraph.DisplayDrawBuffer();
    newGraph.Debug();
}

void SFMLUpdate(Graph &newGraph) {
    ImGui::Begin("Input");
    ImGui::Text("Enter equation:");

    ImGui::InputText(" ", buf, 64);
    if (ImGui::Button("Submit", sf::Vector2f(100, 30))) {
        std::string tmp = buf;
        GraphManipulation(newGraph, tmp);
    }

    ImGui::End();
}

int main()
{
    //test();
    sf::RenderWindow window(sf::VideoMode(1080, 720), "SFML project");
    ImGui::SFML::Init(window);

    Graph newGraph(640, 480, &window, 0.2, 0.8);
    GraphManipulation(newGraph, "tan(exp(sin(tan(abs(tan(sin(x)*2)/2)+2)-1)))");

    sf::Clock deltaTime;
    memset(buf, 0, sizeof(buf));

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

        SFMLUpdate(newGraph);

        newGraph.DrawGraph();

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    
    return 0;
}