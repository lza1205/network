

#include <stdio.h>

#include "typedef.h"
#include "Matrix.h"
#include "Network.h"


float aa[2][3] = {{1,2,3},{4,5,6}};
float bb[3][2] = {{1,4}, {2,5},{3,6}};


UINT32 layout[] = {2, 3, 4, 1};


float test1[7][2] = {{0.2, 0.4}, {0.8, 0.2}, {0.50, 0.45}, {0.13, 0.20}, {0.13, 0.17}, {0.25, 0.10}, {0.8, 0.2}};
float test1_y[7][1] = {{1},     	{0},   		{0},   		{1},   			{1},   			{0},   			{0}};

float test1_test[7][2] = {{0.20002, 0.20001}, {0.8, 0.2}, {0.50, 0.45}, {0.13, 0.20}, {0.13, 0.17}, {0.25, 0.10}, {0.8, 0.2}};


/*
ÓÃÀ´²âÊÔµÄ
*/
int main(void)
{
/*
	T_Matrix *mat_a, *mat_b, *mat_new;
	
	mat_a = Matrix_New_Obj(2, 3);
	mat_b = Matrix_New_Obj(3, 2);
	
	Matrix_Init(mat_a, aa, 2*3);
	Matrix_Init(mat_b, bb, 3*2);

	mat_new = Matrix_New_Obj(mat_a->row, mat_b->column);
	Matrix_Mul(mat_new, mat_a, mat_b);
	Matrix_Printf("mat_new", mat_new);
	*/

	T_Network *net;
	net = Network_New_Obj(layout, 4);

	Network_Load_Training_Data(net, (FLOAT32 *)test1, (FLOAT32 *)test1_y, 7);

	Network_SGD(net, 500000, 0.001);

	Network_Test(net, (FLOAT32 *)test1_test, 1);
	
	while(1);
	return 0;
}


