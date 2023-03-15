#include "MainWindow.h"
#include "Config/config.h"
#include <Windows.h>
#include <qtimer.h>
#include "Algorithm/algorithm.h"
#include <QPainter>
#include <QButtonGroup>
#include <QMessageBox>
#include "selectDialog/selectDialog.h"
#include "selectDialog/selectPathDialog.h"

QPlainTextEdit* MainWindow::gLogstruct = nullptr;
QPlainTextEdit* MainWindow::gLogbox = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setFixedSize(size());

    gLogbox = ui.logBox;
    gLogstruct = ui.logStruct;

    // ��ʼ������
    AlgorithmFuncs = { algorithm::DFS, algorithm::BFS, algorithm::HavePath,
                       algorithm::onePath, algorithm::allPath, algorithm::defaultLengthPath,
                       algorithm::findCycles, algorithm::findnoWeightSPath,
                       algorithm::findnoWeightFnode,algorithm::buildMST,algorithm::dijkstra };

    nodes = { node_(0), node_(1), node_(2), node_(3),
              node_(4), node_(5), node_(6), node_(7) };
    nodeBtnGroup = new QButtonGroup(this);
    // ��ʼ���㷨�еľ�̬����
    algorithm::nodes = nodes;

    for (int i = 0; i < nodes.size();i++) {
        nodes[i]->setCheckable(true);
        nodeBtnGroup->addButton(nodes[i],i);
        nodes[i]->setStyleSheet(nodeNormal+nodeHover+nodeChecked);
    }

    // ѡ�����ݽṹ
    connect(ui.btn_matrix, &QPushButton::clicked, this,&MainWindow::switchtoMatrix);
    connect(ui.btn_list, &QPushButton::clicked, this, &MainWindow::switchtoList);
    switchtoMatrix();
    // ������Ȩ��Ȩ
    connect(ui.btn_weight, &QPushButton::clicked, this, [this]() {
        setLwView(true);
        Config::getInstance()->setGraphWeight(true); });
    connect(ui.btn_noweight, &QPushButton::clicked, this, [this]() {
        setLwView(false);
        Config::getInstance()->setGraphWeight(false); });

    
    connect(ui.btn_DFS, &QPushButton::clicked, [this]() {
        Logclear();
        Loginsert("��ѡ��DFS���\n");
        setNodesCheckable();
        FuncID = 0; });
    connect(ui.btn_BFS, &QPushButton::clicked, [this]() {
        Logclear();
        Loginsert("��ѡ��BFS���\n");
        setNodesCheckable();
        FuncID = 1; });
    connect(ui.btn_havepath, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("�Ƿ���·��\n");
        selectDialog dlg(this);
        dlg.exec();
        algorithm::start = dlg.ui->spb_start->value();
        algorithm::end = dlg.ui->spb_end->value();
        foreach(auto& node,nodes) {
            node->setCheckable(false);
        }
        FuncID = 2; });
    connect(ui.btn_onepath, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("���һ��·��\n");
        selectDialog dlg(this);
        dlg.exec();
        algorithm::start = dlg.ui->spb_start->value();
        algorithm::end = dlg.ui->spb_end->value();
        foreach(auto & node, nodes) {
            node->setCheckable(false);
        }
        FuncID = 3; });
    connect(ui.btn_allpath, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("��ʾ����·��\n");
        refresh();
        selectDialog dlg(this);
        dlg.exec();
        algorithm::start = dlg.ui->spb_start->value();
        algorithm::end = dlg.ui->spb_end->value();
        foreach(auto & node, nodes) {
            node->setCheckable(false);
        }
        FuncID = 4; });
    connect(ui.btn_lengthPath, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("����ָ�����ȵ�·��\n");
        selectDialog dlg(this);
        dlg.ui->label->setText("��ֱ�ѡ������ָ������");
        dlg.ui->label_2->setText("  ���룺");
        dlg.exec();
        algorithm::start = dlg.ui->spb_start->value();
        algorithm::end = dlg.ui->spb_end->value();
        FuncID = 5; });
    connect(ui.btn_findCycles, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("�����Ƿ��л�\n");
        FuncID = 6; });
    connect(ui.btn_noweightSPath, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("��������Ȩ�����·��\n");
        Config::getInstance()->setGraphWeight(false);
        setLwView(false);
        selectDialog dlg(this);
        dlg.exec();
        algorithm::start = dlg.ui->spb_start->value();
        algorithm::end = dlg.ui->spb_end->value();
        FuncID = 7; });
    connect(ui.btn_noweightFnode, &QPushButton::clicked, [this]() {
        Logclear();
        Loginsert("��������Ȩ����Զ����\n");
        setNodesCheckable();
        Config::getInstance()->setGraphWeight(false);
        setLwView(false);
        FuncID = 8; });
    connect(ui.btn_mst, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("������С������\n");
        FuncID = 9; });
    connect(ui.btn_dijkstra, &QPushButton::clicked, [this]() {
        Logclear();
        refresh();
        Loginsert("dijkstra������·��\n");
        selectDialog dlg(this);
        dlg.exec();
        algorithm::start = dlg.ui->spb_start->value();
        algorithm::end = dlg.ui->spb_end->value();
        FuncID = 10; });

    // ������ˢ��
    connect(ui.btn_emulate, &QPushButton::clicked, [this]() { 
        if (FuncID == 0 || FuncID == 1 || FuncID == 8)
            algorithm::start = nodeBtnGroup->checkedId();
        
        // ��������㷨
        AlgorithmFuncs[FuncID]();

        if (FuncID == 4 || FuncID == 5) {
            selectPathDialog* dlg = new selectPathDialog(algorithm::ret.size(), this);
            connect(dlg->pathBtnGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), &algorithm::LightNodes);
        } });
    connect(ui.btn_refresh, &QPushButton::clicked, this, &MainWindow::refresh);
}

