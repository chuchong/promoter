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
#define HtmlNodeNoMoreString 2
//html ��ǩ��,����<p width="1" ��width="1"����

#define pStack Stack<HtmlNode*>*

class HtmlAttribute {
private:
	CharString * name_ = nullptr;//�洢����
	CharString * value_ = nullptr;//�洢ֵ
	
public:
	int parseDeep(CharString * str, int& index, pStack pstack) {
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
public:
	virtual int parseDeep(CharString * str, int& index, pStack pstack) {//indexָ�������ʼ��λ��//����ֵ��Ϊ0 ����, 1 �µĽڵ� 2 �ı�����

	}
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
	void extractInfo(HtmlNode * node){
		if()
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

		if (str->indexOf(meta, index) == 0) {
			return new HtmlMeta();
		}
		else if (str->indexOf(script, index) == 0) {
			return new HtmlScript();
		}
		else if (str->indexOf(comment, index) == 0) {
			return new HtmlComment();
		}
		else if (str->indexOf(declaration, index) == 0) {
			return new HtmlDeclaration();
		}
		else if (str->charAt(0) == L'<') {
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
				case HtmlNodeNoMoreString:
				default:
					break;
				}
			} while (state != HtmlNodeNoMoreString);
		}
	}
	HtmlParser() {

	}
	~HtmlParser() {

	}
};

