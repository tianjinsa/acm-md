# KMP算法

【KMP算法】(Knuth-Morris-Pratt算法)是一种高效的字符串匹配算法，由Donald Knuth、James H. Morris和Vaughan Pratt在1977年共同发表。KMP算法通过利用已匹配部分的信息，避免了不必要的字符比较，将字符串匹配的时间复杂度从O(m*n)降低到O(m+n)。

## 基本原理

KMP算法的核心思想是：**当匹配失败时，不需要回溯主串指针，而是通过预处理的【部分匹配表】(next数组)让模式串指针回退到合适的位置继续匹配**。

KMP算法的关键在于理解和构建next数组，它记录了模式串中前缀和后缀的最长公共元素长度。

## next数组的含义

next[i]表示：对于模式串P的子串P[0...i]，其【最长相等前后缀】的长度。
- 前缀：除了最后一个字符外，子串的所有头部子串
- 后缀：除了第一个字符外，子串的所有尾部子串

例如，对于模式串"ABABC"：
- next[0] = 0，因为单个字符没有相等的前后缀
- next[1] = 0，"AB"的前缀是"A"，后缀是"B"，不相等
- next[2] = 1，"ABA"的前缀有"A","AB"，后缀有"A","BA"，最长公共前后缀是"A"，长度为1
- next[3] = 2，"ABAB"的前缀有"A","AB","ABA"，后缀有"B","AB","BAB"，最长公共前后缀是"AB"，长度为2
- next[4] = 0，"ABABC"没有相等的前后缀

## next数组的构建

构建next数组的过程也可以使用KMP的思想，代码如下：

```cpp
// 构建next数组
void getNext(const string& pattern, vector<int>& next) {
    int n = pattern.length();
    next.resize(n);
    
    next[0] = 0;  // 初始值
    int j = 0;    // j表示前缀末尾
    
    for (int i = 1; i < n; i++) {  // i表示后缀末尾
        // 如果当前字符不匹配，回退j
        while (j > 0 && pattern[i] != pattern[j]) {
            j = next[j - 1];
        }
        
        // 如果当前字符匹配，j向前移动
        if (pattern[i] == pattern[j]) {
            j++;
        }
        
        next[i] = j;  // 记录当前位置的最长相等前后缀长度
    }
}
```

## KMP匹配算法

有了next数组后，KMP匹配的过程如下：

```cpp
// KMP字符串匹配
int kmpSearch(const string& text, const string& pattern) {
    int m = text.length();
    int n = pattern.length();
    
    if (n == 0) return 0;  // 空模式串总是匹配成功
    
    // 构建next数组
    vector<int> next;
    getNext(pattern, next);
    
    int j = 0;  // j表示已匹配的模式串字符数
    
    for (int i = 0; i < m; i++) {
        // 如果当前字符不匹配，回退j
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j - 1];
        }
        
        // 如果当前字符匹配，j向前移动
        if (text[i] == pattern[j]) {
            j++;
        }
        
        // 如果完全匹配
        if (j == n) {
            return i - n + 1;  // 返回匹配的起始位置
        }
    }
    
    return -1;  // 未找到匹配
}
```

## 完整KMP算法实现

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 构建next数组
vector<int> getNext(const string& pattern) {
    int n = pattern.length();
    vector<int> next(n);
    
    next[0] = 0;
    int j = 0;
    
    for (int i = 1; i < n; i++) {
        while (j > 0 && pattern[i] != pattern[j]) {
            j = next[j - 1];
        }
        
        if (pattern[i] == pattern[j]) {
            j++;
        }
        
        next[i] = j;
    }
    
    return next;
}

// KMP字符串匹配，找到所有匹配位置
vector<int> kmpSearchAll(const string& text, const string& pattern) {
    vector<int> positions;
    int m = text.length();
    int n = pattern.length();
    
    if (n == 0) return positions;
    
    // 构建next数组
    vector<int> next = getNext(pattern);
    
    int j = 0;
    
    for (int i = 0; i < m; i++) {
        while (j > 0 && text[i] != pattern[j]) {
            j = next[j - 1];
        }
        
        if (text[i] == pattern[j]) {
            j++;
        }
        
        if (j == n) {
            positions.push_back(i - n + 1);  // 记录匹配位置
            j = next[j - 1];  // 继续寻找下一个匹配
        }
    }
    
    return positions;
}

int main() {
    string text = "ABABCABABCABCABC";
    string pattern = "ABABC";
    
    // 打印next数组
    vector<int> next = getNext(pattern);
    cout << "Next数组: ";
    for (int val : next) {
        cout << val << " ";
    }
    cout << endl;
    
    // 查找所有匹配位置
    vector<int> positions = kmpSearchAll(text, pattern);
    
    if (positions.empty()) {
        cout << "未找到匹配" << endl;
    } else {
        cout << "匹配位置: ";
        for (int pos : positions) {
            cout << pos << " ";
        }
        cout << endl;
    }
    
    return 0;
}
```

## 时间复杂度分析

- 构建next数组：O(m)，其中m是模式串的长度
- KMP匹配：O(n)，其中n是文本串的长度
- 总时间复杂度：O(m + n)

这比朴素的字符串匹配算法O(m*n)要高效得多，尤其是当文本串和模式串很长时。

## 应用场景

1. **字符串匹配**：在文本中查找特定模式串
2. **多模式匹配**：结合其他算法处理多模式匹配问题
3. **循环串问题**：判断字符串是否由某个子串重复组成
4. **最小循环节**：求解字符串的最小循环节长度
5. **字符串压缩**：基于周期性的字符串压缩算法

## 典型例题

### 例题1：字符串查找

**问题**：给定一个文本串和一个模式串，找出模式串在文本串中的所有出现位置。

**解法**：直接应用KMP算法的kmpSearchAll函数。

### 例题2：最小循环节

**问题**：给定一个字符串，判断它是否可以由某个子串重复多次得到，如果可以，找出这个子串。

**思路**：如果字符串S有长度为k的循环节，那么next[n-1]的值会告诉我们最长的相等前后缀。如果n%(n-next[n-1])==0，那么n-next[n-1]就是最小循环节长度。

```cpp
string findMinimumRepeatPattern(const string& s) {
    int n = s.length();
    vector<int> next = getNext(s);
    
    int repeatLength = n - next[n - 1];
    
    // 检查是否能被整除
    if (next[n - 1] > 0 && n % repeatLength == 0) {
        return s.substr(0, repeatLength);
    }
    
    return s;  // 无法分解为重复子串
}
```

## 注意事项

1. 【next数组初始化】：next[0]应该初始化为0
2. 【边界条件】：处理空串的情况
3. 【变形应用】：了解KMP算法在周期串和前缀函数中的应用
4. 【优化next数组】：有时可以使用优化的next数组，使得匹配失败时直接跳转到不同的字符

## 常见错误

1. next数组计算错误
2. 匹配过程中索引处理不当
3. 在处理多个匹配时忘记重置j的值

## 练习题目推荐

1. POJ 3461 - Oulipo (文本中模式串出现次数)
2. SPOJ - NHAY (Needle in the Haystack)
3. LeetCode 28 - Find the Index of the First Occurrence in a String