//解析html,注意是gb2312
//另外几个注意点 js中会有 < > 号 所以需要在js时忽略
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
//4 指<> 5 指<.. />
//html 标签类,即是<p width="1" 中width="1"部分

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
private:
	void extractInfo(HtmlElement * node){
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

			if (endText_ == nullptr)
				endText_ = new CharString(L"");
			
			endText_->concat(blank);

			delete blank;
		}
		static const CharString attribute_name_class(L"class");
		static const CharString attribute_value_source(L"post_time_source");
		if (node->haveAttribute(&attribute_name_class, &attribute_value_source)) {
			CharString* blank = new CharString(L"");
			node->children_first->next_sibling->deepCopyOfText(blank);//在第二个

			if (source_ == nullptr)
				source_ = new CharString(L"");

			source_->concat(blank);

			delete blank;
		}
		static const CharString type_title(L"h1");
		if (node->isName(&type_title)) {
			CharString* blank = new CharString(L"");
			node->deepCopyOfText(blank);//在第二个

			if (title_ == nullptr)
				title_ = new CharString(L"");

			title_->concat(blank);

			delete blank;
		}
	}

	void extractInfoFromMeta(HtmlElement * node) {
		static const CharString prop(L"property");
		static const CharString cont(L"content");

		static const CharString pro_key(L"article:tag");
		static const CharString pro_des(L"og:description");
		static const CharString pro_ath(L"article:author");
		static const CharString pro_ttl(L"og:title");
		static const CharString pro_pub(L"article:published_time");

		if (keywords_ == nullptr && node->haveAttribute(&prop, &pro_key) ) {
			keywords_ = new CharString(node->copyValueOfAttriName(&cont));
		}
		if (description_ == nullptr && node->haveAttribute(&prop, &pro_des)) {
			description_ = new CharString(node->copyValueOfAttriName(&cont));
		}
		if (author_ == nullptr  && node->haveAttribute(&prop, &pro_ath)) {
			author_ = new CharString(node->copyValueOfAttriName(&cont));
		}
		//if (title_ == nullptr && node->haveAttribute(&prop, &pro_ttl)) {
		//	title_ = new CharString(node->copyValueOfAttriName(&cont));
		//}
		if (published_time_ == nullptr && node->haveAttribute(&prop, &pro_pub)) {
			published_time_ = new CharString(node->copyValueOfAttriName(&cont));
		}

	}

	//输入时务必让index处为<
	HtmlElement* createHtmlNode(CharString * str, int& index) {
		/*依照前面的 分为<meta
						<sciprt
						<!--
						<!
						<
						前面的优先
		*/
		//script 中会出现< > 需单独检查
		static const CharString script(L"<script");
		static const CharString comment(L"<!--");
		static const CharString declaration(L"<!");
		static const CharString element(L"<");

		//以下为单标签,其中meta比较特殊,我可能会用到里面的信息,股单独处理下
		const int linklike_num = 6;
		static const CharString meta(L"<meta");
		static const CharString linklike[linklike_num]{ (L"<img"),(L"<source"),(L"<param"),(L"<input"),(L"<hr"),(L"<br") };
		//以下为网易新闻里面不规范使用<p>导致的标签
		static const CharString p(L"<p");
		static const CharString p_(L"</p");

		//移动index 位置
		static const int script_len = 7;
		static const int comment_len = 4;
		static const int declaration_len = 2;
		static const int element_len = 1;

		static const int meta_len = 5;
		static const int linklike_len[linklike_num] = {4,7,6,6,3,3};

		static const int p_len = 2;
		static const int p__len = 3;

		for (int i = 0; i < linklike_num; i++) {
			if (str->indexOf(linklike[i], index) == index) {
				index += linklike_len[i];
				return new HtmlLinkLike();
			}
		}

		if (str->indexOf(meta, index) == index) {
			index += meta_len;
			return new HtmlMeta();
		}
		else if (str->indexOf(script, index) == index) {
			index += script_len;
			return new HtmlScript();
		}
		else if (str->indexOf(p, index) == index) {
			index += p_len;
			return new HtmlLinkLike();
		}
		else if (str->indexOf(p_, index) == index) {
			index += p__len;
			return new HtmlP();
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
			return new HtmlElement();
		}
		else {//超过这个界限就代表出bug了
			bool haveNewElement = 0;
			assert(haveNewElement);
		}
	}
public:
	CharString * getText() {
		return endText_;
	}

	void setDoc(CharString * str) {
		doc_name_ = new CharString(str);
	}

	CharString * getDoc() {
		return doc_name_;
	}
	void parse(std::wstring url) {
		std::locale loc("chs");//windows下ok

		std::wifstream in;
		in.imbue(loc);
		in.open(url);

		std::string str;
		std::wstring wstr;

		pStack stack = new Stack<HtmlElement*>();
		HtmlElement *doc = new HtmlDocument();
		stack->push(doc);
		HtmlElement * now = stack->top();

		std::wofstream os;
		std::wstring buf = L"../input/log.txt";
		os.imbue(loc);
		os.open(buf);

		CharString* text = new CharString(L"");


		while (getline(in, wstr)) {
			int index = 0;
			CharString s(wstr);
			while (s.charAt(s.size() - 1) != L'>') {
				std::wstring next;
				getline(in, next);
				s.concat(next);
			}

			os << s;

			int state;
			do {
				now = stack->top();
				state = now->parseDeep(&s, index, stack);
				switch (state) {
				case HtmlNodeEnd:
					extractInfo(stack->top());
					//delete stack->top();
					stack->pop();
					break;
				case HtmlNodeNew:
				{
					HtmlElement *newNode = createHtmlNode(&s, index);
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
			} while (state != HtmlParseNoMoreString);
		}

		stack->pop();
		//assert(stack->empty());
		delete doc;
		delete stack;

		return;
	}
	HtmlParser() {

	}
	~HtmlParser() {
		deleteProperty(doc_name_);
		deleteProperty(keywords_);
		deleteProperty(title_);
		deleteProperty(description_);
		deleteProperty(author_);
		deleteProperty(published_time_);
		deleteProperty(endText_);
	}

	void output() {
		std::wstring url = doc_name_->to_wstring() + L".info";
		std::locale loc("chs");
		std::wofstream os;
		os.imbue(loc);
		os.open(url);

		if(title_ != nullptr)
			os << (*title_) ;
		os << std::endl;
		if (source_ != nullptr)
			os << (*source_);
		os << std::endl;
		if (published_time_ != nullptr)
			os << (*published_time_);
		os << std::endl;
		if (endText_ != nullptr)
			os << (*endText_);
		os << std::endl;
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

	private:
		void deleteProperty(CharString *s) {
			if (s != nullptr)
				delete s;
		}
};

