#pragma once

// ����ϵͳ����PickAdd
class PickAdd
{
public:
	PickAdd(short pa)
	{
		// ��ȡϵͳ����SELECTIONPREVIEW��ԭʼֵ
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

