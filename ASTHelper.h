#ifndef H_ASTHELPER
#define H_ASTHELPER

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <string>

class textInfoTrack {
    public:
        sf::Text text;

        //constructors
        textInfoTrack(sf::Text tex) {
            this->text = tex;
        }

        textInfoTrack() {} //empty constructor

        //methods
        //getters
        float getPositionX() { return text.getPosition().x; }
        float getPositionY() { return text.getPosition().y; }

        float getWidth() { return text.getGlobalBounds().width; }
        float getHeight() { return text.getGlobalBounds().width; }

        float getBottomX() { return getPositionX() + getWidth(); }
        float getBottomY() { return getPositionY() + getHeight(); }
        
        //setters
        void SetPosition(float x, float y) {
            text.setPosition(sf::Vector2f(x, y));
        }

        void ScaleUp(float sc) { text.scale(sf::Vector2f(sc, sc));}
};

class ASTHelper {
    public:
        ASTHelper();

        void SetFont(sf::Font &f);
        void SetFontSize(int s);

        sf::RenderTexture myTex;

        std::vector<textInfoTrack> GetTextFromDefaultChar(char c);
        std::vector<textInfoTrack> GetTextFromDefaultString(std::string s);
        std::vector<textInfoTrack> MergeTwoTextsToRight(std::vector<textInfoTrack> &first, std::vector<textInfoTrack> &second);

        void CreateBuffer(int width, int height); //destroy and refresh
        void ClearBuffer();
        void RenderToBuffer(std::vector<textInfoTrack> &chars);
        void DisplayBuffer();
        void DrawBufferToWindow(sf::RenderWindow &window);

    private:
        sf::Font textFont;
        int fontSize;
};

#endif