
#include <fstream>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <vfs.h>
#include <console.h>
#include <exception.h>
#include <unzip.h>

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <io.h>
#else
	#include <dirent.h>
#endif

#include <assert.h>

CVirtualFS gfs;

string fixFileName(const string &name){
    //gcon.printString(name + "\n");
	if(name[0] != '/')
		return "/" + name;
	return name;
}

static string fixSlashes(const string &name){
	string nname = name;
    size_t i = 0;
	while((i = nname.find('\\', i)) != string::npos){
		nname.replace(i, 1, "/");
	}
	return nname;
}

CFile::CFile() : name(""), size(0), data(0), vfs(&gfs), pos(0),
        mIsDirectory(false), mMode(FILE_READ), mStream(0) {};

CFile::CFile(const string &fname, eFileMode mode) : size(0), data(0), vfs(&gfs), pos(0),
        mIsDirectory(false), mMode(mode), mStream(0) {
	setName(fname);
    if(mMode == FILE_WRITE){
        fstream *f = new fstream(fname.c_str(), fstream::out);
        setStream(*f);
    }
};

CFile::CFile(const string &fname, CVirtualFS &v, eFileMode mode) : size(0), data(0), pos(0),
    mIsDirectory(false), mMode(mode), mStream(0) {
	setName(fname);
	setVirtualFS(v);
};

CFile::~CFile(){
	if(mStream){
        mStream->flush();
        delete mStream;
	}
	freeData();	
}

void CFile::setVirtualFS(CVirtualFS &fs){
    vfs = &fs;
}

CVirtualFS &CFile::getVirtualFS() {
    return *vfs;
}

void CFile::setSize(U32 s) {
	size = s;
	data = realloc(data, size);
}

U32 CFile::getSize() {
    return size;
}

string &CFile::getName() {
    return name;
}

void CFile::setName(const string &fname){
    name = fixFileName(fname);
    if(name[name.size() - 1] == '/'){
        name.erase(name.size()-1, 1);
    }
}

void *CFile::getData() {
    return data;
}

void *CFile::getDataCopy() {
    void *p = malloc(getSize());
    memcpy(p, getData(), getSize());
    return p;
}

void CFile::loadData(){
	if(!vfs)return;
	
	vfs->readFile(*this);
    pos = 0;
}

void CFile::freeData() {
	if(getData()){
		free(data);
		data = 0;
	}
}

void CFile::chopExtention(){
    setName(CFile::chopExtention(getName()));
}

string CFile::chopExtention(const string &name){
	size_t i = name.rfind(".");
	if(i != string::npos){
		string s;
		return s.assign(name, 0, i);
	} else {
		return name;
	}
}

void CFile::read(S8 **d, S32 *len){
	*d = (S8 *)data;
	*len = getSize();
}

void CFile::read(S8 *d, S32 len){
    memcpy(d, (S8 *)data + pos, len);
    pos += len;
}

S8 CFile::readChar(){
    S8 d;
    read((S8 *)&d, sizeof(S8));
    return d;
}

U8 CFile::readUnsignedChar(){
    U8 d;
    read((S8 *)&d, sizeof(U8));
    return d;
}

S16 CFile::readShort(){
    S16 d;
    read((S8 *)&d, sizeof(S16));
    return d;
}

U16 CFile::readUnsignedShort(){
    U16 d;
    read((S8 *)&d, sizeof(U16));
    return d;
}

S32 CFile::readInt(){
    S32 d;
    read((S8 *)&d, sizeof(S32));
    return d;
}

U32 CFile::readUnsignedInt(){
    U32 d;
    read((S8 *)&d, sizeof(U32));
    return d;
}

F32 CFile::readFloat(){
    F32 d;
    read((S8 *)&d, sizeof(F32));
    return d;
}

F64 CFile::readDouble(){
    F64 d;
    read((S8 *)&d, sizeof(F64));
    return d;
}

