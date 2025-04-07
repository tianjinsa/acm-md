# C++ String 类完全指南

C++ 标准库中的 `string` 类是字符串操作的核心工具。本指南全面介绍其功能和用法，帮助你高效处理字符串。

## 基本操作

### 构造与初始化

```cpp
// 多种初始化方式
string s1;                         // 空字符串
string s2("Hello");                // C风格字符串初始化
string s3 = "Hello";               // 赋值初始化
string s4(5, 'a');                 // 创建字符串 "aaaaa"
string s5(s2, 1, 3);               // 从s2的位置1开始的3个字符 ("ell")
string s6(s2.begin() + 1, s2.end() - 1); // 使用迭代器 ("ell")

// C++11 统一初始化
string s7 = {"Hello"};
string s8{"World"};
```

### 赋值操作

```cpp
string s;
s = "Hello";                  // C风格字符串赋值
s = another_string;           // string对象赋值
s = 'A';                      // 单个字符赋值

// assign方法
s.assign("Hello");            // 替换内容
s.assign(5, 'a');             // 替换为5个'a'
s.assign(str, 2, 3);          // 替换为str从位置2开始的3个字符
s.assign(str.begin(), str.begin() + 3); // 使用迭代器范围赋值
```

### 访问元素

```cpp
string str = "Hello";
char c1 = str[0];             // 'H'，不检查边界
char c2 = str.at(1);          // 'e'，会检查边界，越界时抛出out_of_range异常

// C++11新增
char& front = str.front();    // 引用首字符 'H'
char& back = str.back();      // 引用尾字符 'o'

// C字符串转换
const char* c_str = str.c_str();  // 以空字符结尾的C风格字符串
const char* data = str.data();    // 获取内部数据指针(C++11前不保证空字符结尾)

// C++17 数据视图
string_view sv(str);          // 轻量级、非拥有的字符串视图
```

## 字符串操作

### 添加与连接

```cpp
string s = "Hello";
s += " World";                // 追加C风格字符串，现在s为"Hello World"
s += '!';                     // 追加字符，现在s为"Hello World!"

// append方法
s.append(" Welcome");         // 追加字符串
s.append(3, '!');             // 追加3个感叹号
s.append(another_str, 0, 5);  // 追加another_str的前5个字符

// 单字符添加
s.push_back('.');             // 在末尾添加一个点，C++11

// 字符串连接
string s1 = "Hello";
string s2 = "World";
string s3 = s1 + " " + s2;    // "Hello World"
```

### 插入操作

```cpp
string s = "Hello";
s.insert(5, " World");        // 在位置5插入字符串，结果"Hello World"
s.insert(5, 3, '!');          // 在位置5插入3个'!'，结果"Hello!!! World"
s.insert(0, another_str, 0, 3); // 在开头插入another_str的前3个字符

// 使用迭代器插入
auto it = s.begin() + 5;
s.insert(it, 'X');            // 在位置5插入字符'X'
s.insert(it, 3, 'Y');         // 在位置5插入3个'Y'
```

### 删除操作

```cpp
string s = "Hello World";
s.erase(5, 1);                // 删除位置5的一个字符(空格)，结果"HelloWorld"

// 使用迭代器删除
auto it = s.begin() + 5;
s.erase(it);                  // 删除迭代器指向的字符
s.erase(s.begin(), s.begin() + 5); // 删除范围，结果为"World"

// C++11 
s.pop_back();                 // 删除最后一个字符
s.clear();                    // 清空整个字符串
```

### 替换操作

```cpp
string s = "Hello World";
s.replace(0, 5, "Hi");        // 用"Hi"替换前5个字符，结果"Hi World"
s.replace(3, 0, "Beautiful "); // 在位置3插入字符串，结果"Hi Beautiful World"
s.replace(s.begin(), s.begin() + 2, "Hey"); // 使用迭代器替换，结果"Hey Beautiful World"
```

### 子串提取

```cpp
string s = "Hello World";
string sub = s.substr(0, 5);  // "Hello"，从位置0开始的5个字符
string sub2 = s.substr(6);    // "World"，从位置6到结尾
```

## 字符串信息与搜索

### 大小与容量

