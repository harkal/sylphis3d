/***************************************************************************
                        waitable.cpp  -  description
                             -------------------
    begin                : Sun Oct 19 2003
    copyright            : (C) 2003 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <python/mthread.h>
#include <python/waitable.h>

CWaitable::CWaitable(){

}

CWaitable::~CWaitable(){
	clearThreads();	
}

void CWaitable::wait(){
	PyObject * thread = CMThread::getSingleton()->getCurrentThread();
	mThreads.push_back(thread);
}

void CWaitable::wake(){
	ThreadVector::iterator i;

	for(i = mThreads.begin() ; i != mThreads.end() ; ++i){
		CMThread::getSingleton()->wakeUp(*i);
	}

	clearThreads();
}

void CWaitable::clearThreads(){
	ThreadVector::iterator i;

	for(i = mThreads.begin() ; i != mThreads.end() ; ++i){
		Py_DECREF(*i);
	}

	mThreads.clear();
}
