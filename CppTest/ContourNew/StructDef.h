
#if !defined(__STRUCTDEF_H__)
#define __STRUCTDEF_H__


#include <afxtempl.h>

enum EDotSource
{
    DOTSRC_FAULT=0,
    DOTSRC_SCATTER
};


//描述一个点的物理属性，该点的x,y描述了点的物理坐标
struct SPoint
{
    double x;
    double y;
    double z;
};

//描述一个点的逻辑属性。
struct SDot
{
    //三角形中的任意一个点可能在散点数据中,也可能在断层数据中.
    //描述一个点在散点数据或断层数据中的索引及其县官属性
    EDotSource enumDotSource;	//点的来源,可能为断层,也可能为散点
    long iFaultID;	//在enumDotSource为DOTSRC_FAULT时有效，表示断层的ID号；
    // 也就是数组m_faults的下标号
    long index;	//点在来源中的索引号， 数组m_faults[]下的x的小标号
    // ...增加点的其他属性

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
    //描述一个三角形的三个点在散点数据或断层数据中的索引
    SDot ib1;
    SDot ib2;
    SDot ib3;
    //......增加三角形的其他属性
};

class CTriSet:public CObject
{
public:
    DECLARE_SERIAL(CTriSet)

    CTriSet();
    ~CTriSet();

    CArray<STriangle, STriangle> theTriSet;
    long ibcount; //数组theTriSet中元素的个数
    //......增加三角形集合的其他属性}

    void Serialize(CArchive& ar);
};


#endif // !defined(__STRUCTDEF_H__)