bool CFile::readBool(){
    return readChar() ? true : false;
}

string CFile::readString(){
    string s;
    S32 len = readInt();
    if(len < 0)return "";
    for(S32 i = 0 ; i < len ; i++){
        s += readChar();
    }
    return s;
}

void CFile::write(const S8 *data, S32 len){
    if(!mStream)
        return;
    mStream->write(data, len);
	pos = mStream->tellp();
}

void CFile::writeShort(const S16 value){
    write((S8 *)&value, sizeof(value));
}
void CFile::writeUnsignedShort(U16 value){
    write((S8 *)&value, sizeof(value));
}
void CFile::writeChar(const S8 value){
    write((S8 *)&value, sizeof(value));
}
void CFile::writeUnsignedChar(const U8 value){
    write((S8 *)&value, sizeof(value));
}
void CFile::writeInt(const S32 value){
    write((S8 *)&value, sizeof(value));
}
void CFile::writeUnsignedInt(const U32 value){
    write((S8 *)&value, sizeof(value));
}
void CFile::writeFloat(const F32 value){
    write((S8 *)&value, sizeof(value));
}
void CFile::writeDouble(const F64 value){
	write((S8 *)&value, sizeof(value));
}
void CFile::writeBool(const bool value){
    writeChar(value);
}
void CFile::writeString(const string &value){
    writeInt(value.size());
    write(value.c_str(), value.size());
}

void CFile::setIsDirectory(bool b){
    mIsDirectory = b;
}

bool CFile::isDirectory(){
    return mIsDirectory;
}

void CFile::setStream(iostream &stream){
    mStream = &stream;
}

iostream* CFile::getStream() const {
    return mStream;
}

void CFile::setMode(eFileMode mode){
    mMode = mode;
}

CFile::eFileMode CFile::getMode(){
    return mMode;
}

void CFileText::setSize(U32 s){
	s++;
	CFile::setSize(s);
	*((S8 *)getData() + s -1) = 0;
	pos = 0;
};

U32 CFileText::getSize(){
    return CFile::getSize() - 1;
};

string CFileText::readline(S32 size){
	S8 t[512];

	S32 i = 0;
	while(*((S8 *)getData() + pos)){
		t[i] = *((S8 *)getData() + pos);
		if(t[i] == '\r'){
			pos++;
			continue;
		}
		else if(t[i] == '\n'){
			i++;
			pos++;
			break;
		}
		pos++;
		i++;
	}
	t[i] = 0;

	return string(t);
}

string CFileText::read(S32 size){
    if(size == 0){
        //return string((S8 *)getData() + pos);
        string s;
        while(pos < getSize()){
            s += *((S8 *)getData() + pos++);
        }
        return s;
    } else {
        string s;
        S32 len = 0;
        while((len < size) && (pos < getSize())){
            if((*((S8 *)getData() + pos) != 0))
                len = len;
            s += *((S8 *)getData() + pos++);
            len++;
        }
        return s;
    }
}

