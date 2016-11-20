#include "vector.hpp"             // inkludera din headerfil här
#include <assert.h>             // assert(b) ger felmeddelande om b falsk
#include <iostream>

using std::clog;
using std::endl;

int main() {
     alnesjo::Vector<int> v;
     v.clear();
     std::size_t count = 0;

     clog << "OK: void-konstruktor" << endl;
     assert(v.capacity() == 0);
     v.insert(0, 1);
     assert(v.capacity() == 1);
     v.insert(0, 2);
     assert(v.capacity() == 2);
     v.insert(0, 3);
     assert(v.capacity() == 4);
     v.insert(2, 4);
     assert(v.capacity() == 4);

     for (auto & elem : v) {
          clog << elem << " ";
     }
     clog << endl;
     clog << "OK: insert" << endl;

     v.erase(1);
     v.erase(1);
     v.erase(1);
     v.erase(0);

     for (auto & elem : v) {
          clog << elem << " ", count++;
     }
     clog << endl;
     clog << "OK: erase" << endl;

     assert(count == v.size());
     return 0;
}
