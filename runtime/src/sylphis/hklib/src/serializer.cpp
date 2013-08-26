/***************************************************************************
                         serializer.cpp  -  description
                             -------------------
    begin                : Sat Jun 11 2005
    copyright            : (C) 2005 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <serializer.h>
#include <serialize.h>
#include <vfs.h>

using namespace std;

CSerializer::CSerializer(){

}

CSerializer::~CSerializer(){

}

void CSerializer::setFile(CFile *f){
    mFile = f;
}

CFile *CSerializer::getFile(){
    return mFile;
}


S16 CSerializer::readShort(){
    return mFile->readShort();
}
U16 CSerializer::readUnsignedShort(){
    return mFile->readUnsignedShort();
}
S8 CSerializer::readChar(){
    return mFile->readChar();
}
U8 CSerializer::readUnsignedChar(){
    return mFile->readUnsignedChar();
}
S32 CSerializer::readInt(){
    return mFile->readInt();
}
U32 CSerializer::readUnsignedInt(){
    return mFile->readUnsignedInt();
}
F32 CSerializer::readFloat(){
    return mFile->readFloat();
}
F64 CSerializer::readDouble(){
    return mFile->readDouble();
}
bool CSerializer::readBool(){
    return mFile->readBool();
}
std::string CSerializer::readString(){
    return mFile->readString();
}

void* CSerializer::readPointer(){
	void* d;
    mFile->read((S8 *)&d, sizeof(void*));
    return d;
}

CSerializable* CSerializer::readSerializable(){
	string classname = readString();
	void* id = readPointer();

	if(classname == "__ref"){	
		if(!id)
			return 0;
		return mReadObjects[id];
	} else {
		CSerializable* obj;
		obj = CClassManager::getSingleton()->factory(classname);
		mReadObjects[id] = obj;

		obj->load(this);
		return obj;
	}
}

void CSerializer::writeShort(const S16 value){
    mFile->writeShort(value);
}
void CSerializer::writeUnsignedShort(U16 value){
    mFile->writeUnsignedShort(value);
}
void CSerializer::writeChar(const S8 value){
    mFile->writeChar(value);
}
void CSerializer::writeUnsignedChar(const U8 value){
    mFile->writeUnsignedChar(value);
}
void CSerializer::writeInt(const S32 value){
    mFile->writeInt(value);
}
void CSerializer::writeUnsignedInt(const U32 value){
    mFile->writeUnsignedInt(value);
}
void CSerializer::writeFloat(const F32 value){
    mFile->writeFloat(value);
}
void CSerializer::writeDouble(const F64 value){
    mFile->writeDouble(value);
}
void CSerializer::writeBool(const bool value){
    mFile->writeBool(value);
}
void CSerializer::writeString(const string &value){
    mFile->writeString(value);
}

void CSerializer::writePointer(const void *value){
	mFile->write((S8 *)&value, sizeof(void *));
}

void CSerializer::writeSerializable(const CSerializable *obj) {
	if(!obj){
		writeString("__ref");
		writePointer((void *)0);
		return;
	}

	if(mWriteObjects.find((void *)obj) == mWriteObjects.end()){
		mWriteObjects[(void*)obj] = obj;
		writeString(obj->getClassName());
		writePointer(obj);
		obj->save(this);
	} else {
		writeString("__ref");
		writePointer(obj);
	}
}

