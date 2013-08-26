/***************************************************************************
                          tokenizer.h  -  description
                             -------------------
    begin                : Thu Mar 14 2002
    copyright            : (C) 2002 by Harry Kalogirou
    email                : harkal@gmx.net
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


#include <stdio.h>

#include "exception.h"
using namespace std;

class CTokenizerBase
{
  public:
    class CTokenizerException:public CException
    {
      public:
        CTokenizerException(CTokenizerBase & t, const string & b)
        {
            char s[256];
              sprintf(s, "line %d : %s", t.getLine(), b.c_str());
              what = s;
        };
    };
    CTokenizerBase(const string & b);
	virtual ~CTokenizerBase(){};

    virtual bool nextToken();
    virtual void expect(const string & token);
    static string lowerCase(string & a);
    void setLowerCaseTokens(bool b);
    void skipToNextLine();
    void skipToToken(const string & token);
    void backToStartOfLine();
    void skipWhitespace();
    void nextChar();
    char currentChar()
    {
        return buffer[pos];
    };
    string & getToken();
    int getLine();
  protected:
    bool lowerCaseTokens;
    int pos;
    int line;
    bool endOfStream;
    string token;
    string buffer;
};

class CTokenizer:public CTokenizerBase
{
  public:
    class CSeparator
    {
      public:
        CSeparator(const string & s):sep(s)
        {
        };
        string sep;
    };
    CTokenizer(const string & b):CTokenizerBase(b)
    {
    };

	virtual ~CTokenizer(){};

    virtual bool nextToken();

    void addSeparator(const CSeparator & s);
    void addSeparator(const string & s);
  protected:
    vector < CSeparator > separators;
};

class CTokenizerNoComments:public CTokenizer
{
  public:
    CTokenizerNoComments(const string & b):CTokenizer(b)
    {
        addSeparator("//");
    };
	virtual ~CTokenizerNoComments(){};
    virtual bool nextToken();
};


#endif
