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
#include "StringHash.h"

CStringHash::CStringHash()
{
	m_nNextIndex = 0;
}

CStringHash::~CStringHash(void)
{
	m_str2int.clear();
}

bool CStringHash::AddHash(const std::wstring &lpszString, unsigned short nIndex)
{
	m_str2int[lpszString] = nIndex;
	m_nNextIndex = nIndex + 1;
	return true;
}

unsigned short CStringHash::GetNextIndex()
{
	return m_nNextIndex;
}

int CStringHash::FindIndex(const std::wstring &lpszString)
{
	std::map<std::wstring, int>::iterator iter = m_str2int.find(lpszString);
	if (iter != m_str2int.end())
	{
		return iter->second;
	}
	return -1;
}
