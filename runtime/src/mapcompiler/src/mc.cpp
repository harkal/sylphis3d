/***************************************************************************
    copyright            : (C) 2002 by Vassili Tenente
                           (C) 2002-2005 Harry Kalogirou (harkal@gmail.com)
  ***************************************************************************
    This file is part of "Sylphis3D Game Engine".

    "Sylphis3D Game Engine" is free software; you can redistribute it and/or 
    modify it under the terms of the GNU General Public License as 
    published by the Free Software Foundation; either version 2 of the 
    License, or (at your option) any later version, 
    plus a special exception:
      Linking "Sylphis3D Game Engine" statically or dynamically with other 
    modules is making a combined work based on this library. Thus, the 
    terms and conditions of the GNU General Public License cover the whole 
    combination. As a special exception, the copyright holders of this 
    library give you permission to link this library with independent 
    modules to produce an executable, regardless of the license terms of 
    these independent modules, and to copy and distribute the resulting 
    executable under terms of your choice, provided that you also meet, 
    for each linked independent module, the terms and conditions of the 
    license of that module. An independent module is a module which is 
    not derived from or based on this program. If you modify this program,
    you may extend this exception to your version of the library, but you 
    are not obligated to do so. If you do not wish to do so, delete this 
    exception statement from your version.

    "Sylphis3D Game Engine" is distributed in the hope that it will be 
    useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with "Sylphis3D Game Engine"; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
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

