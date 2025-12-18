#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>


class ProgramManager;
class Expression;

class Statement
{
public:
    virtual ~Statement() = default;

    virtual void execute(ProgramManager& pm) = 0;
};


class RemStmt : public Statement
{
public:
    std::string comment;

    RemStmt(const std::string comment);
    void execute(ProgramManager& state) override;
};


class LetStmt : public Statement
{
public:
    std::string varName;
    Expression* expression;

public:
    LetStmt(const std::string varName, Expression* expression);
    void execute(ProgramManager& state) override;
};


class PrintStmt : public Statement
{
public:
    Expression* expression;

    PrintStmt(Expression* expression);
    void execute(ProgramManager& state) override;
};


class InputStmt : public Statement
{
public:
    std::string varName;

    InputStmt(const std::string varName);
    void execute(ProgramManager& state) override;
};


class GotoStmt : public Statement
{
public:
    int targetLineIndex;

    GotoStmt(const int targetLineIndex);
    void execute(ProgramManager& state) override;
};


class IfStmt : public Statement
{
public:
    typedef enum
    {
        EQUAL = 1,
        LESS_THAN,
        GREATER_THAN
    } ComparisonType;

    Expression* leftExp;
    Expression* rightExp;
    ComparisonType comp;
    int targetLineIndex;

private:
    int true_cnt;
    int false_cnt;

public:
    IfStmt(Expression* leftExp, Expression* rightExp, const ComparisonType comp, const int targetLineIndex);
    void execute(ProgramManager& state) override;
};


class EndStmt : public Statement
{
public:
    EndStmt();
    void execute(ProgramManager& state) override;
};


#endif // STATEMENT_H
