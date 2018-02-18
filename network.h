#ifndef __NETWORK_H__
#define __NETWORK_H__

#ifdef __NETWORK_C__
#define __NETWORK_EXT__
#else
#define __NETWORK_EXT__ extern
#endif

#include "typedef.h"
#include "Matrix.h"

#define DBG_NETWORK(x)	//x

typedef struct st_Network{
	UINT32 			lay_size;		//神经网络有多少层
	UINT32			*layout;

	T_Matrix **mat_w;			//W权重
	T_Matrix **mat_b;			//B权重

	T_Matrix **mat_delta;			//权重递增值
	T_Matrix **mat_w_delta;			//权重递增值

	T_Matrix **mat_w_nabla; 		//W权重递增值
	T_Matrix **mat_b_nabla; 		//B权重递增值

	T_Matrix **mat_z_sigmoid_prime;			//z的偏导

	T_Matrix ***x;					//神级元结构， x[0][0] 是第几个数据的输入  x[0][lay] 是计算输出
	T_Matrix ***z;
	T_Matrix **y;					//对应的输出数据
	T_Matrix **cost;				//方差值

	T_Matrix *mat_evl;				//平均值

	T_Matrix ***test_x;					//神级元结构， x[0][0] 是第几个数据的输入  x[0][lay] 是计算输出
	T_Matrix ***test_z;
	T_Matrix **test_y;				//测试结果
	
	UINT32 size;					//训练数据的大小
	
}T_Network;

//神经网络最后一次输出，
#define NET_X_LAST		net->lay_size - 1

T_Network *Network_New_Obj(UINT32 *layout, UINT32 len);
int Network_Load_Training_Data(T_Network *net, FLOAT32 *x, FLOAT32 *y, UINT32 size);
int Network_Cost(T_Network *net);
int Network_Backprop(T_Network *net, FLOAT32 eta);
int Network_SGD(T_Network *net, UINT32 epochs, FLOAT32 eta);


#endif

