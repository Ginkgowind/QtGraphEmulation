#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include "Config/config.h"

//#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")// ��ָ���֧��VS����
//#endif


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // �����ļ�
    string conFigfilePath = "./Config/nodeInfos.txt";
    graph *mGraph = Config::getInstance()->parse(conFigfilePath);
    Config::getInstance()->buildAdjacencyMatrix();
    Config::getInstance()->buildAdjacencyList();

    // ����mGraph
    MainWindow w;
    w.show();
    return a.exec();
}
