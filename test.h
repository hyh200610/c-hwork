#ifndef TEST_H
#define TEST_H

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdexcept>

class bill
{
	private:
		std::string name;
		int id;
		double amt;
	public:
		bill(){}
		bill(std::string name1,int id1,double amt1):name(name1),id(id1),amt(amt1){}
		~bill(){
			// std::cout<<"id:"<<id<<"删除成功"<<std::endl;
		}
		std::string get_n() const {return name;}
		int get_id() const {return id;}
		double get_a() const {return amt;}
		void set_n(std::string n){name=n;}
		void set_id(int id1){id=id1;}
		void set_amt(double a){amt=a;}
};

class accountbook
{
	private:
		std::vector<bill> list1;
		std::string name;
		int realIdSeed;
		double totalBalance; // 全局账目余额
	public:
		accountbook():name("默认账本"),realIdSeed(1),totalBalance(0.0){}
		accountbook(std::string n):name(n),realIdSeed(1),totalBalance(0.0){}

		// 获取余额get、设置余额set 成对齐全
		double getBalance() const
		{
			return totalBalance;
		}
		void setBalance(double bal)
		{
			totalBalance = bal;
		}

		void add(std::string n,double a)
		{
			try{
				if(n.empty()){
					throw std::invalid_argument("账单名称不能为空");
				}
				int realId = realIdSeed++;
				list1.push_back(bill(n, realId, a));
				totalBalance += a; // 新增账单同步更新余额
				int showIndex = static_cast<int>(list1.size());
				std::cout<<"账单添加成功，序号："<<showIndex<<std::endl;
			}catch(const std::exception& e){
				std::cout<<"添加账单失败："<<e.what()<<std::endl;
			}
		}

		void show() const
		{
			try{
				if(list1.empty())
				{
					std::cout<<"暂无账单记录\n";
					return;
				}
				std::cout<<"======"<<name<<"======\n";
				for(size_t i=0;i<list1.size();i++)
				{
					std::cout<<"序号:"<<i+1<<" 名称:"<<list1[i].get_n()<<" 金额:"<<list1[i].get_a()<<std::endl;
				}
				std::cout<<"当前登记账目余额："<<totalBalance<<std::endl;
			}catch(...){
				std::cout<<"读取账单列表异常，展示失败\n";
			}
		}

		void deleteByIndex(int showIdx)
		{
			try
			{
				if(showIdx <= 0)
					throw std::invalid_argument("序号必须是正整数");
				size_t targetPos = static_cast<size_t>(showIdx - 1);
				if(targetPos >= list1.size())
					throw std::runtime_error("该序号不存在");

				// 删除前先扣减余额
				double delMoney = list1[targetPos].get_a();
				totalBalance -= delMoney;

				list1.erase(list1.begin() + targetPos);
				std::cout<<"序号"<<showIdx<<" 删除成功\n";
			}
			catch(const std::exception& e)
			{
				std::cout<<"删除失败："<<e.what()<<std::endl;
			}
		}

	private:
		void deleteid(int delid)
		{
			try{
				if(delid <= 0){
					throw std::invalid_argument("ID必须为正整数");
				}
				int idx=-1;
				for(size_t i=0;i<list1.size();i++)
				{
					if(list1[i].get_id()==delid)
					{
						idx=static_cast<int>(i);
						break;
					}
				}
				if(idx==-1)
				{
					throw std::runtime_error("未找到该底层ID账单");
				}
				double delMoney = list1[idx].get_a();
				totalBalance -= delMoney;
				list1.erase(list1.begin()+idx);
				std::cout<<"底层ID:"<<delid<<" 删除成功\n";
			}catch(const std::exception& e){
				std::cout<<"删除失败："<<e.what()<<std::endl;
			}
		}

	public:
		void stat() const
		{
			try{
				double income=0,expend=0;
				for(const auto &b:list1)
				{
					double money=b.get_a();
					if(money>0) income+=money;
					else expend-=money;
				}
				std::cout<<"========收支统计========"<<std::endl;
				std::cout<<"总收入："<<income<<std::endl;
				std::cout<<"总支出："<<expend<<std::endl;
				std::cout<<"账面理论结余："<<income-expend<<std::endl;
				std::cout<<"手动登记账目余额："<<totalBalance<<std::endl;
			}catch(...){
				std::cout<<"统计数据异常\n";
			}
		}

		// 文件第一行保存余额，后续每行保存账单
		void saveToFile(std::string filePath = "bill.txt")
		{
			try{
				std::ofstream out(filePath);
				if (!out.is_open())
				{
					throw std::runtime_error("文件无法打开，无写入权限");
				}
				out << totalBalance << std::endl; // 第一行写入余额
				for(const auto &b : list1)
				{
					out << b.get_id() << " " << b.get_n() << " " << b.get_a() << std::endl;
				}
				out.close();
				std::cout<<"数据已保存到文件\n";
			}catch(const std::exception& e){
				std::cout<<"保存失败："<<e.what()<<std::endl;
			}
		}

		void loadFromFile(std::string filePath = "bill.txt")
		{
			try{
				std::ifstream in(filePath);
				if (!in.is_open())
				{
					std::cout << "无本地账单文件\n";
					totalBalance = 0; // 无文件余额归零
					return;
				}
				list1.clear();
				// 先读取第一行余额
				in >> totalBalance;
				int id;
				std::string name;
				double amt;
				while (in >> id >> name >> amt)
				{
					if(id > 0)
					{
						list1.push_back(bill(name, id, amt));
						if(id >= realIdSeed)
							realIdSeed = id + 1;
					}
				}
				in.close();
				std::cout << "读取本地账单完成\n";
				std::cout << "读取到登记账目余额：" << totalBalance << std::endl;
			}catch(const std::exception& e){
				std::cout<<"读取文件失败："<<e.what()<<std::endl;
				totalBalance = 0;
			}
		}
};

#endif
