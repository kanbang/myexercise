#include "StdAfx.h"

#include "FFC_Data.h"
#include "FFC_Def.h"
#include "GasParam.h"
#include "SysTool.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/* ȫ�ֺ���(DoubleLineHelper.cpp) */
extern void CreateLineArray( AcDbObjectIdArray& objIds, AcDbIntArray& ccIds,
                             AcDbVoidPtrArray& lines, double truncLen,
                             AcDbObjectIdArray& ob_objIds,
                             AcGePoint3dArray& ob_pts,
                             AcDbIntArray& ob_types );

extern void DeleteLineArray( const AcDbVoidPtrArray& lines );

extern void FilterLines( const AcDbVoidPtrArray& all_lines, AcDbVoidPtrArray& lines, bool findWS );

/* ȫ�ֺ���(BuildWSRegion.cpp) */
extern void BuildWSRegion( const AcDbIntArray& ccIds, const AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions );

/* ȫ�ֺ���(BuildGoafRegion.cpp) */
extern void BuildGoafRegion( const AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions );

/* ȫ�ֺ���(ToplogyHelper.cpp) */
extern void BuildTopolgyFromRegions( const AcDbVoidPtrArray& regions, AcGePoint3dArray& vertices, AcDbIntArray& edges, AcDbIntArray& faces, AcDbIntArray& faces_info );

/* ȫ�ֺ���(RegionHelper.cpp) */
extern void DeletAllRegions( AcDbVoidPtrArray& regions );

/* ȫ�ֺ���(FindBoundary.cpp) */
extern void FindInletBoundary( const AcDbObjectIdArray& objIds,
                               const AcDbVoidPtrArray& lines,
                               AcGePoint3dArray& inlet_spts,
                               AcGePoint3dArray& inlet_epts,
                               AcGeDoubleArray& inlet_dirs,
                               AcDbObjectIdArray& inlet_objIds );

/* ȫ�ֺ���(FindGasBoundary.cpp) */
extern void FindGasBoundary( const AcDbObjectIdArray& objIds,
                             const AcDbVoidPtrArray& lines,
                             AcGePoint3dArray& spts,
                             AcGePoint3dArray& epts,
                             AcGeDoubleArray& dirs,
                             AcDbIntArray& gas_types,
                             AcDbObjectIdArray& gas_objIds );

/* ȫ�ֺ���(FindPressureFacility.cpp) */
extern void FindPressureFacility( const AcDbVoidPtrArray& lines,
                                  AcGePoint3dArray& spts,
                                  AcGePoint3dArray& epts,
                                  AcGeDoubleArray& dirs );

/* ȫ�ֺ���(GeoTool.cpp) */
extern void LinesToEdges( AcGePoint3dArray& spts, AcGePoint3dArray& epts, const AcGePoint3dArray& vertices, const AcDbIntArray& edges, AcDbIntArray& bounds );

/* ȫ�ֺ���(FindWallFacility.cpp) */
extern void AddWallToSplitFace( AcGePoint3dArray& vertices, AcDbIntArray& edges,
                                const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                                const AcDbIntArray& goafs,
                                AcDbIntArray& split_faces,
                                AcDbIntArray& split_faces_info,
                                AcDbIntArray& split_face_edges );

/* ȫ�ֺ���(AdjustObturationPointSource.cpp) */
extern void AdjustObturationPointSource( const AcDbVoidPtrArray& lines, AcGePoint3dArray& ob_pts );
extern void FindAirLeakPointSource( AcDbObjectIdArray& al_objIds, AcGePoint3dArray& al_pts );
extern void FindGasPipePointSource( AcDbObjectIdArray& gas_pipe_objIds, AcGePoint3dArray& gas_pipe_pts );
extern void FindNitrogenPipePointSource( AcDbObjectIdArray& n2_pipe_objIds, AcGePoint3dArray& n2_pipe_pts );

