# ST表

【ST表】(Sparse Table)是一种用于解决【区间最值查询】(Range Maximum/Minimum Query, RMQ)问题的数据结构。它支持O(1)的区间查询操作，但只适用于静态数组（不支持修改操作），预处理时间复杂度为O(nlogn)。

## 基本原理

ST表基于【倍增思想】，预处理出所有长度为2^k的区间的最值。当查询任意区间[L, R]的最值时，可以将其分解为两个有重叠的2^k长度区间，然后取这两个区间最值的最值。

关键思想：任意区间[L, R]可以被覆盖为两个长度为2^k的区间，其中k=log₂(R-L+1)。

## 数据结构定义

对于一个长度为n的数组，我们定义ST表`st[i][j]`表示：
- 从第i个元素开始，长度为2^j的区间内的最值（最大值或最小值）
- 即区间[i, i+2^j-1]的最值

例如：
- `st[3][2]`表示从下标3开始，长度为2^2=4的区间[3, 6]的最值

## 预处理

预处理过程分为两步：
1. 处理所有长度为1的区间（即单个元素）
2. 利用【动态规划】计算所有更长的区间

这是最大值ST表的预处理代码：

```cpp
// 预处理ST表，求区间最大值
void preprocess(const vector<int>& arr) {
    int n = arr.size();
    
    // 计算log2(n)向下取整，用于确定ST表的列数
    int logn = log2(n);
    
    // 分配ST表空间
    st.resize(n, vector<int>(logn + 1));
    
    // 初始化：长度为1的区间
    for (int i = 0; i < n; i++) {
        st[i][0] = arr[i];
    }
    
    // 动态规划计算其他区间
    for (int j = 1; j <= logn; j++) {
        for (int i = 0; i + (1 << j) - 1 < n; i++) {
            // st[i][j]表示区间[i, i+2^j-1]的最大值
            // 将其分解为两个长度为2^(j-1)的子区间
            st[i][j] = max(st[i][j-1], st[i + (1 << (j-1))][j-1]);
        }
    }
}
```

## 区间查询

ST表最大的优势是支持O(1)时间的区间查询。对于区间[L, R]，我们可以：

1. 计算k = log₂(R-L+1)，即区间长度的对数向下取整
2. 将区间分解为[L, L+2^k-1]和[R-2^k+1, R]两个重叠区间
3. 取这两个区间最值的最值

查询代码如下：

```cpp
// 查询区间[l,r]的最大值
int query(int l, int r) {
    int k = log2(r - l + 1);
    // 将区间[l,r]分解为两个长度为2^k的区间
    return max(st[l][k], st[r - (1 << k) + 1][k]);
}
```

## 完整实现

以下是ST表的完整实现：

```cpp
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class SparseTable {
private:
    vector<vector<int>> st;  // ST表
    vector<int> log2_table;  // 预计算log2值，避免重复计算

public:
    // 构造函数，预处理ST表
    SparseTable(const vector<int>& arr) {
        int n = arr.size();
        
        // 预计算log2(i)，优化查询性能
        precomputeLog2(n);
        
        // 计算ST表的列数
        int logn = log2_table[n];
        
        // 初始化ST表
        st.resize(n, vector<int>(logn + 1));
        
        // 填充ST表
        // 处理长度为1的区间
        for (int i = 0; i < n; i++) {
            st[i][0] = arr[i];
        }
        
        // 动态规划计算其他区间
        for (int j = 1; j <= logn; j++) {
            for (int i = 0; i + (1 << j) - 1 < n; i++) {
                st[i][j] = max(st[i][j-1], st[i + (1 << (j-1))][j-1]);
            }
        }
    }
    
    // 预计算log2(i)函数，优化查询
    void precomputeLog2(int n) {
        log2_table.resize(n + 1);
        log2_table[1] = 0;
        for (int i = 2; i <= n; i++) {
            log2_table[i] = log2_table[i / 2] + 1;
        }
    }
    
    // 查询区间[l,r]的最大值
    int queryMax(int l, int r) {
        int k = log2_table[r - l + 1];
        return max(st[l][k], st[r - (1 << k) + 1][k]);
    }
    
    // 查询区间[l,r]的最小值（如果需要）
    int queryMin(int l, int r) {
        int k = log2_table[r - l + 1];
        return min(st[l][k], st[r - (1 << k) + 1][k]);
    }
};

int main() {
    vector<int> arr = {5, 2, 9, 1, 7, 3, 8, 4, 6};
    SparseTable st(arr);
    
    cout << "原数组: ";
    for (int num : arr) cout << num << " ";
    cout << endl;
    
    // 测试区间查询
    cout << "区间[0,4]的最大值: " << st.queryMax(0, 4) << endl;
    cout << "区间[3,7]的最大值: " << st.queryMax(3, 7) << endl;
    cout << "区间[2,8]的最大值: " << st.queryMax(2, 8) << endl;
    
    return 0;
}
```

