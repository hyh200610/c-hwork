#include "test.h"
#include<iostream>
#include<string>
#include<cstdlib>
#include <limits>
using namespace std;

// 菜单函数：顶部实时显示当前余额
void menu(double balance)
{
	cout << "=====个人记账系统=====\n";
	cout << "【当前登记账目余额：" << balance << "】\n";
	cout << "1.新增账单\n";
	cout << "2.查看全部账单\n";
	cout << "3.根据序号删除账单\n";
	cout << "4.收支统计\n";
	cout << "5.保存账单到文件\n";
	cout << "6.手动修改/填写账目余额\n";
	cout << "0.退出程序\n";
	cout << "请输入功能序号：";
}

void clearCinErr()
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 用getline读取整行，彻底清空所有输入，无残留
void waitEnter()
{
	string temp;
	getline(cin, temp);
}

int main()
{
	try
	{
		accountbook book("我的日常账本");
		book.loadFromFile();
		cout << "初始化完成，按回车键进入主菜单...";
		string tmp;
		getline(cin, tmp);

		int op;
		while(true)
		{
			system("cls");
			double nowBal = book.getBalance();
			menu(nowBal); // 传入余额打印在菜单顶部
			cin >> op;

			// 拦截字母、符号等非法输入
			if(cin.fail())
			{
				clearCinErr();
				cout << "\n错误：仅允许输入数字！按回车键返回菜单..." << endl;
				waitEnter();
				continue;
			}

			// 限制只能输入0~6，大数直接拦截
			if(op < 0 || op > 6)
			{
				cout << "\n输入序号无效，请输入0-6之间的数字,按回车键返回菜单...\n";
				waitEnter();
				waitEnter();
				continue;
			}

			if(op == 0)
			{
				cout << "程序即将退出，是否保存？0取消退出 1保存 2不保存：";
				int s;
				cin >> s;
				clearCinErr(); // 清cin>>s留下的换行
				if(s!=0)
				{
					if(s == 1)
					{
						book.saveToFile();
					}
					cout << "程序结束\n";
					break;
				}
				else
				{
					waitEnter();
				}	
			}
			else if(op == 1)
			{
				string name;
				double money;
				cout << "请输入账单名称：";
				cin >> name;
				cout << "请输入金额(正数收入，负数支出)：";
				cin >> money;
				clearCinErr(); // 清数字输入残留换行
				book.add(name, money);
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
			}
			else if(op == 2)
			{
				book.show();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
				waitEnter();
			}
			else if(op == 3)
			{
				int showNum;
				cout << "请输入要删除账单的展示序号：";
				cin >> showNum;
				clearCinErr();
				book.deleteByIndex(showNum);
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
			}
			else if(op == 4)
			{
				book.stat();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
				waitEnter();
			}
			else if(op == 5)
			{
				book.saveToFile();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
				waitEnter();
			}
			else if(op == 6)
			{
				double newBal;
				cout << "请输入新的账目余额：";
				if(!(cin >> newBal))
				{
					clearCinErr();
					cout << "输入格式错误，请输入数字！\n";
				}
				else
				{
					book.setBalance(newBal);
					cout << "账目余额修改成功！\n";
				}
				clearCinErr();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
			}
		}
	}
	catch(const exception& e)
	{
		cout << "\n程序发生致命异常：" << e.what() << endl;
		cout << "程序安全退出\n";
	}
	catch(...)
	{
		cout << "\n未知异常，程序安全退出\n";
	}
	return 0;
}


