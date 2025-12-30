#include "statement.h"

#include "programmanager.h"
#include "expression.h"

#include <sstream>

void Statement::execute(ProgramManager* pm)
{
    executionCnt++;
    doExecute(pm);
}

int Statement::getExecutionCnt() { return executionCnt; }

void Statement::resetStats()
{
    executionCnt = 0;
    doResetStats();
}


RemStmt::RemStmt(const std::string comment)
    : comment(comment) {}

std::string RemStmt::getTreeDisplay(ProgramManager* pm)
{
    std::stringstream treeDisplay;
    treeDisplay << "REM " << getExecutionCnt() << std::endl;
    treeDisplay << "    " << comment << std::endl;
    return treeDisplay.str();
}

void RemStmt::doExecute(ProgramManager* pm) {}
void RemStmt::doResetStats() {}


LetStmt::LetStmt(const std::string varName, Expression* expression)
    : varName(varName), expression(expression) {}

std::string LetStmt::getTreeDisplay(ProgramManager* pm)
{
    std::stringstream treeDisplay;
    treeDisplay << "LET = " << getExecutionCnt() << std::endl;
    treeDisplay << "    " << varName << " " << pm->getVarUseCnt(varName) << std::endl;
    treeDisplay << expression->getSyntaxTree(4) << std::endl;
    return treeDisplay.str();
}

void LetStmt::doExecute(ProgramManager* pm)
{
    pm->setVarValue(varName, expression);
}

void LetStmt::doResetStats() {}


PrintStmt::PrintStmt(Expression* expression)
    : expression(expression) {}

std::string PrintStmt::getTreeDisplay(ProgramManager* pm)
{
    std::stringstream treeDisplay;
    treeDisplay << "PRINT " << getExecutionCnt() << std::endl;
    treeDisplay << expression->getSyntaxTree(4) << std::endl;
    return treeDisplay.str();
}

void PrintStmt::doExecute(ProgramManager* pm)
{
    int value = pm->getExpressionValue(expression);
    pm->println(std::to_string(value));
}

void PrintStmt::doResetStats() {}


InputStmt::InputStmt(const std::string varName)
    : varName(varName) {}

std::string InputStmt::getTreeDisplay(ProgramManager* pm)
{
    std::stringstream treeDisplay;
    treeDisplay << "INPUT " << getExecutionCnt() << std::endl;
    treeDisplay << "    " << varName << " " << pm->getVarUseCnt(varName) << std::endl;
    return treeDisplay.str();
}

void InputStmt::doExecute(ProgramManager* pm)
{
    pm->inputVar(varName);
}

void InputStmt::doResetStats() {}


GotoStmt::GotoStmt(const int targetLineIndex)
    : targetLineIndex(targetLineIndex) {}

std::string GotoStmt::getTreeDisplay(ProgramManager* pm)
{
    std::stringstream treeDisplay;
    treeDisplay << "GOTO " << getExecutionCnt() << std::endl;
    treeDisplay << "    " << targetLineIndex << std::endl;
    return treeDisplay.str();
}

void GotoStmt::doExecute(ProgramManager* pm)
{
    pm->gotoLine(targetLineIndex);
}

void GotoStmt::doResetStats() {}


IfStmt::IfStmt(Expression* leftExp, Expression* rightExp, const ComparisonType comp, const int targetLineIndex)
    : leftExp(leftExp), rightExp(rightExp), comp(comp), targetLineIndex(targetLineIndex) {}

std::string IfStmt::getTreeDisplay(ProgramManager* pm)
{
    std::stringstream treeDisplay;
    treeDisplay << "IF THEN " << getTrueCnt() << " " << getFalseCnt() << std::endl;
    treeDisplay << leftExp->getSyntaxTree(4) << std::endl;
    switch (comp)
    {
        case EQUAL: treeDisplay << "    =" << std::endl; break;
        case LESS_THAN: treeDisplay << "    <" << std::endl; break;
        case GREATER_THAN: treeDisplay << "    >" << std::endl; break;
        default: treeDisplay << "???" << std::endl; break;
    }
    treeDisplay << rightExp->getSyntaxTree(4) << std::endl;
    return treeDisplay.str();
}

void IfStmt::doExecute(ProgramManager* pm)
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
    {
        trueCnt++;
        pm->gotoLine(targetLineIndex);
    }
    else
    {
        falseCnt++;
    }
}

int IfStmt::getTrueCnt() { return trueCnt; }
int IfStmt::getFalseCnt() { return falseCnt; }
void IfStmt::doResetStats() { trueCnt = falseCnt = 0; }


EndStmt::EndStmt() {}

std::string EndStmt::getTreeDisplay(ProgramManager* pm)
{
    std::stringstream treeDisplay;
    treeDisplay << "END " << getExecutionCnt() << std::endl;
    return treeDisplay.str();
}

void EndStmt::doExecute(ProgramManager* pm)
{
    pm->stopRunning();
}

void EndStmt::doResetStats() {}
