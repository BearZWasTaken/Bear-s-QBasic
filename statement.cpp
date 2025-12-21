#include "statement.h"

#include "programmanager.h"
#include "expression.h"

RemStmt::RemStmt(const std::string comment)
    : comment(comment) {}

void RemStmt::execute(ProgramManager* pm) {}


LetStmt::LetStmt(const std::string varName, Expression* expression)
    : varName(varName), expression(expression) {}

void LetStmt::execute(ProgramManager* pm)
{
    pm->setIdentifierValue(varName, expression);
}


PrintStmt::PrintStmt(Expression* expression)
    : expression(expression) {}

void PrintStmt::execute(ProgramManager* pm)
{
    int value = pm->getExpressionValue(expression);
    pm->println(std::to_string(value));
}


InputStmt::InputStmt(const std::string varName)
    : varName(varName) {}

void InputStmt::execute(ProgramManager* pm)
{
    pm->inputVar(varName);
}


GotoStmt::GotoStmt(const int targetLineIndex)
    : targetLineIndex(targetLineIndex) {}

void GotoStmt::execute(ProgramManager* pm)
{
    pm->gotoLine(targetLineIndex);
}


IfStmt::IfStmt(Expression* leftExp, Expression* rightExp, const ComparisonType comp, const int targetLineIndex)
    : leftExp(leftExp), rightExp(rightExp), comp(comp), targetLineIndex(targetLineIndex) {}

void IfStmt::execute(ProgramManager* pm)
{
    int leftExpValue = pm->getExpressionValue(leftExp);
    int rightExpValue = pm->getExpressionValue(rightExp);
    bool satisfied = false;
    switch (comp)
    {
        case EQUAL: satisfied = leftExpValue == rightExpValue; break;
        case LESS_THAN: satisfied = leftExpValue < rightExpValue; break;
        case GREATER_THAN: satisfied = leftExpValue > rightExpValue; break;
        default: break;
    }
    if (satisfied)
        pm->gotoLine(targetLineIndex);
}


EndStmt::EndStmt() {}

void EndStmt::execute(ProgramManager* pm)
{
    pm->stopRunning();
}
