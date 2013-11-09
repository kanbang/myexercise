#ifndef RS_GEO_H
#define RS_GEO_H

#include <CGAL/Origin.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Point_2.h>
#include <CGAL/Vector_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/Ray_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Direction_2.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Aff_transformation_2.h>

#define RS_ORIGIN CGAL::ORIGIN
typedef CGAL::Cartesian<double> Kernel;
typedef CGAL::Point_2<Kernel> RS_Point2D;
typedef CGAL::Vector_2<Kernel> RS_Vector2D;
typedef CGAL::Line_2<Kernel> RS_Line2D;
typedef CGAL::Ray_2<Kernel> RS_Ray2D;
typedef CGAL::Segment_2<Kernel> RS_Segment2D;
typedef CGAL::Circle_2<Kernel> RS_Circle2D;
typedef CGAL::Direction_2<Kernel> RS_Direction2D;
typedef CGAL::Bbox_2 RS_Bbox2D;
typedef CGAL::Aff_transformation_2<Kernel> RS_Transform2D;

#endif // RS_GEO_H