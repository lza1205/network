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
	UINT32 			lay_size;		//�������ж��ٲ�
	UINT32			*layout;

	T_Matrix **mat_w;			//WȨ��
	T_Matrix **mat_b;			//BȨ��

	T_Matrix **mat_delta;			//Ȩ�ص���ֵ
	T_Matrix **mat_w_delta;			//Ȩ�ص���ֵ

	T_Matrix **mat_w_nabla; 		//WȨ�ص���ֵ
	T_Matrix **mat_b_nabla; 		//BȨ�ص���ֵ

	T_Matrix **mat_z_sigmoid_prime;			//z��ƫ��

	T_Matrix ***x;					//��Ԫ�ṹ�� x[0][0] �ǵڼ������ݵ�����  x[0][lay] �Ǽ������
	T_Matrix ***z;
	T_Matrix **y;					//��Ӧ���������
	T_Matrix **cost;				//����ֵ

	T_Matrix *mat_evl;				//ƽ��ֵ

	T_Matrix ***test_x;					//��Ԫ�ṹ�� x[0][0] �ǵڼ������ݵ�����  x[0][lay] �Ǽ������
	T_Matrix ***test_z;
	T_Matrix **test_y;				//���Խ��
	
	UINT32 size;					//ѵ�����ݵĴ�С
	
}T_Network;

//���������һ�������
#define NET_X_LAST		net->lay_size - 1

T_Network *Network_New_Obj(UINT32 *layout, UINT32 len);
int Network_Load_Training_Data(T_Network *net, FLOAT32 *x, FLOAT32 *y, UINT32 size);
int Network_Cost(T_Network *net);
int Network_Backprop(T_Network *net, FLOAT32 eta);
int Network_SGD(T_Network *net, UINT32 epochs, FLOAT32 eta);


#endif

