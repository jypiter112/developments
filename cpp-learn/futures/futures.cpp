#include <iostream>
#include <functional>
#include <deque>
/*
 * Retrieve result value or exception from a function
 * That is being called, or has executed in a different thread
 * */

void fun1(){
  std::cout << "hello\n";
}
void _do(::std::function<void()> fun){
  fun();
  fun();
}

int main (int argc, char *argv[]) {
  auto print_list = [](std::deque<int> l){
    std::cout << "[list]\n";
    for(auto e : l)
      std::cout << e << ",";
    std::cout << "\n";
  };
  std::deque<int> list {1,2,3,4};
  list.insert(list.begin()+2, 22);
  print_list(list);
  list.pop_front();
  print_list(list);
  list.erase(list.begin() + 1);
  print_list(list);
  list.push_front(11);
  print_list(list);

  _do(fun1);

  return 0;
}
