#include <iostream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <time.h>
#include <ctime>
using namespace std;

const int SNUM = 16;
const int ANUM = 4;
const double R = 0.8;  //折扣因子
const double ALPHA = 0.8; //学习率
const double REWARD = 0; //收益
const double EPSILON = 0.3; //Epsilon greedy
const double THETA = 0.1; //判断当前v和上一轮v差值是否足够小
const int OBSTACLE1 = 50;  //障碍物的位置
const int OBSTACLE2 = 140;

//函数原型
void print_value(double *v);
void print_policy(double(*pi)[ANUM]);
void value_iteration(double *v, double(*pi)[ANUM], double delta);
void mc_evaluation(double *v, double(*episode)[ANUM*ANUM], double *r);
void episode_generate(double(*pi)[ANUM], double(*episode)[ANUM*ANUM]);
void td_evaluation(double *v, double(*episode)[ANUM*ANUM]);
void td_control(double *v, double(*pi)[ANUM]);
void policy_evaluation(double *v, double(*pi)[ANUM], double delta);

//策略评估
void policy_evaluation(double *v, double(*pi)[ANUM], double delta = 0) {
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
	cout << "策略评估后：" << endl;
	print_value(v);
}

//值迭代
void value_iteration(double *v, double(*pi)[ANUM], double delta = 0) {
	double argmaxv[SNUM][ANUM] = { 0 }; //每个状态的策略

	do {
		delta = 0;
		double each_value[ANUM] = { 0 };

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
		//print_value(v);  //打印
	} while (delta >= THETA);

	//导出策略
	for (int t = 1; t < 16; t++) {
		for (int s = 0; s < 4; s++) {
			pi[t][s] = argmaxv[t][s];
		}
	}

	/*cout << "策略已达最优" << endl;
	cout << "V*： " << endl;
	print_value(v);
	cout << "Pi*： " << endl;
	print_policy(pi);*/
}

//穷举episode
void episode_generate(double(*pi)[ANUM], double(*episode)[ANUM*ANUM]) {
	int num_epi = 0;  //记录穷举过程中的当前编号
	int foot_epi = 0;  //记录每个episode的当前步数
	bool is_fin = false;  //判断当前episode是否到达终点 

	for (int j = 0; j < SNUM*ANUM; j++) {   //初始化episode
		for (int k = 0; k < ANUM*ANUM; k++) {
			episode[j][k] = 0;
		}
	}

	for (int i = 1; i < SNUM; i++) {
		if (i != OBSTACLE1 && i != OBSTACLE2) {  //判断是否为障碍物
			int tempi = i;  //记录当前i
			bool mult_root = false; //判断是否存在岔路
			episode[num_epi][foot_epi] = i;
			while (is_fin == false) {  //找出一条完整的episode(计算多选择节点的主选项)
				if (pi[i][0] == 1.0) {
					i = i - 4;
				}
				else if (pi[i][1] == 1.0) {
					i = i + 4;
				}
				else if (pi[i][2] == 1.0) {
					i = i - 1;
				}
				else if (pi[i][3] == 1.0) {
					i = i + 1;
				}
				else if (pi[i][0] == 0.5 && pi[i][1] == 0.5) {
					i = i - 4;
					mult_root = true;
				}
				else if (pi[i][0] == 0.5 && pi[i][2] == 0.5) {
					i = i - 4;
					mult_root = true;
				}
				else if (pi[i][0] == 0.5 && pi[i][3] == 0.5) {
					i = i - 4;
					mult_root = true;
				}
				else if (pi[i][1] == 0.5 && pi[i][2] == 0.5) {
					i = i - 1;
					mult_root = true;
				}
				else if (pi[i][1] == 0.5 && pi[i][3] == 0.5) {
					i = i + 4;
					mult_root = true;
				}
				else if (pi[i][2] == 0.5 && pi[i][3] == 0.5) {
					i = i - 1;
					mult_root = true;
				}
				foot_epi++;
				episode[num_epi][foot_epi] = i;
				if (i == 0) {
					num_epi++;
					foot_epi = 0;
					i = tempi;
					is_fin = true;
				}
			}

			if (mult_root == true) {  //判断是否有多路径
				episode[num_epi][foot_epi] = i;
				is_fin = false;
				while (is_fin == false) {  //找出一条完整的episode(计算多选择节点的次选项)
					if (pi[i][0] == 1) {
						i = i - 4;
					}
					else if (pi[i][1] == 1) {
						i = i + 4;
					}
					else if (pi[i][2] == 1) {
						i = i - 1;
					}
					else if (pi[i][3] == 1) {
						i = i + 1;
					}
					else if (pi[i][0] == 0.5 && pi[i][1] == 0.5) {
						i = i + 4;
					}
					else if (pi[i][0] == 0.5 && pi[i][2] == 0.5) {
						i = i - 1;
					}
					else if (pi[i][0] == 0.5 && pi[i][3] == 0.5) {
						i = i + 1;
					}
					else if (pi[i][1] == 0.5 && pi[i][2] == 0.5) {
						i = i + 4;
					}
					else if (pi[i][1] == 0.5 && pi[i][3] == 0.5) {
						i = i + 1;
					}
					else if (pi[i][2] == 0.5 && pi[i][3] == 0.5) {
						i = i + 1;
					}
					foot_epi++;
					episode[num_epi][foot_epi] = i;
					if (i == 0) {
						i = tempi;
						num_epi++;
						foot_epi = 0;
						is_fin = true;
					}
				}
			}
		}
		is_fin = false;
	}

	cout << "该策略下的所有episode为： " << endl;
	for (int i = 0; i<64; i++)
	{
		for (int j = 0; j<16; j++)
		{
			cout << episode[i][j] << " ";
		}
		cout << endl;  //加上这个在外层循环中
	}
	cout << endl;
}

