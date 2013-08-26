/***************************************************************************
                             vfs.h  -  description
                             -------------------
    begin                : Wed Jun 15 2005
    copyright            : (C) 2005 by Harry Kalogirou
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
#ifndef _VFS_H_
#define _VFS_H_

#include <platform/types.h>
#include <iostream>
#include <vector>
#include <list>
using namespace std;

class CVirtualFS;

///
/// The basic file class. Used to access files in the filesystem
///
class CFile {
public:
    enum eFileMode {
        FILE_WRITE,
        FILE_READ
    };

	CFile();
	CFile(const string &fname, eFileMode mode = FILE_READ);
	CFile(const string &fname, CVirtualFS &v, eFileMode mode = FILE_READ);
	virtual ~CFile();

    /// Sets the filesystem the file will use
	void        setVirtualFS(CVirtualFS &fs);
    /// Returns the filesystem used by the file
	CVirtualFS& getVirtualFS();

    /// Sets the file size
	virtual void            setSize(U32 s);
    /// Returns the file size
	virtual U32    getSize();

    /// Get the file name
	string& getName();
    /// Set the file name
	void    setName(const string &fname);
    /// Remove the extension from the file name
	void    chopExtention();

    /// Load all the data from the file system
    void    loadData();
    /// Get a pointer to the data in memory
	void*   getData();
    /// Get a pointer to a private copy of the data in memory.
    /// You are responsible for freeing this memory
    void*   getDataCopy();
    /// Free the allocated data
	void    freeData();
	
	void            read(S8 **d, S32 *len);
    void            read(S8 *d, S32 len);
    S16           readShort();
    U16  readUnsignedShort();
    S8            readChar();
    U8   readUnsignedChar();
    S32             readInt();
    U32    readUnsignedInt();
    F32           readFloat();
	F64			readDouble();
    bool            readBool();
    string          readString();

    void    write(const S8 *data, S32 len);
	void    writeShort(const S16 value);
	void    writeUnsignedShort(U16 value);
	void    writeChar(const S8 value);
	void    writeUnsignedChar(const U8 value);
    void    writeInt(const S32 value);
	void    writeUnsignedInt(const U32 value);
	void    writeFloat(const F32 value);
	void	writeDouble(const F64 value);
	void    writeBool(const bool value);
	void    writeString(const string &value);
    
    /// Set a flag to specify if this file is a directory
    void    setIsDirectory(bool b);
    /// Returns true if the file is a directory
    bool    isDirectory();

    void        setStream(iostream &stream);
    iostream*   getStream() const;

    void        setMode(eFileMode mode);
    eFileMode   getMode();

	static  string chopExtention(const string &name);

protected:
    S32             pos;

private:
	string          name;
	U32    size;
	void*           data;
	CVirtualFS*     vfs;
    
    bool            mIsDirectory;
    eFileMode       mMode;
    iostream*       mStream;
};

class CFileText : public CFile {
public:
	CFileText() : CFile() {};
	CFileText(const string &fname) : CFile(fname) {};
	virtual void            setSize(U32 s);
	virtual U32    getSize();

	string readline(S32 size = 0);
    string read(S32 size = 0);
};

class CFileCollection {
	friend class CVirtualFS;
public:
	enum ColType {
		NativeCollection,
		QuakePAK,
		Directory
	};
	CFileCollection(){};

	void readFile(CFile &file);
private:
	string	    colPath;
	string	    mMountPoint;
	ColType	    col_type;
	FILE*       h;
	void*       zh;
	S32         filesnum;
};

class CVirtualFS {
public:
	typedef vector<CFile> FindResult;

	void startup();
	void setBaseDir(const string &basedir);
	void addCollection(const string &fname);
	void addCollection(CFileCollection &col);
	void mount(const string &mpoint, const string &target);
	void findFiles(const string &path, const string &extension, FindResult &fr);

    void    readFile(CFile &file);
    CFile*  createFile(const string &name);
    CFile*  openFile(const string &name);

	void setDebug(bool d);
	bool getDebug();
private:
	string			appendBaseDir(const string &filename);
	typedef list<CFileCollection> CollectionList;

	CollectionList  fileCollections;
	bool            mDebug;
	string			mBaseDir;
};

extern CVirtualFS gfs;

#endif

