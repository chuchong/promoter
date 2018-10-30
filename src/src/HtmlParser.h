//解析html,注意是gb2312
//另外几个注意点 js中会有 < > 号 所以需要在js时忽略
#pragma once
#include <string>
#include <fstream>
#include <ostream>
#include <codecvt>
#include <assert.h>
#include "Stack.h"
#include "CharString.h"
#define HtmlNodeEnd 0
#define HtmlNodeNew 1
#define HtmlParseNoMoreString 2
#define HtmlNodeNotEnd 3
#define HtmlAttributeEnd 4
//4 指<> 5 指<.. />
//html 标签类,即是<p width="1" 中width="1"部分

#define pStack Stack<HtmlNode*>*

class HtmlAttribute {
private:
	CharString * name_ = nullptr;//存储名称
	CharString * value_ = nullptr;//存储值
public:
	int parseDeep(CharString * str, int& index) {
		if (index > str->size())
			return HtmlParseNoMoreString;
	}

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
	bool hasLeft = 0;//指示是否已经读取完了左半部分
public:
	virtual int parseAttribute(CharString * str, int& index) {//结束返回位置为最后一个attribute后面的非空字符 
		HtmlAttribute *p, *q;
		head_ = p = q = new HtmlAttribute();
		int result = head_->parseDeep(str, index);
		if (result == HtmlParseNoMoreString)
			return HtmlParseNoMoreString;
		if (result == HtmlAttributeEnd) {
			delete head_;
			head_ = nullptr;
			return HtmlAttributeEnd;
		}

		do {
			p = new HtmlAttribute();
			result = p->parseDeep(str, index);
			if (result == HtmlParseNoMoreString)
				return HtmlParseNoMoreString;
			if (result == HtmlAttributeEnd) {
				delete p;
				return HtmlAttributeEnd;
			}
			else {
				q->next_ = p;
				p->prior_ = q;
				q = q->next_;
			}
		} while (1);
	}

	virtual int parseDeep(CharString * str, int& index, pStack pstack) {//index指向分析开始的位置//返回值分为0 结束, 1 新的节点 2 文本读尽
		if (index > str->size())
			return HtmlParseNoMoreString;
		if (hasLeft == 0) {
			int first_blank = str->indexOf(L"", index);
			name_ = str->subString(index, first_blank);
			int result = parseAttribute(str, index);
			if (result == HtmlParseNoMoreString)
				return HtmlParseNoMoreString;
			if (result == HtmlAttributeEnd) {

			}
		}
	}

	virtual bool isName(const CharString * name) { return name_->equal(name); }
	virtual bool haveAttribute(const CharString * name,const CharString * value);
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
//document 节点存储一个html文件,在解析时其实没太大作用,主要的作用还是假如要用c++生成html文件时,需要加这样一个头结点
//形式上就用它来读取<!DocType html>
class HtmlDocument : public HtmlNode {
public:
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override{

	}
};

class HtmlDeclaration : public HtmlNode {
public:
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override {

	}
};

class HtmlParser
{
private:
	CharString * endText_ = nullptr;
private:
	void extractInfo(HtmlNode * node){
		static const CharString type_div(L"div");
		static const CharString attribute_name_id (L"id");
		static const CharString attribute_value_endText(L"endText");
		if (dynamic_cast<HtmlMeta *>(node) != nullptr) {
			//TODO meta是将信息藏在了attribute中,找一种方法提取出来
		}
		else {
			if (node->isName(&type_div) && node->haveAttribute(&attribute_name_id, &attribute_value_endText)) {
				endText_ = new CharString(node->deepCopyOfText());
			}
		}
	}
	HtmlNode* createHtmlNode(CharString * str, int& index){
	/*依照前面的 分为<meta 
					<sciprt
					<!--
					<!
					<
					前面的优先
	*/
		static const CharString meta(L"<meta");
		static const CharString script(L"<script");
		static const CharString comment(L"<!--");
		static const CharString declaration(L"<!");
		static const CharString element(L"<");
		//移动index 位置
		static const int meta_len = 5;
		static const int script_len = 7;
		static const int comment_len = 4;
		static const int declaration_len = 2;
		static const int element_len = 1;

		if (str->indexOf(meta, index) == 0) {
			index += meta_len;
			return new HtmlMeta();
		}
		else if (str->indexOf(script, index) == 0) {
			index += script_len;
			return new HtmlScript();
		}
		else if (str->indexOf(comment, index) == 0) {
			index += comment_len;
			return new HtmlComment();
		}
		else if (str->indexOf(declaration, index) == 0) {
			index += declaration_len;
			return new HtmlDeclaration();
		}
		else if (str->charAt(0) == L'<') {
			index += element_len;
			return new HtmlNode();
		}

		//超过这个界限就代表出bug了
		bool haveNewElement = 0;
		assert(!haveNewElement);
	}
public:
	void parse(std::wstring url) {

		std::locale loc("chs");//windows下ok

		std::wifstream in;
		in.imbue(loc);
		in.open(url);

		std::string str;
		std::wstring wstr;

		pStack stack = new Stack<HtmlNode*>();
		stack->push(new HtmlDocument());
		HtmlNode * now = stack->top();

		while (getline(in, wstr)) {
			CharString s(wstr);
			now = stack->top();
			int index = 0;
			int state;
			do {
				state = now->parseDeep(&s, index, stack);
				switch (state) {
				case HtmlNodeEnd:
					extractInfo(stack->top());
					delete stack->top();
					stack->pop();
					break;
				case HtmlNodeNew:
					HtmlNode *newNode = createHtmlNode(&s, index);
					stack->push(newNode);
				case HtmlParseNoMoreString:
				default:
					break;
				}
			} while (state != HtmlParseNoMoreString);
		}
	}
	HtmlParser() {

	}
	~HtmlParser() {
		delete endText_;
	}
};

