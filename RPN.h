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
        static std::vector<std::string> infixToRPN(std::string inp, int xVal);
        static float RPNToValue(std::vector<std::string> &rpn);

    private:
        static std::map<char, int> precedence;
        static std::string alpha;
        static std::string num;
};

#endif