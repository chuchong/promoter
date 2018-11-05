#include "Dict.h"

int HashDict::innercode(wchar_t ch) {
	int hi = ch >> 8;//highbyte;
	int low = ch & 0x00FF;//lowbyte
	int ans = hi * 94 + low;
	return ans;
}

int HashDict::hash(CharString * str) {
	wchar_t c1 = str->charAt(0);
	wchar_t c2 = str->charAt(1);
	int inner1 = innercode(c1);
	int inner2 = innercode(c2);
	return ((inner1) * (inner2)) % mod_;//前面乘积应该没超过int 上限 不用删除
}

HashDict::HashDict(int mod) :mod_(mod) {
	map_ = new CharStringCollection*[mod_];
	for (int i = 0; i < mod_; i++)
		map_[i] = new CharStringLink();
}

void HashDict::addCopyOf(CharString * str) {
	int a = hash(str);
	map_[a]->add(str);
}

HashDict::~HashDict() {
	for (int i = 0; i < mod_; i++)
		delete map_[i];
	delete[] map_;
}
