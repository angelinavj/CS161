#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

#define LEFT 0 
#define UP 1
#define DIAGONAL 2

#define MAXLENGTH 2002
typedef struct {
  int upper_row[MAXLENGTH];
  int lower_row[MAXLENGTH];
} Path;

typedef struct {
  int lcs_length;
  int direction;
} DP_Entry;



DP_Entry dptable[2 * MAXLENGTH][MAXLENGTH];
Path allPaths[MAXLENGTH];

/*int arr[2048][2048];

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
}*/

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

/*
 * Given the upperbound and lower bound, returns true if the node, represented by row and column,
 * does not cross / in the middle of upperBound and lowerBound.
 */
bool isValidNode(int row, int column, Path upperBound, Path lowerBound) {
  if ((row < upperBound.upper_row[column]) || (row > lowerBound.lower_row[column])) {
    return false;
  }
  return true;
}

/* Given a DP table entry, starting node and destination node,
 * reconstruct the shortest path from start to destination.
 */
Path reconstructPathFromTable(int start_row, int start_column, int dest_row, int dest_column) {
  // trace back from dest to satart.
  // return a path

  Path temp;
  return temp;
}


/*
  Given:
    - strings A and B, the strings we want to find CLCS to.
    - cutPlaceA, the location where we cut string A. String B will never be cut.
    - upperBound and lowerBound represent the upper and lower paths that we can't cross
  Returns the shortest path information from (cutPlaceA, 0) to (cutPlace + strlen(A), strlen(B)).
 */
Path singleShortestPath(string A, string B, int cutPlaceA, Path upperBound, Path lowerBound,
                        int *lcsLength) {
  // First compute the DP table from (cutPlaceA, 0) to (cutPlace + strlen(A), strlen(B)), without
  // ever crossing the path boundaries.


  int m = A.length(), n = B.length();

  int i, j; 
  for (i = 0; i <= 2 * m; i++) dptable[i][0].lcs_length = 0;

  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      if (!isValidNode(cutPlaceA + i, j, upperBound, lowerBound)) 
        break;
     

      dptable[cutPlaceA + i][j].lcs_length = 0;

      if ((isValidNode(cutPlaceA + i - 1, j, upperBound, lowerBound)) && (i != 1)) {
        dptable[cutPlaceA + i][j].lcs_length = max (dptable[cutPlaceA + i][j].lcs_length,
                                                    dptable[cutPlaceA + i - 1][j].lcs_length);
      }

      if (isValidNode(cutPlaceA + i, j - 1, upperBound, lowerBound)) {
        dptable[cutPlaceA + i][j].lcs_length = max (dptable[cutPlaceA + i][j].lcs_length,
                                                    dptable[cutPlaceA + i][j- 1].lcs_length);
      }

      if (isValidNode(cutPlaceA + i - 1, j - 1, upperBound, lowerBound) && (i != 1)) {
        if (A[(cutPlaceA + i - 1) % m] == B[j-1]) {
          dptable[cutPlaceA + i][j].lcs_length = max (dptable[cutPlaceA + i][j].lcs_length,
                                                    dptable[cutPlaceA + i - 1][j- 1].lcs_length + 1);
        }
      } else {
          dptable[cutPlaceA + i][j].lcs_length = max (dptable[cutPlaceA + i][j].lcs_length,
                                                      1);
      }
    }
  }
  
  (*lcsLength) = dptable[cutPlaceA + m][n].lcs_length;

  // From the DP table, reconstructPathFromTable.
  
  // return a path.
  Path temp;
  return temp;
}


int findCLCSLength(string A, string B, int upper_row, int lower_row) {

  return 0;

  // int maxLength = 0;
  // singleShortestPath(A, B, m, p[upperrow], p[lowerrow], &maxLength);
  // return max (maxLength, max(findShortestPaths(A, B, allPaths, upperrow, m)
  //                              findShortestPaths(A, B, allPaths, m, lowerrow)))

}; 


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

    // Initialize allpaths.
    for (int i = 0; i <= A.length(); i++) {
      for (int j = 0; j <= B.length(); j++) {
        allPaths[i].upper_row[j] = 0;
        allPaths[i].lower_row[j] = 0; 
      }
    }
  
    // Find the path starting from index i = 0, with no boundaries.
    Path upper;
    Path lower;
    for (int i = 0; i < B.length(); i++) {
      upper.upper_row[i] = 0;
      upper.lower_row[i] = 0;
      lower.upper_row[i] = A.length();
      lower.lower_row[i] = A.length(); 
    }

    
    for (int i = 0; i < A.length(); i++) {
      int length = 0;
      singleShortestPath(A, B, i, upper, lower, &length);
      clcs_result = max(length, clcs_result);
    }
    /*
    int length = 0;
    Path cut_zero = singleShortestPath(A, B, 0, upper, lower, &length);
    Path cut_m;
    for (int i = 0; i < B.length(); i++) {
      cut_m.upper_row[i] = cut_zero.upper_row[i] + A.length();
      cut_m.lower_row[i] = cut_zero.lower_row[i] + A.length();
    }

    clcs_result = findCLCSLength(A, B, 0, A.length());*/
    cout << clcs_result << endl; 
  }
  return 0;
}

