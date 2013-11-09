#include "StdAfx.h"

#include "FFC_Data.h"
#include "FFC_Def.h"
#include "GasParam.h"
#include "SysTool.h"

#include "../MineGE/HelperClass.h"
#include "../ArxHelper/HelperClass.h"

/* 全局函数(DoubleLineHelper.cpp) */
extern void CreateLineArray( AcDbObjectIdArray& objIds, AcDbIntArray& ccIds,
                             AcDbVoidPtrArray& lines, double truncLen,
                             AcDbObjectIdArray& ob_objIds,
                             AcGePoint3dArray& ob_pts,
                             AcDbIntArray& ob_types );

extern void DeleteLineArray( const AcDbVoidPtrArray& lines );

extern void FilterLines( const AcDbVoidPtrArray& all_lines, AcDbVoidPtrArray& lines, bool findWS );

/* 全局函数(BuildWSRegion.cpp) */
extern void BuildWSRegion( const AcDbIntArray& ccIds, const AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions );

/* 全局函数(BuildGoafRegion.cpp) */
extern void BuildGoafRegion( const AcDbVoidPtrArray& lines, AcDbVoidPtrArray& regions );

/* 全局函数(ToplogyHelper.cpp) */
extern void BuildTopolgyFromRegions( const AcDbVoidPtrArray& regions, AcGePoint3dArray& vertices, AcDbIntArray& edges, AcDbIntArray& faces, AcDbIntArray& faces_info );

/* 全局函数(RegionHelper.cpp) */
extern void DeletAllRegions( AcDbVoidPtrArray& regions );

/* 全局函数(FindBoundary.cpp) */
extern void FindInletBoundary( const AcDbObjectIdArray& objIds,
                               const AcDbVoidPtrArray& lines,
                               AcGePoint3dArray& inlet_spts,
                               AcGePoint3dArray& inlet_epts,
                               AcGeDoubleArray& inlet_dirs,
                               AcDbObjectIdArray& inlet_objIds );

/* 全局函数(FindGasBoundary.cpp) */
extern void FindGasBoundary( const AcDbObjectIdArray& objIds,
                             const AcDbVoidPtrArray& lines,
                             AcGePoint3dArray& spts,
                             AcGePoint3dArray& epts,
                             AcGeDoubleArray& dirs,
                             AcDbIntArray& gas_types,
                             AcDbObjectIdArray& gas_objIds );

/* 全局函数(FindPressureFacility.cpp) */
extern void FindPressureFacility( const AcDbVoidPtrArray& lines,
                                  AcGePoint3dArray& spts,
                                  AcGePoint3dArray& epts,
                                  AcGeDoubleArray& dirs );

/* 全局函数(GeoTool.cpp) */
extern void LinesToEdges( AcGePoint3dArray& spts, AcGePoint3dArray& epts, const AcGePoint3dArray& vertices, const AcDbIntArray& edges, AcDbIntArray& bounds );

/* 全局函数(FindWallFacility.cpp) */
extern void AddWallToSplitFace( AcGePoint3dArray& vertices, AcDbIntArray& edges,
                                const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                                const AcDbIntArray& goafs,
                                AcDbIntArray& split_faces,
                                AcDbIntArray& split_faces_info,
                                AcDbIntArray& split_face_edges );

/* 全局函数(AdjustObturationPointSource.cpp) */
extern void AdjustObturationPointSource( const AcDbVoidPtrArray& lines, AcGePoint3dArray& ob_pts );
extern void FindAirLeakPointSource( AcDbObjectIdArray& al_objIds, AcGePoint3dArray& al_pts );
extern void FindGasPipePointSource( AcDbObjectIdArray& gas_pipe_objIds, AcGePoint3dArray& gas_pipe_pts );
extern void FindNitrogenPipePointSource( AcDbObjectIdArray& n2_pipe_objIds, AcGePoint3dArray& n2_pipe_pts );

