#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

#define LEFT 0 
#define UP 1
#define DIAGONAL 2

#define MAXLENGTH 2002
typedef struct {
  int left_bound[2 * MAXLENGTH];
  int right_bound[2 * MAXLENGTH];
} Path;

typedef struct {
  int lcs_length;
  int direction;
} DP_Entry;



DP_Entry dptable[2 * MAXLENGTH][MAXLENGTH];
Path allPaths[MAXLENGTH];

/*int dptable[2048][2048];

int LCS(string A, string B) {
  int m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= m; i++) dptable[i][0] = 0;
  for (j = 0; j <= n; j++) dptable[0][j] = 0;
  
  for (i = 1; i <= m; i++) {
    for (j = 1; j <= n; j++) {
      dptable[i][j] = max(dptable[i-1][j], arr[i][j-1]);
      if (A[i-1] == B[j-1]) dptable[i][j] = max(dptable[i][j], arr[i-1][j-1]+1);
    }
  }
  
  return dptable[m][n];
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
bool isValidNode(int row, int column, Path *upperBound, Path *lowerBound) {
  if ((column > upperBound->right_bound[row]) || (column < lowerBound->left_bound[row])) {
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

  Path result;
  for (int i = start_row; i <= dest_row; i++) {
    result.left_bound[i] = MAXLENGTH;
    result.right_bound[i] = 0;
  } 

  int current_row = dest_row;
  int current_column = dest_column;
  while ((current_row != start_row) || (current_column != start_column)) {
    result.left_bound[current_row] = min(result.left_bound[current_row],
                                          current_column);

    result.right_bound[current_row] = min(result.right_bound[current_row],
                                          current_column);

    if (dptable[current_row][current_column].direction ==UP) {
      current_row--;
    } else if (dptable[current_row][current_column].direction == LEFT) {
      current_column--;
    } else if (dptable[current_row][current_column].direction == DIAGONAL) {
      current_row--;
      current_column--;
    }
  }
  return result;
}


/*
  Given:
    - strings A and B, the strings we want to find CLCS to.
    - cutPlaceA, the location where we cut string A. String B will never be cut.
    - upperBound and lowerBound represent the upper and lower paths that we can't cross
  Returns the shortest path information from (cutPlaceA, 0) to (cutPlace + strlen(A), strlen(B)).
 */
Path singleShortestPath(string A, string B, int cutPlaceA, Path *upperBound, Path *lowerBound,
                        int *lcsLength) {

  // First compute the DP table from (cutPlaceA, 0) to (cutPlace + strlen(A), strlen(B)), without
  // ever crossing the path boundaries.


  int m = A.length(), n = B.length();
  int i, j;
  for (i = 0; i <= 2 * m; i++){
     dptable[i][0].lcs_length = 0;
     dptable[i][0].direction = UP;

  }
  for (j = 0; j <= n; j++) {
      dptable[cutPlaceA][j].lcs_length = 0;
      dptable[cutPlaceA][j].direction = LEFT;
  }
  
  for (i = 1; i <= m; i++) {
    for (j = lowerBound->left_bound[cutPlaceA + i]; j <= upperBound->right_bound[cutPlaceA + i]; j++) {
      dptable[cutPlaceA + i][j].lcs_length = 0;

      if (isValidNode(cutPlaceA+i-1,j,upperBound, lowerBound) &&
            (dptable[cutPlaceA+i-1][j].lcs_length > dptable[cutPlaceA + i][j].lcs_length)) {
        dptable[cutPlaceA + i][j].lcs_length = dptable[cutPlaceA+i-1][j].lcs_length;
        dptable[cutPlaceA + i][j].direction = UP;
      }

      if (isValidNode(cutPlaceA+i,j-1,upperBound, lowerBound) &&
            (dptable[cutPlaceA+i][j-1].lcs_length > dptable[cutPlaceA + i][j].lcs_length)) { 
        dptable[cutPlaceA + i][j].lcs_length =  dptable[cutPlaceA+i][j-1].lcs_length;
        dptable[cutPlaceA + i][j].direction = LEFT;
      }


      if (A[(cutPlaceA+i-1) % m] == B[j-1]) {
        int diag_value = 0;
        if (isValidNode(cutPlaceA+i-1,j-1,upperBound, lowerBound) ) { 
          diag_value= dptable[cutPlaceA+i-1][j-1].lcs_length+1;

          if (diag_value > dptable[cutPlaceA + i][j].lcs_length) {
            dptable[cutPlaceA+i][j].lcs_length = diag_value;
            dptable[cutPlaceA+i][j].direction = DIAGONAL;
          }
        } 


      }
    }
  }
  
  (*lcsLength) = dptable[cutPlaceA + m][n].lcs_length;
  // From the DP table, reconstructPathFromTable.

  Path result = reconstructPathFromTable(cutPlaceA, 0, cutPlaceA + m, n);

  // return a path.

  return result;
}


int findCLCSLength(string A, string B, int left_bound, int right_bound) {
  int mid = (left_bound + right_bound) / 2;


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
    for (int i = 0; i <= 2 * A.length(); i++) {
      for (int j = 0; j <= B.length(); j++) {
        allPaths[i].left_bound[j] = 0;
        allPaths[i].right_bound[j] = 0; 
      }
    }
  
    // Find the path starting from index i = 0, with no boundaries.
    Path upper;
    Path lower;
    for (int i = 0; i <= 2 * A.length(); i++) {
      upper.left_bound[i] = B.length();
      upper.right_bound[i] = B.length();
      lower.left_bound[i] = 0;
      lower.right_bound[i] = 0; 
    }

    
    for (int i = 0; i <= 2 * A.length(); i++)
      dptable[i][0].lcs_length = 0;

    int length = 0;
    allPaths[0] = singleShortestPath(A, B, 0, &upper, &lower, &length);
    allPaths[A.length()] = allPaths[0]; 
    /*
    for (int i = 0; i < A.length(); i++) {
      int length = 0;
      singleShortestPath(A, B, i, &upper, &lower, &length);
      clcs_result = max(length, clcs_result);
    }
    
    int length = 0;
    Path cut_zero = singleShortestPath(A, B, 0, upper, lower, &length);
    Path cut_m;
    for (int i = 0; i < B.length(); i++) {
      cut_m.left_bound[i] = cut_zero.upper_row[i] + A.length();
      cut_m.right_bound[i] = cut_zero.lower_row[i] + A.length();
    }

    clcs_result = findCLCSLength(A, B, 0, A.length());*/
    cout << clcs_result << endl; 
  }
  return 0;
}

