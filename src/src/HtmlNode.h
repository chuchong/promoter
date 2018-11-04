#pragma once
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "Stack.h"
#include "CharString.h"
#include <iostream>
#define HtmlNodeEnd 0
#define HtmlNodeNew 1
#define HtmlParseNoMoreString 2
#define HtmlNodeNotEnd 3
#define HtmlAttributeEnd 4
#define HtmlPleaseAddString 5
//4 指<> 5 指<.. />
//html 标签类,即是<p width="1" 中width="1"部分

#define pStack Stack<HtmlElement*>*


class HtmlAttribute {
private:
	CharString * name_ = nullptr;//存储名称
	CharString * value_ = nullptr;//存储值
public:

	int parseDeep(CharString * str, int& index) {

		do {
			if (index >= str->size())
				return HtmlParseNoMoreString;

			if (str->charAt(index) != L' ') {
				if (str->charAt(index) == L'/' || str->charAt(index) == L'>' || str->charAt(index) == L'-') {
					return HtmlAttributeEnd;
				}
				else {
					int nameIndex = str->indexOf(L"=", index);
					//有可能有bug 就直接跳过完了吧
					if (nameIndex == -1) {
						if (str->indexOf(L"/", index) != -1)
							nameIndex = str->indexOf(L"/", index);
						else
							nameIndex = str->indexOf(L">", index);

						index = nameIndex;
						return HtmlAttributeEnd;
					}
					if (name_ == nullptr)
						name_ = str->subString(index, nameIndex);
					index = nameIndex + 1;
					//认为值都是在""中
					//三种情况 "" '' 没有
					if (str->charAt(index) == L'\'') {
						index++;
						while (str->charAt(index) != L'\'') {
							index++;
						}
						if (value_ == nullptr)
							value_ = str->subString(nameIndex + 2, index);
						index++;
					}
					else if (str->charAt(index) == L'\"') {
						index++;
						while (str->charAt(index) != L'\"') {
							index++;
						}
						if (value_ == nullptr)
							value_ = str->subString(nameIndex + 2, index);
						index++;
					}
					else {
						while (str->charAt(index) != L' ' && str->charAt(index) != L'/' && str->charAt(index) != L'>') {
							index++;
						}
						if (value_ == nullptr)
							value_ = str->subString(nameIndex + 1, index);
					}
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
public:
	HtmlNode *children_first = nullptr;
	HtmlNode *children_last = nullptr;
	HtmlNode *next_sibling = nullptr;
public:
	virtual void deepCopyOfText(CharString * result) = 0;
	virtual void clearSons() {
		while (children_first != nullptr) {
			HtmlNode * next = children_first->next_sibling;
			//children_first->clearSons();
			delete children_first;
			children_first = next;
		}
	}
	virtual ~HtmlNode() {
		clearSons();
	}
};

class HtmlText : public HtmlNode {
	CharString * text_;
public:
	HtmlText(CharString * t) {
		text_ = new CharString(t);
	}
	~HtmlText() {
		delete text_;
	}
	virtual void deepCopyOfText(CharString * result) override {
		if (text_ != nullptr) {
			//TODO 这样可能会有问题
			int i = 0;
			for (; i < text_->size(); i++) {
				if (text_->charAt(i) != L' ')
					break;
			}
			if (i == text_->size())
				return;
			result->concat(text_);
		}
		return;
	}
};

class HtmlElement :public HtmlNode {
protected:
	HtmlAttribute * head_ = nullptr;//一个双向链表,只需要头指针就够了
	CharString * name_ = nullptr;//存储自身名字
	//CharString * text_ = nullptr;//存储所有文字
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
			while (index < str->size() && str->charAt(index) != L' ' && str->charAt(index) != L'/' && str->charAt(index) != L'>')
				index++;

			if (name_ == nullptr)
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
					HtmlNode * text = new HtmlText(sub_str);
					push_child(text);
					delete sub_str;
				}
				return HtmlParseNoMoreString;
			}
			else {
				if (nextIndex > index) {
					CharString * sub_str = str->subString(index, nextIndex);
					HtmlNode * text = new HtmlText(sub_str);
					push_child(text);
					delete sub_str;
				}

				index = nextIndex;
				//TODO: +1有些暴躁就是了
				if (str->charAt(index + 1) == L'/') {

					if (str->indexOf(name_, index) != index + 2) {
						//std::locale loc("chs");
						//std::wcout.imbue(loc);
						//std::wcout << *name_ << std::endl;
						//CharString * a = new CharString(L"");
						//deepCopyOfText(a);
						//std::wcout << (*a);
						return HtmlNodeNew;//<p>可能会单独出现,与此对应的</p>也会单独出现的,我我我我...
					}
					index = index + 2;
					index += (name_->size() + 1); // </name>
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
		return name_->equal(name);
	}

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
	virtual void deepCopyOfText(CharString * result) {
		//TODO 为了格式的无奈之举


		int size = result->size();

		HtmlNode * node = children_first;
		while (node != nullptr) {
			CharString i(L"i");
			CharString sty(L"style");
			if (!((dynamic_cast<HtmlElement*>(node) != nullptr && dynamic_cast<HtmlElement*>(node)->isName(&i))
				||(dynamic_cast<HtmlElement*>(node) != nullptr && dynamic_cast<HtmlElement*>(node)->isName(&sty))
				)
				) {
				node->deepCopyOfText(result);
			}
			node = node->next_sibling;
		}
		CharString s(L"p");
		if (isName(&s) && result->size() > size)
			result->push_back(L'\n');
	}
	HtmlElement() {
		//name_ = new CharString(L"");
		//text_ = new CharString(L"");
	}

	virtual ~HtmlElement() {
		delete name_;
		//delete text_;
		while (head_ != nullptr) {
			HtmlAttribute *attri = head_;
			head_ = head_->next_;
			delete attri;
		}
	}
public:

	void push_child(HtmlNode * child) {
		if (children_first == nullptr) {
			children_first = children_last = child;
		}
		else {
			children_last->next_sibling = child;
			children_last = child;
		}
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
class HtmlScript : public HtmlElement {
public:
	HtmlScript() {
		name_ = new CharString(L"script");
	}
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
		if (index >= str->size())
			return HtmlParseNoMoreString;

		if (hasLeft == 0) {
			int result = parseAttribute(str, index);
			if (result == HtmlParseNoMoreString)
				return HtmlParseNoMoreString;

			if (result == HtmlAttributeEnd) {
				//这里只考虑/> 和 >情况 ,特殊用法会在特殊子类中进行修改
				while (1) {
					if (index >= str->size())
						return HtmlParseNoMoreString;

					if (str->charAt(index) == L'>') {
						hasLeft = true;
						index++;
						return HtmlNodeNotEnd;
					}
					index++;
				}
			}
		}
		else {
			int nextIndex = str->indexOf(L"</script>", index);
			if (nextIndex == -1)
				return HtmlParseNoMoreString;
			else {
				index = nextIndex + 9;//</script>长9
				return HtmlNodeEnd;
			}
		}
	}
};
//注释部分也需要单独处理
class HtmlComment : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack stack) override {
		int next = str->indexOf(L"-->", index);
		if (next == -1)
			return HtmlParseNoMoreString;
		else {
			index = next + 3;
			return HtmlNodeEnd;
		}
	}
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

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
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
//还有哪些类似htmllink不需要结束符号的都一起拿来弄了
class HtmlLinkLike : public HtmlElement {
public:
	HtmlLinkLike() {
		name_ = new CharString(L"");//没名字,不影响
	}

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
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
					if (str->charAt(index) == L'>') {
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
class HtmlP :public HtmlLinkLike{
public:
	HtmlP() {
		CharString f = L"\n";
		this->children_first = new HtmlText(&f);
	}
};
//document 节点存储一个html文件,在解析时其实没太大作用,主要的作用还是假如要用c++生成html文件时,需要加这样一个头结点
//形式上就用它来读取<!DocType html>
class HtmlDocument : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
		do {
			if (index >= str->size())
				return HtmlParseNoMoreString;
			if (str->charAt(index) == L'<')
				return HtmlNodeNew;
			index++;
		} while (1);
	}

	//}
};

class HtmlDeclaration : public HtmlElement {
public:
	//virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
	virtual int parseDeep(CharString * str, int& index, pStack stack) override {
		index = str->indexOf(L">", index);
		index++;
		return HtmlNodeEnd;
	}
	//}
};