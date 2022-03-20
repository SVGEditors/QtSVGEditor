/**
*
* @license MIT
*
* @copyright: 2022 LinJi
*
* @technical support: www.svgsvg.cn
*
* @email: 93681992@qq.com
*
* @module: QtSVGEditor
*
* 版权申明:
*
* 本源码开源基于MIT协议。
*
* 该软件及其相关文档对所有人免费，可以任意处置，包括使用，复制，修改，合并，发表，分发，再授权，或者销售。
*
* 唯一的限制是需要保留我们的许可申明。
*
*/
#pragma once
#include <string>
#include <map>
class CStringHash
{
public:
	CStringHash();
	~CStringHash(void);
public:
	bool   AddHash(const std::wstring &lpszString, unsigned short nIndex);
	unsigned short GetNextIndex();
	int    FindIndex(const std::wstring &lpszString);
private:
	unsigned short             m_nNextIndex;
	std::map<std::wstring, int>m_str2int;
};