/* ȫ�ֺ���(Tool.cpp) */
extern void GetObturationData( const AcDbObjectIdArray& ob_objIds, const AcDbIntArray& ob_types, ObturationDataArray& ob_datas );
extern void GetAirLeakData( const AcDbObjectIdArray& al_objIds, AirLeakDataArray& al_datas );
extern void GetGasPipeData( const AcDbObjectIdArray& gas_pipe_objIds, GasPipeDataArray& gas_pipe_datas );
extern void GetNitrogenPipeData( const AcDbObjectIdArray& n2_pipe_objIds, NitrogenPipeDataArray& n2_pipe_datas );

extern void GetInletBoundaryData( const AcDbObjectIdArray& objIds, InletDataArray& datas );
extern void GetGasBoundaryData( const AcDbObjectIdArray& gas_objIds, GasParam& gp, GasBoundaryDataArray& gas_datas );

/* ȫ�ֺ���(WriteJournalFile.cpp) */
extern void WriteJournalFile(
    const CString& filepath,
    /* �������˹�ϵ */
    const AcGePoint3dArray& vertices, const AcDbIntArray& edges,
    const AcDbIntArray& faces, const AcDbIntArray& faces_info,
    /* �ָ��� */
    const AcDbIntArray& split_faces,
    const AcDbIntArray& split_faces_info,
    const AcDbIntArray& split_face_edges,
    /* ͨ��߽� */
    const AcDbIntArray& inlet_edges,
    /* ��˹�߽� */
    const AcDbIntArray& gas_edges,
    /* ��ǽ��ѹ��ʩ */
    const AcDbIntArray& press_edges,
    /* �ɿ�����׽��� */
    const AcDbIntArray& goafs );

/* ȫ�ֺ���(WriteDataFile.cpp) */
extern void WriteBoundaryDataFile( const CString& filepath,
                                   /* ͨ��߽����� */
                                   const AcGePoint3dArray& inlet_spts,
                                   const AcGePoint3dArray& inlet_epts,
                                   const AcGeDoubleArray inlet_dirs,
                                   const InletDataArray& inlet_datas,
                                   /* ��˹�߽����� */
                                   const AcGePoint3dArray& gas_spts,
                                   const AcGePoint3dArray& gas_epts,
                                   const AcGeDoubleArray& gas_dirs,
                                   const AcDbIntArray& gas_types,
                                   const GasParam& gp,
                                   const GasBoundaryDataArray& gas_datas,
                                   /* ��������ɿ��������ѹ��ʩ���� */
                                   const AcGePoint3dArray& press_spts,
                                   const AcGePoint3dArray& press_epts,
                                   const AcGeDoubleArray press_dirs,
                                   /* ��ǽ��ѹ���� */
                                   const AcDbIntArray& split_faces,
                                   const AcDbIntArray& split_faces_info,
                                   const AcDbIntArray split_face_edges,
                                   /* �ɿ������� */
                                   const AcDbIntArray& goafs );

extern void WriteGoafTBGasDataFile( const CString& filepath,
                                    const AcGePoint3dArray& vertices, const AcDbIntArray& edges,
                                    const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                                    const AcDbIntArray& goafs,
                                    const GasParam& gp );

extern void WritePorousDataFile( const CString& filepath,
                                 const AcGePoint3dArray& press_spts,
                                 const AcGePoint3dArray& press_epts,
                                 const AcGeDoubleArray& press_dirs );

extern void WriteObturationDataFile( const CString& filepath,
                                     const AcGePoint3dArray& ob_pts,
                                     const ObturationDataArray& ob_datas );

extern void WriteAirLeakDataFile( const CString& filepath,
                                  const AcGePoint3dArray al_pts,
                                  const AirLeakDataArray& al_datas );

extern void WriteGasPipeDataFile( const CString& filepath,
                                  const AcGePoint3dArray& gas_pipe_pts,
                                  const GasPipeDataArray& gas_pipe_datas );

