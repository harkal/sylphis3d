

// Export the current state of ODE to a PSCL (Physics SCripting Language) file.
// -James Dolan.

#ifndef _ODE_EXPORT_PSC_
#define _ODE_EXPORT_PSC_

#include <ode/common.h>

// Export the current state of a World/Space pair to a PSCL file.
void dExportPSC(dWorldID world, dSpaceID space, FILE *file);

#endif