/***************************************************************************
	  					imagefile.cpp  -  description
                             -------------------
    begin                : Fri Mar 15 2002
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

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <vector.h>
#include "globaldefs.h"

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "imagefile.h"
#include "exception.h"
#include "myglu.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <jpeglib.h>
#ifdef __cplusplus
}
#endif

//
// CPixel
//

CPixel CPixel::normalsAdd(const CPixel &a, const CPixel &b){
    const F32 oneOver127 = 1.0f/127.0f;
    CVector3 n;

    n.x = (oneOver127 * a.r - 1.0) + (oneOver127 * b.r - 1.0);
    n.y = (oneOver127 * a.g - 1.0) + (oneOver127 * b.g - 1.0);
    n.z = (oneOver127 * a.b - 1.0) + (oneOver127 * b.b - 1.0);

    if(n.x == 0.0 && n.y == 0.0 && n.z == 0.0){
        return CPixel(0,0,0,255);
    }

    normalize(n);

    return CPixel(128 + 127*n.x, 128 + 127*n.y, 128 + 127*n.z, 255);
}

//
// CImage
//

CImage::CImage(){
}
CImage::~CImage(){
}

//
// CImageBase
//

CImageBase::CImageBase(){
    width = 0;
    height = 0;
    imageData = 0;
    setColorFormat(RGBA);
}
CImageBase::~CImageBase(){
    freeImageData();
}

static unsigned char *downSampleNormalMap(unsigned char *in, S32 w2, S32 h2, S32 w, S32 h){
    const F32 oneOver127 = 1.0f/127.0f;
    const F32 oneOver255 = 1.0f/255.0f;

    F32 x, y, z, l, invl;
    F32 mag00, mag01, mag10, mag11;
    S32 i, j, ii, jj;

    /* Allocate space for the downsampled normal map level. */
    unsigned char *out = (unsigned char *) malloc(w * h * 4);

    h2 &= ~1;
    w2 &= ~1;

    for (i=0; i<h2; i+=2) {
        for (j=0; j<w2; j+=2) {
            S32 modh = (i + 1) % h2;
            S32 modw = (j + 1) % w2;

            /* Fetch the magnitude of the four vectors to be downsampled. */
            mag00 = oneOver255 * in[ ((i)*w2 + (j))*4 + 3 ];
            mag01 = oneOver255 * in[ ((i)*w2 + (modw))*4 + 3 ];
            mag10 = oneOver255 * in[((modh)*w2 + j)*4  + 3 ];
            mag11 = oneOver255 * in[((modh)*w2 + (modw))*4 + 3 ];

            /* Sum 2x2 footprint of red component scaled back to [-1,1] floating point range. */
            x =  mag00 * (oneOver127 * in[(i*w2 + j)*4] - 1.0f);
            x += mag01 * (oneOver127 * in[(i*w2 + modw)*4] - 1.0f);
            x += mag10 * (oneOver127 * in[((modh)*w2 + j)*4] - 1.0f);
            x += mag11 * (oneOver127 * in[((modh)*w2 + modw)*4] - 1.0f);

            /* Sum 2x2 footprint of green component scaled back to [-1,1] floating point range. */
            y =  mag00 * (oneOver127 * in[(i*w2 + j)*4 + 1 ] - 1.0f);
            y += mag01 * (oneOver127 * in[(i*w2 + modw)*4 + 1 ] - 1.0f);
            y += mag10 * (oneOver127 * in[(modh*w2 + j)*4 + 1 ] - 1.0f);
            y += mag11 * (oneOver127 * in[(modh*w2 + modw)*4 + 1 ] - 1.0f);

            /* Sum 2x2 footprint of blue component scaled back to [-1,1] floating point range. */
            z =  mag00 * (oneOver127 * in[(i*w2 + j)*4 + 2 ] - 1.0f);
            z += mag01 * (oneOver127 * in[(i*w2 + modw)*4 + 2 ] - 1.0f);
            z += mag10 * (oneOver127 * in[(modh*w2 + j)*4 + 2 ] - 1.0f);
            z += mag11 * (oneOver127 * in[(modh*w2 + modw)*4 + 2 ] - 1.0f);

            /* Compute length of the (x,y,z) vector. */
            l = (F32) sqrt(x*x + y*y + z*z);
            if (l == 0.0) {
                /* Ugh, a zero length vector.  Avoid division by zero and just
                use the unpeturbed normal (0,0,1). */
                x = 0.0;
                y = 0.0;
                z = 1.0;
            } else {
                /* Normalize the vector to unit length. */
                invl = 1.0f/l;
                x = x*invl;
                y = y*invl;
                z = z*invl;
            }

            ii = i >> 1;
            jj = j >> 1;

            /* Pack the normalized vector into an RGB unsigned byte vector
            in the downsampled image. */
            out[(ii*w+jj)*4+0] = (unsigned char) (128 + 127*x);
            out[(ii*w+jj)*4+1] = (unsigned char) (128 + 127*y);
            out[(ii*w+jj)*4+2] = (unsigned char) (128 + 127*z);

            /* Store the magnitude of the average vector in the alpha
            component so we keep track of the magntiude. */
            l = l/4.0f;
            if (l > 1.0) {
                out[(ii*w+jj)*4+3] = 255;
            } else {
                out[(ii*w+jj)*4+3] = (unsigned char) (255.0f*l);
            }
        }
    }

    return out;
}

