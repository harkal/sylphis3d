/***************************************************************************
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

#define __REVISION__ "$Rev: 938 $"

#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <stdio.h>

#ifdef MEMPROF
#include "mmgr.h"
#endif

#include "entity.h"
#include "polygon.h"
#include "loader.h"
#include "bspcreator.h"
#include "portal.h"
#include "bspwriter.h"
#include "timer.h"

using namespace std;

CMapLoader loader;
CBspCreator bsp;

CTimerAdvanced timer;

int main(int argc, char **argv){ 
	int rev;
	char dummy[64];
    bool neighbors;
	sscanf(__REVISION__, "%s %d", dummy, &rev);    
	cout << "Sylphis map compiler version 2.5";
#ifndef NDEBUG
#ifdef _DEBUG
    cout << " [Debug]";
#endif
#endif
	cout << " (r" << rev << ")" << endl;

	map<string, string> parameters;

	int c=0;
	string cParameter="mc";
	parameters[cParameter]="";
	while(c!=argc){
		if(argv[c][0] == '-'){
			cParameter = &argv[c][1];
			parameters[cParameter]="";
			++c;
			continue;
		}
		parameters[cParameter] +=&argv[c][0];
		++c;
	}

	string outFile="";

	timer.start();

	if(parameters.find("read")!=parameters.end()){
		outFile = parameters["read"];
		cout << "Reading from " << outFile << endl;      
		if (!loader.load(outFile))
			cout << "loader failed to open:"<<outFile << endl;

		CEntity* playerInfo = loader.getPlayerStart();
		if(!playerInfo){
			cout << "ERROR : No player start" << endl;
			exit(0);
		}

		bsp.setMapLoader(&loader);
		bsp.setPlaneManager(loader.getPlaneManager());

		if(parameters.find("noforcedetail")!=parameters.end())
			bsp.setForceDetail(false);
		else
			bsp.setForceDetail(true);

        if(parameters.find("noneighbors")!=parameters.end())
			neighbors = false;
		else
			neighbors = true;

		if(parameters.find("nobsp")==parameters.end()){
			bsp.buildBsp();
			double totalTime = timer.stop();
			cout << "\nTotal time : " << totalTime << " sec\n" << endl;
		}

		if(parameters.find("write")!=parameters.end()){
			outFile = parameters["write"];
		}

		if(parameters.find("nobsp")==parameters.end()) {    
			CBspWriter(&bsp, true, outFile);
		}else{
			CBspWriter writer( &bsp, false, outFile, neighbors);
			writer.rewriteEntities();        
		}
	} else {
		cout << "usage : mc -read mapfile [ -write [outputFile] ] | [ -nobsp ] | [ -view ]" << endl;
		exit(1);
	}

	return 1;
}

