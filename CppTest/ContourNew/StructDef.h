
#if !defined(__STRUCTDEF_H__)
#define __STRUCTDEF_H__


#include <afxtempl.h>

enum EDotSource
{
    DOTSRC_FAULT=0,
    DOTSRC_SCATTER
};


//����һ������������ԣ��õ��x,y�����˵����������
struct SPoint
{
    double x;
    double y;
    double z;
};

//����һ������߼����ԡ�
struct SDot
{
    //�������е�����һ���������ɢ��������,Ҳ�����ڶϲ�������.
    //����һ������ɢ�����ݻ�ϲ������е����������ع�����
    EDotSource enumDotSource;	//�����Դ,����Ϊ�ϲ�,Ҳ����Ϊɢ��
    long iFaultID;	//��enumDotSourceΪDOTSRC_FAULTʱ��Ч����ʾ�ϲ��ID�ţ�
    // Ҳ��������m_faults���±��
    long index;	//������Դ�е������ţ� ����m_faults[]�µ�x��С���
    // ...���ӵ����������

public:
    bool operator==(const SDot& right) const
    {
        if (enumDotSource == DOTSRC_SCATTER)
            if (enumDotSource == right.enumDotSource && index==right.index)
                return true;
            else
                return false;
        else if (enumDotSource == right.enumDotSource && index==right.index && iFaultID==right.iFaultID)
            return true;
        else
            return false;
    }
};

struct STriangle
{
    //����һ�������ε���������ɢ�����ݻ�ϲ������е�����
    SDot ib1;
    SDot ib2;
    SDot ib3;
    //......���������ε���������
};

class CTriSet:public CObject
{
public:
    DECLARE_SERIAL(CTriSet)

    CTriSet();
    ~CTriSet();

    CArray<STriangle, STriangle> theTriSet;
    long ibcount; //����theTriSet��Ԫ�صĸ���
    //......���������μ��ϵ���������}

    void Serialize(CArchive& ar);
};


#endif // !defined(__STRUCTDEF_H__)