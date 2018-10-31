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
		
		do {
			if (index >= str->size())
				return HtmlParseNoMoreString;
			
			if (str->charAt(index) == L'/' || str->charAt(index) == L'>' || str->charAt(index) == L'-') 
				return HtmlAttributeEnd;

			if (str->charAt(index) != L' ') {
				if (str->charAt(index) == L'/' || str->charAt(index) == L'>' || str->charAt(index) == L'-') {
					return HtmlAttributeEnd;
				}
				else {
					int nameIndex = str->indexOf(L"=", index);
					name_ = str->subString(index, nameIndex);
					index = nameIndex;
					//认为值都是在""中
					int leftIndex = str->indexOf(L"\"", index) + 1;
					index = leftIndex;
					int rightIndex = str->indexOf(L"\"", index);
					index = rightIndex + 1;
					value_ = str->subString(leftIndex, rightIndex);

					return HtmlNodeNotEnd;
				}
			}
			index++;
		} while (1);
	}

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
//html 节点类指定parse 策略
class HtmlNode {
protected:
	HtmlAttribute * head_ = nullptr;//一个双向链表,只需要头指针就够了
	CharString * name_ = nullptr;//存储自身名字
	CharString * text_ = nullptr;//存储所有文字
								//TODO:如果要生成html对应语法树的话,需要将text部分作为一个单独的Node子类型
	bool hasLeft = 0;//指示是否已经读取完了左半部分
public:
	virtual bool isMeta() {
		return 0;
	}

	//结束返回位置为最后一个attribute后面的非空字符 
	virtual int parseAttribute(CharString * str, int& index) {
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

	//默认是从前缀< <! <!-- 这些的后一位开始
	//index指向分析开始的位置//返回值分为0 结束, 1 新的节点 2 文本读尽
	virtual int parseDeep(CharString * str, int& index, pStack pstack) {
		if (index >= str->size())
			return HtmlParseNoMoreString;
		if (hasLeft == 0) {
			int name_left_index = index;
			while (str->charAt(index) != L' ' && str->charAt(index) != L'/' && str->charAt(index) != L'>')
				index++;
			name_ = str->subString(name_left_index, index);

			int result = parseAttribute(str, index);
			if (result == HtmlParseNoMoreString)
				return HtmlParseNoMoreString;
			if (result == HtmlAttributeEnd) {
				//这里只考虑/> 和 >情况 ,特殊用法会在特殊子类中进行修改
				while (1) {
					if (index >= str->size())
						return HtmlParseNoMoreString;

					if (str->charAt(index) != L' ') {
						if (str->charAt(index) == L'/') {
							//自闭
							index += 2;//"/>"
							return HtmlNodeEnd;
						}
						else if (str->charAt(index) == L'>') {
							hasLeft = true;
							index++;
							return HtmlNodeNotEnd;
						}
					}
					index++;
				}
			}
		}
		else {
			int nextIndex = str->indexOf(L"<", index);
			if (nextIndex == -1) {
				if (nextIndex > index) {
					CharString * sub_str = str->subString(index, str->size());
					text_->concat(sub_str);
					delete sub_str;
				}
				return HtmlParseNoMoreString;
			}
			else {
				if (nextIndex > index) {
					CharString * sub_str = str->subString(index, nextIndex);
					text_->concat(sub_str);
					delete sub_str;
				}

				index = nextIndex;
				//TODO: +1有些暴躁就是了
				if (str->charAt(index + 1) == L'/') {
					index += name_->size() + 2;
					return HtmlNodeEnd;
				}
				else
					return HtmlNodeNew;
			}
		}
	}

	virtual bool isName(const CharString * name) {
		if (name_ == nullptr)
			return false;
		return name_->equal(name); }

	virtual bool haveAttribute(const CharString * name, const CharString * value) {
		HtmlAttribute * node = head_;
		while (node != nullptr) {
			if (node->match(name, value))
				return true;
			node = node->next_;
		}
		return false;
	}

	//返回拥有指定名称的attri的值的复制,若不存在,返回nullptr
	virtual CharString* copyValueOfAttriName(const CharString * name) {
		HtmlAttribute * node = head_;
		while (node != nullptr) {
			if (node->isName(name)) {
				return node->copyOfValue();
			}
			node = node->next_;
		}
		return nullptr;
	}

	//返回复制自身text_的深度复制引用变量 
	virtual CharString * deepCopyOfText() {
		return new CharString(text_);
	}
	HtmlNode() {
		//name_ = new CharString(L"");
		text_ = new CharString(L"");
	}

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
	virtual int parseDeep(CharString * str, int& index, pStack stack) override {
		int next = str->indexOf(L"-->", index);
		if(next == -1)
			return HtmlParseNoMoreString;
		else {
			index = next + 3;
			return HtmlNodeEnd;
		}
	}
};
//meta的语法比较特殊
class HtmlMeta : public HtmlNode {
	public:
	HtmlMeta() {
		name_ = new CharString(L"meta");
	}
	virtual bool isMeta() override{
		return true;
	}

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override{
		if (index >= str->size())
			return HtmlParseNoMoreString;
	
		int result = parseAttribute(str, index);
		if (result == HtmlParseNoMoreString)
			return HtmlParseNoMoreString;
		if (result == HtmlAttributeEnd) {
			//这里只考虑/> 和 >情况 ,特殊用法会在特殊子类中进行修改
			while (1) {
				if (index >= str->size())
					return HtmlParseNoMoreString;

				if (str->charAt(index) != L' ') {
					if (str->charAt(index) == L'/') {
						//自闭
						index += 2;//"/>"
						return HtmlNodeEnd;
					}
					else if (str->charAt(index) == L'>') {
						hasLeft = true;
						index++;
						return HtmlNodeEnd;
					}
				}
				index++;
			}
		}
	}
};
//document 节点存储一个html文件,在解析时其实没太大作用,主要的作用还是假如要用c++生成html文件时,需要加这样一个头结点
//形式上就用它来读取<!DocType html>
class HtmlDocument : public HtmlNode {
public:
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
		if (index >= str->size())
			return HtmlParseNoMoreString;
		if (str->charAt(index) == L'<')
			return HtmlNodeNew;
	}