//MC策略评估
/*void mc_evaluation(double *v, double(*episode)[ANUM*ANUM], double *r) {
double num_arr[16] = { 0 }; //记录每个状态存在的episode个数
for (int i = 0; i < SNUM*ANUM; i++) {
if (episode[i][0] == 0) {   //若此条episode无信息则跳出循环
continue;
}
else {
for (int j = 0; j < ANUM*ANUM; j++) {
if (episode[i][j] == 0) {   //若此条episode无信息则跳出循环
break;
}
else {
int state = episode[i][j];  //记录当前是哪个状态
num_arr[state] ++;
for (int k = j; k < ANUM*ANUM; k++) {
if (episode[i][k] != 0)
r[state] --;  //每走一步收益都为-1
else
break;
}
v[state] = r[state] / num_arr[state];  //计算V值
}
}
}
}
cout << "MC算法计算出得V值为： " << endl;
print_value(v);
}*/

//MC策略评估 （constant-αMC ） 
void mc_evaluation(double *v, double(*episode)[ANUM*ANUM], double *r) {
	//double num_arr[16] = { 0 }; //记录每个状态存在的episode个数
	for (int i = 0; i < SNUM*ANUM; i++) {
		if (episode[i][0] == 0) {   //若此条episode无信息则跳出循环
			continue;
		}
		else {
			for (int j = 0; j < ANUM*ANUM; j++) {
				if (episode[i][j] == 0) {   //若此条episode无信息则跳出循环
					break;
				}
				else {
					int state = episode[i][j];  //记录当前是哪个状态
												//num_arr[state] ++;
					double Gt = 0;
					int s = 0;
					for (int k = j; k < ANUM*ANUM; k++) {
						if (episode[i][k] != 0) {
							double temp = pow(R, s);
							Gt = Gt + (temp * REWARD);
							s++;
						}
						else
							break;
					}
					//v[state] = r[state] / num_arr[state];  //计算V值
					v[state] = v[state] + ALPHA*(Gt - v[state]);  //计算V值
				}
			}
		}
	}
	cout << "MC算法计算出得V值为： " << endl;
	print_value(v);
}

