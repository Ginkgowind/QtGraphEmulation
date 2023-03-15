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
        QMessageBox::critical(nullptr, "������ʾ", "��û��ѡ��DFS��㣡");
        return;
    }

    Loginsert("DFS����˳��Ϊ \n");
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
                DFS_recurPart(next->dst, visit);//�ݹ�
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
        QMessageBox::critical(nullptr, "������ʾ", "��û��ѡ��DFS��㣡");
        return;
    }

    Loginsert("BFS����˳��Ϊ \n");
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
                if (bisMatrix) {    //�����￪ʼ����
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
        QMessageBox::critical(nullptr, "������ʾ", "�����յ�ѡ������");
        return;
    }

    int n = findPaths().size();
    if (n == 0)
        Loginsert("����֮��û��·��\n");
    else
        Loginsert("����֮����·��\n");
}

void algorithm::onePath()
{
    if (start == -1 || end == -1) {
        QMessageBox::critical(nullptr, "������ʾ", "�����յ�ѡ������");
        return;
    }

    ret = findPaths();
    LightNodes(0);
}

void algorithm::allPath()
{
    if (start == -1 || end == -1) {
        QMessageBox::critical(nullptr, "������ʾ", "�����յ�ѡ������");
        return;
    }

    ret = findPaths();
}

vector<vector<int>> algorithm::findPaths()
{
    vector<vector<int>> result;
    vector<int> path;
    vector<bool> visit(GETGRAPH->nodenum, false);

    // ���ݽṹ�ݹ��ﴦ��
    findPaths_recurPart(start, end, visit, path, result);

    return result;
}

void algorithm::findPaths_recurPart(int u, int end, vector<bool>& visited, vector<int>& path, vector<vector<int>>& result)
{
    visited[u] = true;
    path.push_back(u);

    if (u == end)
    { // �ҵ�һ��·��
        result.push_back(path);
    }
    else
    {
        if (bisMatrix) { // ʹ���ڽӾ���
            for (int i = 0; i < GETGRAPH->nodenum; i++)
            {
                if (GETMATRIX[u][i] != INT_MAX && !visited[i])
                {                              // ���i��u֮���бߣ�����iû�б����ʹ�
                    findPaths_recurPart(i, end, visited, path, result); // ��������
                }
            }
        }
        else {          // ʹ���ڽӱ�
            for (ArcNode* e = GETLIST[u]->firstarc; e != nullptr; e = e->nextarc) {
                int v = e->dst;
                if (!visited[v]) {
                    findPaths_recurPart(v, end, visited, path, result);
                }
            }
        }
    }

    // ����
    visited[u] = false;
    path.pop_back();
}

void algorithm::defaultLengthPath()
{
    vector<vector<int>> result;
    vector<bool> visit(GETGRAPH->nodenum, false);
    vector<int> path;
    path.push_back(start);
    // ���ݽṹ�ݹ��ﴦ��
    // �������ͣ��˴�end����ָ����ֵdst��param3��ָ��ǰ�н��ľ���
    defaultLengthPath_recurPart(start, end, 0, visit, path, result);

    ret = result;
}

void algorithm::defaultLengthPath_recurPart(int start, int dst, int dist, vector<bool>& visited, vector<int>& path, vector<vector<int>>& res)
{
    if (dist == dst) { // �ҵ�һ������������·��
        res.push_back(path);
        return;
    }

    if (bisMatrix) {     // ʹ���ڽӾ���
        for (int i = 0; i < GETGRAPH->nodenum; i++) {
            if (GETMATRIX[start][i] != INT_MAX && !visited[i]) { // ���start��i�б߲���iû�б����ʹ�
                visited[i] = true; // ���i�Ѿ������ʹ�
                path.push_back(i); // ���i����ǰ·����
                defaultLengthPath_recurPart(i, dst, dist + GETMATRIX[start][i], visited, path, res); // �ݹ����i
                visited[i] = false; // ����
                path.pop_back(); // ��·�����Ƴ�i
            }
        }
    }
    else {          // ʹ���ڽӱ�
        for (ArcNode* e = GETLIST[start]->firstarc; e != nullptr; e = e->nextarc) {
            int v = e->dst;
            if (e->weight != INT_MAX && !visited[v]) {
                visited[v] = true; // ���i�Ѿ������ʹ�
                path.push_back(v); // ���i����ǰ·����
                defaultLengthPath_recurPart(v, dst, dist + e->weight, visited, path, res);
                visited[v] = false; // ����
                path.pop_back(); // ��·�����Ƴ�i
            }
        }
    }
}