void CImageBase::scale(S32 w, S32 h){
    unsigned char *out;
    if(getWidth() == w && getHeight() == h)
        return;

    S32 numBytes;
    GLenum type;
    if(getColorFormat() == CImage::RGBA){ numBytes = 4;type = GL_RGBA;}
    else if(getColorFormat() == CImage::RGB){ numBytes = 3;type = GL_RGB;}

	if(getColorFormat() != CImage::NORMAL){
		out = (unsigned char *)malloc(w * h * numBytes);
		myScaleImage(type, getWidth(), getHeight(), GL_UNSIGNED_BYTE, getImageData(),
			w, h, GL_UNSIGNED_BYTE, out);
	} else {
        numBytes = 4;
		out = downSampleNormalMap((unsigned char *)getImageData(), getWidth(), getHeight(), w, h);
	}

    allocateImageData(w * h * numBytes);
    memcpy(getImageData(), out, w * h * numBytes);
    width = w;
    height = h;
    free(out);
}

void CImageBase::setConvertParam(F32 p1){
	param1 = p1;
}

void CImageBase::convertTo(EColorFormat f){
    S32 w = getWidth();
    S32 h = getHeight();
    unsigned char *out, *in;
    S32 numBytes;

    EColorFormat colorFormat = getColorFormat();

    switch(f){
    case GRAYSCALE:
        if(colorFormat == GRAYSCALE)return;
        if(colorFormat == RGB || colorFormat == RGBA){
            S32 c = colorFormat == RGB ? 3 : 4;
            //c = 4;
            in = (unsigned char *)getImageData();
            out = (unsigned char *)malloc(w * h);
            for(S32 y = 0 ; y < h ; y++ )
                for(S32 x = 0 ; x < w ; x++){
                    S32 sum = 0;
                    S32 index = y * w + x;

                    sum += in[index*c];
                    sum += in[index*c + 1];
                    sum += in[index*c + 2];
                    out[index] = sum / 3;
                }
                numBytes = 1;
        }
        break;
    case NORMAL:
        numBytes = 4;
        switch(colorFormat){
		case GRAYSCALE:
			{
            F32 sqlen, reciplen, nx, ny, nz;

            F32 oneOver255 = 1.0f/255.0f;

            F32 c, cx, cy, dcx, dcy;
            S32 wr = w;
            S32 hr = h;

            in = (unsigned char *)getImageData();
            out = (unsigned char *) malloc(w * h * numBytes);

            for(S32 i = 0 ; i < h ; i++) {
                for(S32 j = 0 ; j < w ; j++) {
                    /* Expand [0,255] texel values to the [0,1] range. */
                    c = in[i*wr + j] * oneOver255;
                    /* Expand the texel to its right. */
                    cx = in[i*wr + (j+1)%wr] * oneOver255;
                    /* Expand the texel one up. */
                    cy = in[((i+1)%hr)*wr + j] * oneOver255;
                    dcx = param1 * (c - cx);
                    dcy = param1 * (c - cy);

                    /* Normalize the vector. */
                    sqlen = dcx*dcx + dcy*dcy + 1;
                    reciplen = 1.0f/(F32)sqrt(sqlen);
                    nx = dcx*reciplen;
                    ny = dcy*reciplen;
                    nz = reciplen;

                    /* Repack the normalized vector into an RGB unsigned byte
                    vector in the normal map image. */
                    out[(i*w+j)*4+0] = (unsigned char) (128 + 127*nx);
                    out[(i*w+j)*4+1] = (unsigned char) (128 + 127*ny);
                    out[(i*w+j)*4+2] = (unsigned char) (128 + 127*nz);

                    /* The highest resolution mipmap level always has a
                    unit length magnitude. */
                    out[(i*w+j)*4+3] = 255;
                }
            }
            break;
			}
		case RGB:
            in = (unsigned char *)getImageData();
            out = (unsigned char *) malloc(w * h * numBytes);
            for(S32 i = 0 ; i < w * h ; ++i){
                out[i*4+0] = in[i*3+0];
                out[i*4+1] = 255 - in[i*3+1];
                out[i*4+2] = in[i*3+2];
                out[i*4+3] = 255;
            }
            break;
		case RGBA:
            //return;
            mColorFormat = f; //Just "rename" the format
            in = (unsigned char *)getImageData();
            for(S32 i = 0 ; i < w * h ; ++i){
               in[i*4+1] = 255-in[i*4+1];
               in[i*4+3] = 255;
            }
			return;
        case NORMAL:
            return;
        } 
        break;
    default:
        throw CException("Unsuppored Convertion");
    }

    mColorFormat = f;
    allocateImageData(w * h * numBytes);
    memcpy(getImageData(), out, w * h * numBytes);
    free(out);
}

