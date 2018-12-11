#pragma once
//HtmlNode��Html���﷨����Ӧ�Ľڵ���,����������һ��html�﷨�������н���
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "Stack.h"
#include "CharString.h"

#include <iostream>
//�궨���һЩ����,��������ֵ����Ϣ����
#define HtmlNodeEnd 0
#define HtmlNodeNew 1
#define HtmlParseNoMoreString 2
#define HtmlNodeNotEnd 3
#define HtmlAttributeEnd 4
#define HtmlPleaseAddString 5
//4 ָ<> 5 ָ<.. />
//html ��ǩ��,����<p width="1" ��width="1"����

//������û����...
#define pStack Stack<HtmlElement*>*
class HtmlFilter;
//����,����ָ<>�м䲿�ֵ���Ϣ,���ǽڵ�
class HtmlAttribute {
private:
	CharString * name_ = nullptr;//�洢����
	CharString * value_ = nullptr;//�洢ֵ
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
//html �ڵ���ָ��, ���Բ����﷨��

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
//�ı� �Ǳ�ǩ����
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
//�������н�����,����< xxx >��״��,���ò���ģʽ
class HtmlElement :public HtmlNode {
protected:
	HtmlAttribute * head_ = nullptr;//һ��˫������,ֻ��Ҫͷָ��͹���
	CharString * name_ = nullptr;//�洢��������
	//CharString * text_ = nullptr;//�洢��������
								//TODO:���Ҫ����html��Ӧ�﷨���Ļ�,��Ҫ��text������Ϊһ��������Node������
	friend class HtmlFilter;
	bool hasLeft = 0;//ָʾ�Ƿ��Ѿ���ȡ������벿��
public:
	virtual bool isMeta() {
		return 0;
	}

	//��������λ��Ϊ���һ��attribute����ķǿ��ַ� 
	virtual int parseAttribute(CharString * str, int& index);

	//Ĭ���Ǵ�ǰ׺< <! <!-- ��Щ�ĺ�һλ��ʼ
	//indexָ�������ʼ��λ��//����ֵ��Ϊ0 ����, 1 �µĽڵ� 2 �ı�����
	virtual int parseDeep(CharString * str, int& index, pStack pstack);

	virtual bool isName(const CharString * name) {
		if (name_ == nullptr)
			return false;
		return name_->equal(name);
	}

	virtual bool haveAttribute(const CharString * name, const CharString * value);

	//����ӵ��ָ�����Ƶ�attri��ֵ�ĸ���,��������,����nullptr
	virtual CharString* copyValueOfAttriName(const CharString * name);

	//���ظ�������text_����ȸ������ñ��� 
	virtual void deepCopyOfText(CharString * result);
	HtmlElement() {
	}

	virtual ~HtmlElement();
public:

	void push_child(HtmlNode * child);
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
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;
};
//ע�Ͳ���Ҳ��Ҫ��������
class HtmlComment : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack stack) override;
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

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;
};
//����html��link��ǩ ��������,����Ҫ��������
class HtmlLinkLike : public HtmlElement {
public:
	HtmlLinkLike() {
		name_ = new CharString(L"");//û����,��Ӱ��
	}

	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;

};
//�����ж�,����������</p>,���ɻ��оͺ�
class HtmlP :public HtmlLinkLike{
public:
	HtmlP() {
		CharString f = L"\n";
		this->push_child(new HtmlText(&f));
	}
};
//document �ڵ�洢һ��html�ļ�,�ڽ���ʱ��ʵû̫������,��Ҫ�����û��Ǽ���Ҫ��c++����html�ļ�ʱ,��Ҫ������һ��ͷ���
//��ʽ�Ͼ���������ȡ<!DocType html>
class HtmlDocument : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack pstack) override;

	//}
};

//�洢<!��ʽ�ı�
class HtmlDeclaration : public HtmlElement {
public:
	virtual int parseDeep(CharString * str, int& index, pStack stack) override;
	//}
};