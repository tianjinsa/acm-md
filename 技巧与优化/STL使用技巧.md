# STL使用技巧

## 概述

STL (Standard Template Library) 是C++标准库的重要组成部分，在ACM竞赛中合理使用STL可以大幅提高编码效率和代码质量。本文将从实战角度介绍STL的关键组件及使用技巧。

## 容器使用技巧

### vector - 动态数组

```cpp
vector<int> v;  // 创建空vector
vector<int> v(10);  // 创建大小为10的vector，元素为0
vector<int> v(10, 5);  // 创建大小为10的vector，元素全为5

// 【常用操作】及其时间复杂度
v.push_back(x);  // O(1) 均摊复杂度，添加元素到末尾
v.pop_back();  // O(1)，移除末尾元素
v[i];  // O(1)，访问第i个元素（无边界检查）
v.at(i);  // O(1)，访问第i个元素（有边界检查）
v.size();  // O(1)，获取大小
v.empty();  // O(1)，检查是否为空
v.clear();  // O(n)，清空vector
v.erase(v.begin() + i);  // O(n)，删除第i个元素
```

**优化技巧**:
1. 使用`reserve()`预分配空间，避免频繁扩容
```cpp
v.reserve(10000);  // 预分配10000个元素的空间
```

2. 使用`emplace_back()`代替`push_back()`，减少不必要的拷贝
```cpp
// 比v.push_back(pair<int, int>(1, 2))更高效
v.emplace_back(1, 2);
```

### map/unordered_map - 映射

```cpp
map<string, int> m;  // 有序映射，基于红黑树
unordered_map<string, int> um;  // 无序映射，基于哈希表

// 【常用操作】及其时间复杂度
// map: O(log n), unordered_map: O(1) 平均
m["key"] = 100;  // 插入或更新元素
m.count("key");  // 查询键是否存在
m.find("key");  // 返回迭代器，未找到则返回m.end()
m.erase("key");  // 删除元素
```

**使用技巧**:
1. 选择合适的映射类型：
   - `map`: 需要按键排序时
   - `unordered_map`: 注重查找速度，不需要排序时

2. 使用`count()`检查键是否存在，而不是先`find()`再比较
```cpp
// 推荐
if (m.count(key)) { /* key存在 */ }

// 不推荐
if (m.find(key) != m.end()) { /* key存在 */ }
```

### set/unordered_set - 集合

```cpp
set<int> s;  // 有序集合，红黑树实现
unordered_set<int> us;  // 无序集合，哈希表实现

// 【常用操作】及其时间复杂度
// set: O(log n), unordered_set: O(1) 平均
s.insert(x);  // 插入元素
s.count(x);  // 统计元素出现次数（0或1）
s.erase(x);  // 删除元素
```

**优化技巧**:
1. 判断元素是否在集合中，使用`count()`而非`find()`，代码更简洁

2. 使用`insert()`的返回值判断插入是否成功
```cpp
auto [it, success] = s.insert(42);
if (success) {
    // 成功插入新元素
} else {
    // 元素已存在
}
```

### priority_queue - 优先队列

```cpp
// 默认大根堆
priority_queue<int> pq;  
// 小根堆
priority_queue<int, vector<int>, greater<int>> min_pq;

// 【常用操作】O(log n)
pq.push(x);  // 添加元素
pq.pop();  // 弹出顶部元素
pq.top();  // 获取顶部元素 O(1)
```

**自定义比较器**:
```cpp
struct Compare {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first;  // 按first小的优先
    }
};

priority_queue<pair<int, int>, vector<pair<int, int>>, Compare> pq;
```

### deque - 双端队列

```cpp
deque<int> dq;

// 【常用操作】O(1)
dq.push_back(x);  // 尾部添加
dq.push_front(x);  // 头部添加
dq.pop_back();  // 尾部删除
dq.pop_front();  // 头部删除
dq[i];  // 随机访问，O(1)
```

**应用场景**:
- 需要在两端频繁插入/删除的场景
- 实现滑动窗口

## 算法库使用技巧

### 排序

```cpp
// 数组排序
sort(arr, arr + n);  // O(n log n)
// vector排序
sort(v.begin(), v.end());  // O(n log n)

// 自定义排序
sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;  // 降序排序
});

// 部分排序
partial_sort(v.begin(), v.begin() + 3, v.end());  // 只排序前3个

// 稳定排序
stable_sort(v.begin(), v.end());  // 保持相等元素的相对顺序
```