CImage *CImage::mix(CImage *a, CImage *b, EMixType type){
    CImage *newimage = new CImageBase();

    // The new image will have the properties of image "a"
    newimage->setColorFormat(a->getColorFormat());
    newimage->setWidth(a->getWidth());
    newimage->setHeight(a->getHeight());
    newimage->create();

    CPixel pn, pa, pb;
    for(S32 x = 0 ; x < a->getWidth() ; ++x){
        for(S32 y = 0 ; y < a->getHeight() ; ++y){
            a->getPixel(pa, x, y);
            b->getPixel(pb, x, y);
            switch(type){
            case ADD:
                if(a->getColorFormat() != CImage::NORMAL)
                    pn = pa + pb;
                else
                    pn = CPixel::normalsAdd(pa, pb);
                break;
            case SUB:
                pn = pa - pb;
                break;
            case MUL: //FIXME
                pn.r = pa.r - pb.r;
                pn.g = pa.g - pb.g;
                pn.b = pa.b - pb.b;
                pn.a = pa.a - pb.a;
                break;
            }
            newimage->setPixel(pn, x, y);
        }
    }

    return newimage;
}

void CImage::makeBlack(){
    unsigned char *data = (unsigned char *)getImageData();
    S32 numBytes;
    switch(getColorFormat()){
    case GRAYSCALE:
        numBytes = 1;
        break;
    case RGB:
        numBytes = 3;
        break;
    case RGBA:
    case NORMAL:
        numBytes = 4;
        break;
    }
    for(S32 i = 0 ; i < getWidth() * getHeight() * numBytes ; ++i){
        data[i] = 0;
    }
}

S32 CImageBase::getWidth(){
    return width;
}
S32 CImageBase::getHeight(){
    return height;
}

void CImageBase::setWidth(F32 w){
    width = w;
}
void CImageBase::setHeight(F32 h){
    height = h;
}

