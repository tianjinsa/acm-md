# ACM竞赛常用算法详解（初学者友好版）

## 目录
1. [基础数据结构](#基础数据结构)
2. [图论算法](#图论算法)
3. [字符串算法](#字符串算法)
4. [动态规划](#动态规划)
5. [数学方法](#数学方法)
6. [计算几何](#计算几何)
7. [高级数据结构](#高级数据结构)
8. [搜索技术](#搜索技术)

## 基础数据结构

### 1. 并查集
**基本原理**: 
并查集是一种用于处理"元素分组"问题的数据结构。它主要支持两种操作：
- 查询(Find)：确定某个元素属于哪个组
- 合并(Union)：将两个组合并成一个组

想象一下，你有一群人，你想知道谁和谁是朋友关系。每次你发现两个人是朋友，你就记录下来，而且朋友的朋友也是朋友。并查集就是用来高效管理这种关系的。

**实现代码**:
```cpp
// MAXN是最大元素数量
int fa[MAXN]; 

// 初始化：每个元素开始时都是独立的集合（自己的父节点是自己）
void init(int n) {
    for (int i = 1; i <= n; i++) fa[i] = i;
}

// 查找根节点（代表元素）：使用路径压缩优化
// 路径压缩：在查找的过程中，将沿途的每个节点都直接连到根节点，减少后续查询的路径长度
int find(int x) {
    // 如果x是根节点，返回x；否则递归查找x的父节点，并更新x的父节点为根节点
    return x == fa[x] ? x : (fa[x] = find(fa[x]));
}

// 合并两个集合：将一个集合的根节点指向另一个集合的根节点
void unite(int x, int y) {
    // 先找到x和y所在集合的根节点
    fa[find(x)] = find(y);  // 将x的根节点指向y的根节点
}
```

**应用场景**:
- **判断图的连通性**：可以确定任意两点是否连通
- **Kruskal最小生成树算法**：用于判断加入一条边是否会形成环
- **动态连通性问题**：维护网络中的连通关系
- **好友关系网络**：管理社交网络中的朋友圈

### 2. 线段树
**基本原理**: 
线段树是一种二叉树形数据结构，用于高效处理区间查询和修改操作。每个节点代表一个区间，父节点的区间是子节点区间的并集。

想象你有一排数字需要频繁地进行区间求和或查找最大/最小值，线段树可以让这些操作变得高效。

**实现代码（含详细注释）**:
```cpp
struct SegTree {
    int n;                // 数组长度
    vector<int> sum;      // 存储区间和
    vector<int> lazy;     // 懒标记，延迟更新
    
    // 构建线段树
    // node: 当前节点编号，通常从1开始
    // start, end: 当前节点表示的区间范围
    // arr: 原始数组
    void build(int node, int start, int end, vector<int>& arr) {
        // 如果是叶子节点（表示单个元素）
        if (start == end) {
            sum[node] = arr[start];  // 直接赋值
            return;
        }
        
        // 找到中间点，将区间分为左右两部分
        int mid = (start + end) / 2;
        
        // 递归构建左子树（左半区间）
        build(node * 2, start, mid, arr);
        
        // 递归构建右子树（右半区间）
        build(node * 2 + 1, mid + 1, end, arr);
        
        // 当前节点的值 = 左子节点的值 + 右子节点的值
        sum[node] = sum[node * 2] + sum[node * 2 + 1];
    }
    
    // 区间更新操作
    // node: 当前节点编号
    // start, end: 当前节点的区间范围
    // l, r: 要更新的区间范围
    // val: 更新的值（这里是加上val）
    void update(int node, int start, int end, int l, int r, int val) {
        // 先处理懒标记：如果当前节点有未传递的更新
        if (lazy[node] != 0) {
            // 更新当前节点的值：区间长度 * 懒标记值
            sum[node] += (end - start + 1) * lazy[node];
            
            // 如果不是叶子节点，则将懒标记传递给子节点
            if (start != end) {
                lazy[node * 2] += lazy[node];         // 传给左子节点
                lazy[node * 2 + 1] += lazy[node];     // 传给右子节点
            }
            
            // 清除当前节点的懒标记
            lazy[node] = 0;
        }
        
        // 如果当前区间完全在目标区间外，直接返回
        if (start > r || end < l) return;
        
        // 如果当前区间完全包含在目标区间内
        if (l <= start && end <= r) {
            // 更新当前节点的值
            sum[node] += (end - start + 1) * val;
            
            // 如果不是叶子节点，设置子节点的懒标记
            if (start != end) {
                lazy[node * 2] += val;
                lazy[node * 2 + 1] += val;
            }
            return;
        }
        
        // 如果区间有部分重叠，则分别处理左右子区间
        int mid = (start + end) / 2;
        update(node * 2, start, mid, l, r, val);          // 更新左子树
        update(node * 2 + 1, mid + 1, end, l, r, val);    // 更新右子树
        
        // 更新当前节点的值为子节点值的和
        sum[node] = sum[node * 2] + sum[node * 2 + 1];
    }
    
    // 区间查询操作
    // 参数同update，返回l到r区间的和
    int query(int node, int start, int end, int l, int r) {
        // 如果当前区间在查询区间外
        if (start > r || end < l) return 0;
        
        // 处理懒标记
        if (lazy[node] != 0) {
            sum[node] += (end - start + 1) * lazy[node];
            
            if (start != end) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            
            lazy[node] = 0;
        }
        
        // 如果当前区间完全包含在查询区间内
        if (l <= start && end <= r) return sum[node];
        
        // 部分重叠，递归查询左右子区间
        int mid = (start + end) / 2;
        int p1 = query(node * 2, start, mid, l, r);       // 查询左子树
        int p2 = query(node * 2 + 1, mid + 1, end, l, r); // 查询右子树
        return p1 + p2;  // 返回左右子树查询结果的和
    }
};
```

**应用场景**:
- **区间求和**：快速计算一段区间内所有元素的和
- **区间最大/最小值**：查询区间内的最值
- **区间更新**：对一段区间内的所有元素进行同一操作（如加上一个值）
- **动态维护数组**：当数组频繁变动且需要频繁查询区间信息时

### 3. 单调队列
**基本原理**：
单调队列是一种特殊的队列，其中的元素保持单调递增或递减顺序。它主要用于解决滑动窗口类问题，特别是需要找出窗口内最大/最小值的问题。

**实现代码**：
```cpp
// 以求滑动窗口最大值为例
vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    vector<int> result;
    deque<int> q;  // 存储下标，而不是值
    
    for (int i = 0; i < nums.size(); i++) {
        // 移除队首超出窗口范围的元素（下标小于i-k+1的）
        while (!q.empty() && q.front() < i - k + 1) {
            q.pop_front();
        }
        
        // 从队尾移除所有小于当前元素的值（维持单调递减）
        while (!q.empty() && nums[q.back()] < nums[i]) {
            q.pop_back();
        }
        
        // 将当前元素的下标加入队列
        q.push_back(i);
        
        // 当窗口形成后，记录当前窗口的最大值（队首元素）
        if (i >= k - 1) {
            result.push_back(nums[q.front()]);
        }
    }
    
    return result;
}
```

**应用场景**：
- **滑动窗口最大/最小值**：O(n)时间复杂度内求解
- **单调性优化问题**：如凸包问题中的单调栈应用
- **某些DP优化**：利用单调性减少遍历范围

### 4. 栈与队列的基础应用
**栈的应用**：后进先出(LIFO)
```cpp
// 检查括号是否匹配
bool isValid(string s) {
    stack<char> st;
    
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            // 左括号直接入栈
            st.push(c);
        } else {
            // 右括号需要检查是否匹配
            if (st.empty()) return false;  // 栈空但出现右括号，不匹配
            
            char top = st.top();
            if ((c == ')' && top == '(') || 
                (c == ']' && top == '[') || 
                (c == '}' && top == '{')) {
                st.pop();  // 匹配成功，弹出左括号
            } else {
                return false;  // 不匹配
            }
        }
    }
    
    return st.empty();  // 栈为空表示所有括号都匹配
}
```

**队列的应用**：先进先出(FIFO)
```cpp
// 广度优先搜索的示例用法
void bfs(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;
    
    // 初始节点
    q.push(start);
    visited[start] = true;
    
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        cout << "访问节点: " << curr << endl;
        
        // 访问相邻节点
        for (int next : graph[curr]) {
            if (!visited[next]) {
                q.push(next);
                visited[next] = true;
            }
        }
    }
}
```

## 图论算法

### 1. 最短路径
#### Dijkstra算法
**基本原理**: 
Dijkstra算法用于计算从一个顶点到其他所有顶点的最短路径，适用于边权为非负的有向或无向图。

该算法的核心思想是贪心：每次选择还未确定最短路径的点中，路径估计值最小的点进行扩展。

**实现代码**:
```cpp
// 使用优先队列优化的Dijkstra算法
// graph[u] 存储顶点u的所有邻接点及边权，格式为pair<顶点,权重>
// dist数组存储从起点到各点的最短距离
void dijkstra(int start, vector<vector<pair<int, int>>>& graph, vector<int>& dist) {
    int n = graph.size();  // 顶点数量
    dist.assign(n, INT_MAX);  // 初始化距离为无穷大
    dist[start] = 0;  // 起点到自身的距离为0
    
    // 优先队列，按距离从小到大排序，存储pair<距离,顶点>
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.push({0, start});  // 加入起点
    
    while (!pq.empty()) {
        int d = pq.top().first;    // 当前最短距离
        int u = pq.top().second;   // 当前顶点
        pq.pop();
        
        // 如果已经找到更短的路径，跳过
        if (d > dist[u]) continue;
        
        // 遍历所有邻接点
        for (auto& edge : graph[u]) {
            int v = edge.first;      // 邻接点
            int weight = edge.second; // 边权
            
            // 如果通过u到达v的路径更短，则更新距离
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});  // 加入新的路径
            }
        }
    }
    // 最终dist数组包含从start到所有点的最短路径长度
}
```

**应用场景**:
- **导航系统**：计算从起点到终点的最短路径
- **网络路由**：数据包的最短路径路由
- **任意带权图的单源最短路径问题**

#### Floyd-Warshall算法
**基本原理**: 
Floyd-Warshall算法用于计算图中所有顶点对之间的最短路径，适用于任何没有负环的图（包含负权边的情况）。

该算法通过动态规划的方式，逐步考虑是否通过中间点k来改善i到j的路径。

**实现代码**:
```cpp
// dist[i][j]表示从顶点i到顶点j的最短路径长度
// 初始时，dist[i][j]为边(i,j)的权重，若不存在则为无穷大，dist[i][i]=0
void floyd(vector<vector<int>>& dist) {
    int n = dist.size();  // 顶点数量
    
    // k是中间点
    for (int k = 0; k < n; k++) {
        // i是起点
        for (int i = 0; i < n; i++) {
            // j是终点
            for (int j = 0; j < n; j++) {
                // 如果通过k能使i到j的路径更短，则更新
                // 需要检查避免溢出
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    // 最终dist[i][j]包含从i到j的最短路径长度
}
```

**应用场景**:
- **所有点对最短路径**：需要计算图中任意两点间的最短距离
- **传递闭包**：判断从任意点i是否能到达点j
- **检测负环**：如果dist[i][i]<0，则图中存在负环

### 2. 最小生成树
#### Kruskal算法
**基本原理**: 
Kruskal算法是基于贪心策略的最小生成树算法，它从小到大遍历所有边，每次加入不会形成环的最小边。通过并查集判断是否会形成环。

**实现代码**:
```cpp
// 边结构体
struct Edge {
    int u, v, weight;  // 起点，终点，权重
    
    // 按权重升序排序
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Kruskal最小生成树算法
// 返回最小生成树的总权重，n为顶点数，edges为所有边
int kruskal(int n, vector<Edge>& edges) {
    // 对边按权重排序
    sort(edges.begin(), edges.end());
    
    int cost = 0;  // 最小生成树的总权重
    vector<int> parent(n + 1);  // 并查集父节点数组
    
    // 初始化并查集
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }
    
    // 并查集的查找函数（带路径压缩）
    auto find = [&](int x) -> int {
        return x == parent[x] ? x : (parent[x] = find(parent[x]));
    };
    
    // 遍历所有边
    for (Edge& e : edges) {
        int u = find(e.u);  // 边的起点所在集合
        int v = find(e.v);  // 边的终点所在集合
        
        // 如果不在同一集合（不会形成环）
        if (u != v) {
            parent[u] = v;  // 合并集合
            cost += e.weight;  // 将边的权重加入总成本
        }
    }
    
    return cost;  // 返回最小生成树的总权重
}
```

**应用场景**:
- **网络设计**：最小成本连接所有节点
- **电路设计**：最小化连线总长度
- **聚类**：通过删除最小生成树的较长边进行聚类

#### Prim算法
**基本原理**:
Prim算法也是求最小生成树的算法，但与Kruskal不同，它从一个顶点开始，每次选择一个与当前树相连的最小权重边。

**实现代码**:
```cpp
// Prim算法求最小生成树
// graph[i] 是顶点i的所有邻接点及边权，格式为pair<顶点,权重>
int prim(vector<vector<pair<int, int>>>& graph) {
    int n = graph.size();
    vector<bool> visited(n, false);  // 记录顶点是否在树中
    vector<int> minEdge(n, INT_MAX);  // 从树到每个顶点的最小边权
    
    int totalWeight = 0;  // 最小生成树的总权重
    minEdge[0] = 0;  // 选择顶点0作为起点
    
    for (int i = 0; i < n; i++) {
        // 找到还未加入树的顶点中，到树距离最小的顶点
        int u = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (u == -1 || minEdge[j] < minEdge[u])) {
                u = j;
            }
        }
        
        // 将顶点u加入树
        visited[u] = true;
        totalWeight += minEdge[u];
        
        // 更新到其他顶点的最小距离
        for (auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;
            
            if (!visited[v] && weight < minEdge[v]) {
                minEdge[v] = weight;  // 更新到v的最小距离
            }
        }
    }
    
    return totalWeight;
}
```

**应用场景**:
- 与Kruskal算法类似，但当图较稠密时更为高效
- 实时网络的构建（因为可以增量式地构建树）

### 3. 拓扑排序
**基本原理**: 
拓扑排序是对有向无环图(DAG)的所有顶点进行线性排序，使得图中任何一对顶点(u,v)，若存在边u->v，则排序中u必须在v之前。

**实现代码**:
```cpp
// graph[i] 是顶点i的所有出边（后继顶点）
vector<int> topologicalSort(vector<vector<int>>& graph) {
    int n = graph.size();  // 顶点数量
    vector<int> inDegree(n, 0);  // 记录每个顶点的入度
    
    // 计算每个顶点的入度
    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            inDegree[v]++;  // u->v，所以v的入度加1
        }
    }
    
    // 将所有入度为0的顶点加入队列（初始可以直接处理的顶点）
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }
    
    vector<int> result;  // 存储拓扑排序结果
    
    // BFS过程
    while (!q.empty()) {
        int u = q.front();  // 取出一个入度为0的顶点
        q.pop();
        result.push_back(u);  // 加入排序结果
        
        // 将所有u的后继顶点的入度减1
        for (int v : graph[u]) {
            if (--inDegree[v] == 0) {  // 如果v的入度变为0，则加入队列
                q.push(v);
            }
        }
    }
    
    // 如果无法完成所有顶点的排序，说明图中有环
    if (result.size() != n) {
        return {};  // 返回空数组表示存在环
    }
    
    return result;  // 返回拓扑排序结果
}
```

**应用场景**:
- **任务调度**：确定依赖任务的执行顺序
- **编译顺序**：确定代码文件的编译顺序
- **课程安排**：确定先修课程和后修课程的顺序

### 4. 深度优先搜索 (DFS)
**基本原理**:
深度优先搜索是一种图遍历算法，它尽可能深地搜索树或图的分支，当无法继续前进时回溯。

**实现代码**:
```cpp
// 图的邻接表表示：graph[i] 表示顶点i的所有邻接点
void dfs(vector<vector<int>>& graph, int start) {
    int n = graph.size();  // 顶点数
    vector<bool> visited(n, false);  // 记录顶点是否被访问
    
    // 递归DFS函数
    function<void(int)> dfsRecursive = [&](int u) {
        // 标记当前顶点为已访问
        visited[u] = true;
        cout << "访问顶点: " << u << endl;  // 处理当前顶点
        
        // 递归访问所有未访问的邻接顶点
        for (int v : graph[u]) {
            if (!visited[v]) {
                dfsRecursive(v);
            }
        }
    };
    
    // 从起点开始DFS
    dfsRecursive(start);
    
    // 如果图不连通，处理剩余的连通分量
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            dfsRecursive(i);
        }
    }
}

// 非递归实现的DFS，使用栈
void dfsIterative(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<int> s;
    
    s.push(start);
    visited[start] = true;
    cout << "访问顶点: " << start << endl;
    
    while (!s.empty()) {
        int u = s.top();
        
        // 寻找未访问的邻接点
        bool foundUnvisited = false;
        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                cout << "访问顶点: " << v << endl;
                s.push(v);
                foundUnvisited = true;
                break;
            }
        }
        
        // 如果没有未访问的邻接点，弹出栈顶
        if (!foundUnvisited) {
            s.pop();
        }
    }
}
```

**应用场景**:
- **路径搜索**：寻找从一个点到另一个点的路径
- **连通分量**：找出图中的所有连通分量
- **拓扑排序**：通过DFS可以实现拓扑排序
- **环检测**：可以用来检测图中是否存在环

### 5. 广度优先搜索 (BFS)
**基本原理**:
广度优先搜索是一种层次遍历算法，它首先访问起点的所有邻接点，然后再访问这些邻接点的所有邻接点，依此类推。

**实现代码**:
```cpp
// 图的邻接表表示：graph[i] 表示顶点i的所有邻接点
void bfs(vector<vector<int>>& graph, int start) {
    int n = graph.size();  // 顶点数
    vector<bool> visited(n, false);  // 记录顶点是否被访问
    queue<int> q;  // BFS使用队列
    
    // 将起点加入队列并标记为已访问
    q.push(start);
    visited[start] = true;
    
    // 进行BFS
    while (!q.empty()) {
        int u = q.front();  // 取出队首顶点
        q.pop();
        
        cout << "访问顶点: " << u << endl;  // 处理当前顶点
        
        // 访问所有未访问的邻接顶点
        for (int v : graph[u]) {
            if (!visited[v]) {
                q.push(v);  // 将顶点加入队列
                visited[v] = true;  // 标记为已访问
            }
        }
    }
    
    // 如果图不连通，处理剩余的连通分量
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            q.push(i);
            visited[i] = true;
            
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                
                cout << "访问顶点: " << u << endl;
                
                for (int v : graph[u]) {
                    if (!visited[v]) {
                        q.push(v);
                        visited[v] = true;
                    }
                }
            }
        }
    }
}

// 计算从起点到各点的最短路径长度（无权图）
vector<int> bfsShortestPath(vector<vector<int>>& graph, int start) {
    int n = graph.size();
    vector<int> distance(n, -1);  // -1表示未访问
    queue<int> q;
    
    distance[start] = 0;  // 起点到自己的距离为0
    q.push(start);
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        
        for (int v : graph[u]) {
            if (distance[v] == -1) {  // 如果v未被访问
                distance[v] = distance[u] + 1;  // 更新距离
                q.push(v);  // 加入队列
            }
        }
    }
    
    return distance;  // 返回从起点到各点的最短路径长度
}
```

**应用场景**:
- **最短路径**：无权图中从起点到各点的最短路径
- **连通分量**：找出图中的所有连通分量
- **层次遍历**：按层次访问图中的节点
- **环检测**：可以用来检测图中是否存在环

## 字符串算法

### 1. KMP算法
**基本原理**: 
KMP算法是一种高效的字符串匹配算法，通过预处理模式串，避免在匹配失败时的重复比较。

**实现代码**:
```cpp
// 计算模式串的部分匹配表（LPS数组）
vector<int> computeLPS(string pattern) {
    int m = pattern.size();
    vector<int> lps(m, 0);
    for (int i = 1, len = 0; i < m; ) {
        if (pattern[i] == pattern[len]) {
            lps[i++] = ++len;
        } else if (len > 0) {
            len = lps[len - 1];
        } else {
            lps[i++] = 0;
        }
    }
    return lps;
}

// KMP字符串匹配算法
vector<int> kmp(string text, string pattern) {
    vector<int> lps = computeLPS(pattern);
    vector<int> positions;
    int n = text.size(), m = pattern.size();
    
    for (int i = 0, j = 0; i < n; ) {
        if (text[i] == pattern[j]) {
            i++; j++;
        }
        
        if (j == m) {
            positions.push_back(i - j);
            j = lps[j - 1];
        } else if (i < n && text[i] != pattern[j]) {
            if (j > 0) j = lps[j - 1];
            else i++;
        }
    }
    
    return positions;
}
```

**应用场景**:
- **字符串匹配**：在文本中查找模式串出现的位置
- **文本编辑器**：实现查找和替换功能
- **生物信息学**：DNA序列比对

### 2. 字典树 (Trie)
**基本原理**: 
字典树是一种用于高效存储和检索字符串集合的树形数据结构，每个节点代表一个字符。

**实现代码**:
```cpp
struct TrieNode {
    TrieNode* children[26];
    bool isEnd;
    
    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
        isEnd = false;
    }
};

class Trie {
private:
    TrieNode* root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    // 插入单词
    void insert(string word) {
        TrieNode* current = root;
        for (char c : word) {
            int index = c - 'a';
            if (!current->children[index]) {
                current->children[index] = new TrieNode();
            }
            current = current->children[index];
        }
        current->isEnd = true;
    }
    
    // 查找单词
    bool search(string word) {
        TrieNode* node = findNode(word);
        return node && node->isEnd;
    }
    
    // 查找前缀
    bool startsWith(string prefix) {
        return findNode(prefix) != nullptr;
    }
    
private:
    TrieNode* findNode(string key) {
        TrieNode* current = root;
        for (char c : key) {
            int index = c - 'a';
            if (!current->children[index]) {
                return nullptr;
            }
            current = current->children[index];
        }
        return current;
    }
};
```

**应用场景**:
- **前缀匹配**：查找具有相同前缀的单词
- **自动补全**：实现输入法的自动补全功能
- **拼写检查**：快速查找单词是否存在

## 动态规划

### 1. 经典问题及解法

#### 背包问题
**0-1背包**:
```cpp
vector<int> dp(capacity + 1, 0);
for (int i = 0; i < n; i++) {
    for (int j = capacity; j >= weights[i]; j--) {
        dp[j] = max(dp[j], dp[j - weights[i]] + values[i]);
    }
}
```

**完全背包**:
```cpp
vector<int> dp(capacity + 1, 0);
for (int i = 0; i < n; i++) {
    for (int j = weights[i]; j <= capacity; j++) {
        dp[j] = max(dp[j], dp[j - weights[i]] + values[i]);
    }
}
```

#### 最长公共子序列 (LCS)
```cpp
int lcs(string s1, string s2) {
    int m = s1.size(), n = s2.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    
    return dp[m][n];
}
```

#### 最长递增子序列 (LIS)
```cpp
int lis(vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n, 1);
    
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (nums[i] > nums[j]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
    }
    
    return *max_element(dp.begin(), dp.end());
}
```

### 2. 状态优化技巧
- **状态压缩**: 使用二进制表示集合状态，减少维度
- **滚动数组**: 优化空间复杂度
- **斜率优化**: 用于优化特定形式的DP转移方程

## 数学方法

### 1. 素数筛法
**埃拉托斯特尼筛法**:
```cpp
vector<bool> sieve(int n) {
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    
    for (int i = 2; i * i <= n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }
    
    return isPrime;
}
```

### 2. 快速幂
```cpp
long long fastPow(long long base, long long exponent, long long mod) {
    long long result = 1;
    base %= mod;
    while (exponent > 0) {
        if (exponent & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exponent >>= 1;
    }
    return result;
}
```

### 3. 组合数学
**组合数计算**:
```cpp
vector<vector<long long>> combination(int n, int mod) {
    vector<vector<long long>> C(n + 1, vector<long long>(n + 1, 0));
    for (int i = 0; i <= n; i++) {
        C[i][0] = 1;
        for (int j = 1; j <= i; j++) {
            C[i][j] = (C[i-1][j-1] + C[i-1][j]) % mod;
        }
    }
    return C;
}
```

## 计算几何

### 1. 基础概念与操作
```cpp
struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

// 计算两点间距离
double distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// 计算叉积
double cross(Point o, Point a, Point b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

// 判断点是否在多边形内
bool isPointInPolygon(Point p, const vector<Point>& polygon) {
    int n = polygon.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if ((polygon[i].y > p.y) != (polygon[j].y > p.y) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}
```

### 2. 凸包算法 (Graham扫描法)
```cpp
vector<Point> grahamScan(vector<Point> points) {
    if (points.size() <= 3) return points;
    
    // 找到y坐标最小的点
    int n = points.size();
    swap(points[0], *min_element(points.begin(), points.end(), [](Point a, Point b) {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
    }));
    
    // 按极角排序
    Point p0 = points[0];
    sort(points.begin() + 1, points.end(), [&p0](Point a, Point b) {
        double cross = (a.x - p0.x) * (b.y - p0.y) - (b.x - p0.x) * (a.y - p0.y);
        if (fabs(cross) < 1e-9) return distance(p0, a) < distance(p0, b);
        return cross > 0;
    });
    
    // Graham扫描
    vector<Point> hull;
    hull.push_back(points[0]);
    hull.push_back(points[1]);
    
    for (int i = 2; i < n; i++) {
        while (hull.size() > 1 && cross(hull[hull.size() - 2], hull.back(), points[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(points[i]);
    }
    
    return hull;
}
```

## 高级数据结构

### 1. 树状数组 (Binary Indexed Tree)
```cpp
struct BIT {
    vector<int> tree;
    int n;
    
    BIT(int size) {
        n = size;
        tree.resize(n + 1);
    }
    
    void update(int i, int val) {
        while (i <= n) {
            tree[i] += val;
            i += i & (-i);
        }
    }
    
    int query(int i) {
        int sum = 0;
        while (i > 0) {
            sum += tree[i];
            i -= i & (-i);
        }
        return sum;
    }
    
    int rangeSum(int l, int r) {
        return query(r) - query(l - 1);
    }
};
```

### 2. ST表 (用于RMQ问题)
```cpp
vector<vector<int>> buildSparseTable(const vector<int>& arr) {
    int n = arr.size();
    int logN = log2(n) + 1;
    vector<vector<int>> st(n, vector<int>(logN));
    
    for (int i = 0; i < n; i++) {
        st[i][0] = arr[i];
    }
    
    for (int j = 1; j < logN; j++) {
        for (int i = 0; i + (1 << j) - 1 < n; i++) {
            st[i][j] = min(st[i][j-1], st[i + (1 << (j-1))][j-1]);
        }
    }
    
    return st;
}

int queryMin(const vector<vector<int>>& st, int L, int R) {
    int j = log2(R - L + 1);
    return min(st[L][j], st[R - (1 << j) + 1][j]);
}
```

## 搜索技术

### 1. A*搜索
```cpp
typedef pair<int, int> State; // (cost + heuristic, node)

int astar(vector<vector<int>>& graph, int start, int goal, function<int(int, int)> heuristic) {
    priority_queue<State, vector<State>, greater<>> pq;
    vector<int> cost(graph.size(), INT_MAX);
    
    pq.push({0 + heuristic(start, goal), start});
    cost[start] = 0;
    
    while (!pq.empty()) {
        int f = pq.top().first;
        int node = pq.top().second;
        pq.pop();
        
        if (node == goal) return cost[goal];
        
        if (f - heuristic(node, goal) > cost[node]) continue;
        
        for (int i = 0; i < graph[node].size(); i++) {
            int next = graph[node][i];
            int newCost = cost[node] + 1; // 假设边权为1
            
            if (newCost < cost[next]) {
                cost[next] = newCost;
                pq.push({newCost + heuristic(next, goal), next});
            }
        }
    }
    
    return -1; // 未找到路径
}
```

### 2. 迭代加深搜索 (IDA*)
```cpp
bool dfs(int node, int depth, int limit, vector<vector<int>>& graph, vector<bool>& visited) {
    if (depth > limit) return false;
    if (isTarget(node)) return true;
    
    visited[node] = true;
    for (int next : graph[node]) {
        if (!visited[next]) {
            if (dfs(next, depth + 1, limit, graph, visited)) {
                return true;
            }
        }
    }
    visited[node] = false;
    return false;
}

int idaStar(vector<vector<int>>& graph, int start) {
    int limit = 0;
    while (limit < MAX_DEPTH) {
        vector<bool> visited(graph.size(), false);
        if (dfs(start, 0, limit, graph, visited)) {
            return limit;
        }
        limit++;
    }
    return -1;
}
```

---

## 实战技巧与优化

### 1. 位运算优化
- 使用位运算代替乘除
- 快速判断奇偶: `n & 1`
- 取模2^k: `n & ((1 << k) - 1)`
- 快速幂: 二进制技巧

### 2. 减少常数
- 减少函数调用
- 使用数组而非链表
- 预计算和缓存结果
- 使用位运算替代算术运算

### 3. 预处理技巧
- 离散化
- 前缀和/差分
- RMQ预处理

### 4. STL使用技巧
- 使用unordered_map/set代替map/set降低时间复杂度
- 使用reserve预分配空间
- 避免频繁的push_front操作
- 使用emplace_back代替push_back

## 总结

本文详细介绍了ACM竞赛中常用的算法及其扩展应用，从基础数据结构到高级算法技巧。熟练掌握这些算法和技术，并理解其背后的原理，将有助于解决各种复杂的编程问题。算法竞赛不仅需要扎实的理论基础，还需要大量的实战练习和经验积累。希望这份资料能够帮助程序设计爱好者提升算法设计和实现能力。