## 时间复杂度与空间复杂度分析

- **预处理时间复杂度**：O(n log n)，其中n是数组长度
- **空间复杂度**：O(n log n)，用于存储ST表
- **查询时间复杂度**：O(1)，这是ST表的主要优势

## ST表的应用场景

1. **静态RMQ问题**：在不需要修改数组的情况下，查询任意区间的最大/最小值
2. **LCA问题**：结合欧拉序列，可以高效解决最近公共祖先问题
3. **GCD(最大公约数)区间查询**：利用ST表可以O(1)查询区间GCD
4. **其他可结合的问题**：任何满足结合律的操作都可以使用ST表优化

## ST表vs线段树

| 特性 | ST表 | 线段树 |
|------|------|------|
| 预处理时间 | O(n log n) | O(n) |
| 空间复杂度 | O(n log n) | O(n) |
| 单点修改 | 不支持 | O(log n) |
| 区间查询 | O(1) | O(log n) |
| 适用场景 | 静态数组的RMQ问题 | 需要动态修改的场景 |

## 典型例题

### 例题1：区间最值查询

**问题**：给定一个长度为n的数组，回答q个询问，每次询问区间[L, R]内的最大值。

**解法**：直接应用ST表的queryMax函数。

### 例题2：最大最小值之差

**问题**：给定一个数组，求所有区间中，最大值与最小值之差最大是多少。

**解法**：
1. 同时构建最大值ST表和最小值ST表
2. 枚举所有可能的区间[L, R]，计算最大值与最小值之差
3. 取最大差值

```cpp
int findMaxDifference(const vector<int>& arr) {
    int n = arr.size();
    
    // 构建最大值ST表和最小值ST表
    SparseTable stMax(arr);  // 最大值表
    SparseTable stMin(arr, false);  // 最小值表，false表示构建最小值表
    
    int maxDiff = 0;
    
    // 枚举所有区间
    for (int l = 0; l < n; l++) {
        for (int r = l; r < n; r++) {
            int maxVal = stMax.query(l, r);
            int minVal = stMin.query(l, r);
            maxDiff = max(maxDiff, maxVal - minVal);
        }
    }
    
    return maxDiff;
}
```

## 注意事项

1. 【适用范围】：ST表只适用于静态数组，不支持修改操作
2. 【内存占用】：对于大型数组，ST表的内存消耗可能很大，需要注意内存限制
3. 【数值溢出】：在处理最大值/最小值时，注意可能的整数溢出
4. 【预计算优化】：可以预计算log2值，避免重复计算，提高效率

## 实现技巧

1. **预计算log2值**：避免每次查询都调用log2函数
2. **位运算优化**：可以用位运算代替一些数学运算，如`1 << j`代替`pow(2, j)`
3. **模板化**：实现一个通用的ST表类，支持不同的聚合操作（最大值、最小值、GCD等）

## 练习题目推荐

1. SPOJ - RMQSQ (Range Minimum Query)
2. CodeForces - 1117E (Maximum Sum)
3. POJ 3264 - Balanced Lineup