#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "graph.h"
#include "RPN.h"
#include "ASTHelper.h"

#include "IMGui Stuffs/imgui.h"
#include "IMGui Stuffs/imgui-SFML.h"

#pragma region variables
sf::RenderWindow window(sf::VideoMode(1440, 720), "SFML project");
Graph newGraph;
ASTHelper myASTHelper;

char expressBuffer[64] = "sin(tan(x))"; 

float graphSpacing = 0.01;
float pixelEqui = 30;
float lineSize = 3;

float bufferX = 50;
float bufferY = 50;
float originX = 0.2;
float originY = 0.8;

float markerSpacing = 0.08;
float textSpacing = 0.17;
int textSize = 15;

float backgroundCol[4] = {1, 1, 1, 1};
float lineGraphCol[4] = {0, 0, 0, 0.6};

int bufferWidth = 640;
int bufferHeight = 480;

bool graphQuarters[4] = {true, true, true, true};

sf::Font myFont;
#pragma endregion

#pragma region Test
void TestInitialize() {
    //test 1
    std::vector<std::string> a = RPN::infixToRPN("(3+7/7-2/5)*(9+6/(3-7))");
    for (int i = 0; i < a.size(); i++) {
        std::cout << a[i] << " ";
    }
    std::cout << '\n';

    //test 2;
    std::cout << RPN::RPNToValue(a, 1.0);
}

void TestInDrawLoop() {
    myASTHelper.SetFontSize(60);
    std::vector<textInfoTrack> char1 = myASTHelper.GetTextFromDefaultString("9");
    myASTHelper.SetFontSize(30);
    std::vector<textInfoTrack> char2 = myASTHelper.GetTextFromDefaultString("x-2+3sin(x)");
    std::vector<textInfoTrack> merge = myASTHelper.MergeTwoTextsToRight(char1, char2);
    myASTHelper.RenderToBuffer(merge);

    myASTHelper.DrawBufferToWindow(window);
}
#pragma endregion

#pragma region helper
void DrawGraph() {
    sf::Texture tex = newGraph.myBuffer->getTexture();
    sf::Sprite tmpSprite(tex);
    tmpSprite.setPosition(bufferX, bufferY);

    window.draw(tmpSprite);
}

void GraphManipulation() {
    if (!myFont.loadFromFile("./res/font/aparaj.ttf")) {
        std::cout << "Font load failed";
    }
    //setters
    newGraph = Graph(bufferWidth, bufferHeight, originX, originY, pixelEqui);
    newGraph.SetFont(myFont);

    newGraph.SetSpacing(graphSpacing);
    newGraph.SetQuarters(graphQuarters[0], graphQuarters[1], graphQuarters[2], graphQuarters[3]);
    newGraph.SetGraphLineThickness(lineSize);
      
    newGraph.SetBackgroundColor(sf::Color(backgroundCol[0] * 255, backgroundCol[1] * 255, backgroundCol[2] * 255, backgroundCol[3] * 255));
    newGraph.SetLineGraphColor(sf::Color(lineGraphCol[0] * 255, lineGraphCol[1] * 255, lineGraphCol[2] * 255, lineGraphCol[3] * 255));

    newGraph.SetBufferPosition(bufferX, bufferY);
    newGraph.SetExpression(expressBuffer);

    //draw & create
    newGraph.ClearDrawBuffer();

    newGraph.CreateAxis();
    newGraph.CreateMarker(markerSpacing);
    newGraph.CreateSpaceText(textSpacing, textSize);
    newGraph.CreateGraph();    

    newGraph.DisplayDrawBuffer();
    newGraph.Debug();
}

void SFMLUpdate() {
    ImGui::Begin("Input");

    //expression
    ImGui::Text("Graph manipulation");
    ImGui::InputText("Equation", expressBuffer, 64);
    ImGui::Text("");

    //graph manipulation
    ImGui::SliderFloat("Spacing", &graphSpacing, 0.001, 2.0, "%.3f");
    ImGui::SliderFloat("Pixel equivalent", &pixelEqui, 1, 200, "%.3f");
    ImGui::SliderFloat("Graph line thickness", &lineSize, 0.1, 10, "%.3f");
    ImGui::Text("");

    ImGui::Checkbox("First quarter", &graphQuarters[0]);
    ImGui::Checkbox("Second quarter", &graphQuarters[1]);
    ImGui::Checkbox("Third quarter", &graphQuarters[2]);
    ImGui::Checkbox("Fourth quarter", &graphQuarters[3]);
    ImGui::Text("");

    //colors
    ImGui::ColorEdit4("Background color", backgroundCol);
    ImGui::ColorEdit4("Line graph color", lineGraphCol);
    ImGui::Text("");

    //buffers
    ImGui::Text("Buffer manipulation");
    ImGui::InputFloat("Buffer position X", &bufferX, 0.5, 0.5);
    ImGui::InputFloat("Buffer position Y", &bufferY, 0.5, 0.5);
    ImGui::InputInt("Buffer width", &bufferWidth);
    ImGui::InputInt("Buffer height", &bufferHeight);
    ImGui::SliderFloat("Origin X", &originX, 0.0, 1.0, "%.3f");
    ImGui::SliderFloat("Origin Y", &originY, 0.0, 1.0, "%.3f");
    ImGui::Text("");

    //others
    ImGui::InputFloat("Marker spacing", &markerSpacing, 0.01, 0.1);
    ImGui::InputFloat("Text spacing", &textSpacing, 0.01, 1);
    ImGui::InputInt("Text size", &textSize);

    if (ImGui::Button("Submit", sf::Vector2f(100, 30))) {
        GraphManipulation();
    }

    ImGui::End();
}
#pragma endregion

int main()
{
    //TestInitialize();
    ImGui::SFML::Init(window);

    newGraph = Graph(bufferWidth, bufferHeight, originX, originY, pixelEqui);
    GraphManipulation();

    sf::Clock deltaTime;

    myASTHelper.SetFont(myFont);
    myASTHelper.SetFontSize(30);

    while (window.isOpen())
    {
        //std::cout << "FPS: " << 1.0/deltaTime.getElapsedTime().asSeconds() << '\n';

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);    
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, deltaTime.restart());
        window.clear();

        SFMLUpdate();

        DrawGraph();
        TestInDrawLoop();

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    
    return 0;
}