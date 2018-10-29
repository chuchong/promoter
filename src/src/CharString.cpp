#include "CharString.h"
#include <cstring>


CharString::CharString(const std::wstring & str)
{
	int len = str.size();
	size_ = len;
	maxSize_ = len;//假如concat操作频繁,可能要给maxSize扩容
	charData_ = new wchar_t[maxSize_];
	int i = 0;
	while (str[i] != '\0') {
		charData_[i] = str[i];
		i++;
	}
}

CharString::CharString(const wchar_t str[])
{
	CharString((std::wstring)(str));
}


CharString::~CharString()
{
	if(charData_ != NULL)
		delete charData_;
}

int CharString::indexOf(const CharString & son_str)
{
	
	//kmp算法
	int len = son_str.size();
	if (len == 0) {
		return 0;
	}
	
	int* kmp_next = new int[len];
	//to calculate kmp
	kmp_next[0] = -1;//由于这里数组从零开始,所以0的位置指向-1
	int i_of_son = 0, i_of_next = -1;//i 指向sonstring j指向数组

	while (i_of_son < len - 1) {
		if (i_of_next == -1 || son_str[i_of_son] == son_str[i_of_next]) {
			i_of_son++;
			i_of_next++;
			if (son_str[i_of_son] != son_str[i_of_next])
				kmp_next[i_of_son] = i_of_next;
			else
				kmp_next[i_of_son] = kmp_next[i_of_next];
		}
		else
			i_of_next = kmp_next[i_of_next];
	}

	//to find index
	int start_son = 0;//指向子串
	int start_father = 0;//指向父串

	int j_of_son = start_son;
	int j_of_father = start_father;

	while (j_of_father < size_) {

		if (j_of_father == size_)
			break;

		if (son_str[j_of_son] == charData_[j_of_father]) {
			j_of_father++;
			j_of_son++;//两位置字符一样,则都加1,结束时,j_of_son会到son_len的位置
		}
		else {
			if (kmp_next[j_of_son] != -1) {
				j_of_son = kmp_next[j_of_son];
				continue;
			}
			j_of_father++;
		}

		if (j_of_son == len)
			return(j_of_father - len);
	}


	delete kmp_next;
	return UNMATCH;//-1 表示没有匹配
}

CharString* CharString::subString(int begin, int end)
{
	if (begin >= end || begin < 0 || end > size_)
		return NULL;
	CharString* new_str = new CharString();
	int new_size = end - begin;
	new_str->resize(new_size);
	for (int i = 0; i < new_size; i++)
		(*new_str)[i] = charData_[i + begin];
	return new_str;
}

void CharString::concat(const CharString & str)
{
	int len = str.size();
	int old_size = size_;//copy
	size_ += len;

	if (len + size_ > maxSize_) {
		resize(len + size_);
	}

	for (int i = 0; i < len; i++) {
		charData_[i + old_size] = str[i];
	}
}

void CharString::operator=(const CharString & str)
{
	size_ = str.size();
	maxSize_ = size_;

	if (charData_ != NULL)
		delete charData_;

	charData_ = new wchar_t[maxSize_];
	for (int i = 0; i++; i < size_)
		charData_[i] = str[i];
}

void CharString::operator=(const std::string & str)
{
	size_ = str.size();
	maxSize_ = size_;

	if (charData_ != NULL)
		delete charData_;

	charData_ = new wchar_t[maxSize_];
	for (int i = 0; i++; i < size_)
		charData_[i] = str[i];
}

void CharString::resize(int maxSize)
{
	maxSize_ = maxSize;
	wchar_t *new_Data = new wchar_t[maxSize_];
	//若改变新的最大大小反而更小了,就截断
	if (maxSize_ < size_)
		size_ = maxSize_;

	for (int i = 0; i < size_; i++)
		new_Data[i] = charData_[i];

	if(charData_ != NULL)
		delete charData_;
	charData_ = new_Data;
}

const wchar_t & CharString::operator[](int index) const
{
	return charData_[index];
}
wchar_t & CharString::operator[](int index)
{
	return charData_[index];
}

const wchar_t & CharString::charAt(int index) const
{
	return charData_[index];// TODO: 在此处插入 return 语句
}

wchar_t & CharString::charAt(int index)
{
	return charData_[index];// TODO: 在此处插入 return 语句
}
