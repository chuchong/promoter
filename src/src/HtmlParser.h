//����html,ע����gb2312
//���⼸��ע��� js�л��� < > �� ������Ҫ��jsʱ����
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
//4 ָ<> 5 ָ<.. />
//html ��ǩ��,����<p width="1" ��width="1"����

#define pStack Stack<HtmlNode*>*

class HtmlAttribute {
private:
	CharString * name_ = nullptr;//�洢����
	CharString * value_ = nullptr;//�洢ֵ
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
//html �ڵ���ָ��parse ����
class HtmlNode {
	HtmlAttribute * head_ = nullptr;//һ��˫������,ֻ��Ҫͷָ��͹���
	CharString * name_ = nullptr;//�洢��������
	CharString * text_ = nullptr;//�洢��������
								//TODO:���Ҫ����html��Ӧ�﷨���Ļ�,��Ҫ��text������Ϊһ��������Node������
	bool hasLeft = 0;//ָʾ�Ƿ��Ѿ���ȡ������벿��
public:
	virtual int parseAttribute(CharString * str, int& index) {//��������λ��Ϊ���һ��attribute����ķǿ��ַ� 
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

	virtual int parseDeep(CharString * str, int& index, pStack pstack) {//indexָ�������ʼ��λ��//����ֵ��Ϊ0 ����, 1 �µĽڵ� 2 �ı�����
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
//document �ڵ�洢һ��html�ļ�,�ڽ���ʱ��ʵû̫������,��Ҫ�����û��Ǽ���Ҫ��c++����html�ļ�ʱ,��Ҫ������һ��ͷ���
//��ʽ�Ͼ���������ȡ<!DocType html>
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
			//TODO meta�ǽ���Ϣ������attribute��,��һ�ַ�����ȡ����
		}
		else {
			if (node->isName(&type_div) && node->haveAttribute(&attribute_name_id, &attribute_value_endText)) {
				endText_ = new CharString(node->deepCopyOfText());
			}
		}
	}
	HtmlNode* createHtmlNode(CharString * str, int& index){
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
		//�ƶ�index λ��
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

		//����������޾ʹ����bug��
		bool haveNewElement = 0;
		assert(!haveNewElement);
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