MainWindow::~MainWindow()
{}

inline void MainWindow::setNodesCheckable()
{
    nodeBtnGroup->setExclusive(false);
    for (int i = 0; i < nodes.size(); i++) {
        nodes[i]->setChecked(false);
        nodes[i]->setCheckable(true);
        nodes[i]->setStyleSheet(nodeNormal + nodeHover + nodeChecked);
    }
    nodeBtnGroup->setExclusive(true);   // ��ѡ
        
    algorithm::start = -1;
}

void MainWindow::setLwView(bool bshow)
{
    for (auto lw : { ui.wl_0, ui.wl_1, ui.wl_2, ui.wl_3, ui.wl_4, ui.wl_5, ui.wl_6, ui.wl_7, ui.wl_8, ui.wl_9 }) {
        lw->setVisible(bshow);
    }
}

void MainWindow::refresh()
{
    algorithm::start = -1;
    algorithm::end = -1;
    foreach(auto & node, nodes) {
        //node->setCheckable(false);
        node->setStyleSheet(nodeNormal);
    }
}

void MainWindow::switchtoMatrix()
{
    algorithm::bisMatrix = true;

    LogstructClear();
    Logstruct("��ǰ���ݽṹ���ڽӾ���:\n");
    int n = GETGRAPH->nodenum;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (GETMATRIX[i][j] == INT_MAX)
                Logstruct(QString("n "));
            else
                Logstruct(QString("%1 ").arg(GETMATRIX[i][j]));
        }
        Logstruct("\n");
    }
}

void MainWindow::switchtoList()
{
    algorithm::bisMatrix = false;

    LogstructClear();
    Logstruct("��ǰ���ݽṹ���ڽӱ�:\n");
    for (int i = 0; i < GETGRAPH->nodenum; i++) {
        Logstruct(QString("node %1 -> ").arg(GETLIST[i]->index));
        for (ArcNode* p = GETLIST[i]->firstarc; p; p = p->nextarc)
            Logstruct(QString("%1 ").arg(p->dst));
        Logstruct("\n");
    }
}