extern void WriteNitrogenPipeDataFile( const CString& filepath,
                                       const AcGePoint3dArray& n2_pipe_pts,
                                       const NitrogenPipeDataArray& n2_pipe_datas );

extern void PrintTopology( AcGePoint3dArray& vertices, AcDbIntArray& edges, AcDbIntArray& faces, AcDbIntArray& faces_info );
extern void PrintSplitFace( AcDbIntArray& split_faces, AcDbIntArray& split_faces_info, AcDbIntArray& split_face_edges );
extern void PrintInletBoundary( const AcGePoint3dArray& inlet_spts, const AcGePoint3dArray& inlet_epts, const AcGeDoubleArray& inlet_dirs );
extern void PrintGasBoundary( AcGePoint3dArray& spts, AcGePoint3dArray& epts, AcGeDoubleArray& dirs, AcDbIntArray& gas_types );
extern void PrintPressFacility( const AcGePoint3dArray press_spts, const AcGePoint3dArray& press_epts, const AcGeDoubleArray& press_dirs );

static void EraseAllRegions()
{
    AcDbObjectIdArray objIds;
    DrawHelper::FindMineGEs( _T( "AcDbRegion" ), objIds );

    ArxEntityHelper::EraseObjects2( objIds, true );
}

extern void PrintLines( const AcDbVoidPtrArray& lines );
extern void PrintObPoints( const AcGePoint3dArray& ob_pts );

extern bool HasWSGasBoundary( const AcDbIntArray& gas_types );

