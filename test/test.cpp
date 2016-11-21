#include <iostream>
#include <assert.h>
#include "vector.hpp"

using std::cout;
using std::endl;
using std::out_of_range;
using std::move;
using alnesjo::vector;

vector<int> v;

int main() {
  {
    v = vector<int>({1,2,3,4});
    assert(v[0]==1);
    assert(v[1]==2);
    assert(v[2]==3);
    assert(v[3]==4);
    cout << "OK: il-constructor" << endl;
  }
  {
    auto rit = v.rbegin() - 2;
    assert(*rit==2);
    cout << "OK: reverse iterator" << endl;
  }
  {
    bool caught = false;
    try {
      v.insert(6, 1);
    } catch (out_of_range e) {
      caught = true;
    }
    assert(caught);
    cout << "OK: out_of_range" << endl;
  }
  {
    v.insert(0, 1);
    assert(v[0]==1);
    v.insert(0, 2);
    assert(v[1]==1);
    v.insert(0, 3);
    assert(v[1]==2);
    v.insert(0, 4);
    assert(v[1]==3);
    cout << "OK: insert" << endl;
  }
  {
    v.erase(3);
    v.erase(2);
    v.erase(1);
    v.erase(0);
    cout << "OK: erase" << endl;
  }
  {
    vector<int> u(v);
    assert(v[0]==u[0]);
    assert(v[1]==u[1]);
    assert(v[2]==u[2]);
    assert(v[3]==u[3]);
    cout << "OK: copy" << endl;
    v = move(u);
    assert(v[0]==1);
    assert(v[1]==2);
    assert(v[2]==3);
    assert(v[3]==4);
    cout << "OK: move" << endl;
  }
  {
    auto f1 = v.find(1);
    auto f5 = v.find(5);
    assert(f1==v.begin());
    assert(f5==v.end());
    cout << "OK: find" << endl;
  }
  return 0;
}
