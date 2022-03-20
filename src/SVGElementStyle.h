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
#include "SVGElement.h"
#include "SVGElementStyleClass.h"
class CSVGElementStyle : public CSVGElement
{
public:
	CSVGElementStyle(CSVGDocument* ownerDoc);
	~CSVGElementStyle();
public:
	virtual void             parseReady();
	virtual CSVGElement  *cloneNodeWithId(bool deep);
	virtual bool             Toxml(std::wstring &retstrxml, int nLevel);
	CSVGElementStyleClass *  getClassByName(const std::wstring &name);
	std::vector<std::wstring>GetClassNames();
	void                     parserValue(const std::wstring &strValue);
	bool                     addClass(CSVGElementStyleClass *pClass);
	std::vector<CSVGElementStyleClass*> GetClass();
private:
	void Clear();
	std::vector<CSVGElementStyleClass*> m_vcclass;
};