void CFileCollection::readFile(CFile &file){
	unz_file_info info;
	U32 size;
	string name;
	switch(col_type){
	case NativeCollection:
		name = fixSlashes(file.getName());
		if(name[0] == '/')
			name.erase(name.begin());
		if (unzLocateFile(zh, name.c_str(), 2) == UNZ_OK){
			if (unzOpenCurrentFile(zh) == UNZ_OK){
				if (unzGetCurrentFileInfo(zh, &info, NULL, 0, NULL, 0, NULL, 0)!= UNZ_OK)
					return;
				size = info.uncompressed_size;
				file.setSize(size);
				if((U32)unzReadCurrentFile(zh, file.getData(), size) != size){
					unzCloseCurrentFile(zh);
					file.freeData();
					return;
				}
				unzCloseCurrentFile(zh);
				return;
			}
        } else {
            string fname = name + "/";
            if (unzLocateFile(zh, fname.c_str(), 2) == UNZ_OK){
                CVirtualFS::FindResult fr;
                Unz_GetStringForDir((unzFile*)zh, fname, "", fr);
                string data;
                CVirtualFS::FindResult::iterator i;
                for(i = fr.begin();i != fr.end();i++){
                    data += (*i).getName();
                    data += "\n";
                }
                data += "\0";
                file.setSize(data.size());
                memcpy(file.getData(), data.c_str(), data.size());
                file.setIsDirectory(true);
                return;
            }
        }
		break;
	case Directory:
		S32 i = 0;
		FILE *fp = 0;
		U32 size;

		string fullName = file.getName();
		i = fullName.find(mMountPoint, 0);
		if(i == string::npos){
			return;
		}
		fullName.replace(i, mMountPoint.size(), "");
		if(fullName[0] == '/')
			fullName.replace(0, 1, "");

		fullName = colPath + fullName;

#ifdef _WIN32
		struct	_finddata_t fileinfo;

		string pattern = fullName + "/*.*";

		S32 hd = _findfirst(pattern.c_str(), &fileinfo);
		if(hd != -1){
			// It is a directory
			string data("");
			do {
				data += fullName + "/" + fileinfo.name + "\n";
			} while (_findnext(hd, &fileinfo) != -1);
			data += "\0";
			file.setSize(data.size());
			memcpy(file.getData(), data.c_str(), data.size());
			file.setIsDirectory(true);
			_findclose(hd);
			return;
		}

#else
		//assert(0 && "NOT IMPLEMENTED DIRECTORY CHECK");
		DIR *dirp;
		struct dirent *dp;

		dirp = opendir(fullName.c_str());
		if(dirp){
			string data("");
			while( (dp = readdir(dirp)) != NULL ){
				if(*dp->d_name == '.')
					continue;
				data += fullName + "/" + dp->d_name + "\n";
			}
			data += "\0";
			file.setSize(data.size());
			memcpy(file.getData(), data.c_str(), data.size());
			file.setIsDirectory(true);
			closedir(dirp);
			return;
		}

#endif
#ifdef _WIN32
		while((i = fullName.find('/', i)) != string::npos){
			fullName.replace(i, 1, "\\");
		}
#endif

		if (!fp) fp = fopen(fullName.c_str(), "rb"); 
		if (fp) { 
			fseek(fp, 0, SEEK_END); 
			size = ftell(fp); 
			fseek(fp, 0, SEEK_SET);
			file.setSize(size);
			if (file.getData())
				fread(file.getData(), 1, size, fp); 
			fclose(fp);
			return;
		}
	}
}

void CVirtualFS::startup(){
	setDebug(false);
}

void CVirtualFS::setBaseDir(const string &basedir){
	mBaseDir = fixSlashes(basedir);
	if(mBaseDir[mBaseDir.size() - 1] == '/'){
		mBaseDir = mBaseDir.substr(0, mBaseDir.size() - 1);
	}
}

string CVirtualFS::appendBaseDir(const string &filename){
	if(filename[0] == '/'){
		return mBaseDir + filename;
	} else {
		return mBaseDir + "/" + filename;
	}
}

void CVirtualFS::addCollection(CFileCollection &col){
	fileCollections.push_front(col);
}

void CVirtualFS::addCollection(const string &fname){
	CFileCollection fileCol;
	string fullName = fname;

	if(getDebug())
		gcon.printf("CVirtualFS::addCollection(%s)\n", fname.c_str());

	unzFile *col = (unzFile *)unzOpen(fullName.c_str());
	if(col){
		if(getDebug())
			gcon.printf("CVirtualFS : Adding %s \n",fullName.c_str());
		fileCol.colPath = fname;
		fileCol.col_type = CFileCollection::NativeCollection;
		fileCol.zh = col;
		addCollection(fileCol);
		return;
	}	
	return;
}

#include <sys/types.h>

