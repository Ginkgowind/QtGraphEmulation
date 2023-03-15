#include "config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDebug>
#include "../MainWindow.h"
#include "../Algorithm/algorithm.h"
//using namespace std;

void Config::setGraphWeight(bool bhaveWeight)
{
    //Logclear();
    Loginsert(bhaveWeight==true? "设置为有权图\n": "设置为无权图\n");
    for (int i = 0; i < mGraph->edgenum; i++) {
        if (bhaveWeight == true) {
            mGraph->edgelist[i].weight = weights[i];
        }
        else {
            mGraph->edgelist[i].weight = 1;
        }
    }
    buildAdjacencyMatrix();
    buildAdjacencyList();

    if (algorithm::bisMatrix) {
        MainWindow::switchtoMatrix();
    }
    else {
        MainWindow::switchtoList();
    }
}

Config::Config()
{
    mGraph = nullptr;
}

Config::~Config()
{
    delete mGraph;
}

graph* Config::parse(string filename)
{
    mGraph = new graph;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
        return nullptr;
    }

    // 解析头
    {
        string line;
        getline(file, line);
        istringstream iss(line);
        iss >> mGraph->nodenum >> mGraph->edgenum;
        mGraph->edgelist = new edge[mGraph->edgenum];
    }

    // 解析边
    string line;
    weights.resize(mGraph->edgenum);
    for (int i = 0; i<mGraph->edgenum; i++) {
        getline(file, line);
        istringstream iss(line);
        iss >> mGraph->edgelist[i].start
            >> mGraph->edgelist[i].end
            >> mGraph->edgelist[i].weight;
        
        weights[i] = mGraph->edgelist[i].weight;
    }

    file.close();

	return mGraph;
}

void Config::buildAdjacencyMatrix()
{
    // 获取节点数量
    int num = mGraph->nodenum;
    mAdjacencyMatrix.resize(num, vector<int>(num, INT_MAX));

    for (int i = 0; i < mGraph->edgenum; i++) {
        int start = mGraph->edgelist[i].start;
        int end = mGraph->edgelist[i].end;
        int weight = mGraph->edgelist[i].weight;
        mAdjacencyMatrix[start][end] = weight;
    }
}

void Config::buildAdjacencyList()
{
    Vnode* p = new Vnode{ 0,nullptr };
    mAdjacencyList.resize(mGraph->nodenum );
    for (int i = 0; i < mGraph->nodenum; i++)
        mAdjacencyList[i] = new Vnode{ i,nullptr };

    vector<Vnode*>& lst = mAdjacencyList;
    // 遍历每一条边
    for (int i = 0; i < mGraph->edgenum; i++) {
        int start = mGraph->edgelist[i].start;
        int end = mGraph->edgelist[i].end;
        int weight = mGraph->edgelist[i].weight;
        
        // 创建新节点
        ArcNode* next = new ArcNode;
        next->dst = end;
        next->weight = weight;
        next->nextarc = nullptr;

        if (lst[start]->firstarc == nullptr)
            lst[start]->firstarc = next;
        else {
            ArcNode* p = lst[start]->firstarc;
            for (; p->nextarc; p = p->nextarc);
            p->nextarc = next;
        }
    }

}




