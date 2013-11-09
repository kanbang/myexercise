#include "StdAfx.h"
#include "UIHelper.h"

#include "DrawManagerDlg.h"
#include "FieldManagerDlg.h"
//#include "PropertyDataDlg.h"
#include "VarListDlg.h"
#include "TypeNameMapDlg.h"
#include "DataObjectStateDlg.h"

void UIHelper::ShowDrawManagerDlg()
{
    // �л���Դ
    CAcModuleResourceOverride myResources;

    DrawManagerDlg dmd;
    dmd.DoModal(); // ���ӻ�Ч���л�������
}

void UIHelper::ShowFieldManagerDlg()
{
    // �л���Դ
    CAcModuleResourceOverride myResources;

    FieldManagerDlg fmd;
    fmd.DoModal(); // ���������ֶι���
}

//void UIHelper::ShowPropertyDataDlg()
//{
//
//}

void UIHelper::ShowVarListDlg()
{
    CAcModuleResourceOverride myResources;

    VarListDlg vld;
    vld.DoModal();
}

void UIHelper::ShowTypeNameMapDlg()
{
    CAcModuleResourceOverride myResources;

    TypeNameMapDlg tnmd;
    tnmd.DoModal();
}

void UIHelper::ShowDataObjectStateDlg()
{
    CAcModuleResourceOverride myResources;

    DataObjectStateDlg dosd;
    dosd.DoModal();
}
