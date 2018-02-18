
#ifndef __MATRIX_C__
#define __MATRIX_C__

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "Matrix.h"

/*=============================================================
	本文件用于实现简单的矩阵运算

	作者: 连志安
	时间:2017 06 25
=============================================================*/
/*=============================================================

=============================================================*/

/*=============================================================
	打印一个矩阵
=============================================================*/
int Matrix_Printf(char * str, T_Matrix * mat)
{
	unsigned int i, j;
	IS_Matrix(mat);

	printf("==========  %s Matrix start =============\r\n", str);
	printf(" Matrix row column : %d X %d\r\n ",mat->row, mat->column);
	for(i = 0; i < mat->row; i++)
	{
		printf("[%d] ", i);
		for(j = 0; j < mat->column; j++)
			printf("%lf ", mat->buf[i * mat->column + j]);
		printf("\r\n");
	}
	printf("========== Matrix end =============\r\n");

	return 0;
}

/*=============================================================
	打印一个矩阵 行 列
=============================================================*/
int Matrix_Printf_Row(T_Matrix * mat, unsigned r)
{
	unsigned int i;
	
	IS_Matrix(mat);

	printf("Row [%d] : ", r);
	for(i = 0; i < mat->column; i ++)
	{
		printf("%f ", mat->buf[r * mat->column + i]);
	}
	printf("\r\n");
	return 0;
}
int Matrix_Printf_Column(T_Matrix *mat, unsigned c)
{
	unsigned int i;
	
	IS_Matrix(mat);

	printf("Column [%d] : ", c);
	for(i = 0; i < mat->row; i ++)
	{
		printf("%f ", mat->buf[mat->column * i + c]);
	}
	printf("\r\n");
	
	return 0;
}

/*=============================================================
	得到一个空矩阵，内容暂无
=============================================================*/
T_Matrix *Matrix_New_NULL_Obj(void)
{
	T_Matrix *mat = NULL;

	mat = (T_Matrix *)malloc(sizeof(T_Matrix));
	if(mat == NULL)
	{
		printf("__ %s %d mat == NULL \r\n", __FILE__, __LINE__);
		return NULL;
	}

	return mat;
}

/*=============================================================
	得到一个矩阵
=============================================================*/
T_Matrix *Matrix_New_Obj(unsigned int row, unsigned int column)
{
	T_Matrix *mat = NULL;

	mat = (T_Matrix *)malloc(sizeof(T_Matrix));
	if(mat == NULL)
	{
		printf("__ %s %d mat == NULL \r\n", __FILE__, __LINE__);
		return NULL;
	}

	//printf("%d \r\n", row*column*sizeof(float));
	
	mat->buf = (float *)malloc(row*column*sizeof(float));
	if(mat->buf == NULL)
	{
		free(mat);
		printf("__ %s %d mat->buf == NULL \r\n", __FILE__, __LINE__);
		return NULL;
	}

	mat->row = row;
	mat->column = column;
	
	return mat;
}

/*=============================================================
	得到一个矩阵,并赋值
=============================================================*/
T_Matrix *Matrix_New_Init_Obj(unsigned int row, unsigned int column, float val)
{
	unsigned int i;
	
	T_Matrix *mat = NULL;
	mat = Matrix_New_Obj(row, column);

	if(mat == NULL)
		return NULL;

	for(i = 0; i < (mat->column * mat->row); i ++)
	{
		mat->buf[i] = val;
	}

	mat->row = row;
	mat->column = column;

	return mat;
}

/*=============================================================
	得到一个随机矩阵
=============================================================*/
T_Matrix *Matrix_New_Random_Obj(unsigned int row, unsigned int column, float st, float end)
{
	unsigned int i;
	
	T_Matrix *mat = NULL;
	mat = Matrix_New_Obj(row, column);

	if(mat == NULL)
		return NULL;

	srand(time(NULL));

	for(i = 0; i < (mat->column * mat->row); i ++)
	{
		mat->buf[i] = (float)(rand()/(RAND_MAX/(end - st)) + st);
	}

	mat->row = row;
	mat->column = column;

	return mat;
}


/*=============================================================
	释放一个矩阵
=============================================================*/
int Matrix_Free_Obj(T_Matrix *mat)
{
	if(mat->buf != NULL)
	{
		free(mat->buf);
	}

	if(mat != NULL)
	{
		free(mat);
	}
}


/*=============================================================
	初始化一个矩阵
=============================================================*/
int Matrix_Init(T_Matrix * mat, float *buf, unsigned int len)
{
	unsigned int i;
	
	IS_Matrix(mat);

	for(i = 0; i < len; i++)
	{
		mat->buf[i] = buf[i];
	}

	return 0;
}

