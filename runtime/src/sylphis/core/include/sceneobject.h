/***************************************************************************
                          sceneobject.h  -  description
                             -------------------
    begin                : Mon Jun 3 2002
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

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <quaternion.h>
#include <vector.h>
#include <pointer.h>
#include <boundbox.h>
#include <interpolator.h>

class CRenderer;
class CSceneManager;
SmartPointer(CSceneNode);

class CPathFollower {
public:
    virtual void followPath(CInterpolatorVector3 &path, F32 startAt = 0.0, F32 loopAt = 0.0);
    virtual void stopFollowPath();

    virtual void lookAtPath(CInterpolatorVector3 &path, F32 startAt = 0.0, F32 loopAt = 0.0);
    virtual void stopLookAtPath();

    virtual void updatePath(F64 time);
    virtual void updateLookAtPath(F64 time);
protected:
    CInterpolatorVector3Ptr mPath;
    F32 mPathPosition;
    F32 mPathLoopPosition;

    CInterpolatorVector3Ptr mLookAtPath;
    F32 mLookAtPathPosition;
    F32 mLookAtPathLoopPosition;
};

SmartPointer(CSceneObject);
class CSceneObject : public CPathFollower, public CRefObject  {
DEFINE_SERIALIZABLE(CSceneObject);
public: 
	CSceneObject();
	CSceneObject(CSceneManager *sm);
	virtual ~CSceneObject();

    void setSceneManager(CSceneManager *sm);
    CSceneManager *getSceneManager();

	void setParent(CSceneNode *node);
	CSceneNode *getParent();

	virtual void setOrientation(const CQuaternion &q);
	virtual void setOrientation(const F32 x, const F32 y, const F32 z, const F32 w);
	CQuaternion getOrientation() const;

    virtual void lookAt(const CVector3 &pos, bool localSpace = false);

	virtual void setPosition(const CVector3 &p);
	virtual void setPosition(const F32 x, const F32 y, const F32 z);
	virtual CVector3 &getPosition();

    bool passClosedPortals();
    void setPassClosedPortals(bool p);


	virtual CQuaternion getDerivedOrientation() const;
	virtual CVector3 getDerivedPosition() const;

	CMatrix4 getTransformMatrix();

    CAABoundBox &getBound();
    CAABoundBox &getDerivedBound();

	void move(const CVector3 &v);
	void moveLocal(const CVector3 &v);

	void rotate(const CQuaternion &q);
	void rotate(const CVector3 &axis, F32 rad);

	void roll(F32 rad);
	void yaw(F32 rad);
	void pitch(F32 rad);

	virtual void updateData(F64 time);
	virtual void update(F64 time, bool startedHere = true);
    void propagateBoundToParent();
    virtual void updateBound();

    virtual void onRender(CRenderer *renderer);
    virtual void render(CRenderer *renderer);

	virtual void postRenderUpdate(CRenderer *renderer);

	enum eFlags {
		NOCULL = 1,
		NOSHADOW = 2,
		NORENDER = 4
	};
	void setFlags(S32 flags){mFlags = flags;};
	S32 getFlags(){return mFlags;};

	void *getExtraData(){ return mExtraData; }
	/* Set by the scenemanager to include extra data it might
     * need. The pointer given here will be deleted when the
	 * object destructs or registers to another scenemanager
	 */
	void setExtraData(void *data){ mExtraData = data; }

    virtual void save(CSerializer *ser) const;
    virtual void load(CSerializer *ser);

protected:
    CSceneManager* mSceneManager;
	CSceneNode* mParent;

	CQuaternion mOrientation;
	CVector3 mPosition;

	CQuaternion mDerivedOrientation;
	CVector3 mDerivedPosition;

    CAABoundBox mBound;
    CAABoundBox mDerivedBound;
    bool mPassClosedPortals;

	void *mExtraData;
	S32 mFlags;

    CQuaternion mLastOrientation;
	CVector3 mLastPosition;

    // Updater staff
    
};

#endif
