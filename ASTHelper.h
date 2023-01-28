#ifndef H_ASTHELPER
#define H_ASTHELPER

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>

enum specialTex {
    LeftBrac = '(',
    RightBrac = ')'
};

class textSizeHelper {
    public:
        static void GetReasonableSize(sf::Text &tex, float requiredHeight) {
            float currentHeight = tex.getGlobalBounds().height;
            int curSize = tex.getCharacterSize();

            if (currentHeight < requiredHeight) { //increased
                while(currentHeight < requiredHeight) {
                    curSize++;
                    tex.setCharacterSize(curSize);
                    currentHeight = tex.getGlobalBounds().height;
                }
            } else { //decreased
                while(currentHeight > requiredHeight) {
                    curSize--;
                    tex.setCharacterSize(curSize);
                    currentHeight = tex.getGlobalBounds().height;
                }
            }
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

        void SetOrigin(float x, float y) {
            if (useText)    {text.setOrigin(sf::Vector2f(x, y));}
            else            {shape.setOrigin(sf::Vector2f(x, y));}
        }

        void SetHardOrigin() {
            if (useText)    {SetOrigin(text.getLocalBounds().left, text.getLocalBounds().top); }
            else            {SetOrigin(shape.getLocalBounds().left, shape.getLocalBounds().top); }
        }

        //others
        void moveX(float x) {
            if (useText)    {text.setPosition(text.getPosition().x + x, text.getPosition().y);}
            else            {shape.setPosition(shape.getPosition().x + x, shape.getPosition().y);}
        }

        void moveY(float x) {
            if (useText)    {text.setPosition(text.getPosition().x, text.getPosition().y + x);}
            else            {shape.setPosition(shape.getPosition().x, shape.getPosition().y + x);}
        }

        void ScaleUp(float sc) { 
            if (useText) {
                float requiredHeight = getHeight() * sc;
                textSizeHelper::GetReasonableSize(this->text, requiredHeight); 
            }
            else {
                shape.scale(sf::Vector2f(sc, sc));
            }
        }
};

class textInfoString {
    public:
        std::vector<textInfoTrack> tex;
        int prevPriority;
        char prevOp;

        textInfoString(std::vector<textInfoTrack> text, int pre, char prevOp) {
            this->tex = text;
            this->prevPriority = pre;
            this->prevOp = prevOp;
        }

        textInfoString() {
            this->prevPriority = 20000;
            this->prevOp = '?';
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

        int getTopX() {
            int x = 1e9;
            for (auto &te: tex) {
                x = std::min((float)x, te.getPositionX());
            }
            return x;
        }

        int getTopY() {
            int y = 1e9;
            for (auto &te: tex) {
                y = std::min((float) y, te.getPositionY());
            }
            return y;
        }

        //setters
        void setPriority(int x) {
            this->prevPriority = x;
        }

        void setPrevOp(char c) {
            this->prevOp = c;
        }

        //others
        void scale(float sc) {
            int oriX = getTopX(), oriY = getTopY();
            for (auto &t: tex) {
                //calc new pos
                sf::Vector2f texOrigin((oriX - t.getPositionX())*(1 - sc), (oriY - t.getPositionY())*(1-sc));                

                //scale
                t.ScaleUp(sc);
                t.SetHardOrigin();

                //move
                t.moveX(texOrigin.x);
                t.moveY(texOrigin.y);
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
        sf::RenderTexture addonTex;

        void SetFont(sf::Font &f);
        void SetFontSize(int s);

        textInfoString GetTextFromDefaultString(sf::String s, bool applyAsRect);
        textInfoString GetVerticalSlash(int width, int height);

        void MergeTwoTextsToRight(textInfoString &first, textInfoString &second, 
            float scaleMain, float scaleSub);
        void MergeTwoTextsToDown(textInfoString &first, textInfoString &second, 
            float scaleMain, float scaleSub);
        void MergeTwoTextsToUpLeft(textInfoString &first, textInfoString &second, float percentUp,
            float scaleMain, float scaleSub);

        void CreateBuffer(int width, int height); //destroy and refresh
        void ClearBuffer();
        void RenderToBuffer(std::vector<textInfoTrack> &chars);
        void DisplayBuffer();
        void DrawBufferToWindow(sf::RenderWindow &window);

    private:
        sf::Texture texArray[256];
};

#endif