
#define __NETWORK_C__


#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "network.h"


T_Network *Network_New_Obj(UINT32 *layout, UINT32 len)
{
	T_Network *net;

	UINT32 i;
	
	net = (T_Network *)malloc(sizeof(T_Network));
	if(net == NULL)
	{
		printf("__ %s %d net == NULL \r\n", __FILE__, __LINE__);
		return NULL;
	}

	net->layout = layout;
	net->lay_size = len;
		
	net->mat_b = (T_Matrix **)malloc((len - 1));

	for(i = 1; i < len; i ++)
	{
		net->mat_b[i - 1] = Matrix_New_Random_Obj(layout[i], 1, -1, 1);
		DBG_NETWORK(Matrix_Printf("net->mat_b[i - 1]", net->mat_b[i - 1]));
	}

	net->mat_w = (T_Matrix **)malloc((len - 1));

	for(i = 0; i < len - 1; i++)
	{
		net->mat_w[i] = Matrix_New_Random_Obj(layout[i+1], layout[i], -1, 1);
		DBG_NETWORK(Matrix_Printf("net->mat_w[i]", net->mat_w[i]));
	}


	net->mat_delta = (T_Matrix **)malloc((len - 1));

	for(i = 1; i < len; i ++)
	{
		net->mat_delta[i - 1] = Matrix_New_Init_Obj(layout[i], 1, 0);
		//DBG_NETWORK(Matrix_Printf("net->mat_b[i - 1]", net->mat_delta[i - 1]));
	}

	net->mat_w_delta = (T_Matrix **)malloc((len - 1));

	for(i = 1; i < len; i ++)
	{
		net->mat_w_delta[i - 1] = Matrix_New_Init_Obj(layout[i], 1, 1);
		//DBG_NETWORK(Matrix_Printf("net->mat_b[i - 1]", net->mat_w_delta[i - 1]));
	}


	net->mat_b_nabla = (T_Matrix **)malloc((len - 1));

	for(i = 1; i < len; i ++)
	{
		net->mat_b_nabla[i - 1] = Matrix_New_Init_Obj(layout[i], 1, 0);
		//DBG_NETWORK(Matrix_Printf("net->mat_b[i - 1]", net->mat_delta[i - 1]));
	}

	net->mat_w_nabla = (T_Matrix **)malloc((len - 1));

	for(i = 0; i < len - 1; i++)
	{
		net->mat_w_nabla[i] = Matrix_New_Init_Obj(layout[i+1], layout[i], 1);
		//DBG_NETWORK(Matrix_Printf("net->mat_w[i]", net->mat_w[i]));
	}


	net->mat_evl = Matrix_New_Init_Obj(net->layout[NET_X_LAST], 1, 0);

	return net;
}


/*
读取训练数据，注意，这里的x 是矩阵数据的
x_size 表示有多少个数据
*/
int Network_Load_Training_Data(T_Network *net, FLOAT32 *x, FLOAT32 *y, UINT32 size)
{
	UINT32 i, j;
	FLOAT32 *x_p = x;
	FLOAT32 *y_p = y;

	net->size = size;
	
	net->x = (T_Matrix ***)malloc(size * sizeof(T_Matrix *));

	for(i = 0; i < size; i ++)
	{
		net->x[i] = (T_Matrix **)malloc(net->lay_size * sizeof(T_Matrix *));
		for(j = 0; j < net->lay_size; j ++)
		{
			net->x[i][j] = Matrix_New_Init_Obj(net->layout[j], 1, 0);
			DBG_NETWORK(printf("____>>>>> i is %d, j is %d:\r\n", i ,j ));
			DBG_NETWORK(Matrix_Printf("net->x[i][j]", net->x[i][j]));
		}
	}

	net->z = (T_Matrix ***)malloc(size * sizeof(T_Matrix *));

	for(i = 0; i < size; i ++)
	{
		net->z[i] = (T_Matrix **)malloc(net->lay_size * sizeof(T_Matrix *));
		for(j = 0; j < net->lay_size; j ++)
		{
			net->z[i][j] = Matrix_New_Init_Obj(net->layout[j], 1, 0);
			DBG_NETWORK(printf("____>>>>> i is %d, j is %d:\r\n", i ,j ));
			DBG_NETWORK(Matrix_Printf("net->z[i][j]", net->z[i][j]));
		}
	}

	net->mat_z_sigmoid_prime = (T_Matrix **)malloc(net->lay_size * sizeof(T_Matrix *));
	for(j = 0; j < net->lay_size; j ++)
	{
		net->mat_z_sigmoid_prime[j] = Matrix_New_Init_Obj(net->layout[j], 1, 0);
		DBG_NETWORK(printf("____>>>>> i is %d, j is %d:\r\n", i ,j ));
		DBG_NETWORK(Matrix_Printf("net->z[i][j]", net->mat_z_sigmoid_prime[j]));
	}

	for(i = 0; i < size; i ++)
	{
		//printf("%d %d %d %d\r\n", net->x[i][0]->row, net->x[i][0]->column, net->layout[0], x[0][0]);
		Matrix_Init(net->x[i][0], x_p, net->layout[0]);

		x_p += net->layout[0];
		
		DBG_NETWORK(printf("____>>>>> i is %d\r\n", i ));
		DBG_NETWORK(Matrix_Printf("net->x[i][0]", net->x[i][0]));
	}

	
	net->y = (T_Matrix **)malloc(size * sizeof(T_Matrix *));

	for(i = 0; i < size; i ++)
	{
		net->y[i] = Matrix_New_Init_Obj(net->layout[net->lay_size - 1], 1, 0);

		Matrix_Init(net->y[i], y_p, net->layout[net->lay_size - 1]);

		y_p += net->layout[net->lay_size - 1];
		
		DBG_NETWORK(Matrix_Printf("net->y[i]", net->y[i]));
	}

	
	net->cost = (T_Matrix **)malloc(size * sizeof(T_Matrix *));

	for(i = 0; i < size; i ++)
	{
		net->cost[i] = Matrix_New_Init_Obj(net->layout[net->lay_size - 1], 1, 0);
	}
	
}

