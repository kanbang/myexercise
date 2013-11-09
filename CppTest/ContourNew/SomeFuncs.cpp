// SomeFuncs.cpp: implementation of the CSomeFuncs class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "contour.h"
#include "SomeFuncs.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSomeFuncs::CSomeFuncs()
{

}

CSomeFuncs::~CSomeFuncs()
{

}

UINT CSomeFuncs::Relation_Of_Lines( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, UINT UINTRelation )
{
    int xx0, xx1, yy0, yy1;
    int a0, a1, a2, a3;

    // float k0, k1;	//�����߶ε�б��
    // k0 = (y1-y0)/(x1-x0);
    // k1 = (y3-y2)/(x3-x2);
    // if (k0 == k1) ...
    // Ϊ���ų��������������С���㾫���������ó˷�������
    //////////////////////////////////////////////////////
    if ( ( y1 - y0 ) * ( x3 - x2 ) == ( x1 - x0 ) * ( y3 - y2 ) ) //ƽ��
    {
        //������ƽ���߶�֮��Ĵ�ֱ���룺
        //   ͨ������ڶ����߶��ϵ�һ���˵㵽��һ��ֱ�ߵľ�����á�
        //   �����һ��ֱ�ߵķ���ΪAx+By+C=0, ���(x2,y2)��ֱ�ߵľ���
        //          |Ax2+By2+C|
        //		d = ------------
        //          ��(A^2+B^2)
        //    ����ֱ����A,B,C
//		A = y0-y1;
//		B = x1-x0;
//		C = x0*y1-x1*y0;
        //   ���d!=0,������ƽ����û���ص���d=0,���ص�
        //   Ҫ�ж�d�Ƿ�Ϊ0��ֻ��Ҫ�ж�Ax2+By2+C�Ƿ�Ϊ0��
        if( ( y0 - y1 )*x2 + ( x1 - x0 )*y2 + ( x0 * y1 - x1 * y0 ) != 0 )	//���벻Ϊ0
        {
            UINTRelation |= ROL_PARA_DISTANCE;
            return UINTRelation;
        }
        else //����Ϊ0
        {
            //Ҫ�ж������߶��Ƿ��ص������м����ֻ��Ҫ�ж����ǵĶ˵�
            //    ��˳���ϵ�����ǵ�ֱ�߿���ƽ����x���y�ᣬ���Ǹ���б��
            //    ��ȷ��ѡ�ö˵��x���껹����y������ȷ���˵������˳��
            //              |y0-y1|
            //	   ��� k = -------- > 1, ��ѡ��y����ֵ��Ϊ�������ݣ� ����...
            //              |x0-x1|
            double dy = y0 - y1;
            double dx = x0 - x1;
            if( fabs( dy ) > fabs( dx ) ) //ѡ��y
            {
                if( y0 > y1 )
                {
                    a0 = y1;  //��һ���߶�
                    a1 = y0;
                }
                else
                {
                    a0 = y0;
                    a1 = y1;
                }
                if( y2 > y3 )
                {
                    a2 = y3;  //�ڶ����߶�
                    a3 = y2;
                }
                else
                {
                    a2 = y2;
                    a3 = y3;
                }
            }
            else //ѡ��x
            {
                if( x0 > x1 )
                {
                    a0 = x1;
                    a1 = x0;
                }
                else
                {
                    a0 = x0;
                    a1 = x1;
                }
                if( x2 > x3 )
                {
                    a2 = x3;
                    a3 = x2;
                }
                else
                {
                    a2 = x2;
                    a3 = x3;
                }
            }
            //�����һ���߶ε���С�˵���ڵڶ����߶ε����˵����
            //    ��һ���߶ε����˵�С�ڵڶ����߶ε���С�˵㣬�������߶��м�϶
            if ( a0 > a3 || a1 < a2 )
            {
                UINTRelation |= ROL_PARA_NOOVERNODISTANCE;
                return UINTRelation;
            }
            else if( a0 == a3 || a1 == a2 )
            {
                UINTRelation |= ROL_PARA_LINK;
                return UINTRelation;
            }
            else
            {
                UINTRelation |= ROL_PARA_OVERLAP;
                return UINTRelation;
            }
        }
    }
    else	//	��ƽ��
    {
        if ( ( x0 == x2 && y0 == y2 ) || ( x0 == x3 && y0 == y3 ) || ( x1 == x2 && y1 == y2 ) || ( x1 == x3 && y1 == y3 ) )
        {
            UINTRelation |= ROL_NOPARA_BOTHLINES;
            return UINTRelation;
        }

        //��һ��ֱ�ߵķ���Ϊ(x0-x1)*y=(y0-y1)*x+(x0-x1)*y1-(y0-y1)*x1,
        //    �����(x2,y2)��������ֱ�߷��̲���x2��[x0,x1]֮����ߵ�(x3,y3)...
        //    �򽻵��ڵ�һ���߶�֮�䲢���ڵڶ����߶εĶ˵�
        if ( x0 < x1 )
        {
            xx0 = x0;
            xx1 = x1;
        }
        else
        {
            xx0 = x1;
            xx1 = x0;
        }
        if ( y0 < y1 )
        {
            yy0 = y0;
            yy1 = y1;
        }
        else
        {
            yy0 = y1;
            yy1 = y0;
        }
        if( ( ( ( ( x0 - x1 )*y2 ) == ( ( y0 - y1 )*x2 + ( x0 - x1 )*y1 - ( y0 - y1 )*x1 ) ) && x2 >= xx0 && x2 <= xx1 && y2 >= yy0 && y2 <= yy1 ) ||
                ( ( ( ( x0 - x1 )*y3 ) == ( ( y0 - y1 )*x3 + ( x0 - x1 )*y1 - ( y0 - y1 )*x1 ) ) && x3 >= xx0 && x3 <= xx1 && y3 >= yy0 && y3 <= yy1 ) )
        {
            UINTRelation |= ROL_NOPARA_FIRSTLINE;	//�����ڵ�һ���߶���
            return UINTRelation;
        }

        //�����ڵڶ����߶���
        if ( x2 < x3 )
        {
            xx0 = x2;
            xx1 = x3;
        }
        else
        {
            xx0 = x3;
            xx1 = x2;
        }
        if ( y2 < y3 )
        {
            yy0 = y2;
            yy1 = y3;
        }
        else
        {
            yy0 = y3;
            yy1 = y2;
        }
        if( ( ( ( ( x2 - x3 )*y0 ) == ( ( y2 - y3 )*x0 + ( x2 - x3 )*y2 - ( y2 - y3 )*x2 ) ) && x0 >= xx0 && x0 <= xx1 && y0 >= yy0 && y0 <= yy1 ) ||
                ( ( ( ( x2 - x3 )*y1 ) == ( ( y2 - y3 )*x1 + ( x2 - x3 )*y2 - ( y2 - y3 )*x2 ) ) && x1 >= xx0 && x1 <= xx1 && y1 >= yy0 && y1 <= yy1 ) )
        {
            UINTRelation |= ROL_NOPARA_SECONDLINE;	//�����ڵڶ����߶���
            return UINTRelation;
        }

        //ֻʣ�����������1���ڽ���2���ӳ����ཻ
        //����ֱ�߷���ʽy=A1*x+B1��y=A2*x+B2
        double A1, A2, B1, B2, x, y;
        A1 = ( ( y0 - y1 ) / ( double )( x0 - x1 ) );
        A2 = ( ( y3 - y2 ) / ( double )( x3 - x2 ) );
        B1 = ( y0 - ( ( y0 - y1 ) / ( double )( x0 - x1 ) ) * x0 );
        B2 = ( y2 - ( ( y3 - y2 ) / ( double )( x3 - x2 ) ) * x2 );
        x = ( B1 - B2 ) / ( A2 - A1 );	//���������
        y = A1 * ( ( B1 - B2 ) / ( A2 - A1 ) ) + B1;
        //��С�������е�һ��ֱ���ϵ�������
        if ( x0 < x1 )
        {
            xx0 = x0;
            xx1 = x1;
        }
        else
        {
            xx0 = x1;
            xx1 = x0;
        }
        if ( y0 < y1 )
        {
            yy0 = y0;
            yy1 = y1;
        }
        else
        {
            yy0 = y1;
            yy1 = y0;
        }
        if( x >= xx0 && x <= xx1 && y >= yy0 && y <= yy1 )
        {
            if ( x2 < x3 )
            {
                xx0 = x2;
                xx1 = x3;
            }
            else
            {
                xx0 = x3;
                xx1 = x2;
            }
            if ( y2 < y3 )
            {
                yy0 = y2;
                yy1 = y3;
            }
            else
            {
                yy0 = y3;
                yy1 = y2;
            }
            if( x >= xx0 && x <= xx1 && y >= yy0 && y <= yy1 )
            {
                UINTRelation |= ROL_NOPARA_INTERCROSS; //�ڽ�
                return UINTRelation;
            }
        }

    }
    UINTRelation |= ROL_NOPARA_NOCROSS;
    return UINTRelation;
}

