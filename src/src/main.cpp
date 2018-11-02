//ע���ҽ�dict�е�bomͷɾ����
//�ڶ�ȡ�������ļ�ʱ,����Ҫ���bomͷ
//dict���ǰ��ձ����С�����ŵ�
//һ19968 ��40863
#include "Stack.h"
#include "CharString.h"
#include "CharStringLink.h"
#include "HashDict.h"
#include "Searcher.h"
#include "HtmlParser.h"
#include "Devider.h"
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <locale>
#include <codecvt>
#include <io.h>
#include <vector>//ֻ������ȡͬ��Ŀ¼�������ļ�
#include <crtdbg.h>


#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

void EnableMemLeakCheck()
{
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpFlag);
}

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



	cout << s->indexOf(str_1, 0);
}

void testDict() {
	Searcher searcher;
	searcher.parse("../dictionary/�ʿ�.dic");
	CharString s(L"һ��һ��");
	CharString s2(L"��ħ��");
	cout << searcher.haveStr(&s);
	cout << searcher.haveStr(&s2);

}

void testParser() {
	//HtmlParser parser;
	//parser.parse(L"../input/test.html");
	//parser.output();
	Devider d;
	Searcher s;
	s.parse("../dictionary/�ʿ�.dic");

	for (int i = 21; i <= 25; i++) {
		CharStringLink l;
		HtmlParser parser;
		cout << "---------------------------" << i <<"---------------------" << endl;
		CharString a(L"./output/1");
		parser.setDoc(&a);
		parser.parse(L"../input/" + to_wstring(i) + L".html");
		parser.output();
		d.devide(parser.getText(), &s, &l);
		l.output(parser.getDoc());
		cout << "---------------------------" << i << "---------------------" << endl;
	}
	return;
}

void testMemory() {
	//CharString
	while (1) {
		CharString* a = new CharString(L"aaaaaaaaaaaa");
		a->concat(L"aaaa");
		delete a;
	}
}

void testDevider() {
	Devider d;
	CharString test_word(L"�����������Ļ���������ڴ�һ��Ҫ������Ȼ�������������");
	Searcher s;
	s.parse("../dictionary/�ʿ�.dic");
	CharStringLink l;
	HtmlParser p;
	p.parse(L"../input/20.html");
	p.getText();
	d.devide(p.getText(), &s, &l);
	l.print();
	return;
}

void testINI() {
	Searcher s;
	s.parse("../dictionary/�ʿ�.dic");

	return;
}


std::wstring s2ws(const string& s)
{
	setlocale(LC_ALL, "chs");

	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, "C");

	return result;
}
void getFiles(string path, vector<string>& files)
{
	//�ļ����  
	long   hFile = 0;
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

void work() {
	std::vector<string> files;

	string filePath("input/");
	////��ȡ��·���µ������ļ�  
	getFiles(filePath, files);

	Devider d;
	Searcher s;
	s.parse("../dictionary/�ʿ�.dic");

	for (string file : files) {
		std::wstring  wide = s2ws(file);
		CharString str(wide);
		if (str.indexOf(L".html",0) == -1)
			continue;

		int left = str.indexOf(L"\\", 0) + 1;
		int right = str.indexOf(L".html", 0);
		CharStringLink l;
		HtmlParser parser;

		CharString * sub = str.subString(left, right);
		CharString out(L"./output/");
		out.concat(sub);
		delete sub;
		parser.setDoc(&out);
		std::locale loc("chs");
		std::wcout.imbue(loc);

		std::wcout << "---------------------------" << str << "---------------------" << endl;
		parser.parse(wide);
		parser.output();
		d.devide(parser.getText(), &s, &l);
		l.output(parser.getDoc());
		std::wcout << "---------------------------" <<str<< "---------------------" << endl;
	}

}
int main() {
	//_CrtSetBreakAlloc(1134017);
	//_CrtSetBreakAlloc(1134014);
	//testStack();
	//testString();
	//testDict();
	//testParser();
	//testMemory();
	//EnableMemLeakCheck();
	//testDevider();
	work();
	return 0;
}