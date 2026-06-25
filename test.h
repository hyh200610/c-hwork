#ifndef TEST_H
#define TEST_H

#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<stdexcept>
#include<ctime>
#include<sstream>
#include<cctype>
#include<algorithm>
#include<iomanip>

// 单独日期基类
class Date
{
protected:
	std::string dateStr; // 统一格式 yyyy.MM.dd
public:
	// 获取当前系统日期，自动格式化为 yyyy.xx.xx
	std::string getCurrentDate()
	{
		std::time_t now = std::time(nullptr);
		std::tm* t = std::localtime(&now);
		std::ostringstream oss;
		oss << std::setfill('0')
			<< t->tm_year + 1900 << "."
			<< std::setw(2) << t->tm_mon + 1 << "."
			<< std::setw(2) << t->tm_mday;
		return oss.str();
	}

	// 校验格式：必须包含两个点，仅数字和点
	bool checkDateValid(const std::string& s)
	{
		size_t p1 = s.find('.');
		size_t p2 = s.find('.', p1 + 1);
		if (p1 == std::string::npos || p2 == std::string::npos)
			return false;
		for (char c : s)
		{
			if (c != '.' && !isdigit(c))
				return false;
		}
		return true;
	}

	// 标准化日期：2026.1.3 → 2026.01.03
	std::string formatDate(const std::string& s)
	{
		size_t p1 = s.find('.');
		size_t p2 = s.find('.', p1 + 1);
		int y = stoi(s.substr(0, p1));
		int m = stoi(s.substr(p1 + 1, p2 - p1 - 1));
		int d = stoi(s.substr(p2 + 1));

		std::ostringstream oss;
		oss << std::setfill('0')
			<< y << "."
			<< std::setw(2) << m << "."
			<< std::setw(2) << d;
		return oss.str();
	}

	void setDate(std::string d)
	{
		if (d.empty() || !checkDateValid(d))
			dateStr = getCurrentDate();
		else
			dateStr = formatDate(d);
	}

	std::string getDate() const
	{
		return dateStr;
	}

	// 转数字用于排序 20260105
	long long dateToNum() const
	{
		size_t p1 = dateStr.find('.');
		size_t p2 = dateStr.find('.', p1 + 1);
		int y = stoi(dateStr.substr(0, p1));
		int m = stoi(dateStr.substr(p1 + 1, p2 - p1 - 1));
		int d = stoi(dateStr.substr(p2 + 1));
		return (long long)y * 10000 + m * 100 + d;
	}
};

// bill公有继承Date
class bill : public Date
{
private:
	std::string name;
	int id;
	double amt;
public:
	bill()
	{
		setDate(getCurrentDate());
	}
	bill(std::string name1, int id1, double amt1) : name(name1), id(id1), amt(amt1)
	{
		setDate(getCurrentDate());
	}
	bill(std::string name1, int id1, double amt1, std::string date1) : name(name1), id(id1), amt(amt1)
	{
		setDate(date1);
	}
	~bill(){}
	std::string get_n() const { return name; }
	int get_id() const { return id; }
	double get_a() const { return amt; }
	void set_n(std::string n) { name = n; }
	void set_id(int id1) { id = id1; }
	void set_amt(double a) { amt = a; }
};

class accountbook
{
private:
	std::vector<bill> list1;
	std::string name;
	int realIdSeed;
	double initMoney;

	double getAllBillSum() const
	{
		double sum = 0;
		for (const auto& b : list1)
			sum += b.get_a();
		return sum;
	}

public:
	accountbook() : name("默认账本"), realIdSeed(1), initMoney(0.0) {}
	accountbook(std::string n) : name(n), realIdSeed(1), initMoney(0.0) {}

	double getBalance() const
	{
		return initMoney + getAllBillSum();
	}
	void setInitMoney(double bal)
	{
		initMoney = bal;
	}
	double getInitMoney() const
	{
		return initMoney;
	}

	void add(std::string n, double a, std::string inputDate = "")
	{
		try {
			if (n.empty()) {
				throw std::invalid_argument("账单名称不能为空");
			}
			int realId = realIdSeed++;
			list1.push_back(bill(n, realId, a, inputDate));
			int showIndex = static_cast<int>(list1.size());
			std::cout << "账单添加成功，序号：" << showIndex << std::endl;
		}
		catch (const std::exception& e) {
			std::cout << "添加账单失败：" << e.what() << std::endl;
		}
	}

