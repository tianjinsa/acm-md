# STL常用函数指南 - ACM竞赛（C++11）

本文档系统地介绍了ACM竞赛中最常用的STL函数，按功能类别整理，附带详细用法说明、复杂度分析和实例。所有内容均与C++11标准兼容。

## 目录
- [算法相关函数](#算法相关函数)
  - [排序与查找](#排序与查找)
  - [集合操作](#集合操作)
  - [数值算法](#数值算法)
  - [修改序列](#修改序列)
  - [排列组合](#排列组合)
  - [堆操作](#堆操作)
- [迭代器相关函数](#迭代器相关函数)
- [函数对象](#函数对象)
- [数值处理函数](#数值处理函数)
- [字符串处理函数](#字符串处理函数)
- [时间复杂度总结](#时间复杂度总结)
- [实用技巧与最佳实践](#实用技巧与最佳实践)

## 算法相关函数

> 使用前需要包含头文件 `#include <algorithm>`

### 排序与查找

#### `sort`

```cpp
// 对容器内的元素进行排序
// 返回值：无
// 参数说明：
// - first: 指向要排序范围首元素的迭代器
// - last: 指向要排序范围末尾的下一个元素的迭代器
// - comp: 比较函数（可选），接收两个元素并返回bool值，表示第一个参数是否应排在第二个参数前面
sort(first, last, comp);
```

参数详解：
- `first`：指向要排序范围首元素的迭代器。必须是随机访问迭代器
- `last`：指向要排序范围末尾的下一个元素的迭代器（即排序区间为[first, last)）
- `comp`：比较函数（可选），是一个返回bool值的二元谓词，形式为`bool cmp(const Type1 &a, const Type2 &b)`
  - 当`comp(a, b)`返回true时，a会排在b前面
  - 如果不提供，默认使用`operator<`进行升序排序
  - 可以是函数指针、函数对象或Lambda表达式

返回值：无。排序会直接修改原容器中的元素顺序。

时间复杂度：O(N log N)，其中N为排序元素的个数。实际实现通常使用快速排序、堆排序和插入排序的混合算法，在最坏情况下保证O(N log N)。

空间复杂度：O(log N)，用于递归调用栈。

使用场景：
- 需要对数组、vector等随机访问容器进行快速排序
- 可以用于基本数据类型和自定义类型的排序
- 当需要稳定排序（保持相等元素的相对顺序）时，应使用`stable_sort`而非`sort`

示例：

```cpp
vector<int> v = {5, 2, 8, 1, 9};
// 升序排序 - sort将v中的元素按从小到大排序
// 调用后v中的元素会被重新排列为{1, 2, 5, 8, 9}
sort(v.begin(), v.end());
// 结果: {1, 2, 5, 8, 9}

// 降序排序 - 使用greater<int>()作为比较函数
// greater<int>()是一个函数对象，当a>b时返回true，实现从大到小排序
sort(v.begin(), v.end(), greater<int>());
// 结果: {9, 8, 5, 2, 1}

// 使用自定义比较函数 - lambda表达式实现降序排序
// [](int a, int b) { return a > b; } 定义了一个匿名函数，当a>b时返回true
// 这使得排序算法将较大的元素放在前面，实现降序排序
sort(v.begin(), v.end(), [](int a, int b) {
    return a > b;  // 降序排序：当a>b时返回true，表示a应排在b前面
});

// 结构体排序 - 根据Person的age字段升序排序
struct Person {
    string name;
    int age;
};
vector<Person> people = {{"Alice", 25}, {"Bob", 20}, {"Charlie", 30}};
// 使用Lambda表达式作为比较函数，比较两个Person对象的age字段
// 当a.age < b.age时返回true，这样年龄小的Person将排在前面
sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    return a.age < b.age;  // 按年龄升序排序
});

// 多字段排序 - 先按年龄升序，年龄相同时按姓名字母顺序排序
sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
    if (a.age != b.age) return a.age < b.age;  // 主要排序键：年龄
    return a.name < b.name;  // 次要排序键：姓名
});
```

错误处理：
- 如果提供的迭代器不是随机访问迭代器，将导致编译错误
- 比较函数必须提供严格弱序关系，否则行为未定义
- 使用有问题的比较函数（如不满足传递性）可能导致排序结果不确定

注意事项：
- `sort`是不稳定的排序算法，不保证相等元素的相对顺序
- 比较函数必须定义**严格弱序**关系，即满足以下性质：
  - 非自反性：对于任意x，不能有x<x为真
  - 非对称性：如果x<y为真，则y<x为假
  - 传递性：如果x<y为真且y<z为真，则x<z为真
- 只适用于随机访问迭代器的容器，如vector、array、deque等
- 对于list等非随机访问的容器，应使用其成员函数sort
- 在处理大型对象时，尽量使用引用作为比较函数的参数类型，避免不必要的拷贝

#### `stable_sort`

```cpp
// 稳定排序，保持相等元素的相对顺序
// 返回值：无
// 参数说明：
// - first: 指向要排序范围首元素的迭代器
// - last: 指向要排序范围末尾的下一个元素的迭代器
// - comp: 比较函数（可选），接收两个元素并返回bool值
stable_sort(first, last, comp);
```

参数和用法同 `sort`，但保证相等元素的相对顺序不变。

时间复杂度：
- O(N log N)（如果有足够额外内存）
- O(N (log N)²)（在最坏情况下）

示例：

```cpp
struct Student {
    string name;
    int grade;
};
vector<Student> students = {
    {"Alice", 90}, {"Bob", 85}, {"Charlie", 90}, {"David", 85}
};

// 按成绩降序排序，成绩相同时保持原有顺序
// 当grade相同时，stable_sort保证了元素的相对顺序不变
// 例如，Alice和Charlie的grade都是90，排序后Alice仍在Charlie之前
stable_sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
    return a.grade > b.grade;
});
// 结果: [{"Alice", 90}, {"Charlie", 90}, {"Bob", 85}, {"David", 85}]
```

#### `partial_sort`

```cpp
// 将范围中最小的N个元素排序放在开头
// 返回值：指向排好序部分末尾的迭代器（即middle）
// 参数说明：
// - first: 范围起始迭代器
// - middle: 部分排序结束位置（排序[first,middle)区间）
// - last: 范围结束迭代器
// - comp: 比较函数（可选）
partial_sort(first, middle, last, comp);
```

参数说明：
- `first`：范围起始迭代器
- `middle`：部分排序结束位置（排序[first,middle)区间）
- `last`：范围结束迭代器
- `comp`：比较函数（可选）

时间复杂度：O(N log M)，其中N是范围大小，M是要排序的元素个数

使用场景：当只需要前k小/大元素时，比完全排序更高效

示例：

```cpp
vector<int> v = {5, 2, 8, 1, 9, 3, 7, 6, 4};
// 对前5个最小元素排序
// 此操作将v中最小的5个元素（即1,2,3,4,5）排序并放在v的前5个位置
// v.begin() + 5表示排序截止的位置，v.end()表示要考虑的全部元素范围
partial_sort(v.begin(), v.begin() + 5, v.end());
// 可能结果: {1, 2, 3, 4, 5, ?, ?, ?, ?}，其中?表示未排序的元素
// 注意：后面的元素不保证任何特定顺序，只确保它们都大于或等于前5个元素
```

#### `nth_element`

```cpp
// 将第n小的元素放在正确位置，并将小于它的元素放在它前面，大于它的放在后面
// 返回值：无
// 参数说明：
// - first: 范围起始迭代器
// - nth: 指向要定位的元素的迭代器
// - last: 范围结束迭代器
// - comp: 比较函数（可选）
nth_element(first, nth, last, comp);
```

参数说明：
- `first`：范围起始迭代器
- `nth`：指向要定位的元素的迭代器
- `last`：范围结束迭代器
- `comp`：比较函数（可选）

时间复杂度：平均O(N)，最坏O(N²)

使用场景：
- 查找中位数、分位数
- 需要将数组划分为两部分，但不需要完全排序

示例：

```cpp
vector<int> v = {5, 2, 8, 1, 9, 3, 7, 6, 4};
// 找出第5小的元素（索引为4）
// 此操作将把第5小的元素（值为5）放在v[4]的位置
// 保证v[0]到v[3]的所有元素都小于等于v[4]，v[5]到v[8]的所有元素都大于等于v[4]
nth_element(v.begin(), v.begin() + 4, v.end());
// v[4]是第5小的元素，前面的元素都小于等于它，后面的元素都大于等于它
cout << "第5小的元素是: " << v[4] << endl;

// 找中位数
// 将v.size()/2位置的元素（即中位数）放在正确的位置
// 对于含有9个元素的v，中位数索引为4，值为5
nth_element(v.begin(), v.begin() + v.size()/2, v.end());
cout << "中位数是: " << v[v.size()/2] << endl;
```

#### `binary_search`

```cpp
// 二分查找，返回元素是否存在
// 返回值：bool值，表示值是否在有序区间中
// 参数说明：
// - first, last: 指定查找范围的迭代器
// - value: 要查找的值
// - comp: 比较函数（可选）
bool binary_search(first, last, value, comp);
```

参数说明：
- `first`、`last`：指定查找范围的迭代器
- `value`：要查找的值
- `comp`：比较函数（可选）

时间复杂度：O(log N)

前提条件：
- 区间必须已经排序
- 对于自定义比较函数，区间必须按照该函数规定的顺序排序

示例：

```cpp
vector<int> v = {1, 2, 5, 8, 9};
// 在已排序的vector中查找值5
// binary_search返回true，因为5存在于v中
bool found = binary_search(v.begin(), v.end(), 5); // 返回 true
// 查找不存在的值6
// binary_search返回false，因为6不存在于v中
found = binary_search(v.begin(), v.end(), 6);     // 返回 false

// 使用自定义比较函数进行查找
// 这里使用lambda表达式定义比较函数，当a<b时返回true
bool found_custom = binary_search(v.begin(), v.end(), 5, [](int a, int b) {
    return a < b;  // 升序排序下的查找
});
```

注意事项：
- 仅返回是否存在，不返回元素位置
- 如果需要获取位置，应使用`lower_bound`或`upper_bound`

#### `lower_bound`

```cpp
// 返回指向不小于指定值的第一个元素的迭代器
// 返回值：迭代器，指向第一个大于等于value的元素；如果所有元素都小于value，则返回last
// 参数说明：
// - first, last: 指定查找范围的迭代器
// - value: 查找的值
// - comp: 比较函数（可选）
auto it = lower_bound(first, last, value, comp);
```

时间复杂度：O(log N)

使用场景：
- 查找第一个大于等于某值的元素
- 在有序数组中查找插入位置
- 处理有重复元素的有序区间

示例：

```cpp
vector<int> v = {1, 2, 2, 3, 3, 3, 4, 5};
// 查找第一个大于等于3的元素
// lower_bound返回指向第一个值为3的迭代器（索引为3的位置）
auto it = lower_bound(v.begin(), v.end(), 3);
int pos = it - v.begin(); // 返回位置 3

// 查找第一个不小于2.5的元素
// 在v中没有2.5，所以lower_bound返回指向第一个大于2.5的元素，即索引为2的第一个3
auto it2 = lower_bound(v.begin(), v.end(), 2.5);
int pos2 = it2 - v.begin(); // 返回位置 2 (指向第一个3)

// 计算有序容器中元素的出现次数
// 使用upper_bound和lower_bound的差值计算元素3出现的次数
// upper_bound(v, 3) - lower_bound(v, 3) = 索引6 - 索引3 = 3
int val = 3;
int count = upper_bound(v.begin(), v.end(), val) - lower_bound(v.begin(), v.end(), val);
// count = 3，表示值3出现了3次
```

注意事项：
- 如果没找到（所有元素都小于value），返回last迭代器
- 区间必须已排序
- 通常与upper_bound配合使用

#### `upper_bound`

```cpp
// 返回指向大于指定值的第一个元素的迭代器
// 返回值：迭代器，指向第一个大于value的元素；如果所有元素都不大于value，则返回last
// 参数说明：
// - first, last: 指定查找范围的迭代器
// - value: 查找的值
// - comp: 比较函数（可选）
auto it = upper_bound(first, last, value, comp);
```

时间复杂度：O(log N)

使用场景：
- 查找第一个大于某值的元素
- 与lower_bound配合查找某个值的范围

示例：

```cpp
vector<int> v = {1, 2, 2, 3, 3, 3, 4, 5};
// 查找第一个大于3的元素
// upper_bound返回指向第一个大于3的元素，即值为4的位置（索引为6）
auto it = upper_bound(v.begin(), v.end(), 3);
int pos = it - v.begin(); // 返回位置 6（指向4）

// 查找一个不存在的值的上界
// upper_bound返回指向第一个大于3.5的元素，即值为4的位置（索引为6）
auto it2 = upper_bound(v.begin(), v.end(), 3.5);
int pos2 = it2 - v.begin(); // 返回位置 6（指向4）

// 配合lower_bound获取等值区间
// 获取所有值为3的元素的范围
auto range_begin = lower_bound(v.begin(), v.end(), 3); // 索引3
auto range_end = upper_bound(v.begin(), v.end(), 3);   // 索引6
// 现在[range_begin, range_end)包含所有值为3的元素（索引3,4,5）
// 可以用于遍历所有值为3的元素
for (auto it = range_begin; it != range_end; ++it) {
    // 对每个值为3的元素进行操作
    cout << *it << " at position " << (it - v.begin()) << endl;
}
```

注意事项：
- 如果没找到（所有元素都不大于value），返回last迭代器
- 区间必须已排序

#### `equal_range`

```cpp
// 返回等于指定值的元素范围，返回pair<迭代器,迭代器>
// 返回值：pair对象，first成员是lower_bound的结果，second成员是upper_bound的结果
// 参数说明：
// - first, last: 指定查找范围的迭代器
// - value: 查找的值
// - comp: 比较函数（可选）
auto range = equal_range(first, last, value, comp);
```

时间复杂度：O(log N)

使用场景：
- 需要同时获取lower_bound和upper_bound时
- 在排序容器中寻找等值区间

示例：

```cpp
vector<int> v = {1, 2, 2, 3, 3, 3, 4, 5};
// 获取所有等于3的元素范围
// equal_range返回的pair中，first指向第一个值为3的元素（索引3）
// second指向第一个大于3的元素（索引6）
auto range = equal_range(v.begin(), v.end(), 3);
int first_pos = range.first - v.begin();   // 返回 3
int last_pos = range.second - v.begin();   // 返回 6
// [range.first, range.second)区间内的所有值都等于3
// 可以用于检查元素数量或遍历所有匹配元素
int count = range.second - range.first; // 值为3的元素数量

// 检查元素是否存在更简洁的方法
// 对于不存在的值7，equal_range返回的range.first和range.second相等
auto range2 = equal_range(v.begin(), v.end(), 7);
bool exists = (range2.first != range2.second); // 返回false，因为7不存在
// 如果元素存在，range2.first和range2.second之间会有至少一个元素
```

注意事项：
- equal_range相当于同时调用lower_bound和upper_bound
- 区间必须已排序

### 集合操作

#### `merge`

```cpp
// 合并两个已排序的区间到目标区间
// 返回值：指向目标区间末尾的迭代器
// 参数说明：
// - first1, last1: 第一个已排序范围的迭代器
// - first2, last2: 第二个已排序范围的迭代器
// - result: 目标范围的起始迭代器
// - comp: 比较函数（可选）
auto result_end = merge(first1, last1, first2, last2, result, comp);
```

参数说明：
- `first1`, `last1`：第一个已排序范围
- `first2`, `last2`：第二个已排序范围
- `result`：目标范围的起始迭代器
- `comp`：比较函数（可选）

时间复杂度：O(N+M)，其中N和M是两个输入范围的大小

前提条件：两个输入范围都必须已经按相同顺序排序

使用场景：
- 合并两个有序数组
- 归并排序算法实现
- 在保持顺序的前提下组合两个数据集

示例：

```cpp
vector<int> v1 = {1, 3, 5, 7};
vector<int> v2 = {2, 4, 6, 8};
vector<int> result(v1.size() + v2.size());
// 合并两个有序向量v1和v2到result中
// merge函数保持元素的有序性，按照从小到大排序合并
// 返回值是指向结果序列末尾的迭代器
auto end_it = merge(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
// 结果: {1, 2, 3, 4, 5, 6, 7, 8}
// 可以用返回值检查合并的元素个数
int merged_count = end_it - result.begin(); // 应为8

// 使用比较函数进行降序合并
vector<int> desc_v1 = {7, 5, 3, 1};
vector<int> desc_v2 = {8, 6, 4, 2};
// 使用greater<int>()作为比较函数，表示在比较时认为greater返回true的元素排在前面
// 因此合并的结果是降序的
auto desc_end_it = merge(desc_v1.begin(), desc_v1.end(), desc_v2.begin(), desc_v2.end(), 
      result.begin(), greater<int>());
// 结果: {8, 7, 6, 5, 4, 3, 2, 1}
```

注意事项：
- 目标区间必须有足够的空间
- 对于目标与源重叠的情况，结果不确定

#### `inplace_merge`

```cpp
// 将同一序列中的两个连续有序子序列合并为一个有序序列
// 返回值：无
// 参数说明：
// - first: 序列起始位置
// - middle: 第一个子序列的结束位置（也是第二个子序列的起始位置）
// - last: 序列结束位置
// - comp: 比较函数（可选）
inplace_merge(first, middle, last, comp);
```

参数说明：
- `first`：序列起始位置
- `middle`：第一个子序列的结束位置（也是第二个子序列的起始位置）
- `last`：序列结束位置
- `comp`：比较函数（可选）

时间复杂度：
- 如果有足够的额外内存：O(N)
- 否则：O(N log N)

使用场景：
- 归并排序的一部分
- 当已经有两个相邻的有序子序列需要合并时

示例：

```cpp
vector<int> v = {1, 3, 5, 7, 2, 4, 6, 8};
// v中[0,4)和[4,8)两个子序列都已排序
// inplace_merge将这两个有序子序列原地合并成一个有序序列
// 操作会修改v，使整个范围变为有序
inplace_merge(v.begin(), v.begin() + 4, v.end());
// 结果: {1, 2, 3, 4, 5, 6, 7, 8}

// 使用自定义比较函数进行降序合并
vector<int> desc_v = {7, 5, 3, 1, 8, 6, 4, 2};
// 两个子序列分别是[0,4)和[4,8)，都是降序排列的
inplace_merge(desc_v.begin(), desc_v.begin() + 4, desc_v.end(), greater<int>());
// 结果: {8, 7, 6, 5, 4, 3, 2, 1}
```

#### `set_union`

```cpp
// 计算两个已排序区间的并集
// 返回值：指向结果集合末尾的迭代器
// 参数说明：
// - first1, last1: 第一个已排序范围
// - first2, last2: 第二个已排序范围
// - result: 存储结果的起始迭代器
// - comp: 比较函数（可选）
auto end_it = set_union(first1, last1, first2, last2, result, comp);
```

时间复杂度：O(N+M)，其中N和M是两个输入范围的大小

使用场景：
- 需要合并两个集合且不重复
- 在有序集合上执行集合运算

示例：

```cpp
vector<int> v1 = {1, 2, 3, 4, 5};
vector<int> v2 = {3, 4, 5, 6, 7};
vector<int> result(10);  // 足够大以容纳所有可能结果
// 计算v1和v2的并集
// set_union返回一个指向结果范围末尾的迭代器
auto it = set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
// 调整result的大小为实际元素个数
result.resize(it - result.begin());
// 结果: {1, 2, 3, 4, 5, 6, 7}
// 注意：并集中的每个元素只出现一次

// 处理重复元素
vector<int> v3 = {1, 2, 2, 3, 4};
vector<int> v4 = {2, 3, 3, 4, 5};
result.resize(10);  // 重置result大小
it = set_union(v3.begin(), v3.end(), v4.begin(), v4.end(), result.begin());
result.resize(it - result.begin());
// 结果: {1, 2, 2, 3, 3, 4, 5}
// 注意重复元素在结果中的出现次数是两个输入序列中该元素出现次数的最大值
// v3中有2个'2'，v4中有1个'2'，并集中有2个'2'
// v3中有1个'3'，v4中有2个'3'，并集中有2个'3'
```

注意事项：
- 两个输入区间必须已排序
- 结果区间需要有足够空间
- 函数返回的是结果序列的末尾迭代器

#### `set_intersection`

```cpp
// 计算两个已排序区间的交集
// 返回值：指向结果集合末尾的迭代器
// 参数说明：
// - first1, last1: 第一个已排序范围
// - first2, last2: 第二个已排序范围
// - result: 存储结果的起始迭代器
// - comp: 比较函数（可选）
auto end_it = set_intersection(first1, last1, first2, last2, result, comp);
```

时间复杂度：O(N+M)，其中N和M是两个输入范围的大小

使用场景：
- 查找两个集合的共有元素
- 确定共同兴趣、共同特征等

示例：

```cpp
vector<int> v1 = {1, 2, 3, 4, 5};
vector<int> v2 = {3, 4, 5, 6, 7};
vector<int> result(5);  // 足够大以容纳可能的结果
// 计算v1和v2的交集
// set_intersection返回指向结果范围末尾的迭代器
auto it = set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
// 调整result的大小为实际元素个数
result.resize(it - result.begin());
// 结果: {3, 4, 5} - 这些是v1和v2中共有的元素

// 处理重复元素
vector<int> v3 = {1, 2, 2, 3, 4};
vector<int> v4 = {2, 3, 3, 4, 5};
result.resize(5);  // 重置result大小
it = set_intersection(v3.begin(), v3.end(), v4.begin(), v4.end(), result.begin());
result.resize(it - result.begin());
// 结果: {2, 3, 4}
// 注意重复元素在结果中的出现次数是两个输入序列中该元素出现次数的最小值
// v3中有两个2，v4中有一个2，交集中有一个2
// v3中有一个3，v4中有两个3，交集中有一个3
```

#### `set_difference`

```cpp
// 计算两个已排序区间的差集 (在区间1中但不在区间2中)
// 返回值：指向结果集合末尾的迭代器
// 参数说明：
// - first1, last1: 第一个已排序范围
// - first2, last2: 第二个已排序范围
// - result: 存储结果的起始迭代器
// - comp: 比较函数（可选）
auto end_it = set_difference(first1, last1, first2, last2, result, comp);
```

时间复杂度：O(N+M)，其中N和M是两个输入范围的大小

使用场景：
- 找出第一个集合中独有的元素
- 移除共有元素

示例：

```cpp
vector<int> v1 = {1, 2, 3, 4, 5};
vector<int> v2 = {3, 4, 5, 6, 7};
vector<int> result(5);
// 计算v1和v2的差集（在v1中但不在v2中）
// set_difference返回指向结果范围末尾的迭代器
auto it = set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
// 调整result的大小为实际元素个数
result.resize(it - result.begin());
// 结果: {1, 2}

// 注意差集不是对称的
// 计算v2和v1的差集（在v2中但不在v1中）
it = set_difference(v2.begin(), v2.end(), v1.begin(), v1.end(), result.begin());
result.resize(it - result.begin());
// 结果: {6, 7}

// 处理重复元素
vector<int> v3 = {1, 2, 2, 3, 4};
vector<int> v4 = {2, 3, 3, 4, 5};
result.resize(5);
it = set_difference(v3.begin(), v3.end(), v4.begin(), v4.end(), result.begin());
result.resize(it - result.begin());
// 结果: {1, 2} 
// v3中有两个2，v4中有一个2，差集中保留一个2
```

#### `set_symmetric_difference`

```cpp
// 计算两个已排序区间的对称差集 (在区间1或区间2中，但不同时在两者中)
// 返回值：指向结果集合末尾的迭代器
// 参数说明：
// - first1, last1: 第一个已排序范围
// - first2, last2: 第二个已排序范围
// - result: 存储结果的起始迭代器
// - comp: 比较函数（可选）
auto end_it = set_symmetric_difference(first1, last1, first2, last2, result, comp);
```

时间复杂度：O(N+M)，其中N和M是两个输入范围的大小

使用场景：
- 寻找两个集合中不共有的元素
- XOR类型的集合运算

示例：

```cpp
vector<int> v1 = {1, 2, 3, 4, 5};
vector<int> v2 = {3, 4, 5, 6, 7};
vector<int> result(10);
// 计算v1和v2的对称差集（在v1或v2中，但不同时在两者中）
// set_symmetric_difference返回指向结果范围末尾的迭代器
auto it = set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());
// 调整result的大小为实际元素个数
result.resize(it - result.begin());
// 结果: {1, 2, 6, 7}

// 处理重复元素
vector<int> v3 = {1, 2, 2, 3, 4};
vector<int> v4 = {2, 3, 3, 4, 5};
result.resize(10);
it = set_symmetric_difference(v3.begin(), v3.end(), v4.begin(), v4.end(), result.begin());
result.resize(it - result.begin());
// 结果: {1, 2, 3, 5}
// v3有两个2，v4有一个2，对称差集中有一个2
// v4有两个3，v3有一个3，对称差集中有一个3
```

注意事项：
- 对称差集是元素在一个集合但不在另一个集合中的所有元素
- 可以看作是 (A-B) ∪ (B-A)

### 数值算法

#### `count` / `count_if`

```cpp
// 计算指定元素在范围中的出现次数
// 返回值：int，表示元素出现的次数
// 参数说明：
// - first, last: 指定范围的迭代器
// - value: 要计数的值
int num = count(first, last, value);

// 计算满足谓词条件的元素个数
// 返回值：int，表示满足条件的元素个数
// 参数说明：
// - first, last: 指定范围的迭代器
// - pred: 谓词函数，接收一个元素并返回bool值
int num = count_if(first, last, pred);
```

时间复杂度：O(N)，其中N是范围内的元素个数

使用场景：
- 统计特定元素出现次数
- 统计满足特定条件的元素个数

示例：

```cpp
vector<int> v = {1, 2, 3, 2, 5, 2, 7};
// 计算值为2的元素出现次数
int cnt = count(v.begin(), v.end(), 2); // 返回 3

// 计算满足条件的元素个数
// 这里使用lambda表达式定义谓词函数，判断元素是否为偶数
int even_count = count_if(v.begin(), v.end(), [](int x) { 
    return x % 2 == 0; 
}); // 返回 3（2出现三次）

// 在字符串中计数
string str = "Hello World";
// 计算字符'l'出现的次数
int l_count = count(str.begin(), str.end(), 'l'); // 返回 3

// 复杂条件统计
vector<pair<string, int>> students = {
    {"Alice", 85}, {"Bob", 92}, {"Charlie", 85}, {"David", 78}
};
// 计算成绩大于等于85的学生数量
int high_score = count_if(students.begin(), students.end(), [](const pair<string, int>& s) {
    return s.second >= 85;
}); // 返回 3
```

#### `min_element` / `max_element` / `minmax_element`

```cpp
// 返回范围中最小/最大元素的迭代器
// 返回值：迭代器，指向最小/最大元素
// 参数说明：
// - first, last: 指定范围的迭代器
// - comp: 比较函数（可选）
auto min_it = min_element(first, last, comp);
auto max_it = max_element(first, last, comp);

// 返回范围中最小和最大元素的迭代器
// 返回值：pair对象，first成员是最小元素的迭代器，second成员是最大元素的迭代器
// 参数说明：
// - first, last: 指定范围的迭代器
// - comp: 比较函数（可选）
auto minmax = minmax_element(first, last, comp);
```

时间复杂度：
- `min_element`/`max_element`: O(N)
- `minmax_element`: O(N)，但比单独调用min_element和max_element更高效

使用场景：
- 寻找数组中的最值
- 寻找最大/最小对象
- 同时需要最大和最小值时

示例：

```cpp
vector<int> v = {5, 2, 8, 1, 9};
// 查找最小元素
auto min_it = min_element(v.begin(), v.end()); // 指向 1
// 查找最大元素
auto max_it = max_element(v.begin(), v.end()); // 指向 9

cout << "最小值: " << *min_it << " 位置: " << (min_it - v.begin()) << endl;
cout << "最大值: " << *max_it << " 位置: " << (max_it - v.begin()) << endl;

// 同时获取最小和最大元素
auto minmax = minmax_element(v.begin(), v.end());
int min_val = *minmax.first;   // 1
int max_val = *minmax.second;  // 9

// 使用自定义比较函数
struct Person {
    string name;
    int age;
};
vector<Person> people = {{"Alice", 25}, {"Bob", 20}, {"Charlie", 30}};
// 查找年龄最大的Person
auto oldest = max_element(people.begin(), people.end(), 
    [](const Person& a, const Person& b) { return a.age < b.age; });
cout << "最年长的人是: " << oldest->name << ", " << oldest->age << "岁" << endl;
```

注意事项：
- 返回的是迭代器，需要解引用获取值
- 如果有多个最值元素，返回第一个最小值和最后一个最大值

#### `accumulate`

```cpp
// 计算范围内元素的累加和
// 需引入头文件 #include <numeric>
// 返回值：累加结果
// 参数说明：
// - first, last: 指定范围的迭代器
// - init_val: 初始值
// - op: 二元操作符（可选），默认为加法
T sum = accumulate(first, last, init_val, op);
```

参数说明：
- `first`, `last`：指定范围
- `init_val`：初始值
- `op`：二元操作符（可选），默认为加法

时间复杂度：O(N)

使用场景：
- 计算元素总和
- 将容器中所有元素连接起来
- 执行自定义累积操作

示例：

```cpp
#include <numeric>
#include <functional> // 提供multiplies等函数对象

vector<int> v = {1, 2, 3, 4, 5};
// 计算累加和
int sum = accumulate(v.begin(), v.end(), 0); // 返回 15

// 计算乘积
int product = accumulate(v.begin(), v.end(), 1, multiplies<int>()); // 返回 120

// 字符串连接
vector<string> words = {"Hello", "World", "C++"};
// 使用accumulate将字符串连接起来
// next(words.begin())跳过第一个元素，避免在结果前面多一个空格
string sentence = accumulate(next(words.begin()), words.end(), words[0],
    [](const string& a, const string& b) { return a + " " + b; });
// 返回 "Hello World C++"

// 更复杂的累积
vector<pair<string, int>> sales = {{"A", 100}, {"B", 200}, {"C", 150}};
// 计算所有销售额的总和
int total = accumulate(sales.begin(), sales.end(), 0,
    [](int sum, const pair<string, int>& p) { return sum + p.second; });
// 返回 450
```

注意事项：
- 初始值的类型决定了累积结果的类型
- 操作必须满足结合律才能保证结果正确

#### `inner_product`

```cpp
// 计算两个区间的内积，需引入头文件 #include <numeric>
// 返回值：内积结果
// 参数说明：
// - first1, last1: 第一个区间的迭代器
// - first2: 第二个区间的起始迭代器
// - init: 初始值
// - binary_op1: 累计操作（默认为加法）
// - binary_op2: 元素间操作（默认为乘法）
T result = inner_product(first1, last1, first2, init, binary_op1, binary_op2);
```

参数说明：
- `first1`, `last1`：第一个区间
- `first2`：第二个区间的起始位置
- `init`：初始值
- `binary_op1`：累计操作（默认为加法）
- `binary_op2`：元素间操作（默认为乘法）

时间复杂度：O(N)

使用场景：
- 计算向量点积
- 计算加权和
- 比较两个序列的相似度

示例：

```cpp
#include <numeric>

vector<int> v1 = {1, 2, 3};
vector<int> v2 = {4, 5, 6};

// 计算点积: 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
int dot_product = inner_product(v1.begin(), v1.end(), v2.begin(), 0);
cout << "点积: " << dot_product << endl;

// 自定义操作：计算匹配元素数量
int matches = inner_product(v1.begin(), v1.end(), v2.begin(), 0,
                          std::plus<int>(),
                          [](int a, int b) { return a == b ? 1 : 0; });

// 计算欧几里得距离的平方
int euclidean_dist_squared = inner_product(
    v1.begin(), v1.end(), v2.begin(), 0,
    std::plus<int>(),
    [](int a, int b) { return (a - b) * (a - b); });
```

注意事项：
- 第二个区间至少需要与第一个区间相同长度
- 操作的顺序是先应用binary_op2到对应元素，然后用binary_op1累积结果

#### `partial_sum`

```cpp
// 计算部分和序列，需引入头文件 #include <numeric>
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 输出范围的起始迭代器
// - op: 二元操作（可选），默认为加法
partial_sum(first, last, result, op);
```

时间复杂度：O(N)

参数说明：
- `first`, `last`：输入范围
- `result`：输出范围的起始位置
- `op`：二元操作（可选），默认为加法

使用场景：
- 计算前缀和
- 计算累积乘积
- 动态规划中的状态转移

示例：

```cpp
#include <numeric>

vector<int> v = {1, 2, 3, 4, 5};
vector<int> result(v.size());

// 计算部分和
partial_sum(v.begin(), v.end(), result.begin());
// 结果: {1, 3, 6, 10, 15}

// 使用自定义操作：累积乘积
partial_sum(v.begin(), v.end(), result.begin(), multiplies<int>());
// 结果: {1, 2, 6, 24, 120}
```

注意事项：
- 输出序列可以与输入序列相同
- 一般用于快速计算前缀和，避免重复累加计算

#### `adjacent_difference`

```cpp
// 计算相邻元素的差，需引入头文件 #include <numeric>
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 输出范围的起始迭代器
// - op: 二元操作（可选），默认为减法
adjacent_difference(first, last, result, op);
```

时间复杂度：O(N)

参数说明：
- `first`, `last`：输入范围
- `result`：输出范围的起始位置
- `op`：二元操作（可选），默认为减法

使用场景：
- 计算序列的差分
- 寻找序列中的变化
- 时间序列分析

示例：

```cpp
#include <numeric>

vector<int> v = {1, 3, 6, 10, 15};
vector<int> result(v.size());

// 计算相邻元素的差
adjacent_difference(v.begin(), v.end(), result.begin());
// 结果: {1, 2, 3, 4, 5}
// 第一个元素保持不变，其余元素为当前元素减前一个元素

// 使用自定义操作：计算比值
adjacent_difference(v.begin(), v.end(), result.begin(), 
                   [](int a, int b) { return a / b; });
// 结果: {1, 3, 2, 10/6, 15/10} = {1, 3, 2, 1.67, 1.5}
```

注意事项：
- 第一个元素直接复制，不应用操作
- 输入输出范围可以是同一序列

#### `iota`

```cpp
// 用连续递增的值填充范围，需引入头文件 #include <numeric>
// 返回值：无
// 参数说明：
// - first, last: 要填充的范围的迭代器
// - value: 起始值
iota(first, last, value);
```

时间复杂度：O(N)

参数说明：
- `first`, `last`：要填充的范围
- `value`：起始值

使用场景：
- 创建一个递增序列
- 生成索引数组
- 初始化容器

示例：

```cpp
#include <numeric>

vector<int> v(10);
// 用0到9填充数组
iota(v.begin(), v.end(), 0);
// 结果: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}

// 创建自定义序列
vector<int> seq(5);
iota(seq.begin(), seq.end(), 100);
// 结果: {100, 101, 102, 103, 104}
```

### 修改序列

#### `copy` / `copy_if` / `copy_n` / `copy_backward`

```cpp
// 复制元素到目标范围
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 目标范围的起始迭代器
auto end_it = copy(first, last, result);

// 根据条件复制元素
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 目标范围的起始迭代器
// - pred: 谓词函数，接收一个元素并返回bool值
auto end_it = copy_if(first, last, result, pred);

// 复制指定数量的元素
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first: 输入范围的起始迭代器
// - count: 要复制的元素数量
// - result: 目标范围的起始迭代器
auto end_it = copy_n(first, count, result);

// 从后向前复制（避免覆盖）
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result_end: 目标范围的末尾迭代器
auto end_it = copy_backward(first, last, result_end);
```

时间复杂度：O(N)

使用场景：
- 将一个容器的内容复制到另一个容器
- 选择性地复制满足条件的元素
- 处理重叠区域的复制

示例：

```cpp
vector<int> v1 = {1, 2, 3, 4, 5};
vector<int> v2(5);

// 基本复制
copy(v1.begin(), v1.end(), v2.begin());
// v2: {1, 2, 3, 4, 5}

// 条件复制：只复制偶数
vector<int> even_numbers;
copy_if(v1.begin(), v1.end(), back_inserter(even_numbers), [](int x) {
    return x % 2 == 0;
});
// even_numbers: {2, 4}

// 复制前3个元素
vector<int> v3(3);
copy_n(v1.begin(), 3, v3.begin());
// v3: {1, 2, 3}

// 从后向前复制（适用于目标和源重叠的情况）
vector<int> v4 = {1, 2, 3, 4, 5};
copy_backward(v4.begin(), v4.begin() + 3, v4.begin() + 4);
// v4: {1, 2, 1, 2, 3}
```

注意事项：
- 目标区域必须有足够空间
- 使用`back_inserter`可以避免预分配空间
- 处理重叠区域时，`copy`从前向后复制，`copy_backward`从后向前复制

#### `move` / `move_backward`

```cpp
// 移动元素到目标范围
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 目标范围的起始迭代器
auto end_it = move(first, last, result);

// 从后向前移动（避免覆盖）
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result_end: 目标范围的末尾迭代器
auto end_it = move_backward(first, last, result_end);
```

时间复杂度：O(N)

使用场景：
- 高效转移资源所有权
- 避免不必要的拷贝操作
- 实现移动语义

示例：

```cpp
vector<string> v1 = {"Hello", "World", "C++"};
vector<string> v2(3);

// 移动元素（避免拷贝）
move(v1.begin(), v1.end(), v2.begin());
// v2包含移动后的字符串，v1中的字符串可能变为空

// 处理重叠区域
vector<string> v3 = {"A", "B", "C", "D", "E"};
move_backward(v3.begin(), v3.begin() + 3, v3.begin() + 4);
// v3可能变为: {"A", "A", "B", "C", "E"}
```

注意事项：
- 移动后源对象处于有效但未指定状态
- 移动语义通常比复制更高效，特别是对于大型对象

#### `fill` / `fill_n`

```cpp
// 用指定值填充范围
// 返回值：无
// 参数说明：
// - first, last: 要填充的范围的迭代器
// - value: 填充的值
fill(first, last, value);

// 用指定值填充指定数量的元素
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first: 要填充的范围的起始迭代器
// - count: 要填充的元素数量
// - value: 填充的值
fill_n(first, count, value);
```

时间复杂度：O(N)

使用场景：
- 初始化数组或向量
- 重置容器的所有值
- 填充特定区域

示例：

```cpp
vector<int> v(5);
// 用7填充整个范围
fill(v.begin(), v.end(), 7);
// 结果: {7, 7, 7, 7, 7}

// 只填充部分元素
fill_n(v.begin(), 3, 9);
// 结果: {9, 9, 9, 7, 7}

// 二维数组的初始化
vector<vector<int>> matrix(3, vector<int>(3));
for (auto& row : matrix) {
    fill(row.begin(), row.end(), 0);
}
// 所有元素都被设置为0
```

注意事项：
- `fill_n` 不检查空间是否足够，调用者需确保有足够空间
- 对于自定义类型，要确保赋值操作是高效的

#### `transform`

```cpp
// 对范围内的元素应用函数，结果放入目标范围
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first1, last1: 输入范围的迭代器
// - result: 目标范围的起始迭代器
// - unary_op: 一元操作函数，接收一个元素并返回转换后的值
auto end_it = transform(first1, last1, result, unary_op);

// 对两个范围内的对应元素应用二元函数
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first1, last1: 第一个输入范围的迭代器
// - first2: 第二个输入范围的起始迭代器
// - result: 目标范围的起始迭代器
// - binary_op: 二元操作函数，接收两个元素并返回转换后的值
auto end_it = transform(first1, last1, first2, result, binary_op);
```

时间复杂度：O(N)

使用场景：
- 元素级别的数据转换
- 对两个序列执行元素对应的运算
- 应用映射或过滤函数

示例：

```cpp
vector<int> v = {1, 2, 3, 4, 5};
vector<int> result(v.size());

// 一元操作：每个元素平方
transform(v.begin(), v.end(), result.begin(), [](int x) {
    return x * x;
});
// 结果: {1, 4, 9, 16, 25}

// 二元操作：对应元素相加
vector<int> v2 = {10, 20, 30, 40, 50};
transform(v.begin(), v.end(), v2.begin(), result.begin(), plus<int>());
// 结果: {11, 22, 33, 44, 55}

// 复杂变换：结构转换
struct Person { string name; int age; };
struct PersonView { string display; };

vector<Person> people = {{"Alice", 25}, {"Bob", 30}};
vector<PersonView> views(people.size());

transform(people.begin(), people.end(), views.begin(), [](const Person& p) {
    return PersonView{p.name + " (" + to_string(p.age) + ")"};
});
// 结果: [{"Alice (25)"}, {"Bob (30)"}]
```

注意事项：
- 目标范围必须有足够空间
- 源和目标可以是同一范围
- 可以使用标准库中的函数对象或自定义函数

#### `replace` / `replace_if` / `replace_copy`

```cpp
// 替换范围内的指定值
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - old_value: 要替换的值
// - new_value: 替换后的值
replace(first, last, old_value, new_value);

// 根据条件替换
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - pred: 谓词函数，接收一个元素并返回bool值
// - new_value: 替换后的值
replace_if(first, last, pred, new_value);

// 替换并复制到新范围
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 目标范围的起始迭代器
// - old_value: 要替换的值
// - new_value: 替换后的值
auto end_it = replace_copy(first, last, result, old_value, new_value);
```

时间复杂度：O(N)

使用场景：
- 数据清洗和转换
- 条件替换
- 生成替换后的新序列

示例：

```cpp
vector<int> v = {1, 2, 3, 2, 5};
// 替换值为2的元素为99
replace(v.begin(), v.end(), 2, 99);
// 结果: {1, 99, 3, 99, 5}

// 条件替换：将所有负数替换为0
vector<int> v2 = {-1, 2, -3, 4, -5};
replace_if(v2.begin(), v2.end(), [](int x) { return x < 0; }, 0);
// 结果: {0, 2, 0, 4, 0}

// 替换并复制
vector<int> v3 = {1, 2, 3, 2, 5};
vector<int> result(v3.size());
replace_copy(v3.begin(), v3.end(), result.begin(), 2, 99);
// v3不变，result: {1, 99, 3, 99, 5}
```

#### `unique` / `unique_copy`

```cpp
// 移除连续重复元素，返回新逻辑结尾的迭代器
// 返回值：指向新逻辑结尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - pred: 比较函数（可选），接收两个元素并返回bool值
auto new_end = unique(first, last, pred);

// 复制不重复的元素到目标范围
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 目标范围的起始迭代器
// - pred: 比较函数（可选），接收两个元素并返回bool值
auto result_end = unique_copy(first, last, result, pred);
```

时间复杂度：O(N)

使用场景：
- 移除序列中的连续重复元素
- 创建不包含连续重复元素的副本
- 数据去重（需要先排序）

示例：

```cpp
vector<int> v = {1, 1, 2, 2, 3, 3, 3, 4, 5, 5};
// 移除连续重复元素
auto it = unique(v.begin(), v.end());
v.resize(it - v.begin());
// 结果: {1, 2, 3, 4, 5}

// 使用自定义比较
vector<string> words = {"apple", "APPLE", "banana", "BANANA", "orange"};
auto it2 = unique(words.begin(), words.end(), [](const string& a, const string& b) {
    return a.size() == b.size();  // 根据长度判断是否相同
});
words.resize(it2 - words.begin());
// 可能的结果: {"apple", "banana", "orange"}

// 复制不重复元素到新容器
vector<int> v2 = {1, 1, 2, 2, 3, 3, 3, 4, 5, 5};
vector<int> result;
unique_copy(v2.begin(), v2.end(), back_inserter(result));
// 结果: {1, 2, 3, 4, 5}
```

注意事项：
- `unique`只移除连续重复的元素，所以通常先排序
- 不会改变容器大小，需手动调整（通常用resize）
- 被"移除"的元素仍在容器中，但处于未指定状态

#### `reverse` / `reverse_copy`

```cpp
// 反转序列中的元素顺序
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
reverse(first, last);

// 将反转后的序列复制到目标范围
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, last: 输入范围的迭代器
// - result: 目标范围的起始迭代器
auto result_end = reverse_copy(first, last, result);
```

时间复杂度：O(N)

使用场景：
- 反转数组、字符串或向量
- 创建反转的副本而不修改原序列
- 算法中需要处理反向序列

示例：

```cpp
vector<int> v = {1, 2, 3, 4, 5};
// 反转整个序列
reverse(v.begin(), v.end());
// 结果: {5, 4, 3, 2, 1}

// 反转部分序列
string s = "Hello World";
reverse(s.begin() + 6, s.end());
// 结果: "Hello dlroW"

// 创建反转副本
vector<int> v2 = {1, 2, 3, 4, 5};
vector<int> result(v2.size());
reverse_copy(v2.begin(), v2.end(), result.begin());
// v2不变，result: {5, 4, 3, 2, 1}
```

#### `rotate` / `rotate_copy`

```cpp
// 旋转序列中的元素，将middle指向的元素放到首位
// 返回值：指向旋转后原来首元素的新位置的迭代器
// 参数说明：
// - first, middle: 输入范围的起始迭代器和旋转点
// - last: 输入范围的结束迭代器
auto new_first = rotate(first, middle, last);

// 旋转并复制到目标范围
// 返回值：指向目标范围末尾的迭代器
// 参数说明：
// - first, middle: 输入范围的起始迭代器和旋转点
// - last: 输入范围的结束迭代器
// - result: 目标范围的起始迭代器
auto result_end = rotate_copy(first, middle, last, result);
```

时间复杂度：O(N)

使用场景：
- 循环移位
- 重新排列数据
- 实现循环缓冲区

示例：

```cpp
vector<int> v = {1, 2, 3, 4, 5};
// 将3放到开头（向左旋转2个位置）
rotate(v.begin(), v.begin() + 2, v.end());
// 结果: {3, 4, 5, 1, 2}

// 复制旋转结果到新容器
vector<int> v2 = {1, 2, 3, 4, 5};
vector<int> result(v2.size());
rotate_copy(v2.begin(), v2.begin() + 1, v2.end(), result.begin());
// 结果: {2, 3, 4, 5, 1}
```

注意事项：
- `middle`必须在`first`和`last`之间
- 返回值是旋转后原来首元素的新位置
- 可以用于高效实现循环队列和缓冲区

#### `shuffle`

```cpp
// 随机打乱序列，使用随机数生成器
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - gen: 随机数生成器
shuffle(first, last, gen);
```

时间复杂度：O(N)

使用场景：
- 随机化数据集
- 实现洗牌算法
- Monte Carlo方法

示例：

```cpp
#include <random>

vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
// 使用随机数引擎
random_device rd;
mt19937 g(rd());
shuffle(v.begin(), v.end(), g);
// 结果: 序列随机排列，如 {3, 1, 7, 8, 4, 2, 5, 6}
```

注意事项：
- `shuffle`比`random_shuffle`提供更好的随机性，推荐使用
- 需要高质量随机性时，避免使用`rand()`
- 推荐使用`<random>`头文件中的随机数生成器

### 排列组合

#### `next_permutation` / `prev_permutation`

```cpp
// 生成下一个字典序排列，成功返回true，否则返回false
// 返回值：bool值，表示是否生成了下一个排列
// 参数说明：
// - first, last: 输入范围的迭代器
// - comp: 比较函数（可选）
bool has_next = next_permutation(first, last, comp);

// 生成上一个字典序排列，成功返回true，否则返回false
// 返回值：bool值，表示是否生成了上一个排列
// 参数说明：
// - first, last: 输入范围的迭代器
// - comp: 比较函数（可选）
bool has_prev = prev_permutation(first, last, comp);
```

时间复杂度：平均O(N)，最坏O(N²)

使用场景：
- 枚举所有可能的排列
- 组合优化问题
- 生成下一个排列

示例：

```cpp
vector<int> v = {1, 2, 3};
// 枚举所有排列
do {
    // 处理当前排列
    for (int n : v) cout << n << ' ';
    cout << '\n';
} while (next_permutation(v.begin(), v.end()));

// 输出:
// 1 2 3
// 1 3 2
// 2 1 3
// 2 3 1
// 3 1 2
// 3 2 1

// 对于已排序的最大排列，下一个排列会回到开始
vector<int> v2 = {3, 2, 1};
bool has_next = next_permutation(v2.begin(), v2.end());
// has_next为false，v2变为{1, 2, 3}

// 使用prev_permutation
vector<int> v3 = {3, 2, 1};
do {
    // 处理当前排列
    for (int n : v3) cout << n << ' ';
    cout << '\n';
} while (prev_permutation(v3.begin(), v3.end()));

// 输出与next_permutation相反的顺序
```

注意事项：
- 初始序列需要是有效的排列（没有重复元素）
- 想要生成所有排列，起始序列应该是最小排列（已排序）
- 函数会修改序列，将其变为下一个/上一个排列

### 堆操作

```cpp
// 创建堆
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - comp: 比较函数（可选）
make_heap(first, last, comp);

// 添加元素到堆
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - comp: 比较函数（可选）
push_heap(first, last, comp);

// 移除堆顶元素
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - comp: 比较函数（可选）
pop_heap(first, last, comp);

// 堆排序
// 返回值：无
// 参数说明：
// - first, last: 输入范围的迭代器
// - comp: 比较函数（可选）
sort_heap(first, last, comp);
```

时间复杂度：
- `make_heap`: O(N)
- `push_heap`: O(log N)
- `pop_heap`: O(log N)
- `sort_heap`: O(N log N)

使用场景：
- 优先队列实现
- 堆排序
- 动态维护最大/最小值

示例：

```cpp
vector<int> v = {3, 1, 4, 1, 5, 9};

// 构建最大堆
make_heap(v.begin(), v.end());
// v可能变为: {9, 5, 4, 1, 1, 3}

// 添加元素
v.push_back(6);
push_heap(v.begin(), v.end());
// 先添加元素到末尾，然后调整堆

// 移除堆顶元素
pop_heap(v.begin(), v.end());
int top = v.back(); // 获取堆顶元素
v.pop_back();       // 从容器中移除
// pop_heap将堆顶元素移到末尾，并重新调整堆

// 堆排序
sort_heap(v.begin(), v.end());
// 结果: 升序排列
```

详细用法：

```cpp
// 创建最小堆
vector<int> min_heap = {6, 1, 2, 8, 3, 4};
make_heap(min_heap.begin(), min_heap.end(), greater<int>());
// min_heap可能为: {1, 3, 2, 8, 6, 4}

// 弹出最小元素
pop_heap(min_heap.begin(), min_heap.end(), greater<int>());
int min_val = min_heap.back();
min_heap.pop_back();
// min_val = 1

// 添加元素到最小堆
min_heap.push_back(0);
push_heap(min_heap.begin(), min_heap.end(), greater<int>());
// 0现在是堆顶最小元素

// 自定义堆比较函数
struct Person {
    string name;
    int priority;
};

vector<Person> persons = {
    {"Alice", 3}, {"Bob", 1}, {"Charlie", 2}
};

make_heap(persons.begin(), persons.end(), 
    [](const Person& a, const Person& b) {
        return a.priority < b.priority;  // 优先级高的放堆顶
    });
```

注意事项：
- 默认创建的是最大堆
- 常用于实现优先队列和堆排序
- 需要手动维护容器大小

## 迭代器相关函数

> 使用前需要包含头文件 `#include <iterator>`

#### `advance`

```cpp
// 将迭代器向前或向后移动n个位置
// 返回值：无
// 参数说明：
// - it: 要移动的迭代器
// - n: 移动的距离，可以为负
advance(it, n);
```

时间复杂度：
- 随机访问迭代器: O(1)
- 其他迭代器: O(N)

示例：

```cpp
list<int> lst = {1, 2, 3, 4, 5};
auto it = lst.begin();
advance(it, 2); // 现在it指向3
```

注意事项：
- 没有边界检查，调用者必须确保移动后迭代器仍然有效
- 对于双向迭代器，n可以为负
- 对于单向迭代器，n必须为非负

#### `distance`

```cpp
// 计算两个迭代器之间的距离
// 返回值：int，表示两个迭代器之间的距离
// 参数说明：
// - first, last: 要计算距离的两个迭代器
int d = distance(first, last);
```

时间复杂度：
- 随机访问迭代器: O(1)
- 其他迭代器: O(N)

示例：

```cpp
vector<int> v = {1, 2, 3, 4, 5};
int d = distance(v.begin(), v.end()); // 返回5

list<int> lst = {10, 20, 30, 40};
auto it = lst.begin();
advance(it, 2);
int d2 = distance(lst.begin(), it); // 返回2
```

注意事项：
- first必须可达last，否则行为未定义
- 对于非随机访问迭代器，需要遍历计算距离

#### `next` / `prev`

```cpp
// 返回前进n个位置的迭代器（不修改原迭代器）
// 返回值：前进n个位置后的迭代器
// 参数说明：
// - it: 要前进的迭代器
// - n: 前进的距离，默认为1
auto next_it = next(it, n = 1);

// 返回后退n个位置的迭代器（不修改原迭代器）
// 返回值：后退n个位置后的迭代器
// 参数说明：
// - it: 要后退的迭代器
// - n: 后退的距离，默认为1
auto prev_it = prev(it, n = 1);
```

时间复杂度：与`advance`相同

示例：

```cpp
list<int> lst = {1, 2, 3, 4, 5};
auto it = lst.begin();
auto it2 = next(it, 2); // 指向3，不修改it
auto it3 = prev(lst.end(), 1); // 指向5
```

注意事项：
- 与`advance`不同，不会修改原迭代器
- C++11引入，提供更安全的迭代器移动方式

#### `back_inserter` / `front_inserter` / `inserter`

```cpp
// 创建插入迭代器
// 返回值：插入迭代器
// 参数说明：
// - container: 要插入元素的容器
back_inserter(container);   // 在容器末尾插入
front_inserter(container);  // 在容器开头插入
inserter(container, it);    // 在指定位置插入
```

使用场景：
- 在STL算法中动态扩展容器
- 避免手动调整容器大小
- 实现流式处理

示例：

```cpp
vector<int> source = {1, 2, 3, 4, 5};
vector<int> dest;

// 使用back_inserter避免预先分配空间
copy(source.begin(), source.end(), back_inserter(dest));
// dest: {1, 2, 3, 4, 5}

// 过滤并插入
copy_if(source.begin(), source.end(), back_inserter(dest),
       [](int x) { return x % 2 == 0; });
// 添加到dest后面: {1, 2, 3, 4, 5, 2, 4}

// front_inserter适用于双端队列和列表
list<int> lst;
copy(source.begin(), source.end(), front_inserter(lst));
// lst: {5, 4, 3, 2, 1} (注意顺序反转)

// 在特定位置插入
list<int> lst2 = {10, 20, 30, 40};
auto pos = next(lst2.begin(), 2); // 指向30
copy(source.begin(), source.end(), inserter(lst2, pos));
// lst2: {10, 20, 1, 2, 3, 4, 5, 30, 40}
```

注意事项：
- `back_inserter`调用`push_back`，适用于vector、deque、list等
- `front_inserter`调用`push_front`，只适用于deque、list等
- `inserter`调用`insert`，适合所有支持插入的容器

## 函数对象

> 需包含头文件 `#include <functional>`

STL提供了许多预定义的函数对象，它们是实现了函数调用运算符（`operator()`）的类。这些对象可以像函数一样使用，但有更多灵活性。

### 预定义函数对象

```cpp
// 算术运算
plus<T>()          // 函数对象，实现 a + b 的加法操作
minus<T>()         // 函数对象，实现 a - b 的减法操作
multiplies<T>()    // 函数对象，实现 a * b 的乘法操作
divides<T>()       // 函数对象，实现 a / b 的除法操作
modulus<T>()       // 函数对象，实现 a % b 的取模操作
negate<T>()        // 函数对象，实现 -a 的取负操作

// 比较运算
equal_to<T>()      // 函数对象，检查 a == b 是否成立
not_equal_to<T>()  // 函数对象，检查 a != b 是否成立
greater<T>()       // 函数对象，检查 a > b 是否成立
less<T>()          // 函数对象，检查 a < b 是否成立
greater_equal<T>() // 函数对象，检查 a >= b 是否成立
less_equal<T>()    // 函数对象，检查 a <= b 是否成立

// 逻辑运算
logical_and<T>()   // 函数对象，实现 a && b 的逻辑与操作
logical_or<T>()    // 函数对象，实现 a || b 的逻辑或操作
logical_not<T>()   // 函数对象，实现 !a 的逻辑非操作
```

函数对象的优点：
1. 比函数指针更高效，因为编译器可以内联它们
2. 可以有状态（成员变量）
3. 可以与STL算法无缝配合
4. 便于组合和自定义

使用场景：
- 作为排序和比较操作的谓词
- 参与STL算法中的函数组合
- 实现自定义的操作，尤其是需要保存状态的操作

### 基本用法示例

```cpp
#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>

vector<int> v = {5, 2, 8, 1, 9};

// 使用greater函数对象进行降序排序
// greater<int>()创建一个函数对象，当第一个参数大于第二个参数时返回true
// 这会导致较大的元素排在前面，实现降序排序
sort(v.begin(), v.end(), greater<int>());
// 结果: {9, 8, 5, 2, 1}

// 使用less函数对象进行升序排序（这实际上是默认行为）
// less<int>()创建一个函数对象，当第一个参数小于第二个参数时返回true
sort(v.begin(), v.end(), less<int>());
// 结果: {1, 2, 5, 8, 9}
```

### 函数对象用于算术运算

```cpp
#include <functional>
#include <algorithm>
#include <numeric>
#include <vector>

// 准备两个向量
vector<int> v1 = {1, 2, 3, 4};
vector<int> v2 = {10, 20, 30, 40};
vector<int> result(v1.size());

// 使用plus函数对象对应元素相加
// transform以v1和v2为输入，以result为输出，对每对对应元素应用plus<int>()操作
// plus<int>()会将两个参数相加并返回和
transform(v1.begin(), v1.end(), v2.begin(), result.begin(), plus<int>());
// 结果: {11, 22, 33, 44}

// 使用multiplies函数对象对应元素相乘
// multiplies<int>()会将两个参数相乘并返回乘积
transform(v1.begin(), v1.end(), v2.begin(), result.begin(), multiplies<int>());
// 结果: {10, 40, 90, 160}

// 使用accumulate计算乘积
// 这里用multiplies<int>()替代默认的加法操作，实现连乘
int product = accumulate(v1.begin(), v1.end(), 1, multiplies<int>());
// 结果: 24 (= 1*1*2*3*4)
```

### 创建自定义函数对象

```cpp
// 自定义函数对象：计算平方
struct Square {
    // 重载函数调用运算符，使Square对象可以像函数一样被调用
    int operator()(int x) const {
        return x * x;
    }
};

vector<int> nums = {1, 2, 3, 4, 5};
vector<int> squares(nums.size());

// 使用自定义函数对象
// 创建一个Square对象并将其传给transform
// transform会对nums中每个元素调用Square对象，计算其平方值
transform(nums.begin(), nums.end(), squares.begin(), Square());
// 结果: {1, 4, 9, 16, 25}

// 带状态的函数对象
class Accumulator {
private:
    int sum; // 状态：保存累计和
public:
    // 构造函数初始化状态
    Accumulator() : sum(0) {}
    
    // 函数调用运算符：累加并返回当前和
    int operator()(int x) {
        sum += x;
        return sum;
    }
    
    // 获取当前累计结果
    int getSum() const {
        return sum;
    }
};

// 使用带状态的函数对象
vector<int> cumulative(nums.size());
Accumulator acc;
// transform调用acc处理nums中每个元素，acc会维护累计和
transform(nums.begin(), nums.end(), cumulative.begin(), ref(acc));
// 结果: {1, 3, 6, 10, 15}
// 注：使用ref()包装函数对象以传引用，确保状态被正确累计
```

### 绑定参数与函数组合

```cpp
#include <functional>

// 使用bind创建新的函数对象
// bind将第一个参数(multiplies<int>())与常量5绑定，生成新的函数
auto times5 = bind(multiplies<int>(), placeholders::_1, 5);
// times5(x)等价于x*5

vector<int> v = {1, 2, 3, 4};
vector<int> result(v.size());

// 使用绑定后的函数对象
transform(v.begin(), v.end(), result.begin(), times5);
// 结果: {5, 10, 15, 20}

// 更复杂的函数组合
vector<int> nums = {1, -2, 3, -4, 5};

// 使用lambda表达式与accumulate组合
// 这个lambda仅累加正数，忽略负数
int positive_sum = accumulate(nums.begin(), nums.end(), 0,
                             [](int sum, int x) {
                                 return sum + (x > 0 ? x : 0);
                             });
// positive_sum = 9 (= 1 + 3 + 5)
```

### C++14透明函数对象

C++14 引入了透明的函数对象，无需指定类型，编译器可以从上下文推导：

```cpp
// C++14特性：透明函数对象
sort(v.begin(), v.end(), greater<>()); // 不指定类型，自动推导
sort(v.begin(), v.end(), less<>());    // 不指定类型，自动推导

// 这样写可以处理不同类型的比较
vector<string> vs = {"apple", "Orange", "banana"};
// 使用less<>可以直接比较不同类型
sort(vs.begin(), vs.end(), less<>()); // 默认字典序
```

### 函数包装器 `std::function`

`std::function`是一个通用的函数包装器，可以存储、复制和调用任何可调用目标：

```cpp
#include <functional>

// 创建一个接受两个int并返回int的函数类型
function<int(int, int)> op;

// 存储函数指针
int add(int a, int b) { return a + b; }
op = add;
int result1 = op(2, 3); // 结果: 5

// 存储函数对象
op = multiplies<int>();
int result2 = op(2, 3); // 结果: 6

// 存储lambda表达式
op = [](int a, int b) { return a - b; };
int result3 = op(5, 3); // 结果: 2

// 用于回调函数
void processNumbers(vector<int>& v, function<void(int&)> processor) {
    for (auto& num : v) {
        processor(num);
    }
}

// 使用不同的处理函数
vector<int> numbers = {1, 2, 3, 4, 5};
processNumbers(numbers, [](int& x) { x *= 2; });  // 所有元素乘2
// 结果: {2, 4, 6, 8, 10}
```

## 数值处理函数

> 使用前需包含头文件 `#include <cmath>`

STL 提供了丰富的数学函数，这些函数广泛用于科学计算、几何处理和数值分析等场景。

### 基本数学函数

```cpp
// 基本运算
abs(x)       // 整数绝对值，参数和返回值类型相同
fabs(x)      // 浮点数绝对值，返回double类型
sqrt(x)      // 平方根，参数可以是任何数值类型，返回浮点类型
cbrt(x)      // 立方根，参数可以是任何数值类型，返回浮点类型
hypot(x, y)  // 计算sqrt(x*x + y*y)，避免中间溢出

// 指数和对数
pow(x, y)    // x的y次幂，两个参数可以是不同类型
exp(x)       // e的x次幂
log(x)       // 自然对数(以e为底)，参数必须为正
log10(x)     // 以10为底的对数，参数必须为正
log2(x)      // 以2为底的对数，参数必须为正
```

### 取整函数

```cpp
ceil(x)      // 向上取整，返回大于等于x的最小整数
floor(x)     // 向下取整，返回小于等于x的最大整数
round(x)     // 四舍五入到最接近的整数
trunc(x)     // 截断取整（向0取整），丢弃小数部分
```

### 三角函数和双曲函数

```cpp
// 三角函数（参数为弧度）
sin(x)       // 正弦
cos(x)       // 余弦
tan(x)       // 正切
asin(x)      // 反正弦，参数范围[-1,1]，返回[-π/2,π/2]
acos(x)      // 反余弦，参数范围[-1,1]，返回[0,π]
atan(x)      // 反正切，返回[-π/2,π/2]
atan2(y, x)  // 计算y/x的反正切，考虑象限，返回[-π,π]

// 双曲函数
sinh(x)      // 双曲正弦
cosh(x)      // 双曲余弦
tanh(x)      // 双曲正切
asinh(x)     // 反双曲正弦
acosh(x)     // 反双曲余弦，参数必须≥1
atanh(x)     // 反双曲正切，参数范围(-1,1)
```

### 特殊数值判断和操作

```cpp
isnan(x)     // 检查x是否为NaN(不是一个数)，返回bool
isinf(x)     // 检查x是否为无穷大，返回bool
isfinite(x)  // 检查x是否为有限值，返回bool
signbit(x)   // 检查x的符号位是否被设置（是否为负），返回bool
copysign(x,y) // 返回带有y符号的x的绝对值
nextafter(x,y) // 返回x向着y方向的下一个可表示的数
```

### 实际使用示例

```cpp
#include <cmath>
#include <iostream>
#include <iomanip>

// 基本计算
double a = sqrt(16);             // 计算16的平方根：4.0
double b = pow(2, 3);            // 计算2的3次方：8.0
double c = log10(100);           // 计算log₁₀(100)：2.0

// 取整操作
double d = ceil(3.14);           // 向上取整：4.0
double e = floor(3.14);          // 向下取整：3.0
double f = round(3.5);           // 四舍五入：4.0
double g = trunc(-2.7);          // 截断取整：-2.0

// 三角函数（注意使用弧度）
double h = sin(M_PI / 2);        // sin(90°)：1.0
double i = cos(M_PI);            // cos(180°)：-1.0
double j = atan2(1, 1);          // 计算1/1的反正切：π/4

// 特殊值判断
double inf = 1.0 / 0.0;          // 创建无穷大值
double nan_val = 0.0 / 0.0;      // 创建NaN值
bool is_inf = isinf(inf);        // 检查是否为无穷大：true
bool is_nan = isnan(nan_val);    // 检查是否为NaN：true
bool is_finite = isfinite(inf);  // 检查是否为有限值：false

// 高精度浮点数比较
// 浮点数不应直接用==比较，而应考虑误差范围
const double epsilon = 1e-9;     // 误差容忍度
bool is_equal = fabs(a - 4.0) < epsilon;  // 检查a是否等于4.0：true

// 实际应用：计算两点之间的欧几里得距离
double distance(double x1, double y1, double x2, double y2) {
    // hypot函数避免了中间计算可能的溢出问题
    return hypot(x2 - x1, y2 - y1);
}
double dist = distance(0, 0, 3, 4);  // 计算原点到(3,4)的距离：5.0

// 使用三角函数计算角度（弧度转角度）
double angle_rad = atan2(4, 3);      // 计算向量(3,4)与x轴的角度（弧度）
double angle_deg = angle_rad * 180 / M_PI;  // 将弧度转换为角度：约53.13°
```

### 复数相关函数

对于复数操作，需要包含头文件`<complex>`：

```cpp
#include <complex>

// 定义复数
complex<double> z1(3, 4);  // 实部3，虚部4
complex<double> z2(1, 2);

// 复数运算
auto sum = z1 + z2;        // 复数加法：(4,6)
auto product = z1 * z2;    // 复数乘法：(3*1-4*2, 3*2+4*1)=(−5,10)

// 复数函数
double mag = abs(z1);      // 复数的模：5
double arg = arg(z1);      // 复数的辐角
complex<double> conj = conj(z1);  // 共轭复数：(3,-4)
complex<double> sqr = sqrt(z1);   // 复数的平方根
```

## 字符串处理函数

C++提供了两种处理字符串的方式：C风格的字符串函数和C++ string类。对于现代C++编程，推荐使用C++ string类，它更安全，更灵活。

### C风格字符串函数 (`<cstring>`)

```cpp
// 字符串长度和比较
strlen(str)                  // 计算字符串长度（不包括null结束符）
strcmp(s1, s2)               // 比较字符串，相等返回0，s1<s2返回负值，s1>s2返回正值
strncmp(s1, s2, n)           // 比较最多n个字符

// 字符串复制和连接
strcpy(dest, src)            // 将src复制到dest，包括null结束符
strncpy(dest, src, n)        // 复制最多n个字符，不保证null结束符
strcat(dest, src)            // 将src连接到dest末尾
strncat(dest, src, n)        // 连接最多n个字符

// 字符串搜索
strchr(str, c)               // 查找字符c首次出现的位置，返回指针或NULL
strrchr(str, c)              // 查找字符c最后出现的位置，返回指针或NULL
strstr(str, substr)          // 查找子串substr首次出现的位置，返回指针或NULL
strpbrk(str, charset)        // 查找charset中任意字符首次出现的位置

// 字符串分割和转换
strtok(str, delim)           // 按分隔符分割字符串，返回下一个标记或NULL
atoi(str)                    // 将字符串转换为int
atof(str)                    // 将字符串转换为double
```

详细示例：

```cpp
#include <cstring>
#include <iostream>

// 基本字符串操作
char s1[100] = "Hello";
char s2[] = "World";

// 获取字符串长度
size_t len = strlen(s1);        // 返回5（不计算结束符'\0'）

// 字符串比较
int cmp = strcmp(s1, s2);       // 返回负值，因为'H'的ASCII码小于'W'
bool is_equal = (strcmp(s1, "Hello") == 0); // 检查s1是否等于"Hello"：true

// 字符串复制和连接
strcpy(s1 + 5, " ");            // 将空格复制到s1[5]位置，s1变为"Hello "
strcat(s1, s2);                 // 将s2连接到s1末尾，s1变为"Hello World"

// 字符串搜索
char* found = strchr(s1, 'o');  // 查找'o'首次出现位置，found指向"o World"
char* last_o = strrchr(s1, 'o'); // 查找'o'最后出现位置，指向"orld"中的'o'
char* substr = strstr(s1, "World"); // 查找"World"，substr指向"World"

// 字符串分割
char csv[] = "apple,banana,orange";
char* token = strtok(csv, ","); // 获取第一个标记：apple
while (token != nullptr) {
    std::cout << token << std::endl; // 输出当前标记
    token = strtok(nullptr, ",");    // 获取下一个标记
}
// 输出:
// apple
// banana
// orange

// 注意：strtok会修改原字符串，插入null字符('\0')来分隔标记
// 如果需要保留原字符串，应事先创建副本

// 安全的字符串复制（防止缓冲区溢出）
char dest[10];
strncpy(dest, "This is a very long string", 9); // 只复制前9个字符
dest[9] = '\0'; // 手动添加结束符确保安全
// dest包含"This is a"

// 字符串转数字
int num = atoi("123");         // 将字符串转换为整数：123
double dbl = atof("3.14159");  // 将字符串转换为浮点数：3.14159
```

注意事项：
- C风格字符串函数不检查边界，可能导致缓冲区溢出和安全问题
- 使用时必须确保目标数组有足够空间
- 函数如`strncpy`不会自动添加null结束符，需手动添加
- 推荐使用C++ string类替代这些函数，更安全可靠

### C++ string类函数 (`<string>`)

```cpp
// 构造和基本操作
string str;              // 创建空字符串
string str(n, c);        // 创建包含n个字符c的字符串
string str(other_str);   // 从另一个string创建（复制构造）
string str(char_array);  // 从C风格字符串创建

// 大小和容量
str.length(), str.size() // 字符串长度（以字符计）
str.empty()              // 检查字符串是否为空
str.capacity()           // 返回当前分配的存储空间大小
str.reserve(n)           // 预留至少能存储n个字符的空间
str.shrink_to_fit()      // 减少容量以适应实际大小

// 元素访问
str[i]                   // 访问第i个字符（无边界检查）
str.at(i)                // 访问第i个字符（有边界检查，越界抛出异常）
str.front()              // 访问第一个字符
str.back()               // 访问最后一个字符
str.data(), str.c_str()  // 获取底层的C风格字符串指针

// 修改操作
str.clear()              // 清空字符串内容
str.insert(pos, s)       // 在位置pos插入字符串s
str.erase(pos, len)      // 从位置pos开始删除len个字符
str.replace(pos, len, s) // 用s替换从pos开始的len个字符
str.append(s)            // 追加字符串s到末尾
str.push_back(c)         // 追加字符c到末尾
str.pop_back()           // 删除最后一个字符
str += s                 // 字符串连接（操作符重载）

// 查找操作
str.find(s)              // 查找子串s首次出现的位置
str.rfind(s)             // 查找子串s最后出现的位置
str.find_first_of(s)     // 查找s中任何字符首次出现的位置
str.find_last_of(s)      // 查找s中任何字符最后出现的位置
str.find_first_not_of(s) // 查找不在s中的任何字符首次出现位置
str.find_last_not_of(s)  // 查找不在s中的任何字符最后出现位置

// 子串操作
str.substr(pos, len)     // 提取子串，从pos开始len个字符

// 比较操作
str.compare(s)           // 比较两个字符串，相等返回0
str == s, str != s       // 字符串相等/不等比较
str < s, str > s         // 字符串大小比较（字典序）

// 数值转换
stoi(str), stol(str)     // 字符串转整数（int, long）
stof(str), stod(str)     // 字符串转浮点数（float, double）
to_string(val)           // 数值转字符串
```

详细示例：

```cpp
#include <string>
#include <iostream>
#include <vector>

// 字符串创建和基本操作
string s1 = "Hello";
string s2 = " World";
string s3 = s1 + s2;          // 字符串连接："Hello World"

// 字符串检查
bool empty = s1.empty();       // 检查是否为空：false
size_t len = s1.length();      // 获取长度：5
char first = s1[0];            // 获取第一个字符：'H'
char last = s1.back();         // 获取最后一个字符：'o'
char safe_access = s1.at(2);   // 安全访问第3个字符：'l'

// 字符串修改
s1.append(" C++");             // 追加字符串："Hello C++"
s1.push_back('!');             // 追加字符：'Hello C++!'
s1.insert(5, " Modern");       // 在位置5插入字符串："Hello Modern C++!"
s1.replace(6, 6, "New");       // 替换位置6开始的6个字符："Hello New C++!"
s1.erase(5, 4);                // 删除从位置5开始的4个字符："Hello C++!"
s1.pop_back();                 // 删除最后一个字符："Hello C++"
s1.clear();                    // 清空字符串：""

// 子串操作
string sentence = "Hello World C++";
string sub = sentence.substr(6, 5);  // 提取子串："World"

// 搜索操作
size_t pos = sentence.find("World"); // 查找子串："World"位置为6
pos = sentence.rfind("l");          // 从右侧查找'l'，位置为9
pos = sentence.find_first_of("Wo"); // 查找'W'或'o'首次出现，位置为6（'W'）
pos = sentence.find_last_of("Wo");  // 查找'W'或'o'最后出现，位置为8（'o'）

// string::npos 常量表示"未找到"或"直到字符串末尾"
if (sentence.find("Java") == string::npos) {
    cout << "Java not found in the string" << endl;
}

// 字符串比较
bool equals = (s1 == s2);           // 检查s1是否等于s2
int cmp = sentence.compare("Hello"); // 比较字符串，不相等返回非零值

// 字符串与数值转换
string num_str = "123.45";
int i = stoi("123");          // 字符串转int：123
double d = stod(num_str);     // 字符串转double：123.45

string str_i = to_string(42);  // 整数转字符串："42"
string str_d = to_string(3.14); // 浮点数转字符串："3.140000"

// 字符串分割（C++没有内置的split函数，需要自己实现）
string csv = "apple,banana,orange,grape";
vector<string> tokens;
size_t start = 0;
size_t end = csv.find(",");
while (end != string::npos) {
    tokens.push_back(csv.substr(start, end - start));
    start = end + 1;
    end = csv.find(",", start);
}
tokens.push_back(csv.substr(start)); // 添加最后一个部分

// 输出分割后的结果：
// tokens = {"apple", "banana", "orange", "grape"}

// 字符串连接（join操作）
vector<string> words = {"Hello", "Modern", "C++"};
string joined = words[0];
for (size_t i = 1; i < words.size(); ++i) {
    joined += " " + words[i];
}
// joined = "Hello Modern C++"

// 更高效的字符串构建（对于大量拼接）
string large_str;
large_str.reserve(1000); // 预先分配空间避免多次重新分配
for (int i = 0; i < 100; ++i) {
    large_str += "item " + to_string(i) + ", ";
}
```

字符串处理的高级技巧：

```cpp
// 字符串转换为大写或小写
string to_upper(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return toupper(c); });
    return s;
}

string to_lower(string s) {
    transform(s.begin(), s.end(), s.begin(),
              [](unsigned char c) { return tolower(c); });
    return s;
}

// 修剪字符串（去除前后空白）
string trim(const string& s) {
    // 查找第一个非空白字符
    auto start = s.find_first_not_of(" \t\n\r\f\v");
    // 如果字符串全是空白，则返回空字符串
    if (start == string::npos) return "";
    
    // 查找最后一个非空白字符
    auto end = s.find_last_not_of(" \t\n\r\f\v");
    // 返回修剪后的子串
    return s.substr(start, end - start + 1);
}

// 字符串分割函数
vector<string> split(const string& s, const string& delimiter) {
    vector<string> tokens;
    size_t last = 0;
    size_t next = 0;
    while ((next = s.find(delimiter, last)) != string::npos) {
        tokens.push_back(s.substr(last, next - last));
        last = next + delimiter.length();
    }
    tokens.push_back(s.substr(last));
    return tokens;
}

// 字符串连接函数
string join(const vector<string>& strs, const string& delimiter) {
    if (strs.empty()) return "";
    
    string result = strs[0];
    for (size_t i = 1; i < strs.size(); ++i) {
        result += delimiter + strs[i];
    }
    return result;
}

// 字符串替换（替换所有匹配子串）
string replace_all(string str, const string& from, const string& to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // 避免无限循环
    }
    return str;
}

// 检查字符串是否以某个前缀开始
bool starts_with(const string& str, const string& prefix) {
    return str.size() >= prefix.size() && 
           str.compare(0, prefix.size(), prefix) == 0;
}

// 检查字符串是否以某个后缀结束
bool ends_with(const string& str, const string& suffix) {
    return str.size() >= suffix.size() && 
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// 将字符串转换为数值类型，带错误处理
template<typename T>
optional<T> string_to_number(const string& str) {
    T value;
    try {
        if constexpr (is_same_v<T, int>)
            value = stoi(str);
        else if constexpr (is_same_v<T, long>)
            value = stol(str);
        else if constexpr (is_same_v<T, double>)
            value = stod(str);
        else if constexpr (is_same_v<T, float>)
            value = stof(str);
        else
            return nullopt;
        
        return value;
    } catch (...) {
        return nullopt; // 转换失败
    }
}

// 字符串格式化（C++20特性，此处仅为概念示例）
// 在C++20之前，可以使用sprintf或stringstream实现类似功能
string format_example() {
    string name = "Alice";
    int age = 30;
    
    // C++20方式（需要支持<format>库）
    // return std::format("{} is {} years old", name, age);
    
    // 使用stringstream的替代方法
    stringstream ss;
    ss << name << " is " << age << " years old";
    return ss.str();
}
```

注意事项：
- C++17标准库没有内建的字符串split、join功能，需要自行实现
- C++20引入了`starts_with`、`ends_with`方法和`<format>`库，但在竞赛中可能不可用
- 处理大量字符串连接时，使用`stringstream`或`reserve`+`append`比反复`+`更高效
- 字符串查找失败时返回`string::npos`，记得检查这种情况
- C++中不同于某些语言，字符串是可变的，许多操作会直接修改原字符串

## 时间复杂度总结

下面是常见STL函数的时间复杂度：

### 容器操作

| 操作               | vector | deque  | list   | set/map | unordered_set/map |
|-------------------|--------|--------|--------|---------|-------------------|
| 随机访问 []        | O(1)   | O(1)   | O(N)   | N/A     | N/A               |
| 插入/删除（末尾）   | O(1)*  | O(1)   | O(1)   | O(logN) | O(1) 平均         |
| 插入/删除（开头）   | O(N)   | O(1)   | O(1)   | O(logN) | O(1) 平均         |
| 插入/删除（中间）   | O(N)   | O(N)   | O(1)** | O(logN) | O(1) 平均         |
| 查找               | O(N)   | O(N)   | O(N)   | O(logN) | O(1) 平均         |

\* 对于vector，如果需要扩容则为O(N)
\** 对于list，如果已知位置（迭代器）则为O(1)，否则需要O(N)查找

### 常用算法

| 算法               | 时间复杂度       | 空间复杂度    |
|-------------------|----------------|--------------|
| sort              | O(N log N)     | O(log N)     |
| stable_sort       | O(N log N)     | O(N) 或 O(1) |
| nth_element       | O(N) 平均      | O(1)         |
| binary_search     | O(log N)       | O(1)         |
| merge             | O(N+M)         | O(N+M)       |
| accumulate        | O(N)           | O(1)         |
| find/count        | O(N)           | O(1)         |
| min/max_element   | O(N)           | O(1)         |
| copy/transform    | O(N)           | O(1)         |
| unique            | O(N)           | O(1)         |
| next_permutation  | O(N)           | O(1)         |

## 实用技巧与最佳实践

### 1. 使用合适的容器

- 需要频繁随机访问元素：`vector`
- 需要频繁在两端操作：`deque`
- 需要频繁在中间插入/删除：`list`
- 需要保持元素排序：`set`, `map`
- 需要快速查找但不关心顺序：`unordered_set`, `unordered_map`

### 2. 迭代器优化

```cpp
// 避免重复计算end()
vector<int> v = {1, 2, 3, 4, 5};
// 较差的方式
for (int i = 0; i < v.size(); ++i) {
    // v.size() 每次循环都会调用
}

// 更好的方式
for (vector<int>::size_type i = 0, size = v.size(); i < size; ++i) {
    // size只计算一次
}

// 现代C++的方式
for (const auto& item : v) {
    // 使用范围for循环
}

// 迭代器失效问题
auto it = v.begin();
v.erase(it);     // it现在无效
it = v.begin();  // 重新获取有效迭代器
```

### 3. 算法组合技巧

```cpp
// 寻找前k大/小元素
vector<int> v = {5, 2, 8, 1, 9, 3, 7, 6, 4};
int k = 3;

// 第一种方法：排序后取前k个
sort(v.begin(), v.end());
// 前k小: v[0]到v[k-1]

// 第二种方法：使用nth_element更高效
nth_element(v.begin(), v.begin() + k - 1, v.end());
// 现在前k个元素是最小的k个（不一定有序）

// 第三种方法：使用partial_sort
partial_sort(v.begin(), v.begin() + k, v.end());
// 前k个元素是有序的最小k个

// 查找特定范围的元素
auto range = equal_range(v.begin(), v.end(), 5);
bool found = (range.first != range.second);
```

### 4. 使用合适的函数

```cpp
// 计算乘积：使用accumulate而非循环
vector<int> v = {1, 2, 3, 4, 5};
int product = accumulate(v.begin(), v.end(), 1, multiplies<int>());

// 高效寻找最大值和位置
vector<int> v = {5, 2, 8, 1, 9, 3};
auto max_it = max_element(v.begin(), v.end());
int max_value = *max_it;
int max_position = distance(v.begin(), max_it);

// 去除重复元素（需要先排序）
sort(v.begin(), v.end());
auto new_end = unique(v.begin(), v.end());
v.erase(new_end, v.end());
```

### 5. 性能优化

```cpp
// 预分配容器空间
vector<int> v;
v.reserve(10000); // 避免频繁重新分配内存
for (int i = 0; i < 10000; ++i) {
    v.push_back(i); // 不会导致重新分配
}

// 批量插入
vector<int> src = {1, 2, 3, 4, 5};
vector<int> dest;
// 比循环中push_back更高效
dest.insert(dest.end(), src.begin(), src.end());

// 交换技巧清空容器
vector<int> v = {1, 2, 3, 4, 5};
vector<int>().swap(v); // 高效清空并释放内存
```

### 6. 常见错误避免

```cpp
// 避免在循环中调整容器大小
vector<int> v;
for (int i = 0; i < 10000; ++i) {
    v.push_back(i);  // 可能导致多次重新分配
}
// 修正：使用reserve()或resize()预先分配空间

// 避免迭代器失效
vector<int> v = {1, 2, 3, 4, 5};
// 错误方式
for (auto it = v.begin(); it != v.end(); ++it) {
    if (*it % 2 == 0) {
        v.erase(it);  // 迭代器失效!
    }
}
// 正确方式
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0) {
        it = v.erase(it); // erase返回下一个有效迭代器
    } else {
        ++it;
    }
}

// 或使用remove_if和erase的组合
v.erase(
    remove_if(v.begin(), v.end(), [](int n){ return n % 2 == 0; }),
    v.end()
);
```

---

这份指南涵盖了ACM竞赛中最常用的STL函数及其用法。通过熟练掌握这些函数，你可以用更少的代码解决更复杂的问题，并且提高代码的效率和可读性。在实际竞赛中，选择合适的算法和数据结构往往比编写复杂的自定义实现更为明智。
