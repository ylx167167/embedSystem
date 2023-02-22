#include "tcpp.h"
#include <iostream>

bank* bank::m_instance = NULL;

void Others::usingTest() {
  // namespace a = Appled;
  // a::color = 1;
  // printf("%d\r\n", a::color);
}


Animal::Animal(/* args */) { std::cout << "我是一个动物" << std::endl; }
Animal::~Animal() { std::cout << "我这个动物消失了" << std::endl; }
void Animal::Animal_action() {}
void Animal::Animal_eat() { std::cout << "我不知道我干嘛的" << std::endl; }


Pepole::Pepole(/* args */) {
  // count.key = std::string("haha");
}
Pepole::Pepole(std::string& c, std::string& k) {
  count.count = c;
  count.key = k;
}
// Pepole::Pepole(Pepole& other) {
//   count.count = other.count.count;
//   count.key = other.count.key;
// }
void Pepole::setlanguage(std::string temp) {
  int size = temp.size();
  language = new char[size];
  memcpy(language, temp.c_str(), size);
}

Pepole::~Pepole() {}
void Pepole::Animal_action() {}
void Pepole::Animal_eat() { std::cout << "我吃饭" << std::endl; }
std::pair<std::string, std::string> Pepole::getcountpair() {
  return std::pair<std::string, std::string>(count.key, count.count);
}

std::ostream& operator<<(std::ostream& stream, const Pepole& p) {
  stream << "p.language :" << p.language
         << " addr :" << static_cast<void*>(p.language);
  return stream;
}
Dog::Dog() {}
Dog::Dog(Pepole* p) : master(p) {}
Dog::~Dog() {}
void Dog::Animal_action() {}
void Dog::Animal_eat() {}

Others::Others(/* args */) {}
Others::~Others() {}

void dtest() {
  // namespace a = Appled;
  // a::color = 1;
  // printf("%d\r\n", a::color);
}

void Others::dtest() {
  Animal a1;
  Pepole p1;
  //智能指针
  std::unique_ptr<Pepole> p2(new Pepole());
  // p2 = p1; 错误 不能指向
  std::shared_ptr<Pepole> p3(new Pepole());
  std::shared_ptr<Pepole> p4 = p3;
  std::cout << p4.use_count() << p3.use_count() << std::endl;
  std::weak_ptr<Pepole> p5 = p4;
  p4 = (p5.lock());  //返回share_ptr指针
  std::cout << p4.use_count() << std::endl;
  std::cout << "=========================" << std::endl;
  // std::pair std::tuple
  // std::shared_ptr<Pepole> p6(
  //     new Pepole(new std::string("123456"), new std::string("2313")));
  std::string s1("123456");
  std::string s2("2313");
  std::shared_ptr<Pepole> p6(new Pepole(s1, s2));
  auto p6inf = p6->getcountpair();
  std::cout << p6inf.first << std::endl;
  std::tuple<Pepole, int, int> t1(p1, 10, 10);
  std::get<1>(t1) = 9;
  std::cout << std::get<2>(t1) << std::get<1>(t1) << std::endl;
  std::cout << "=========================" << std::endl;
  // const
  char buf[] = {'a', 'b', 'c'};
  char buf2[] = {'a', 'b', 'c'};
  const char* c1 = buf;
  char* const c2 = buf;
  // c2 = buf2;  //错误
  c1 = buf2;
  // c1[2] = '2';  //错误
  c2[2] = '2';
  // char* const c2 = ;
  std::cout << sizeof(buf) << std::endl;
  std::cout << "=========================" << std::endl;
  Pepole p7;
  Pepole p8 = p7;
  p7.setlanguage(std::string("chinese"));
  std::cout << p7 << std::endl;
  std::cout << p8 << std::endl;
  std::cout << "=========================" << std::endl;
  // Animal* An = (Animal*)&p1;
  // Animal* An = static_cast<Animal*>(&p1);
  //子类尽量不要指向父类 static_cast会做类型检测 reinterpret_cast不会
  Pepole* An = static_cast<Pepole*>(&a1);
  // int* An = reinterpret_cast<int*>(&a1);
  // An->haveIq();
  // An->Animal_eat();
  Dog d(&p1);
  Group<Dog, 5> doggroup;
  doggroup.Getsize();
  // Dog d1;
}