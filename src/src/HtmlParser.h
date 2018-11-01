//����html,ע����gb2312
//���⼸��ע��� js�л��� < > �� ������Ҫ��jsʱ����
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

#define pStack Stack<HtmlNode*>*

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
					assert(nameIndex != -1);
					name_ = str->subString(index, nameIndex);
					index = nameIndex + 1;
					//��Ϊֵ������""��
					//������� "" '' û��
					if (str->charAt(index) == L'\'') {
						index++;
						while (str->charAt(index) != L'\'') {
							index++;
						}
						value_ = str->subString(nameIndex + 2, index);
						index++;
					}
					else if (str->charAt(index) == L'\"') {
						index++;
						while (str->charAt(index) != L'\"') {
							index++;
						}
						value_ = str->subString(nameIndex + 2, index);
						index++;
					}
					else {
						while (str->charAt(index) != L' ' && str->charAt(index) != L'/' && str->charAt(index) != L'>') {
							index++;
						}
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
protected:
	HtmlAttribute * head_ = nullptr;//һ��˫������,ֻ��Ҫͷָ��͹���
	CharString * name_ = nullptr;//�洢��������
	CharString * text_ = nullptr;//�洢��������
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
			while ( index < str->size() && str->charAt(index) != L' ' && str->charAt(index) != L'/' && str->charAt(index) != L'>')
				index++;
			
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
				//TODO: +1��Щ���������
				if (str->charAt(index + 1) == L'/') {
					index += name_->size() + 3; // </name>
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
		
		if (text_ != nullptr)
			result->concat(text_);
		HtmlNode * node = children_first;
		while (node != nullptr) {
			node->deepCopyOfText(result);
			node = node->next_sibling;
		}
	}
	HtmlNode() {
		//name_ = new CharString(L"");
		text_ = new CharString(L"");
	}

	~HtmlNode() {
		delete name_;
		delete text_;
		while (head_ != nullptr) {
			HtmlAttribute *attri = head_;
			head_ = head_->next_;
			delete attri;
		}
	}
	public:
		HtmlNode *children_first = nullptr;
		HtmlNode *children_last = nullptr;
		HtmlNode *next_sibling = nullptr;
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
class HtmlScript: public HtmlNode {
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
class HtmlComment: public HtmlNode{
public:
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
//meta���﷨�Ƚ�����
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
class HtmlLinkLike: public HtmlNode {
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
//document �ڵ�洢һ��html�ļ�,�ڽ���ʱ��ʵû̫������,��Ҫ�����û��Ǽ���Ҫ��c++����html�ļ�ʱ,��Ҫ������һ��ͷ���
//��ʽ�Ͼ���������ȡ<!DocType html>
class HtmlDocument : public HtmlNode {
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

class HtmlDeclaration : public HtmlNode {
public:
	//virtual int parseDeep(CharString * str, int& index, pStack pstack) override {
	virtual int parseDeep(CharString * str, int& index, pStack stack) override{
		index = str->indexOf(L">",index);
		index++;
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
		if (node->haveAttribute(&attribute_name_id, &attribute_value_endText)) {
			CharString* blank = new CharString(L"");
			node->deepCopyOfText(blank);
			endText_ = new CharString(blank);
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

	//����ʱ�����index��Ϊ<
	HtmlNode* createHtmlNode(CharString * str, int& index) {
		/*����ǰ��� ��Ϊ<meta
						<sciprt
						<!--
						<!
						<
						ǰ�������
		*/
		static const CharString meta(L"<meta");
		static const CharString script(L"<script");
		static const CharString comment(L"<!--");
		static const CharString declaration(L"<!");
		static const CharString element(L"<");
		static const CharString linklike(L"<link");
		static const CharString img(L"<img");

		//�ƶ�index λ��
		static const int meta_len = 5;
		static const int script_len = 7;
		static const int comment_len = 4;
		static const int declaration_len = 2;
		static const int element_len = 1;
		static const int linklike_len = 5;
		static const int img_len = 4;

		if (str->indexOf(meta, index) == index) {
			index += meta_len;
			return new HtmlMeta();
		}
		else if (str->indexOf(script, index) == index) {
			index += script_len;
			return new HtmlScript();
		}
		else if (str->indexOf(linklike, index) == index) {
			index += linklike_len;
			return new HtmlLinkLike();
		}
		else if (str->indexOf(img, index) == index) {
			index += img_len;
			return new HtmlLinkLike();
		}
		else if (str->indexOf(comment, index) == index) {
			index += comment_len;
			return new HtmlComment();
		}
		else if (str->indexOf(declaration, index) == index) {
			index += declaration_len;
			return new HtmlDeclaration();
		}
		else if (str->charAt(index) == L'<') {
			index += element_len;
			return new HtmlNode();
		}
		else {//����������޾ʹ����bug��
			bool haveNewElement = 0;
			assert(haveNewElement);
		}
	}
public:
	void parse(std::wstring url) {

		std::locale loc("chs");//windows��ok

		std::wifstream in;
		in.imbue(loc);
		in.open(url);

		std::string str;
		std::wstring wstr;

		pStack stack = new Stack<HtmlNode*>();
		HtmlNode *doc = new HtmlDocument();
		stack->push(doc);
		HtmlNode * now = stack->top();

		std::wofstream os;
		std::wstring buf = L"../input/log.txt";
		os.imbue(loc);
		os.open(buf);

		CharString* text = new CharString(L"");

		while (getline(in, wstr)) {
			CharString s(wstr);
			text->concat(&s);
		}
		int siz = text->size();
		int index = 0;
		int state;

		do {
			now = stack->top();
			state = now->parseDeep(text, index, stack);
			switch (state) {
			case HtmlNodeEnd:
				extractInfo(stack->top());
				//delete stack->top();
				stack->pop();
				break;
			case HtmlNodeNew:
			{
				HtmlNode *newNode = createHtmlNode(text, index);
				stack->top()->push_child(newNode);
				stack->push(newNode);
			}
				break;
			case HtmlParseNoMoreString:
				os << wstr << std::endl;
				break;
			default:
				break;
			}
		} while (index < siz);
		//һ���ж�ȡ̫��������
		//while (getline(in, wstr)) {
		//	CharString s(wstr);

		//	int index = 0;
		//	int state;
		//	do {
		//		now = stack->top();
		//		state = now->parseDeep(&s, index, stack);
		//		switch (state) {
		//		case HtmlNodeEnd:
		//			extractInfo(stack->top());
		//			//delete stack->top();
		//			stack->pop();
		//			break;
		//		case HtmlNodeNew:
		//		{
		//			HtmlNode *newNode = createHtmlNode(&s, index);
		//			stack->top()->push_child(newNode);
		//			stack->push(newNode);
		//		}
		//			break;
		//		case HtmlParseNoMoreString:
		//			os << wstr << std::endl;
		//			break;
		//		default:
		//			break;
		//		}
		//	} while (state != HtmlParseNoMoreString);
		//}
		return;
	}
	HtmlParser() {

	}
	~HtmlParser() {
		delete endText_;
	}

	void print() {
		if (endText_ == nullptr) {
			std::wcout << "bug!";
			return;
		}
		for (int i = 0; i < endText_->size(); i++) {
			std::locale loc("chs");
			std::wcout.imbue(loc);
			std::wcout << endText_->charAt(i);
		}
	}
};