void *CImageBase::getImageData(){
    return imageData;
}
CImage::EColorFormat CImageBase::getColorFormat(){
    return mColorFormat;
}
void CImageBase::setColorFormat(CImage::EColorFormat f){
    mColorFormat = f;
}
void CImageBase::create(){
    S32 numBytes;
    switch(getColorFormat()){
    case GRAYSCALE:
        numBytes = 1;
        break;
    case RGB:
        numBytes = 3;
        break;
    case RGBA:
    case NORMAL:
        numBytes = 4;
        break;
    }
    allocateImageData(getWidth() * getHeight() * numBytes);
}
void CImageBase::getPixel(CPixel &p, S32 x, S32 y){
    unsigned char *im = (unsigned char *)getImageData();
    if(x > getWidth() || y > getHeight()){
        p.r = p.g = p.b = p.a = 0;
        return;
    }
    switch(getColorFormat()){
    case GRAYSCALE:
        p.r = p.g = p.b = p.a = im[y * getWidth() + x];
        break;
    case RGB:
        p.r = im[(y * getWidth() + x) * 3 + 0];
        p.g = im[(y * getWidth() + x) * 3 + 1];
        p.b = im[(y * getWidth() + x) * 3 + 2];
        p.a = 255;
        break;
    case RGBA:
    case NORMAL:
        p.r = im[(y * getWidth() + x) * 4 + 0];
        p.g = im[(y * getWidth() + x) * 4 + 1];
        p.b = im[(y * getWidth() + x) * 4 + 2];
        p.a = im[(y * getWidth() + x) * 4 + 3];
        break;
    }
}
void CImageBase::setPixel(const CPixel &p, S32 x, S32 y){
    unsigned char *im = (unsigned char *)getImageData();
    if(x > getWidth() || y > getHeight()){
        return;
    }
    switch(getColorFormat()){
    case GRAYSCALE:
        im[y * getWidth() + x] = p.r;
        break;
    case RGB:
        im[(y * getWidth() + x) * 3 + 0] = p.r;
        im[(y * getWidth() + x) * 3 + 1] = p.g;
        im[(y * getWidth() + x) * 3 + 2] = p.b;
        break;
    case RGBA:
    case NORMAL:
        im[(y * getWidth() + x) * 4 + 0] = p.r;
        im[(y * getWidth() + x) * 4 + 1] = p.g;
        im[(y * getWidth() + x) * 4 + 2] = p.b;
        im[(y * getWidth() + x) * 4 + 3] = p.a;
        break;
    }
}
S32 CImageBase::getImageDataSize(){
    return imageDataSize;
}
void CImageBase::allocateImageData(S32 size){
    imageDataSize = size;
    imageData = realloc(imageData, size);
}
void CImageBase::freeImageData(){
    if(imageData){
        free(imageData);
        imageData = 0;
    }
}

void CImageBase::process(void *data, S32 size){
}

//
// CImageDummy
//

CImageDummy::CImageDummy(){
    width = 8;
    height = 8;

    allocateImageData(width * height * 3);
    mColorFormat = RGB;

    unsigned char *data = (unsigned char *)getImageData();
    for(S32 i = 0 ; i < width * height * 3 ; i++){
        data[i] = 255;
    }
}

CImageDummy::~CImageDummy(){
}

void CImageDummy::process(void *data, S32 size){
}

//
// CImagePNG
//

#include "libpng/png.h"

CImagePNG::CImagePNG(){
}
CImagePNG::~CImagePNG(){
}

static void pngRead(png_structp png_ptr, png_bytep data, png_size_t length){
    memcpy(data, (png_bytep)png_ptr->io_ptr, length);
    png_ptr->io_ptr = (png_bytep)png_ptr->io_ptr + length;
}

void CImagePNG::process(void *data, S32 size){
    void *d = data;

    png_structp png_ptr = png_create_read_struct
        (PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
    if (!png_ptr)
        throw CException("Failed to load PNG file");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr){
        png_destroy_read_struct(&png_ptr,
            (png_infopp)NULL, (png_infopp)NULL);
        throw CException("Failed to load PNG file");
    }

    png_set_read_fn(png_ptr, (png_voidp)d, pngRead);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);

    S32 bitDepth, colorType;
    bitDepth = png_get_bit_depth(png_ptr, info_ptr);
    colorType = png_get_color_type(png_ptr, info_ptr);

    if(bitDepth != 8)
        throw CException("Only 8-bit depth PNGs supported");

    switch(colorType){
        case PNG_COLOR_TYPE_PALETTE:
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            mColorFormat = CImage::GRAYSCALE;
            //throw CException("Unsupported PNG");
            break;
        case PNG_COLOR_TYPE_RGB:
            mColorFormat = CImage::RGB;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            mColorFormat = CImage::RGBA;
            break;
        case PNG_COLOR_TYPE_GRAY:
            mColorFormat = CImage::GRAYSCALE;
            break;
    }

    S32 rowSpan = png_get_rowbytes(png_ptr, info_ptr);

    allocateImageData(height * rowSpan);
    byte *imData = (byte *)getImageData();

    png_bytep* rowPtr = new png_bytep[height];
    for (S32 i = 0; i < height; i++)
        rowPtr[i] = (png_bytep)&(imData[i*rowSpan]);

    png_read_image(png_ptr, rowPtr);

    png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

    delete [] rowPtr;
}

