#include "ode\ode.h"

/* Set/Get length */
void dGeomRaySetLength(dxGeom* g, dReal Length);
dReal dGeomRayGetLength(dxGeom* g);

/* Utility function to override the ray's pos + rot */
void dGeomRaySet(dxGeom* g, dVector3 Origin, dVector3 Direction);
void dGeomRayGet(dxGeom* g, dVector3 Origin, dVector3 Direction);
