//ע���ҽ�dict�е�bomͷɾ����
//�ڶ�ȡ�������ļ�ʱ,����Ҫ���bomͷ
//dict���ǰ��ձ����С�����ŵ�
//һ19968 ��40863
#include "Stack.h"
#include "CharString.h"
#include "CharStringLink.h"
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
	CharString str(L"1123333333333333333333333333333333333333");
	CharString str_1(L"13");
	str_1[1] = L'2';
	str_1.concat(L"33");
	str_1.resize(3);
	CharString * s = str.subString(0, 5);



	cout << s->indexOf(str_1);
}

void testFile() {
	//std::wstring str = L"123,abc:����˭��";

	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

	//std::string narrowStr = conv.to_bytes(str);
	//{
	//	std::ofstream ofs("d:\\test.txt");			//�ļ���utf8����
	//	ofs << narrowStr;
	//}

	//std::ifstream ifs(L"d:\\test.txt");
	//while (!ifs.eof())
	//{
	//	string line;
	//	getline(ifs, line);
	//	wstring wb = conv.from_bytes(line);
	//	wcout.imbue(locale("chs"));			//������������ ֻΪ����̨�����ʾ
	//	wcout << wb << endl;
	//}


	locale loc("chs");//windows��ok
	CharStringLink* list = new CharStringLink();

	string str;
	ifstream in;
	in.imbue(loc);
	in.open("../�ʿ�/�ʿ�.dic");
	in >> str;
	wstring wstr;
	wstr = conv.from_bytes(str);
	wcout.imbue(loc);
	wcout << wstr;

	wchar_t first = wstr[1];//0λ��Ӧ�ñ�ʾ�����??
	CharString s(wstr);

	ofstream os;
	wstring buf = L"../�ʿ�/��Ŀ";
	buf += first;
	buf += L".dic";
	os.imbue(loc);
	os.open(buf);
	
	do {
		wstr = conv.from_bytes(str);
		os << str << endl;
		CharString s(wstr);
		//list->add(str);
	} while (in >> str);

	wcout << buf << endl;
	wcout << first;
}
int main() {
	//testStack();
	//testString();
	testFile();

	return 0;
}