//
// CImageTGA
//

typedef struct {    // TGA file header
    unsigned char   id_length, colormap_type, image_type;
    U16  colormap_index, colormap_length;
    //unsigned char   colormap_size;
    U16  x_origin, y_origin, width, height;
    unsigned char   pixel_size, attributes;
} TgaHeader;

CImageTGA::CImageTGA(){
}
CImageTGA::~CImageTGA(){
}

void CImageTGA::process(void *data, S32 size){
    S32 columns, rows, numPixels;
    S32 row, column;
    byte *rgba, *dest, *tga;
    TgaHeader *header;
    byte red,green,blue,alpha,packetHeader,packetSize,j;



    tga = (byte *)data;
    header = (TgaHeader *)tga;
    tga += sizeof(TgaHeader);


    // Only types 2 and 10 are supported
    if (header->image_type!=2 && header->image_type!=10) {
        throw CException("Unsupported TGA file");
    }
    header->id_length = 0;
    header->colormap_type = 0;
    // Only 24bit or 32bit pixels are supported
    if (header->colormap_type!=0||
        (header->pixel_size!=32&&header->pixel_size!=24)) {
            throw CException("Unsupported TGA color depth");
        }

        if(header->pixel_size == 32)mColorFormat = CImage::RGBA;
        if(header->pixel_size == 24)mColorFormat = CImage::RGB;

        columns = header->width;
        rows = header->height;
        numPixels = columns * rows;

        width = columns;
        height = rows;

        // Allocate memory for decoded image
        allocateImageData(numPixels * 4);
        rgba = (byte *)getImageData();

        // Skip comment
        if (header->id_length!=0)
            tga+=header->id_length;

        // Uncompressed RGB image
        if (header->image_type==2) {
            //for(row=rows-1; row>=0; row--) {
            for(row=0; row < rows; row++) {
                dest = rgba + row*columns*4;
                for(column=0; column<columns; column++) {
                    if (header->pixel_size==24) {
                        blue = *tga; tga++;
                        green = *tga; tga++;
                        red = *tga; tga++;
                        *dest++ = red;
                        *dest++ = green;
                        *dest++ = blue;
                        *dest++ = 255;
                    }
                    else if (header->pixel_size==32) {

                        blue = *tga; tga++;
                        green = *tga; tga++;
                        red = *tga; tga++;
                        alpha = *tga; tga++;

                        *dest++ = red;
                        *dest++ = green;
                        *dest++ = blue;
                        *dest++ = alpha;
                    }
                }
            }
        }
        // RLE RGB image
        else if (header->image_type==10) {
            //for(row=rows-1; row>=0; row--) {
            for(row=0; row < rows ; row++) {
                dest = rgba + row*columns*4;
                for(column=0; column<columns; ) {
                    packetHeader=*tga; tga++;
                    packetSize = 1 + (packetHeader & 0x7f);
                    // RLE packet
                    if (packetHeader & 0x80) {
                        if (header->pixel_size==24) {
                            blue = *tga; tga++;
                            green = *tga; tga++;
                            red = *tga; tga++;
                            alpha = 255;
                        }
                        else if (header->pixel_size==32) {
                            blue = *tga; tga++;
                            green = *tga; tga++;
                            red = *tga; tga++;
                            alpha = *tga; tga++;
                        } else {
                            blue = green = red = alpha = 0;
                        }

                        for(j=0;j<packetSize;j++) {
                            *dest++=red;
                            *dest++=green;
                            *dest++=blue;
                            *dest++=alpha;
                            column++;
                            if (column==columns) {
                                column=0;
                                if (row>0)
                                    row--;
                                else
                                    goto end_decode;
                                dest = rgba + row*columns*4;
                            }
                        }
                    }
                    // Non-RLE packet
                    else {
                        for(j=0;j<packetSize;j++) {
                            if (header->pixel_size==24) {
                                blue = *tga; tga++;
                                green = *tga; tga++;
                                red = *tga; tga++;
                                *dest++ = red;
                                *dest++ = green;
                                *dest++ = blue;
                                *dest++ = 255;
                            }
                            else {
                                blue = *tga; tga++;
                                green = *tga; tga++;
                                red = *tga; tga++;
                                alpha = *tga; tga++;
                                *dest++ = red;
                                *dest++ = green;
                                *dest++ = blue;
                                *dest++ = alpha;
                            }
                            column++;
                            if (column==columns) {
                                column=0;
                                if (row>0)
                                    row--;
                                else
                                    goto end_decode;
                                dest = rgba + row*columns*4;
                            }                       
                        }
                    }
                }
end_decode:;
            }
        }   
}

