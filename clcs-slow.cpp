#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

int arr[2048][2048];

int LCS(string A, string B) {
  int m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= m; i++) arr[i][0] = 0;
  for (j = 0; j <= n; j++) arr[0][j] = 0;
  
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      arr[i][j] = max(arr[i-1][j], arr[i][j-1]);
      if (A[i-1] == B[j-1]) arr[i][j] = max(arr[i][j], arr[i-1][j-1]+1);
    }
  }
  
  return arr[m][n];
}

int min(int a, int b) {
  if (a < b) return a;
  return b;
}

int max (int a, int b) {
  if (a > b) return a;
  return b;
}

string cut(string A, int cut_place) {
  return A.substr(cut_place) + A.substr(0, cut_place); 
}

int main() {
  int T;
  string A, B;

  cin >> T;
  for (int tc = 0; tc < T; tc++) {
    int clcs_result = 0;

    cin >> A >> B;
    if (min(A.length(), B.length()) == B.length()) {
      string temp = A;
      A = B;
      B = temp;
    }

    for (int i = 0; i < A.length(); i++) {
      string A_cut = cut(A, i);
      clcs_result = max(clcs_result, LCS(A_cut, B));
    }

    cout << clcs_result << endl;
  }
  return 0;
}

