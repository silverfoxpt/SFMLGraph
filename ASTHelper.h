#ifndef H_ASTHELPER
#define H_ASTHELPER

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <string>

class textSizeHelper {
    public:
        float size[401];

        textSizeHelper() {
            
        }

        textSizeHelper(sf::Text &myText) {
            int oldSize = myText.getCharacterSize();
            for (int i = 1; i <= 400; i++) {
                myText.setCharacterSize(i);
                size[i] = myText.getGlobalBounds().height;
            }
            myText.setCharacterSize(oldSize);
        }

        int heightToCharSize(float height) {
            return std::lower_bound(size+1, size+400, height) - size;
        }
};

class textInfoTrack {
    public:
        sf::Text text;
        sf::RectangleShape shape;
        bool useText;

        //textSizeHelper helper;

        //constructors
        textInfoTrack(sf::Text tex) {
            this->text      = tex;
            this->useText   = true;

            //this->helper    = textSizeHelper(this->text);
        }

        textInfoTrack(sf::RectangleShape shape) {
            this->shape     = shape;
            this->useText   = false;
        }

        void Debug() {
            if (text.getString().toAnsiString() == "(") 
                std::cout << text.getGlobalBounds().width 
                    << text.getLocalBounds().width << ";"
                    << getWidth() << " "
                    << getPositionX() << " "
                    << getBottomX()
                    << "\n";
        }

        //methods
        //getters
        float getPositionX() { return ((useText) ? text.getPosition().x : shape.getPosition().x); }
        float getPositionY() { return ((useText) ? text.getPosition().y : shape.getPosition().y); }

        float getWidth()    { return ((useText) ? text.getGlobalBounds().width   : shape.getGlobalBounds().width); }
        float getHeight()   { return ((useText) ? text.getGlobalBounds().height  : shape.getGlobalBounds().height); }

        float getBottomX() { return getPositionX() + getWidth(); }
        float getBottomY() { return getPositionY() + getHeight(); }
        
        //setters
        void SetPosition(float x, float y) {
            if (useText)    {text.setPosition(sf::Vector2f(x, y));}
            else            {shape.setPosition(sf::Vector2f(x, y));}
        }

        void moveX(float x) {
            if (useText)    {text.setPosition(text.getPosition().x + x, text.getPosition().y);}
            else            {shape.setPosition(shape.getPosition().x + x, shape.getPosition().y);}
        }

        void moveY(float x) {
            if (useText)    {text.setPosition(text.getPosition().x, text.getPosition().y + x);}
            else            {shape.setPosition(shape.getPosition().x, shape.getPosition().y + x);}
        }

        void ScaleUp(float sc) { 
            //if (useText)    {text.setCharacterSize(text.getCharacterSize() * sc);}


            if (useText)    {
                text.scale(sf::Vector2f(sc, sc)); 
                //int x = this->helper.heightToCharSize(getHeight() * sc);
                //text.setCharacterSize(x);
            }
            else            {shape.scale(sf::Vector2f(sc, sc));}
        }
};

class textInfoString {
    public:
        std::vector<textInfoTrack> tex;
        int prevPriority;

        textInfoString(std::vector<textInfoTrack> text, int pre) {
            this->tex = text;
            this->prevPriority = pre;
        }

        textInfoString() {

        }

        //methods
        //getters
        int getTotalHeight() {
            int h = 0;
            for (auto &te: tex) {
                h = std::max((float) h, te.getBottomY());
            }
            return h;
        }

        int getTotalWidth() {
            int h = 0;
            for (auto &te: tex) {
                h = std::max((float) h, te.getBottomX());
            }
            return h;
        }

        //setters
        void setPriority(int x) {
            this->prevPriority = x;
        }

        //others
        void scale(float sc) {
            for (auto &t: tex) {
                t.ScaleUp(sc);
            }
        }

        void moveX(float x) {
            for (auto &t: tex) {
                t.moveX(x);
            }
        }

        void moveY(float y) {
            for (auto &t: tex)  {
                t.moveY(y);
            }
        }

        void merge(textInfoString &other) {
            for (auto &t: other.tex) {
                tex.push_back(t);
            }
        }
};

class ASTHelper {
    public:
        ASTHelper();

        sf::Font textFont;
        int fontSize;
        sf::RenderTexture myTex;

        void SetFont(sf::Font &f);
        void SetFontSize(int s);

        textInfoString GetTextFromDefaultString(sf::String s);
        textInfoString MergeTwoTextsToRight(textInfoString &first, textInfoString &second, 
            float scaleMain, float scaleSub);
        textInfoString MergeTwoTextsToDown(textInfoString &first, textInfoString &second, 
            float scaleMain, float scaleSub);
        textInfoString MergeTwoTextsToUpLeft(textInfoString &first, textInfoString &second, float percentUp,
            float scaleMain, float scaleSub);
        
        textInfoString GetVerticalSlash(int width, int height);

        void CreateBuffer(int width, int height); //destroy and refresh
        void ClearBuffer();
        void RenderToBuffer(std::vector<textInfoTrack> &chars);
        void DisplayBuffer();
        void DrawBufferToWindow(sf::RenderWindow &window);

    private:
};

#endif