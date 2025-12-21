#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <string>
#include <vector>
#include <variant>

class RuntimeContext;

class Expression
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

    typedef enum
    {
        NUMBER = 1,
        IDENTIFIER,
        OPERATION,
        LPAREN,
        RPAREN,
        ERROR_TOKEN
    } TokenType;

    struct Token
    {
        TokenType type;
        std::string value;

        Token(TokenType type, const std::string& value) : type(type), value(value) {}
    };

public:
    virtual ~Expression() = default;

    virtual int getValue(RuntimeContext* context) const = 0;
    virtual std::string getSyntaxTree(int indent = 0) const = 0;

    static Expression* newExpFromStr(const std::string& exprStr);

private:
    static std::vector<Token> tokenize(const std::string& exprStr);
    static std::vector<Token> infixToPostfix(const std::vector<Token>& tokens);
    static OperationType operationFromStr(const std::string opStr);
    static int getOperationPrecedence(const OperationType opType);
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
    Expression* leftExp;
    Expression* rightExp;
    OperationType operation;

    CompoundExp(Expression* leftExp, Expression* rightExp, const OperationType operation);
    ~CompoundExp();
    int getValue(RuntimeContext* context) const override;
    std::string getSyntaxTree(int indent) const override;

private:
    int basicDivide(const int a, const int b) const;
    int basicMod(const int a, const int b) const;
    int basicPower(const int a, const int b) const;
};


#endif // EXPRESSION_H
