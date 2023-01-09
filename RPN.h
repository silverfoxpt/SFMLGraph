#ifndef RPN_CPP
#define RPN_CPP

#include <string>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <cmath>

class RPN {
    public:
        static std::vector<std::string> infixToRPN(std::string inp);
        static float RPNToValue(std::vector<std::string> &rpn, float xVal);

    private:
        static std::map<char, int> precedence;
        static std::map<std::string, char> opConvert;
        static std::string alpha;
        static std::string num;
};

#endif