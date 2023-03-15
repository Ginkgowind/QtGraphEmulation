#include "algorithm.h"
#include <QEventLoop>
#include <QTimer>
#include <QPushButton>
#include <QMessageBox>
#include <queue>
#include <fstream>
#include <sstream>
#include "../MainWindow.h"
#include "../Config/config.h"
#include "../selectDialog/selectPathDialog.h"
#include <set>

vector<QPushButton*> algorithm::nodes;
vector<vector<int>> algorithm::ret;
bool algorithm::bisMatrix = true;
int algorithm::start = -1;
int algorithm::end = -1;

void algorithm::DFS()
{
    if (start == -1) {
        QMessageBox::critical(nullptr, "错误提示", "你没有选择DFS起点！");
        return;
    }

    Loginsert("DFS遍历顺序为 \n");
    const int n = GETGRAPH->nodenum;
    vector<bool> visit(n, false);
    DFS_recurPart(start, visit);
    for (int i = 0; i < n; i++) {
        if (!visit[i])
            DFS_recurPart(i, visit);
    }
}

void algorithm::DFS_recurPart(int v, vector<bool>& visit)
{   
    if (visit[v] == true) return;

    SETNODEVISITED(v);
    if (bisMatrix) {
        int n = GETGRAPH->nodenum;
        for (int i = 0; i < n; i++) {
            if (GETMATRIX[v][i] == INT_MAX)
                continue;
            else if (!visit[i])
                DFS_recurPart(i, visit);
        }
    }
    else {
        ArcNode* next = GETLIST[v]->firstarc;
        while (next) {
            if (!visit[next->dst]) {
                DFS_recurPart(next->dst, visit);//递归
            }
            else {
                next = next->nextarc;
            }
        }
    }
    
}

void algorithm::BFS()
{
    if (start == -1) {
        QMessageBox::critical(nullptr, "错误提示", "你没有选择DFS起点！");
        return;
    }

    Loginsert("BFS遍历顺序为 \n");
    const int n = GETGRAPH->nodenum;
    vector<bool> visit(n, false);
    queue<int> q;
    
    for (int i = 0; i < n; i++) {
        int num = (start + i) % n;
        if (visit[num] == false) {
            SETNODEVISITED(num);
            q.push(num);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                if (bisMatrix) {    //从这里开始区分
                    for (int j = 0; j < n; j++) {
                        if (GETMATRIX[u][j] == INT_MAX)continue;
                        else if (visit[j] == false) {
                            SETNODEVISITED(j);
                            q.push(j);
                        }
                    }
                }
                else {
                    for (ArcNode* next = GETLIST[u]->firstarc; next; next = next->nextarc) {
                        SETNODEVISITED(next->dst);
                        q.push(next->dst);
                    }
                }
            }
        }
    }
}

void algorithm::HavePath()
{
    if (start == -1 || end == -1) {
        QMessageBox::critical(nullptr, "错误提示", "起点或终点选择有误！");
        return;
    }

    int n = findPaths().size();
    if (n == 0)
        Loginsert("两点之间没有路径\n");
    else
        Loginsert("两点之间有路径\n");
}

void algorithm::onePath()
{
    if (start == -1 || end == -1) {
        QMessageBox::critical(nullptr, "错误提示", "起点或终点选择有误！");
        return;
    }

    ret = findPaths();
    LightNodes(0);
}

void algorithm::allPath()
{
    if (start == -1 || end == -1) {
        QMessageBox::critical(nullptr, "错误提示", "起点或终点选择有误！");
        return;
    }

    ret = findPaths();
}

vector<vector<int>> algorithm::findPaths()
{
    vector<vector<int>> result;
    vector<int> path;
    vector<bool> visit(GETGRAPH->nodenum, false);

    // 数据结构递归里处理
    findPaths_recurPart(start, end, visit, path, result);

    return result;
}

void algorithm::findPaths_recurPart(int u, int end, vector<bool>& visited, vector<int>& path, vector<vector<int>>& result)
{
    visited[u] = true;
    path.push_back(u);

    if (u == end)
    { // 找到一条路径
        result.push_back(path);
    }
    else
    {
        if (bisMatrix) { // 使用邻接矩阵
            for (int i = 0; i < GETGRAPH->nodenum; i++)
            {
                if (GETMATRIX[u][i] != INT_MAX && !visited[i])
                {                              // 如果i和u之间有边，并且i没有被访问过
                    findPaths_recurPart(i, end, visited, path, result); // 继续搜索
                }
            }
        }
        else {          // 使用邻接表
            for (ArcNode* e = GETLIST[u]->firstarc; e != nullptr; e = e->nextarc) {
                int v = e->dst;
                if (!visited[v]) {
                    findPaths_recurPart(v, end, visited, path, result);
                }
            }
        }
    }

    // 回溯
    visited[u] = false;
    path.pop_back();
}