/*=============================================================
	得到矩阵中第几行第几列的数据
=============================================================*/
int Matrix_GetVal(T_Matrix * mat, unsigned int row, unsigned int column, float *val)
{
	IS_Matrix(mat);

	IS_Matrix_w_h(mat);
	
	*val = mat->buf[row * (mat->column) + column];
	return 0;
}


/*=============================================================
	设置矩阵中第几行第几列的数据
=============================================================*/
int Matrix_SetVal(T_Matrix * mat, unsigned int row, unsigned int column, float val)
{
	IS_Matrix(mat);

	IS_Matrix_w_h(mat);

	mat->buf[row * (mat->column) + column] = val;
	return 0;
}

/*=============================================================
	获得矩阵的某一行
=============================================================*/
int Matrix_Get_Row(float *row, T_Matrix *mat, unsigned r)
{
	unsigned int i;
	
	if(row == NULL)
		return -1;
	
	IS_Matrix(mat);

	for(i = 0; i < mat->column; i ++)
	{
		row[i] = mat->buf[r * mat->column + i];
	}
	
	return 0;
}

/*=============================================================
	获得矩阵的某一列
=============================================================*/
int Matrix_Get_Column(float *column, T_Matrix *mat, unsigned c)
{
	unsigned int i;
	
	if(column == NULL)
		return -1;
	
	IS_Matrix(mat);

	for(i = 0; i < mat->row; i ++)
	{
		column[i] = mat->buf[mat->column * i + c];
	}
	
	return 0;
}



/*=============================================================
	返回一个矩阵的装置矩阵
=============================================================*/
T_Matrix *Matrix_Transpose(T_Matrix *mat)
{
	T_Matrix *mat_t;
	unsigned int i, j;
	
	//IS_Matrix(mat);
	
	mat_t = Matrix_New_Init_Obj(mat->column, mat->row, 0);

	for(i = 0; i < mat->row; i ++)
	{
		for(j = 0; j < mat->column; j ++)
		{
			Matrix_val(mat_t, j, i) = Matrix_val(mat, i, j);
		}
	}
	
	return mat_t;
}


/*=============================================================
	矩阵乘法
=============================================================*/
int Matrix_Mul(T_Matrix *mat_new, T_Matrix *mat_a, T_Matrix *mat_b)
{
	unsigned int i, j, k;
	float sum;
	float *row = NULL, *Column = NULL;
	
	IS_Matrix(mat_a);
	IS_Matrix(mat_b);
	IS_Matrix(mat_new);

	row = (float *)malloc(mat_a->column*sizeof(float));
	if(row == NULL)
		return -1;
	
	Column = (float *)malloc(mat_b->row*sizeof(float));
	if(Column == NULL)
		return -1;
	
	for(i = 0; i < mat_a->row; i++)
	{
		for(j = 0; j < mat_b->column; j++)
		{
			sum = 0;
			//Matrix_Printf_Row(mat_a, i);
			//Matrix_Printf_Column(mat_b, j);
			//printf("____ \r\n", i, j);
			 Matrix_Get_Row(row, mat_a, i);
			 Matrix_Get_Column(Column, mat_b, j);
			 for(k = 0; k < mat_a->column; k++)
			 {
				sum += row[k] * Column[k];
			 }
			 Matrix_SetVal(mat_new, i, j, sum);
			 //printf(" %d %d %d \r\n", i, j);
		}
	}

	//printf("____ \r\n");

	if(row != NULL)
		free(row);
	//printf("____ \r\n");
	if(Column != NULL)
		free(Column);
	//printf("____ \r\n");

	return 0;
}



/*=============================================================
	矩阵加某个常量
=============================================================*/
int Matrix_Add_Val(T_Matrix *mat_sum, T_Matrix *mat, float val)
{
	unsigned int i;
	
	IS_Matrix(mat);

	for(i = 0; i < (mat->column * mat->row); i ++)
	{
		mat_sum->buf[i] = mat->buf[i] + val;
	}

	return 0;
}

/*=============================================================
	矩阵乘某个常量
=============================================================*/
int Matrix_Mul_Val(T_Matrix *mat_sum, T_Matrix *mat, float val)
{
	unsigned int i;
	
	IS_Matrix(mat);

	for(i = 0; i < (mat->column * mat->row); i ++)
	{
		mat_sum->buf[i] = mat->buf[i] * val;
	}

	return 0;
}


/*=============================================================
	矩阵加矩阵
=============================================================*/
int Matrix_Add_Mat(T_Matrix *mat_sum, T_Matrix *mat_a, T_Matrix *mat_b)
{
	unsigned int i;
	
	IS_Matrix(mat_sum);
	IS_Matrix(mat_a);
	IS_Matrix(mat_b);

	if(mat_a->column != mat_b->column)
		return -1;

	if(mat_a->row != mat_b->row)
		return -1;

	for(i = 0; i < (mat_a->column * mat_a->row); i ++)
	{
		mat_sum->buf[i] = mat_a->buf[i] + mat_b->buf[i];
	}

	return 0;
}


