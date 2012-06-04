/*
  Group:
  -Grant Ho, SUID: grantho, SUID Number: 05653617
  -Angelina Veni Johanna, SUID: veni, SUID Number: 05651929
*/


#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

#define LEFT 0 
#define UP 1
#define DIAGONAL 2

#define MAXLENGTH 2002

/*
 * A path struct represents the path
 * boundaries that delimit the recursive
 * DP tables. The left_bound array represents the
 * farthest left we can go in row [index]; similar for the right_bound.
 * first_row and last_row allow us to keep track of the height of the path
 * boundary and whether or not we're in a region bounded by the path or not.
 */
typedef struct {
  int left_bound[2 * MAXLENGTH];
  int right_bound[2 * MAXLENGTH];
  int first_row;
  int last_row;
} Path;

/*
 * DP_Entry represents an entry in the DP table.
 * the direction field is used to reconstruct a path
 * so it can be passed as a boundary for recursive calls
 */
typedef struct {
  int lcs_length;
  int direction;
} DP_Entry;


/*
 * Global variables for the DP table and 
 * the array of all paths from all possible starting 
 * indexes [0, 2m]
 */
DP_Entry dptable[2 * MAXLENGTH][MAXLENGTH];
Path allPaths[2 * MAXLENGTH];

//Helper function to return minimum to two ints
int min(int a, int b) {
  if (a < b) return a;
  return b;
}

//Helper function to return maximum of two ints
int max (int a, int b) {
  if (a > b) return a;
  return b;
}

//Helper function to return the cut of a string
string cut(string A, int cut_place) {
  return A.substr(cut_place) + A.substr(0, cut_place); 
}

/*
 * Given the upperbound and lower bound, returns true if the node, represented by row and column,
 * does not cross (i.e. is within the boundary) of upperBound and lowerBound.
 */
bool isValidNode(int row, int column, Path *upperBound, Path *lowerBound) {
  if ((row >= upperBound->first_row) && (row <= upperBound->last_row)) {
    if (column > upperBound->right_bound[row]) return false;
  }

  if ((row >= lowerBound->first_row) && (row <= upperBound->last_row)) {
    if (column < lowerBound->left_bound[row]) return false;
  }
  return true;
}

/* Given the DP table coordinatos of the starting node and
 *  ending/destination node of a path reconstructPathFromTable
 * reconstruct the shortest path from start to destination.
 */
