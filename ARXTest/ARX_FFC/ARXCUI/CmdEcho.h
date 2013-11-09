#pragma once

// 设置类型为short的系统变量
class CmdEcho
{
public:
	CmdEcho(short pa) : m_varName(_T("CMDECHO"))
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

	~CmdEcho()
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

