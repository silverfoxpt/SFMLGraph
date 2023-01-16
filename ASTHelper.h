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
            tex.setPosition(tex.getPosition().x, tex.getPosition().y - (tex.getGlobalBounds().top - tex.getPosition().y));
            this->text = tex;
        }

        textInfoTrack() {} //empty constructor

        //methods
        //getters
        float getPositionX() { return text.getPosition().x; }
        float getPositionY() { return text.getPosition().y; }

        float getWidth() { return text.getGlobalBounds().width; }
        float getHeight() { return text.getGlobalBounds().height; }

        float getBottomX() { return text.getGlobalBounds().left + getWidth(); }
        float getBottomY() { return text.getGlobalBounds().top + getHeight(); }
        
        //setters
        void SetPosition(float x, float y) {
            text.setPosition(sf::Vector2f(x, y));
        }

        void moveX(float x) {
            text.setPosition(text.getPosition().x + x, text.getPosition().y);
        }

        void moveY(float x) {
            text.setPosition(text.getPosition().x, text.getPosition().y + x);
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