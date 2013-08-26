/***************************************************************************
                          renderablequeuemember.h  -  description
                             -------------------
    begin                : Thu Jun 6 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
   ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 ***************************************************************************/

#ifndef RENDERABLEQUEUEMEMBER_H
#define RENDERABLEQUEUEMEMBER_H

#include "matrix.h"
#include <boundbox.h>
#include "quaternion.h"
#include <renderable.h>
#include "pointer.h"



SmartPointer(CRenderableQueueMember);
class CRenderableQueueMember : public CRefObject {
public: 
	CRenderableQueueMember();
	virtual ~CRenderableQueueMember();

	void setOrientation(const CQuaternion &q);
	CQuaternion &getOrientation();

	void setPosition(const CVector3 &p);
	CVector3 &getPosition();

    void setBounds(const CAABoundBox &bbox);
    CAABoundBox &getBounds();

	CMatrix4 getTransformMatrix() const;
	CMatrix4 getInverseTransformMatrix() const;

    void setLastTransform(const CQuaternion &q, const CVector3 &v);
    CMatrix4 getLastTransformMatrix() const;

    void setRenderable(CRenderable *r);
    CRenderable *getRenderable();

    void setNoShadows(bool b);
    bool getNoShadows() const;
private:
	CQuaternion mOrientation;
	CVector3 mPosition;
    CAABoundBox mBound;
    CRenderablePtr mRenderable;
    CQuaternion mLastOrientation;
	CVector3 mLastPosition;
    bool mNoShadows;
};

#endif
