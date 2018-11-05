#include "CharStringLink.h"




inline void CharStringLink::print() {
	ListNode * node = head_;
	while (node != nullptr) {
		node->str_->print();
		std::cout << " ";
		node = node->next_;
	}
}

//delete iterator ָ��� node,ע��:!!iter��ʧЧ 
 void CharStringLink::remove(ListIterator * iter) {
	assert(iter->checker == this);
	ListNode * p = iter->node;
	//��p
	if (p == nullptr)
		return;
	//ͷ���
	bool head_flag = 0, last_flag = 0;
	if (p->prior_ == nullptr) {
		head_flag = 1;
		//����ͷ���
		this->head_ = p->next_;
		if (head_)
			this->head_->prior_ = nullptr;
	}
	//β�ڵ�
	if (p->next_ == nullptr) {
		last_flag = 1;
		this->last_ = p->prior_;
		if (last_)
			this->last_->next_ = nullptr;
	}

	if (head_flag == 0) {
		p->prior_->next_ = p->next_;
	}
	if (last_flag == 0) {
		p->next_->prior_ = p->prior_;
	}

	delete p;
}

void CharStringLink::add(const std::wstring & str)
{
	ListNode * node= new ListNode();
	node->str_ = new CharString(str);

	if (head_ == NULL) {
		head_ = last_ = node;
	}
	else {
		last_->next_ = node;
		node->prior_ = last_;
		last_ = node;
	}

}

void CharStringLink::add(CharString * node)
{
	if (head_ == NULL) {
		head_ = last_ = new ListNode(node);
	}
	else {
		last_->next_ = new ListNode(node);
		last_->next_->prior_ = last_;
		last_ =  last_->next_;
	}

}

//����ɾ��ĳ��ͬ���ַ����ĵ�һ��ƥ����

void CharStringLink::remove(CharString * str) {
	ListNode * p = head_;
	ListNode * q = head_;
	if (p == nullptr)
		return;
	else {
		q = p->next_;
		if (p->str_->equal(str)) {// ��һ������
			head_ = q;
			delete p;
			return;
		}
		while (q != nullptr) {//��һ������
			if (q->str_->equal(str)) {//qλ����
				p->next_ = q->next_;
				delete q;
				return;
			}
			p = q;
			q = q->next_;
		}
	}
}

bool CharStringLink::search(CharString * str) {
	ListNode * p = head_;
	while (p != nullptr) {
		if (p->str_->equal(str))
			return 1;
		p = p->next_;
	}
	return 0;
}

void CharStringLink::output(CharString * place) {
	std::wstring url = place->to_wstring() + L".txt";
	std::locale loc("chs");
	std::wofstream os;
	os.imbue(loc);
	os.open(url);

	ListNode *node = head_;
	while (node != nullptr) {
		os << (*(node->str_)) << std::endl;
		node = node->next_;
	}
}

CharStringLink::~CharStringLink()
{
	ListNode * p = head_;
	while (p != nullptr) {
		ListNode * q = p;
		p = p->next_;
		delete q;
	}
}

ListNode::ListNode(CharString * str) {
	str_ = new CharString(str);
}
