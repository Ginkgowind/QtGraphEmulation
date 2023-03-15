#pragma once

// ��ֹ��������
#ifdef WIN32  
#pragma execution_character_set("utf-8")  
#endif

#include <string>
#include <vector>
using namespace std;

struct edge {
	int start;
	int end;
	int weight;
};

struct graph {
	int nodenum;
	int edgenum;
	edge* edgelist;
};

// �����ڽӱ����ݽṹ
struct ArcNode {
	int dst;
	int weight;
	ArcNode* nextarc;
};

struct Vnode {
	int index;
	ArcNode* firstarc;
};

// ����ģʽ
class Config
{
public:
	static Config* getInstance()
	{
		static Config instance;
		return &instance;
	}

	graph* parse(string filename);
	void buildAdjacencyMatrix();
	void buildAdjacencyList();

	inline vector<vector<int>> getMatrix() {
		return mAdjacencyMatrix;
	}
	inline vector<Vnode*> getList() {
		return mAdjacencyList;
	}
	inline const graph* getGraph() {
		return mGraph;
	}

	void setGraphWeight(bool bhaveWeight);

private:
	graph* mGraph;
	vector<int> weights;
	vector<vector<int>> mAdjacencyMatrix;
	vector<Vnode*> mAdjacencyList;

private:
	Config();
	virtual ~Config();
};

#define GETMATRIX Config::getInstance()->getMatrix()
#define GETLIST Config::getInstance()->getList()
#define GETGRAPH Config::getInstance()->getGraph()