static GEO_ERROR_CODE GenGeometryAndData_Impl( const CString& appDir, AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions )
{
    AcDbObjectIdArray objIds;
    AcDbIntArray ccIds;
    const double truncLen = 50;  // �ضϳ���

    // ��Դ(�����ܱյ�Դ����������ɵ�Դ�����)
    AcDbObjectIdArray ob_objIds;
    AcGePoint3dArray ob_pts;
    AcDbIntArray ob_types;

    // �������(������)ͼԪ������DoubleLine����
    CreateLineArray( objIds, ccIds, lines, truncLen, ob_objIds, ob_pts, ob_types );

    // ����:û��ֱ��(����͹�����)
    if( lines.isEmpty() ) return GEC_NO_LINES;

    // ����:û�й�����
    AcDbVoidPtrArray ws_lines;
    FilterLines( lines, ws_lines, true );
    if( ws_lines.isEmpty() ) return GEC_NO_WS;

    PrintLines( lines );

    acutPrintf( _T( "\n����ǰ: " ) );
    PrintObPoints( ob_pts );

    acutPrintf( _T( "\n������: " ) );
    // ���¼����Դ������
    AdjustObturationPointSource( lines, ob_pts );

    PrintObPoints( ob_pts );

    // ��ȡ��Դ����
    ObturationDataArray ob_datas;
    GetObturationData( ob_objIds, ob_types, ob_datas );

    // 1) �����زɹ���������(��������͹�����)
    BuildWSRegion( ccIds, lines, regions );

    // 2) �����ɿ�������
    //    ��¼�����id�����䷶Χ
    int s = regions.length();
    BuildGoafRegion( lines, regions );
    int t = regions.length();
    AcDbIntArray goafs;
    for( int i = s; i < t; i++ )
    {
        goafs.append( i );
    }

    // ����:û�вɿ���
    if( goafs.isEmpty() ) return GEC_NO_GOAF;
    // ����:û�й�������
    if( regions.isEmpty() ) return GEC_NO_REGION;

    // 3) �ֽ�����
    AcGePoint3dArray vertices;        // ������
    AcDbIntArray edges;               // ��֧����
    AcDbIntArray faces;               // ������
    AcDbIntArray faces_info;          // �������Ϣ
    BuildTopolgyFromRegions( regions, vertices, edges, faces, faces_info );

    // 4) �������е�ǽ
    AcDbIntArray split_faces;
    AcDbIntArray split_faces_info;
    AcDbIntArray split_face_edges;
    AddWallToSplitFace( vertices, edges, faces, faces_info, goafs, split_faces, split_faces_info, split_face_edges );

    // ��ӡ�����ꡢ��֧�Լ���
    PrintTopology( vertices, edges, faces, faces_info );

    // ��ӡ�ָ���
    PrintSplitFace( split_faces, split_faces_info, split_face_edges );

    // 4) ����ͨ��߽�
    //	4.1) ����ͨ��߽�ı������Լ�����
    AcGePoint3dArray inlet_spts, inlet_epts;
    AcGeDoubleArray inlet_dirs;
    AcDbObjectIdArray inlet_objIds;
    FindInletBoundary( objIds, lines, inlet_spts, inlet_epts, inlet_dirs, inlet_objIds );
    //	4.2) ��ȡͨ��߽������
    InletDataArray inlet_datas;
    GetInletBoundaryData( inlet_objIds, inlet_datas );
    //	4.3) ������ת���ɷ�֧���
    AcDbIntArray inlet_edges;
    LinesToEdges( inlet_spts, inlet_epts, vertices, edges, inlet_edges );

    PrintInletBoundary( inlet_spts, inlet_epts, inlet_dirs );

    // 5) ������˹�߽�
    //	5.1) ���Ҳɿ�����˹�߽�ı������Լ�����
    AcGePoint3dArray gas_spts, gas_epts;
    AcGeDoubleArray gas_dirs;
    AcDbIntArray gas_types;
    AcDbObjectIdArray gas_objIds;
    FindGasBoundary( objIds, lines, gas_spts, gas_epts, gas_dirs, gas_types, gas_objIds );
    //	5.2) ������ת���ɷ�֧���
    AcDbIntArray gas_edges;
    LinesToEdges( gas_spts, gas_epts, vertices, edges, gas_edges );
    //	5.3) ��ȡ��˹����
    GasParam gp;
    GasBoundaryDataArray gas_datas;
    GetGasBoundaryData( gas_objIds, gp, gas_datas );

    // ��ӡ��˹�߽�
    PrintGasBoundary( gas_spts, gas_epts, gas_dirs, gas_types );

    // ����:û�й�������˹�߽�
    //       ������û�кͲɿ���������һ��
    if( !HasWSGasBoundary( gas_types ) ) return GEC_NO_WS_GAS;

    // 6) ���ҹ�������ɿ��������ѹ����
    //	6.1) ���ҹ�������ɿ��������ѹ���õı����꼰����
    AcGePoint3dArray press_spts, press_epts;
    AcGeDoubleArray press_dirs;
    FindPressureFacility( lines, press_spts, press_epts, press_dirs );
    //	6.2) ������ת���ɷ�֧���
    AcDbIntArray press_edges;
    LinesToEdges( press_spts, press_epts, vertices, edges, press_edges );

    // ��ӡ��������ɿ��������ѹ����
    PrintPressFacility( press_spts, press_epts, press_dirs );

    // ����:û�й�������ɿ��������ѹ����
    //      ������û�кͲɿ���������һ��
    if( press_spts.isEmpty() ) return GEC_NO_PRESS;

    // 7) ©��Դ������
    //	7.1) ����©��Դ��
    AcDbObjectIdArray al_objIds;
    AcGePoint3dArray al_pts;
    FindAirLeakPointSource( al_objIds, al_pts );
    //	7.2) ��ȡ©��Դ������
    AirLeakDataArray al_datas;
    GetAirLeakData( al_objIds, al_datas );

    // 8) ��˹�������
    //	8.1) ������˹���
    AcDbObjectIdArray gas_pipe_objIds;
    AcGePoint3dArray gas_pipe_pts;
    FindGasPipePointSource( gas_pipe_objIds, gas_pipe_pts );
    //	8.2) ��ȡ��˹�������
    GasPipeDataArray gas_pipe_datas;
    GetGasPipeData( gas_pipe_objIds, gas_pipe_datas );

    // 9) ע��������
    //	9.1) ����ע����
    AcDbObjectIdArray n2_pipe_objIds;
    AcGePoint3dArray n2_pipe_pts;
    FindNitrogenPipePointSource( n2_pipe_objIds, n2_pipe_pts );
    //	9.2) ��ȡע��������
    NitrogenPipeDataArray n2_pipe_datas;
    GetNitrogenPipeData( n2_pipe_objIds, n2_pipe_datas );

    // 10) ����jou�ļ�
    CString filepath;
    filepath.Format( _T( "%s%s" ), appDir, JOU_FILE_NAME );
    WriteJournalFile(
        filepath,
        vertices, edges,
        faces, faces_info,
        split_faces,
        split_faces_info,
        split_face_edges,
        inlet_edges,
        gas_edges,
        press_edges,
        goafs );

    // 11) ���������ļ�
    CString data_dir;
    data_dir.Format( _T( "%s%s" ), appDir, DATA_DIR );

    // ���������ļ���
    CreateDataDirectory( data_dir );

    //	11.1) �߽������ļ�
    WriteBoundaryDataFile(
        BuildPath( data_dir, BOUND_FILE_NAME ),
        inlet_spts, inlet_epts, inlet_dirs, inlet_datas,
        gas_spts, gas_epts, gas_dirs, gas_types, gp, gas_datas,
        press_spts, press_epts, press_dirs,
        split_faces, split_faces_info, split_face_edges,
        goafs );

    //	11.2) �ɿ������װ���˹Դ
    WriteGoafTBGasDataFile(
        BuildPath( data_dir, GOAF_TB_GAS_FILE_NAME ),
        vertices, edges, faces, faces_info,
        goafs, gp );

    //	11.3) ��׽�������
    WritePorousDataFile(
        BuildPath( data_dir, POROUS_FILE_NAME ),
        press_spts, press_epts, press_dirs );

    //	11.4) �ܱ�Դ��
    WriteObturationDataFile(
        BuildPath( data_dir, OBTURATION_FILE_NAME ),
        ob_pts, ob_datas );

    //	11.5) ©��Դ��
    WriteAirLeakDataFile(
        BuildPath( data_dir, AIR_LEAK_FILE_NAME ),
        al_pts, al_datas );

    //	11.6) ��˹���
    WriteGasPipeDataFile(
        BuildPath( data_dir, GAS_PIPE_FILE_NAME ),
        gas_pipe_pts, gas_pipe_datas );

    //	11.7) ע����
    WriteNitrogenPipeDataFile(
        BuildPath( data_dir, N2_PIPE_FILE_NAME ),
        n2_pipe_pts, n2_pipe_datas );

    return GEC_NO_ERROR;
}

