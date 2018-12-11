#include "HtmlParser.h"


//
//HtmlParser::HtmlParser()
//{
//}
//
//
//HtmlParser::~HtmlParser()
//{
//}

void HtmlParser::extractInfo(HtmlElement * node) {
	if (dynamic_cast<HtmlMeta*> (node) != nullptr) {
		extractInfoFromMeta(node);
		return;
	}
	static const CharString type_div(L"div");
	//-------------------------------------by attribute id
	static const CharString attribute_name_id(L"id");
	static const CharString attribute_value_endText(L"endText");
	if (node->haveAttribute(&attribute_name_id, &attribute_value_endText)) {
		CharString* blank = new CharString(L"");
		node->deepCopyOfText(blank);

		if (endText_ == nullptr)
			endText_ = new CharString(L"");

		endText_->concat(blank);

		delete blank;
	}
	static const CharString attribute_value_ne_article_source(L"ne_article_source");
	if (node->haveAttribute(&attribute_name_id, &attribute_value_ne_article_source)) {
		CharString* blank = new CharString(L"");
		node->deepCopyOfText(blank);//�ڵڶ���

		if (source_ == nullptr) {
			source_ = new CharString(L"");
			source_->concat(blank);
		}
		delete blank;
	}
	//---------------------------by attribute class
	static const CharString attribute_name_class(L"class");
	static const CharString attribute_value_source(L"post_time_source");
	static const CharString attribute_value_time2(L"ep-time-soure cDGray");
	if (node->haveAttribute(&attribute_name_class, &attribute_value_source)
		|| node->haveAttribute(&attribute_name_class, &attribute_value_time2)) {
		CharString* blank = new CharString(L"");
		if (node->children_first != nullptr)
			node->children_first->deepCopyOfText(blank);//�ڵڶ���

														//if (source_ == nullptr) {
														//	source_ = new CharString(L"");
														//	source_->concat(blank);
														//} // source �ڵڶ���
		if (published_time_ == nullptr) {
			published_time_ = new CharString(L"");
			published_time_->concat(blank);
		}

		while (published_time_->charAt(published_time_->size() - 1) > L'9' ||
			published_time_->charAt(published_time_->size() - 1) < L'0')
			published_time_->pop_back();//��ƨ���ϵ�"��Դ"����

		delete blank;
	}

	static const CharString attribute_value_time3(L"ptime");
	if (node->haveAttribute(&attribute_name_class, &attribute_value_time3)) {
		CharString* blank = new CharString(L"");
		node->deepCopyOfText(blank);
		int i = 0;
		while (i < blank->size() &&
			(blank->charAt(i) > L'9' || blank->charAt(i) < L'0')
			)
			i++;

		CharString * sub = blank->subString(i, blank->size());

		if (published_time_ == nullptr) {
			published_time_ = new CharString(L"");
			published_time_->concat(sub);
		}
		//��ƨ���ϵ�"��Դ"����
		delete sub;
		delete blank;
	}
	//--------------------------by h1
	static const CharString type_title(L"h1");
	if (node->isName(&type_title)) {
		CharString* blank = new CharString(L"");
		node->deepCopyOfText(blank);//�ڵڶ���

		if (title_ == nullptr)
			title_ = new CharString(L"");

		title_->concat(blank);

		delete blank;
	}
	//------------------------by textarea
	static const CharString name_textarea(L"textarea");
	if (node->isName(&name_textarea)) {
		CharString* blank = new CharString(L"");
		node->deepCopyOfText(blank);//�ڵڶ���

		if (endText_ == nullptr)
			endText_ = new CharString(L"");

		endText_->concat(blank);

		delete blank;
	}

}

//��meta����ȡ��Ϣ

void HtmlParser::extractInfoFromMeta(HtmlElement * node) {
	static const CharString prop(L"property");
	static const CharString cont(L"content");

	static const CharString pro_key(L"article:tag");
	static const CharString pro_des(L"og:description");
	static const CharString pro_ath(L"article:author");
	static const CharString pro_ttl(L"og:title");
	static const CharString pro_pub(L"article:published_time");

	if (keywords_ == nullptr && node->haveAttribute(&prop, &pro_key)) {
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
	//if (published_time_ == nullptr && node->haveAttribute(&prop, &pro_pub)) {
	//	published_time_ = new CharString(node->copyValueOfAttriName(&cont));
	//}

}

//����ʱ�����index��Ϊ<

HtmlElement * HtmlParser::createHtmlNode(CharString * str, int & index) {
	/*����ǰ��� ��Ϊ<meta
	<sciprt
	<!--
	<!
	<
	ǰ�������
	*/
	//script �л����< > �赥�����
	static const CharString script(L"<script");
	static const CharString comment(L"<!--");
	static const CharString declaration(L"<!");
	static const CharString element(L"<");

	//����Ϊ����ǩ,����meta�Ƚ�����,�ҿ��ܻ��õ��������Ϣ,�ɵ���������
	const int linklike_num = 6;
	static const CharString meta(L"<meta");
	static const CharString linklike[linklike_num]{ (L"<img"),(L"<source"),(L"<param"),(L"<input"),(L"<hr"),(L"<br") };
	//����Ϊ�����������治�淶ʹ��<p>���µı�ǩ
	static const CharString p(L"<p");
	static const CharString p_(L"</p");

	//�ƶ�index λ��
	static const int script_len = 7;
	static const int comment_len = 4;
	static const int declaration_len = 2;
	static const int element_len = 1;

	static const int meta_len = 5;
	static const int linklike_len[linklike_num] = { 4,7,6,6,3,3 };

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
	else {//����������޾ʹ����bug��
		bool haveNewElement = 0;
		assert(haveNewElement);
	}
}

void HtmlParser::parse(std::wstring url) {
	std::locale loc("chs");//windows��ok

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
			std::wstring next_;
			getline(in, next_);
			s.concat(next_);
		}

		//os << s;

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
	doc_head = doc;
	delete stack;

	return;
}

HtmlParser::~HtmlParser() {
	delete doc_head;

	deleteProperty(doc_name_);
	deleteProperty(keywords_);
	deleteProperty(title_);
	deleteProperty(description_);
	deleteProperty(author_);
	deleteProperty(published_time_);
	deleteProperty(endText_);
}

void HtmlParser::output() {
	std::wstring url = doc_name_->to_wstring() + L".info";
	std::locale loc("chs");
	std::wofstream os;
	os.imbue(loc);
	os.open(url);

	if (title_ != nullptr)
		os << (*title_);
	os << std::endl;

	if (source_ != nullptr)
		os << (*source_);
	else
		os << L"����û������...";
	os << std::endl;

	if (published_time_ != nullptr)
		os << (*published_time_);
	os << std::endl;

	if (endText_ != nullptr)
		os << (*endText_);
	os << std::endl;
}

void HtmlParser::print() {
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