//TD策略评估
void td_evaluation(double *v, double(*episode)[ANUM*ANUM]) {
	for (int i = 0; i < SNUM*ANUM; i++) {
		if (episode[i][0] == 0) {   //若此条episode无信息则跳出循环
			continue;
		}
		else {
			for (int j = 0; j < ANUM*ANUM; j++) {
				if (episode[i][j] == 0) {   //若此条episode无信息则跳出循环
					break;
				}
				else {
					int state = episode[i][j];  //记录当前是哪个状态
					int next_state = episode[i][j + 1];  //记录当前状态的下一个状态
					v[state] = v[state] + ALPHA*(REWARD + R*v[next_state] - v[state]);  //计算V值
				}
			}
		}
	}
	cout << "TD算法计算出得V值为： " << endl;
	print_value(v);
}

//TD-control
void td_control(double *v, double(*pi)[ANUM]) {
	//init
	double q[SNUM][ANUM] = { 0 };  //记录q值
	int times = 0;

	while (times < 10000000) {
		//(a)
		int episode[10000] = { 0 };
		int action[10000] = { 0 };  //记录每个状态的动作
		int curr_s = 15;  //设置起点为15,也可随机生成
		int curr_epis = 0;  //本条episode当前位置
		episode[curr_epis] = curr_s;
		srand((unsigned)time(0));  //生成随机种子
		while (curr_s != 0) {
	
			int rand_num = (rand() % (100) + 1);   //随机生成1-100中的数
			//cout << rand_num << endl;
			//cout << curr_s << " ";
			int up_num, down_num, left_num, right_num = 0;
			up_num = pi[curr_s][0] * 100;   //向上动作的截至数（同时也是下一动作的开始数）  
			down_num = up_num + pi[curr_s][1] * 100;  //向下动作的截至数（...）
			left_num = down_num + pi[curr_s][2] * 100;
			right_num = left_num + pi[curr_s][3] * 100;
			//cout  << "randnum :"<< rand_num << endl;
			//cout << "num: " ;
			//cout << up_num << " " << down_num << " " << left_num << " " << right_num << endl ;
			if (rand_num <= up_num) {
				action[curr_epis] = 0;
				curr_epis++;
				if (curr_s - 4 > 0) {
					curr_s = curr_s - 4;
				}
				episode[curr_epis] = curr_s;
			}
			else if (rand_num > up_num && rand_num <= down_num) {
				action[curr_epis] = 1;
				curr_epis++;
				if (curr_s + 4 < 15) {
					curr_s = curr_s + 4;
				}
				episode[curr_epis] = curr_s;
			}
			else if (rand_num > down_num && rand_num <= left_num) {
				action[curr_epis] = 2;
				curr_epis++;
				if (curr_s - 1 == 3 || curr_s - 1 == 7 || curr_s - 1 == 11) {
					curr_s = curr_s;
				}
				else {
					curr_s = curr_s - 1;
				}
				episode[curr_epis] = curr_s;
			}
			else if (rand_num > left_num) {
				action[curr_epis] = 3;
				curr_epis++;
				if (curr_s + 1 == 4 || curr_s + 1 == 8 || curr_s + 1 == 12 || curr_s + 1 == 16) {
					curr_s = curr_s;
				}
				else{
					curr_s = curr_s + 1;
				}
				episode[curr_epis] = curr_s;
			}
			if (curr_epis > 10000) {  //若生成episode过长，则舍弃之
				break;
			}
		}
		if (curr_epis > 10000) {   //若生成episode过长，则进行下一次循环
			/*cout << "过长的episode：" << endl;
			for (int o = 0; o < 10000; o++) {
				cout << episode[o] << " ";
			}*/
			continue;
		}
		 //打印episode
		/*cout << "episode：" << endl;
		for (int o = 0; o < curr_epis; o++) {
		cout << episode[o] << " ";
		}*/


		//(b)
		//srand((unsigned)time(0));  //生成随机种子
		for (int i = 0; i < curr_epis; i++) {

			//计算当前状态做出的动作
			/*int action = 0;  //记录当前动作
			int a = episode[(i + 1)] - episode[i];

			if (a == -4)
				action = 0;
			else if (a == 4)
				action = 1;
			else if (a == -1)
				action = 2;
			else if (a == 1)
				action = 3;
			else if (a == 0) {
				if (episode[i] == 12 || episode[i] == 13 || episode[i] == 14 || episode[i] == 15)
					action = 1;
				else if (episode[i] == 1 || episode[i] == 2 || episode[i] == 3 || episode[i] == 0)
					action = 0;
				else if (episode[i] == 4 || episode[i] == 8)
					action = 2;
				else if (episode[i] == 7 || episode[i] == 11)
					action = 3;
			}*/

			//计算下一状态做出的动作
			int action_next = 0;
			//srand((unsigned)time(NULL));  //生成随机种子
			int rand_num = (rand() % (100) + 1);   //随机生成1-100中的数
			int up_num = pi[episode[(i + 1)]][0] * 100;   //向上动作的截至数（同时也是下一动作的开始数）  
			int down_num = up_num + pi[episode[(i + 1)]][1] * 100;  //向下动作的截至数（...）
			int left_num = down_num + pi[episode[(i + 1)]][2] * 100;
			int right_num = left_num + pi[episode[(i + 1)]][3] * 100;
			//cout << "randnum :" << rand_num << endl;
			//cout << up_num << " " << down_num << " " << left_num << " " << right_num << endl;
			
			if (rand_num <= up_num) {
				action_next = 0;
			}
			else if (rand_num > up_num && rand_num <= down_num) {
				action_next = 1;
			}
			else if (rand_num > down_num && rand_num <= left_num) {
				action_next = 2;
			}
			else if (rand_num > left_num) {
				action_next = 3;
			}
			//cout << "next :" << action_next;
			//计算Q值
			if (episode[(i + 1)] == 0) {
				//cout << "!!!";
				q[episode[i]][action[i]] = q[episode[i]][action[i]] + ALPHA*(100 + R*0 - q[episode[i]][action[i]]);
			}
			else {
				q[episode[i]][action[i]] = q[episode[i]][action[i]] + ALPHA*(REWARD + R*q[episode[(i + 1)]][action_next] - q[episode[i]][action[i]]);
			}
			

		}

		//cout << endl;

		//(c)
		bool policy_stable = true;
		double old_action[ANUM] = { 0 };
		double num = 0;  //最大q值的数量
		int tab[ANUM] = { 0 };  //用于标记

		for (int j = 0; j < curr_epis; j++) {  //遍历当前episode中所有的点
			//double argmaxv[SNUM][ANUM] = { 0 }; //每个状态的策略
			//int argmaxq = 0;
			num = 0;
			for (int u = 0; u < 4; u++) {
				tab[u] = 0;
				old_action[u] = pi[episode[j]][u];
			}

			/*if (q[episode[j]][0] == q[episode[j]][1] && q[episode[j]][1] == q[episode[j]][2] && q[episode[j]][2] == q[episode[j]][3]) {  //若q相同则保持
				continue;
			}*/
			//else {  //否则最大的q值即最好的策略
				double temp = q[episode[j]][0];
				for (int k = 0; k < 4; k++) {
					if (q[episode[j]][k] > temp) {
						temp = q[episode[j]][k];
						//argmaxq = k;  //找到最大q值对应的action
					}
				}

				//以下两个for用于计算每个状态的动作转移概率
				for (int t = 0; t < 4; t++) {
					if (q[episode[j]][t] == temp) {
						num++;
						tab[t] = 1;  //标记最大v值对应的action
					}
				}

				/*for (int s = 0; s < 4; s++) {
					if (pi[episode[j]][s] == 1) {
						pi[episode[j]][s] = (1 / num);
					}
					else
						pi[episode[j]][s] = 0;
				}*/
			//}

			for (int m = 0; m < 4; m++) {  //使用贪心策略更新
				if (tab[m] == 1 ) {
					pi[episode[j]][m] = 1 / num - EPSILON / num; 
				}
				else {
					pi[episode[j]][m] = EPSILON / (4 - num);
				}
			}
			for (int n = 0; n < 4; n++) {  //判断策略是否变化
				if (pi[episode[j]][n] != old_action[n]) {
					policy_stable = false;
					break;
				}
			}
		}


		/*cout << "该策略为： " << endl;
		for (int i = 0; i<16; i++)
		{
			for (int j = 0; j<4; j++)
			{
				cout << pi[i][j] << " ";
			}
			cout << endl;  //加上这个在外层循环中
		}
		cout << endl;*/

		/*if (policy_stable) {     //判断策略是否达到最优
			cout << "经过" << times << "次循环******************************，";
			cout << "策略已达最优" << endl;
			cout << "Pi*： " << endl;
			cout << "最优策略为： " << endl;
			for (int i = 0; i<16; i++)
			{
				for (int j = 0; j<4; j++)
				{
					cout << pi[i][j] << " ";
				}
				cout << endl;  //加上这个在外层循环中
			}
			/*cout << "Q*： " << endl;
			cout << "最优Q为： " << endl;
			for (int i = 0; i<16; i++)
			{
				for (int j = 0; j<4; j++)
				{
					cout << q[i][j] << " ";
				}
				cout << endl;  //加上这个在外层循环中
			}
			cout << endl;
			cout << endl;
			//break;
		}*/
		//cout << "here";
		times++;
	}
	cout << "Pi*： " << endl;
	cout << "最优策略为： " << endl;
	for (int i = 0; i<16; i++)
	{
		for (int j = 0; j<4; j++)
		{
			cout << pi[i][j] << " ";
		}
		cout << endl;  //加上这个在外层循环中
	}
	cout << endl;
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
void print_policy(double(*pi)[ANUM]) {
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
	double *value = new double[SNUM];  //状态集
	for (int i = 0; i < 16; i++) {
		if (i == OBSTACLE1 || i == OBSTACLE2) {
			value[i] = -404;
		}
		else {
			value[i] = 0;
		}
	}

	double policy[SNUM][ANUM];   //策略集 ANUM下标分别对应‘上下左右’
	for (int j = 0; j < 16; j++) {
		for (int k = 0; k < 4; k++) {
			policy[j][k] = 0.25;
		}
	}

	//policy_evaluation(value, policy);   //策略迭代
	//value_iteration(value, policy);   //值迭代
	td_control(value ,policy);
	//policy_evaluation(value, policy);


									  //使用值迭代计算出一条策略用于接下来的MC与TD算法 
	/*cout << "用于MC和TD算法的策略：" << endl;
	print_policy(policy);   //打印用于MC和TD算法的策略
	cout << "用于MC和TD算法的初始V值：" << endl;
	for (int i = 0; i < 16; i++) {
		if (i == OBSTACLE1 || i == OBSTACLE2) {
			value[i] = -404;
		}
		else {
			value[i] = 0;
		}
	}
	print_value(value);  //打印用于MC和TD算法的初始V值

	double *returns = new double[SNUM];  //每个状态的累计回报
	for (int i = 0; i < 16; i++) {
		if (i == OBSTACLE1 || i == OBSTACLE2) {
			returns[i] = -404;
		}
		else {
			returns[i] = 0;
		}
	}

	double episode[SNUM*ANUM][ANUM*ANUM];   //存放episode，行表示第几个episode，列表示状态
	for (int j = 0; j < SNUM*ANUM; j++) {
		for (int k = 0; k < ANUM*ANUM; k++) {
			episode[j][k] = 0;
		}
	}


	episode_generate(policy, episode); //穷举episode
	mc_evaluation(value, episode, returns);  //MC算法计算V值
	td_evaluation(value, episode);  //TD算法计算V值
	*/


	delete[]value;  //回收指针
	value = NULL;

	system("pause");
	return 0;
}