#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>


class ProgramManager;
class Expression;

class Statement
{
private:
    int executionCnt = 0;

public:
    virtual ~Statement() = default;

    virtual std::string getTreeDisplay(ProgramManager* pm) = 0;
    void execute(ProgramManager* pm);
    virtual void doExecute(ProgramManager* pm) = 0;
    int getExecutionCnt();
    void resetStats();
    virtual void doResetStats() = 0;
};


class RemStmt : public Statement
{
public:
    std::string comment;

    RemStmt(const std::string comment);
    std::string getTreeDisplay(ProgramManager* pm) override;
    void doExecute(ProgramManager* pm) override;
    void doResetStats() override;
};


class LetStmt : public Statement
{
public:
    std::string varName;
    Expression* expression;

public:
    LetStmt(const std::string varName, Expression* expression);
    std::string getTreeDisplay(ProgramManager* pm) override;
    void doExecute(ProgramManager* pm) override;
    void doResetStats() override;
};


class PrintStmt : public Statement
{
public:
    Expression* expression;

    PrintStmt(Expression* expression);
    std::string getTreeDisplay(ProgramManager* pm) override;
    void doExecute(ProgramManager* pm) override;
    void doResetStats() override;
};


class InputStmt : public Statement
{
public:
    std::string varName;

    InputStmt(const std::string varName);
    std::string getTreeDisplay(ProgramManager* pm) override;
    void doExecute(ProgramManager* pm) override;
    void doResetStats() override;
};


class GotoStmt : public Statement
{
public:
    int targetLineIndex;

    GotoStmt(const int targetLineIndex);
    std::string getTreeDisplay(ProgramManager* pm) override;
    void doExecute(ProgramManager* pm) override;
    void doResetStats() override;
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
    int trueCnt = 0;
    int falseCnt = 0;

public:
    IfStmt(Expression* leftExp, Expression* rightExp, const ComparisonType comp, const int targetLineIndex);
    std::string getTreeDisplay(ProgramManager* pm) override;
    void doExecute(ProgramManager* pm) override;
    int getTrueCnt();
    int getFalseCnt();
    void doResetStats() override;
};


class EndStmt : public Statement
{
public:
    EndStmt();
    std::string getTreeDisplay(ProgramManager* pm) override;
    void doExecute(ProgramManager* pm) override;
    void doResetStats() override;
};


#endif // STATEMENT_H
