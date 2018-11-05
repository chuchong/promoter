//���ֹ�ϣ
//ʹ��((innercode of ch1 * innercode of ch2) ) mod N ��ʵ��
//һ19968 ��40863
//innercode = (highbyte ) * 94 + (lowbyte)//�ַ�����
//N Ӧ��Ϊ����
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
//�ӿ���,֮��Ҳ������������ʽ��ʾһ���ֵ�
class Dict
{
public:
	virtual void addCopyOf(CharString *str) = 0;
	virtual void remove(CharString *str) = 0;
	virtual bool search(CharString *str) = 0;
	//virtual void generateINI(std::wstring url) = 0;
	//virtual void getINI(std::wstring url) = 0;
};

//-------����: ��������ʵ��������CharStringCollection,���Ҿ�ֱ�ӱ�����һ��
//HASH �ֵ�,���д洢�����д���
//hash ����ÿ���ڵ�ľ���ʵ��,�ڵ��Ͽ��Դ洢��ֹһ������,������д���ӿ�,�������Ըı�ڵ����ݽṹ���Ч��
class HashDict: public Dict
{
	int mod_;//��ģ��,ͬʱҲ��hash���С
	CharStringCollection* * map_;//ǰһ�������ñ���,map��һ������
	//ÿ�����ַ������Ӧ����
	int innercode(wchar_t ch);
	//�����ַ�����hash ֵ,��ǰ��������ĳ˻�
	int hash(CharString * str);
public:
	HashDict(int mod);
	//����ַ���
	void addCopyOf(CharString *str);
	void remove(CharString *str) {
		map_[hash(str)]->remove(str);
	}
	bool search(CharString *str) {
		return map_[hash(str)]->search(str);
	}
	~HashDict();
	//�Զ����ֵ��ʽ��û�мӿ��ٶ� ��ʱ�����������...
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
	//	std::locale loc("chs");//windows��ok

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
	//	//		if (s.charAt(i) == L',')//���˵�c++ 
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


