/***************************************************************************
                        modelmanager.h  -  description
                             -------------------
    begin                : Wed Dec 11 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
  ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

	Copyright (c) 2013 Charilaos Kalogirou.
	All rights reserved.

	Redistribution and use in source and binary forms are permitted
	provided that the above copyright notice and this paragraph are
	duplicated in all such forms and that any documentation,
	advertising materials, and other materials related to such
	distribution and use acknowledge that the software was developed
	by Charilaos Kalogirou. The name of the
	Charilaos Kalogirou may not be used to endorse or promote products derived
	from this software without specific prior written permission.
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include <string>
#include <resourcemanager.h>

class CModel;
class CSkelModel;
class CMD3Model;
class CSMFModel;
class CSkeleton;
class CMesh;
class CSkelAnimation;
class CShader;

class CModelManager : public CResourceManager {
public:
    CModelManager(void);
    ~CModelManager(void);

    CModel *registerModel(CModel *model);
    CMD3Model *registerMD3Model(const std::string fname);
	CSMFModel *registerSMFModel(const std::string fname);
	CSkelModel *registerSkelModel(const std::string fname);
    CModel *findModel(const std::string name);

    CSkeleton *registerSkeleton(CSkeleton *skeleton);
    CSkeleton *registerSkeleton(const std::string fname);
    CSkeleton *findSkeleton(const std::string name);

    CMesh *registerMesh(CMesh *mesh);
    CMesh *registerMesh(const std::string fname);
    CMesh *findMesh(const std::string name);

    CSkelAnimation *registerSkelAnimation(CSkelAnimation *mesh);
    CSkelAnimation *registerSkelAnimation(const std::string fname);
    CSkelAnimation *findSkelAnimation(const std::string name);
};


#endif
