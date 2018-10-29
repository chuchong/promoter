#include "CharString.h"
//二字哈希
//使用((innercode of ch1 * innercode of ch2) / 10) mod N 来实现
//一19968 龟40863
//innercode = (highbyte - 176) * 94 + (lowbyte) * 161//翻译成16进制
//N 最好是质数
#pragma once
class HashDict
{
public:
	HashDict();
	~HashDict();
};

