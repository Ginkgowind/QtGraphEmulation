#pragma once

#include <vector>
#include <unordered_set>
using namespace std;

class QPushButton;
class Vnode;

class algorithm
{
public:
	static void DFS();
	static void DFS_recurPart(int n, vector<bool>& visit);
	static void BFS();
	static void HavePath();
	static void onePath();
	static void allPath();
	static vector<vector<int>> findPaths();
	static void findPaths_recurPart(int u, int start, vector<bool>& visited,
		vector<int>& path, vector<vector<int>>& result);
	static void defaultLengthPath();
	static void defaultLengthPath_recurPart(int start, int dst, int dist, 
		vector<bool>& visited, vector<int>& path, vector<vector<int>>& res);
	static void findCycles();
	static void findCycles_recurPart(int u, int end, vector<bool>& visited,
		unordered_set<int>& path, vector<vector<int>>& cycles);
	static void findnoWeightSPath();
	static void findnoWeightFnode();
	static void findnoWeightFnode_recurPart(int start, int dist, int& maxDist, int& farthestNode, vector<bool>& visited, vector<int>& path);
	static void buildMST();
	static void rebuildDS(vector<vector<int>>& matrix, vector<Vnode*>& lst);
	static void dijkstra();

public:
	static void LightNodes(int index);
	static void Mydelay(int milliseconds);
	static vector<QPushButton*> nodes;
	static vector<vector<int>> ret;
	static bool bisMatrix; //使用邻接矩阵还是邻接表
	static int start, end;
};

#define SETNODEVISITED(i)  visit[i] = true;\
							algorithm::nodes[i]->setStyleSheet(nodeLighted);\
							Mydelay(300); \
							Loginsert(QString("%1 ").arg(i))