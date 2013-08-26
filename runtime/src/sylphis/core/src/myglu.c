/***************************************************************************
                          myglu.cpp  -  description
                             -------------------
    begin                : Wed Mar 20 2002
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

//
// Mesa GLU functions to avoid linking to glu lib
//
#include "globaldefs.h"
#include <platform/types.h>

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <assert.h>
#include <math.h>
#include <GL/gl.h>

#ifndef NULL
#define	NULL	0
#endif

//From glu.h
#define GLU_INVALID_ENUM        100900
#define GLU_INVALID_VALUE       100901
#define GLU_OUT_OF_MEMORY       100902
#define GLU_INCOMPATIBLE_GL_VERSION     100903

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

 

/* Pixel storage modes */
typedef struct {
   GLint pack_alignment;
   GLint pack_row_length;
   GLint pack_skip_rows;
   GLint pack_skip_pixels;
   GLint pack_lsb_first;
   GLint pack_swap_bytes;
   GLint pack_skip_images;
   GLint pack_image_height;

   GLint unpack_alignment;
   GLint unpack_row_length;
   GLint unpack_skip_rows;
   GLint unpack_skip_pixels;
   GLint unpack_lsb_first;
   GLint unpack_swap_bytes;
   GLint unpack_skip_images;
   GLint unpack_image_height;
} PixelStorageModes;

typedef union {
    unsigned char ub[4];
    U16 us[2];
    U32 ui;
    char b[4];
    S16 s[2];
    S32 i;
    F32 f;
} Type_Widget;

static GLint bytes_per_pixel( GLenum format, GLenum type )
{
   GLint n, m;

   switch (format) {
      case GL_COLOR_INDEX:
      case GL_STENCIL_INDEX:
      case GL_DEPTH_COMPONENT:
      case GL_RED:
      case GL_GREEN:
      case GL_BLUE:
      case GL_ALPHA:
      case GL_LUMINANCE:
	 n = 1;
	 break;
      case GL_LUMINANCE_ALPHA:
	 n = 2;
	 break;
      case GL_RGB:
	 n = 3;
	 break;
      case GL_RGBA:
#ifdef GL_EXT_abgr
      case GL_ABGR_EXT:
#endif
	 n = 4;
	 break;
      default:
	 n = 0;
   }

   switch (type) {
      case GL_UNSIGNED_BYTE:	m = sizeof(GLubyte);	break;
      case GL_BYTE:		m = sizeof(GLbyte);	break;
      case GL_BITMAP:		m = 1;			break;
      case GL_UNSIGNED_SHORT:	m = sizeof(GLushort);	break;
      case GL_SHORT:		m = sizeof(GLshort);	break;
      case GL_UNSIGNED_INT:	m = sizeof(GLuint);	break;
      case GL_INT:		m = sizeof(GLint);	break;
      case GL_FLOAT:		m = sizeof(GLfloat);	break;
      default:			m = 0;
   }

   return n * m;
}


static GLint elements_per_group(GLenum format, GLenum type)
{
    /*
     * Return the number of elements per group of a specified format
     */

    /* Types are not packed pixels, so get elements per group */
    switch(format) {
      case GL_RGB:
	return 3;
      case GL_LUMINANCE_ALPHA:
	return 2;
      case GL_RGBA:
	return 4;
      default:
	return 1;
    }
}

static GLfloat bytes_per_element(GLenum type)
{
    /*
     * Return the number of bytes per element, based on the element type
     */
    switch(type) {
      case GL_BITMAP:
	return 1.0 / 8.0;
      case GL_UNSIGNED_SHORT:
	return(sizeof(GLushort));
      case GL_SHORT:
	return(sizeof(GLshort));
      case GL_UNSIGNED_BYTE:
	return(sizeof(GLubyte));
      case GL_BYTE:
	return(sizeof(GLbyte));
      case GL_INT:
	return(sizeof(GLint));
      case GL_UNSIGNED_INT:
	return(sizeof(GLuint));
      case GL_FLOAT:
	return(sizeof(GLfloat));

      default:
	return 4;
    }
}

static GLint is_index(GLenum format)
{
    return format == GL_COLOR_INDEX || format == GL_STENCIL_INDEX;
}

