#include "RPN.h"

std::string ReplaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

#pragma region vars
std::map<char, int> RPN::precedence = {
    {'^',3},
    {'*',2},
    {'/',2},
    {'+',1},
    {'-',1},

    {'!', 4},
    {'@', 4},
    {'#', 4},
    {'$', 4},

    {'|', 4},

    {'%', 4},
    {'&', 4},

    {'{', 4},
    {'}', 4},
    {'[', 4},
    {']', 4},
};

//commented operations are handled seperately
std::map<char, sf::String> RPN::opDisplay = {
    //optional
    {'(', "("},
    {')', ")"},

    //{'^',3},
    {'*', sf::String(L"⋅")},
    //{'/', ""},
    {'+', "+"},
    {'-', "-"},

    {'!', "sin"},
    {'@', "cos"},
    {'#', "tan"},
    {'$', "cot"},

    //{"abs", '|'},

    //{"sqrt", '%'},
    //{"cbrt", '&'},

    {'{', "exp"},
    {'}', "ln"},
    {'[', "log10"},
    {']', "log2"}
};
std::map<char, textInfoString> RPN::opDefault;

std::string RPN::alpha = "qwertyuiopasdfghjklzxcvbnm";
std::string RPN::num = "1234567890.";
std::map<std::string, char> RPN::opConvert = {
    {"sin", '!'},
    {"cos", '@'},
    {"tan", '#'},
    {"cot", '$'},

    {"abs", '|'},

    {"sqrt", '%'},
    {"cbrt", '&'},

    {"exp", '{'},
    {"ln", '}'},
    {"log10", '['},
    {"log2", ']'},
};
#pragma endregion

void RPN::RPNInitialize(ASTHelper &helper) {
    for (std::map<char, sf::String>::iterator it = opDisplay.begin(); it != opDisplay.end(); it++) {
        char c = it->first;
        sf::String s = it->second;

        textInfoString tmp = helper.GetTextFromDefaultString(s);
        tmp.setPriority(-1);

        opDefault[c] = tmp;
    }
}

std::vector<std::string> RPN::infixToRPN(std::string inp)  {
    //replace uncommon functions
    for (std::map<std::string, char>::iterator it = opConvert.begin(); it != opConvert.end(); it++) {
        inp = ReplaceString(inp, it->first, std::string(1, it->second));
    }

    //conversion
    std::vector<std::string> RPN;
    std::stack<char> operators;

    std::string curNumStr = "";

    for (int i = 0; i < (int) inp.size(); i++) {
        char c = inp[i];
        if (c == 'x') {
            RPN.push_back("x");
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
                    operators.pop();                    
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

float RPN::RPNToValue(std::vector<std::string> &rp, float xVal) {
    //evaluate
    std::stack<float> val;
    for (const std::string &x: rp) {
        if (x[0] == 'x') {
            val.push(xVal);
        }

        else if (num.find(x[0]) != std::string::npos) { //is a number
            val.push(std::stof(x));
        } 
        
        else { //operator
            switch(x[0]) {
                case '+': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y+x);
                    break;
                }

                case '-': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y-x);
                    break;
                }

                case '*': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y*x);
                    break;
                }

                case '/': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(y/x);
                    break;
                }

                case '^': {
                    float x = val.top(); val.pop();
                    float y = val.top(); val.pop();
                    val.push(pow(y, x));
                    break;
                }

                //TRIG FUNCTIONS
                case '!': {
                    float x = val.top(); val.pop();
                    val.push(std::sin(x));
                    break;
                }

                case '@': {
                    float x = val.top(); val.pop();
                    val.push(std::cos(x));
                    break;
                }

                case '#': {
                    float x = val.top(); val.pop();
                    val.push(std::tan(x));
                    break;
                }

                case '$': {
                    float x = val.top(); val.pop();
                    val.push(1/std::tan(x));
                    break;
                }

                //abs
                case '|': {
                    float x = val.top(); val.pop();
                    val.push(std::abs(x));
                    break;
                }

                //root
                case '%': {
                    float x = val.top(); val.pop();
                    val.push(std::sqrt(x));
                    break;
                }

                case '&': {
                    float x = val.top(); val.pop();
                    val.push(std::cbrt(x));
                    break;
                }

                //log
                case '{': {
                    float x = val.top(); val.pop();
                    val.push(std::exp(x));
                    break;
                }

                case '}': {
                    float x = val.top(); val.pop();
                    val.push(std::log(x));
                    break;
                }

                case '[': {
                    float x = val.top(); val.pop();
                    val.push(std::log10(x));
                    break;
                }

                case ']': {
                    float x = val.top(); val.pop();
                    val.push(std::log2(x));
                    break;
                }

                default: {
                    std::cout << "Unknown" << '\n'; break;
                }
            }
        }
    }
    return val.top();
}

