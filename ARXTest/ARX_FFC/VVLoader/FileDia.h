#pragma once

// 设置系统变量FileDia
class FileDia
{
public:
	FileDia(short pa) : m_varName(_T("FILEDIA"))
	{
		// 获取系统变量FILEDIA的原始值
		struct resbuf rb;
		acedGetVar(m_varName, &rb);
		m_pa = rb.resval.rint;

		if(pa == 0 || pa == 1)
		{
			rb.resval.rint = pa;
			acedSetVar(m_varName, &rb);
		}
	}

	~FileDia()
	{
		struct resbuf rb;
		rb.restype = RTSHORT;
		rb.resval.rint = m_pa;
		acedSetVar(m_varName, &rb);
	}

private:
	int m_pa;
	CString m_varName;
};