//正向传播
int Network_Forward(T_Network *net)
{
	UINT32 i, j;

	for(i = 0; i < net->size; i ++)
	{
		for(j = 0; j < net->lay_size - 1; j++)
		{
			//printf(" i is %d  j is %d \r\n", i, j);
			//(Matrix_Printf("net->mat_w[j]", net->mat_w[j]));
			//(Matrix_Printf("net->mat_b[j]", net->mat_b[j]));
			//(Matrix_Printf("net->x[i][j]", net->x[i][j]));

			//相乘 W*X
			Matrix_Mul(net->z[i][j + 1], net->mat_w[j], net->x[i][j]);
			//(Matrix_Printf("net->z[i][j + 1]", net->z[i][j + 1]));
			//相加 W*X + B
			Matrix_Add_Mat(net->z[i][j + 1], net->z[i][j + 1], net->mat_b[j]);
			//(Matrix_Printf("net->z[i][j + 1]", net->z[i][j + 1]));
			//sigmod函数
			Matrix_Sigmoid(net->x[i][j + 1], net->z[i][j + 1]);
			//(Matrix_Printf("net->z[i][j + 1]", net->z[i][j + 1]));
			
			//(Matrix_Printf("net->x[i][j + 1]", net->x[i][j + 1]));

			
		}
		//printf("\r\n\r\n#####################################\r\n\r\n");
		//if(i = 2)
			//while(1);
		//Matrix_Printf("out is ", net->x[i][NET_X_LAST]);
	}

	return 0;
}


//计算方差
int Network_Cost(T_Network *net)
{
	UINT32 i, j;
	float cost = 0.0;
	float tmp;

	for(i = 0; i < net->size; i ++)
	{
		//printf(" i is %d  \r\n", i);
		//DBG_NETWORK(Matrix_Printf("net->x[i][NET_X_LAST]", net->x[i][NET_X_LAST]));
		//DBG_NETWORK(Matrix_Printf("net->y[i]", net->y[i]));
			
		Matrix_Minus_Mat(net->cost[i], net->x[i][NET_X_LAST], net->y[i]);

		//DBG_NETWORK(Matrix_Printf("net->cost[i]", net->cost[i]));

		Matrix_Hadamard(net->cost[i], net->cost[i], net->cost[i]);

		//DBG_NETWORK(Matrix_Printf("net->cost[i]", net->cost[i]));

		tmp = Matrix_Sum(net->cost[i]);
		cost += tmp;

		//printf("___>>>> tmp is %lf \r\n", tmp);
	}

	//printf("___>>>> cost is %lf \r\n", cost);

	cost = cost / (2*net->size);

	//printf("___>>>> cost is %lf \r\n", cost);
	return cost;
}


