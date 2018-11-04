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
//4 ָ<> 5 ָ<.. />
//html ��ǩ��,����<p width="1" ��width="1"����

#define pStack Stack<HtmlElement*>*


class HtmlAttribute {
private:
	CharString * name_ = nullptr;//�洢����
	CharString * value_ = nullptr;//�洢ֵ
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
					//�п�����bug ��ֱ���������˰�
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
					//��Ϊֵ������""��
					//������� "" '' û��
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
//html �ڵ���ָ��parse ����
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
			//TODO �������ܻ�������
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
	HtmlAttribute * head_ = nullptr;//һ��˫������,ֻ��Ҫͷָ��͹���
	CharString * name_ = nullptr;//�洢��������
	//CharString * text_ = nullptr;//�洢��������
								//TODO:���Ҫ����html��Ӧ�﷨���Ļ�,��Ҫ��text������Ϊһ��������Node������

	bool hasLeft = 0;//ָʾ�Ƿ��Ѿ���ȡ������벿��
public:
	virtual bool isMeta() {
		return 0;
	}

	//��������λ��Ϊ���һ��attribute����ķǿ��ַ� 
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

	//Ĭ���Ǵ�ǰ׺< <! <!-- ��Щ�ĺ�һλ��ʼ
	//indexָ�������ʼ��λ��//����ֵ��Ϊ0 ����, 1 �µĽڵ� 2 �ı�����
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
				//����ֻ����/> �� >��� ,�����÷��������������н����޸�
				while (1) {
					if (index >= str->size())
						return HtmlParseNoMoreString;

					if (str->charAt(index) != L' ') {
						if (str->charAt(index) == L'/') {
							//�Ա�
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
				//TODO: +1��Щ���������
				if (str->charAt(index + 1) == L'/') {

					if (str->indexOf(name_, index) != index + 2) {
						//std::locale loc("chs");
						//std::wcout.imbue(loc);
						//std::wcout << *name_ << std::endl;
						//CharString * a = new CharString(L"");
						//deepCopyOfText(a);
						//std::wcout << (*a);
						return HtmlNodeNew;//<p>���ܻᵥ������,��˶�Ӧ��</p>Ҳ�ᵥ�����ֵ�,��������...
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

	//����ӵ��ָ�����Ƶ�attri��ֵ�ĸ���,��������,����nullptr
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

	//���ظ�������text_����ȸ������ñ��� 
	virtual void deepCopyOfText(CharString * result) {
		//TODO Ϊ�˸�ʽ������֮��


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
////div �вŴ������Ĳ�����������Ҫ����ר�Ÿ�������һ��,���������־Ϳ��Ժ���text��,��Ϊ�˿��ܵĹ�����Ҫ,�ݲ�д���
//class HtmlDiv: public HtmlNode {
//
//public:
//
//};
//

//��Ϊjavascript�����л���ָ��ŵ�< > ����,������Ҫ������ȡ��
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
				//����ֻ����/> �� >��� ,�����÷��������������н����޸�
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
				index = nextIndex + 9;//</script>��9
				return HtmlNodeEnd;
			}
		}
	}
};
//ע�Ͳ���Ҳ��Ҫ��������
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
//meta���﷨�Ƚ�����
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
			//����ֻ����/> �� >��� ,�����÷��������������н����޸�
			while (1) {
				if (index >= str->size())
					return HtmlParseNoMoreString;

				if (str->charAt(index) != L' ') {
					if (str->charAt(index) == L'/') {
						//�Ա�
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
//������Щ����htmllink����Ҫ�������ŵĶ�һ������Ū��
class HtmlLinkLike : public HtmlElement {
public:
	HtmlLinkLike() {
		name_ = new CharString(L"");//û����,��Ӱ��
	}

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
		if (index >= str->size())
			return HtmlParseNoMoreString;

		int result = parseAttribute(str, index);
		if (result == HtmlParseNoMoreString)
			return HtmlParseNoMoreString;
		if (result == HtmlAttributeEnd) {
			//����ֻ����/> �� >��� ,�����÷��������������н����޸�
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
//document �ڵ�洢һ��html�ļ�,�ڽ���ʱ��ʵû̫������,��Ҫ�����û��Ǽ���Ҫ��c++����html�ļ�ʱ,��Ҫ������һ��ͷ���
//��ʽ�Ͼ���������ȡ<!DocType html>
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