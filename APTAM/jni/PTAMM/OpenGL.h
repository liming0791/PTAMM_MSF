// Copyright 2008 Isis Innovation Limited
#ifndef __OPENGL_INCLUDES_H
#define __OPENGL_INCLUDES_H

#ifdef _LINUX
#include <GL/gl.h>
#include <GL/glext.h>
#endif

#ifdef _OSX
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <GL/glew.h>
#endif

#ifdef __ANDROID__
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <cvd/gl_helpers.h>
#include <android/log.h>
#endif

inline void CheckGLError(const char* prefix)
{
	GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
	    //cerr << "OpenGL error: " << err << endl;
	    __android_log_print(ANDROID_LOG_INFO, "opengl error", "%s: %d",prefix, err);
    }
}

#endif
