#include "stdafx.h"

#include "FFC_Data.h"
#include "GasParam.h"

extern double FacePolygonArea( const AcGePoint3dArray& vertices, const AcDbIntArray& edges,
                               const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                               int k );

static void WriteInletBoundaryData( CStdioFile& outfile,
                                    const AcGePoint3dArray& inlet_spts,
                                    const AcGePoint3dArray& inlet_epts,
                                    const AcGeDoubleArray inlet_dirs,
                                    const InletDataArray& inlet_datas )
{
    // ͨ��߽���Ŀ
    CString str;
    str.Format( _T( "%d\n" ), inlet_datas.length() );
    outfile.WriteString( str );

    // ͨ��߽�����
    for( int i = 0; i < inlet_datas.length(); i++ )
    {
        InletData data = inlet_datas[i];
        double dir = inlet_dirs[i];

        //�Ƕ�	ͨ����	���	����Ũ��(%)	����Ũ��(%)
        str.Format( _T( "%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n" ), dir, data.q, data.area, data.ch4, data.o2, data.t );
        outfile.WriteString( str );
    }
}

static double AverageGas( const AcGePoint3dArray& gas_spts,
                          const AcGePoint3dArray& gas_epts,
                          const AcDbIntArray& gas_types,
                          const GasParam& gp,
                          int type )
{
    // ָ�����͵��ܳ���
    double L = 0;
    for( int i = 0; i < gas_types.length(); i++ )
    {
        if( gas_types[i] == type )
        {
            double dist = gas_spts[i].distanceTo( gas_epts[i] );
            L += dist;
        }
    }
    if( L == 0 ) return 0;

    double d = 0;
    switch( type )
    {
        // ����
    case 1:
        d = gp.f2 / L;
        break;

        // ������(�����)
        // Ӧ��ȡʵ�ʵĹ���������
        //case 2:
        //	d = gp.f1/L;
        //	break;

        // ������
    case 3:
        d = gp.f3 / L;
        break;
    }
    return d;
}

static double CaclWSGas( double q, const AcGePoint3d& spt, const AcGePoint3d& ept )
{
    return q / ( spt.distanceTo( ept ) );
}

static void WriteGasBoundaryData( CStdioFile& outfile,
                                  const AcGePoint3dArray& gas_spts,
                                  const AcGePoint3dArray& gas_epts,
                                  const AcGeDoubleArray& gas_dirs,
                                  const AcDbIntArray& gas_types,
                                  const GasParam& gp,
                                  const GasBoundaryDataArray& gas_datas )
{
    // ����ƽ����˹ӿ��
    double q1 = AverageGas( gas_spts, gas_epts, gas_types, gp, 1 );
    // ������ƽ����˹ӿ��
    //double q2 = AverageGas(gas_spts, gas_epts, gas_types, gp, 2);
    // ������ƽ����˹ӿ��
    double q3 = AverageGas( gas_spts, gas_epts, gas_types, gp, 3 );

    // ��˹�߽����
    CString str;
    str.Format( _T( "%d\n" ), gas_types.length() );
    outfile.WriteString( str );

    // ��˹�߽�����
    for( int i = 0; i < gas_types.length(); i++ )
    {
        switch( gas_types[i] )
        {
        case 1:
            str.Format( _T( "%.4f\n" ), q1 );
            break;

        case 2:
            //str.Format(_T("%.4f\n"), q2);
            str.Format( _T( "%.4f\n" ), CaclWSGas( gas_datas[i].q, gas_spts[i], gas_epts[i] ) );
            break;

        case 3:
            str.Format( _T( "%.4f\n" ), q3 );
            break;
        }
        outfile.WriteString( str );
    }
}

static void WritePressFacility( CStdioFile& outfile,
                                const AcGePoint3dArray& press_spts,
                                const AcGePoint3dArray& press_epts,
                                const AcGeDoubleArray press_dirs )
{
    CString str;
    str.Format( _T( "%d\n" ), press_spts.length() );
    outfile.WriteString( str );
}

static void WriteWallFacility( CStdioFile& outfile,
                               const AcDbIntArray& split_faces,
                               const AcDbIntArray& split_faces_info,
                               const AcDbIntArray split_face_edges )
{
    CString str;
    str.Format( _T( "%d\n" ), split_face_edges.length() );
    outfile.WriteString( str );
}

static void WriteGoaf( CStdioFile& outfile, const AcDbIntArray& goafs )
{
    CString str;
    str.Format( _T( "%d\n" ), goafs.length() );
    outfile.WriteString( str );
}

void WriteBoundaryDataFile( const CString& filepath,
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
                            const AcDbIntArray& goafs )
{
    CStdioFile outfile;
    outfile.Open( filepath, CFile::modeCreate | CFile::modeWrite );

    acutPrintf( _T( "\nд��ͨ��߽�����" ) );
    WriteInletBoundaryData( outfile, inlet_spts, inlet_epts, inlet_dirs, inlet_datas );

    acutPrintf( _T( "\nд����˹�߽�����" ) );
    WriteGasBoundaryData( outfile, gas_spts, gas_epts, gas_dirs, gas_types, gp, gas_datas );

    acutPrintf( _T( "\nд�빤���潵ѹ��ʩ����" ) );
    WritePressFacility( outfile, press_spts, press_epts, press_dirs );

    acutPrintf( _T( "\nд���ǽ��ѹ��ʩ����" ) );
    WriteWallFacility( outfile, split_faces, split_faces_info, split_face_edges );

    acutPrintf( _T( "\nд��ɿ�����׽�������" ) );
    WriteGoaf( outfile, goafs );

    outfile.Close();
}