static GLint image_size(GLint width, GLint height, GLenum format, GLenum type)
{
    S32 bytes_per_row;
    S32 components;

    components = elements_per_group(format,type);
    if (type == GL_BITMAP) {
	bytes_per_row = (width + 7) / 8;
    } else {
	bytes_per_row = bytes_per_element(type) * width;
    }
    return bytes_per_row * height * components;
}

static GLboolean isTypePackedPixel(GLenum type)
{
    return 0;
} /* isTypePackedPixel() */

static GLboolean legalFormat(GLenum format)
{
    switch(format) {
      case GL_COLOR_INDEX:
      case GL_STENCIL_INDEX:
      case GL_DEPTH_COMPONENT:
      case GL_RED:
      case GL_GREEN:
      case GL_BLUE:
      case GL_ALPHA:
      case GL_RGB:
      case GL_RGBA:
      case GL_LUMINANCE:
      case GL_LUMINANCE_ALPHA:
	return GL_TRUE;
      default:
	return GL_FALSE;
    }
}

static GLboolean legalType(GLenum type)
{
    switch(type) {
      case GL_BITMAP:
      case GL_BYTE:
      case GL_UNSIGNED_BYTE:
      case GL_SHORT:
      case GL_UNSIGNED_SHORT:
      case GL_INT:
      case GL_UNSIGNED_INT:
      case GL_FLOAT:

         return GL_TRUE;
      default:
	return GL_FALSE;
    }
}


static void retrieveStoreModes(PixelStorageModes *psm)
{
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &psm->unpack_alignment);
    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &psm->unpack_row_length);
    glGetIntegerv(GL_UNPACK_SKIP_ROWS, &psm->unpack_skip_rows);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &psm->unpack_skip_pixels);
    glGetIntegerv(GL_UNPACK_LSB_FIRST, &psm->unpack_lsb_first);
    glGetIntegerv(GL_UNPACK_SWAP_BYTES, &psm->unpack_swap_bytes);

    glGetIntegerv(GL_PACK_ALIGNMENT, &psm->pack_alignment);
    glGetIntegerv(GL_PACK_ROW_LENGTH, &psm->pack_row_length);
    glGetIntegerv(GL_PACK_SKIP_ROWS, &psm->pack_skip_rows);
    glGetIntegerv(GL_PACK_SKIP_PIXELS, &psm->pack_skip_pixels);
    glGetIntegerv(GL_PACK_LSB_FIRST, &psm->pack_lsb_first);
    glGetIntegerv(GL_PACK_SWAP_BYTES, &psm->pack_swap_bytes);
}

static void halveImage(GLint components, GLuint width, GLuint height,
		       const GLushort *datain, GLushort *dataout)
{
    S32 i, j, k;
    S32 newwidth, newheight;
    S32 delta;
    GLushort *s;
    const GLushort *t;

    newwidth = width / 2;
    newheight = height / 2;
    delta = width * components;
    s = dataout;
    t = datain;

    /* Piece o' cake! */
    for (i = 0; i < newheight; i++) {
	for (j = 0; j < newwidth; j++) {
	    for (k = 0; k < components; k++) {
		s[0] = (t[0] + t[components] + t[delta] +
			t[delta+components] + 2) / 4;
		s++; t++;
	    }
	    t += components;
	}
	t += delta;
    }
}

static void halve1Dimage_ubyte(GLint components, GLuint width, GLuint height,
			       const GLubyte *dataIn, GLubyte *dataOut,
			       GLint element_size, GLint ysize,
			       GLint group_size)
{
   GLint halfWidth= width / 2;
   GLint halfHeight= height / 2;
   const char *src= (const char *) dataIn;
   GLubyte *dest= dataOut;
   S32 jj;

   if (height == 1) {		/* 1 row */

      halfHeight= 1;

      for (jj= 0; jj< halfWidth; jj++) {
	 S32 kk;
	 for (kk= 0; kk< components; kk++) {
	    *dest= (*(const GLubyte*)src +
		 *(const GLubyte*)(src+group_size)) / 2;

	    src+= element_size;
	    dest++;
	 }
	 src+= group_size;	/* skip to next 2 */
      }
      {
	 S32 padBytes= ysize - (width*group_size);
	 src+= padBytes;	/* for assertion only */
      }
   }
   else if (width == 1) {	/* 1 column */
      S32 padBytes= ysize - (width * group_size);
      halfWidth= 1;
      /* one vertical column with possible pad bytes per row */
      /* average two at a time */

      for (jj= 0; jj< halfHeight; jj++) {
	 S32 kk;
	 for (kk= 0; kk< components; kk++) {
	    *dest= (*(const GLubyte*)src + *(const GLubyte*)(src+ysize)) / 2;

	    src+= element_size;
	    dest++;
	 }
	 src+= padBytes; /* add pad bytes, if any, to get to end to row */
	 src+= ysize;
      }
   }

} /* halve1Dimage_ubyte() */


