/***************************************************************************
                        configfile.h  -  description
                             -------------------
    begin                : Mon Oct 7 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#ifndef _CONFIGFILE_H_
#define _CONFIGFILE_H_

#include <string>
#include <Python.h>

#include <platform/types.h>

class CConfigFile {
public:
	CConfigFile();
	~CConfigFile();

	void connect(const std::string &module, const std::string &object);
	S32 getAsInt(const std::string &var);
	F32 getAsFloat(const std::string &var);
protected:
	PyObject *getAttr(const std::string &attr);
	PyObject *mPyCfg;
};

extern CConfigFile gCfgFile;

#endif
