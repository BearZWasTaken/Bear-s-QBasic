#include "expression.h"

#include "runtimecontext.h"

#include <stack>
#include <QDebug>

Expression* Expression::newExpFromStr(const std::string& expStr)
{
    std::vector<Token> tokens = tokenize(expStr);
    std::vector<Token> postfixTokens = infixToPostfix(tokens);
    std::stack<Expression*> expStack;

    for (const auto& token : postfixTokens)
    {
        switch (token.type)
        {
            case NUMBER:
            {
                int value = std::stoi(token.value);
                expStack.push(new ConstantExp(value));
                break;
            }

            case IDENTIFIER:
            {
                expStack.push(new IdentifierExp(token.value));
                break;
            }

            case OPERATION:
            {
                if (expStack.size() < 2)
                {
                    while (!expStack.empty())
                    {
                        delete expStack.top();
                        expStack.pop();
                    }
                    throw "Incomplete expression";
                }

                Expression* right = expStack.top();
                expStack.pop();

                Expression* left = expStack.top();
                expStack.pop();

                OperationType opType = operationFromStr(token.value);

                expStack.push(new CompoundExp(left, right, opType));
                break;
            }

            case ERROR_TOKEN:
            default:
            {
                while (!expStack.empty())
                {
                    delete expStack.top();
                    expStack.pop();
                }
                throw "Invalid token: " + token.value;
            }
        }
    }

    if (expStack.size() != 1)
    {
        while (!expStack.empty())
        {
            delete expStack.top();
            expStack.pop();
        }
        throw "Incomplete expression";
    }

    return expStack.top();
}

std::vector<Expression::Token> Expression::tokenize(const std::string& expStr)
{
    std::vector<Token> tokens;
    std::string currentToken;
    size_t len = expStr.length();

    for (size_t i = 0; i < len; i++)
    {
        char c = expStr[i];

        if (std::isspace(c))
        {
            if (!currentToken.empty())
            {
                tokens.push_back( {ERROR_TOKEN, currentToken} );
                currentToken.clear();
            }
            continue;
        }

        // number
        if (std::isdigit(c) || (c == '-' && (i == 0 || expStr[i-1] == '(')))
        {
            currentToken += c;
            i++;
            while (i < len && std::isdigit(expStr[i]))
            {
                currentToken += expStr[i];
                i++;
            }
            i--;
            tokens.push_back( {NUMBER, currentToken} );
            currentToken.clear();
            continue;
        }

        // operator
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')')
        {
            if (!currentToken.empty())
            {
                tokens.push_back( {OPERATION, currentToken} );
                currentToken.clear();
            }

            if (c == '*' && i+1 < len && expStr[i+1] == '*')
            {
                tokens.push_back( {OPERATION, "**"} );
                i++;
            }
            else
            {
                tokens.push_back( {OPERATION, std::string(1, c)} );
            }
            continue;
        }

        // MOD
        if (std::toupper(c) == 'M' && i + 2 < len)
        {
            std::string sub = expStr.substr(i, 3);

            if (sub == "MOD")
            {
                if (!currentToken.empty())
                {
                    tokens.push_back( {ERROR_TOKEN, currentToken} );
                    currentToken.clear();
                }
                tokens.push_back( {OPERATION, "MOD"} );
                i += 2;
                continue;
            }
        }

        // identifier
        if (std::isalpha(c) || c == '_')
        {
            currentToken += c;
            i++;
            while (i < len && (std::isalnum(expStr[i]) || expStr[i] == '_'))
            {
                currentToken += expStr[i];
                i++;
            }
            i--;
            tokens.push_back( {IDENTIFIER, currentToken} );
            currentToken.clear();
            continue;
        }

        // error
        if (!currentToken.empty())
        {
            tokens.push_back( {ERROR_TOKEN, currentToken} );
            currentToken.clear();
            throw "Unknown character: " + std::string(1, c);
        }
    }

    if (!currentToken.empty())
    {
        tokens.push_back( {ERROR_TOKEN, currentToken} );
    }

    return tokens;
}

