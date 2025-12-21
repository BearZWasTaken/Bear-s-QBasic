#include "programmanager.h"

#include "runtimecontext.h"
#include "statement.h"
#include "expression.h"

ProgramManager::ProgramManager(Ui::MainWindow* ui)
    : ui(ui) , context(new RuntimeContext) {}

ProgramManager::~ProgramManager()
{
    if (context)
        delete context;

    for (auto statement : statements)
        if(statement.second)
            delete statement.second;
}

bool ProgramManager::addCommand(QString& command)
{
    QTextStream textStream(&command);

    int lineIndex = 0;
    QString keyword;
    QString arguments;

    textStream >> lineIndex;
    textStream >> keyword;
    arguments = textStream.readAll().trimmed();

    qDebug() << "---------------------------";
    qDebug() << "Raw command: " << command;
    qDebug() << "  lineIndex:" << lineIndex;
    qDebug() << "  keyword:" << keyword;
    qDebug() << "  arguments:" << arguments;

    Statement* new_statement = nullptr;

    if (keyword == "REM")
    {
        new_statement = new RemStmt(arguments.toStdString());
    }

    else if (keyword == "LET")
    {
        int eqPos = arguments.indexOf('=');
        if (eqPos == -1)
        {
            errors[lineIndex] = "LET statement with no '='";
            return false;
        }

        QString varName = arguments.left(eqPos).trimmed();
        QString expStr = arguments.mid(eqPos + 1).trimmed();

        if (varName.isEmpty())
        {
            errors[lineIndex] = "LET statement with no variable name";
            return false;
        }

        if (expStr.isEmpty())
        {
            qDebug() << "LET statement with no expression";
            return false;
        }

        Expression* exp = Expression::newExpFromStr(expStr.toStdString());
        new_statement = new LetStmt(varName.toStdString(), exp);
    }

    else if (keyword == "PRINT")
    {

    }

    else if (keyword == "INPUT")
    {

    }

    else if (keyword == "GOTO")
    {

    }

    else if (keyword == "IF")
    {

    }

    else if (keyword == "END")
    {

    }

    statements[lineIndex] = new_statement;
    return true;
}