```cpp
string s = "Hello World";
size_t len = s.length();      // 11，字符串长度
size_t size = s.size();       // 11，同length()
bool empty = s.empty();       // false，检查是否为空

// 容量管理
size_t cap = s.capacity();    // 获取当前容量（可能大于size）
s.reserve(100);               // 预留至少100个字符的空间
s.shrink_to_fit();            // 释放多余容量，C++11
s.resize(5);                  // 调整大小为5，结果"Hello"
s.resize(10, '*');            // 调整大小为10，不足部分用*填充，结果"Hello*****"
```

### 查找与搜索

```cpp
string s = "Hello World, Hello C++";

// 正向查找
size_t pos1 = s.find("Hello");       // 0，第一次出现的位置
size_t pos2 = s.find("Hello", 1);    // 13，从位置1开始查找
size_t pos3 = s.find('W');           // 6，查找字符
size_t pos4 = s.find_first_of("aeiou"); // 1，第一个元音字母的位置('e')

// 反向查找
size_t rpos1 = s.rfind("Hello");     // 13，最后一次出现的位置
size_t rpos2 = s.find_last_of("aeiou"); // 19，最后一个元音字母的位置('e')

// 查找不在集合中的字符
size_t pos5 = s.find_first_not_of("Helo "); // 6，第一个不是"Helo "中字符的位置('W')
size_t pos6 = s.find_last_not_of("+ "); // 19，最后一个不是"+ "中字符的位置('e')

// 检查是否找到
if (s.find("Java") == string::npos) {
    cout << "未找到Java" << endl;
}
```

### 比较操作

```cpp
string s1 = "Apple";
string s2 = "Banana";

// 比较方法
int comp1 = s1.compare(s2);           // 负值，s1 < s2
int comp2 = s1.compare(0, 1, "A");    // 0，s1的第一个字符等于"A"
int comp3 = s1.compare(0, 3, s2, 0, 3); // 比较部分子串

// 比较运算符
bool b1 = (s1 == s2);  // false
bool b2 = (s1 != s2);  // true
bool b3 = (s1 < s2);   // true，按字典序比较
bool b4 = (s1 > s2);   // false

// C++20 前缀后缀检查
// bool b5 = s1.starts_with("App");  // true，C++20
// bool b6 = s1.ends_with("le");     // true，C++20

// C++20之前的前缀后缀检查实现
bool starts_with(const string& str, const string& prefix) {
    return str.size() >= prefix.size() && 
           str.compare(0, prefix.size(), prefix) == 0;
}

bool ends_with(const string& str, const string& suffix) {
    return str.size() >= suffix.size() && 
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}
```

## 高级功能

### 字符串与数值转换 (C++11)

```cpp
// 【字符串转数值】- ACM比赛常用功能
string num_str = "123.456";

// 整数转换 - 掌握这三个基本够用
int i = stoi("42");              // 字符串转int: 42
long l = stol("1234567890");     // 字符串转long
long long ll = stoll("1234567890123456789"); // 字符串转long long (ACM中常用)

// 浮点数转换 - 记住这两个最常用
double d = stod("2.71828");      // 字符串转double (ACM中常用)
float f = stof("3.14");          // 字符串转float

// 其他整数转换函数
unsigned long ul = stoul("12345");
unsigned long long ull = stoull("12345678901234567890");

// 进制转换 - ACM比赛非常实用
// 第二个参数是指针，用于存储成功解析的字符数量
// 第三个参数是进制，默认为10
size_t pos = 0;
int hex_value = stoi("0xFF", &pos, 16);  // 十六进制转换: 255, pos = 4
int bin_value = stoi("101010", nullptr, 2);  // 二进制转换: 42
int oct_value = stoi("052", nullptr, 8);  // 八进制转换: 42

// 错误处理 - 别忘了这点！
try {
    int invalid = stoi("not_a_number");  // 会抛出std::invalid_argument异常
    int overflow = stoi("999999999999");  // 太大的数会抛出std::out_of_range异常
} catch (const invalid_argument& e) {
    cout << "无效输入: " << e.what() << endl;
} catch (const out_of_range& e) {
    cout << "数值超出范围: " << e.what() << endl;
}

// 【数值转字符串】- 简单好记
string s1 = to_string(42);       // 整数转字符串: "42"
string s2 = to_string(3.14159);  // 浮点数转字符串: "3.14159"

// 【ACM比赛技巧】: 当需要控制输出格式时，使用stringstream比to_string更灵活
#include <iomanip>
#include <sstream>
ostringstream oss;
oss << fixed << setprecision(3) << 3.14159;  // 控制小数点后位数
string formatted = oss.str();    // "3.142"

// 输出特定进制
ostringstream hex_oss;
hex_oss << hex << uppercase << 255;  // 十六进制大写
string hex_str = hex_oss.str();      // "FF"

ostringstream bin_oss;
bin_oss << bitset<8>(42);            // 二进制，8位
string bin_str = bin_oss.str();      // "00101010"
```