/* 全局函数(Tool.cpp) */
extern void GetObturationData( const AcDbObjectIdArray& ob_objIds, const AcDbIntArray& ob_types, ObturationDataArray& ob_datas );
extern void GetAirLeakData( const AcDbObjectIdArray& al_objIds, AirLeakDataArray& al_datas );
extern void GetGasPipeData( const AcDbObjectIdArray& gas_pipe_objIds, GasPipeDataArray& gas_pipe_datas );
extern void GetNitrogenPipeData( const AcDbObjectIdArray& n2_pipe_objIds, NitrogenPipeDataArray& n2_pipe_datas );

extern void GetInletBoundaryData( const AcDbObjectIdArray& objIds, InletDataArray& datas );
extern void GetGasBoundaryData( const AcDbObjectIdArray& gas_objIds, GasParam& gp, GasBoundaryDataArray& gas_datas );

/* 全局函数(WriteJournalFile.cpp) */
extern void WriteJournalFile(
    const CString& filepath,
    /* 面域拓扑关系 */
    const AcGePoint3dArray& vertices, const AcDbIntArray& edges,
    const AcDbIntArray& faces, const AcDbIntArray& faces_info,
    /* 分割面 */
    const AcDbIntArray& split_faces,
    const AcDbIntArray& split_faces_info,
    const AcDbIntArray& split_face_edges,
    /* 通风边界 */
    const AcDbIntArray& inlet_edges,
    /* 瓦斯边界 */
    const AcDbIntArray& gas_edges,
    /* 闭墙降压设施 */
    const AcDbIntArray& press_edges,
    /* 采空区多孔介质 */
    const AcDbIntArray& goafs );

