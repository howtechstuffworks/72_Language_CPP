#include <iostream>
#include <memory>

class A {
public:
  A() { std::cout<<"Constructor called"<<std::endl; }
  ~A() { std::cout<<"Destructor called"<<std::endl; }
  void show() { std::cout<<"A::show()"<<std::endl; }
};

void foo(std::unique_ptr<A> foo_ptr) {
  foo_ptr->show();
  std::cout<<"Foo ptr:"<<foo_ptr.get()<<std::endl;
}

int main() {
  std::cout<<"Unique ptr"<<std::endl;

  std::unique_ptr<A> p1(new A());
  p1->show();
  std::cout<<"P1: "<<p1.get()<<std::endl;

  std::unique_ptr<A> p2 = std::move(p1);
  p2->show();
  std::cout<<"P1: "<<p1.get()<<std::endl;
  std::cout<<"P2: "<<p2.get()<<std::endl;

  foo(std::move(p2));
  std::cout<<"P1: "<<p1.get()<<std::endl;
  std::cout<<"P2: "<<p2.get()<<std::endl;
}