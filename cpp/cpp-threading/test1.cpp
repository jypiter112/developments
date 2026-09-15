#include <bits/stdc++.h>
using namespace std;

void fun(){
  cout << "Hello concurrency" << endl;
}

int main (int argc, char *argv[]) {
  thread t(fun);

  // main thread waits for t to finish
  if(t.joinable())
    t.join();

  cout << "T finished\n";
  return 0;
}
