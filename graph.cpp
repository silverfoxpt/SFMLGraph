#include "graph.h"

void Graph::Debug() {
    //std::cout << this->graphLine.size() << '\n';
}

#pragma region mathStuff
const float radToDeg = 57.295779513;

float dist(float startX, float startY, float endX, float endY) {
    return std::sqrt((startX - endX) * (startX - endX) + (startY - endY) * (startY - endY));
}

float dotProduct(float x1, float y1, float x2, float y2) {
    return x1*x2 + y1*y2;
}

float angleBetweenTwoVector(float x1, float y1, float x2, float y2) {
    return std::acos(
        dotProduct(x1, y1, x2, y2)/
        (dist(x1, y1, 0, 0) * dist(x2, y2, 0, 0)) 
    ) * radToDeg;
}
#pragma endregion

Graph::Graph(int width, int height, sf::RenderWindow *window, 
    float originPercentWidth = 0.5, float originPercentHeight = 0.5,
    float pixelEquivalent = 30) {

    this->myWindow = window;
    this->pixelEquivalent = pixelEquivalent;

    this->windowWidth = width;
    this->windowHeight = height;

    this->myBuffer = new sf::RenderTexture();
    this->myBuffer->create(this->windowWidth, this->windowHeight);
    this->myBuffer->setSmooth(true);

    this->originPercentWidth = originPercentWidth;
    this->originPercentHeight = originPercentHeight;

    this->originX = width * originPercentWidth;
    this->originY = height * originPercentHeight;

    this->rightDescartesBound = (this->windowWidth - this->originX) / this->pixelEquivalent;
    this->leftDescartesBound = (-this->originX) / this->pixelEquivalent;

    this->topDescartesBound = (this->originY) / this->pixelEquivalent;
    this->bottomDescartesBound = -(this->windowWidth - this->originY) / this->pixelEquivalent;

    this->backgroundColor = sf::Color::White;
    this->lineColor = sf::Color::Black;
}

#pragma region lines & axes
void Graph::CreateRectLine(float startX, float startY, float endX, float endY, float lineThickness, sf::Color col = sf::Color::White) {
    std::pair<float, float> toScreen = this->ConvertCoordsToScreen(startX, startY);
    std::pair<float, float> toScreen2 = this->ConvertCoordsToScreen(endX, endY);
    float width = dist(toScreen.first, toScreen.second, toScreen2.first, toScreen2.second);

    sf::RectangleShape line(sf::Vector2f(width, lineThickness));
    line.setOrigin(sf::Vector2f(0, lineThickness/2.0));
    line.setPosition(sf::Vector2f(toScreen.first, toScreen.second));
    line.setFillColor(col);

    //rotation
    float x1 = endX - startX, y1 = endY - startY;
    float angle = angleBetweenTwoVector(x1, y1, 1, 0);
    line.setRotation((endY < startY) ? angle : 360-angle);

    this->myBuffer->draw(line);
}

void Graph::CreateSingleLine(float startX, float startY, float endX, float endY, sf::Color col) {
    std::pair<float, float> p1 = this->ConvertCoordsToScreen(startX, startY);
    std::pair<float, float> p2 = this->ConvertCoordsToScreen(endX, endY);

    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(p1.first, p1.second)),
        sf::Vertex(sf::Vector2f(p2.first, p2.second))
    };
    line[0].color = col;
    line[1].color = col;

    this->myBuffer->draw(line, 2, sf::Lines);
}

