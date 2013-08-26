/***************************************************************************
                          tokenizer.h  -  description
                             -------------------
    begin                : Thu Mar 14 2002
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

#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

#include <string>
#include <vector>
using namespace std;

#include <stdio.h>

#include <exception.h>
#include <pointer.h>

SmartPointer(CTokenizerBase);
class CTokenizerBase : public CRefObject {
public:
	CTokenizerBase(const string &b);
	
	virtual bool nextToken();
	virtual void expect(const string &token, bool nextAfter = false);
	static string lowerCase(const string &a);	
	void setLowerCaseTokens(bool b);
	void skipToNextLine();
	void skipToToken(const string &token);
    void bookmark();
    void gotoBookmark();
	void backToStartOfLine();
	void skipWhitespace();
	void nextChar();
	char currentChar(){ return buffer[pos]; };
	string getToken();
	S32 getLine();
protected:
	bool lowerCaseTokens;
	S32 pos;
    S32 bookmarkPos;
    S32 bookmarkLine;
	S32 line;
	bool endOfStream;
	string token;
	string buffer;
};

class CTokenizerException : public CException {
public:
	CTokenizerException(CTokenizerBase &t, const string &b) {
		char s[256];
		sprintf(s, "line %d : %s", t.getLine(), b.c_str());
		mWhat = s;
	};
};

class CSeparator {
public:
	CSeparator(const string &s) : sep(s) {};
	string sep;
};

class CTokenizer : public CTokenizerBase {
public:
	CTokenizer(const string &b) : CTokenizerBase(b) {};

	virtual bool nextToken();

	void addSeparator(const CSeparator &s);
	void addSeparator(const string &s);
protected:
	vector<CSeparator> separators;
};

class CTokenizerNoComments : public CTokenizer {
public:
	CTokenizerNoComments(const string &b) : CTokenizer(b) {
		addSeparator("//");
	};
	virtual bool nextToken();
};


#endif