//反向传播
int Network_Backprop(T_Network *net, FLOAT32 eta)
{
	INT32 i, j;
	FLOAT32 nabla_b = 0.0, nabla_w = 0.0;
	T_Matrix *mat_t;

	//printf("___>>>>> %d\r\n", NET_X_LAST - 2);
	
	//先计算最后一层的误差值
	for(i = 0; i < net->size; i ++)
	{
		//printf("___>>>>> %s %d %d\r\n", __FILE__, __LINE__, NET_X_LAST - 2);
		//相减
		//Matrix_Printf("net->x[i][NET_X_LAST]", net->x[i][NET_X_LAST]);
		//Matrix_Printf("net->y[i]", net->y[i]);
		Matrix_Minus_Mat(net->mat_delta[NET_X_LAST - 1], net->x[i][NET_X_LAST], net->y[i]);
		//Matrix_Printf("net->mat_delta[NET_X_LAST - 1]", net->mat_delta[NET_X_LAST - 1]);

		//while(1);
		
		//计算偏导
		Matrix_Sigmoid_prime(net->mat_z_sigmoid_prime[NET_X_LAST], net->z[i][NET_X_LAST]);

		Matrix_Hadamard(net->mat_delta[NET_X_LAST - 1], net->mat_delta[NET_X_LAST - 1], net->mat_z_sigmoid_prime[NET_X_LAST]);

		//B
		Matrix_Add_Mat(net->mat_b_nabla[NET_X_LAST - 1], net->mat_b_nabla[NET_X_LAST - 1], net->mat_delta[NET_X_LAST - 1]);

		//W		
		Matrix_Mul(net->mat_w_delta[NET_X_LAST - 1], net->mat_delta[NET_X_LAST - 1], net->x[i][NET_X_LAST - 1]);
		Matrix_Add_Mat(net->mat_w_nabla[NET_X_LAST - 1], net->mat_w_nabla[NET_X_LAST - 1], net->mat_w_delta[NET_X_LAST - 1]);
	}

		
	//先球平均值
	//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);
	Matrix_Mul_Val(net->mat_b_nabla[NET_X_LAST - 1], net->mat_b_nabla[NET_X_LAST - 1], (float)(1.0/net->size));
	//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);

	//while(1);
	//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);
	Matrix_Mul_Val(net->mat_w_nabla[NET_X_LAST - 1], net->mat_w_nabla[NET_X_LAST - 1], (float)(1.0/net->size));
	//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);

	//学习率
	//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);
	Matrix_Mul_Val(net->mat_b_nabla[NET_X_LAST - 1], net->mat_b_nabla[NET_X_LAST - 1], (float)eta);
	//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);

	//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);
	Matrix_Mul_Val(net->mat_w_nabla[NET_X_LAST - 1], net->mat_w_nabla[NET_X_LAST - 1], (float)eta);
	//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);

	//Matrix_Printf("net->mat_b[i]", net->mat_b[i]);
	Matrix_Minus_Mat(net->mat_b[NET_X_LAST - 1], net->mat_b[NET_X_LAST - 1], net->mat_b_nabla[NET_X_LAST - 1]);
	//Matrix_Printf("net->mat_b[i]", net->mat_b[i]);

	//Matrix_Printf("net->mat_w[i]", net->mat_w[i]);
	Matrix_Minus_Mat(net->mat_w[NET_X_LAST - 1], net->mat_w[NET_X_LAST - 1], net->mat_w_nabla[NET_X_LAST - 1]);
	//Matrix_Printf("net->mat_w[i]", net->mat_w[i]);

	//printf("___>>>>> %s %d %d\r\n", __FILE__, __LINE__, NET_X_LAST - 2);
	
	//反向误差传播回去
	for(i = NET_X_LAST - 2; i >= 0; i --)
	{
		
		for(j = 0; j < net->size; j++)
		{
			//计算偏导
			//printf("___>>>>> %s %d %d\r\n", __FILE__, __LINE__, i);
			Matrix_Sigmoid_prime(net->mat_z_sigmoid_prime[i+1], net->z[j][i+1]);

			//Matrix_Printf("net->mat_z_sigmoid_prime[i]", net->mat_z_sigmoid_prime[i+1]);
			
			mat_t = Matrix_Transpose(net->mat_w[i + 1]);
			//printf("___>>>>> %s %d %d\r\n", __FILE__, __LINE__, NET_X_LAST - 2);

			DBG_NETWORK(Matrix_Printf("net->mat_w[i + 1]", net->mat_w[i + 1]));
			DBG_NETWORK(Matrix_Printf("mat_t", mat_t));
			
			//net
			//Matrix_Printf("net->mat_delta[i + 1]", net->mat_delta[i + 1]);
			Matrix_Mul(net->mat_delta[i], mat_t, net->mat_delta[i + 1]);
			//Matrix_Printf("net->mat_delta[i]", net->mat_delta[i]);
			//while(1);

			Matrix_Hadamard(net->mat_delta[i], net->mat_delta[i], net->mat_z_sigmoid_prime[i+1]);
			//Matrix_Printf("net->mat_delta[i]", net->mat_delta[i]);
			//while(1);
			
			//B
			//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);
			Matrix_Add_Mat(net->mat_b_nabla[i], net->mat_b_nabla[i], net->mat_delta[i]);
			//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);
			
			//W 	
			Matrix_Mul(net->mat_w_delta[i], net->mat_delta[i], net->x[j][i + 1]);
			Matrix_Add_Mat(net->mat_w_nabla[i], net->mat_w_nabla[i], net->mat_w_delta[i]);
		}

		
		//先球平均值
		//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);
		Matrix_Mul_Val(net->mat_b_nabla[i], net->mat_b_nabla[i], (float)(1.0/net->size));
		//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);

		//while(1);
		//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);
		Matrix_Mul_Val(net->mat_w_nabla[i], net->mat_w_nabla[i], (float)(1.0/net->size));
		//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);

		//学习率
		//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);
		Matrix_Mul_Val(net->mat_b_nabla[i], net->mat_b_nabla[i], (float)eta);
		//Matrix_Printf("net->mat_b_nabla[i]", net->mat_b_nabla[i]);

		//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);
		Matrix_Mul_Val(net->mat_w_nabla[i], net->mat_w_nabla[i], (float)eta);
		//Matrix_Printf("net->mat_w_nabla[i]", net->mat_w_nabla[i]);

		//Matrix_Printf("net->mat_b[i]", net->mat_b[i]);
		Matrix_Minus_Mat(net->mat_b[i], net->mat_b[i], net->mat_b_nabla[i]);
		//Matrix_Printf("net->mat_b[i]", net->mat_b[i]);

		//Matrix_Printf("net->mat_w[i]", net->mat_w[i]);
		Matrix_Minus_Mat(net->mat_w[i], net->mat_w[i], net->mat_w_nabla[i]);
		//Matrix_Printf("net->mat_w[i]", net->mat_w[i]);

		//while(1);
	}
}