static void halveImage_ubyte(GLint components, GLuint width, GLuint height,
			const GLubyte *datain, GLubyte *dataout,
			GLint element_size, GLint ysize, GLint group_size)
{
    S32 i, j, k;
    S32 newwidth, newheight;
    GLubyte *s;
    const char *t;

    /* handle case where there is only 1 column/row */
    if (width == 1 || height == 1) {
       halve1Dimage_ubyte(components,width,height,datain,dataout,
			  element_size,ysize,group_size);
       return;
    }

    newwidth = width / 2;
    newheight = height / 2;
    s = dataout;
    t = (const char *)datain;

    /* Piece o' cake! */
    for (i = 0; i < newheight; i++) {
	for (j = 0; j < newwidth; j++) {
	    for (k = 0; k < components; k++) {
                s[0] = (*(const GLubyte*)t +
			*(const GLubyte*)(t+group_size) +
                        *(const GLubyte*)(t+ysize) +
                        *(const GLubyte*)(t+ysize+group_size) + 2) / 4;
                s++; t += element_size;
	    }
	    t += group_size;
	}
	t += ysize;
    }
}

static void scale_internal(GLint components, GLint widthin, GLint heightin,
			   const GLushort *datain,
			   GLint widthout, GLint heightout,
			   GLushort *dataout)
{
    static F32 x, lowx, highx, convx, halfconvx;
    static F32 y, lowy, highy, convy, halfconvy;
    F32 xpercent,ypercent;
    F32 percent;
    /* Max components in a format is 4, so... */
    F32 totals[4];
    F32 area;
    S32 i,j,k,yint,xint,xindex,yindex;
    S32 temp;

    if (widthin == widthout*2 && heightin == heightout*2) {
	halveImage(components, widthin, heightin, datain, dataout);
	return;
    }
    convy = (F32) heightin/heightout;
    convx = (F32) widthin/widthout;
    halfconvx = convx/2;
    halfconvy = convy/2;
    for (i = 0; i < heightout; i++) {
	y = convy * (i+0.5);
	if (heightin > heightout) {
	    highy = y + halfconvy;
	    lowy = y - halfconvy;
	} else {
	    highy = y + 0.5;
	    lowy = y - 0.5;
	}
	for (j = 0; j < widthout; j++) {
	    x = convx * (j+0.5);
	    if (widthin > widthout) {
		highx = x + halfconvx;
		lowx = x - halfconvx;
	    } else {
		highx = x + 0.5;
		lowx = x - 0.5;
	    }

	    /*
	    ** Ok, now apply box filter to box that goes from (lowx, lowy)
	    ** to (highx, highy) on input data into this pixel on output
	    ** data.
	    */
	    totals[0] = totals[1] = totals[2] = totals[3] = 0.0;
	    area = 0.0;

	    y = lowy;
	    yint = floor(y);
	    while (y < highy) {
		yindex = (yint + heightin) % heightin;
		if (highy < yint+1) {
		    ypercent = highy - y;
		} else {
		    ypercent = yint+1 - y;
		}

		x = lowx;
		xint = floor(x);

		while (x < highx) {
		    xindex = (xint + widthin) % widthin;
		    if (highx < xint+1) {
			xpercent = highx - x;
		    } else {
			xpercent = xint+1 - x;
		    }

		    percent = xpercent * ypercent;
		    area += percent;
		    temp = (xindex + (yindex * widthin)) * components;
		    for (k = 0; k < components; k++) {
			totals[k] += datain[temp + k] * percent;
		    }

		    xint++;
		    x = xint;
		}
		yint++;
		y = yint;
	    }

	    temp = (j + (i * widthout)) * components;
	    for (k = 0; k < components; k++) {
		/* totals[] should be rounded in the case of enlarging an RGB
		 * ramp when the type is 332 or 4444
		 */
		dataout[temp + k] = (totals[k]+0.5)/area;
	    }
	}
    }
}

