#pragma once
//下个版本用来进行信息过滤,现在测试版本,可以过滤编辑
//本质是进行子树匹配,若匹配成功,返回对应文本
//用来进行文本信息提取会更有效

//一些脑洞:或许可以让机器自己学习怎么过滤信息
#include "CharString.h"
class HtmlNode;


class HtmlFilter
{
private:
	HtmlNode * list_head;
	CharString * value;

	bool equal(HtmlNode * node1, HtmlNode * node2);
public:
	bool filter(HtmlNode * node, HtmlNode * filt);

	void searchTree(HtmlNode * tree);

	HtmlFilter();
	~HtmlFilter();
};

