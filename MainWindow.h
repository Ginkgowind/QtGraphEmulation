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

// ��������Ὣ����ƶ����ı��Ŀ�ͷ
#define Logset(content) MainWindow::gLogbox->setPlainText(content)
// ��������Ὣ����ƶ����ı���ĩβ�����Զ����У�Ȼ��ʼ���
#define Logappend(content) MainWindow::gLogbox->appendPlainText(content)
// ��������Ὣ����ƶ��������ı��ĺ��棬�����Զ����У�Ȼ��ʼ���
#define Loginsert(content) MainWindow::gLogbox->insertPlainText(content)
// ���
#define Logclear() MainWindow::gLogbox->clear()

#define Logstruct(content) MainWindow::gLogstruct->insertPlainText(content)
#define LogstructClear() MainWindow::gLogstruct->clear()