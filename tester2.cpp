#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;
#define TC 100

int main() {

  cout << TC << endl;
  srand(time(NULL));
  for (int i = 0; i < TC; i++) {
    int lengthA = (rand() % 20) + 1;
    for (int j = 0; j < lengthA; j++) {
       cout << (char)((int)('A') + (rand() % 26));
    }

    cout << " " ;
    int lengthB = (rand() % 20) + 1;

    for (int j = 0; j < lengthB; j++) {

       cout << (char)((int)('A') + (rand() % 26));
    }

    cout << endl;
  }
}
