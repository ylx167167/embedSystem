#ifndef TCPP_H
#define TCPP_H
#include <string.h>
#include <iostream>
#include <vector>
#include <memory>
#include <utility>

void dtest();
// namespace Appled {
// int dd;
// }
class Others {
 private:
  /* data */
 public:
  Others(/* args */);
  ~Others();
  void usingTest();
  void dtest();
};
//模板类实现一个动物的叫声
template <typename T, int N>
class Group {
 private:
  T m_Group[N];

 public:
  void printf(T& type) { std::cout << type << std::endl; }

  int Getsize() {
    std::cout << "总数为" << N << std::endl;
    return N;
  }
};


//动物类
class Animal {
 protected:
  int age;
  std::string name;
  char* home;

 public:
  // =0 说明这是一个纯虚基类 不能实例化
  virtual void Animal_action();  //每个动物的运动方式
  virtual void Animal_eat();     //每个动物吃的东西不一样
  Animal(/* args */);
  virtual ~Animal();  //虚析构函数
};

class Pepole : public Animal {
 private:
  typedef struct bankcount {
    std::string key;
    std::string count;
  } bankcount;
  bankcount count;
  char* language;

 public:
  virtual void Animal_action() override;
  virtual void Animal_eat() override;
  void setlanguage(std::string language);
  void haveIq() { std::cout << count.key << std::endl; }
  std::pair<std::string, std::string> getcountpair();
  friend std::ostream& operator<<(std::ostream& stream, const Pepole& p);

  Pepole(std::string& c, std::string& k);
  Pepole(/* args */);
  ~Pepole();
  // Pepole(Pepole& other);  //拷贝构造函数 克隆人
};

class Dog : public Animal {
 private:
  Pepole* master;  //主人是哪位

 public:
  virtual void Animal_action() override;
  virtual void Animal_eat() override;
  Dog();
  Dog(Pepole* p);
  ~Dog();
};


//单例模式实现一个银行
class bank {
 private:
  // std::vector<Pepole> list;
  bank(/* args */){};
  static bank* m_instance;  // 静态变量一定要初始化
 public:
  static bank* GetInsance() {
    if (m_instance == NULL) {
      m_instance = new bank();
    }
    return m_instance;
  }
  void DestroyEntity() {
    if (m_instance != NULL) {
      delete m_instance;
      m_instance = NULL;
    }
  }
  ~bank(){};
};

#endif