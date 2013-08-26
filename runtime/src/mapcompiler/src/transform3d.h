/***************************************************************************
                          transform3d.h  -  description
                             -------------------
    begin                : Sun Jan 12 2003
    copyright            : (C) 2003 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
 ***************************************************************************/

#ifndef _TRANSFORM3D_H_
#define _TRANSFORM3D_H_

namespace MYNAME {

#include "vector.h"
#include <math.h>



float tcos, tsin;

void calcCosSin(const float &angle)
{
    tcos=cos(angle);
    tsin=sin(angle);
}
//WARNING : to work these functions down here you should call the calcCosSin first
//with the angle you want to have rotation

//X Rotation
CVector3 getRotatedX(const CVector3 &v)
{
    return CVector3(v.x, v.y*tcos-v.z*tsin, v.y*tsin+v.z*tcos);
}

void rotateX(CVector3 &v, const float &angle){
    calcCosSin(angle);
    v=getRotatedX(v);
}

//Y Rotation
CVector3 getRotatedY(const CVector3 &v)
{
    return CVector3(v.x*tcos+v.z*tsin, v.y, v.z*tcos-v.x*tsin);
}

void rotateY(CVector3 &v, const float &angle){
    calcCosSin(angle);
    v=getRotatedY(v);
}

//Z Rotation
CVector3 getRotatedZ(const CVector3 &v)
{
    return CVector3(v.x*tcos-v.y*tsin, v.x*tsin+v.y*tcos, v.z);
}

void rotateZ(CVector3 &v, const float &angle){
    calcCosSin(angle);
    v=getRotatedZ(v);
}

//ONE AXE ROTATIONS WITH ANGLES
//X 
CVector3 getRotatedX(const CVector3 &v, const float &angle)
{
    calcCosSin(angle);
    return CVector3(v.x, v.y*tcos-v.z*tsin, v.y*tsin+v.z*tcos);
}

void rotateX(CVector3 &v){    
    v=getRotatedX(v);
}

//Y Rotation
CVector3 getRotatedY(const CVector3 &v, const float &angle)
{
    calcCosSin(angle);
    return CVector3(v.x*tcos+v.z*tsin, v.y, v.z*tcos-v.x*tsin);
}

void rotateY(CVector3 &v){
    v=getRotatedY(v);
}

//Z Rotation
CVector3 getRotatedZ(const CVector3 &v, const float &angle)
{
    calcCosSin(angle);
    return CVector3(v.x*tcos-v.y*tsin, v.x*tsin+v.y*tcos, v.z);
}

void rotateZ(CVector3 &v){
    v=getRotatedZ(v);
}

//These functions do NOT need the calcCosSin calling
//
//XY Rotation
//
CVector3 getRotatedXY(const CVector3 &v, const float &angleX, const float &angleY)
{
    calcCosSin(angleX);
    CVector3 rv=getRotatedX(v);
    calcCosSin(angleY);
    return getRotatedY(rv);
}

void rotateXY(CVector3 &v, const float &angleX, const float &angleY)
{
    calcCosSin(angleX);
    rotateX(v);
    calcCosSin(angleY);
    rotateY(v);
}

//
//XZ Rotation
//
CVector3 getRotatedXZ(const CVector3 &v, const float &angleX, const float &angleZ)
{
    calcCosSin(angleZ);
    CVector3 rv=getRotatedX(v);
    calcCosSin(angleZ);
    return getRotatedZ(rv);
}

void rotateXZ(CVector3 &v, const float &angleX, const float &angleZ)
{
    calcCosSin(angleX);
    rotateX(v);
    calcCosSin(angleZ);
    rotateZ(v);
}

//
//YZ Rotation
//
CVector3 getRotatedYZ(const CVector3 &v, const float &angleY, const float &angleZ)
{
    calcCosSin(angleY);
    CVector3 rv=getRotatedY(v);
    calcCosSin(angleZ);
    return getRotatedZ(rv);
}

void rotateYZ(CVector3 &v, const float &angleY, const float &angleZ)
{
    calcCosSin(angleY);
    rotateY(v);
    calcCosSin(angleZ);
    rotateZ(v);
}

//
//XYZ Rotation
//
CVector3 getRotatedXYZ(const CVector3 &v, const float &angleX, const float &angleY, const float &angleZ)
{
    calcCosSin(angleX);
    CVector3 rv=getRotatedX(v);
    calcCosSin(angleY);
    rv=getRotatedY(rv);
    calcCosSin(angleZ);
    return getRotatedZ(rv);
}

void rotateXYZ(CVector3 &v, const float &angleX, const float &angleY, const float &angleZ)
{
    calcCosSin(angleX);
    rotateX(v);
    calcCosSin(angleY);
    rotateY(v);
    calcCosSin(angleZ);
    rotateZ(v);
}

}

#endif //_TRANSFORM3D_H_//