static void fill_image(const PixelStorageModes *psm,
		       GLint width, GLint height, GLenum format,
		       GLenum type, GLboolean index_format,
		       const void *userdata, GLushort *newimage)
{
    GLint components;
    GLint element_size;
    GLint rowsize;
    GLint padding;
    GLint groups_per_line;
    GLint group_size;
    GLint elements_per_line;
    const GLubyte *start;
    const GLubyte *iter;
    GLushort *iter2;
    GLint i, j;
    GLint myswap_bytes;

    myswap_bytes = psm->unpack_swap_bytes;
    components = elements_per_group(format,type);
    if (psm->unpack_row_length > 0) {
	groups_per_line = psm->unpack_row_length;
    } else {
	groups_per_line = width;
    }

    /* All formats except GL_BITMAP fall out trivially */
    if (type == GL_BITMAP) {
	GLint bit_offset;
	GLint current_bit;

	rowsize = (groups_per_line * components + 7) / 8;
	padding = (rowsize % psm->unpack_alignment);
	if (padding) {
	    rowsize += psm->unpack_alignment - padding;
	}
	start = (const GLubyte *) userdata + psm->unpack_skip_rows * rowsize +
		(psm->unpack_skip_pixels * components / 8);
	elements_per_line = width * components;
	iter2 = newimage;
	for (i = 0; i < height; i++) {
	    iter = start;
	    bit_offset = (psm->unpack_skip_pixels * components) % 8;
	    for (j = 0; j < elements_per_line; j++) {
		/* Retrieve bit */
		if (psm->unpack_lsb_first) {
		    current_bit = iter[0] & (1 << bit_offset);
		} else {
		    current_bit = iter[0] & (1 << (7 - bit_offset));
		}
		if (current_bit) {
		    if (index_format) {
			*iter2 = 1;
		    } else {
			*iter2 = 65535;
		    }
		} else {
		    *iter2 = 0;
		}
		bit_offset++;
		if (bit_offset == 8) {
		    bit_offset = 0;
		    iter++;
		}
		iter2++;
	    }
	    start += rowsize;
	}
    } else {
	element_size = bytes_per_element(type);
	group_size = element_size * components;
	if (element_size == 1) myswap_bytes = 0;

	rowsize = groups_per_line * group_size;
	padding = (rowsize % psm->unpack_alignment);
	if (padding) {
	    rowsize += psm->unpack_alignment - padding;
	}
	start = (const GLubyte *) userdata + psm->unpack_skip_rows * rowsize +
		psm->unpack_skip_pixels * group_size;
	elements_per_line = width * components;

	iter2 = newimage;
	for (i = 0; i < height; i++) {
	    iter = start;
	    for (j = 0; j < elements_per_line; j++) {
		Type_Widget widget;

		switch(type) {
		  case GL_UNSIGNED_BYTE:
		    if (index_format) {
			*iter2++ = *iter;
		    } else {
			*iter2++ = (*iter) * 257;
		    }
		    break;
		  case GL_BYTE:
		    if (index_format) {
			*iter2++ = *((const GLbyte *) iter);
		    } else {
			/* rough approx */
			*iter2++ = (*((const GLbyte *) iter)) * 516;
		    }
		    break;
		
		  case GL_UNSIGNED_SHORT:
		  case GL_SHORT:
		    if (myswap_bytes) {
			widget.ub[0] = iter[1];
			widget.ub[1] = iter[0];
		    } else {
			widget.ub[0] = iter[0];
			widget.ub[1] = iter[1];
		    }
		    if (type == GL_SHORT) {
			if (index_format) {
			    *iter2++ = widget.s[0];
			} else {
			    /* rough approx */
			    *iter2++ = widget.s[0]*2;
			}
		    } else {
			*iter2++ = widget.us[0];
		    }
		    break;

		  case GL_INT:
		  case GL_UNSIGNED_INT:
		  case GL_FLOAT:
		    if (myswap_bytes) {
			widget.ub[0] = iter[3];
			widget.ub[1] = iter[2];
			widget.ub[2] = iter[1];
			widget.ub[3] = iter[0];
		    } else {
			widget.ub[0] = iter[0];
			widget.ub[1] = iter[1];
			widget.ub[2] = iter[2];
			widget.ub[3] = iter[3];
		    }
		    if (type == GL_FLOAT) {
			if (index_format) {
			    *iter2++ = widget.f;
			} else {
			    *iter2++ = 65535 * widget.f;
			}
		    } else if (type == GL_UNSIGNED_INT) {
			if (index_format) {
			    *iter2++ = widget.ui;
			} else {
			    *iter2++ = widget.ui >> 16;
			}
		    } else {
			if (index_format) {
			    *iter2++ = widget.i;
			} else {
			    *iter2++ = widget.i >> 15;
			}
		    }
		    break;
		}
		iter += element_size;
	    } /* for j */
	    start += rowsize;
#if 1
	    /* want 'iter' pointing at start, not within, row for assertion
	     * purposes
	     */
	    iter= start;	
#endif
	} /* for i */

       /* iterators should be one byte past end */
       if (!isTypePackedPixel(type)) {
			iter2 = &newimage[width*height*components];
       }
       else {
			iter2 = &newimage[width*height*elements_per_group(format,0)];
       }
       iter = &((const GLubyte *)userdata)[rowsize*height +
					psm->unpack_skip_rows * rowsize +
					psm->unpack_skip_pixels * group_size];

    } /* else */
} /* fill_image() */