/* 全局函数(WriteDataFile.cpp) */
extern void WriteBoundaryDataFile( const CString& filepath,
                                   /* 通风边界数据 */
                                   const AcGePoint3dArray& inlet_spts,
                                   const AcGePoint3dArray& inlet_epts,
                                   const AcGeDoubleArray inlet_dirs,
                                   const InletDataArray& inlet_datas,
                                   /* 瓦斯边界数据 */
                                   const AcGePoint3dArray& gas_spts,
                                   const AcGePoint3dArray& gas_epts,
                                   const AcGeDoubleArray& gas_dirs,
                                   const AcDbIntArray& gas_types,
                                   const GasParam& gp,
                                   const GasBoundaryDataArray& gas_datas,
                                   /* 工作面与采空区间隔降压设施数据 */
                                   const AcGePoint3dArray& press_spts,
                                   const AcGePoint3dArray& press_epts,
                                   const AcGeDoubleArray press_dirs,
                                   /* 闭墙降压数据 */
                                   const AcDbIntArray& split_faces,
                                   const AcDbIntArray& split_faces_info,
                                   const AcDbIntArray split_face_edges,
                                   /* 采空区数据 */
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
    const double truncLen = 50;  // 截断长度

    // 点源(包括密闭点源和其它处理成点源的巷道)
    AcDbObjectIdArray ob_objIds;
    AcGePoint3dArray ob_pts;
    AcDbIntArray ob_types;

    // 查找巷道(工作面)图元，构建DoubleLine数组
    CreateLineArray( objIds, ccIds, lines, truncLen, ob_objIds, ob_pts, ob_types );

    // 错误:没有直线(巷道和工作面)
    if( lines.isEmpty() ) return GEC_NO_LINES;

    // 错误:没有工作面
    AcDbVoidPtrArray ws_lines;
    FilterLines( lines, ws_lines, true );
    if( ws_lines.isEmpty() ) return GEC_NO_WS;

    PrintLines( lines );

    acutPrintf( _T( "\n调整前: " ) );
    PrintObPoints( ob_pts );

    acutPrintf( _T( "\n调整后: " ) );
    // 重新计算点源的坐标
    AdjustObturationPointSource( lines, ob_pts );

    PrintObPoints( ob_pts );

    // 获取点源数据
    ObturationDataArray ob_datas;
    GetObturationData( ob_objIds, ob_types, ob_datas );

    // 1) 创建回采工作面面域(包含巷道和工作面)
    BuildWSRegion( ccIds, lines, regions );

    // 2) 创建采空区面域
    //    记录面域的id号区间范围
    int s = regions.length();
    BuildGoafRegion( lines, regions );
    int t = regions.length();
    AcDbIntArray goafs;
    for( int i = s; i < t; i++ )
    {
        goafs.append( i );
    }

    // 错误:没有采空区
    if( goafs.isEmpty() ) return GEC_NO_GOAF;
    // 错误:没有构成面域
    if( regions.isEmpty() ) return GEC_NO_REGION;

    // 3) 分解面域
    AcGePoint3dArray vertices;        // 点坐标
    AcDbIntArray edges;               // 分支拓扑
    AcDbIntArray faces;               // 面拓扑
    AcDbIntArray faces_info;          // 面个数信息
    BuildTopolgyFromRegions( regions, vertices, edges, faces, faces_info );

    // 4) 查找所有的墙
    AcDbIntArray split_faces;
    AcDbIntArray split_faces_info;
    AcDbIntArray split_face_edges;
    AddWallToSplitFace( vertices, edges, faces, faces_info, goafs, split_faces, split_faces_info, split_face_edges );

    // 打印点坐标、分支以及面
    PrintTopology( vertices, edges, faces, faces_info );

    // 打印分割面
    PrintSplitFace( split_faces, split_faces_info, split_face_edges );

    // 4) 查找通风边界
    //	4.1) 查找通风边界的边坐标以及方向
    AcGePoint3dArray inlet_spts, inlet_epts;
    AcGeDoubleArray inlet_dirs;
    AcDbObjectIdArray inlet_objIds;
    FindInletBoundary( objIds, lines, inlet_spts, inlet_epts, inlet_dirs, inlet_objIds );
    //	4.2) 获取通风边界的数据
    InletDataArray inlet_datas;
    GetInletBoundaryData( inlet_objIds, inlet_datas );
    //	4.3) 边坐标转换成分支编号
    AcDbIntArray inlet_edges;
    LinesToEdges( inlet_spts, inlet_epts, vertices, edges, inlet_edges );

    PrintInletBoundary( inlet_spts, inlet_epts, inlet_dirs );

    // 5) 查找瓦斯边界
    //	5.1) 查找采空区瓦斯边界的边坐标以及方向
    AcGePoint3dArray gas_spts, gas_epts;
    AcGeDoubleArray gas_dirs;
    AcDbIntArray gas_types;
    AcDbObjectIdArray gas_objIds;
    FindGasBoundary( objIds, lines, gas_spts, gas_epts, gas_dirs, gas_types, gas_objIds );
    //	5.2) 边坐标转换成分支编号
    AcDbIntArray gas_edges;
    LinesToEdges( gas_spts, gas_epts, vertices, edges, gas_edges );
    //	5.3) 读取瓦斯数据
    GasParam gp;
    GasBoundaryDataArray gas_datas;
    GetGasBoundaryData( gas_objIds, gp, gas_datas );

    // 打印瓦斯边界
    PrintGasBoundary( gas_spts, gas_epts, gas_dirs, gas_types );

    // 错误:没有工作面瓦斯边界
    //       工作面没有和采空区关联在一起
    if( !HasWSGasBoundary( gas_types ) ) return GEC_NO_WS_GAS;

    // 6) 查找工作面与采空区间隔降压设置
    //	6.1) 查找工作面与采空区间隔降压设置的边坐标及方向
    AcGePoint3dArray press_spts, press_epts;
    AcGeDoubleArray press_dirs;
    FindPressureFacility( lines, press_spts, press_epts, press_dirs );
    //	6.2) 边坐标转换成分支编号
    AcDbIntArray press_edges;
    LinesToEdges( press_spts, press_epts, vertices, edges, press_edges );

    // 打印工作面与采空区间隔降压设置
    PrintPressFacility( press_spts, press_epts, press_dirs );

    // 错误:没有工作面与采空区间隔降压设置
    //      工作面没有和采空区关联在一起
    if( press_spts.isEmpty() ) return GEC_NO_PRESS;

    // 7) 漏风源汇数据
    //	7.1) 查找漏风源汇
    AcDbObjectIdArray al_objIds;
    AcGePoint3dArray al_pts;
    FindAirLeakPointSource( al_objIds, al_pts );
    //	7.2) 读取漏风源汇数据
    AirLeakDataArray al_datas;
    GetAirLeakData( al_objIds, al_datas );

    // 8) 瓦斯钻孔数据
    //	8.1) 查找瓦斯钻孔
    AcDbObjectIdArray gas_pipe_objIds;
    AcGePoint3dArray gas_pipe_pts;
    FindGasPipePointSource( gas_pipe_objIds, gas_pipe_pts );
    //	8.2) 读取瓦斯钻孔数据
    GasPipeDataArray gas_pipe_datas;
    GetGasPipeData( gas_pipe_objIds, gas_pipe_datas );

    // 9) 注氮孔数据
    //	9.1) 查找注氮孔
    AcDbObjectIdArray n2_pipe_objIds;
    AcGePoint3dArray n2_pipe_pts;
    FindNitrogenPipePointSource( n2_pipe_objIds, n2_pipe_pts );
    //	9.2) 读取注氮孔数据
    NitrogenPipeDataArray n2_pipe_datas;
    GetNitrogenPipeData( n2_pipe_objIds, n2_pipe_datas );

    // 10) 生成jou文件
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

    // 11) 生成数据文件
    CString data_dir;
    data_dir.Format( _T( "%s%s" ), appDir, DATA_DIR );

    // 创建数据文件夹
    CreateDataDirectory( data_dir );

    //	11.1) 边界数据文件
    WriteBoundaryDataFile(
        BuildPath( data_dir, BOUND_FILE_NAME ),
        inlet_spts, inlet_epts, inlet_dirs, inlet_datas,
        gas_spts, gas_epts, gas_dirs, gas_types, gp, gas_datas,
        press_spts, press_epts, press_dirs,
        split_faces, split_faces_info, split_face_edges,
        goafs );

    //	11.2) 采空区顶底板瓦斯源
    WriteGoafTBGasDataFile(
        BuildPath( data_dir, GOAF_TB_GAS_FILE_NAME ),
        vertices, edges, faces, faces_info,
        goafs, gp );

    //	11.3) 多孔介质设置
    WritePorousDataFile(
        BuildPath( data_dir, POROUS_FILE_NAME ),
        press_spts, press_epts, press_dirs );

    //	11.4) 密闭源汇
    WriteObturationDataFile(
        BuildPath( data_dir, OBTURATION_FILE_NAME ),
        ob_pts, ob_datas );

    //	11.5) 漏风源汇
    WriteAirLeakDataFile(
        BuildPath( data_dir, AIR_LEAK_FILE_NAME ),
        al_pts, al_datas );

    //	11.6) 瓦斯钻孔
    WriteGasPipeDataFile(
        BuildPath( data_dir, GAS_PIPE_FILE_NAME ),
        gas_pipe_pts, gas_pipe_datas );

    //	11.7) 注氮孔
    WriteNitrogenPipeDataFile(
        BuildPath( data_dir, N2_PIPE_FILE_NAME ),
        n2_pipe_pts, n2_pipe_datas );

    return GEC_NO_ERROR;
}

