// Gradient descent1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

#define M 5000    // 样本数
#define TIME 10000    // 迭代次数
#define RATE 0.01 // 变化率

double value[25000][385];     // 25000个训练样本
double feature[385];          // 385个参数值h=feature[0]*value[][0]+...+feature[384]
int sample[25000];            // 从中取2500个随机样本
double h[M];                  // 每个样本计算得到的h

double testvalue[25000][384]; // 25000个测试样本
double reference[25000];      // 测试集结果

void getRandSample();         // 随机取样本
double getJ(int index);       // 计算cost函数
void getH();                  // 计算h值

int main()
{
	// 初始化
	for (int i = 0; i < 25000; i++) {
		sample[i] = i;
		for (int j = 0; j < 385; j++)
			value[i][j] = 0;
	}

	double temp_feature[385];  // 临时特征值

	// 随机取特征值
	srand((unsigned)time(NULL));
	for (int i = 0; i < 385; i++) {
		feature[i] = (double)rand() / ((double)RAND_MAX / 5);
		temp_feature[i] = 3;
	}

	// 读取训练集文件
	FILE* fp;
	errno_t err = fopen_s(&fp, "save_train.txt", "r");
	
	if (err != 0) {
		cout << "failed to read the train set..." << endl;
		exit(1);
	}

	char firstline[3500];    // 第一行
	fgets(firstline, sizeof(firstline), fp);    // 读取第一行不保存
	for (int i = 0; i < 25000; i++) {
		int id;  // id值，不保存
		fscanf_s(fp, "%d", &id);
		for (int j = 0; j < 385; j++) {
			fscanf_s(fp, "%lf", &value[i][j]);
		}
	}
	cout << "finish reading..." << endl;
	fclose(fp);

	// 迭代次数
	int time = TIME;
	while (time--) {
		// 每次迭代都取一批样本
		getRandSample();
		// 每次迭代都实现计算不同样本的h-y
		getH();
		// 计算临时参数值
		for (int i = 0; i < 385; i++) {
			//double j = getJ(i);
			temp_feature[i] = feature[i] - RATE * getJ(i) / M;
		}
		// 更新参数值
		for (int i = 0; i < 385; i++) {
			feature[i] = temp_feature[i];
		}
	}
	cout << "finish iteration..." << endl;
	
	// 读取测试集文件
	err = fopen_s(&fp, "save_test.txt", "r");
	if (err != 0) {
		cout << "failed to read the test set..." << endl;
		exit(1);
	}

	char useless[3500];
	fgets(useless, sizeof(useless), fp);   // 跳过第一行
	// 读取数据
	for (int i = 0; i < 25000; i++) {
		int id;
		fscanf_s(fp, "%d", &id);
		for (int j = 0; j < 384; j++) {
			fscanf_s(fp, "%lf", &testvalue[i][j]);
		}
	}
	fclose(fp);


	// 计算测试集
	for (int i = 0; i < 25000; i++) {
		reference[i] = 0;
		for (int j = 0; j < 385; j++) {
			if (j == 384)
				reference[i] += feature[j];
			else
				reference[i] += feature[j] * testvalue[i][j];
		}
	}

	// 将测试集结果写入csv文件
	ofstream outfile;
	outfile.open("result.csv", ios::out | ios::trunc);
	outfile << "id,reference" << endl;
	for (int i = 0; i < 25000; i++)
		outfile << i << "," << reference[i] << endl;
	outfile.close();

	cout << "finish writing result.csv..." << endl;
	system("pause");
    return 0;
}

void getRandSample()
{
	for (int i = 0; i < 25000; i++) {
		int index = rand() % 25000;
		int temp = sample[i];
		sample[i] = sample[index];
		sample[index] = temp;
	}
}

double getJ(int index) {
	double J = 0;
	for (int i = 0; i < M; i++) {
		int reindex = sample[i];
		double x = value[reindex][index];  // 每一个特征值对应的value值
		if (index == 384) {
			J += h[i];
		}
		else {
			J += (h[i] * x);
		}
	}
	return J;
}

void getH() {
	for (int i = 0; i < M; i++) {
		h[i] = 0;
		int index = sample[i];
		for (int j = 0; j < 385; j++) {
			if (j == 384) {
				h[i] += feature[384];
			}
			else {
				h[i] += feature[j] * value[index][j];
			}
		}
		h[i] -= value[index][384];
	}
}
