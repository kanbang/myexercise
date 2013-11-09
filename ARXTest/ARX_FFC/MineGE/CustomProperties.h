#pragma once

// ��������
class CIntProp : public CMFCPropertyGridProperty
{
public:
	CIntProp(const CString& strName, const COleVariant& varValue, int nMinValue, int nMaxValue, LPCTSTR lpszDescr = NULL);

	virtual BOOL OnUpdateValue();
private:
	int m_nMinValue;
	int m_nMaxValue;
};

// ����������
class CNumericProp : public CMFCPropertyGridProperty
{
public:
	CNumericProp(const CString& strName, const COleVariant& varValue, double dMinValue, double dMaxValue, unsigned short precise, LPCTSTR lpszDescr = NULL);

	virtual BOOL OnUpdateValue();

private:
	double m_dMinValue;
	double m_dMaxValue;
	unsigned short m_precise; // С���㾫��
};

class IntStrProp : public CMFCPropertyGridProperty
{
public:
	IntStrProp(const CString& name, int value, const AcDbIntArray& intValues, const AcStringArray& strValues, LPCTSTR lpszDescr = NULL);
	virtual BOOL OnUpdateValue();
	virtual CString FormatProperty();

private:
	CString Int2Str(int intValue);
	int Str2Int(const CString& strValue);

	int m_value;
	AcDbIntArray m_intValues;
	AcStringArray m_strValues;
};

class DateTimeProperty : public CMFCPropertyGridProperty
{
public:
	DateTimeProperty(const CString& strName, const COleDateTime& timeSrc, LPCTSTR lpszDescr = NULL);

	virtual CString FormatProperty();
	virtual BOOL OnUpdateValue();

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual void OnDrawDescription(CDC* pDC, CRect rect);
};

class MyDateTimeCtrl : public CDateTimeCtrl
{
public:
	MyDateTimeCtrl(	DateTimeProperty* pProp) : m_pProp(pProp) {}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDtnDatetimechange(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMKillfocus(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnDtnCloseup(NMHDR *pNMHDR, LRESULT *pResult);
	// ʹ��NM_KILLFOCUS��DTN_CLOSEUP��2����ϢҲ�ܹ����Ŀ��

protected:
	DateTimeProperty* m_pProp;
};


class DataObjectProperty : public CMFCPropertyGridProperty
{
public:
	DataObjectProperty(const CString& name, const CString& value, const AcStringArray& strValues, LPCTSTR lpszDescr = NULL);

//protected:
//	virtual BOOL HasButton() const { return TRUE; } // ����list֮���������Ч����û����ʾһ��...��ť
//	virtual void OnClickButton(CPoint point) { AfxMessageBox(_T("helllo")); }
};
// ����һ��ClickProp���μ�MFCPropertyGridCtrlHelper.cppĩβ����