/*
** Insert array into user's data applying all pixel store modes.
** The internal format is an array of unsigned shorts.
** empty_image() because it is the opposite of fill_image().
*/
static void empty_image(const PixelStorageModes *psm,
			GLint width, GLint height, GLenum format,
		        GLenum type, GLboolean index_format,
			const GLushort *oldimage, void *userdata)
{
    GLint components;
    GLint element_size;
    GLint rowsize;
    GLint padding;
    GLint groups_per_line;
    GLint group_size;
    GLint elements_per_line;
    GLubyte *start;
    GLubyte *iter;
    const GLushort *iter2;
    GLint i, j;
    GLint myswap_bytes;

    myswap_bytes = psm->pack_swap_bytes;
    components = elements_per_group(format,type);
    if (psm->pack_row_length > 0) {
	groups_per_line = psm->pack_row_length;
    } else {
	groups_per_line = width;
    }

    /* All formats except GL_BITMAP fall out trivially */
    if (type == GL_BITMAP) {
	GLint bit_offset;
	GLint current_bit;

	rowsize = (groups_per_line * components + 7) / 8;
	padding = (rowsize % psm->pack_alignment);
	if (padding) {
	    rowsize += psm->pack_alignment - padding;
	}
	start = (GLubyte *) userdata + psm->pack_skip_rows * rowsize +
		(psm->pack_skip_pixels * components / 8);
	elements_per_line = width * components;
	iter2 = oldimage;
	for (i = 0; i < height; i++) {
	    iter = start;
	    bit_offset = (psm->pack_skip_pixels * components) % 8;
	    for (j = 0; j < elements_per_line; j++) {
		if (index_format) {
		    current_bit = iter2[0] & 1;
		} else {
		    if (iter2[0] > 32767) {
			current_bit = 1;
		    } else {
			current_bit = 0;
		    }
		}

		if (current_bit) {
		    if (psm->pack_lsb_first) {
			*iter |= (1 << bit_offset);
		    } else {
			*iter |= (1 << (7 - bit_offset));
		    }
		} else {
		    if (psm->pack_lsb_first) {
			*iter &= ~(1 << bit_offset);
		    } else {
			*iter &= ~(1 << (7 - bit_offset));
		    }
		}

		bit_offset++;
		if (bit_offset == 8) {
		    bit_offset = 0;
		    iter++;
		}
		iter2++;
	    }
	    start += rowsize;
	}
    } else {


	element_size = bytes_per_element(type);
	group_size = element_size * components;
	if (element_size == 1) myswap_bytes = 0;

	rowsize = groups_per_line * group_size;
	padding = (rowsize % psm->pack_alignment);
	if (padding) {
	    rowsize += psm->pack_alignment - padding;
	}
	start = (GLubyte *) userdata + psm->pack_skip_rows * rowsize +
		psm->pack_skip_pixels * group_size;
	elements_per_line = width * components;

	iter2 = oldimage;
	for (i = 0; i < height; i++) {
	    iter = start;
	    for (j = 0; j < elements_per_line; j++) {
		Type_Widget widget;

		switch(type) {
		  case GL_UNSIGNED_BYTE:
		    if (index_format) {
			*iter = *iter2++;
		    } else {
			*iter = *iter2++ >> 8;
		    }
		    break;
		  case GL_BYTE:
		    if (index_format) {
			*((GLbyte *) iter) = *iter2++;
		    } else {
			*((GLbyte *) iter) = *iter2++ >> 9;
		    }
		    break;
		  		  case GL_UNSIGNED_SHORT:
		  case GL_SHORT:
		    if (type == GL_SHORT) {
			if (index_format) {
			    widget.s[0] = *iter2++;
			} else {
			    widget.s[0] = *iter2++ >> 1;
			}
		    } else {
			widget.us[0] = *iter2++;
		    }
		    if (myswap_bytes) {
			iter[0] = widget.ub[1];
			iter[1] = widget.ub[0];
		    } else {
			iter[0] = widget.ub[0];
			iter[1] = widget.ub[1];
		    }
		    break;
		
		  case GL_INT:
		  case GL_UNSIGNED_INT:
		  case GL_FLOAT:
		    if (type == GL_FLOAT) {
			if (index_format) {
			    widget.f = *iter2++;
			} else {
			    widget.f = *iter2++ / (F32) 65535.0;
			}
		    } else if (type == GL_UNSIGNED_INT) {
			if (index_format) {
			    widget.ui = *iter2++;
			} else {
			    widget.ui = (U32) *iter2++ * 65537;
			}
		    } else {
			if (index_format) {
			    widget.i = *iter2++;
			} else {
			    widget.i = ((U32) *iter2++ * 65537)/2;
			}
		    }
		    if (myswap_bytes) {
			iter[3] = widget.ub[0];
			iter[2] = widget.ub[1];
			iter[1] = widget.ub[2];
			iter[0] = widget.ub[3];
		    } else {
			iter[0] = widget.ub[0];
			iter[1] = widget.ub[1];
			iter[2] = widget.ub[2];
			iter[3] = widget.ub[3];
		    }
		    break;
		}
		iter += element_size;
	    } /* for j */
	    start += rowsize;
#if 1
	    /* want 'iter' pointing at start, not within, row for assertion
	     * purposes
	     */
	    iter= start;	
#endif
	} /* for i */

	/* iterators should be one byte past end */
	if (!isTypePackedPixel(type)) {
	   assert(iter2 == &oldimage[width*height*components]);
	}
	else {
	   assert(iter2 == &oldimage[width*height*
				     elements_per_group(format,0)]);
	}
	assert(iter == &((GLubyte *)userdata)[rowsize*height +
					psm->pack_skip_rows * rowsize +
					psm->pack_skip_pixels * group_size]);

    } /* else */
} /* empty_image() */


