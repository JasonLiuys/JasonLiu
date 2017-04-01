#include <iostream>
#include <algorithm>
#include <math.h>
#include <iomanip>
using namespace std;

const int SNUM = 16;
const int ANUM = 4;
const double R = 0.9;  //折扣因子
const double REWARD = -1; //收益
const double THETA = 0.1; //判断当前v和上一轮v差值是否足够小
const int OBSTACLE1 = 5;  //障碍物的位置
const int OBSTACLE2 = 14;  

//函数原型
void policy_evaluation(double *v, double(*pi)[ANUM], double delta);
void print_value(double *v);
void policy_improvement(double *v, double(*pi)[ANUM]);
void print_policy(double(*pi)[ANUM]);
void value_iteration(double *v, double(*pi)[ANUM], double delta);

//策略评估
void policy_evaluation( double *v , double (*pi)[ANUM] , double delta = 0) {
	//cout << "策略评估前：" << endl;
	//print_value(v);
	do {
		delta = 0;

		double temp_array[16];   //临时数组存放上一次迭代的v值
		for (int j = 0; j < 16; j++) {  
			temp_array[j] = v[j];
		} 
		//print_value(temp_array);
		for (int i = 1; i < 16; i++) {
			if (i != OBSTACLE1 && i != OBSTACLE2) {  //判断是否为障碍物
				double temp = v[i];

				int up, down, left, right = 0;   //计算当前状态的上下左右状态
				if (i - 1 == 3 || i - 1 == 7 || i - 1 == 11 || i - 1 == OBSTACLE1 || i - 1 == OBSTACLE2) {
					left = i;
				}
				else {
					left = i - 1;
				}
				if (i + 1 == 4 || i + 1 == 8 || i + 1 == 12 || i + 1 == 16 || i + 1 == OBSTACLE1 || i + 1 == OBSTACLE2) {
					right = i;
				}
				else {
					right = i + 1;
				}
				if (i - 4 < 0 || i - 4 == OBSTACLE1 || i - 4 == OBSTACLE2) {
					up = i;
				}
				else {
					up = i - 4;
				}
				if (i + 4 > 15 || i + 4 == OBSTACLE1 || i + 4 == OBSTACLE2) {
					down = i;
				}
				else {
					down = i + 4;
				}

				//计算本次迭代的v值
				v[i] = pi[i][0] * (REWARD + R*temp_array[up]) + pi[i][1] * (REWARD + R*temp_array[down]);
				v[i] += pi[i][2] * (REWARD + R*temp_array[left]) + pi[i][3] * (REWARD + R*temp_array[right]);

				double foo = fabs((temp_array[i] - v[i]));
				delta = max(delta, foo);
			}

		}
		
	} while (delta >= THETA);
	//cout << "策略评估后：" << endl;
	//print_value(v);
	policy_improvement(v, pi);  //进行策略改进
}

