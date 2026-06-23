#include "test.h"
#include<iostream>
#include<string>
#include<cstdlib>
using namespace std;

void menu()
{
	cout << "=====个人记账系统=====\n";
	cout << "1.新增账单\n";
	cout << "2.查看全部账单\n";
	cout << "3.根据序号删除账单\n";
	cout << "4.收支统计\n";
	cout << "5.保存账单到文件\n";
	cout << "0.退出程序\n";
	cout << "请输入功能序号：";
}

void clearCinErr()
{
	cin.clear();
	cin.ignore(9999, '\n');
}

int main()
{
	try
	{
		accountbook book("我的日常账本");
		book.loadFromFile();
		cout << "初始化完成，按回车键进入主菜单...";
		cin.get();
		clearCinErr();

		int op;
		while(true)
		{
			system("cls");
			menu();
			cin >> op;

			// 拦截字母、符号等非法输入
			if(cin.fail())
			{
				clearCinErr();
				cout << "\n错误：仅允许输入数字！" << endl;
				cout << "按回车键返回菜单...";
				cin.get();
				continue;
			}

			if(op == 0)
			{
				cout << "程序即将退出，是否自动保存？1保存 0不保存：";
				int s;
				cin >> s;
				clearCinErr();
				if(s == 1)
					book.saveToFile();
				cout << "程序结束\n";
				break;
			}
			else if(op == 1)
			{
				string name;
				double money;
				cout << "请输入账单名称：";
				cin >> name;
				cout << "请输入金额(正数收入，负数支出)：";
				cin >> money;
				clearCinErr();
				book.add(name, money);
				cout << "\n操作完成，按回车返回菜单...";
				cin.ignore();
				cin.get();
			}
			else if(op == 2)
			{
				book.show();
				cout << "\n按回车返回菜单...";
				cin.ignore();
				cin.get();
			}
			else if(op == 3)
			{
				int showNum;
				cout << "请输入要删除账单的展示序号：";
				cin >> showNum;
				clearCinErr();
				book.deleteByIndex(showNum);
				cout << "\n操作完成，按回车返回菜单...";
				cin.ignore();
				cin.get();
			}
			else if(op == 4)
			{
				book.stat();
				cout << "\n按回车返回菜单...";
				cin.ignore();
				cin.get();
			}
			else if(op == 5)
			{
				book.saveToFile();
				cout << "\n操作完成，按回车返回菜单...";
				cin.ignore();
				cin.get();
			}
			else
			{
				cout << "输入序号无效，请输入0-5之间的数字\n";
				cout << "按回车返回菜单...";
				cin.ignore();
				cin.get();
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
