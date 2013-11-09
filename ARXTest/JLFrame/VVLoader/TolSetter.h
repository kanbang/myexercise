#pragma once

// ϵͳ�������
class TolSetter
{
public:
	TolSetter(double eq)
	{
		m_originEq = AcGeContext::gTol.equalPoint();
		setEq(eq);
	}

	void setEq(double eq)
	{
		AcGeContext::gTol.setEqualPoint(eq);
	}

	~TolSetter()
	{
		AcGeContext::gTol.setEqualPoint(m_originEq);
	}

private:
	double m_originEq;
};

/*
ʹ�÷���:
TolSetter ts(0.001); // ����������ʱ���Զ���ԭϵͳ���

�ȼ���:
// ����ϵͳ���
double ep = AcGeContext::gTol.equalPoint();
// �������
AcGeContext::gTol.setEqualPoint(0.001); // 10-3

// do something
// ...

// ��ԭϵͳ���
AcGeContext::gTol.setEqualPoint(ep);
*/