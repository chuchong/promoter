#pragma once
//HtmlNode是Html的语法树对应的节点们,用他能生成一棵html语法树并进行解析
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "Stack.h"
#include "CharString.h"

#include <iostream>
//宏定义的一些变量,用作返回值的信息传递
#define HtmlNodeEnd 0
#define HtmlNodeNew 1
#define HtmlParseNoMoreString 2
#define HtmlNodeNotEnd 3
#define HtmlAttributeEnd 4
#define HtmlPleaseAddString 5
//4 指<> 5 指<.. />
//html 标签类,即是<p width="1" 中width="1"部分

//这个最后没用上...
#define pStack Stack<HtmlElement*>*
class HtmlFilter;
//属性,即是指<>中间部分的信息,不是节点
class HtmlAttribute {
private:
	CharString * name_ = nullptr;//存储名称
	CharString * value_ = nullptr;//存储值
public:
	friend class HtmlFilter;
	int parseDeep(CharString * str, int& index);

	HtmlAttribute *prior_ = nullptr;
	HtmlAttribute *next_ = nullptr;
	bool match(const CharString * name, const CharString * value) {
		return name_->equal(name) && value_->equal(value);
	}

	bool isName(const CharString * name) {
		return name_->equal(name);
	}

	CharString* copyOfValue() {
		return value_;
	}
	~HtmlAttribute() {
		delete name_;
		delete value_;
	}
};
//html 节点类指定, 用以产生语法树

class HtmlNode {
public:
	HtmlNode *children_first = nullptr;
	HtmlNode *children_last = nullptr;
	HtmlNode *next_sibling = nullptr;
	friend class HtmlFilter;
public:
	virtual void deepCopyOfText(CharString * result) = 0;
	virtual void clearSons() {
		while (children_first != nullptr) {
			HtmlNode * next_ = children_first->next_sibling;
			//children_first->clearSons();
			delete children_first;
			children_first = next_;
		}
	}
	virtual ~HtmlNode() {
		clearSons();
	}
};
//文本 非标签部分
class HtmlText : public HtmlNode {
	CharString * text_;
public:
	HtmlText(CharString * t) {
		text_ = new CharString(t);
	}
	~HtmlText() {
		delete text_;
	}
	virtual void deepCopyOfText(CharString * result) override;
};
//用来进行解析的,具有< xxx >形状的,采用策略模式
class HtmlElement :public HtmlNode {
protected:
	HtmlAttribute * head_ = nullptr;//一个双向链表,只需要头指针就够了
	CharString * name_ = nullptr;//存储自身名字
	//CharString * text_ = nullptr;//存储所有文字
								//TODO:如果要生成html对应语法树的话,需要将text部分作为一个单独的Node子类型
	friend class HtmlFilter;
	bool hasLeft = 0;//指示是否已经读取完了左半部分
public:
	virtual bool isMeta() {
		return 0;
	}

	//结束返回位置为最后一个attribute后面的非空字符 
	virtual int parseAttribute(CharString * str, int& index);

	//默认是从前缀< <! <!-- 这些的后一位开始
	//index指向分析开始的位置//返回值分为0 结束, 1 新的节点 2 文本读尽
	virtual int parseDeep(CharString * str, int& index, pStack pstack);

	virtual bool isName(const CharString * name) {
		if (name_ == nullptr)
			return false;
		return name_->equal(name);
	}

	virtual bool haveAttribute(const CharString * name, const CharString * value);

	//返回拥有指定名称的attri的值的复制,若不存在,返回nullptr
	virtual CharString* copyValueOfAttriName(const CharString * name);

	//返回复制自身text_的深度复制引用变量 
	virtual void deepCopyOfText(CharString * result);
	HtmlElement() {
	}

	virtual ~HtmlElement();
public:

	void push_child(HtmlNode * child);
};
////div 中才存有正文部分所以有需要可以专门给他设立一个,而其他部分就可以忽略text了,但为了可能的工作需要,暂不写这个
//class HtmlDiv: public HtmlNode {
//
//public:
//
//};
//

//因为javascript代码中会出现干扰的< > 符号,所以需要单独抽取出
class HtmlScript : public HtmlElement {
public:
	HtmlScript() {
		name_ = new CharString(L"script");
	}
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;
};
//注释部分也需要单独处理
class HtmlComment : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack stack) override;
};
//meta的语法比较特殊
class HtmlMeta : public HtmlElement {
public:
	HtmlMeta() {
		name_ = new CharString(L"meta");
	}
	virtual bool isMeta() override {
		return true;
	}

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;
};
//类似html中link标签 单个出现,不需要结束符号
class HtmlLinkLike : public HtmlElement {
public:
	HtmlLinkLike() {
		name_ = new CharString(L"");//没名字,不影响
	}

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;

};
//网易有毒,这个拿来存放</p>,理解成换行就好
class HtmlP :public HtmlLinkLike{
public:
	HtmlP() {
		CharString f = L"\n";
		this->push_child(new HtmlText(&f));
	}
};
//document 节点存储一个html文件,在解析时其实没太大作用,主要的作用还是假如要用c++生成html文件时,需要加这样一个头结点
//形式上就用它来读取<!DocType html>
class HtmlDocument : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;

	//}
};

//存储<!形式文本
class HtmlDeclaration : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack stack) override;
	//}
};