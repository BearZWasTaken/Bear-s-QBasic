#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "programmanager.h"

#include <QFileDialog>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pm(new ProgramManager(ui))
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pm;
}

void MainWindow::on_cmdLineEdit_editingFinished()
{
    QString text = ui->cmdLineEdit->text();
    if (!pm->isRunning())
    {
        ui->CodeDisplay->append(text);
    }
    else if (pm->isWaitingForInput())
    {
        QString valueStr;
        if (text.startsWith(" ? "))
            valueStr = text.mid(3);
        else
            valueStr = text;
        pm->gotInput(valueStr.toInt());
    }
    ui->cmdLineEdit->setText("");
}

void MainWindow::on_btnLoadCode_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "加载BASIC程序",
        "",
        "BASIC文件 (*.bas *.txt);;所有文件 (*.*)"
        );

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    QTextStream fin(&file);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "错误", "无法打开文件: " + fileName);
        return;
    }

    ui->CodeDisplay->clear();
    pm->clearCommand();
    int lineCount = 0;
    while (!fin.atEnd())
    {
        QString line = fin.readLine().trimmed();
        lineCount++;

        ui->CodeDisplay->append(line);
        pm->addCommand(line);
    }

    file.close();
}

void MainWindow::on_btnRunCode_clicked()
{
    pm->runCode();
}

void MainWindow::on_btnClearCode_clicked()
{
    ui->CodeDisplay->clear();
    ui->textBrowser->clear();
    ui->treeDisplay->clear();
}
