/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
	if (M == 32 && N == 32) {
		int i,j,i1,j1;
		int temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
		for (i=0;i<N;i+=8) {
			for (j=0;j<M;j+=8){
				for (i1=i;i1<i+8;i1++) {
					if (i==j){
					temp1=A[i1][j];temp2=A[i1][j+1];temp3=A[i1][j+2];temp4=A[i1][j+3];
					temp5=A[i1][j+4];temp6=A[i1][j+5];temp7=A[i1][j+6];temp8=A[i1][j+7];
					B[j][i1]=temp1;B[j+1][i1]=temp2;B[j+2][i1]=temp3;B[j+3][i1]=temp4;
					B[j+4][i1]=temp5;B[j+5][i1]=temp6;B[j+6][i1]=temp7;B[j+7][i1]=temp8;
					continue;
					}
					for (j1=j;j1<j+8;j1++){
						B[j1][i1]=A[i1][j1];
					}
				}
			}
		}
	}
	else if (M == 64 && N == 64) {
		int i,j,i1,j1;
		int temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
		for (i=0;i<N;i+=8) {
			for (j=0;j<M;j+=8){
				for (i1=i;i1<i+4;i1++) {
					temp1=A[i1][j];temp2=A[i1][j+1];temp3=A[i1][j+2];temp4=A[i1][j+3];
					temp5=A[i1][j+4];temp6=A[i1][j+5];temp7=A[i1][j+6];temp8=A[i1][j+7];

					B[j][i1]=temp1;B[j+1][i1]=temp2;B[j+2][i1]=temp3;B[j+3][i1]=temp4;
					B[j][i1+4]=temp5;B[j+1][i1+4]=temp6;B[j+2][i1+4]=temp7;B[j+3][i1+4]=temp8;
				}
				for (j1=j;j1<j+4;j1++) {
					temp1=A[i+4][j1];temp2=A[i+5][j1];temp3=A[i+6][j1];temp4=A[i+7][j1];
					temp5=B[j1][i+4];temp6=B[j1][i+5];temp7=B[j1][i+6];temp8=B[j1][i+7];
					
					B[j1][i+4]=temp1;B[j1][i+5]=temp2;B[j1][i+6]=temp3;B[j1][i+7]=temp4;
					B[j1+4][i]=temp5;B[j1+4][i+1]=temp6;B[j1+4][i+2]=temp7;B[j1+4][i+3]=temp8;
				}
				for (i1=i+4;i1<i+8;i1++) {
					temp1=A[i1][j+4];temp2=A[i1][j+5];temp3=A[i1][j+6];temp4=A[i1][j+7];

					B[j+4][i1]=temp1;B[j+5][i1]=temp2;B[j+6][i1]=temp3;B[j+7][i1]=temp4;
				}
			}
		}
	}
	else if (M == 61 && N == 67) {
		int i, j, i1, j1;
		int block = 17;
		int temp;
		for (i = 0; i < N; i += block)
			for (j = 0; j < N; j += block)
				for (i1 = j; i1 < j + block && i1 < N; ++i1)
					for (j1 = i; j1 < i + block && j1 < M; ++j1) {
						temp = A[i1][j1];
						B[j1][i1] = temp;
					}
	}
}

/* 
 * you can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 
char transpose_M32N32_desc[] = "Transpose M32N32";
void transpose_M32N32(int M, int N, int A[N][M], int B[M][N])
{
	int i,j,i1,j1;
	int temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	for (i=0;i<N;i+=8) {
		for (j=0;j<M;j+=8){
			for (i1=i;i1<i+8;i1++) {
				if (i==j){
				temp1=A[i1][j];temp2=A[i1][j+1];temp3=A[i1][j+2];temp4=A[i1][j+3];
				temp5=A[i1][j+4];temp6=A[i1][j+5];temp7=A[i1][j+6];temp8=A[i1][j+7];
				B[j][i1]=temp1;B[j+1][i1]=temp2;B[j+2][i1]=temp3;B[j+3][i1]=temp4;
				B[j+4][i1]=temp5;B[j+5][i1]=temp6;B[j+6][i1]=temp7;B[j+7][i1]=temp8;
				continue;
				}
				for (j1=j;j1<j+8;j1++){
					B[j1][i1]=A[i1][j1];
				}
			}
		}
	}
}
char transpose_M64N64_desc[] = "Transpose M64N64";
void transpose_M64N64(int M, int N, int A[N][M], int B[M][N])
{
	int i,j,i1,j1;
	int temp1,temp2,temp3,temp4,temp5,temp6,temp7,temp8;
	for (i=0;i<N;i+=8) {
		for (j=0;j<M;j+=8){
			for (i1=i;i1<i+4;i1++) {
				temp1=A[i1][j];temp2=A[i1][j+1];temp3=A[i1][j+2];temp4=A[i1][j+3];
				temp5=A[i1][j+4];temp6=A[i1][j+5];temp7=A[i1][j+6];temp8=A[i1][j+7];

				B[j][i1]=temp1;B[j+1][i1]=temp2;B[j+2][i1]=temp3;B[j+3][i1]=temp4;
				B[j][i1+4]=temp5;B[j+1][i1+4]=temp6;B[j+2][i1+4]=temp7;B[j+3][i1+4]=temp8;
			}
			for (j1=j;j1<j+4;j1++) {
				temp1=A[i+4][j1];temp2=A[i+5][j1];temp3=A[i+6][j1];temp4=A[i+7][j1];
				temp5=B[j1][i+4];temp6=B[j1][i+5];temp7=B[j1][i+6];temp8=B[j1][i+7];
				
				B[j1][i+4]=temp1;B[j1][i+5]=temp2;B[j1][i+6]=temp3;B[j1][i+7]=temp4;
				B[j1+4][i]=temp5;B[j1+4][i+1]=temp6;B[j1+4][i+2]=temp7;B[j1+4][i+3]=temp8;
			}
			for (i1=i+4;i1<i+8;i1++) {
				temp1=A[i1][j+4];temp2=A[i1][j+5];temp3=A[i1][j+6];temp4=A[i1][j+7];

				B[j+4][i1]=temp1;B[j+5][i1]=temp2;B[j+6][i1]=temp3;B[j+7][i1]=temp4;
			}
		}
	}
}
char transpose_M61N67_desc[] = "Transpose M61N67";
void transpose_M61N67(int M, int N, int A[N][M], int B[M][N])
{
	int i, j, i1, j1;
	int block = 17;
	int temp;
	for (i = 0; i < N; i += block)
		for (j = 0; j < N; j += block)
			for (i1 = j; i1 < j + block && i1 < N; ++i1)
				for (j1 = i; j1 < i + block && j1 < M; ++j1) {
					temp = A[i1][j1];
					B[j1][i1] = temp;
				}
}
/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);  
    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. you can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