static double CaclTotalGoafArea( const AcGePoint3dArray& vertices, const AcDbIntArray& edges,
                                 const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                                 const AcDbIntArray& goafs )
{
    double S = 0;
    for( int i = 0; i < goafs.length(); i++ )
    {
        S += FacePolygonArea( vertices, edges, faces, faces_info, goafs[i] );
    }
    return S;
}

void WriteGoafTBGasDataFile( const CString& filepath,
                             const AcGePoint3dArray& vertices, const AcDbIntArray& edges,
                             const AcDbIntArray& faces, const AcDbIntArray& faces_info,
                             const AcDbIntArray& goafs,
                             const GasParam& gp )
{
    CStdioFile outfile;
    outfile.Open( filepath, CFile::modeCreate | CFile::modeWrite );

    // д��ɿ�������
    CString str;
    str.Format( _T( "%d\n" ), goafs.length() );
    outfile.WriteString( str );

    // д��ɿ��������
    double area = CaclTotalGoafArea( vertices, edges, faces, faces_info, goafs );
    str.Format( _T( "%.4f\n" ), area );
    outfile.WriteString( str );

    // д��(�ɿ���+���װ�)��˹ӿ����֮��
    double q = gp.f1 + gp.f4 + gp.f5;
    str.Format( _T( "%.4f\n" ), q );
    outfile.WriteString( str );

    outfile.Close();
}

void WritePorousDataFile( const CString& filepath,
                          const AcGePoint3dArray& press_spts,
                          const AcGePoint3dArray& press_epts,
                          const AcGeDoubleArray& press_dirs )
{
    CStdioFile outfile;
    outfile.Open( filepath, CFile::modeCreate | CFile::modeWrite );

    // д�����
    CString str;
    str.Format( _T( "%d\n" ), press_dirs.length() );
    outfile.WriteString( str );

    // д�뷽���Լ����ĵ�����
    for( int i = 0; i < press_dirs.length(); i++ )
    {
        str.Format( _T( "%.4f\n" ), press_dirs[i] );

        AcGeVector3d v = press_epts[i] - press_spts[i];
        AcGePoint3d pt = press_spts[i] + v * 0.5;
        str.AppendFormat( _T( "%.4f\t%.4f\t%.4f\n" ), pt.x, pt.y, pt.z );
        outfile.WriteString( str );
    }

    outfile.Close();
}

void WriteAirLeakDataFile( const CString& filepath,
                           const AcGePoint3dArray al_pts,
                           const AirLeakDataArray& al_datas )
{
    CStdioFile outfile;
    outfile.Open( filepath, CFile::modeCreate | CFile::modeWrite );

    CString str;
    str.Format( _T( "%d\n" ), al_pts.length() );
    outfile.WriteString( str );

    for( int i = 0; i < al_pts.length(); i++ )
    {
        AcGePoint3d pt = al_pts[i];
        str.Format( _T( "%.4f\t%.4f\t%.4f\t" ), pt.x, pt.y, pt.z );

        AirLeakData data = al_datas[i];
        str.AppendFormat( _T( "%.4f\n" ), data.q );

        outfile.WriteString( str );
    }
    outfile.Close();
}

void WriteGasPipeDataFile( const CString& filepath,
                           const AcGePoint3dArray& gas_pipe_pts,
                           const GasPipeDataArray& gas_pipe_datas )
{
    CStdioFile outfile;
    outfile.Open( filepath, CFile::modeCreate | CFile::modeWrite );

    CString str;
    str.Format( _T( "%d\n" ), gas_pipe_pts.length() );
    outfile.WriteString( str );

    for( int i = 0; i < gas_pipe_pts.length(); i++ )
    {
        AcGePoint3d pt = gas_pipe_pts[i];
        str.Format( _T( "%.4f\t%.4f\t%.4f\t" ), pt.x, pt.y, pt.z );

        GasPipeData data = gas_pipe_datas[i];
        str.AppendFormat( _T( "%.4f\t%.4f\t%.4f\n" ), data.q, data.ch4, data.o2 );

        outfile.WriteString( str );
    }

    outfile.Close();
}

void WriteNitrogenPipeDataFile( const CString& filepath,
                                const AcGePoint3dArray& n2_pipe_pts,
                                const NitrogenPipeDataArray& n2_pipe_datas )
{
    CStdioFile outfile;
    outfile.Open( filepath, CFile::modeCreate | CFile::modeWrite );

    CString str;
    str.Format( _T( "%d\n" ), n2_pipe_pts.length() );
    outfile.WriteString( str );

    for( int i = 0; i < n2_pipe_pts.length(); i++ )
    {
        AcGePoint3d pt = n2_pipe_pts[i];
        str.Format( _T( "%.4f\t%.4f\t%.4f\t" ), pt.x, pt.y, pt.z );

        NitrogenPipeData data = n2_pipe_datas[i];
        str.AppendFormat( _T( "%.4f\n" ), data.n2 );

        outfile.WriteString( str );
    }

    outfile.Close();
}

void WriteObturationDataFile( const CString& filepath,
                              const AcGePoint3dArray& ob_pts,
                              const ObturationDataArray& ob_datas )
{
    CStdioFile outfile;
    outfile.Open( filepath, CFile::modeCreate | CFile::modeWrite );

    CString str;
    str.Format( _T( "%d\n" ), ob_pts.length() );
    outfile.WriteString( str );

    for( int i = 0; i < ob_pts.length(); i++ )
    {
        AcGePoint3d pt = ob_pts[i];
        str.Format( _T( "%.4f\t%.4f\t%.4f\t" ), pt.x, pt.y, pt.z );

        ObturationData data = ob_datas[i];
        str.AppendFormat( _T( "%.4f\t%.4f\t%.4f\n" ), data.q, data.ch4, data.o2 );

        outfile.WriteString( str );
    }

    outfile.Close();
}