#include "Devider.h"


//第三个是
void Devider::devide(CharString * str, Searcher * searcher, CharStringLink * link)
{
	int index = 0;
	do {
		while (index < str->size() && !searcher->charIn(str->charAt(index)))
			index++;
		int left = index;
		while (index < str->size() && searcher->charIn(str->charAt(index)))
			index++;
		int right = index;
		do {
			CharString * sub = nullptr;
			if (right > left)
				sub = str->subString(left, right);
			while (sub != nullptr && sub->size() >= 2 && !searcher->haveStr(sub))
				sub->pop_back();
			if (sub != nullptr) {
				if (sub->size() > 1)
					link->add(sub);
				left = left + sub->size();
			}
		} while (left < right);
		index = right;
	} while (index < str->size());
}

Devider::Devider()
{
}


Devider::~Devider()
{
}
