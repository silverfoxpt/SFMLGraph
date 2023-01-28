#include "ASTHelper.h"

#pragma region helper
ASTHelper::ASTHelper() {

}

void ASTHelper::SetFont(sf::Font &font) {
    this->textFont = font;

    //auto& texture = const_cast<sf::Texture&>(font.getTexture(30));   
    //texture.setSmooth(false);
}

void ASTHelper::SetFontSize(int fs) {
    this->fontSize = fs;
}

void ASTHelper::CreateBuffer(int width, int height) {
    this->myTex.create(width, height);
    this->myTex.setSmooth(true);
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
        //text.Debug();

        maxWidth    = std::max((float) maxWidth, text.getBottomX());
        maxHeight   = std::max((float) maxHeight, text.getBottomY());
    }

    this->CreateBuffer(maxWidth, maxHeight);
    this->ClearBuffer();
    for (auto &text: chars) {
        if (text.useText)   this->myTex.draw(text.text);
        else                this->myTex.draw(text.shape);
    }
    this->DisplayBuffer();
}
#pragma endregion

#pragma region textManipulation
textInfoString ASTHelper::GetTextFromDefaultString(sf::String s, bool applyAsRect = false) {
    std::vector<textInfoTrack> ret;

    sf::Text tmp;
    tmp.setFillColor(sf::Color::White);
    tmp.setFont(this->textFont);
    tmp.setCharacterSize(this->fontSize);
    tmp.setString(s);

    //VERY IMPORTANT LINE :) Put text origin back to its true bound: https://en.sfml-dev.org/forums/index.php?topic=20284.0
    tmp.setOrigin(sf::Vector2f(tmp.getGlobalBounds().left, tmp.getGlobalBounds().top)); 

    if (!applyAsRect) {
        
    }
    /*//PRECAUTION: USE IN INITIALIZATION FUNCTIONS ONLY. 
    //CREATING sf::RenderTexture is every frame is stupid, costly, and crashing (literally)
    else { 
        int width = tmp.getGlobalBounds().width;
        int height = tmp.getGlobalBounds().height;

        this->addonTex.create(width, height);
        this->addonTex.setSmooth(true);
        this->addonTex.clear(sf::Color(0, 0, 0, 255));
        this->addonTex.draw(tmp);
        this->addonTex.display();

        sf::RectangleShape shape(sf::Vector2f(width, height));
        //shape.setFillColor(sf::Color(0, 0, 0, 255));
        
        sf::Texture newTex = this->addonTex.getTexture();
        this->texArray[s[0]] = newTex;

        shape.setTexture(&texArray[s[0]]);

        //return
        textInfoTrack tex(shape);
        ret.push_back(tex);
    }*/

    textInfoTrack tex(tmp);
    ret.push_back(tex);

    textInfoString inf(ret, 20000, '?'); //priority will change anyway
    return inf;
}

textInfoString ASTHelper::GetVerticalSlash(int width, int height) {
    std::vector<textInfoTrack> ret;

    sf::RectangleShape tmp(sf::Vector2f(width, height));
    tmp.setFillColor(sf::Color::White);

    textInfoTrack tex(tmp);
    ret.push_back(tex);

    textInfoString inf(ret, 10000, '?');

    return inf;
}

void ASTHelper::MergeTwoTextsToRight(textInfoString &main, textInfoString &sub,
    float scaleMain = 1.0, float scaleSub = 1.0) {
    //scale shit up
    main.scale(scaleMain);
    sub.scale(scaleSub);

    //calculate heights & widths
    int mainHeight  = main.getTotalHeight(),    mainWidth   = main.getTotalWidth();
    int subHeight   = sub.getTotalHeight()/*,     subWidth    = sub.getTotalWidth()*/;

    //start moving shit around
    //move all of sub to right of main
    sub.moveX(mainWidth + 4); //hardcode

    //move main/sub up or down depends on if main/sub is higher/lower than the other
    if (mainHeight > subHeight) {
        int newY = mainHeight/2.0 - subHeight/2.0;
        sub.moveY(newY);
    }
    else if (subHeight > mainHeight) {
        int newY = subHeight/2.0 - mainHeight/2.0;
        main.moveY(newY);
    }

    //merge 2 file
    main.merge(sub);
}

void ASTHelper::MergeTwoTextsToDown(textInfoString &main, textInfoString &sub, 
    float scaleMain = 1.0, float scaleSub = 1.0) {
    //meddle something with scaling here? no need yet... though
    main.scale(scaleMain);
    sub.scale(scaleSub);

    //calculate heights & widths
    int mainHeight = main.getTotalHeight(), mainWidth = main.getTotalWidth();
    int /*subHeight = sub.getTotalHeight(),*/ subWidth = sub.getTotalWidth();

    //start moving shit around
    //move all of sub to bottom of main
    sub.moveY(mainHeight + 6); //hardcode

    //move main/sub left/right depends on if main/sub is wider/shorter than the other
    if (mainWidth > subWidth) {
        int newX = mainWidth/2.0 - subWidth/2.0;
        sub.moveX(newX);
    }
    else if (subWidth > mainWidth) {
        int newX = subWidth/2.0 - mainWidth/2.0;
        main.moveX(newX);
    }

    //merge 2 file
    main.merge(sub);
}

//used for exponents
void ASTHelper::MergeTwoTextsToUpLeft(textInfoString &main, textInfoString &sub, float percentDown,
    float scaleMain = 1.0, float scaleSub = 1.0) {
    //scale shit up
    main.scale(scaleMain);
    sub.scale(scaleSub);

    //calculate heights & widths
    int /*mainHeight = main.getTotalHeight(),*/ mainWidth = main.getTotalWidth();
    int subHeight = sub.getTotalHeight()/*, subWidth = sub.getTotalWidth()*/;

    //start moving shit around
    //move all of sub to right of main
    sub.moveX(mainWidth + 4); //hardcode

    //move the base down
    float heightDown = subHeight * percentDown;
    main.moveY((int) heightDown);    

    //merge 2 file
    main.merge(sub);
}
#pragma endregion

