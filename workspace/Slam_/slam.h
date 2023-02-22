#ifndef SLAM_TEST
#define SLAM_TEST
#include <vector>
// 静态变量 继承 虚函数 纯虚函数
// 可见性(private public protected(子类可以访问)  firend)
// constexp std::array
//宽字符 const(类中使用const） mutable lambda
//成员初始化  三元操作符 类初始化(new和malloc的区别)
// explicit（禁止隐式转换）
//运算符重载 std::ostream
// this关键字

// 堆栈作用于 unique_str超出作用于后自行销毁（智能指针的内容）
//智能指针本质是对原始指针的包装 <memory>)
// unique_str(如果超出作用域自行销毁且单独唯一)make_unique
// shared_ptr make_shared weak_ptr
// 复制与拷贝构造函数 浅拷贝和深拷贝

// STL
// std::vector emplace_back
// 处理多返回值 tuple<utility> pair
// 模板类 堆栈的区别
// c++的宏(关键)
// auto关键字（using typedef）
// std::array和数组改用哪个
// c++函数指针 匿名函数lambda
// namespace c++线程
// std::sort union
// 虚析构函数从 c++类型转换
// c++的类型转换 四个cast
// 左值右值 移动语意和std::move
class slam_ {
 private:
  /* data */
 public:
  static int x, y;
  slam_(/* args */);
  ~slam_();
};

void slam_test();
void showss(std::vector<int>& b);
#endif