Path reconstructPathFromTable(int start_row, int start_column, int dest_row, int dest_column) {
  Path result;
  
  //Initialize the entries for the reconstructed path
  for (int i = start_row; i <= dest_row; i++) {
    result.left_bound[i] = MAXLENGTH;
    result.right_bound[i] = 0;
  } 
  result.first_row = start_row;
  result.last_row = dest_row;

  int current_row = dest_row;
  int current_column = dest_column;
  while (true) {
    result.left_bound[current_row] = min(result.left_bound[current_row],
                                          current_column);

    result.right_bound[current_row] = max(result.right_bound[current_row],
                                          current_column);

    //If we've reconstructed the path back to the start node, we're done
    if ( (current_row == start_row) && (current_column == start_column)) break;
    
    /*
     * The directions are relative to the destination node; in other words
     * the direction we go when we are reconstructing the path. The actual
     * path moves are in the "reverse direction" when the lcs is made.
     */
    //Optimal path took a down move/skipped letter in A 
    if (dptable[current_row][current_column].direction == UP) {
      current_row--;
    }
    //Optimal path took a right move/skipped letter in B
    else if (dptable[current_row][current_column].direction == LEFT) {
      current_column--;
    } 
    //Optimal path took a digonal move/matched letter in A and B
    else if (dptable[current_row][current_column].direction == DIAGONAL) {
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

  /*
   * Initialize the DP Table
   */
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
    
    /*
     * For each row, determine the exact layout of this subproblem's DP
     * table/how much of each row we need to look at by looking at what entries the path
     * boundaries mark off; if the current row is in between the lowest
     * row of the upper path boundary and the highest row of the lower
     * path boundary, then the entire row from [1, n] is valid and needs
     * to be filled in
     */
    int start_column;
    if ((cutPlaceA+i >= lowerBound->first_row) && (cutPlaceA+i <= lowerBound->last_row)) {
      start_column = max(lowerBound->left_bound[cutPlaceA + i], 1);
    } else {
      start_column = 1;
    }
     
    int end_column; 
    if ((cutPlaceA+i >= upperBound->first_row) && (cutPlaceA+i <= upperBound->last_row)) {
      end_column = max(upperBound->right_bound[cutPlaceA + i], 1);
    } else {
      end_column = n;
    }

    /*For row i (outer for loop), construct the DP table based on the path boundaries */
    for (j = start_column; j <= end_column; j++) {
      dptable[cutPlaceA + i][j].lcs_length = 0;

      //If moving down/skipping a letter in A is optimal
      //make it and update DP table
      if (isValidNode(cutPlaceA+i-1,j,upperBound, lowerBound) &&
            (dptable[cutPlaceA+i-1][j].lcs_length > dptable[cutPlaceA + i][j].lcs_length)) {
        dptable[cutPlaceA + i][j].lcs_length = dptable[cutPlaceA+i-1][j].lcs_length;
        dptable[cutPlaceA + i][j].direction = UP;
      }

      //If moving right/skipping a letter in B is optimal
      //make it and update DP table
      if (isValidNode(cutPlaceA+i,j-1,upperBound, lowerBound) &&
            (dptable[cutPlaceA+i][j-1].lcs_length > dptable[cutPlaceA + i][j].lcs_length)) { 
        dptable[cutPlaceA + i][j].lcs_length =  dptable[cutPlaceA+i][j-1].lcs_length;
        dptable[cutPlaceA + i][j].direction = LEFT;
      }

      /*If the letter in A and B match, check if making a diagonal move
       * will make the current path optimal and add it if it
       * does. Otherwise, make the optimal(right, down) move that was
       * computed in one of the two if statements above
       */
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
  
  //Update the optimal/max path length based on the move just made
  (*lcsLength) = dptable[cutPlaceA + m][n].lcs_length;
  
  // From the DP table, reconstructPathFromTable.
  Path result = reconstructPathFromTable(cutPlaceA, 0, cutPlaceA + m, n);
  
  return result;
}

/*
 * The recursive function that returns the CLCS for two strings A and B;
 * this function is the same as FindShortestPaths in the theory homework
 * but it's modified to return the overall maximum path length (CLCS
 * length) as opposed to all paths -- by updating the max CLCS length
 * after each optimal path is computed, we don't need to iterate over
 * all paths to find the max CLCS length
 */
int findCLCSLength(string A, string B, int low_index, int high_index) {
  if (high_index - low_index <= 1) {
    return 0;
  }
  int mid = (high_index + low_index) / 2;
  int length = 0;
  allPaths[mid] = singleShortestPath(A, B, mid, &allPaths[low_index], &allPaths[high_index], &length);
  return max(length,
            max (findCLCSLength(A, B, low_index, mid),
                  findCLCSLength(A, B, mid, high_index)));
}; 


int main() {
  int T;
  string A, B;

  cin >> T;
  for (int tc = 0; tc < T; tc++) {
    int clcs_result = 0;

    cin >> A >> B;

    //To make things easier to code, we always want A to be
    //the longer string -- if B happens to be the longer string
    //from the input file, we simply swap them interally and
    //pass them into our algorithm with new A as the longer string
    if (min(A.length(), B.length()) == B.length()) {
      string temp = A;
      A = B;
      B = temp;
    }

    //For every test case, re-initialize/empty the DP Table and Paths
    for (int i = 0; i <= 2 * A.length(); i++) {
      for (int j = 0; j <= B.length(); j++) {
        dptable[i][j].lcs_length = 0;
        dptable[i][j].direction = LEFT;
      }

      if (i > A.length()) continue;

      allPaths[i].first_row = i;
      allPaths[i].last_row = i + A.length();
      for (int j = allPaths[i].first_row; j <= allPaths[i].last_row; j++) {
        allPaths[i].left_bound[j] = 0;
        allPaths[i].right_bound[j] = B.length(); 
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

    upper.first_row = 0;
    upper.last_row = 2 * A.length();
    lower.first_row = 0;
    lower.last_row = 2 * A.length();
    
    for (int i = 0; i <= 2 * A.length(); i++)
      dptable[i][0].lcs_length = 0;

    int length = 0;

    //Compute the shortest path from [0,0]->[m,n]
    allPaths[0] = singleShortestPath(A, B, 0, &upper, &lower, &length);
    //Copy this shortest path to start at [m,0] to [2m, n]
    allPaths[A.length()+1] = allPaths[0]; 
    allPaths[A.length()+1].first_row = A.length();
    allPaths[A.length()+1].last_row = 2 * A.length();

    //Compute the CLCS from the DP table using the two paths above as boundaries
    clcs_result = max(length, findCLCSLength(A, B, 0, A.length()));
    cout << clcs_result << endl; 
  }
  return 0;
}