GLint
myScaleImage(GLenum format, GLsizei widthin, GLsizei heightin,
		    GLenum typein, const void *datain,
		    GLsizei widthout, GLsizei heightout, GLenum typeout,
		    void *dataout)
{
    S32 components;
    GLushort *beforeImage;
    GLushort *afterImage;
    PixelStorageModes psm;

    if (widthin == 0 || heightin == 0 || widthout == 0 || heightout == 0) {
	return 0;
    }
    if (widthin < 0 || heightin < 0 || widthout < 0 || heightout < 0) {
	return GLU_INVALID_VALUE;
    }
    if (!legalFormat(format) || !legalType(typein) || !legalType(typeout)) {
	return GLU_INVALID_ENUM;
    }

    beforeImage = (GLushort *)
	malloc(image_size(widthin, heightin, format, GL_UNSIGNED_SHORT));
    afterImage =(GLushort *)
	malloc(image_size(widthout, heightout, format, GL_UNSIGNED_SHORT));
    if (beforeImage == NULL || afterImage == NULL) {
	return GLU_OUT_OF_MEMORY;
    }

    retrieveStoreModes(&psm);
    fill_image(&psm,widthin, heightin, format, typein, is_index(format),
	    datain, beforeImage);
    components = elements_per_group(format, 0);
    scale_internal(components, widthin, heightin, beforeImage,
	    widthout, heightout, afterImage);
    empty_image(&psm,widthout, heightout, format, typeout,
	    is_index(format), afterImage, dataout);
    free((GLbyte *) beforeImage);
    free((GLbyte *) afterImage);

    return 0;
}

