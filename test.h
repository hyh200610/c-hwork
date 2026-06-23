#ifndef TEST_H
#define TEST_H

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdexcept> // 异常头文件

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
			// 注释掉打印，避免销毁时访问野id闪退
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
		int realIdSeed; // 全局唯一真实ID生成种子
	public:
		accountbook():name("默认账本"),realIdSeed(1){}
		accountbook(std::string n):name(n),realIdSeed(1){}

		void add(std::string n,double a)
		{
			try{
				if(n.empty()){
					throw std::invalid_argument("账单名称不能为空");
				}
				int realId = realIdSeed++;
				list1.push_back(bill(n, realId, a));
				// 只输出展示序号，隐藏底层ID
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
					// 仅打印连续展示序号，不输出底层真实ID
					std::cout<<"序号:"<<i+1<<" 名称:"<<list1[i].get_n()<<" 金额:"<<list1[i].get_a()<<std::endl;
				}
			}catch(...){
				std::cout<<"读取账单列表异常，展示失败\n";
			}
		}

		// 【用户唯一可用删除接口】按界面展示序号删除，直接按下标定位，不检索底层ID
		void deleteByIndex(int showIdx)
		{
			try
			{
				if(showIdx <= 0)
					throw std::invalid_argument("序号必须是正整数");
				size_t targetPos = static_cast<size_t>(showIdx - 1);
				if(targetPos >= list1.size())
					throw std::runtime_error("该序号不存在");

				// 直接按下标erase，完全不遍历查找底层ID
				list1.erase(list1.begin() + targetPos);
				std::cout<<"序号"<<showIdx<<" 删除成功\n";
			}
			catch(const std::exception& e)
			{
				std::cout<<"删除失败："<<e.what()<<std::endl;
			}
		}

		// 私有，屏蔽外部调用，仅内部备用，用户菜单不再提供该功能
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
				std::cout<<"结余："<<income-expend<<std::endl;
			}catch(...){
				std::cout<<"统计数据异常\n";
			}
		}

		void saveToFile(std::string filePath = "bill.txt")
		{
			try{
				std::ofstream out(filePath);
				if (!out.is_open())
				{
					throw std::runtime_error("文件无法打开，无写入权限");
				}
				// txt文件依旧保存底层真实ID，打开文件可查看底层编号
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
					return;
				}
				list1.clear();
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
			}catch(const std::exception& e){
				std::cout<<"读取文件失败："<<e.what()<<std::endl;
			}
		}
};

#endif
