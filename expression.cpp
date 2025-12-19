#include "expression.h"

#include "runtimecontext.h"

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
