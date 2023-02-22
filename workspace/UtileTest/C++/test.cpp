
#include <iostream>
/*
 *Filename         :test.cpp
 *Description      :
 *Author           :wayneyao
 *Email            :417838124@qq.com
 *Time             :2023/02/17 14:06:30 */
class test1
{
private:
    int a;

public:
    test1(int b);
    ~test1();
};

test1::test1(int b) : a(2)
{
    std::cout << TISE(a) << std::endl;
}
test1::~test1()
{
}
