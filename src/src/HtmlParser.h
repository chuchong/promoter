//解析html,注意是gb2312
//另外几个注意点 js中会有 < > 号 所以需要在js时忽略
#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <codecvt>

#include "CharString.h"
//html 标签类,即是<p width="1" 中width="1"部分
class HtmlAttribute {
private:
	CharString * name_ = nullptr;//存储名称
	CharString * value_ = nullptr;//存储值
public:
	void parseDeep(CharString * str, int index = 0);//index指向分析开始的位置
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
//html 节点类指定parse 策略
class HtmlNode {
	HtmlAttribute * head_ = nullptr;//一个双向链表,只需要头指针就够了
	CharString * name_ = nullptr;//存储自身名字
	CharString * text_ = nullptr;//存储所有文字
								//TODO:如果要生成html对应语法树的话,需要将text部分作为一个单独的Node子类型
public:
	virtual void parseDeep(CharString * str, int index = 0);
	virtual bool isName(CharString * name) { return name->equal(name_); }
	virtual bool haveAttribute(CharString * name, CharString * value);
	//返回复制自身text_的深度复制引用变量 
	virtual CharString * deepCopyOfText();
	~HtmlNode() {
		delete name_;
		delete text_;

	}
};
////div 中才存有正文部分所以有需要可以专门给他设立一个,而其他部分就可以忽略text了,但为了可能的工作需要,暂不写这个
//class HtmlDiv: public HtmlNode {
//
//public:
//
//};
//

//因为javascript代码中会出现干扰的< > 符号,所以需要单独抽取出
class HtmlScript: public HtmlNode {

};
//注释部分也需要单独处理
class HtmlComment: public HtmlNode{

};
//meta的语法比较特殊
class HtmlMeta : public HtmlNode {

};

class HtmlParser
{
public:
	void parse(std::wstring url) {
		//std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		std::locale loc("chs");//windows下ok

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

