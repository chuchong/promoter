//注意我将dict中的bom头删掉了
//在读取其他人文件时,还需要添加bom头
//dict中是按照编码从小到大排的
//一19968 龟40863
#include "Stack.h"
#include "CharString.h"
#include "CharStringLink.h"
#include "HashDict.h"
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>
using namespace std;
void testStack() {
	Stack<char> c;
	for (int i = 1; i < 100; i ++)
		c.push('a');
	while (!c.empty()) {
		c.top();
		c.pop();
	}
}

void testString() {
	wstring aaa(L"AAA");
	CharString a(aaa);
	CharString str(L"1123333333333333333333333333333333333333");
	CharString str_1(L"13");
	str_1[1] = L'2';
	str_1.concat(L"33");
	str_1.resize(3);
	CharString * s = str.subString(0, 5);



	cout << s->indexOf(str_1);
}

void testDict(HashDict & dic) {
 	CharString* s = new CharString(L"一来二去");
	cout << dic.search(s) << endl;
	CharString* s1 = new CharString(L"红魔乡");
	cout << dic.search(s1) << endl;
	dic.add(s1);
	cout << dic.search(s1) << endl;
}

void testFile() {
	//std::wstring str = L"123,abc:我是谁！";

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

	//std::string narrowStr = conv.to_bytes(str);
	//{
	//	std::ofstream ofs("d:\\test.txt");			//文件是utf8编码
	//	ofs << narrowStr;
	//}

	//std::ifstream ifs(L"d:\\test.txt");
	//while (!ifs.eof())
	//{
	//	string line;
	//	getline(ifs, line);
	//	wstring wb = conv.from_bytes(line);
	//	wcout.imbue(locale("chs"));			//更改区域设置 只为控制台输出显示
	//	wcout << wb << endl;
	//}


	locale loc("chs");//windows下ok
	CharStringLink* list = new CharStringLink();

	string str;
	ifstream in;
	in.imbue(loc);
	in.open("../dictionary/词库.dic");
	in >> str;
	wstring wstr;
	wstr = conv.from_bytes(str);
	wcout.imbue(loc);
	wcout << wstr;

	wchar_t first = wstr[1];//0位置应该表示编码吧??
	CharString s(wstr);

	ofstream os;
	wstring buf = L"../dictionary/词目";
	buf += first;
	buf += L".dic";
	os.imbue(loc);
	os.open(buf);
	HashDict dict(6763);//291143 比较接近字典数目的大小
	do {
		wstr = conv.from_bytes(str);
		//os << str << endl;
		CharString s(wstr);
		dict.add(&s);
		//list->add(str);
	} while (in >> str);

	testDict(dict);

	wcout << buf << endl;
	wcout << first;
}


int main() {
	//testStack();
	testString();
	testFile();


	return 0;
}