UINT CSomeFuncs::Relation_Of_Lines( CPoint pt0, CPoint pt1, CPoint pt2, CPoint pt3, UINT UINTRelation )
{
    int x0, x1, x2, x3;
    int y0, y1, y2, y3;
    x0 = pt0.x;
    y0 = pt0.y;
    x1 = pt1.x;
    y1 = pt1.y;
    x2 = pt2.x;
    y2 = pt2.y;
    x3 = pt3.x;
    y3 = pt3.y;
    return Relation_Of_Lines( x0, y0, x1, y1, x2, y2, x3, y3, UINTRelation );
}



//��src������Ѱ�����е����ֵ����Сֵ��numberΪsrc��Ԫ�صĸ�����
//bhasinited��ʾmaxi��mini�Ƿ��Ѿ���ʼ����
void CSomeFuncs::SearchMaxMin( CArray<double, double>& src, double& maxi, double& mini, bool bHasInited )
{
    int number = src.GetSize();
    double* ptSrc = new double[number];
    for( int i = 0; i < number; i++ )
        ptSrc[i] = src[i];

    SearchMaxMin( ptSrc, number, maxi, mini, bHasInited );

    delete []ptSrc;

}

void CSomeFuncs::SearchMaxMin( double src[], int number, double& maxi, double& mini, bool bHasInited )
{
    //src��Ԫ�شӱ��0����number��1
    double maximum;
    double minimum;
    if( bHasInited )
    {
        maximum = maxi;
        minimum = mini;
    }
    else
    {
        maximum = src[0];
        minimum = src[0];
    }
    for( int i = 1 ; i < number; i++ )
    {
        if ( src[i] > maximum )
        {
            maximum = src[i];
            continue;
        }
        if ( src[i] < minimum )
        {
            minimum = src[i];
            continue;
        }
    }
    maxi = maximum;
    mini = minimum;
}





