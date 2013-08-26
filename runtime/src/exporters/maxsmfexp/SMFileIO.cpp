/***************************************************************************
                         SMFileIO.cpp  -  description
                             -------------------
    begin                : 5 Jan 2004
    copyright            : (c) 2004 by Harry Kalogirou
    email                : harkal@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <iostream>
#include "SMFileIO.h"

using namespace std;

SMFile::SMFile(){
}

void SMFile::loadData(const string &fname){
	mFile.open(fname.c_str());
	mVertices.clear();
	mTriangles.clear();
	while(!mFile.eof()){
		string s;
		mFile >> s;
		if (s[0] == '#'){
			continue;
		}
		else if(s == "v"){
			TVertex v;
			mFile >> v.x;
			mFile >> v.y;
			mFile >> v.z;
			addVertex(v);
		}
		else if(s == "f"){
			TTriangle t;
			mFile >> t.a;
			mFile >> t.b;
			mFile >> t.c;
			t.a--;
			t.b--;
			t.c--;
			addTriangle(t);
		}
	}
	mFile.close();
}

unsigned int SMFile::getVerticesCount(){
	return (unsigned int)mVertices.size();
}

unsigned int SMFile::getTrianglesCount(){
	return (unsigned int)mTriangles.size();
}

SMFile::TVertex SMFile::getVertex(unsigned int i){
	return mVertices[i];
}

SMFile::TTriangle SMFile::getTriangle(unsigned int i){
	return mTriangles[i];
}

void SMFile::writeData(const string &fname){
	mFile.open(fname.c_str(), ios::out | ios::trunc );

	mFile << "# Vertices : " << mVertices.size() << endl;
	mFile << "# Faces    : " << mTriangles.size() << endl;

	VertexVector::iterator v;
	for(v = mVertices.begin() ; v != mVertices.end() ; ++v){
		mFile << " v ";
		mFile << (*v).x << " " << (*v).y << " " << (*v).z << " " << (*v).u << " " << (*v).v << endl;
	}
	mFile << endl;
	TriangleVector::iterator t;
	for(t = mTriangles.begin() ; t != mTriangles.end() ; ++t){
		mFile << " f ";
		mFile << (*t).a + 1 << " " << (*t).b + 1 << " " << (*t).c + 1 << endl;
	}
	mFile << endl;
	mFile.close();
}

void SMFile::addVertex(const SMFile::TVertex &v){
	mVertices.push_back(v);
}

void SMFile::addTriangle(const SMFile::TTriangle &t){
	mTriangles.push_back(t);
}