void Graph::CreateAxis() {
    //float halfWidth = this->windowWidth/2.0;
    //float halfHeight = this->windowHeight/2.0;

    //this->axes.push_back(this->CreateLine(0, halfHeight, 0, -halfHeight, axisThickness, sf::Color::Red));
    //this->axes.push_back(this->CreateLine(-halfWidth, 0, halfWidth, 0, axisThickness, sf::Color::Red));

    //this->myBuffer->draw(this->CreateLine(0, this->originY, 0, -(this->windowHeight - this->originY), axisThickness, sf::Color::Red));
    //this->myBuffer->draw(this->CreateLine(-this->originX, 0, this->windowWidth - this->originX, 0, axisThickness, sf::Color::Red));
    
    //this->CreateSingleLine(0, this->originY, 0, -(this->windowHeight - this->originY), sf::Color::Red);
    //this->CreateSingleLine(-this->originX, 0, this->windowWidth - this->originX, 0, sf::Color::Red);

    if (this->quarters[0]) {
        this->CreateSingleLine(0, this->originY, 0, 0, sf::Color::Red);
        this->CreateSingleLine(0, 0, this->windowWidth - this->originX, 0, sf::Color::Red);
    } 
    if (this->quarters[1]) {
        this->CreateSingleLine(0, this->originY, 0, 0, sf::Color::Red);
        this->CreateSingleLine(-this->originX, 0, 0, 0, sf::Color::Red);
    }
    if (this->quarters[2]) {
        this->CreateSingleLine(0, 0, 0, -(this->windowHeight - this->originY), sf::Color::Red);
        this->CreateSingleLine(-this->originX, 0, 0, 0, sf::Color::Red);
    }
    if (this->quarters[3]) {
        this->CreateSingleLine(0, 0, 0, -(this->windowHeight - this->originY), sf::Color::Red);
        this->CreateSingleLine(0, 0, this->windowWidth - this->originX, 0, sf::Color::Red);
    }
}

void Graph::CreateMarker(float markerThickness) {
    if (this->quarters[0] || this->quarters[3]) {
        int x = 0;
        while(x <= this->rightDescartesBound) {
            x += 1;
            this->CreateSingleLine(x, markerThickness, x, -markerThickness, sf::Color::Red);  
        } 
    }

    if (this->quarters[0] || this->quarters[1]) {
        int y = 0;
        while (y <= this->topDescartesBound) {
            y += 1;
            this->CreateSingleLine(-markerThickness, y, markerThickness, y, sf::Color::Red);  
        }
    }

    if (this->quarters[1] || this->quarters[2]) {
        int x = 0;
        while(x >= this->leftDescartesBound) {
            x -= 1;
            this->CreateSingleLine(x, markerThickness, x, -markerThickness, sf::Color::Red);  
        } 
    }

    if (this->quarters[2] || this->quarters[3]) {
        int y = 0;
        while (y >= this->bottomDescartesBound) {
            y -= 1;
            this->CreateSingleLine(-markerThickness, y, markerThickness, y, sf::Color::Red);  
        }
    }
}
#pragma endregion

#pragma region pointConversion
std::pair<float, float> Graph::ConvertCoordsToDescartes(float x, float y) {
    std::pair<float, float> newPair;
    newPair.first = (x - this->originX) / this->pixelEquivalent;
    newPair.second = -(y - this->originY) / this->pixelEquivalent;

    return newPair;
}

std::pair<float, float> Graph::ConvertCoordsToScreen(float x, float y) {
    x *= this->pixelEquivalent;
    y *= this->pixelEquivalent;

    std::pair<float, float> newPair;
    newPair.first = x + this->originX;
    newPair.second = -y + this->originY;

    return newPair;
}
#pragma endregion

#pragma region point
void Graph::CreatePoint(float x, float y, float rad) {
    std::pair<float, float> coords = ConvertCoordsToScreen(x, y);

    sf::CircleShape point = sf::CircleShape(rad);
    point.setOrigin(sf::Vector2f(rad, rad));
    point.setFillColor(sf::Color::Red);
    point.setPosition(sf::Vector2f(coords.first, coords.second));

    //this->points.push_back(point);
    this->myBuffer->draw(point);
}
#pragma endregion

#pragma region graph
void Graph::CreateGraph() {
    float x = this->leftDescartesBound;
    std::vector<std::pair<float, float>> po;

    while(x <= this->rightDescartesBound) {
        float plotX = x;
        float plotY = this->CalculateGraph(x);
        x += this->spacing;

        bool rightQuart = false;
        for (int i = 0; i < 4; i++) {
            if (!this->quarters[i]) {continue;}
            rightQuart = rightQuart || this->QuarterCheck(plotX, plotY, i);
        }
        if (!rightQuart) {continue;}

        std::pair<float, float> newPoint(plotX, plotY);
        po.push_back(newPoint);
    }

    //create lines
    int n = po.size();
    for (int i = 0; i < n-1; i++) {
        float x1 = po[i].first, y1 = po[i].second;
        float x2 = po[i+1].first, y2 = po[i+1].second;

        //this->CreateSingleLine(x1, y1, x2, y2, this->lineColor); 
        this->CreateRectLine(x1, y1, x2, y2, 3, this->lineColor);
    }
    this->myBuffer->display();
}

