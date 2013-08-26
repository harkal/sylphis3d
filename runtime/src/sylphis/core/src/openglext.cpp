
#include <string.h>
#include "openglext.h"

#include <console.h>

GLenum glIncrStencil;
GLenum glDecrStencil;
S32 maxMultiTextureUnits = 0;
S32 maxGeneralCombiners = 0;
bool isGLSLSupported = false;
bool isFloatTexturesSupported = false;


void initOpenGlExtensions(){
    gcon.printf("... Initialising opengl extentions\n");

    GLenum err = glewInit();
    if (GLEW_OK != err){
        
    }

    glIncrStencil = GLEW_EXT_stencil_wrap ? GL_INCR_WRAP_EXT : GL_INCR;
    glDecrStencil = GLEW_EXT_stencil_wrap ? GL_DECR_WRAP_EXT : GL_DECR;

	maxMultiTextureUnits = 0;

	if(GLEW_ARB_multitexture){
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &maxMultiTextureUnits);
	}

    gcon.printf("    - Texture units : %d\n", maxMultiTextureUnits);
    if(GLEW_NV_register_combiners){
        glGetIntegerv(GL_MAX_GENERAL_COMBINERS_NV, &maxGeneralCombiners);
        gcon.printf("    - NV register combiners : %d\n", maxGeneralCombiners);
    } else {
        gcon.printf("    - No NV register combiners\n");
    }

    isGLSLSupported = GLEW_ARB_shader_objects
                   && GLEW_ARB_vertex_shader 
                   && GLEW_ARB_fragment_shader;

    if(isGLSLSupported){
        gcon.printf("    - GLSL supported\n");
    } else {
        gcon.printf("    - No GLSL support\n");
    }

	isFloatTexturesSupported = GLEW_ATI_texture_float || GLEW_ARB_texture_float;

	if(isFloatTexturesSupported){
		gcon.printf("    - Floating point textures supported\n");
	} else {
		gcon.printf("    - Floating point textures NOT supported\n");
	}

}