/*
'C TREND(Data, m, n, NT, IP)
'C  WHERE M SAMPLE POINS NUMBER
'C        N any POINT CONTAINS ELEMENTS NUMBER(X,Y,Z,ESTIMATED Z,DEVLATION)
'C        NT TREND SURFACE TIMES
'C  TREND SURFACE ANALYSIS   OCT.4,1991,DISIGNED BY ZZK
'C  REFERENCE THE BOOK OF PETROLEM MATHMATICS GEOLOGY,PETROLEUM  UNIVERCITY
'c  Zi=(Xi,yi) i=1,m
'c  z(x,y)=b1+b2*x+b3*y+b4*x*x+b5*x*y+b6*y*y+...+EPSON
'C
'C m
'C f = e ^ 2 = Sum(Zi - z(Xi, yi)) ^ 2
'C i = 1
'C
'c  df/db1=0,df/db2=0,...
'C
'C |SUM1   SUMX    SUMY    SUMXX     SUMX*Y    SUMYY    ...| |B1|  |SUMZ   |
'C |SUMX   SUMX^2  SUMXY   SUMX^3    SUMX^2Y   SUMXY^2  ...| |B2|  |SUMXZ  |
'C |SUMY   SUMXY   SUMY^2  SUMX^2Y   SUMXY^2   SUMY^3   ...| |B3|  |SUMYZ  |
'C |SUMX^2 SUMX^3  SUMX^2Y SUMX^4    SUMX^3Y   SUMX^2Y^2...| |B4|= |SUMX^2Y|
'C |SUMXY  SUMX^2Y SUMY^2  SUMX^3Y   SUMX^2Y^2 SUMXY^3  ...| |B5|  |SUMXYZ |
'C |SUMY^2 SUMXY^2 SUMY^3  SUMX^2Y^2 SUMXY^3   SUMY^4   ...| |B6|  |SUMY^2Z|
'C | .        .      .        .        .         .       . | |. |  |   .   |
'C | .        .      .        .        .         .       . | |. |  |   .   |
'C | .        .      .        .        .         .       . | |. |  |   .   |
'C                  MATRAX  A * MATRAX B = MATRAX C
'C
*/
// Sub tend(port_grid As Integer, port_scalar As Integer, port_vector As Integer,
//			m As Integer, n As Integer, dx As Single, dy As Single, i_a As Integer,
//			j_a As Integer, Rx As Double, Ry As Double, Rxy As Double,
//			Status As Integer)