### 二分查找

```cpp
// 在已排序的区间中查找元素
binary_search(v.begin(), v.end(), value);  // 返回bool

// 找到第一个大于等于value的位置
auto it = lower_bound(v.begin(), v.end(), value);

// 找到第一个大于value的位置
auto it = upper_bound(v.begin(), v.end(), value);

// 同时获取上述两个边界
auto [first, last] = equal_range(v.begin(), v.end(), value);
```

### 堆操作

```cpp
vector<int> v = {3, 1, 4, 1, 5, 9};

// 将v转化为堆
make_heap(v.begin(), v.end());  // O(n)

// 添加元素到堆
v.push_back(6);
push_heap(v.begin(), v.end());  // O(log n)

// 弹出堆顶元素
pop_heap(v.begin(), v.end());  // O(log n)，堆顶被移动到末尾
v.pop_back();  // 实际删除
```

## 迭代器技巧

### 迭代器失效情况

【易错点】了解迭代器何时会失效:
- `vector`/`deque` 扩容时，所有迭代器都失效
- 对于删除操作:
  - `vector`/`deque` 中，删除点之后的所有迭代器失效
  - `list`/`map`/`set` 中，仅删除点的迭代器失效

安全删除技巧:
```cpp
// vector中安全删除满足条件的元素
for (auto it = v.begin(); it != v.end(); ) {
    if (shouldRemove(*it)) {
        it = v.erase(it);  // erase返回下一个有效迭代器
    } else {
        ++it;
    }
}

// 更简洁的方法(C++20前)
v.erase(remove_if(v.begin(), v.end(), 
         [](int x) { return x % 2 == 0; }), v.end());

// C++20 erase_if
erase_if(v, [](int x) { return x % 2 == 0; });
```

## ACM比赛中的STL使用策略

1. **容器选择策略**:
   - 需频繁在任意位置插入/删除: `list`
   - 频繁在两端操作: `deque`
   - 需要按键快速查找: `unordered_map`/`unordered_set`
   - 需要有序性: `map`/`set`
   - 需要频繁随机访问: `vector`

2. **避免不必要的拷贝**:
   - 使用引用参数传递大型容器
   - 尽可能用`emplace`系列函数替代`push`/`insert`

3. **输入输出优化**:

```cpp
// 仅用于ACM竞赛，普通开发中不要使用
ios::sync_with_stdio(false);  // 关闭C和C++的输入输出同步
cin.tie(nullptr);  // 解除cin和cout的绑定
```

## 常见陷阱与注意事项

1. 【易错点】使用`map`/`unordered_map`的`[]`操作会自动创建键
```cpp
map<string, int> m;
if (m["key"] == 0) { /* 即使key不存在也会进入，因为[]会创建键 */ }
```

2. 【易错点】使用`erase`时迭代器失效
```cpp
// 错误方式
for (auto it = v.begin(); it != v.end(); ++it) {
    if (shouldRemove(*it))
        v.erase(it);  // 迭代器已失效，++it会导致未定义行为
}
```

3. 【易错点】`priority_queue`默认是大根堆
```cpp
// 这是小根堆
priority_queue<int, vector<int>, greater<int>> minHeap;
```

4. 【性能陷阱】`unordered_map`在哈希冲突严重时性能会显著下降

5. 【易错点】使用自定义类型作为容器键时需要提供比较函数或哈希函数
```cpp
struct Point { int x, y; };

// 为set自定义比较
struct PointCmp {
    bool operator()(const Point& a, const Point& b) const {
        if (a.x != b.x) return a.x < b.x;
        return a.y < b.y;
    }
};
set<Point, PointCmp> points;

// 为unordered_set自定义哈希
struct PointHash {
    size_t operator()(const Point& p) const {
        return hash<int>()(p.x) ^ hash<int>()(p.y);
    }
};
struct PointEqual {
    bool operator()(const Point& a, const Point& b) const {
        return a.x == b.x && a.y == b.y;
    }
};
unordered_set<Point, PointHash, PointEqual> points;
```

## 练习题推荐

1. **检验map/set用法**: [Codeforces 4C - Registration System](https://codeforces.com/problemset/problem/4/C)
2. **优先队列应用**: [POJ 1094 - Sorting It All Out](http://poj.org/problem?id=1094)
3. **STL算法实践**: [UVa 10107 - What is the Median?](https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=1048)