void algorithm::defaultLengthPath()
{
    vector<vector<int>> result;
    vector<bool> visit(GETGRAPH->nodenum, false);
    vector<int> path;
    path.push_back(start);
    // 数据结构递归里处理
    // 参数解释：此处end用于指距离值dst，param3是指当前行进的距离
    defaultLengthPath_recurPart(start, end, 0, visit, path, result);

    ret = result;
}

void algorithm::defaultLengthPath_recurPart(int start, int dst, int dist, vector<bool>& visited, vector<int>& path, vector<vector<int>>& res)
{
    if (dist == dst) { // 找到一条满足条件的路径
        res.push_back(path);
        return;
    }

    if (bisMatrix) {     // 使用邻接矩阵
        for (int i = 0; i < GETGRAPH->nodenum; i++) {
            if (GETMATRIX[start][i] != INT_MAX && !visited[i]) { // 如果start到i有边并且i没有被访问过
                visited[i] = true; // 标记i已经被访问过
                path.push_back(i); // 添加i到当前路径中
                defaultLengthPath_recurPart(i, dst, dist + GETMATRIX[start][i], visited, path, res); // 递归访问i
                visited[i] = false; // 回溯
                path.pop_back(); // 从路径中移除i
            }
        }
    }
    else {          // 使用邻接表
        for (ArcNode* e = GETLIST[start]->firstarc; e != nullptr; e = e->nextarc) {
            int v = e->dst;
            if (e->weight != INT_MAX && !visited[v]) {
                visited[v] = true; // 标记i已经被访问过
                path.push_back(v); // 添加i到当前路径中
                defaultLengthPath_recurPart(v, dst, dist + e->weight, visited, path, res);
                visited[v] = false; // 回溯
                path.pop_back(); // 从路径中移除i
            }
        }
    }
}

void algorithm::findCycles()
{
    unordered_set<int> path; // 当前搜索路径上的节点集合
    vector<vector<int>> cycles; // 存储所有的环
    vector<bool> visit(GETGRAPH->nodenum, false);
    ret = {};

    for (int i = 0; i < GETGRAPH->nodenum; i++) {
        // 数据结构递归里处理
        findCycles_recurPart(i, i, visit, path, cycles);
    }

    // 去重
    set<set<int>> s;
    for (const auto& elem : cycles)
    {
        s.insert(set<int>(elem.begin(), elem.end()));
    }
    
    for (const auto& elem : s)
    {
        vector<int> cycle;
        for (const auto& i : elem)
        {
            cycle.push_back(i);
        }
        ret.push_back(cycle);
    }

    // 点亮环
    for (int i = 0; i < ret.size(); i++) {
        Logappend(QString("第%1个环：").arg(i));
        for (int j : ret[i]) {
            Loginsert(QString("%1 ").arg(j));
            nodes[j]->setStyleSheet(nodeLighted);
            Mydelay(200);
        }
    }
}

void algorithm::findCycles_recurPart(int u, int start, vector<bool>& visited, unordered_set<int>& path, vector<vector<int>>& cycles)
{
    visited[u] = true;
    path.insert(u);

    if (bisMatrix) {    // 邻接矩阵
        for (int i = 0; i < GETGRAPH->nodenum; i++) {
            if (GETMATRIX[u][i] != INT_MAX) { // 如果i和u之间有边
                if (i == start) { // 如果i等于起点，说明找到了一个环
                    vector<int> cycle(path.begin(), path.end());
                    cycles.push_back(cycle);
                }
                else if (!visited[i] && path.count(i) == 0) { // 如果i没有被访问过，并且不在当前路径上
                    findCycles_recurPart(i, start, visited, path, cycles); // 继续搜索
                }
            }
        }
    }
    else {
        for (ArcNode* e = GETLIST[u]->firstarc; e != nullptr; e = e->nextarc) {
            int i = e->dst;
            if (i == start) { // 如果i等于起点，说明找到了一个环
                vector<int> cycle(path.begin(), path.end());
                cycles.push_back(cycle);
            }
            else if (!visited[i] && path.count(i) == 0) { // 如果i没有被访问过，并且不在当前路径上
                findCycles_recurPart(i, start, visited, path, cycles); // 继续搜索
            }
        }
    }
    
    
    // 回溯
    visited[u] = false;
    path.erase(u);
}

