//用以解析html的类,在解析过程中会产生一棵语法树
//注意是读取gb2312文档
#pragma once
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "Stack.h"
#include "CharString.h"
#include "HtmlNode.h"
#include <iostream>
#define HtmlNodeEnd 0
#define HtmlNodeNew 1
#define HtmlParseNoMoreString 2
#define HtmlNodeNotEnd 3
#define HtmlAttributeEnd 4
#define HtmlPleaseAddString 5
//4 指<> 5 指<.. />

#define pStack Stack<HtmlElement*>*


class HtmlParser
{
private:
	CharString * keywords_ = nullptr;
	CharString * title_ = nullptr;
	CharString * source_ = nullptr;
	CharString * description_ = nullptr;
	CharString * author_ = nullptr;
	CharString * published_time_ = nullptr;
	CharString * endText_ = nullptr;

	CharString * doc_name_ = nullptr;
	HtmlNode * doc_head = nullptr;
private:
	//提取信息
	void extractInfo(HtmlElement * node);
	//从meta中提取信息
	void extractInfoFromMeta(HtmlElement * node);

	//输入时务必让index处为<
	HtmlElement* createHtmlNode(CharString * str, int& index);
public:
	//正文
	CharString * getText() {
		return endText_;
	}

	void setDoc(CharString * str) {
		doc_name_ = new CharString(str);
	}
	//解析文本的名称
	CharString * getDoc() {
		return doc_name_;
	}
	//解析正文
	void parse(std::wstring url);

	HtmlParser() {
	}

	~HtmlParser();

	//输出info文件
	void output();
	void print();

	HtmlNode * getDocNode() {
		return doc_head;
	}
	private:
		//delete自己的一个成员变量
		void deleteProperty(CharString *s) {
			if (s != nullptr)
				delete s;
		}
};