/*=============================================================
	矩阵减矩阵
=============================================================*/
int Matrix_Minus_Mat(T_Matrix *mat_sum, T_Matrix *mat_a, T_Matrix *mat_b)
{
	unsigned int i;
	
	IS_Matrix(mat_sum);
	IS_Matrix(mat_a);
	IS_Matrix(mat_b);

	if(mat_a->column != mat_b->column)
		return -1;

	if(mat_a->row != mat_b->row)
		return -1;

	for(i = 0; i < (mat_a->column * mat_a->row); i ++)
	{
		mat_sum->buf[i] = mat_a->buf[i] - mat_b->buf[i];
	}

	return 0;
}


/*=============================================================
	矩阵Hadamard 乘法
=============================================================*/
int Matrix_Hadamard(T_Matrix *mat_dest, T_Matrix *mat_a, T_Matrix *mat_b)
{
	unsigned int i;
	
	IS_Matrix(mat_dest);
	IS_Matrix(mat_a);
	IS_Matrix(mat_b);

	if(mat_a->column != mat_b->column)
		return -1;

	if(mat_a->row != mat_b->row)
		return -1;

	for(i = 0; i < (mat_a->column * mat_a->row); i ++)
	{
		mat_dest->buf[i] = mat_a->buf[i] * mat_b->buf[i];
	}

	return 0;
}


/*=============================================================
	矩阵复制
=============================================================*/
int Matrix_Copy(T_Matrix *mat_dest, T_Matrix *mat_src)
{
	unsigned int i;
	
	IS_Matrix(mat_dest);
	IS_Matrix(mat_src);

	for(i = 0; i < (mat_src->column * mat_src->row); i ++)
	{
		mat_dest->buf[i] = mat_src->buf[i];
	}

	return 0;
}


/*=============================================================
	获取矩阵所有元素的和
=============================================================*/
float Matrix_Sum(T_Matrix *mat)
{
	unsigned int i;
	float sum = 0.0;
		
	IS_Matrix(mat);

	for(i = 0; i < (mat->column * mat->row); i ++)
	{
		sum += mat->buf[i];
	}

	return sum;
}


/*=============================================================
	矩阵加一列矩阵
=============================================================*/
int Matrix_Add_One_Column_Mat(T_Matrix *mat_sum, T_Matrix *mat_a, T_Matrix *mat_b)
{
	unsigned int i, j;
	float val;
	
	IS_Matrix(mat_sum);
	IS_Matrix(mat_a);
	IS_Matrix(mat_b);

	for(i = 0; i < (mat_a->column * mat_a->row); i ++)
	{
		mat_sum->buf[i] = mat_a->buf[i];
	}

	for(i = 0; i < (mat_sum->row); i ++)
	{
		for(j = 0; j < mat_sum->column; j++)
		{
			Matrix_GetVal(mat_a, i, j, &val);
			Matrix_SetVal(mat_sum, i, j, mat_b->buf[i] + val);
		}
	}

	return 0;
}

//Sigmoid
int Matrix_Sigmoid(T_Matrix *mat_dest, T_Matrix *mat_src)
{
	unsigned int i, j;
	float val;
	float tmp;

	IS_Matrix(mat_src);
	IS_Matrix(mat_dest);
	
	for(i = 0; i < (mat_src->row); i ++)
	{
		for(j = 0; j < mat_src->column; j++)
		{
			Matrix_GetVal(mat_src, i, j, &val);
			tmp = 1/(1 + exp((float)(0.0 - val)));
			Matrix_SetVal(mat_dest, i, j, tmp);
		}
	}
}


//对Sigmoid 的导数
int Matrix_Sigmoid_prime(T_Matrix *mat_dest, T_Matrix *mat_src)
{
	unsigned int i, j;
	float val;
	float tmp;

	IS_Matrix(mat_src);
	IS_Matrix(mat_dest);

	//printf("___>>>>> %s %d %d %d\r\n", __FILE__, __LINE__, mat_src->row, mat_src->column);
	
	for(i = 0; i < (mat_src->row); i ++)
	{
		for(j = 0; j < mat_src->column; j++)
		{
			//printf("___>>>>> %s %d %d %d\r\n", __FILE__, __LINE__, i, j);
			
			Matrix_GetVal(mat_src, i, j, &val);
			tmp = 1/(1 + exp((float)(0.0 - val)));

			tmp = tmp*(1-tmp);
			Matrix_SetVal(mat_dest, i, j, tmp);
		}
	}
}


#endif

