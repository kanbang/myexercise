#include "StdAfx.h"
#include "UIHelper.h"

#include "DrawManagerDlg.h"
#include "FieldManagerDlg.h"
#include "DataObjectStateDlg.h"
#include "VarListDlg.h"

#include "PropertyDataDlgHelper.h"

#include "../ArxHelper/HelperClass.h"

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

void UIHelper::ShowDataObjectStateDlg()
{
    CAcModuleResourceOverride myResources;

    DataObjectStateDlg dosd;
    dosd.DoModal();
}

void UIHelper::DisplayData()
{
    // ��ʾͼԪ����������
    AcDbObjectId objId = ArxUtilHelper::SelectObject( _T( "��ѡ��һ��ͼԪ:" ) );
    if( objId.isNull() ) return;

    PropertyDataDlgHelper::DisplayPropertyDataDlg( objId );
}

void UIHelper::DisplayDataByDoubleClick()
{
    //acutPrintf(_T("\n˫���Զ���..."));
    AcDbObjectIdArray objIds;
    ArxUtilHelper::GetPickSetEntity( objIds );
    if( objIds.length() != 1 ) return;

    // ��ʾ���ԶԻ���
    PropertyDataDlgHelper::DisplayPropertyDataDlg( objIds[0] );
}