CImageJPEG::CImageJPEG(){
}
CImageJPEG::~CImageJPEG(){
}


void CImageJPEG::jpg_noop(jpeg_decompress_struct * cinfo){
}
#ifdef __GNUC__
#define boolean S32
#endif
boolean CImageJPEG::jpg_fill_input_buffer(jpeg_decompress_struct * cinfo){
    //printf("Premeture end of jpeg file");
    return 1;
}
#ifdef __GNUC__
#undef boolean
#endif

void CImageJPEG::jpg_skip_input_data(jpeg_decompress_struct * cinfo, long num_bytes){

    cinfo->src->next_input_byte += (size_t) num_bytes;
    cinfo->src->bytes_in_buffer -= (size_t) num_bytes;
    if (cinfo->src->bytes_in_buffer < 0)
        printf("Premeture end of jpeg file");
}

void CImageJPEG::jpeg_mem_src(jpeg_decompress_struct * cinfo, byte *mem, S32 len){
    cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small)((j_common_ptr) cinfo,
        JPOOL_PERMANENT,
        sizeof(struct jpeg_source_mgr));
    cinfo->src->init_source = jpg_noop;
    cinfo->src->fill_input_buffer = jpg_fill_input_buffer;
    cinfo->src->skip_input_data = jpg_skip_input_data;
    cinfo->src->resync_to_restart = jpeg_resync_to_restart;
    cinfo->src->term_source = jpg_noop;
    cinfo->src->bytes_in_buffer = len;
    cinfo->src->next_input_byte = mem;
}

void CImageJPEG::process(void *data, S32 size){
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    byte *img, *c, *rgb, *rgba;
    S32 i;

    rgb = (byte *)data;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, rgb, size);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    if (cinfo.output_components != 3 && cinfo.output_components != 1 ){
        throw CException("Bad number of JPEG components");
    }

    img = c = (unsigned char *)malloc(cinfo.output_width * cinfo.output_height * 3);
    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, &c, 1);
        c += cinfo.output_width * cinfo.output_components;
    }

    width = cinfo.output_width;
    height = cinfo.output_height;

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);


    if(cinfo.output_components == 3){
        mColorFormat = CImage::RGB;
        allocateImageData(width * height * 3);
        rgba = (byte *)getImageData();
        for(i=0;i<width*height;i++){
            *(rgba+i*3) = *(img+i*3);
            *(rgba+i*3+1) = *(img+i*3+1);
            *(rgba+i*3+2) = *(img+i*3+2);
        }
    } else if(cinfo.output_components == 1){
        mColorFormat = CImage::GRAYSCALE;
        allocateImageData(width * height);
        rgba = (byte *)getImageData();
        for(i=0;i<width*height;i++){
            *(rgba+i) = *(img+i);
        }
    }

    free(img);
}

CImage *CImageLoader::loadImage(CFile &file){   

    file.setName(file.getName() + ".png");
    try {
        file.loadData();
    } catch (CException e) {
        file.chopExtention();
        file.setName(file.getName() + ".jpg");

        try {
            file.loadData();
        } catch (CException e) {
            file.chopExtention();
            file.setName(file.getName() + ".tga");

            try {
                file.loadData();
            } catch (CException e) {
                throw CException("No supported image found");
            }

            CImage *image = new CImageTGA();
            try {
                image->process(file.getData(), file.getSize());
            } catch (CException e) {
                delete image;
                throw e;
            }

            return image;
        }

        CImage *image = new CImageJPEG();
        try {
            image->process(file.getData(), file.getSize());
        } catch (CException e) {
            delete image;
            throw e;
        }

        return image;
    }

    CImage *image = new CImagePNG();

    try {
        image->process(file.getData(), file.getSize());
    } catch (CException e) {
        delete image;
        throw e;
    }

    return image;
}
