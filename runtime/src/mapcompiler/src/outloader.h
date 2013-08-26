/***************************************************************************
                          outloader.h  -  description
                             -------------------
    begin                : Tue Dec 31 2002
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
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


#ifndef _OUTLOADER_H_
#define _OUTLOADER_H_

#include "bspwrite.h"
#include "tree.h"
#include "polygon.h"
#include <stdio.h>
#include <errno.h>

template < class T > class COutLoader {

public:
        T *data;

        int size;
        FILE *f;
        COutLoader(int lumpNo, string fileName, bool closeFile=true){
            f = fopen(fileName.c_str(), "r");
            if(!f){
              cout <<" Error reading file: "<<strerror(errno)<<endl;
              return;
            }
            cout << "opened file " << fileName<<endl;
            fseek(f, sizeof(BspOFile::tBSPHeader)+sizeof(BspOFile::tBSPLump)*(lumpNo), SEEK_SET);
            BspOFile::tBSPLump *lump=new BspOFile::tBSPLump();
            fread(lump,sizeof(BspOFile::tBSPLump), 1, f);

            size=lump->length/sizeof(T);
            //data = new T[size];
            
            char *theBytes = new char[lump->length];
            fseek(f, lump->offset, SEEK_SET);
            fread(theBytes,1, lump->length, f);
            
            cout << lump->length <<" lump->length"<< endl;
            cout << lump->offset <<" lump->offset"<< endl;            
            data = (T*)theBytes;
            cout << "read " <<size << " things :)"<<endl;
            if(closeFile){
                fclose(f);
                cout<<"  \nclosing file"<<endl;
            }
        }
        
        COutLoader(int lumpNo, FILE *file, bool closeFile=true)
        {
            f=file;
            fseek(f, sizeof(BspOFile::tBSPHeader)+sizeof(BspOFile::tBSPLump)*(lumpNo), SEEK_SET);
            BspOFile::tBSPLump *lump=new BspOFile::tBSPLump();
            fread(lump,sizeof(BspOFile::tBSPLump), 1, f);

            size=lump->length/sizeof(T);
            char *theBytes = new char[lump->length];
            fseek(f, lump->offset, SEEK_SET);
            fread(theBytes,1, lump->length, f);
            
            data = (T*)theBytes;
            
            cout << lump->length <<" lump->length"<< endl;
            cout << lump->offset <<" lump->offset"<< endl;            
  
            cout << "read " <<size << " things :)"<<endl;
            if(closeFile){
                fclose(f);
                cout<<"  \nclosing file"<<endl;
            }
        }
        ~COutLoader(){}
};
#endif

