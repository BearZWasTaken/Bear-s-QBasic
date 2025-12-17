#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>


class RuntimeContext;

class Expression
{
public:
    virtual ~Expression() = default;

    virtual int getValue(RuntimeContext* context) const = 0;
    virtual std::string getSyntaxTree(int indent = 0) const = 0;
};


class ConstantExp : public Expression
{
public:
    int value;

    ConstantExp(const int value);
    int getValue(RuntimeContext* context) const override;
    std::string getSyntaxTree(int indent) const override;
};


class IdentifierExp : public Expression
{
public:
    std::string name;

    IdentifierExp(const std::string name);
    int getValue(RuntimeContext* context) const override;
    std::string getSyntaxTree(int indent) const override;
};


class CompoundExp : public Expression
{
public:
    typedef enum
    {
        ADD = 1,
        SUB,
        MUL,
        DIV,
        MOD,
        POW
    } OperationType;

public:
    Expression* leftExp;
    Expression* rightExp;
    OperationType operation;

    CompoundExp(Expression* leftExp, Expression* rightExp, const OperationType operation);
    int getValue(RuntimeContext* context) const override;
    std::string getSyntaxTree(int indent) const override;

private:
    int basicDivide(const int a, const int b) const;
    int basicMod(const int a, const int b) const;
    int basicPower(const int a, const int b) const;
};


#endif // EXPRESSION_H