////////////////////////
//�������Ϊ6����x,y,zֵ����Щֵ�����ڽṹ��ָ����,
//    ��Ŀ�ĵ��SPoint�ṹ������x,y��֪��Zδ֪.
//���ز���Ϊ�����zֵ
//  ****���õ�������������±�Ϊ1��ʼ.��basicת��������
double CSomeFuncs::tend( SPoint* srcPnt, SPoint& desPnt )
{
    const double eps1 = 0.0000001;
    const int M_a = 6;	//������
    const int NT = 2;	//����ʽ����ߴ���Ϊ���η�
    const int MM = 6;	//����a,b,c��ʵ�ʴ�С
    //MM��һͨ�����·��������
    //	   MM = 1;
    //	   for( i = 1; i<= NT; i++)
    //		   MM = MM + (i + 1);
    double a[MM + 1][MM + 1], b[MM + 1], c[MM + 1];
    double DATA[M_a + 1][6];	//���������ԣ�data(1,1)--x , data(1,2)--y, data(1,3)--z,
    //	data(1,4)--Z_c,data(1,5)--err_z
    // ע�⣬ֻ����DATA[1~6, 1~5]
    int Status = 1;
    int i, j, k, JB;

    //��ʼ��DATA��ֵ
    for( i = 1; i <= M_a; i++ )
    {
        DATA[i][1] = srcPnt[i - 1].x;
        DATA[i][2] = srcPnt[i - 1].y;
        DATA[i][3] = srcPnt[i - 1].z;
    }

    // SET UP COEFICINT MATRAX
    c[1] = 1.0;
    // C ARRAY IS USED AS TEMPRARY VARIATION
    // CALCULATE NUMBER OF COEFFICINTS
    // Reset
    for( i = 1; i <= MM; i++ )
    {
        b[i] = 0.0;
        // B ARRAY IS C ARRAY IN THE FORMULA
        for ( j = 1; j <= MM; j++ )
            a[i][j] = 0.0;
    }

    //      to create a matrix
    for( i = 1; i <= M_a; i++ ) //M_aΪ������
    {
        JB = 1;
        for( j = 1; j <= NT; j++ )
        {
            for( k = j; k >= 0; k-- )
            {
                JB = JB + 1;
                c[JB] = pow( DATA[i][1], k ) * pow( DATA[i][2], ( j - k ) );
            }
        }
        for( j = 1; j <= MM; j++ )
        {
            b[j] = b[j] + c[j] * DATA[i][3];
            for( k = 1; k <= MM; k++ )
                a[j][k] = a[j][k] + c[j] * c[k];
        }
    }

    Gauss( a, b, MM, Status ); //���ø�˹������ͬʱb��������

    // CALCULATE ESTIMATE VALUE DATA(I,4) AND DEVIATION  DATA(I,5) FOR ANY POINT
    for( i = 1; i <= M_a; i++ )
    {
        JB = 1;
        for ( j = 1; j <= NT; j++ )
            for( k = j; k <= 0; k-- )
            {
                JB = JB + 1;
                c[JB] = pow( DATA[i][1], k ) * pow( DATA[i][2], ( j - k ) );
            }

        DATA[i][4] = 0.0;
        for( j = 1; j <= MM; j++ )
            DATA[i][4] = DATA[i][4] + b[j] * c[j];
        DATA[i][5] = DATA[i][3] - DATA[i][4];
    }

    //���㺯������ֵ
    // z(x,y)=b1+b2*x+b3*y+b4*x*x+b5*x*y+b6*y*y+...+EPSON
    desPnt.z = ( b[1] + b[2] * desPnt.x + b[3] * desPnt.y + b[4] * desPnt.x * desPnt.x + b[5] * desPnt.x * desPnt.y + b[6] * desPnt.y * desPnt.y + eps1 );
    return desPnt.z;
}//end sub

//////////////////////////////////////////////////
//    Sub Gauss(a() As Double, b() As Double, n As Integer, Status As Integer)
//  ****���õ�������������±�Ϊ1��ʼ.��basicת��������
void CSomeFuncs::Gauss( double a[][7], double b[7], int n, int& Status ) //nΪ����a,b��ʵ�ʴ�С
{
    int i, j, k, I0, nm1, kp1, Ip1;
    const double eps = 0.00000001;
    double c, t;

    nm1 = n - 1;

    for( k = 1; k <= nm1; k++ )
    {
        c = 0.0;
        for( i = k; i <= n; i++ )
        {
            if( ( fabs( a[i][k] ) > fabs( c ) ) )
            {
                c = a[i][k];
                I0 = i;
            }
        }

        if( ( fabs( c ) < eps ) )
        {
            Status = -1;
            return;
        }

        if ( I0 != k )
        {
            for ( j = k; j <= n; j++ )
            {
                t = a[k][j];
                a[k][j] = a[I0][j];
                a[I0][j] = t;
            }

            t = b[k];
            b[k] = b[I0];
            b[I0] = t;
        }

        kp1 = k + 1;
        c = 1.0 / c;
        b[k] = b[k] * c;

        for( j = kp1; j <= n; j++ )
        {
            a[k][j] = a[k][j] * c;
            for( i = kp1; i <= n; i++ )
                a[i][j] = a[i][j] - a[i][k] * a[k][j];
            b[j] = b[j] - a[j][k] * b[k];
        }
    }//  Next k

    b[n] = b[n] / a[n][n];

    for( k = 1; k <= nm1; k++ )
    {
        i = n - k;
        c = 0.0;
        Ip1 = i + 1;
        for( j = Ip1; j <= n; j++ )
            c = c + a[i][j] * b[j];
        b[i] = b[i] - c;
    }
} // End Sub