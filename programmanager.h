#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <QString>
#include "ui_mainwindow.h"

class RuntimeContext;
class Statement;

class ProgramManager
{
private:
    Ui::MainWindow* ui;
    RuntimeContext* context;
    std::map<int, Statement*> statements;
    std::map<int, std::string> errors;

public:
    ProgramManager(Ui::MainWindow* ui);
    ~ProgramManager();
    bool addCommand(QString& command);
};

#endif // PROGRAMMANAGER_H
