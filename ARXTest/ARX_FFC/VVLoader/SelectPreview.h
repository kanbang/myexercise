#pragma once

// 设置系统变量SelectPreview
class SelectPreview
{
public:
	SelectPreview(short sp)
	{
		// 获取系统变量SELECTIONPREVIEW的原始值
		struct resbuf rb;
		acedGetVar(_T("SELECTIONPREVIEW"), &rb);
		m_origin_sp = rb.resval.rint;

		if(sp >=0 && sp <= 2)
		{
			rb.resval.rint = sp;
			acedSetVar(_T("SELECTIONPREVIEW"), &rb);
		}
	}

	~SelectPreview()
	{
		struct resbuf rb;
		rb.restype = RTSHORT;
		rb.resval.rint = m_origin_sp;
		acedSetVar(_T("SELECTIONPREVIEW"), &rb);
	}

private:
	short m_origin_sp;
};