void Graph::DrawGraph() {
    const sf::Texture& texture = this->myBuffer->getTexture();
    sf::Sprite tmpSprite(texture);

    tmpSprite.setPosition(this->bufferPos.first, this->bufferPos.second);

    this->myWindow->draw(tmpSprite);
}

float Graph::CalculateGraph(float x) {
    return RPN::RPNToValue(this->myRPN, x);
}
#pragma endregion

#pragma region buffer
void Graph::ClearDrawBuffer() {
    this->myBuffer->clear(this->backgroundColor);
}

void Graph::DisplayDrawBuffer() {
    this->myBuffer->display();
}
#pragma endregion

#pragma region setters
void Graph::SetQuarters(bool first, bool sec, bool third, bool fourth) {
    this->quarters[0] = first;
    this->quarters[1] = sec;
    this->quarters[2] = third;
    this->quarters[3] = fourth;
}

void Graph::SetBackgroundColor(sf::Color col) {
    this->backgroundColor = col;
}

void Graph::SetLineGraphColor(sf::Color col) {
    this->lineColor = col;
}

void Graph::SetBufferPosition(float x, float y) {
    this->bufferPos = std::pair<float, float>(x, y);
}
void Graph::SetPixelEquivalent(float eqi) {
    this->pixelEquivalent = eqi;
}

void Graph::SetSpacing(float spacing) {
    this->spacing = spacing;
}

void Graph::SetExpression(std::string ex) {
    this->expression = ex;
    this->myRPN = RPN::infixToRPN(ex);
}
#pragma endregion

#pragma region quarters
bool Graph::QuarterCheck(float x, float y, int quart) {
    if (quart == 0) {
        return (x >= 0) & (y >= 0);
    } else if (quart == 1) {
        return (x <= 0) & (y >= 0);
    } else if (quart == 2) {
        return (x <= 0) & (y <= 0);
    } else if (quart == 3) {
        return (x >= 0) & (y <= 0);
    }
}
#pragma endregion

#pragma region spaceText
void Graph::SetFont(sf::Font &font) {
    this->spaceFont = font;
}

void Graph::CreateText(float x, float y, std::string text, float size, std::pair<float, float> spacing, std::pair<float, float> originPer) {
    std::pair<float, float> screenCoords = this->ConvertCoordsToScreen(x + spacing.first, y + spacing.second);

    sf::Text tex; 
    tex.setFont(this->spaceFont);
    tex.setCharacterSize(size);
    tex.setString(text);
    tex.setFillColor(sf::Color::Red);

    int a = tex.getLocalBounds().width * originPer.first;
    int b = tex.getLocalBounds().height * originPer.second;
    tex.setOrigin(sf::Vector2f(a, b));

    tex.setPosition((int) screenCoords.first, (int) screenCoords.second);

    this->myBuffer->draw(tex);
}

void Graph::CreateSpaceText(float space, float size) {
    if (this->quarters[0] || this->quarters[3]) {
        int x = 0;
        int c = 0;

        while(x <= this->rightDescartesBound) {
            x += 1; 
            c++;
            
            this->CreateText(x, 0, std::to_string(c), size, std::pair<float, float>(0, -space), std::pair<float, float>(0.5, 0));
        } 
    }

    if (this->quarters[0] || this->quarters[1]) {
        int y = 0;
        int c = 0;

        while (y <= this->topDescartesBound) {
            y += 1; 
            c++;
            
            this->CreateText(0, y, std::to_string(c), size, std::pair<float, float>(-space, 0), std::pair<float, float>(1, 1.5));
        }
    }

    if (this->quarters[1] || this->quarters[2]) {
        int x = 0;
        int c = 0;

        while(x >= this->leftDescartesBound) {
            x -= 1; 
            c--;
            
            this->CreateText(x, 0, std::to_string(c), size, std::pair<float, float>(0, -space), std::pair<float, float>(0.5, 0));
        } 
    }

    if (this->quarters[2] || this->quarters[3]) {
        int y = 0;
        int c = 0;

        while (y >= this->bottomDescartesBound) {
            y -= 1; 
            c--;
            
            this->CreateText(0, y, std::to_string(c), size, std::pair<float, float>(-space, 0), std::pair<float, float>(1, 1.5));
            
        }
    }
}
#pragma endregion