static void AddRegionToDB( const AcDbVoidPtrArray& regions )
{
    // 测试用
    // 添加到数据库显示图形
    if( !regions.isEmpty() )
    {
        for( int i = 0; i < regions.length(); i++ )
        {
            AcDbRegion* pRegion = ( AcDbRegion* )regions[i];
            if( !ArxUtilHelper::PostToModelSpace( pRegion ) ) delete pRegion;
        }
        acutPrintf( _T( "\n创建%d个面域成功!" ), regions.length() );
    }
    else
    {
        acutPrintf( _T( "\n创建面域失败!" ) );
    }
}

// 应用程序所在目录
// 目前假设在桌面上(测试用)
//CString appDir = _T("C:\\Users\\anheihb03dlj\\Desktop\\");
GEO_ERROR_CODE GenGeometryAndData( const CString& appDir )
{
    AcDbVoidPtrArray lines;
    AcDbVoidPtrArray regions;
    GEO_ERROR_CODE gec = GenGeometryAndData_Impl( appDir, lines, regions );

    // 删除所有的面域(测试用)
    //EraseAllRegions();

    // 绘制面域(测试用)
    //AddRegionToDB(regions);

    // 清理工作
    DeletAllRegions( regions );
    DeleteLineArray( lines );

    return gec;
}
