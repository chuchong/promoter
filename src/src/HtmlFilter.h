#pragma once
#include "HtmlNode.h"
#include "CharString.h"
class HtmlFilter
{
public:
	HtmlNode * list_head;
	CharString * value;
public:
	bool filter(HtmlNode * node, HtmlNode * filt) {
		if (node->equal(filt)) {
			if (filt->child_first == nullptr) {
				value = node->deepCopyOfValue();
				return 1;
			}
			else {
				HtmlNode * son = node->child_first;
				while (son != nullptr) {
					if (!filter(son, filt->child_first))
						son = son->next_sibling;
				}
			}
		}

		return 0;
	}

	void searchTree(HtmlNode * tree) {
		Queque q;
		q.push(tree);
		while (!q.isEmpty()) {
			HtmlNode * n = q.headValue();
			if (filter(node, list_head))
				break;
			HtmlNode * son = n->child_first;
			while (son != nullptr) {
				q.push(son);
				son = son->next_sibling;
			}
		}
	}
	HtmlFilter();
	~HtmlFilter();
};

