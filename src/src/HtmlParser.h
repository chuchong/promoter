//���Խ���html����,�ڽ��������л����һ���﷨��
//ע���Ƕ�ȡgb2312�ĵ�
#pragma once
#include <string>
#include <fstream>

#include <ostream>
#include <locale>
#include <codecvt>
#include <assert.h>
#include "Stack.h"
#include "CharString.h"
#include "HtmlNode.h"
#include <iostream>
#define HtmlNodeEnd 0
#define HtmlNodeNew 1
#define HtmlParseNoMoreString 2
#define HtmlNodeNotEnd 3
#define HtmlAttributeEnd 4
#define HtmlPleaseAddString 5
//4 ָ<> 5 ָ<.. />

#define pStack Stack<HtmlElement*>*


class HtmlParser
{
private:
	CharString * keywords_ = nullptr;
	CharString * title_ = nullptr;
	CharString * source_ = nullptr;
	CharString * description_ = nullptr;
	CharString * author_ = nullptr;
	CharString * published_time_ = nullptr;
	CharString * endText_ = nullptr;

	CharString * doc_name_ = nullptr;
	HtmlNode * doc_head = nullptr;
private:
	//��ȡ��Ϣ
	void extractInfo(HtmlElement * node);
	//��meta����ȡ��Ϣ
	void extractInfoFromMeta(HtmlElement * node);

	//����ʱ�����index��Ϊ<
	HtmlElement* createHtmlNode(CharString * str, int& index);
public:
	//����
	CharString * getText() {
		return endText_;
	}

	void setDoc(CharString * str) {
		doc_name_ = new CharString(str);
	}
	//�����ı�������
	CharString * getDoc() {
		return doc_name_;
	}
	//��������
	void parse(std::wstring url);

	HtmlParser() {
	}

	~HtmlParser();

	//���info�ļ�
	void output();
	void print();

	HtmlNode * getDocNode() {
		return doc_head;
	}
	private:
		//delete�Լ���һ����Ա����
		void deleteProperty(CharString *s) {
			if (s != nullptr)
				delete s;
		}
};

