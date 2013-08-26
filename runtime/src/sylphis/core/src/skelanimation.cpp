/***************************************************************************
                         skelanimation.cpp  -  description
                             -------------------
    begin                : Fri Dec 13 2002
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

#include <skelanimation.h>
#include <vfs.h>
#include <exception.h>

CSkelAnimation::CSkelAnimation(){

}

CSkelAnimation::~CSkelAnimation(){

}

void CSkelAnimation::addBoneTrack(CBoneTrack *bt){
    mBoneTracks.push_back(bt);
}

void CSkelAnimation::setDuration(F32 t){
    mDuration = t;
}

const S32 CSkelAnimation::EARLIEST_COMPATIBLE_FILE_VERSION = 699;
const S32 CSkelAnimation::CURRENT_FILE_VERSION = 910;

void CSkelAnimation::load(const std::string &n, CVirtualFS *vfs){
    setName(n);

    CFile f(n);
    vfs->readFile(f);

    if(f.readChar() != 'C' || f.readChar() != 'A' || f.readChar() != 'F' || f.readChar() != 0)
        throw CException("Not skeleton file");

    S32 version = f.readInt();
    if(version < EARLIEST_COMPATIBLE_FILE_VERSION || version > CURRENT_FILE_VERSION)
		throw CException("File version not supported");

    F32 duration = f.readFloat();
    if(duration < 0.0)
        throw CException("File corrupt");

    setDuration(duration);

    S32 numTracks = f.readInt();
    if(numTracks < 0)
        throw CException("File corrupt");

    for(S32 i = 0 ; i < numTracks ; i++){
        CBoneTrackPtr tr = new CBoneTrack();
        tr->load(&f);
        addBoneTrack(tr);
    }
}

//
// ********* Skeleton Controler ***********
//

CBoneControler::CBoneControler(){

}

CBoneControler::~CBoneControler(){

}

void CBoneControler::getAt(F32 time, CVector3 &trans, CQuaternion &rot){
    trans = mTranslation;
    rot = mRotation;
}

void CBoneControler::setTranslation(const CVector3 &t){
    mTranslation = t;
}

const CVector3& CBoneControler::getTranslation() const {
    return mTranslation;
}

void CBoneControler::setRotation(const CQuaternion &q){
    mRotation = q;
}

const CQuaternion& CBoneControler::getRotation() const {
    return mRotation;
}


CSkelControler::CSkelControler(){

}

CSkelControler::~CSkelControler(){

}



