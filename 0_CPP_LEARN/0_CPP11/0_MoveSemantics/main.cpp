#include <iostream>

// 1. Lvalue and RValue
int foo_fn() {
  int num = 10;
  return num;
}

int& foo_fn_ref(int& a) {
  return a;
}

void UnderstandingLandRValue() {
  // Foo is Lvalue, 1 is rvalue
  int foo = 1;

  // foo is lvalue, foo_fn can only return rvalue
  foo = foo_fn();

  // Ref Val is 10 here.
  int ref_val = 10;
  // Ref_val now is 20, because the ref is passed to func and set on this line.
  foo_fn_ref(ref_val) = 20;

  int dummy = 30;
}

int main() {
  std::cout<<"Move semantics"<<std::endl;
  UnderstandingLandRValue();
  return 0;
}