void algorithm::findCycles()
{
    unordered_set<int> path; // ��ǰ����·���ϵĽڵ㼯��
    vector<vector<int>> cycles; // �洢���еĻ�
    vector<bool> visit(GETGRAPH->nodenum, false);
    ret = {};

    for (int i = 0; i < GETGRAPH->nodenum; i++) {
        // ���ݽṹ�ݹ��ﴦ��
        findCycles_recurPart(i, i, visit, path, cycles);
    }

    // ȥ��
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

    // ������
    for (int i = 0; i < ret.size(); i++) {
        Logappend(QString("��%1������").arg(i));
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

    if (bisMatrix) {    // �ڽӾ���
        for (int i = 0; i < GETGRAPH->nodenum; i++) {
            if (GETMATRIX[u][i] != INT_MAX) { // ���i��u֮���б�
                if (i == start) { // ���i������㣬˵���ҵ���һ����
                    vector<int> cycle(path.begin(), path.end());
                    cycles.push_back(cycle);
                }
                else if (!visited[i] && path.count(i) == 0) { // ���iû�б����ʹ������Ҳ��ڵ�ǰ·����
                    findCycles_recurPart(i, start, visited, path, cycles); // ��������
                }
            }
        }
    }
    else {
        for (ArcNode* e = GETLIST[u]->firstarc; e != nullptr; e = e->nextarc) {
            int i = e->dst;
            if (i == start) { // ���i������㣬˵���ҵ���һ����
                vector<int> cycle(path.begin(), path.end());
                cycles.push_back(cycle);
            }
            else if (!visited[i] && path.count(i) == 0) { // ���iû�б����ʹ������Ҳ��ڵ�ǰ·����
                findCycles_recurPart(i, start, visited, path, cycles); // ��������
            }
        }
    }
    
    
    // ����
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
    int maxDist = -1, farthestNode; // ��¼������Զ�Ľڵ�;���
    vector<int> path;
    path.push_back(start);

    findnoWeightFnode_recurPart(start, 0, maxDist, farthestNode, visit, path);

    LightNodes(0);

    Loginsert(QString("��Զ�����ǣ�%1�Žڵ�").arg(ret[0].back()));
}

void algorithm::findnoWeightFnode_recurPart(int start, int dist, int& maxDist, int& farthestNode, vector<bool>& visited, vector<int>& path)
{
    if (dist > maxDist) { // �����ǰ�ڵ��������Զ
        maxDist = dist; // ������Զ����
        farthestNode = start; // ���¾�����Զ�Ľڵ�
        ret = {};
        ret.push_back(path);
    }

    if (bisMatrix) {    // ʹ���ڽӾ���
        for (int i = 0; i < GETGRAPH->nodenum; i++) {
            if (GETMATRIX[start][i] != INT_MAX && !visited[i]) { // ���start��i�б߲���iû�б����ʹ�
                visited[i] = true; // ���i�Ѿ������ʹ�
                path.push_back(i); // ���i����ǰ·����
                findnoWeightFnode_recurPart(i, dist + GETMATRIX[start][i], maxDist, farthestNode, visited, path); // �ݹ����i
                visited[i] = false; // ���ݣ���i�ķ���״̬��Ϊfalse
                path.pop_back(); // ��·�����Ƴ�i
            }
        }
    }
    else {
        for (ArcNode* e = GETLIST[start]->firstarc; e != nullptr; e = e->nextarc) {
            int v = e->dst;
            if (e->weight != INT_MAX && !visited[v]) {
                visited[v] = true; // ���i�Ѿ������ʹ�
                path.push_back(v); // ���i����ǰ·����
                findnoWeightFnode_recurPart(v, dist + e->weight, maxDist, farthestNode, visited, path);
                visited[v] = false; // ����
                path.pop_back(); // ��·�����Ƴ�i
            }
        }
    }
}

void algorithm::buildMST()
{
    // �ȹ�������ͼ
    vector<vector<int>> matrix;
    vector<Vnode*> lst;
    rebuildDS(matrix, lst);

    // �㷨��ʼ
    if (bisMatrix) {    // ʹ���ڽӾ���
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

        // �����С�������ı�
        for (int i = 0; i < MAX_N; i++) {
            for (int j = i + 1; j < MAX_N; j++) {
                if (min_cost[i][j] != INT_MAX) {
                    Loginsert(QString("%1---%2 : %3\n").arg(i).arg(j).arg(min_cost[i][j]));
                }
            }
        }
    }
    else {      // ʹ���ڽӱ�
        const int MAXN = GETGRAPH->nodenum;
        const int INF = 0x3f3f3f3f;
        vector<edge> MST(MAXN);
        vector<int> visited(MAXN, false); // ��ǽڵ��Ƿ񱻼�����С������
        vector<int> dist(MAXN, INF); // ��¼��ǰ�ڵ㵽��С�������ľ���
        vector<int> parent(MAXN, -1); // ��¼��ǰ�ڵ�����С�������еĸ��ڵ�
        int ans = 0;
        // ��ѡһ���ڵ���Ϊ���
        dist[0] = 0;

        for (int i = 0; i < MAXN; i++) {
            int u = -1;
            int min_dist = INF;
            // ��δ������С�������Ľڵ���ѡ�������С�Ľڵ�
            for (int j = 0; j < MAXN; j++) {
                if (!visited[j] && dist[j] < min_dist) {
                    u = j;
                    min_dist = dist[j];
                }
            }
            if (u == -1) break; // ����δ������С�������Ľڵ����ΪINF��������ͨ
            visited[u] = 1;
            ans += min_dist;
            if (parent[u] != -1) { // ��������С�������ı߼�¼����
                MST[i] = edge{ parent[u], u, matrix[parent[u]][u] };
            }
            // �����뵱ǰ�ڵ�������δ������С�������Ľڵ㵽��С�������ľ���
            for (int v = 0; v < MAXN; v++) {
                if (!visited[v] && matrix[u][v] < dist[v]) {
                    dist[v] = matrix[u][v];
                    parent[v] = u;
                }
            }
        }

        // �����С������
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
    // �����ڽӾ���
    matrix.resize(GETGRAPH->nodenum, vector<int>(GETGRAPH->nodenum, INT_MAX));

    for (int i = 0; i < MSTedgenum; i++) {
        int start = MSTedgelist[i].start;
        int end = MSTedgelist[i].end;
        int weight = MSTedgelist[i].weight;
        matrix[start][end] = weight;
    }

    // �����ڽӱ�
    Vnode* p = new Vnode{ 0,nullptr };
    lst.resize(GETGRAPH->nodenum);
    for (int i = 0; i < GETGRAPH->nodenum; i++)
        lst[i] = new Vnode{ i,nullptr };

    for (int i = 0; i < GETGRAPH->edgenum; i++) {
        int start = MSTedgelist[i].start;
        int end = MSTedgelist[i].end;
        int weight = MSTedgelist[i].weight;

        // �����½ڵ�
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

        if (bisMatrix) {    // ʹ���ڽӾ���
            for (int v = 0; v < GETGRAPH->nodenum; v++) {
                if (GETMATRIX[u][v] != INT_MAX && !visit[v] && d[v] > d[u] + GETMATRIX[u][v]) {
                    d[v] = d[u] + GETMATRIX[u][v];
                    pre[v] = u;
                    pq.push(make_pair(d[v], v));
                }
            }
        }
        else {
            // ������ǰ�ڵ���ھ�
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
    // ����գ��ٵ���
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
    // ������Qtʱ��ѭ����delay����
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}