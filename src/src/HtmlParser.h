//����html,ע����gb2312
//���⼸��ע��� js�л��� < > �� ������Ҫ��jsʱ����
#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <codecvt>

#include "CharString.h"
//html ��ǩ��,����<p width="1" ��width="1"����
class HtmlAttribute {
private:
	CharString * name_ = nullptr;//�洢����
	CharString * value_ = nullptr;//�洢ֵ
public:
	void parseDeep(CharString * str, int index = 0);//indexָ�������ʼ��λ��
	HtmlAttribute *prior_ = nullptr;
	HtmlAttribute *next_ = nullptr;
	bool match(CharString * name, CharString * value) {
		return name->equal(name_) && value->equal(value_);
	}
	~HtmlAttribute() {
		delete name_;
		delete value_;
	}
};
//html �ڵ���ָ��parse ����
class HtmlNode {
	HtmlAttribute * head_ = nullptr;//һ��˫������,ֻ��Ҫͷָ��͹���
	CharString * name_ = nullptr;//�洢��������
	CharString * text_ = nullptr;//�洢��������
								//TODO:���Ҫ����html��Ӧ�﷨���Ļ�,��Ҫ��text������Ϊһ��������Node������
public:
	virtual void parseDeep(CharString * str, int index = 0);
	virtual bool isName(CharString * name) { return name->equal(name_); }
	virtual bool haveAttribute(CharString * name, CharString * value);
	//���ظ�������text_����ȸ������ñ��� 
	virtual CharString * deepCopyOfText();
	~HtmlNode() {
		delete name_;
		delete text_;

	}
};
////div �вŴ������Ĳ�����������Ҫ����ר�Ÿ�������һ��,���������־Ϳ��Ժ���text��,��Ϊ�˿��ܵĹ�����Ҫ,�ݲ�д���
//class HtmlDiv: public HtmlNode {
//
//public:
//
//};
//

//��Ϊjavascript�����л���ָ��ŵ�< > ����,������Ҫ������ȡ��
class HtmlScript: public HtmlNode {

};
//ע�Ͳ���Ҳ��Ҫ��������
class HtmlComment: public HtmlNode{

};
//meta���﷨�Ƚ�����
class HtmlMeta : public HtmlNode {

};

class HtmlParser
{
public:
	void parse(std::wstring url) {
		//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::locale loc("chs");//windows��ok

		std::wifstream in;
		in.imbue(loc);
		in.open(url);

		std::string str;
		std::wstring wstr;
		//wstr = conv.from_bytes(str);
		//std::wcout.imbue(loc);
		//std::wcout << wstr;
		while (getline(in, wstr)) {
			//wstr = conv.from_bytes(str);
			CharString s(wstr);
		}
	}
	HtmlParser() {

	}
	~HtmlParser() {

	}
};