textInfoString RPN::RPNToDisplay(std::vector<std::string> &rpn, ASTHelper &myASTHelper) {
    //evaluate
    std::stack<textInfoString> val;

    for (const std::string &x: rpn) {
        if (x[0] == 'x' || num.find(x[0]) != std::string::npos) { //is a number or x
            textInfoString text = myASTHelper.GetTextFromDefaultString(x);
            text.setPriority(999); //highest priority

            val.push(text);
        } 
        
        else { //operator
            switch(x[0]) {
                case '+': case '-': case '*': {
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();
                    
                    textInfoString sign = opDefault[x[0]];

                    if (precedence[x[0]] >= leftVal.prevPriority) {
                        textInfoString rightBracket = opDefault[')'];
                        textInfoString leftBracket = opDefault['('];

                        float inc = leftVal.getTotalHeight() / rightBracket.getTotalHeight();
                        textInfoString merge = myASTHelper.MergeTwoTextsToRight(leftBracket, leftVal, inc, 1.0);
                        textInfoString merge2 = myASTHelper.MergeTwoTextsToRight(merge, rightBracket, 1.0, inc);

                        leftVal = merge2;
                    }

                    if (precedence[x[0]] >= rightVal.prevPriority) {
                        textInfoString rightBracket = opDefault[')'];
                        textInfoString leftBracket = opDefault['('];

                        float inc = rightVal.getTotalHeight() / rightBracket.getTotalHeight();
                        textInfoString merge = myASTHelper.MergeTwoTextsToRight(leftBracket, rightVal, inc, 1.0);
                        textInfoString merge2 = myASTHelper.MergeTwoTextsToRight(merge, rightBracket, 1.0, inc);

                        rightVal = merge2;
                    }

                    textInfoString merge = myASTHelper.MergeTwoTextsToRight(rightVal, sign, 1.0, 1.0);
                    textInfoString merge2 = myASTHelper.MergeTwoTextsToRight(merge, leftVal, 1.0, 1.0);

                    merge2.setPriority(precedence[x[0]]);
                    val.push(merge2);

                    break;
                }

                case '/': {
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();

                    int maxWidth = std::max(leftVal.getTotalWidth(), rightVal.getTotalWidth()) + 8 * 2;
                    int height = 1.5;

                    textInfoString slash    = myASTHelper.GetVerticalSlash(maxWidth, height);
                    
                    textInfoString merge    = myASTHelper.MergeTwoTextsToDown(rightVal, slash, 1.0, 1.0);
                    textInfoString merge2   = myASTHelper.MergeTwoTextsToDown(merge, leftVal, 1.0, 1.0);

                    merge2.setPriority(precedence[x[0]]);
                    val.push(merge2);

                    break;
                }
                
                case '^': {
                    textInfoString leftVal = val.top(); val.pop();
                    textInfoString rightVal = val.top(); val.pop();

                    //only need brackets for base, exponents are moved upward anyway
                    if (precedence[x[0]] >= rightVal.prevPriority) {
                        textInfoString rightBracket = opDefault[')'];
                        textInfoString leftBracket = opDefault['('];

                        float inc = rightVal.getTotalHeight() / rightBracket.getTotalHeight();
                        textInfoString merge = myASTHelper.MergeTwoTextsToRight(leftBracket, rightVal, inc, 1.0);
                        textInfoString merge2 = myASTHelper.MergeTwoTextsToRight(merge, rightBracket, 1.0, inc);

                        rightVal = merge2;
                    }

                    textInfoString merge = myASTHelper.MergeTwoTextsToUpLeft(rightVal, leftVal, 0.9, 1.0, 0.7);

                    merge.setPriority(precedence[x[0]]);
                    val.push(merge);
                    break;
                }

                /*
                //TRIG FUNCTIONS
                case '!': {
                    float x = val.top(); val.pop();
                    val.push(std::sin(x));
                    break;
                }

                case '@': {
                    float x = val.top(); val.pop();
                    val.push(std::cos(x));
                    break;
                }

                case '#': {
                    float x = val.top(); val.pop();
                    val.push(std::tan(x));
                    break;
                }

                case '$': {
                    float x = val.top(); val.pop();
                    val.push(1/std::tan(x));
                    break;
                }

                //abs
                case '|': {
                    float x = val.top(); val.pop();
                    val.push(std::abs(x));
                    break;
                }

                //root
                case '%': {
                    float x = val.top(); val.pop();
                    val.push(std::sqrt(x));
                    break;
                }

                case '&': {
                    float x = val.top(); val.pop();
                    val.push(std::cbrt(x));
                    break;
                }

                //log
                case '{': {
                    float x = val.top(); val.pop();
                    val.push(std::exp(x));
                    break;
                }

                case '}': {
                    float x = val.top(); val.pop();
                    val.push(std::log(x));
                    break;
                }

                case '[': {
                    float x = val.top(); val.pop();
                    val.push(std::log10(x));
                    break;
                }

                case ']': {
                    float x = val.top(); val.pop();
                    val.push(std::log2(x));
                    break;
                }*/

                default: {
                    std::cout << "Unknown" << '\n'; return textInfoString(); break;
                }
            }
        }
    }
    return val.top();
}   