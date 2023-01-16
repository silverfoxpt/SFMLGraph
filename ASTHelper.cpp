#include "ASTHelper.h"

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

    this->CreateBuffer(maxWidth, maxHeight);
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
    int maxHeight = 0;
    int maxWidth = 0;
}

