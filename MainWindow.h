#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <string>
#include "Config/config.h"
#include <functional>

class QPlainTextEdit;

#define node_(n) ui.nodebtn_##n

#define nodeNormal QString("QPushButton { background-color: white; \
                                          border-radius: 30px; \
                                          border-style: solid; \
                                          border-width: 3px; \
                                          border-color: black; }")
#define nodeHover QString("QPushButton:hover { background-color: yellow; \
                                          border-radius: 30px; \
                                          border-style: solid; \
                                          border-width: 5px; \
                                          border-color: black;}")
#define nodeChecked QString("QPushButton:checked{ background-color: red; \
                                          border-radius: 30px; \
                                          border-style: solid; \
                                          border-width: 7px; \
                                          border-color: black; }")
#define nodeLighted QString("QPushButton { background-color: red; \
                                          border-radius: 30px; \
                                          border-style: solid; \
                                          border-width: 7px; \
                                          border-color: black; }")

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    inline void setNodesCheckable();
    inline vector<QPushButton*> getNodes() 
                            { return nodes; };

    void setLwView(bool bshow);
    void refresh();

    vector<function<void()>> AlgorithmFuncs;
    int FuncID;

    static QPlainTextEdit* gLogstruct;
    static QPlainTextEdit* gLogbox;
private:
    Ui::MainWindowClass ui;
    vector<QPushButton*> nodes;
    QButtonGroup* nodeBtnGroup;

public:
    static void switchtoMatrix();
    static void switchtoList();
};

// 这个方法会将光标移动到文本的开头
#define Logset(content) MainWindow::gLogbox->setPlainText(content)
// 这个方法会将光标移动到文本的末尾，并自动换行，然后开始输出
#define Logappend(content) MainWindow::gLogbox->appendPlainText(content)
// 这个方法会将光标移动到插入文本的后面，不会自动换行，然后开始输出
#define Loginsert(content) MainWindow::gLogbox->insertPlainText(content)
// 清空
#define Logclear() MainWindow::gLogbox->clear()

#define Logstruct(content) MainWindow::gLogstruct->insertPlainText(content)
#define LogstructClear() MainWindow::gLogstruct->clear()