#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

using namespace std;

int main() {
	cout << "<计算核酸序列中的CG含量>  " << endl;
	cout << "------Copyright 2017 何昕格------- " << endl << endl;

	while (1) {
		cout << endl << "***命令行输入核酸序列请输入1, 读取txt文件请输入2, 退出请输入其他字符***" << endl;
		string choose;
		cin >> choose;
		double length = 0;		  //核酸序列总长度
		double CG_num = 0;		  //核酸序列中CG的数量
		double content = 0;       //核酸序列中CG的含量

								  //命令行输入核酸序列
		if (choose == "1") {
			cout << "***请输入完整的核酸序列：" << endl;
			string Str;
			cin >> Str;
			length = Str.length();  //核酸序列总长度

			for (int i = 0; i < length; i++) {
				if (Str[i] == 'C' || Str[i] == 'c' || Str[i] == 'G' || Str[i] == 'g') {
					CG_num++;
				}
			}
			content = CG_num / length;
			cout << "---核酸序列中CG的数量为：" << CG_num << endl;
			cout << "---核酸序列中CG的含量为：" << content * 100 << "%" << endl;
		}

		//读取txt文件
		else if (choose == "2") {
			cout << "***请输入文件名：（包括拓展名txt）" << endl;
			string file;
			cin >> file;
			ifstream infile;
			infile.open(file.data());
			if (infile.is_open()) {
				char c;
				while (!infile.eof())
				{
					infile >> c;
					if (infile.fail())
						break;
					if (c == 'C' || c == 'c' || c == 'G' || c == 'g') {
						CG_num++;
					}
					length++;
				}
				content = CG_num / length;
				cout << "---核酸序列中CG的数量为：" << CG_num << endl;
				cout << "---核酸序列中CG的含量为：" << content * 100 << "%" << endl;
				infile.close();             //关闭文件输入流 
			}
			else {
				cout << "---找不到文件，请将文件放在与本程序同一目录下." << endl;
				//assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
			}

		}

		else {
			break;
			return 0;
		}
	}
	return 0;
}