void CVirtualFS::readFile(CFile &file){

	if(getDebug())
		gcon.printf("CVirtualFS::readFile('%s')\n", file.getName().c_str());

	{ // Search all the collection files...
		CollectionList::iterator col;
		file.freeData();
		for(col = fileCollections.begin() ; col != fileCollections.end() ; col++){
			col->readFile(file);
			if(file.getData())return;
		}
		if(getDebug())
			gcon.printf("CVirtualFS::readFile('%s') not found\n", file.getName().c_str());
		throw CException("File not found : " + file.getName());
	} 
	
}

void CVirtualFS::mount(const string &mpoint, const string &target){
	CFileCollection fileCol;

	if(mpoint == "" || target == "")
		return;
	
	fileCol.mMountPoint = fixFileName(mpoint);
	if(target[target.size() - 1] == '/'){
		fileCol.colPath = target;
	} else {
		fileCol.colPath = target + "/";
	}
	fileCol.col_type = CFileCollection::Directory;
	addCollection(fileCol);

	if(getDebug())
		gcon.printf("CVirtualFS::mount(%s, %s)\n", fileCol.mMountPoint.c_str(), fileCol.colPath.c_str());
}

void CVirtualFS::findFiles(const string &path, const string &extension, FindResult &fr){
	S32 i;
#ifdef _WIN32
	struct	_finddata_t fileinfo;
#else
	struct	dirent **entlist, *ent;
#endif

	CollectionList::iterator col;

	for(col = fileCollections.begin() ; col != fileCollections.end() ; col++){
		if(col->col_type == CFileCollection::QuakePAK){		
		} else 
		if(col->col_type == CFileCollection::NativeCollection){
            string fullPath = path;
            {S32 i=0;
				while((i = fullPath.find('\\', i)) != string::npos){
					fullPath.replace(i, 1, "/");
			}}
			Unz_GetStringForDir((unzFile*)col->zh, fullPath, extension, fr);
		} else 
			if(col->col_type == CFileCollection::Directory){
				string fullPath = fixFileName(path);
				i = fullPath.find(col->mMountPoint, 0);
				if(i != 0){
					return;
				}
				fullPath.replace(0, col->mMountPoint.size(), "");
				if(fullPath[0] == '/')
					fullPath.replace(0, 1, "");

				fullPath = col->colPath + fullPath;
#ifdef _WIN32	
				fullPath += "*." + extension;
				{S32 i=0;
				while((i = fullPath.find('/', i)) != string::npos){
					fullPath.replace(i, 1, "\\");
				}}
				S32 hd;
				hd = _findfirst(fullPath.c_str(), &fileinfo);
				if(hd != -1){
					do {
						fr.push_back(CFile(path + fileinfo.name));
					} while (_findnext(hd, &fileinfo) != -1);
					_findclose(hd);
				}
#else
				S32 numfiles = scandir(fullPath.c_str(), &entlist, NULL, NULL);
				for(S32 i = 0 ; i < numfiles ; i++){
					ent = entlist[i];
					// Skip hidden
					if(ent->d_name[0]=='.')continue;

					// Keep requested files
					if(strncmp(extension.c_str(),
						ent->d_name + strlen(ent->d_name) - extension.length(),
						extension.length()))continue;

					fr.push_back(CFile(path + ent->d_name));
				}
#endif		
			}
	}
}

CFile* CVirtualFS::createFile(const string &name){
    CFile *file = new CFile(name, CFile::FILE_WRITE);
    file->setMode(CFile::FILE_WRITE);

    fstream *stream = new fstream();
	stream->open(appendBaseDir(name).c_str(), ios_base::out | ios_base::trunc | ios_base::binary);

    file->setStream(*stream);

    return file;
}

CFile* CVirtualFS::openFile(const string &name){
    CFile *file = new CFile(name);
    file->loadData();

    return file;
}

void CVirtualFS::setDebug(bool d){
	mDebug = d;
}

bool CVirtualFS::getDebug(){
	return mDebug;
}