S32 round2(S32 x){
   S32 v=2;

   while(x>v) v*=2;

   return v;
}

GLint myBuild2DMipmaps( GLenum target, GLint components,
                         GLsizei width, GLsizei height, GLenum format,
                         GLenum type, const void *data )
{
   GLint w, h, maxsize;
   void *image, *newimage;
   GLint neww, newh, level, bpp;
   S32 error;
   GLboolean done;
   GLint retval = 0;
   GLint unpackrowlength, unpackalignment, unpackskiprows, unpackskippixels;
   GLint packrowlength, packalignment, packskiprows, packskippixels;

   if (width < 1 || height < 1)
      return GL_INVALID_VALUE;

   glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxsize );

   w = round2(width);
   if (w>maxsize) {
     w = maxsize;
   }
   h = round2(height);
   if (h>maxsize) {
      h = maxsize;
   }
	
   bpp = bytes_per_pixel( format, type );
   if (bpp==0) {
      /* probably a bad format or type enum */
      return GL_INVALID_ENUM;
   }

   /* Get current glPixelStore values */
   glGetIntegerv( GL_UNPACK_ROW_LENGTH, &unpackrowlength );
   glGetIntegerv( GL_UNPACK_ALIGNMENT, &unpackalignment );
   glGetIntegerv( GL_UNPACK_SKIP_ROWS, &unpackskiprows );
   glGetIntegerv( GL_UNPACK_SKIP_PIXELS, &unpackskippixels );
   glGetIntegerv( GL_PACK_ROW_LENGTH, &packrowlength );
   glGetIntegerv( GL_PACK_ALIGNMENT, &packalignment );
   glGetIntegerv( GL_PACK_SKIP_ROWS, &packskiprows );
   glGetIntegerv( GL_PACK_SKIP_PIXELS, &packskippixels );

   /* set pixel packing */
   glPixelStorei( GL_PACK_ROW_LENGTH, 0 );
   glPixelStorei( GL_PACK_ALIGNMENT, 1 );
   glPixelStorei( GL_PACK_SKIP_ROWS, 0 );
   glPixelStorei( GL_PACK_SKIP_PIXELS, 0 );

   done = GL_FALSE;

   if (w!=width || h!=height) {
      /* must rescale image to get "top" mipmap texture image */
      image = (void *)malloc( (w+4) * h * bpp );
      if (!image) {
	 return GL_OUT_OF_MEMORY;
      }
      error = myScaleImage( format, width, height, type, data,
			     w, h, type, image );
      if (error) {
         retval = error;
         done = GL_TRUE;
      }
   }
   else {
      image = (void *) data;
   }

   level = 0;
   while (!done) {
      if (image != data) {
         /* set pixel unpacking */
         glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
         glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
         glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
         glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
      }

      glTexImage2D( target, level, components, w, h, 0, format, type, image );

      if (w==1 && h==1)  break;

      neww = (w<2) ? 1 : w/2;
      newh = (h<2) ? 1 : h/2;
      newimage = (void *)malloc( (neww+4) * newh * bpp );
      if (!newimage) {
	 return GL_OUT_OF_MEMORY;
      }

      error =  myScaleImage( format, w, h, type, image,
			      neww, newh, type, newimage );
      if (error) {
         retval = error;
         done = GL_TRUE;
      }

      if (image!=data) {
	 free( image );
      }
      image = newimage;

      w = neww;
      h = newh;
      level++;
   }

   if (image!=data) {
      free( image );
   }

   /* Restore original glPixelStore state */
   glPixelStorei( GL_UNPACK_ROW_LENGTH, unpackrowlength );
   glPixelStorei( GL_UNPACK_ALIGNMENT, unpackalignment );
   glPixelStorei( GL_UNPACK_SKIP_ROWS, unpackskiprows );
   glPixelStorei( GL_UNPACK_SKIP_PIXELS, unpackskippixels );
   glPixelStorei( GL_PACK_ROW_LENGTH, packrowlength );
   glPixelStorei( GL_PACK_ALIGNMENT, packalignment );
   glPixelStorei( GL_PACK_SKIP_ROWS, packskiprows );
   glPixelStorei( GL_PACK_SKIP_PIXELS, packskippixels );

   return retval;
}