void algorithm::findnoWeightSPath()
{
    dijkstra();
}

void algorithm::findnoWeightFnode()
{
    vector<vector<int>> result;
    vector<bool> visit(GETGRAPH->nodenum, false);
    int maxDist = -1, farthestNode; // 记录距离最远的节点和距离
    vector<int> path;
    path.push_back(start);

    findnoWeightFnode_recurPart(start, 0, maxDist, farthestNode, visit, path);

    LightNodes(0);

    Loginsert(QString("最远顶点是：%1号节点").arg(ret[0].back()));
}

void algorithm::findnoWeightFnode_recurPart(int start, int dist, int& maxDist, int& farthestNode, vector<bool>& visited, vector<int>& path)
{
    if (dist > maxDist) { // 如果当前节点距离起点更远
        maxDist = dist; // 更新最远距离
        farthestNode = start; // 更新距离最远的节点
        ret = {};
        ret.push_back(path);
    }

    if (bisMatrix) {    // 使用邻接矩阵
        for (int i = 0; i < GETGRAPH->nodenum; i++) {
            if (GETMATRIX[start][i] != INT_MAX && !visited[i]) { // 如果start到i有边并且i没有被访问过
                visited[i] = true; // 标记i已经被访问过
                path.push_back(i); // 添加i到当前路径中
                findnoWeightFnode_recurPart(i, dist + GETMATRIX[start][i], maxDist, farthestNode, visited, path); // 递归访问i
                visited[i] = false; // 回溯，将i的访问状态设为false
                path.pop_back(); // 从路径中移除i
            }
        }
    }
    else {
        for (ArcNode* e = GETLIST[start]->firstarc; e != nullptr; e = e->nextarc) {
            int v = e->dst;
            if (e->weight != INT_MAX && !visited[v]) {
                visited[v] = true; // 标记i已经被访问过
                path.push_back(v); // 添加i到当前路径中
                findnoWeightFnode_recurPart(v, dist + e->weight, maxDist, farthestNode, visited, path);
                visited[v] = false; // 回溯
                path.pop_back(); // 从路径中移除i
            }
        }
    }
}

void algorithm::buildMST()
{
    // 先构建无向图
    vector<vector<int>> matrix;
    vector<Vnode*> lst;
    rebuildDS(matrix, lst);

    // 算法开始
    if (bisMatrix) {    // 使用邻接矩阵
        const int MAX_N = GETGRAPH->nodenum;
        vector<int> min_dist(MAX_N, INT_MAX);
        vector<int> pre(MAX_N, -1);
        vector<int> visited(MAX_N, false);
        vector<vector<int>> min_cost(MAX_N, vector<int>(MAX_N, INT_MAX));

        min_dist[0] = 0;

        for (int i = 0; i < MAX_N; i++) {
            int u = -1;
            int min_val = INT_MAX;
            for (int j = 0; j < MAX_N; j++) {
                if (!visited[j] && min_dist[j] < min_val) {
                    u = j;
                    min_val = min_dist[j];
                }
            }
            if (u == -1) return;
            visited[u] = true;

            for (int v = 0; v < MAX_N; v++) {
                if (!visited[v] && matrix[u][v] < min_dist[v]) {
                    min_dist[v] = matrix[u][v];
                    pre[v] = u;
                }
            }
        }

        for (int i = 1; i < MAX_N; i++) {
            int u = pre[i];
            int v = i;
            min_cost[u][v] = matrix[u][v];
            min_cost[v][u] = matrix[v][u];
        }

        // 输出最小生成树的边
        for (int i = 0; i < MAX_N; i++) {
            for (int j = i + 1; j < MAX_N; j++) {
                if (min_cost[i][j] != INT_MAX) {
                    Loginsert(QString("%1---%2 : %3\n").arg(i).arg(j).arg(min_cost[i][j]));
                }
            }
        }
    }
    else {      // 使用邻接表
        const int MAXN = GETGRAPH->nodenum;
        const int INF = 0x3f3f3f3f;
        vector<edge> MST(MAXN);
        vector<int> visited(MAXN, false); // 标记节点是否被加入最小生成树
        vector<int> dist(MAXN, INF); // 记录当前节点到最小生成树的距离
        vector<int> parent(MAXN, -1); // 记录当前节点在最小生成树中的父节点
        int ans = 0;
        // 任选一个节点作为起点
        dist[0] = 0;

        for (int i = 0; i < MAXN; i++) {
            int u = -1;
            int min_dist = INF;
            // 从未加入最小生成树的节点中选择距离最小的节点
            for (int j = 0; j < MAXN; j++) {
                if (!visited[j] && dist[j] < min_dist) {
                    u = j;
                    min_dist = dist[j];
                }
            }
            if (u == -1) break; // 所有未加入最小生成树的节点距离为INF，即不连通
            visited[u] = 1;
            ans += min_dist;
            if (parent[u] != -1) { // 将加入最小生成树的边记录下来
                MST[i] = edge{ parent[u], u, matrix[parent[u]][u] };
            }
            // 更新与当前节点相邻且未加入最小生成树的节点到最小生成树的距离
            for (int v = 0; v < MAXN; v++) {
                if (!visited[v] && matrix[u][v] < dist[v]) {
                    dist[v] = matrix[u][v];
                    parent[v] = u;
                }
            }
        }

        // 输出最小生成树
        for (int i = 1; i < MAXN; i++) {
            Loginsert(QString("%1---%2 : %3\n").arg(MST[i].start).arg(MST[i].end).arg(MST[i].weight));
        }
    }
}