**ACM比赛提示：**
1. `stoi`, `stod` 和 `to_string` 是最常用的转换函数，重点掌握
2. 处理大整数时，记得使用 `stoll` 而不是 `stoi`
3. 字符串数值转换错误会抛出异常，在稳定性要求高的场景需要处理
4. 自定义格式化数值时，使用 `stringstream` 比 `to_string` 更灵活
5. 进制转换功能在位运算和编码题中非常有用
```

### 字符串流操作

```cpp
#include <sstream>

// 字符串解析
string data = "123 3.14 Hello";
istringstream iss(data);
int i; double d; string s;
iss >> i >> d >> s;  // i=123, d=3.14, s="Hello"

// 字符串构建
ostringstream oss;
oss << "Value: " << 42 << ", Pi: " << 3.14;
string result = oss.str();  // "Value: 42, Pi: 3.14"

// 数值格式化
ostringstream oss2;
oss2 << fixed << setprecision(2) << 3.14159;  // 需要 #include <iomanip>
string formatted = oss2.str();  // "3.14"
```

### 内存管理与性能优化

```cpp
// 预分配内存避免频繁重新分配
string result;
result.reserve(1000);  // 预留1000个字符的空间
for (int i = 0; i < 100; i++) {
    result += "Some text ";  // 不会导致频繁重新分配内存
}

// 移除多余容量
result.shrink_to_fit();  // C++11

// 使用 swap 技巧释放内存
{
    string(result).swap(result);  // C++11前清空并最小化容量的技巧
    // C++11后可以直接用：result.clear(); result.shrink_to_fit();
}

// 移动语义(C++11)，避免不必要的拷贝
string build_message() {
    string msg = "Hello " + user_name + "!";
    return msg;  // 编译器可能使用移动而非拷贝
}

string message = std::move(build_message());  // 显式移动
```

## 实用案例

### 案例1: 分割字符串

```cpp
// 按分隔符分割字符串
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    istringstream tokenStream(s);
    string token;
    
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {  // 忽略空标记（如连续分隔符）
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

// 使用示例
string csv = "apple,banana,cherry,date";
vector<string> fruits = split(csv, ',');
// fruits包含: "apple", "banana", "cherry", "date"
```

### 案例2: 字符串替换

```cpp
// 替换字符串中的所有匹配项
string replace_all(string str, const string& from, const string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();  // 跳过刚刚插入的内容
    }
    return str;
}

// 使用示例
string text = "Hello [name], welcome to [city]!";
text = replace_all(text, "[name]", "Alice");
text = replace_all(text, "[city]", "Wonderland");
// 结果: "Hello Alice, welcome to Wonderland!"
```

### 案例3: URL编码/解码

```cpp
// URL编码
string url_encode(const string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (char c : value) {
        // 保留字母数字和一些特殊字符
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            // 其他字符编码为%XX形式
            escaped << uppercase;
            escaped << '%' << setw(2) << int((unsigned char)c);
            escaped << nouppercase;
        }
    }

    return escaped.str();
}

// URL解码
string url_decode(const string &encoded) {
    string result;
    for (size_t i = 0; i < encoded.length(); ++i) {
        if (encoded[i] == '%') {
            if (i + 2 < encoded.length()) {
                int value;
                istringstream is(encoded.substr(i + 1, 2));
                if (is >> hex >> value) {
                    result += static_cast<char>(value);
                    i += 2;
                } else {
                    result += '%';
                }
            } else {
                result += '%';
            }
        } else if (encoded[i] == '+') {
            result += ' ';
        } else {
            result += encoded[i];
        }
    }
    return result;
}
```

### 案例4: 文本修剪(Trim)

```cpp
// 去除字符串首尾的空白字符
string trim(const string& str) {
    // 找到第一个非空白字符
    const string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    
    // 如果字符串全是空白字符
    if (start == string::npos) 
        return "";
        
    // 找到最后一个非空白字符
    size_t end = str.find_last_not_of(whitespace);
    
    // 提取并返回子串
    return str.substr(start, end - start + 1);
}