	//}
};

class HtmlDeclaration : public HtmlNode {
public:
	//virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
	virtual int parseDeep(CharString * str, int& index, pStack stack) override{
		index = str->size();
		return HtmlNodeEnd;
	}
	//}
};

class HtmlParser
{
private:
	CharString * keywords_ = nullptr;
	CharString * title_ = nullptr;
	CharString * description_ = nullptr;
	CharString * author_ = nullptr;
	CharString * published_time_ = nullptr;
	CharString * endText_ = nullptr;
private:
	void extractInfo(HtmlNode * node){
		if (dynamic_cast<HtmlMeta*> (node) != nullptr) {
			extractInfoFromMeta(node);
			return;
		}
		static const CharString type_div(L"div");
		static const CharString attribute_name_id (L"id");
		static const CharString attribute_value_endText(L"endText");
		if (dynamic_cast<HtmlMeta *>(node) != nullptr) {
			//TODO meta是将信息藏在了attribute中,找一种方法提取出来
		}
		else {
			if (node->isName(&type_div) && node->haveAttribute(&attribute_name_id, &attribute_value_endText)) {
				endText_ = node->deepCopyOfText();
			}
		}
	}

	void extractInfoFromMeta(HtmlNode * node) {
		static const CharString prop(L"property");
		static const CharString cont(L"content");

		static const CharString pro_key(L"article:tag");
		static const CharString pro_des(L"og:description");
		static const CharString pro_ath(L"article:author");
		static const CharString pro_ttl(L"og:title");
		static const CharString pro_pub(L"article:published_time");

		if (node->haveAttribute(&prop, &pro_key)) {
			keywords_ = new CharString(node->copyValueOfAttriName(&cont));
		}
		if (node->haveAttribute(&prop, &pro_des)) {
			description_ = new CharString(node->copyValueOfAttriName(&cont));
		}
		if (node->haveAttribute(&prop, &pro_ath)) {
			author_ = new CharString(node->copyValueOfAttriName(&cont));
		}
		if (node->haveAttribute(&prop, &pro_ttl)) {
			title_ = new CharString(node->copyValueOfAttriName(&cont));
		}
		if (node->haveAttribute(&prop, &pro_pub)) {
			published_time_ = new CharString(node->copyValueOfAttriName(&cont));
		}

	}

	//输入时务必让index处为<
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

		if (str->indexOf(meta, index) == index) {
			index += meta_len;
			return new HtmlMeta();
		}
		else if (str->indexOf(script, index) == index) {
			index += script_len;
			return new HtmlScript();
		}
		else if (str->indexOf(comment, index) == index) {
			index += comment_len;
			return new HtmlComment();
		}
		else if (str->indexOf(declaration, index) == index) {
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

			int index = 0;
			int state;
			do {
				now = stack->top();
				state = now->parseDeep(&s, index, stack);
				switch (state) {
				case HtmlNodeEnd:
					extractInfo(stack->top());
					delete stack->top();
					stack->pop();
					break;
				case HtmlNodeNew:
				{
					HtmlNode *newNode = createHtmlNode(&s, index);
					stack->push(newNode);
				}
					break;
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

