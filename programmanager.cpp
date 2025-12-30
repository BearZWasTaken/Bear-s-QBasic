#include "programmanager.h"

#include "runtimecontext.h"
#include "statement.h"
#include "expression.h"

#include <QTimer>
#include <sstream>

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

void ProgramManager::clearCommand()
{
    statements.clear();
    context->clear();
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

    try
    {
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
            Expression* exp = Expression::newExpFromStr(arguments.toStdString());
            new_statement = new PrintStmt(exp);
        }

        else if (keyword == "INPUT")
        {
            new_statement = new InputStmt(arguments.toStdString());
        }

        else if (keyword == "GOTO")
        {
            int targetLineIndex = arguments.toInt();
            new_statement = new GotoStmt(targetLineIndex);
        }

        else if (keyword == "IF")
        {
            int thenPos = arguments.indexOf("THEN");
            if (thenPos == -1)
            {
                errors[lineIndex] = "IF statement with no 'THEN'";
                return false;
            }

            QString ifCondition = arguments.left(thenPos).trimmed();
            int targetLineIndex = arguments.mid(thenPos + 4).trimmed().toInt();

            int compOpPos = arguments.indexOf('=');
            IfStmt::ComparisonType compType = IfStmt::EQUAL;
            if (compOpPos == -1)
            {
                compOpPos = arguments.indexOf('<');
                compType = IfStmt::LESS_THAN;
            }
            if (compOpPos == -1)
            {
                compOpPos = arguments.indexOf('>');
                compType = IfStmt::GREATER_THAN;
            }
            if (compOpPos == -1)
            {
                errors[lineIndex] = "IF statement with no condition operator";
                return false;
            }

            QString leftExpStr = ifCondition.left(compOpPos).trimmed();
            QString rightExpStr = ifCondition.mid(compOpPos + 1).trimmed();
            Expression* leftExp = Expression::newExpFromStr(leftExpStr.toStdString());
            Expression* rightExp = Expression::newExpFromStr(rightExpStr.toStdString());

            new_statement = new IfStmt(leftExp, rightExp, compType, targetLineIndex);
        }

        else if (keyword == "END")
        {
            new_statement = new EndStmt();
        }

        else
        {
            throw "Unknown keyword";
        }
    }
    catch (const char* errMsg)
    {
        errors[lineIndex] = std::string(errMsg);
        statements.erase(lineIndex);
        qDebug() << "Caught error: " + std::string(errMsg);
        return false;
    }

    new_statement->rawCodeWithIndex = command.toStdString();
    statements[lineIndex] = new_statement;
    return true;
}

void ProgramManager::showCode()
{
    std::stringstream res;
    for (auto& statement : statements)
        res << statement.second->rawCodeWithIndex << "\n";
    ui->CodeDisplay->setText(QString::fromStdString(res.str()));
}

void ProgramManager::runCode()
{
    qDebug() << "Run!";
    ui->textBrowser->clear();
    _isRunning = true;
    _isWaitingForInput = false;
    context->clear();
    for (auto& statement : statements)
        statement.second->resetStats();
    curLineIter = statements.begin();
    continueRunning();
}

void ProgramManager::continueRunning()
{
    while (_isRunning && !_isWaitingForInput)
    {
        auto tempLineIter = curLineIter;
        if (curLineIter == statements.end())
        {
            stopRunning();
            break;
        }
        qDebug() << "Executing line #" + std::to_string(curLineIter->first);
        curLineIter->second->execute(this);
        if (curLineIter == tempLineIter)
            ++curLineIter;
    }
}

void ProgramManager::stopRunning()
{
    _isRunning = false;
    generateSyntaxTree();
}

bool ProgramManager::isRunning() { return _isRunning; }
bool ProgramManager::isWaitingForInput() { return _isWaitingForInput; }

int ProgramManager::getVarValue(const std::string& name) const
{
    return context->getVarValue(name);
}

int ProgramManager::getExpressionValue(Expression* expression) const
{
    return expression->getValue(context);
}

int ProgramManager::getVarUseCnt(const std::string& name) const
{
    return context->getVarUseCnt(name);
}

void ProgramManager::setVarValue(const std::string& name, Expression* expression)
{
    context->setVarValue(name, expression->getValue(context));
}

void ProgramManager::println(const std::string& str)
{
    ui->textBrowser->append(QString::fromStdString(str));
}

void ProgramManager::runtimeError(const std::string& str)
{
    ui->textBrowser->append(QString::fromStdString("[Runtime Error] " + str));
    stopRunning();
}

void ProgramManager::inputVar(const std::string& varName)
{
    _isWaitingForInput = true;
    ui->textBrowser->append(" ? ");
    varNameWaitingForInput = varName;

    QTimer* inputCheckTimer = new QTimer();
    connect(inputCheckTimer, &QTimer::timeout, this, [this]() {
        if (_isWaitingForInput && !ui->cmdLineEdit->text().startsWith(" ? "))
            ui->cmdLineEdit->setText(" ? ");
    });
    inputCheckTimer->start(100);
}

void ProgramManager::gotInput(int value)
{
    ui->textBrowser->insertPlainText(QString::number(value));
    context->setVarValue(varNameWaitingForInput, value);
    _isWaitingForInput = false;
    continueRunning();
}

void ProgramManager::gotoLine(int targetLineIndex)
{
    curLineIter = statements.find(targetLineIndex);
    if (curLineIter == statements.end())
    {
        runtimeError("GOTO statement with a non-existed line index");
    }
}

void ProgramManager::generateSyntaxTree()
{
    std::stringstream res;

    for (auto& statement : statements)
    {
        res << statement.first << " " << statement.second->getTreeDisplay(this);
    }

    ui->treeDisplay->setText(QString::fromStdString(res.str()));
}
