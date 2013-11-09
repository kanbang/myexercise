#include "JournalHelper.h"
#include <algorithm>

bool operator==( const CFD_Edge& e1, const CFD_Edge& e2 )
{
    return ( e1.s == e2.s ) && ( e1.t == e2.t );
}

JournalHelper::JournalHelper( void )
{
}

JournalHelper::~JournalHelper( void )
{
    for( FaceArray::iterator itr = m_faces.begin(); itr != m_faces.end(); ++itr )
    {
        delete *itr;
    }
    m_faces.clear();
}

void JournalHelper::addPoint( const CFD_Point& pt )
{
    m_vertices.push_back( pt );
}

void JournalHelper::addEdge( const CFD_Edge& e )
{
    if( std::find( m_edges.begin(), m_edges.end(), e ) == m_edges.end() )
    {
        m_edges.push_back( e );
    }
}

int JournalHelper::addFace()
{
    m_faces.push_back( new IntArray() );
    return m_faces.size() - 1;
}

void JournalHelper::addEdgeToFace( int e, int face_id )
{
    if( e < 0 || face_id < 0 || face_id >= ( int )m_faces.size() ) return;

    IntArray* pFace = m_faces[face_id];
    if( pFace->end() == std::find( pFace->begin(), pFace->end(), e ) )
    {
        pFace->push_back( e );
    }
}

void JournalHelper::write( const char* filename )
{
    OutFileStream outfile( filename );
    if( !outfile ) return;

    // 写入节点
    writeVertices( outfile );

    // 写入分支
    writeEdges( outfile );

    // 写入面
    writeFaces( outfile );

    outfile.close();
}

void JournalHelper::writeVertex( OutFileStream& outfile, const CFD_Point& pt )
{
    outfile << "vertex create coordinates "
            << pt.x << " "
            << pt.y << " "
            << pt.z << "\n";
}

void JournalHelper::writeEdge( OutFileStream& outfile, const CFD_Edge& e )
{
    outfile << "edge create straight "
            << "\"vertex." << e.s << "\" "
            << "\"vertex." << e.t << "\n";
}

void JournalHelper::writeFace( OutFileStream& outfile, IntArray& face )
{
    outfile << "face create wireframe ";
    for( IntArray::iterator itr = face.begin(); itr != face.end(); ++itr )
    {
        outfile << "\"edge." << *itr << "\" ";
    }
    outfile << "real\n";
}

void JournalHelper::writeVertices( OutFileStream& outfile )
{
    for( PointArray::iterator itr = m_vertices.begin(); itr != m_vertices.end(); ++itr )
    {
        writeVertex( outfile, *itr );
    }
}

void JournalHelper::writeEdges( OutFileStream& outfile )
{
    for( EdgeArray::iterator itr = m_edges.begin(); itr != m_edges.end(); ++itr )
    {
        writeEdge( outfile, *itr );
    }
}

void JournalHelper::writeFaces( OutFileStream& outfile )
{
    for( FaceArray::iterator itr = m_faces.begin(); itr != m_faces.end(); ++itr )
    {
        writeFace( outfile, *( *itr ) );
    }
}