#include "kth_cprog_template_container.hpp"             // inkludera din headerfil här
#include <assert.h>             // assert(b) ger felmeddelande om b falsk
#include <iostream>

using namespace std;

int main()
{     
     typedef int value_type;
     Vector<value_type> v;
     v.clear();
     std::size_t count = 0;

     clog << "OK: void-konstruktor" << endl;
     
     assert(v.capacity() == 0);
     v.insert(0, 1);          // lägg till ett element sist 
     assert(v.capacity() == 1);
     v.insert(0, 2);          // lägg till före element 0, dvs först
     assert(v.capacity() == 2);
     v.insert(0, 3);
     assert(v.capacity() == 4);
     v.insert(2, 4);
     assert(v.capacity() == 4);
     
     for(auto & elem : v) clog << elem << " ";
     clog << endl;
     clog << "OK: insert" << endl;
     
     v.erase(1);
     v.erase(1);
     v.erase(1);
     v.erase(0);
     

     
     for(auto & elem : v) clog << elem << " ", count++;
     clog << endl;
     
     assert(count == v.size());
#if 0          
#ifdef SORT_DEFINED
     assert(v.size() == 2);      // nu ligger två element i vektorn
     v.sort(false);              // sortera i fallande ordning
     assert(v[0] == 3.14 &&      // hamnade de rätt?
	    v[1] == 2.10);       
     assert(v.size() == 2);      // ingenting ändrat?
     v[1] = 2.11;                // tilldelning av enstaka element;

     const Vector<double> &vc = v;  // skapa konstant referens
     assert(vc.size() == 2);     // ok: ändrar ej vektorn som är konstant
     assert(vc[0] == 3.14 &&     // ok: ändrar ej vektorn som är konstant
	    vc[1] == 2.11);
#endif    
     v.erase(0);                 // ta bort första elementet               
     assert(v.size() == 1);      // rätt antal elelment
     v.clear();                  // töm hela vektorn
     assert(v.size() == 0);      // tom när alla element är borttagna
    
#if 0     
     // kontrollera att följande rader inte går att kompilera
     vc[0] = 3.1415;             // fel: tilldelning av konstant objekt
     Vector<char> c = v;         // fel: tilldelning av olika typer
#endif
#endif
     return 0;
}