//策略改进
void policy_improvement(double *v, double(*pi)[ANUM]) {
	//cout << "策略改进前：" << endl;
	//print_policy(pi);
	bool policy_stable = true;
	double old_action[ANUM] = { 0 };
	double q[ANUM] = { 0 };  //四个动作的q值
	double argmaxv[SNUM][ANUM] = { 0 }; //每个状态的策略
	int argmaxq = 0;

	for (int i = 1; i < 16; i++) {
		if (i != OBSTACLE1 && i != OBSTACLE2) {  //判断是否为障碍物
			for (int j = 0; j < 4; j++) {
				old_action[j] = pi[i][j];
			}

			int up, down, left, right = 0;   //计算当前状态的上下左右状态
			if (i - 1 == 3 || i - 1 == 7 || i - 1 == 11 || i - 1 == OBSTACLE1 || i - 1 == OBSTACLE2) {
				left = i;
			}
			else {
				left = i - 1;
			}
			if (i + 1 == 4 || i + 1 == 8 || i + 1 == 12 || i + 1 == 16 || i + 1 == OBSTACLE1 || i + 1 == OBSTACLE2) {
				right = i;
			}
			else {
				right = i + 1;
			}
			if (i - 4 < 0 || i - 4 == OBSTACLE1 || i - 4 == OBSTACLE2) {
				up = i;
			}
			else {
				up = i - 4;
			}
			if (i + 4 > 15 || i + 4 == OBSTACLE1 || i + 4 == OBSTACLE2) {
				down = i;
			}
			else {
				down = i + 4;
			}

			q[0] = (REWARD + R*v[up]);   //计算q值
			q[1] = (REWARD + R*v[down]);
			q[2] = (REWARD + R*v[left]);
			q[3] = (REWARD + R*v[right]);

			//cout << q[0] << "  " << q[1] << "  " << q[2] << "  " << q[3] << "  " << endl;

			if (q[0] == q[1] && q[1] == q[2] && q[2] == q[3]) {  //若q相同则保持
				continue;
			}
			else {  //否则最大的q值即最好的策略
				double temp = q[0];
				argmaxq = 0;
				double num = 0;
				for (int k = 0; k < 4; k++) {
					if (q[k] > temp) {
						temp = q[k];
						argmaxq = k;  //找到最大q值对应的action
					}
				}

				//以下两个for用于计算每个状态的动作转移概率
				for (int k = 0; k < 4; k++) {
					if (q[k] == temp) {
						num++;
						argmaxv[i][k] = 1;  //标记最大v值对应的action
					}
				}
				for (int q = 0; q < 4; q++) {
					if (argmaxv[i][q] == 1)
						argmaxv[i][q] = (1 / num);
					else
						argmaxv[i][q] = 0;
				}
			}


			for (int m = 0; m < 4; m++) {  //使用贪心策略更新
				pi[i][m] = argmaxv[i][m];
			}

			for (int n = 0; n < 4; n++) {  //判断策略是否变化
				if (pi[i][n] != old_action[n]) {
					policy_stable = false;
					break;
				}
			}
		}  
	}

	cout << "策略改进后：" << endl;
	print_policy(pi);
	print_value(v);

	if (policy_stable) {     //判断策略是否达到最优
		cout << "策略已达最优" << endl;
		cout << "V*： " << endl;
		print_value(v);
		cout << "Pi*： " << endl;
		print_policy(pi);
	}
	else {
		policy_evaluation(v, pi);  //进行策略评估
	}
}

//值迭代
void value_iteration(double *v, double(*pi)[ANUM], double delta = 0){
	double argmaxv[SNUM][ANUM] = { 0 }; //每个状态的策略

	do {
		delta = 0;
		double each_value[ANUM] = { 0 } ;

		double temp_array[16];   //临时数组存放上一次迭代的v值
		for (int j = 0; j < 16; j++) {
			temp_array[j] = v[j];
		}
		
		for (int i = 1; i < 16; i++) {
			if (i != OBSTACLE1 && i != OBSTACLE2) {  //判断是否为障碍物
				double temp = v[i];

				int up, down, left, right = 0;   //计算当前状态的上下左右状态
				if (i - 1 == 3 || i - 1 == 7 || i - 1 == 11 || i - 1 == OBSTACLE1 || i - 1 == OBSTACLE2) {
					left = i;
				}
				else {
					left = i - 1;
				}
				if (i + 1 == 4 || i + 1 == 8 || i + 1 == 12 || i + 1 == 16 || i + 1 == OBSTACLE1 || i + 1 == OBSTACLE2) {
					right = i;
				}
				else {
					right = i + 1;
				}
				if (i - 4 < 0 || i - 4 == OBSTACLE1 || i - 4 == OBSTACLE2) {
					up = i;
				}
				else {
					up = i - 4;
				}
				if (i + 4 > 15 || i + 4 == OBSTACLE1 || i + 4 == OBSTACLE2) {
					down = i;
				}
				else {
					down = i + 4;
				}

				//计算每个动作的v值
				each_value[0] = (REWARD + R*temp_array[up]);
				each_value[1] = (REWARD + R*temp_array[down]);
				each_value[2] = (REWARD + R*temp_array[left]);
				each_value[3] = (REWARD + R*temp_array[right]);


				double temp_max = each_value[0];
				double num = 0;

				//找到最大的v值
				for (int k = 0; k < 4; k++) {
					if (each_value[k] > temp_max) {
						temp_max = each_value[k];
					}
				}

				//以下两个for用于计算每个状态的动作转移概率
				//找出最大v值有几个
				for (int k = 0; k < 4; k++) {
					if (each_value[k] == temp_max) {
						num++;
						argmaxv[i][k] = 1;  //标记最大v值对应的action
					}
				}

				for (int q = 0; q < 4; q++) {
					if (argmaxv[i][q] == 1)
						argmaxv[i][q] = (1 / num);
					else
						argmaxv[i][q] = 0;
				}

				v[i] = temp_max;   //将最大v值赋给当前v

				double foo = fabs((temp_array[i] - v[i]));
				delta = max(delta, foo);
			}
			
		}
		print_value(v);  //打印
	} while (delta >= THETA);

	//导出策略
	for (int t = 1; t < 16; t++ ) {
		for (int s = 0; s < 4; s++) {
			pi[t][s] = argmaxv[t][s];
		}
	}

	cout << "策略已达最优" << endl;
	cout << "V*： " << endl;
	print_value(v);
	cout << "Pi*： " << endl;
	print_policy(pi);
}

