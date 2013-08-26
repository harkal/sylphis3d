/***************************************************************************
                           bound.h  -  description
                             -------------------
    begin                : Sun Jan 12 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef BOUND_H
#define BOUND_H

#include <vector.h>
#include <serialize.h>

class CAABoundBox;
class CRenderer;
class CPlane;

class CBound : public CSerializable {
public:
	enum EType {
		AABOX,
		FRUSTUM
	};
	CBound(){};
	virtual ~CBound(){};
	virtual EType getType() = 0; 

	virtual F32 distance(const CPlane &p) const = 0;
    virtual F32 distance(const CVector3 &v) const = 0;

    virtual void translate(const CVector3 &v) = 0;
    virtual bool collide(CBound *b) = 0;
	virtual bool collide(const CVector3 &v){return distance(v) <= 0.0;}

	virtual void render(CRenderer *r){};
};

#endif
