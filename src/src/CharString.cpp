#include "CharString.h"
#include <cstring>


CharString::CharString(const std::wstring & str)
{
	int len = str.size();
	size_ = len;
	maxSize_ = len;//����concat����Ƶ��,����Ҫ��maxSize����
	charData_ = new wchar_t[maxSize_];
	int i = 0;
	for (int i = 0; i < len; i++) {
		charData_[i] = str[i];
	}
}

CharString::CharString(const wchar_t str[]):CharString((std::wstring)(str))
{
}

CharString::CharString(CharString * p_str)
{
	maxSize_ = p_str->maxSize_;
	size_ = p_str->size_;
	charData_ = new wchar_t[maxSize_];
	for (int i = 0; i < size_; i++) {
		charData_[i] = p_str->charAt(i);
	}
}


CharString::~CharString()
{
	if(charData_ != NULL)
		delete[] charData_;
}

int CharString::indexOf(const CharString & son_str, int start)
{
	
	//kmp�㷨
	int len = son_str.size();
	if (len == 0) {
		return 0;
	}
	
	int* kmp_next = new int[len];
	//to calculate kmp
	kmp_next[0] = -1;//��������������㿪ʼ,����0��λ��ָ��-1
	int i_of_son = 0, i_of_next = -1;//i ָ��sonstring jָ������

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
	int start_son = 0;//ָ���Ӵ�
	int start_father = start;//ָ�򸸴�

	int j_of_son = start_son;
	int j_of_father = start_father;

	while (j_of_father < size_) {

		if (j_of_father == size_)
			break;

		if (son_str[j_of_son] == charData_[j_of_father]) {
			j_of_father++;
			j_of_son++;//��λ���ַ�һ��,�򶼼�1,����ʱ,j_of_son�ᵽson_len��λ��
		}
		else {
			if (kmp_next[j_of_son] != -1) {
				j_of_son = kmp_next[j_of_son];
				continue;
			}
			j_of_father++;
		}

		if (j_of_son == len) {
			delete[] kmp_next;//ԭ����������ڴ�bug!!!!!
			return(j_of_father - len);
		}
	}


	delete[] kmp_next;
	return UNMATCH;//-1 ��ʾû��ƥ��
}

CharString* CharString::subString(int begin, int end)
{
	if (begin >= end || begin < 0 || end > size_)
		return NULL;
	CharString* new_str = new CharString(L"");
	int new_size = end - begin;
	new_str->resize(new_size);
	for (int i = 0; i < new_size; i++)
		new_str->push_back(this->charAt(i + begin));
	return new_str;
}

void CharString::concat(const CharString & str)
{
	int len = str.size();
	int old_size = size_;//copy
	
	int new_size = size_ + len;
	if (new_size > maxSize_) {
		resize(new_size);
	}

	size_ = new_size;
	for (int i = 0; i < len; i++) {
		charData_[i + old_size] = str.charAt(i);
	}
}

void CharString::operator=(const CharString & str)
{
	size_ = str.size();
	maxSize_ = size_;

	if (charData_ != NULL)
		delete[] charData_;

	charData_ = new wchar_t[maxSize_];
	for (int i = 0; i < size_; i++)
		charData_[i] = str[i];
}

void CharString::operator=(const std::wstring & str)
{
	size_ = str.size();
	maxSize_ = size_;

	if (charData_ != NULL)
		delete[] charData_;

	charData_ = new wchar_t[maxSize_];
	for (int i = 0; i < size_; i++)
		charData_[i] = str[i];
}

void CharString::resize(int maxSize)
{
	//size_����,maxsize_�仯
	wchar_t *new_Data = new wchar_t[maxSize];
	//���ı��µ�����С������С��,�ͽض�
	if (maxSize_ < size_)
		size_ = maxSize_;

	for (int i = 0; i < size_; i++)
		new_Data[i] = charData_[i];

	if(charData_ != NULL)
		delete[] charData_;
	charData_ = new_Data;
	maxSize_ = maxSize;
}

const wchar_t & CharString::operator[](int index) const
{
	assert(index < size_);
	return charData_[index];
}
wchar_t & CharString::operator[](int index)
{
	assert(index < size_);
	return charData_[index];
}

const wchar_t & CharString::charAt(int index) const
{
	assert(index < size_);
	return charData_[index];// TODO: �ڴ˴����� return ���
}

wchar_t & CharString::charAt(int index)
{
	assert(index < size_);
	return charData_[index];// TODO: �ڴ˴����� return ���
}

bool CharString::equal(const CharString * rstr)
{
	if(rstr->size() != size_)
		return false;
	else {
		for (int i = 0; i < size_; i++) {
			if (rstr->charAt(i) != charData_[i])
				return false;
		}
		return true;
	}
}

void CharString::push_back(wchar_t ch)
{
	if (size_ >= maxSize_) {
		this->resize(maxSize_ + 1);
	}
	charData_[size_++] = ch;
}

void CharString::pop_back()
{
	size_--;
	charData_[size_] = L'0';
}

std::wstring CharString::to_wstring()
{
	std::wstring str;
	for (int i = 0; i < size_; i++)
		str.push_back(charData_[i]);
	return str;
}

void CharString::print()
{
	std::wcout.imbue(std::locale("chs"));
	for (int i = 0; i < size_; i++)
		std::wcout << charData_[i];
}

std::wostream & operator<<(std::wostream & os, CharString & str)
{
	// TODO: �ڴ˴����� return ���
	for (int i = 0; i < str.size_; i++)
		os << str.charAt(i);
	return os;
}
