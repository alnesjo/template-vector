#include "vector.hpp"             // inkludera din headerfil här
#include <assert.h>             // assert(b) ger felmeddelande om b falsk
#include <iostream>

using std::cout;
using std::endl;
using alnesjo::vector;

int main() {
  vector<int> v({1,2,3,4});
  for (auto & elem : v) {
    cout << elem << " ";
  }
  cout << endl;
  assert(v[0]==1);
  assert(v[1]==2);
  assert(v[2]==3);
  assert(v[3]==4);
  cout << "OK: il-constructor" << endl;

  auto rit = v.rbegin() - 2;
  cout << *rit << endl;
  assert(*rit==2);
  cout << "OK: reverse iterator" << endl;

  {
    bool caught = false;
    try {
      v.insert(6, 1);
    } catch (std::out_of_range e) {
      caught = true;
      cout << e.what() << endl;
    }
    assert(caught);
    cout << "OK: out_of_range" << endl;
  }

  v.insert(0, 1);
  assert(v[0]==1);
  v.insert(0, 2);
  assert(v[1]==1);
  v.insert(0, 3);
  assert(v[1]==2);
  v.insert(0, 4);
  assert(v[1]==3);
  for (auto & elem : v) {
    cout << elem << " ";
  }
  cout << endl;
  cout << "OK: insert" << endl;

  v.erase(3);
  v.erase(2);
  v.erase(1);
  v.erase(0);
  for (auto & elem : v) {
    cout << elem << " ";
  }
  cout << endl;
  cout << "OK: erase" << endl;
  {
    vector<int> u = v;
    for (auto & elem : u) {
      cout << elem << " ";
    }
    cout << endl;
    assert(v[0]==u[0]);
    assert(v[1]==u[1]);
    assert(v[2]==u[2]);
    assert(v[3]==u[3]);
    cout << "OK: copy" << endl;
  }

  {
    auto f1 = v.find(1);
    cout << "find(1) " << f1 << endl;
    auto f5 = v.find(5);
    cout << "find(5) " << f5 << endl;
    assert(f1==v.begin());
    assert(f5==v.end());
    cout << "OK: find" << endl;
  }
  return 0;
}
