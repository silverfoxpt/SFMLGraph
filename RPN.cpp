#include "RPN.h"

std::vector<std::string> RPN::infixToRPN(std::string inp, int xVal)  {
    std::string alpha = "qwertyuiopasdfghjklzxcvbnm";
    std::string num = "1234567890";
    std::map<char, int> precedence = {
        {'^',3},
        {'*',2},
        {'/',2},
        {'+',1},
        {'-',1},
    };

    std::string xPress = std::to_string(xVal);

    //conversion
    std::vector<std::string> RPN;
    std::stack<char> operators;

    std::string curNumStr = "";

    for (int i = 0; i < inp.size(); i++) {
        char c = inp[i];
        if (c == 'x') {
            RPN.push_back(xPress);
        } 
        
        //not an operator
        else if (precedence.find(c) == precedence.end() || c == '(' || c == ')') { 
            //right bracket
            if (c == '(') { 
                if (!curNumStr.empty()) {
                    RPN.push_back(curNumStr); curNumStr = "";
                }
                operators.push(c);
            } 
            
            //left bracket
            else if (c == ')') { 
                if (!curNumStr.empty()) {
                    RPN.push_back(curNumStr); curNumStr = "";
                }
                while(operators.size() > 0 && operators.top() != '(') {
                    RPN.push_back(std::string(1, operators.top())); operators.pop();
                }
                operators.pop();
            } 
            
            //number
            else {
                curNumStr += c; 
            }
            
        } 
        
        //operator
        else { 
            if (!curNumStr.empty()) {
                RPN.push_back(curNumStr); curNumStr = "";
            }

            //While there is an operator O2 at the top of the operator stack with greater or equal precedence than O1, 
            //pop O2 from the operator stack and push it on the output.
            int pred = precedence[c];
            while(operators.size() > 0) {
                int otherPred = precedence[operators.top()];

                if (otherPred >= pred) {
                    RPN.push_back(std::string(1, operators.top()));
                    char cur = operators.top(); operators.pop();                    
                } 
                else {break;}
            }

            //Push O1 on the operator stack.
            operators.push(c);
        }
    }

    //curNumStr != empty -> push
    if (!curNumStr.empty()) {RPN.push_back(curNumStr);}

    //operator not empty -> push
    while(operators.size() > 0) {
        RPN.push_back(std::string(1, operators.top())); operators.pop();
    }
    return RPN;
}