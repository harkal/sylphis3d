/***************************************************************************
                          exception.cpp  -  description
                             -------------------
    begin                : Thu Mar 14 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@sylphis3d.com
 ***************************************************************************/

#include <exception.h>
using namespace std;

#include <platform/types.h>

CException::CException() : runtime_error("") {
	mWhat = "";
}
CException::CException(const std::string &s) : runtime_error(s) {
	mWhat = s;
}

CException::CException(const string &s, const S8 *file, S32 line)
:runtime_error(s), mFileName(file), mLineNumber(line)
{
    S8 sline[128];
    sprintf(sline, "%d", line);
	mWhat = s + " at " + file + sline;
}

std::string CException::getExplanation(){
	return mWhat;
}

const char *CException::what() const throw () {
    return mWhat.c_str();
}
