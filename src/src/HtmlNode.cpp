#include "HtmlNode.h"

int HtmlAttribute::parseDeep(CharString * str, int & index) {

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
					while (str->charAt(index) != L' ' &&  str->charAt(index) != L'>') {
						if ((str->charAt(index) == L'/' && index + 1 < str->size() && str->charAt(index + 1) == L'>'))
							break;// />该停下了 w因为 有href=http://www.people.com.cn 这种智障写法
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

void HtmlText::deepCopyOfText(CharString * result) {
	if (text_ != nullptr) {
		//TODO 这样可能会有问题
		int i = 0;
		for (; i < text_->size(); i++) {
			if (text_->charAt(i) != L' ' && text_->charAt(i) != L'\t')
				break;
		}
		if (i >= text_->size())
			return;
		CharString * sub = text_->subString(i, text_->size());
		result->concat(sub);
		delete sub;
	}
	return;
}

//结束返回位置为最后一个attribute后面的非空字符 

int HtmlElement::parseAttribute(CharString * str, int & index) {
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

int HtmlElement::parseDeep(CharString * str, int & index, pStack pstack) {
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

bool HtmlElement::haveAttribute(const CharString * name, const CharString * value) {
	HtmlAttribute * node = head_;
	while (node != nullptr) {
		if (node->match(name, value))
			return true;
		node = node->next_;
	}
	return false;
}

//返回拥有指定名称的attri的值的复制,若不存在,返回nullptr

CharString * HtmlElement::copyValueOfAttriName(const CharString * name) {
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

void HtmlElement::deepCopyOfText(CharString * result) {

	int size = result->size();

	HtmlNode * node = children_first;
	while (node != nullptr) {
		CharString i(L"i");//图片url 没必要留着
		CharString sty(L"style");//div 没必要
		if (!((dynamic_cast<HtmlElement*>(node) != nullptr && dynamic_cast<HtmlElement*>(node)->isName(&i))
			|| (dynamic_cast<HtmlElement*>(node) != nullptr && dynamic_cast<HtmlElement*>(node)->isName(&sty))
			)
			) {
			node->deepCopyOfText(result);
		}
		node = node->next_sibling;
	}
	//CharString s(L"p");
	//if (isName(&s) && result->size() > size)
	//	result->push_back(L'\n');
}

HtmlElement::~HtmlElement() {
	delete name_;
	//delete text_;
	while (head_ != nullptr) {
		HtmlAttribute *attri = head_;
		head_ = head_->next_;
		delete attri;
	}
}

void HtmlElement::push_child(HtmlNode * child) {
	if (children_first == nullptr) {
		children_first = children_last = child;
	}
	else {
		children_last->next_sibling = child;
		children_last = child;
	}
}

int HtmlScript::parseDeep(CharString * str, int & index, pStack pstack) {
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

int HtmlComment::parseDeep(CharString * str, int & index, pStack stack) {
	int next_ = str->indexOf(L"-->", index);
	if (next_ == -1)
		return HtmlParseNoMoreString;
	else {
		index = next_ + 3;
		return HtmlNodeEnd;
	}
}

int HtmlMeta::parseDeep(CharString * str, int & index, pStack pstack) {
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

int HtmlLinkLike::parseDeep(CharString * str, int & index, pStack pstack) {
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

int HtmlDocument::parseDeep(CharString * str, int & index, pStack pstack) {
	do {
		if (index >= str->size())
			return HtmlParseNoMoreString;
		if (str->charAt(index) == L'<')
			return HtmlNodeNew;
		index++;
	} while (1);
}

//virtual int parseDeep(CharString * str, int& index, pStack pstack) override {

int HtmlDeclaration::parseDeep(CharString * str, int & index, pStack stack) {
	index = str->indexOf(L">", index);
	index++;
	return HtmlNodeEnd;
}
