#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

#include "graph.h"
#include "RPN.h"
#include "ASTHelper.h"
#include "TEST.h"

#include "IMGui Stuffs/imgui.h"
#include "IMGui Stuffs/imgui-SFML.h"

#pragma region variables
sf::RenderWindow window(sf::VideoMode(1440, 720), "SFML project");
Graph newGraph;
ASTHelper myASTHelper;

char expressBuffer[64] = "x"; 

float graphSpacing = 0.01;
float pixelEqui = 30;
float lineSize = 3;

float bufferX = 150;
float bufferY = 150;
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
    for (int i = 0; i < (int) a.size(); i++) {
        std::cout << a[i] << " ";
    }
    std::cout << '\n';

    //test 2;
    std::cout << RPN::RPNToValue(a, 1.0);
}

void TestInDrawLoop() {
    /*
    myASTHelper.SetFontSize(60);
    std::vector<textInfoTrack> char1 = myASTHelper.GetTextFromDefaultString("9");
    myASTHelper.SetFontSize(30);
    std::vector<textInfoTrack> char2 = myASTHelper.GetTextFromDefaultString("x-2+3sin(x)");
    std::vector<textInfoTrack> merge = myASTHelper.MergeTwoTextsToRight(char1, char2);
    myASTHelper.RenderToBuffer(merge);

    myASTHelper.DrawBufferToWindow(window);*/

    //test2
    //std::vector<std::string> rp = RPN::infixToRPN("2*(x-5)");
    //std::vector<std::string> rp = RPN::infixToRPN("(x-2*5)^(x-3)/((x+3)/(x-3+9/x+5/x/x-7/3/x/x-8/9/6/(x+2)))");
    //std::vector<std::string> rp = RPN::infixToRPN("((x+2^2/7x)/5)^(3*x-5)^(2/5/2x^2+2x)");    
    //std::vector<std::string> rp = RPN::infixToRPN("(2/5/x^3+2x^2+x+1)^(2/x-3x+2x^2/5)^(2/5(x-47x^2)x^3/x)");
    //std::vector<std::string> rp = RPN::infixToRPN("2x2(x-1)2(x-1)(x+1)x(x+1/2)/2");
    std::vector<std::string> rp = RPN::infixToRPN("xsin(x/2cos(tan(x)))cos(x+tan(3x^2x))tan(2x/(x^2+2x+1))");

    textInfoString fin = RPN::RPNToDisplay(rp, myASTHelper);

    myASTHelper.RenderToBuffer(fin.tex);
    myASTHelper.DrawBufferToWindow(window);
}
#pragma endregion

#pragma region Helper
void DrawGraph() {
    sf::Texture tex = newGraph.myBuffer->getTexture();
    sf::Sprite tmpSprite(tex);
    tmpSprite.setPosition(bufferX, bufferY);

    window.draw(tmpSprite);
}

void GraphManipulation() {
    if (!myFont.loadFromFile("./res/font/latinmodern-math.otf")) {
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
    ImGui::Text(" ");

    //graph manipulation
    ImGui::SliderFloat("Spacing", &graphSpacing, 0.001, 2.0, "%.3f");
    ImGui::SliderFloat("Pixel equivalent", &pixelEqui, 1, 200, "%.3f");
    ImGui::SliderFloat("Graph line thickness", &lineSize, 0.1, 10, "%.3f");
    ImGui::Text(" ");

    ImGui::Checkbox("First quarter", &graphQuarters[0]);
    ImGui::Checkbox("Second quarter", &graphQuarters[1]);
    ImGui::Checkbox("Third quarter", &graphQuarters[2]);
    ImGui::Checkbox("Fourth quarter", &graphQuarters[3]);
    ImGui::Text(" ");

    //colors
    ImGui::ColorEdit4("Background color", backgroundCol);
    ImGui::ColorEdit4("Line graph color", lineGraphCol);
    ImGui::Text(" ");

    //buffers
    ImGui::Text("Buffer manipulation");
    ImGui::InputFloat("Buffer position X", &bufferX, 0.5, 0.5);
    ImGui::InputFloat("Buffer position Y", &bufferY, 0.5, 0.5);
    ImGui::InputInt("Buffer width", &bufferWidth);
    ImGui::InputInt("Buffer height", &bufferHeight);
    ImGui::SliderFloat("Origin X", &originX, 0.0, 1.0, "%.3f");
    ImGui::SliderFloat("Origin Y", &originY, 0.0, 1.0, "%.3f");
    ImGui::Text(" ");

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
    //INITIALIZATION
    //TestInitialize();
    ImGui::SFML::Init(window);

    newGraph = Graph(bufferWidth, bufferHeight, originX, originY, pixelEqui);
    GraphManipulation();

    myASTHelper.SetFont(myFont);
    myASTHelper.SetFontSize(30);
    RPN::RPNInitialize(myASTHelper);

    //draw loop
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

        SFMLUpdate();

        DrawGraph();
        TestInDrawLoop();

        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    
    return 0;
}