std::vector<Expression::Token> Expression::infixToPostfix(const std::vector<Expression::Token>& tokens)
{
    std::vector<Token> res;
    std::stack<Token> stack;

    for (const auto& token : tokens)
    {
        TokenType type = token.type;
        std::string value = token.value;

        switch (type)
        {
        case NUMBER:
        case IDENTIFIER:
            res.push_back(token);
            break;

        case OPERATION:
            if (value == "(")
            {
                stack.push(token);
            }
            else if (value == ")")
            {
                while (!stack.empty() && stack.top().value != "(")
                {
                    res.push_back(stack.top());
                    stack.pop();
                }

                if (stack.empty())
                {
                    throw "Unmatched parenthese";
                }

                stack.pop();
            }
            else // operation
            {
                while (!stack.empty() && stack.top().value != "(")
                {
                    Token topOp = stack.top();
                    int topPrec = getOperationPrecedence(operationFromStr(topOp.value));
                    int curPrec = getOperationPrecedence(operationFromStr(value));

                    // 栈顶运算符优先级更高，或两个连着的"**"
                    if (topPrec > curPrec || (topPrec == curPrec && operationFromStr(topOp.value) == POW))
                    {
                        res.push_back(topOp);
                        stack.pop();
                    }
                    else
                    {
                        break;
                    }
                }
                stack.push(token);
            }
            break;

        case ERROR_TOKEN:
            throw "Invalid token: " + value;
            break;
        }
    }

    // 弹出剩余运算符
    while (!stack.empty())
    {
        if (stack.top().value == "(" || stack.top().value == ")")
            throw "Unmatched parenthese";
        res.push_back(stack.top());
        stack.pop();
    }

    return res;
}

Expression::OperationType Expression::operationFromStr(const std::string opStr)
{
    if (opStr == "+") return ADD;
    if (opStr == "-") return SUB;
    if (opStr == "*") return MUL;
    if (opStr == "/") return DIV;
    if (opStr == "MOD") return MOD;
    if (opStr == "**") return POW;
}

int Expression::getOperationPrecedence(const OperationType opType)
{
    switch (opType)
    {
        case ADD: return 1;
        case SUB: return 1;
        case MUL: return 2;
        case DIV: return 2;
        case MOD: return 2;
        case POW: return 3;
        break;
    }
    return 0;
}


ConstantExp::ConstantExp(const int value)
    : value(value) {}

int ConstantExp::getValue(RuntimeContext* context) const
{
    return value;
}

std::string ConstantExp::getSyntaxTree(int indent) const
{
    return std::string(indent, ' ') + std::to_string(value);
}


IdentifierExp::IdentifierExp(const std::string name)
    : name(name) {}

int IdentifierExp::getValue(RuntimeContext* context) const
{
    return context->getIdentifierValue(name);
}

std::string IdentifierExp::getSyntaxTree(int indent) const
{
    return std::string(indent, ' ') + name;
}


CompoundExp::CompoundExp(Expression* leftExp, Expression* rightExp, const OperationType operation)
    : leftExp(leftExp), rightExp(rightExp), operation(operation) {}

CompoundExp::~CompoundExp()
{
    if (leftExp) delete leftExp;
    if (rightExp) delete rightExp;
}

int CompoundExp::getValue(RuntimeContext* context) const
{
    int leftValue = leftExp->getValue(context);
    int rightValue = rightExp->getValue(context);

    try
    {
        switch (operation)
        {
            case ADD: return leftValue + rightValue;
            case SUB: return leftValue - rightValue;
            case MUL: return leftValue * rightValue;
            case DIV: return basicDivide(leftValue, rightValue);
            case MOD: return basicMod(leftValue, rightValue);
            case POW: return basicPower(leftValue, rightValue);
        }
    }
    catch (const char* errMsg)
    {
        context->throwError(errMsg);
    }
}

std::string CompoundExp::getSyntaxTree(int indent) const
{
    std::string res = std::string(indent, ' ');
    switch (operation) {
    case ADD: res += "+"; break;
    case SUB: res += "-"; break;
    case MUL: res += "*"; break;
    case DIV: res += "/"; break;
    case MOD: res += "MOD"; break;
    case POW: res += "**"; break;
    }
    res += "\n";
    res += leftExp->getSyntaxTree(indent + 4) + "\n";
    res += rightExp->getSyntaxTree(indent + 4);
    return res;
}

int CompoundExp::basicDivide(const int a, const int b) const
{
    if (b == 0)
        throw "Division by zero";
    return a / b;
}

int CompoundExp::basicMod(const int a, const int b) const
{
    if (b == 0)
        throw "Mod by zero";
    int res = a % b;
    if (res != 0 && ((a < 0) != (b < 0)))
        res += b;
    return res;
}

int CompoundExp::basicPower(const int a, const int b) const
{
    if (b < 0)
        throw "Power with negative exponent";
    int res = 1;
    for (int i = 0; i < b; i++)
        res *= a;
    return res;
}
