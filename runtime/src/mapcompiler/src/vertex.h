/***************************************************************************
                          vertex.h  -  description
                             -------------------
    begin                : Fri Jun 7 2002
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
 ***************************************************************************/

#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "vector.h"

class CVertex {
public: 
	CVertex();
	CVertex(const float &x, const float &y, const float &z);
	CVertex(const CVector3 & realCoor){
		real_coor = realCoor;
		rvId = -1;
	}
	~CVertex(){};

	CVector3 & getRealCoor(void){
		return real_coor;
	};
	const CVector3 & getRealCoor(void) const {
		return real_coor;
	};
	void setRealCoor(const CVector3 & realCoor);

	float getRealX(void) const;
	float getRealY(void) const;
	float getRealZ(void) const;
	float &getXYZ(const int &xyz);

	void setRealX(const float &x);
	void setRealY(const float &y);
	void setRealZ(const float &z);
	void setXYZ(const int &index, const float &xyz);

	void setU(const float u);
	void setV(const float v);
	float getUV(int i) const;
	float getU() const ;
	float getV() const ;
	float* getTheUV();

	//are used for Graph creation at tjucntions
	bool isIn(){return (rvId != -1);};
	int pvId(){ return rvId; }
	void setpvId(const int id){rvId = id;}
	void print();

private:
	CVector3 real_coor;
	float UV[2];
	int rvId;
};

#endif 
