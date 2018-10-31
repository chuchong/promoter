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
					//��Ϊֵ������""��
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
			while (str->charAt(index) != L' ' && str->charAt(index) != L'/' && str->charAt(index) != L'>')
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
//document �ڵ�洢һ��html�ļ�,�ڽ���ʱ��ʵû̫������,��Ҫ�����û��Ǽ���Ҫ��c++����html�ļ�ʱ,��Ҫ������һ��ͷ���
//��ʽ�Ͼ���������ȡ<!DocType html>
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
			//TODO meta�ǽ���Ϣ������attribute��,��һ�ַ�����ȡ����
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

	//����ʱ�����index��Ϊ<
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

