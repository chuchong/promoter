//二字哈希
//使用((innercode of ch1 * innercode of ch2) ) mod N 来实现
//一19968 龟40863
//innercode = (highbyte ) * 94 + (lowbyte)//字符内码
//N 应该为质数
#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include <sstream>
#include "CharString.h"
#include "CharStringLink.h"
//接口类,之后也可以用其他方式表示一个字典
class Dict
{
public:
	virtual void addCopyOf(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
	//virtual void generateINI(std::wstring url) = 0;
	//virtual void getINI(std::wstring url) = 0;
};

//-------以上: 单链表来实现上述的CharStringCollection,查找就直接遍历看一看
//HASH 字典,其中存储了所有词语
//hash 表中每个节点的具体实现,节点上可以存储不止一个单词,这里先写个接口,后续可以改变节点数据结构提高效率
class HashDict: public Dict
{
	int mod_;//被模数,同时也是hash表大小
	CharStringCollection* * map_;//前一个是引用变量,map是一个数组
	//每个宽字符定义对应内码
	int innercode(wchar_t ch);
	//计算字符串的hash 值,用前两个内码的乘积
	int hash(CharString * str);
public:
	HashDict(int mod);
	//添加字符串
	void addCopyOf(CharString *str);
	void remove(CharString *str) {
		map_[hash(str)]->remove(str);
	}
	bool search(CharString *str) {
		return map_[hash(str)]->search(str);
	}
	~HashDict();
	//自定义字典格式并没有加快速度 暂时放在这儿保留...
	//void generateINI(std::wstring url) {
	//	//std::ofstream o;
	//	std::locale loc("chs");
	//	std::wofstream os;
	//	os.imbue(loc);

	//	//o.open(url);
	//	os.open(url);


	//	for (int i = 0; i < mod_; i++) {
	//		if (!map_[i]->isEmpty()) {
	//			os << std::to_wstring(i);
	//			Iterator * iter = map_[i]->begin();
	//			Iterator * end_iter = map_[i]->end();

	//			while (!((*iter) == (*end_iter))) {
	//				os << L" " << (*(*(*iter)));
	//				++(*iter);
	//			}
	//			os << std::endl;
	//			delete iter;
	//			delete end_iter;
	//		}
	//	}
	//}

	//void getINI(std::wstring url) {
	//	std::locale loc("chs");//windows下ok

	//	std::wifstream in;
	//	in.imbue(loc);
	//	in.open(url);

	//	std::wstring wstr;


	//	std::wstringstream ss;

	//	while (getline(in, wstr)) {
	//		ss.clear();
	//		ss.str(wstr);
	//		int num;
	//		ss >> num;
	//		std::wstring value;
	//		while (ss >> value) {
	//			CharString * s = new CharString(value);
	//			map_[num]->add(s);
	//			delete s;
	//		}

	//	}
	//	return;

	//	//while (getline(in, wstr)) {
	//	//	CharString s(wstr);
	//	//	int index = 0;
	//	//	int num_index = s.indexOf(L" ", index);
	//	//	int num = 0;

	//	//	for (int i = index; i < num_index; i++) {
	//	//		if (s.charAt(i) == L',')//烦人的c++ 
	//	//			continue;
	//	//		num = num * 10 + s[i] - L'0';
	//	//	}

	//	//	index = num_index + 1;
	//	//	while(index < s.size()) {
	//	//		int next_index = s.indexOf(L" ", index);
	//	//		if (next_index != -1) {
	//	//			CharString * sub = s.subString(index, next_index);
	//	//			this->map_[num]->add(sub);
	//	//			delete sub;
	//	//			index = next_index;
	//	//		}
	//	//		index ++;
	//	//	} 
	//	//}
	//}
};


