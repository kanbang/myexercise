#include "StdAfx.h"
#include "MVSSDataLink.h"

MVSSDataLink::MVSSDataLink() : DataLink(), objId( -1 )
{

}

void JointDataLink::regDatas()
{
    /*linkDoubleData(_T("����X"), &x);
    linkDoubleData(_T("����Y"), &y);
    linkDoubleData(_T("����Z"), &z);
    linkStringData(_T("����"), &name);
    linkStringData(_T("���ѹ��"), &rp);
    linkStringData(_T("����ѹ"), &p);
    linkStringData(_T("��ע"), &comment);*/
}

void TunnelDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkStringData( _T( "����" ), &q );
    linkStringData( _T( "Ħ������" ), &r );
    linkStringData( _T( "�ֲ�����" ), &lr );
    linkStringData( _T( "������" ), &h );
    linkStringData( _T( "�̶�����" ), &fq );
    linkStringData( _T( "�ܶ�" ), &rho );
    linkStringData( _T( "�ɵ�����" ), &ad );
    linkStringData( _T( "��������" ), &ah );
    linkStringData( _T( "���ڷ���" ), &ar );
    linkStringData( _T( "�����ܶ�" ), &rho1 );
    linkStringData( _T( "�ļ��ܶ�" ), &rho2 );
    linkStringData( _T( "�＾�ܶ�" ), &rho3 );
    linkStringData( _T( "�����ܶ�" ), &rho4 );
    linkStringData( _T( "���Է���" ), &tq );
    linkStringData( _T( "��������" ), &th );
    linkStringData( _T( "�������" ), &l );
    linkStringData( _T( "Ħ������ϵ��" ), &apha );
    linkStringData( _T( "����Ħ������" ), &r100 );
    linkStringData( _T( "������״" ), &atype );
    linkStringData( _T( "�������" ), &area );
    linkStringData( _T( "�ܳ�" ), &c );
    linkStringData( _T( "֧����ʽ" ), &st );
    linkStringData( _T( "�����������" ), &maxV );
    linkStringData( _T( "�����;" ), &tUse );
    linkStringData( _T( "����" ), &tRegion );
    linkStringData( _T( "�¶�" ), &temp );
    linkStringData( _T( "ʪ��" ), &humidity );
    linkStringData( _T( "O2" ), &O2 );
    linkStringData( _T( "CH4" ), &CH4 );
    linkStringData( _T( "CO2" ), &CO2 );
    linkStringData( _T( "CO" ), &CO );
    linkStringData( _T( "�ɿ���" ), &reliability );
    linkStringData( _T( "����" ), &type );
    linkStringData( _T( "��ע" ), &comment );
    linkStringData( _T( "��������" ), &eh );
}

void GateDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkStringData( _T( "ѹ��" ), &rq );
    linkStringData( _T( "©����" ), &flow );
    linkStringData( _T( "��Ч����" ), &er );
    linkStringData( _T( "��Ч�細���" ), &ea );
    linkStringData( _T( "����ѹ��" ), &trq );
}

void FanDataLink::regDatas()
{
    linkStringData( _T( "����" ), &name );
    linkStringData( _T( "����״̬" ), &rs );
    linkStringData( _T( "����" ), &q );
    linkStringData( _T( "��ѹ" ), &p );
    linkStringData( _T( "����" ), &power );
    linkStringData( _T( "Ч��" ), &efficiency );
    linkStringData( _T( "����" ), &r );
    linkStringData( _T( "�Ȼ���" ), &aHole );
    linkStringData( _T( "�ͺ�" ), &model );
    linkStringData( _T( "ҶƬ�Ƕ�" ), &angle );
    linkStringData( _T( "ת��" ), &rotateSpeed );
    linkStringData( _T( "�������" ), &fq );
    linkStringData( _T( "��������" ), &nq );
    linkStringData( _T( "©�����" ), &lq );
    linkStringData( _T( "�����ѹ" ), &fsp );
    linkStringData( _T( "���ȫѹ" ), &ffp );
    linkStringData( _T( "������ѹ" ), &outVP );
    linkStringData( _T( "�����ѹ" ), &inVP );
    linkStringData( _T( "��ھ�ѹ" ), &inSP );
    linkStringData( _T( "���Ծ�ѹ" ), &tSP );
    linkStringData( _T( "�ĵ繦��" ), &cPower );
    linkStringData( _T( "���빦��" ), &inPower );
    linkStringData( _T( "�������" ), &outPower );
    linkStringData( _T( "��������" ), &nPower );
    linkStringData( _T( "©�繦��" ), &lPower );
    linkStringData( _T( "����Ч��" ), &uPower );
    linkStringData( _T( "���Ч��" ), &ePower );
    linkStringData( _T( "��ѹЧ��" ), &sPower );
    linkStringData( _T( "ȫѹЧ��" ), &fPower );
    linkStringData( _T( "�������" ), &fr );
    linkStringData( _T( "��������" ), &nr );
    linkStringData( _T( "©�����" ), &lr );
    linkStringData( _T( "���϶���" ), &fma );
    linkStringData( _T( "���Ϸ���" ), &fmv );
    linkStringData( _T( "���ڶ���" ), &outArea );
    linkStringData( _T( "���ڷ���" ), &outV );
    linkStringData( _T( "��ʼ����" ), &iq );
    linkStringData( _T( "��ע" ), &comment );
    linkStringData( _T( "H_a0" ), &H_a0 );
    linkStringData( _T( "H_a1" ), &H_a1 );
    linkStringData( _T( "H_a2" ), &H_a2 );
    linkStringData( _T( "H_a3" ), &H_a3 );
    linkStringData( _T( "H_a4" ), &H_a4 );
    linkStringData( _T( "H_a5" ), &H_a5 );
    linkStringData( _T( "N_a0" ), &N_a0 );
    linkStringData( _T( "N_a1" ), &N_a1 );
    linkStringData( _T( "N_a2" ), &N_a2 );
    linkStringData( _T( "N_a3" ), &N_a3 );
    linkStringData( _T( "N_a4" ), &N_a4 );
    linkStringData( _T( "N_a5" ), &N_a5 );
    linkStringData( _T( "E_a0" ), &E_a0 );
    linkStringData( _T( "E_a1" ), &E_a1 );
    linkStringData( _T( "E_a2" ), &E_a2 );
    linkStringData( _T( "E_a3" ), &E_a3 );
    linkStringData( _T( "E_a4" ), &E_a4 );
    linkStringData( _T( "E_a5" ), &E_a5 );
}