// 去除字符串左侧空白
string ltrim(const string& str) {
    const string whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    return (start == string::npos) ? "" : str.substr(start);
}

// 去除字符串右侧空白
string rtrim(const string& str) {
    const string whitespace = " \t\n\r\f\v";
    size_t end = str.find_last_not_of(whitespace);
    return (end == string::npos) ? "" : str.substr(0, end + 1);
}
```

### 案例5: 大小写转换

```cpp
// 将字符串转换为大写
string to_upper(string str) {
    transform(str.begin(), str.end(), str.begin(), 
               [](unsigned char c){ return toupper(c); });
    return str;
}

// 将字符串转换为小写
string to_lower(string str) {
    transform(str.begin(), str.end(), str.begin(), 
               [](unsigned char c){ return tolower(c); });
    return str;
}

// 将字符串首字母大写
string capitalize(string str) {
    if (!str.empty()) {
        str[0] = toupper(str[0]);
    }
    return str;
}
```

## 常见错误与最佳实践

### 潜在问题

1. **越界访问** - 使用 `[]` 不进行边界检查，可能导致未定义行为
   ```cpp
   string s = "Hi";
   char c = s[10];  // 未定义行为! 应使用 s.at(10) 触发异常
   ```

2. **迭代器失效** - 修改字符串可能导致迭代器失效
   ```cpp
   string s = "Hello";
   auto it = s.begin() + 2;
   s += " World";   // 可能导致it失效
   *it = 'X';       // 危险操作，可能导致未定义行为
   ```

3. **忽略string::npos检查** - 未检查find结果
   ```cpp
   string s = "Hello";
   size_t pos = s.find("World");  
   string sub = s.substr(pos);    // 错误! 未检查pos是否为npos
   ```

4. **C字符串转换问题** - c_str()指针在string修改后可能失效
   ```cpp
   string s = "Hello";
   const char* ptr = s.c_str();
   s += " World";      // ptr可能失效
   cout << ptr;        // 不安全的操作
   ```

### 最佳实践

1. **使用合适的方法进行边界检查**
   ```cpp
   string s = "Hello";
   // 安全访问 - 越界会抛出异常
   try {
       char c = s.at(10);
   } catch (const out_of_range& e) {
       cerr << "越界访问: " << e.what() << endl;
   }
   ```

2. **字符串连接效率优化**
   ```cpp
   // 低效: 每次+=都可能重新分配内存
   string result;
   for (int i = 0; i < 1000; ++i) {
       result += to_string(i);
   }

   // 高效: 预先分配足够空间
   string result;
   result.reserve(10000);  // 预估大小
   for (int i = 0; i < 1000; ++i) {
       result += to_string(i);
   }

   // 或使用字符串流
   ostringstream oss;
   for (int i = 0; i < 1000; ++i) {
       oss << i;
   }
   string result = oss.str();
   ```

3. **安全地使用find结果**
   ```cpp
   string s = "Hello World";
   size_t pos = s.find("World");
   if (pos != string::npos) {
       string sub = s.substr(pos);  // 安全
   }
   ```

4. **使用string_view减少拷贝(C++17)**
   ```cpp
   // 使用string_view作为函数参数，避免字符串拷贝
   bool starts_with(string_view str, string_view prefix) {
       return str.substr(0, prefix.size()) == prefix;
   }
   ```

5. **避免不必要的临时对象**
   ```cpp
   // 低效: 创建多个临时string对象
   string result = string("Hello ") + string("World");

   // 高效: 直接使用字符串字面值
   string result = "Hello " + string("World");
   ```

## 总结

C++ `string` 类提供了丰富而强大的字符串处理功能。掌握这些操作可以帮助你更高效地处理各种文本处理任务。关键是理解字符串的内存管理方式，选择合适的方法进行操作，并注意潜在的陷阱。

随着C++标准的发展，string类不断获得新功能(如C++17的string_view和C++20的starts_with/ends_with方法)，使字符串处理变得更加便捷和高效。
