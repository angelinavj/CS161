#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
#define TC 5

int main() {

  cout << TC << endl;
  srand(time(NULL));
  for (int i = 0; i < TC; i++) {
    
    for (int j = 0; j < 2000; j++) {
       cout << (char)((int)('A') + (rand() % 26));
    }

    cout << " " ;

    for (int j = 0; j < 2000; j++) {

       cout << (char)((int)('A') + (rand() % 26));
    }

    cout << endl;
  }
}
