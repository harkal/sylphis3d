/***************************************************************************
                          boundbox.h  -  description
                             -------------------
    begin                : Sun Jun 9 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef BOUNDBOX_H
#define BOUNDBOX_H

#include "vector.h"

#include <bound.h>
#include <serialize.h>

class CPlane;

class CAABoundBox : public CBound {
DEFINE_SERIALIZABLE(CAABoundBox);
public: 
	CAABoundBox();
	CAABoundBox(const CVector3 &v);
	virtual ~CAABoundBox();

    void setBounds(const CVector3 &min, const CVector3 &max);
    void setMin(const CVector3 &min);
    CVector3 &getMin();
    void setMax(const CVector3 &max);
    CVector3 &getMax();

	void addPoint(const CVector3 &v);
    void addBound(CAABoundBox &bbox);

    virtual F32 distance(const CPlane &p) const;
    virtual F32 distance(const CVector3 &v) const;
    bool isBehind(const CPlane &p);

    virtual void translate(const CVector3 &v);

	virtual EType getType(){ return AABOX; };
	virtual bool collide(const CVector3 &v);
    virtual bool collide(CBound *b);

	virtual void render(CRenderer *r);

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);

protected:
    CVector3 mMin;
    CVector3 mMax;
};

#define NOT_SUPPORTED assert(0);

#endif
