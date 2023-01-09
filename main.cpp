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

int main()
{
    /*//test();
    sf::RenderWindow window(sf::VideoMode(1080, 840), "SFML project");

    Graph newGraph(&window);
    newGraph.SetExpression("tan(exp(sin(tan(abs(tan(sin(x)*2)/2)+2)-1)))");
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
    }*/

    sf::RenderWindow window(sf::VideoMode(640, 480), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Hello, world!");
        ImGui::Button("Look at this pretty button");
        ImGui::End();

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}