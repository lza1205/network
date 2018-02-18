
#ifndef __MATRIX_H__
#define __MATRIX_H__

#ifdef __MATRIX_C__
#define __MATRIX_EXT__
#else
#define __MATRIX_EXT__ extern
#endif

#include <stdio.h>

typedef struct st_Matrix{
	unsigned int row;		//矩阵多少行
	unsigned int column;		//矩阵多少列
	float *buf;			//实际存储的数据
}T_Matrix;


//判断矩阵是否为NULL
#define IS_Matrix(mat) 	if(mat == NULL)	\
						{				\
							printf("__ %s %d mat is NULL\r\n", __FILE__, __LINE__);	\
							return -1;	\
						}				\
						if(mat->buf == NULL)\
						{						\
							printf("__ %s %d mat buf is NULL\r\n", __FILE__, __LINE__);	return -1;	\
						}

//判断w h是否越界
#define IS_Matrix_w_h(mat)	if(row > mat->row)\
							{				\
								printf("__ %s %d row > mat->row is %d mat->row is %d \r\n", __FILE__, __LINE__, row, mat->row);	\
								return -1;\
							}				\
							if(column > mat->column)\
							{				\
								printf("__ %s %d column > mat->column is %d mat->column is %d\r\n", __FILE__, __LINE__, column, mat->column);	\
								return -1;\
							}	

//得到矩阵的某个数值
#define Matrix_val(mat, r, c)	mat->buf[r * (mat->column) + c]



__MATRIX_EXT__ int Matrix_Printf(char * str, T_Matrix * mat);
__MATRIX_EXT__ T_Matrix *Matrix_New_Obj(unsigned int row, unsigned int column);
__MATRIX_EXT__ int Matrix_Init(T_Matrix * mat, float *buf, unsigned int len);
__MATRIX_EXT__ int Matrix_GetVal(T_Matrix * mat, unsigned int row, unsigned int column, float *val);
__MATRIX_EXT__ int Matrix_SetVal(T_Matrix * mat, unsigned int row, unsigned int column, float val);
__MATRIX_EXT__ int Matrix_Get_Row(float *row, T_Matrix *mat, unsigned r);
__MATRIX_EXT__ int Matrix_Get_Column(float *column, T_Matrix *mat, unsigned c);
__MATRIX_EXT__ int Matrix_Printf_Row(T_Matrix * mat, unsigned r);
__MATRIX_EXT__ int Matrix_Printf_Column(T_Matrix *mat, unsigned c);
__MATRIX_EXT__ int Matrix_Mul(T_Matrix *mat_new, T_Matrix *mat_a, T_Matrix *mat_b);
__MATRIX_EXT__ int Matrix_Add_Val(T_Matrix *mat_sum, T_Matrix *mat, float val);
__MATRIX_EXT__ int Matrix_Add_Mat(T_Matrix *mat_sum, T_Matrix *mat_a, T_Matrix *mat_b);
__MATRIX_EXT__ int Matrix_Minus_Mat(T_Matrix *mat_sum, T_Matrix *mat_a, T_Matrix *mat_b);
__MATRIX_EXT__ T_Matrix *Matrix_New_Random_Obj(unsigned int row, unsigned int column, float st, float end);
__MATRIX_EXT__ T_Matrix *Matrix_New_Init_Obj(unsigned int row, unsigned int column, float val);
__MATRIX_EXT__ int Matrix_Add_One_Column_Mat(T_Matrix *mat_sum, T_Matrix *mat_a, T_Matrix *mat_b);
__MATRIX_EXT__ int Matrix_Sigmoid(T_Matrix *mat_dest, T_Matrix *mat_src);
__MATRIX_EXT__ int Matrix_Hadamard(T_Matrix *mat_dest, T_Matrix *mat_a, T_Matrix *mat_b);
__MATRIX_EXT__ float Matrix_Sum(T_Matrix *mat);
__MATRIX_EXT__ int Matrix_Sigmoid_prime(T_Matrix *mat_dest, T_Matrix *mat_src);
__MATRIX_EXT__ T_Matrix *Matrix_Transpose(T_Matrix *mat);
__MATRIX_EXT__ int Matrix_Mul_Val(T_Matrix *mat_sum, T_Matrix *mat, float val);
__MATRIX_EXT__ int Matrix_Copy(T_Matrix *mat_dest, T_Matrix *mat_src);



#endif
