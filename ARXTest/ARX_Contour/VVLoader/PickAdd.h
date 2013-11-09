#pragma once

// 设置系统变量PickAdd
class PickAdd
{
public:
	PickAdd(short pa)
	{
		// 获取系统变量SELECTIONPREVIEW的原始值
		struct resbuf rb;
		acedGetVar(_T("PICKADD"), &rb);
		m_pa = rb.resval.rint;

		if(pa >=0 && pa <= 1)
		{
			rb.resval.rint = pa;
			acedSetVar(_T("PICKADD"), &rb);
		}
	}

	~PickAdd()
	{
		struct resbuf rb;
		rb.restype = RTSHORT;
		rb.resval.rint = m_pa;
		acedSetVar(_T("PICKADD"), &rb);
	}

private:
	int m_pa;
};