//打印v值
void print_value(double *v) {   //打印所有状态value值
	for (int i = 0; i < 16; i++) {
		cout << fixed << setprecision(1) << v[i] << "   ";
		if (i == 3 || i == 7 || i == 11) {
			cout << endl;
		}
		if (i == 15) {
			cout << endl << endl << endl;
		}
	}
}

//打印策略
void print_policy(double(*pi)[ANUM]){
	cout << "$" << "   ";
	for (int i = 1; i < 16; i++) { 
		if (i != OBSTACLE1 && i != OBSTACLE2) {  //判断是否为障碍物
			if (pi[i][0] == 1)
				cout << "↑" << "   ";
			if (pi[i][1] == 1)
				cout << "↓" << "   ";
			if (pi[i][2] == 1)
				cout << "←" << "   ";
			if (pi[i][3] == 1)
				cout << "→" << "   ";
			if (pi[i][0] == 0.5 && pi[i][1] == 0.5)
				cout << "↕" << "   ";
			if (pi[i][0] == 0.5 && pi[i][2] == 0.5)
				cout << "↖" << "   ";
			if (pi[i][0] == 0.5 && pi[i][3] == 0.5)
				cout << "↗" << "   ";
			if (pi[i][1] == 0.5 && pi[i][2] == 0.5)
				cout << "↙" << "   ";
			if (pi[i][1] == 0.5 && pi[i][3] == 0.5)
				cout << "↘" << "   ";
			if (pi[i][2] == 0.5 && pi[i][3] == 0.5)
				cout << "↔" << "   ";
			if (pi[i][0] == pi[i][1] && pi[i][1] == pi[i][2] && pi[i][2] == pi[i][3])
				cout << "*" << "   ";
			if (i == 3 || i == 7 || i == 11) {
				cout << endl;
			}
			if (i == 15) {
				cout << endl << endl << endl;
			}
		}
		else {
			cout << "X" << "   ";
			if (i == 3 || i == 7 || i == 11) {
				cout << endl;
			}
			if (i == 15) {
				cout << endl << endl << endl;
			}
		}
		
	}
}


int main() {
	double *value = new double[SNUM] ;  //状态集
	for (int i = 0; i < 16; i ++ ) {
		if (i == OBSTACLE1 || i == OBSTACLE2){
			value[i] = -404;
		}
		else {
			value[i] = 0;
		}
	}

	double policy[SNUM][ANUM];   //策略集 ANUM下标分别对应‘上下左右’
	for (int j = 0; j < 16; j ++ ) {
		for (int k = 0; k < 4; k ++ ) {
			policy[j][k] = 0.25; 
		}
	}

	//policy_evaluation(value, policy);   //策略迭代
	value_iteration(value, policy);   //值迭代

	delete []value;  //回收指针
	value = NULL;

	system("pause");
	return 0;
}