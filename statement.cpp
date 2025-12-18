#include "statement.h"


RemStmt::RemStmt(const std::string comment)
    : comment(comment) {}

void RemStmt::execute(ProgramManager& state) {}


LetStmt::LetStmt(const std::string varName, Expression* expression)
    : varName(varName), expression(expression) {}

void LetStmt::execute(ProgramManager& state) {}


PrintStmt::PrintStmt(Expression* expression)
    : expression(expression) {}

void PrintStmt::execute(ProgramManager& state) {}


InputStmt::InputStmt(const std::string varName)
    : varName(varName) {}

void InputStmt::execute(ProgramManager& state) {}


GotoStmt::GotoStmt(const int targetLineIndex)
    : targetLineIndex(targetLineIndex) {}

void GotoStmt::execute(ProgramManager& state) {}


IfStmt::IfStmt(Expression* leftExp, Expression* rightExp, const ComparisonType comp, const int targetLineIndex)
    : leftExp(leftExp), rightExp(rightExp), comp(comp), targetLineIndex(targetLineIndex) {}

void IfStmt::execute(ProgramManager& state) {}


EndStmt::EndStmt() {}

void EndStmt::execute(ProgramManager& state) {}