void algorithm::rebuildDS(vector<vector<int>>& matrix, vector<Vnode*>& lst)
{
    int MSTedgenum = 2 * GETGRAPH->edgenum;
    edge* MSTedgelist = new edge[2 * GETGRAPH->edgenum];
    for (int i = 0; i < GETGRAPH->edgenum; i++) {
        MSTedgelist[i] = GETGRAPH->edgelist[i];
    }
    for (int i = GETGRAPH->edgenum; i < 2 * GETGRAPH->edgenum; i++) {
        MSTedgelist[i] = GETGRAPH->edgelist[i - GETGRAPH->edgenum];
        swap(MSTedgelist[i].start, MSTedgelist[i].end);
    }
    // 构建邻接矩阵
    matrix.resize(GETGRAPH->nodenum, vector<int>(GETGRAPH->nodenum, INT_MAX));

    for (int i = 0; i < MSTedgenum; i++) {
        int start = MSTedgelist[i].start;
        int end = MSTedgelist[i].end;
        int weight = MSTedgelist[i].weight;
        matrix[start][end] = weight;
    }

    // 构建邻接表
    Vnode* p = new Vnode{ 0,nullptr };
    lst.resize(GETGRAPH->nodenum);
    for (int i = 0; i < GETGRAPH->nodenum; i++)
        lst[i] = new Vnode{ i,nullptr };

    for (int i = 0; i < GETGRAPH->edgenum; i++) {
        int start = MSTedgelist[i].start;
        int end = MSTedgelist[i].end;
        int weight = MSTedgelist[i].weight;

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

void algorithm::dijkstra()
{
    vector<vector<int>> result;
    vector<int> path;
    vector<bool> visit(GETGRAPH->nodenum, false);
    vector<int> pre(GETGRAPH->nodenum, -1);
    vector<int> d(GETGRAPH->nodenum, INT_MAX);
    d[start] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push(make_pair(d[start], start));

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        if (visit[u]) continue;
        visit[u] = true;

        if (bisMatrix) {    // 使用邻接矩阵
            for (int v = 0; v < GETGRAPH->nodenum; v++) {
                if (GETMATRIX[u][v] != INT_MAX && !visit[v] && d[v] > d[u] + GETMATRIX[u][v]) {
                    d[v] = d[u] + GETMATRIX[u][v];
                    pre[v] = u;
                    pq.push(make_pair(d[v], v));
                }
            }
        }
        else {
            // 遍历当前节点的邻居
            for (ArcNode* e = GETLIST[u]->firstarc; e != nullptr; e = e->nextarc) {
                int v = e->dst;
                if (!visit[v]) {
                    int newDist = d[u] + e->weight;
                    if (newDist < d[v]) {
                        d[v] = newDist;
                        pre[v] = u;
                        pq.push(make_pair(newDist, v));
                    }
                }
            }
        }
    }

    if (pre[end] != -1) {
        int u = end;
        while (u != start) {
            path.push_back(u);
            u = pre[u];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }

    result.push_back(path);
    ret = result;

    LightNodes(0);
}

void algorithm::LightNodes(int index)
{
    // 先清空，再点亮
    for (int i = 0; i < GETGRAPH->nodenum; i++) {
        nodes[i]->setStyleSheet(nodeNormal);
    }
    for (int i : ret[index]) {
        nodes[i]->setStyleSheet(nodeLighted);
        Mydelay(200);
    }
}

void algorithm::Mydelay(int milliseconds)
{
    // 不阻塞Qt时间循环的delay函数
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}