void gluPerspectiveInf( GLdouble fov, GLdouble aspectr, GLdouble zNear) {
        const F32 nudge = 1.0 - 1.0 / (1<<23);
        GLdouble p[4][4];

        fov = fov * M_PI / 360.0;

        p[1][0] = p[2][0] = p[3][0] = p[0][1] =
        p[2][1] = p[3][1] = p[0][2] = p[1][2] =
        p[0][3] = p[1][3] = p[2][2] = 0;

        p[0][0] = 1.0 / tan(fov);
        p[1][1] = p[0][0] * aspectr;

        p[3][2] = -2.0 * zNear * nudge;
        p[3][3] = -1.0 * nudge;
        p[2][3] = -1.0;

        glLoadMatrixd(&p[0][0]);
}

void gluPerspective( GLdouble fovy, GLdouble aspect,
                     GLdouble zNear, GLdouble zFar )
{
   GLdouble xmin, xmax, ymin, ymax;

   ymax = zNear * tan( fovy * M_PI / 360.0 );
   ymin = -ymax;

   xmin = ymin * aspect;
   xmax = ymax * aspect;

   glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}

void gluLookAt( GLdouble eyex, GLdouble eyey, GLdouble eyez,
                GLdouble centerx, GLdouble centery, GLdouble centerz,
                GLdouble upx, GLdouble upy, GLdouble upz )
{
   GLdouble m[16];
   GLdouble x[3], y[3], z[3];
   GLdouble mag;

   /* Make rotation matrix */

   /* Z vector */
   z[0] = eyex - centerx;
   z[1] = eyey - centery;
   z[2] = eyez - centerz;
   mag = sqrt( z[0]*z[0] + z[1]*z[1] + z[2]*z[2] );
   if (mag) {  /* mpichler, 19950515 */
      z[0] /= mag;
      z[1] /= mag;
      z[2] /= mag;
   }

   /* Y vector */
   y[0] = upx;
   y[1] = upy;
   y[2] = upz;

   /* X vector = Y cross Z */
   x[0] =  y[1]*z[2] - y[2]*z[1];
   x[1] = -y[0]*z[2] + y[2]*z[0];
   x[2] =  y[0]*z[1] - y[1]*z[0];

   /* Recompute Y = Z cross X */
   y[0] =  z[1]*x[2] - z[2]*x[1];
   y[1] = -z[0]*x[2] + z[2]*x[0];
   y[2] =  z[0]*x[1] - z[1]*x[0];

   /* mpichler, 19950515 */
   /* cross product gives area of parallelogram, which is < 1.0 for
    * non-perpendicular unit-length vectors; so normalize x, y here
    */

   mag = sqrt( x[0]*x[0] + x[1]*x[1] + x[2]*x[2] );
   if (mag) {
      x[0] /= mag;
      x[1] /= mag;
      x[2] /= mag;
   }

   mag = sqrt( y[0]*y[0] + y[1]*y[1] + y[2]*y[2] );
   if (mag) {
      y[0] /= mag;
      y[1] /= mag;
      y[2] /= mag;
   }

#define M(row,col)  m[col*4+row]
   M(0,0) = x[0];  M(0,1) = x[1];  M(0,2) = x[2];  M(0,3) = 0.0;
   M(1,0) = y[0];  M(1,1) = y[1];  M(1,2) = y[2];  M(1,3) = 0.0;
   M(2,0) = z[0];  M(2,1) = z[1];  M(2,2) = z[2];  M(2,3) = 0.0;
   M(3,0) = 0.0;   M(3,1) = 0.0;   M(3,2) = 0.0;   M(3,3) = 1.0;
#undef M
   glMultMatrixd( m );

   /* Translate Eye to Origin */
   glTranslated( -eyex, -eyey, -eyez );

}

