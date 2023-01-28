#ifndef RPN_CPP
#define RPN_CPP

#include <string>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <cmath>

#include "ASTHelper.h"
#include "StringHelper.h"

class RPN {
    public:
        static void RPNInitialize(ASTHelper &helper);

        static std::vector<std::string> infixToRPN(std::string inp);
        static float RPNToValue(std::vector<std::string> &rpn, float xVal);
        static textInfoString RPNToDisplay(std::vector<std::string> &rpn, ASTHelper &myASTHelper);

        static bool isTrigFunction(char c);

    private:
        static std::map<char, int> precedence;
        static std::map<std::string, char> opConvert;
        static std::string alpha;
        static std::string num;

        static std::map<char, sf::String> opDisplay;
        static std::map<char, textInfoString> opDefault;

        static std::string MultiplicationInfixToRPNAdder(std::string inp);
};

#endif