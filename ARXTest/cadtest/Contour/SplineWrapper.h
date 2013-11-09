// splinewrapper.h

#ifndef __SPLINEWRAPPER_H__
#define __SPLINEWRAPPER_H__

#include "spline.h"

struct Position {float x, y, z; };
enum CurveType {CATMULLROM, BEZIER, KOCHANEKBARTELS, CARDINAL};

class SplineWrapper
{
public:
	inline SplineWrapper(Spline::ControlPoint* controlPoints,
	                     unsigned int amount, CurveType type);
	inline ~SplineWrapper();

	inline bool     isBeginReached() const;
	inline bool     isEndReached()   const;
	inline Position getPosition()    const;
	inline void     move(float p);
	inline float    getLength()      const;

private:
	float mPosition;

	Spline::Curve* mCurve;
};

// -----------------------------------------------------------------------------
inline SplineWrapper::SplineWrapper(Spline::ControlPoint* controlPoints,
				    unsigned int amount, CurveType type)
	: mPosition(0)
{
	switch(type)
	{
	case CATMULLROM:
		mCurve = new Spline::CatmullRom(controlPoints, amount);
		break;
	case BEZIER:
		mCurve = new Spline::Bezier(controlPoints, amount);
		break;
	case KOCHANEKBARTELS:
		mCurve = new Spline::KochanekBartels(controlPoints, amount, 0, 0, 2.0f);
		break;
	case CARDINAL:
		mCurve = new Spline::Cardinal(controlPoints, amount, 1.0f);
		break;
	}
}

// -----------------------------------------------------------------------------
inline SplineWrapper::~SplineWrapper()
{
	delete mCurve;
}

// -----------------------------------------------------------------------------
inline bool SplineWrapper::isBeginReached() const
{
	return (mPosition <= 0);
}

// -----------------------------------------------------------------------------
inline bool SplineWrapper::isEndReached() const
{
	return (mPosition >= mCurve->getLength());
}

// -----------------------------------------------------------------------------
inline void SplineWrapper::move(float p)
{
	mPosition += p;

	if(mPosition > mCurve->getLength())
		mPosition = mCurve->getLength();
	else if(mPosition < 0)
		mPosition = 0;
}

// -----------------------------------------------------------------------------
inline Position SplineWrapper::getPosition() const
{
	float p[3];

	mCurve->getWorldPosition(p, mPosition);

	Position position;
	position.x = p[0];
	position.y = p[1];
	position.z = p[2];

	return position;
}

// -----------------------------------------------------------------------------
inline float SplineWrapper::getLength() const
{
	return mCurve->getLength();
}

#endif

