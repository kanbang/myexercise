#pragma once

// ����ϵͳ����SelectPreview
class SelectPreview
{
public:
	SelectPreview(short sp)
	{
		// ��ȡϵͳ����SELECTIONPREVIEW��ԭʼֵ
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