//开始训练
//epochs 训练次数  eta学习率
int Network_SGD(T_Network *net, UINT32 epochs, FLOAT32 eta)
{
	UINT32 i;
	float cost = 0.0;
	
	for(i = 0; i < epochs; i ++)
	{
		Network_Forward(net);

		cost = Network_Cost(net);
		//printf("___>>>> cost is %lf \r\n", cost);

		//Matrix_Printf("net->mat_w[j]", net->mat_b[0]);
		
		Network_Backprop(net, eta);

		
	}

	
	for(i = 0; i < net->size; i++)
	{
		printf("i %d \r\n", i);
		(Matrix_Printf("net->x[i][NET_X_LAST]", net->x[i][NET_X_LAST]));
		Matrix_Add_Mat(net->mat_evl, net->mat_evl, net->x[i][NET_X_LAST]);
	}
	//求平均值
	Matrix_Mul_Val(net->mat_evl, net->mat_evl, (float)(1.0/net->size));
	(Matrix_Printf("mat_sum", net->mat_evl));
	
}

//预测单个结果
int Network_Test(T_Network *net, FLOAT32 *x, UINT32 size)
{
	UINT32 i, j, k, index = 0;
	FLOAT32 *x_p = x;
	T_Matrix *tmp_y;
	FLOAT32 tmp_sum = 0.0;
	FLOAT32 tmp_cost = 100000.0;
	
	net->test_x = (T_Matrix ***)malloc(size * sizeof(T_Matrix *));
	tmp_y = Matrix_New_Init_Obj(net->layout[NET_X_LAST], 1, 0);

	for(i = 0; i < size; i ++)
	{
		net->test_x[i] = (T_Matrix **)malloc(net->lay_size * sizeof(T_Matrix *));
		for(j = 0; j < net->lay_size; j ++)
		{
			net->test_x[i][j] = Matrix_New_Init_Obj(net->layout[j], 1, 0);
			DBG_NETWORK(printf("____>>>>> i is %d, j is %d:\r\n", i ,j ));
			DBG_NETWORK(Matrix_Printf("net->test_x[i][j]", net->test_x[i][j]));
		}
	}

	net->test_z = (T_Matrix ***)malloc(size * sizeof(T_Matrix *));

	for(i = 0; i < size; i ++)
	{
		net->test_z[i] = (T_Matrix **)malloc(net->lay_size * sizeof(T_Matrix *));
		for(j = 0; j < net->lay_size; j ++)
		{
			net->test_z[i][j] = Matrix_New_Init_Obj(net->layout[j], 1, 0);
			DBG_NETWORK(printf("____>>>>> i is %d, j is %d:\r\n", i ,j ));
			DBG_NETWORK(Matrix_Printf("net->test_z[i][j]", net->test_z[i][j]));
		}
	}


	net->test_y = (T_Matrix **)malloc(size * sizeof(T_Matrix *));
	
	for(i = 0; i < size; i ++)
	{
		net->test_y[i] = Matrix_New_Init_Obj(net->layout[net->lay_size - 1], 1, 0);
		
		//DBG_NETWORK(Matrix_Printf("net->y[i]", net->y[i]));
	}


	for(i = 0; i < size; i ++)
	{
		//printf("%d %d %d %d\r\n", net->x[i][0]->row, net->x[i][0]->column, net->layout[0], x[0][0]);
		Matrix_Init(net->test_x[i][0], x_p, net->layout[0]);

		x_p += net->layout[0];
		
		DBG_NETWORK(printf("____>>>>> i is %d\r\n", i ));
		DBG_NETWORK(Matrix_Printf("net->x[i][0]", net->test_x[i][0]));
	}


	for(i = 0; i < size; i ++)
	{
		tmp_cost = 100000.0;
		
		for(j = 0; j < net->lay_size - 1; j++)
		{
			//printf(" i is %d	j is %d \r\n", i, j);
			DBG_NETWORK(Matrix_Printf("net->mat_w[j]", net->mat_w[j]));
			DBG_NETWORK(Matrix_Printf("net->mat_b[j]", net->mat_b[j]));
			DBG_NETWORK(Matrix_Printf("net->test_x[i][j]", net->test_x[i][j]));

			//相乘 W*X
			Matrix_Mul(net->test_z[i][j + 1], net->mat_w[j], net->test_x[i][j]);
			DBG_NETWORK(Matrix_Printf("net->test_z[i][j + 1]", net->test_z[i][j + 1]));
			//相加 W*X + B
			Matrix_Add_Mat(net->test_z[i][j + 1], net->test_z[i][j + 1], net->mat_b[j]);
			DBG_NETWORK(Matrix_Printf("net->test_z[i][j + 1]", net->test_z[i][j + 1]));
			//sigmod函数
			Matrix_Sigmoid(net->test_x[i][j + 1], net->test_z[i][j + 1]);
			DBG_NETWORK(Matrix_Printf("net->test_z[i][j + 1]", net->test_z[i][j + 1]));
			
			DBG_NETWORK(Matrix_Printf("net->test_x[i][j + 1]", net->test_x[i][j + 1]));
			
		}
		

		//从样本中找到最接近的
		for(k = 0; k < net->size; k ++)
		{
			//printf("\r\n\r\n ---------------------------- \r\n\r\n\r\n");
			
			Matrix_Minus_Mat(tmp_y, net->x[k][NET_X_LAST], net->test_x[i][NET_X_LAST]);
			Matrix_Mul_Val(tmp_y, tmp_y, 1000.0);

			//Matrix_Printf("tmp_y", tmp_y);

			Matrix_Hadamard(tmp_y, tmp_y, tmp_y);

			//Matrix_Printf("tmp_y1", tmp_y);

			tmp_sum = Matrix_Sum(tmp_y);
			printf("tmp_cost is %lf, tmp_sum is %lf \r\n", tmp_cost, tmp_sum);
			if(tmp_cost > tmp_sum)
			{
				tmp_cost = tmp_sum;
				index = k;
			}
		}

		printf(" index is %d \r\n", index);
		Matrix_Copy(net->test_y[i], net->y[index]);

		Matrix_Printf("net->test_x[i][NET_X_LAST] ", net->test_x[i][NET_X_LAST]);
		Matrix_Printf("net->test_y[i]", net->test_y[i]);
	}

	
	
}


