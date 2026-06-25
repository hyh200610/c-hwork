#include "test.h"
#include<iostream>
#include<string>
#include<cstdlib>
#include <limits>
using namespace std;

void menu(double balance)
{
	cout << "=====个人记账系统=====\n";
	cout << "【当前钱包余额：" << balance << "】\n";
	cout << "1.新增账单\n";
	cout << "2.查看全部账单\n";
	cout << "3.根据序号删除账单\n";
	cout << "4.收支统计\n";
	cout << "5.保存账单到文件\n";
	cout << "6.修改钱包初始本金\n";
	cout << "7.按序号修改账单信息\n";
	cout << "0.退出程序\n";
	cout << "请输入功能序号：";
}

void clearCinErr()
{
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

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
		while (true)
		{
			#ifdef _WIN32
				system("cls");
			#else
				system("clear");
			#endif
			double nowBal = book.getBalance();
			menu(nowBal);
			cin >> op;

			if (cin.fail())
			{
				clearCinErr();
				cout << "\n错误：仅允许输入数字！按回车键返回菜单..." << endl;
				waitEnter();
				continue;
			}

			// 范围0~7
			if (op < 0 || op > 7)
			{
				cout << "\n输入序号无效，请输入0-7之间的数字,按回车键返回菜单...\n";
				waitEnter();
				waitEnter();
				continue;
			}

			if (op == 0)
			{
				cout << "程序即将退出，是否保存？0取消退出 1保存 2不保存：";
				int s;
				cin >> s;
				clearCinErr();
				if (s != 0)
				{
					if (s == 1)
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
			else if (op == 1)
			{
				string name, dateInput;
				double money;
				cout << "请输入账单名称：";
				cin >> name;
				clearCinErr();
				cout << "请输入账单日期(格式2026.1.12，直接回车使用今日)：";
				getline(cin, dateInput);
				cout << "请输入金额(正数收入，负数支出)：";
				cin >> money;
				clearCinErr();
				book.add(name, money, dateInput);
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
			}
			else if (op == 2)
			{
				book.show();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
				waitEnter();
			}
			else if (op == 3)
			{
				cout << "====当前所有账单====" << endl;
				book.show();
				cout << "\n请输入要删除账单的展示序号：";
				int showNum;
				cin >> showNum;
				clearCinErr();
				book.deleteByIndex(showNum);
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
			}
			else if (op == 4)
			{
				book.stat();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
				waitEnter();
			}
			else if (op == 5)
			{
				book.saveToFile();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
				waitEnter();
			}
			else if (op == 6)
			{
				double initMoney;
				cout << "请输入钱包初始本金（最开始手里的钱）：";
				if (!(cin >> initMoney))
				{
					clearCinErr();
					cout << "输入格式错误，请输入数字！\n";
				}
				else
				{
					book.setInitMoney(initMoney);
					cout << "钱包初始本金修改成功！当前余额将自动重新计算\n";
				}
				clearCinErr();
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
			}
			else if (op == 7)
			{
				cout << "====当前所有账单====" << endl;
				book.show();
				int targetIdx, choice;
				cout << "\n请输入需要修改的账单序号：";
				cin >> targetIdx;
				clearCinErr();
				cout << "请选择修改项目：1名称  2金额  3日期：";
				cin >> choice;
				clearCinErr();

				if (choice == 1)
				{
					string newName;
					cout << "输入新账单名称：";
					getline(cin, newName);
					book.modifyBill(targetIdx, 1, newName);
				}
				else if (choice == 2)
				{
					double newMoney;
					cout << "输入新金额(收入正/支出负)：";
					cin >> newMoney;
					clearCinErr();
					book.modifyBill(targetIdx, 2, "", newMoney);
				}
				else if (choice == 3)
				{
					string newDate;
					cout << "输入新日期(2026.1.12，回车默认今日)：";
					getline(cin, newDate);
					book.modifyBill(targetIdx, 3, newDate);
				}
				else
				{
					cout << "选择项目无效！";
				}
				cout << "\n操作完成，按回车键返回菜单...";
				waitEnter();
			}
		}
	}
	catch (const exception& e)
	{
		cout << "\n程序发生致命异常：" << e.what() << endl;
		cout << "程序安全退出\n";
	}
	catch (...)
	{
		cout << "\n未知异常，程序安全退出\n";
	}
	return 0;
}