	// 查看时临时拷贝排序，原始文件存储顺序不变
	void show() const
	{
		try {
			if (list1.empty())
			{
				std::cout << "暂无账单记录\n";
				return;
			}
			std::vector<bill> temp = list1;
			std::sort(temp.begin(), temp.end(), [](const bill& a, const bill& b) {
				long long numA = a.dateToNum();
				long long numB = b.dateToNum();
				if (numA != numB)
					return numA < numB;
				return a.get_id() < b.get_id();
			});

			std::cout << "======" << name << "======\n";
			for (size_t i = 0; i < temp.size(); i++)
			{
				std::cout << "序号:" << i + 1 << " 日期:" << temp[i].getDate() << " 名称:" << temp[i].get_n() << " 金额:" << temp[i].get_a() << std::endl;
			}
			std::cout << "【钱包初始本金】：" << initMoney << std::endl;
			std::cout << "【当前钱包余额】：" << getBalance() << std::endl;
		}
		catch (...) {
			std::cout << "读取账单列表异常，展示失败\n";
		}
	}

	void modifyBill(int idx, int opt, std::string newStr, double newAmt = 0)
	{
		try
		{
			int pos = idx - 1;
			if (pos < 0 || pos >= (int)list1.size())
				throw std::runtime_error("该序号不存在");
			bill& target = list1[pos];
			if (opt == 1)
			{
				target.set_n(newStr);
				std::cout << "账单名称修改完成\n";
			}
			else if (opt == 2)
			{
				target.set_amt(newAmt);
				std::cout << "账单金额修改完成\n";
			}
			else if (opt == 3)
			{
				target.setDate(newStr);
				std::cout << "账单日期修改完成\n";
			}
		}
		catch (const std::exception& e)
		{
			std::cout << "修改失败：" << e.what() << std::endl;
		}
	}

	void deleteByIndex(int showIdx)
	{
		try
		{
			if (showIdx <= 0)
				throw std::invalid_argument("序号必须是正整数");
			size_t targetPos = static_cast<size_t>(showIdx - 1);
			if (targetPos >= list1.size())
				throw std::runtime_error("该序号不存在");
			list1.erase(list1.begin() + targetPos);
			std::cout << "序号" << showIdx << " 删除成功\n";
		}
		catch (const std::exception& e)
		{
			std::cout << "删除失败：" << e.what() << std::endl;
		}
	}

private:
	void deleteid(int delid)
	{
		try {
			if (delid <= 0) {
				throw std::invalid_argument("ID必须为正整数");
			}
			int idx = -1;
			for (size_t i = 0; i < list1.size(); i++)
			{
				if (list1[i].get_id() == delid)
				{
					idx = static_cast<int>(i);
					break;
				}
			}
			if (idx == -1)
			{
				throw std::runtime_error("未找到该底层ID账单");
			}
			list1.erase(list1.begin() + idx);
			std::cout << "底层ID:" << delid << " 删除成功\n";
		}
		catch (const std::exception& e) {
			std::cout << "删除失败：" << e.what() << std::endl;
		}
	}

public:
	void stat() const
	{
		try {
			double income = 0, expend = 0;
			for (const auto& b : list1)
			{
				double money = b.get_a();
				if (money > 0) income += money;
				else expend -= money;
			}
			std::cout << "========收支统计========" << std::endl;
			std::cout << "钱包初始本金：" << initMoney << std::endl;
			std::cout << "总收入：" << income << std::endl;
			std::cout << "总支出：" << expend << std::endl;
			std::cout << "账单净差额：" << income - expend << std::endl;
			std::cout << "当前钱包实际余额：" << getBalance() << std::endl;
		}
		catch (...) {
			std::cout << "统计数据异常\n";
		}
	}

	void saveToFile(std::string filePath = "bill.txt")
	{
		try {
			std::ofstream out(filePath);
			if (!out.is_open())
			{
				throw std::runtime_error("文件无法打开，无写入权限");
			}
			out << initMoney << std::endl;
			for (const auto& b : list1)
			{
				out << b.get_id() << " " << b.getDate() << " " << b.get_n() << " " << b.get_a() << std::endl;
			}
			out.close();
			std::cout << "数据已保存到文件\n";
		}
		catch (const std::exception& e) {
			std::cout << "保存失败：" << e.what() << std::endl;
		}
	}

	void loadFromFile(std::string filePath = "bill.txt")
	{
		try {
			std::ifstream in(filePath);
			if (!in.is_open())
			{
				std::cout << "无本地账单文件\n";
				initMoney = 0;
				return;
			}
			list1.clear();
			in >> initMoney;
			int id;
			std::string date, name;
			double amt;
			while (in >> id >> date >> name >> amt)
			{
				if (id > 0)
				{
					list1.push_back(bill(name, id, amt, date));
					if (id >= realIdSeed)
						realIdSeed = id + 1;
				}
			}
			in.close();
			std::cout << "读取本地账单完成\n";
			std::cout << "读取到钱包初始本金：" << initMoney << std::endl;
		}
		catch (const std::exception& e) {
			std::cout << "读取文件失败：" << e.what() << std::endl;
			initMoney = 0;
		}
	}
};

#endif
