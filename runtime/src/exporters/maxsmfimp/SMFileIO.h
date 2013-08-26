/***************************************************************************
                         SMFileIO.h  -  description
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

#include <string>
#include <fstream>
#include <vector>

class SMFile {
public:
	typedef struct {
		float x,y,z;
	} TVertex;
	typedef struct {
		unsigned int a,b,c;
	} TTriangle;

	SMFile();
	
	void loadData(const std::string &fname);
	unsigned int getVerticesCount();
	unsigned int getTrianglesCount();
	TVertex getVertex(unsigned int i);
	TTriangle getTriangle(unsigned int i);

	void writeData(const std::string &fname);
	void addVertex(const TVertex &v);
	void addTriangle(const TTriangle &t);

protected:
	typedef std::vector<TVertex> VertexVector;
	typedef std::vector<TTriangle> TriangleVector;

	VertexVector mVertices;
	TriangleVector mTriangles;
	std::fstream mFile;
};
