#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <QString>
#include <QObject>
#include "ui_mainwindow.h"

class RuntimeContext;
class Statement;
class Expression;

class ProgramManager : public QObject
{
    Q_OBJECT

private:
    bool _isRunning = false;
    bool _isWaitingForInput = false;
    std::string varNameWaitingForInput;
    std::_Rb_tree_iterator<std::pair<const int, Statement*>> curLineIter;

    Ui::MainWindow* ui;
    RuntimeContext* context;
    std::map<int, Statement*> statements;
    std::map<int, std::string> errors;

public:
    ProgramManager(Ui::MainWindow* ui);
    ~ProgramManager();
    void clearCommand();
    bool addCommand(QString& command);
    void showCode();
    void runCode();
    void continueRunning();
    void stopRunning();
    bool isRunning();
    bool isWaitingForInput();
    int getVarValue(const std::string& name) const;
    int getExpressionValue(Expression* expression) const;
    int getVarUseCnt(const std::string& name) const;
    void setVarValue(const std::string& name, Expression* expression);
    void println(const std::string& str);
    void runtimeError(const std::string& str);
    void inputVar(const std::string& varName);
    void gotInput(int value);
    void gotoLine(int targetLineIndex);
    void generateSyntaxTree();
};

#endif // PROGRAMMANAGER_H
