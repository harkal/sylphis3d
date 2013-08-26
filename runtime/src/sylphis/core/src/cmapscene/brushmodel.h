/***************************************************************************
                          brushmodel.h  -  description
                             -------------------
    begin                : ??? ??? 4 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef BRUSHMODEL_H
#define BRUSHMODEL_H

#include <md3model.h>
#include <vector>
#include <renderable.h>

class CMAP;

SmartPointer(CBrushModel);
class CBrushModel : public CMD3Model  {
public: 
	CBrushModel();
	virtual ~CBrushModel();

	void loadBrushEntity(CMAP &cmap, S32 i);

	virtual void load(const std::string &n, CVirtualFS *vfs);
};

#endif
