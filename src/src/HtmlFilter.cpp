#include "HtmlFilter.h"
#include "HtmlNode.h"
#include "Queue.h"

bool HtmlFilter::equal(HtmlNode * node1, HtmlNode * node2) {
	if (dynamic_cast<HtmlElement *>(node1) == nullptr)
		return false;
	if (dynamic_cast<HtmlElement *>(node2) == nullptr)
		return false;

	HtmlElement * ele1 = dynamic_cast<HtmlElement *>(node1);
	HtmlElement * ele2 = dynamic_cast<HtmlElement *>(node2);
	
	if (ele1->name_ == nullptr || ele2->name_ == nullptr)
		return false;

	if (!(ele1->name_->equal(ele2->name_)))
		return false;

	HtmlAttribute * attr1 = ele1->head_;
	HtmlAttribute * attr2 = ele2->head_;
	while (attr1 != nullptr) {
		if (attr2 == nullptr || !(attr1->match(attr2->name_, attr2->value_)))
			return false;
		attr1 = attr1->next_;
		attr2 = attr2->next_;
	}

	if (attr2 != nullptr)
		return false;

	return true;
}

 bool HtmlFilter::filter(HtmlNode * node, HtmlNode * filt) {
	if (equal(filt, node)) {
		if (filt->children_first == nullptr) {
			node->deepCopyOfText(value);
			return 1;
		}
		else {
			HtmlNode * son = node->children_first;
			while (son != nullptr) {
				if (!filter(son, filt->children_first))
					son = son->next_sibling;
			}
		}
	}

	return 0;
}

 void HtmlFilter::searchTree(HtmlNode * tree) {
	Queue<HtmlNode *> q;
	q.push(tree);
	while (!q.isEmpty()) {
		HtmlNode * node = q.headValue();
		if (filter(node, list_head))
			break;
		HtmlNode * son = node->children_first;
		while (son != nullptr) {
			q.push(son);
			son = son->next_sibling;
		}
		q.pop();
	}
}

HtmlFilter::HtmlFilter()
{
	this->value = new CharString(L"");
	HtmlElement * editor = new HtmlElement();
	editor->name_ = new CharString(L"span");
	editor->head_ = new HtmlAttribute();
	editor->head_->name_ = new CharString(L"class");
	editor->head_->value_ = new CharString(L"ep-editor");
	list_head = editor;
}


HtmlFilter::~HtmlFilter()
{
	auto head = this->list_head;
	while (head != nullptr) {
		auto copy = head;
		head = head->next_sibling;
		delete copy;
	}
}
