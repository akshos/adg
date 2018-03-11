#include <iostream>
#include <cstdlib>

#define ROW_SIZE 20
#define COL_SIZE 20
#define MAX_MAT_COUNT 20
#define INFINITY 62000

using namespace std;

int matrix[ROW_SIZE][COL_SIZE];
int P[MAX_MAT_COUNT];
int matCount, pCount;
unsigned int M[MAX_MAT_COUNT][MAX_MAT_COUNT], S[MAX_MAT_COUNT][MAX_MAT_COUNT];

void readData()
{
    cout << "Enter the number of matrixes : ";
    cin >> matCount;
    pCount = matCount+1;
    cout << "Enter " << pCount << " dimensions of the matrix set : " << endl;
    for( int i = 0; i < pCount; i++ )
    {
        cin >> P[i];
    }
}

void matrixChainOrder()
{  
    for( int i = 1; i <= matCount; i++ )
    {
        M[i][i] = 0;
    }
    for( int l = 2; l <= matCount; l++ )
    {
        for( int i = 1; i <= matCount-l+1; i++ )
        {
            int j = i + l -1;
            M[i][j] = INFINITY;
            for( int k = i; k <= j-1; k++ )
            {
                int q = M[i][k] + M[k+1][j] + (P[i-1]*P[k]*P[j]);
                if( q < M[i][j] )
                {
                    M[i][j] = q;
                    S[i][j] = k;
                }
            }
        }
    }
}

void printOptimalParens(int i, int j)
{
    if( i == j )
        cout << " A" << i << " ";
    else
    {
        cout << "(";
        printOptimalParens(i, S[i][j]);
        printOptimalParens(S[i][j]+1, j);
        cout << ")";
    } 
}

int main()
{
    readData();
    cout << "\nCalculating oprimal parenthezization\n\n";
    matrixChainOrder();
    printOptimalParens(1, matCount);
    cout << endl;
    return 0;
}