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

		CharString * sub = nullptr;
		if(right > left)
			 sub = str->subString(left, right);
		while (sub != nullptr && sub->size() >= 2 && !searcher->haveStr(sub))
			sub->pop_back();
		if (sub != nullptr) {
			link->add(sub);
			index = left + sub->size();
		}
		else {
			return;
		}
	} while (index < str->size());
}

Devider::Devider()
{
}


Devider::~Devider()
{
}
