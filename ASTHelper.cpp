#include "ASTHelper.h"

#pragma region helper
ASTHelper::ASTHelper() {

};

void ASTHelper::SetFont(sf::Font &font) {
    this->textFont = font;
}

void ASTHelper::SetFontSize(int fs) {
    this->fontSize = fs;
}

void ASTHelper::CreateBuffer(int width, int height) {
    this->myTex.create(width, height);
}

void ASTHelper::ClearBuffer() {
    this->myTex.clear(sf::Color(255, 255, 255, 0)); //blank
}

void ASTHelper::DisplayBuffer() {
    this->myTex.display();
}
#pragma endregion

#pragma region textManipulation
void ASTHelper::DrawBufferToWindow(sf::RenderWindow &window) {
    sf::Sprite sp(this->myTex.getTexture());
    window.draw(sp);
}

void ASTHelper::RenderToBuffer(std::vector<textInfoTrack> &chars) {
    int maxWidth = 0;
    int maxHeight = 0;
    for (auto &text: chars) {
        maxWidth    = std::max((float) maxWidth, text.getBottomX());
        maxHeight   = std::max((float) maxHeight, text.getBottomY());
    }

    this->CreateBuffer(200, 200);
    this->ClearBuffer();
    for (auto &text: chars) {
        this->myTex.draw(text.text);
    }
    this->DisplayBuffer();
}

std::vector<textInfoTrack>  ASTHelper::GetTextFromDefaultChar(char c) {
    std::vector<textInfoTrack> ret;

    sf::Text tmp;
    tmp.setFillColor(sf::Color::White);
    tmp.setString(std::string(1, c));
    tmp.setFont(this->textFont);
    tmp.setCharacterSize(this->fontSize);

    textInfoTrack tex(tmp);
    ret.push_back(tex);

    return ret;
}   

std::vector<textInfoTrack> ASTHelper::GetTextFromDefaultString(std::string s) {
    std::vector<textInfoTrack> ret;

    sf::Text tmp;
    tmp.setFillColor(sf::Color::White);
    tmp.setFont(this->textFont);
    tmp.setString(s);    
    tmp.setCharacterSize(this->fontSize);

    textInfoTrack tex(tmp);
    ret.push_back(tex);

    return ret;
}

std::vector<textInfoTrack> ASTHelper::MergeTwoTextsToRight(std::vector<textInfoTrack> &main, std::vector<textInfoTrack> &sub) {
    int mainHeight = 0, mainWidth = 0;
    int subHeight = 0, subWidth = 0;

    for (auto &tex: main) {
        mainWidth = std::max(mainWidth, (int) tex.getBottomX());
        mainHeight = std::max(mainHeight, (int) tex.getBottomY());
    }

    for (auto &tex: sub) {
        subWidth = std::max(subWidth, (int) tex.getBottomX());
        subHeight = std::max(subHeight, (int) tex.getBottomY());
    }

    //start moving shit around
    //move all of sub to right of main
    for (auto &tex: sub) {
        tex.moveX(mainWidth);
    }

    //move main/sub up or down depends on if main/sub is higher/lower than the other
    if (mainHeight > subHeight) {
        int newY = mainHeight/2.0 - subHeight/2.0;
        for (auto &tex: sub) {
            tex.moveY(newY);
        }
    }

    //merge
    std::vector<textInfoTrack> me;
    for (auto &tex: main) {
        me.push_back(tex);
    }

    for (auto &tex: sub) {
        me.push_back(tex);
    }
    return me;
}
#pragma endregion