static void AddRegionToDB( const AcDbVoidPtrArray& regions )
{
    // ������
    // ��ӵ����ݿ���ʾͼ��
    if( !regions.isEmpty() )
    {
        for( int i = 0; i < regions.length(); i++ )
        {
            AcDbRegion* pRegion = ( AcDbRegion* )regions[i];
            if( !ArxUtilHelper::PostToModelSpace( pRegion ) ) delete pRegion;
        }
        acutPrintf( _T( "\n����%d������ɹ�!" ), regions.length() );
    }
    else
    {
        acutPrintf( _T( "\n��������ʧ��!" ) );
    }
}

// Ӧ�ó�������Ŀ¼
// Ŀǰ������������(������)
//CString appDir = _T("C:\\Users\\anheihb03dlj\\Desktop\\");
GEO_ERROR_CODE GenGeometryAndData( const CString& appDir )
{
    AcDbVoidPtrArray lines;
    AcDbVoidPtrArray regions;
    GEO_ERROR_CODE gec = GenGeometryAndData_Impl( appDir, lines, regions );

    // ɾ�����е�����(������)
    //EraseAllRegions();

    // ��������(������)
    //AddRegionToDB(regions);

    // ������
    DeletAllRegions( regions );
    DeleteLineArray( lines );

    return gec;
}
