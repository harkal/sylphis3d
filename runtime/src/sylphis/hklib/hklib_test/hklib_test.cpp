
#include "stdafx.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <serialize.h>
#include <serializer.h>
#include <refobject.h>
#include <pointer.h>
#include <vfs.h>

using namespace std;

SmartPointer(testser);
class testser : public CRefObject {
DEFINE_SERIALIZABLE(testser);
public:

    virtual void save(CSerializer *file) const {
        CRefObject::save(file);
        file->writeInt(mNum);
        cout << "testser save"<< endl;
    }

    virtual void load(CSerializer *file){
        CRefObject::load(file);
        mNum = file->readInt();
    }

    int mNum;
};
IMPLEMENT_SERIALIZABLE(testser);

SmartPointer(CMother);
class CMother : public CRefObject {
DEFINE_SERIALIZABLE(CMother);
public:

    virtual void save(CSerializer *file) const {
        CRefObject::save(file);
        file->writeInt(mNum);
		file->writeSerializable(child);
		file->writeSerializable(other);
        cout << "CMother save"<< endl;
    }

    virtual void load(CSerializer *file){
        CRefObject::load(file);
        mNum = file->readInt();
		child = (testser *)file->readSerializable();
		other = (CMother *)file->readSerializable();
    }

    int mNum;
	testser *child;
	CMother *other;
};

IMPLEMENT_SERIALIZABLE(CMother);

void testserialization(){
    CSerializer a,b;
    CFile *sf = gfs.createFile("test.save");
    a.setFile(sf);

    testser *r;
    r = dynamic_cast<testser *>(CClassManager::getSingleton()->factory("testser"));
    r->mNum = 10;

	CMother *mom1 = new CMother();
	CMother *mom2 = new CMother();

	mom1->mNum = 1;
	mom1->child = r;
	mom1->other = mom2;
	
	mom2->mNum = 2;
	mom2->child = r;
	mom2->other = mom1;
	
    
    cout << "about to wrire" << endl;
    a.writeSerializable(mom1);
    cout << "wrote" <<  endl;

    delete sf;

    cout << "File saved"<<endl;

#if 1
    CFile *f = gfs.openFile("test.save");
	f->loadData();
    b.setFile(f);
    CMother *m;
	m = (CMother *)b.readSerializable();

	if(m->child->mNum == 10){
        cout << "Serialization test passed... " << endl;
    } else {
        cout << "Serialization test FAILED!" << endl;
    }
#endif

}

int main(int argc, char* argv[])
{
    cout << "hklib test" << endl;

    gfs.mount("/", "./");

    CFile *testFile = gfs.createFile("test.txt");

    float fvalue = 0.43;

    testFile->writeFloat(fvalue);
    testFile->writeBool(false);
    testFile->writeString("test");
    testFile->writeInt(12345);

    delete testFile;

    testFile = gfs.openFile("test.txt");

    assert(testFile->readFloat() == fvalue);
    assert(!testFile->readBool());
    assert(testFile->readString() == "test");
    assert(testFile->readInt() == 12345);

    testserialization();

    return 0;
}

