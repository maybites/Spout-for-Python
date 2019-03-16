/*
//
//
//			spoutGLextensions.cpp
//
//			Used for load of openGL extensions with option
//			to use Glew or disable dynamic load of specific extensions
//			See spoutGLext.h
//
//			01.09.15	- added MessageBox error warning in LoadGLextensions
//			11.11.15	- removed (unsigned) cast from GetProcAddress in FBO extensions
//			17.03.16	- added bgra extensions to find out if they are supported at compile and runtime
//			28.03.16	- caps is returned instead of fail for interop extensions
//			29.03.16	- Fixed loadInteropExtensions flag test in loadGLextensions
//			12.08.16	- Removed "isExtensionSupported" (https://github.com/leadedge/Spout2/issues/19)
//			13.01.17	- Removed try/catch from wglDXRegisterObjectNV calls
//						- Clean up #ifdefs in all functions - return true if FBO of PBO are defined elsewhere
//			27.10.18	- Test for opengl context in loadglextensions
//			21.11.18	- Add copy extensions for future use
//			23.11.18	- Fix test for wglDXCloseDeviceNV in loadInteropExtensions
//

	Copyright (c) 2014-2019, Lynn Jarvis. All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, 
	are permitted provided that the following conditions are met:

		1. Redistributions of source code must retain the above copyright notice, 
		   this list of conditions and the following disclaimer.

		2. Redistributions in binary form must reproduce the above copyright notice, 
		   this list of conditions and the following disclaimer in the documentation 
		   and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"	AND ANY 
	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE	ARE DISCLAIMED. 
	IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "spoutGLextensions.h"

#ifndef USE_GLEW

// GL/DX extensions
PFNWGLDXOPENDEVICENVPROC				wglDXOpenDeviceNV				= NULL;
PFNWGLDXREGISTEROBJECTNVPROC			wglDXRegisterObjectNV			= NULL;
PFNWGLDXSETRESOURCESHAREHANDLENVPROC	wglDXSetResourceShareHandleNV	= NULL;
PFNWGLDXLOCKOBJECTSNVPROC				wglDXLockObjectsNV				= NULL;
PFNWGLDXUNLOCKOBJECTSNVPROC				wglDXUnlockObjectsNV			= NULL;
PFNWGLDXCLOSEDEVICENVPROC				wglDXCloseDeviceNV				= NULL;
PFNWGLDXUNREGISTEROBJECTNVPROC			wglDXUnregisterObjectNV			= NULL;

// FBO extensions
#ifdef USE_FBO_EXTENSIONS
glBindFramebufferEXTPROC				glBindFramebufferEXT			= NULL;
glBindRenderbufferEXTPROC				glBindRenderbufferEXT			= NULL;
glCheckFramebufferStatusEXTPROC			glCheckFramebufferStatusEXT		= NULL;
glDeleteFramebuffersEXTPROC				glDeleteFramebuffersEXT			= NULL;
glDeleteRenderBuffersEXTPROC			glDeleteRenderBuffersEXT		= NULL;
glFramebufferRenderbufferEXTPROC		glFramebufferRenderbufferEXT	= NULL;
glFramebufferTexture1DEXTPROC			glFramebufferTexture1DEXT		= NULL;
glFramebufferTexture2DEXTPROC			glFramebufferTexture2DEXT		= NULL;
glFramebufferTexture3DEXTPROC			glFramebufferTexture3DEXT		= NULL;
glGenFramebuffersEXTPROC				glGenFramebuffersEXT			= NULL;
glGenRenderbuffersEXTPROC				glGenRenderbuffersEXT			= NULL;
glGenerateMipmapEXTPROC					glGenerateMipmapEXT				= NULL;
glGetFramebufferAttachmentParameterivEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
glGetRenderbufferParameterivEXTPROC		glGetRenderbufferParameterivEXT	= NULL;
glIsFramebufferEXTPROC					glIsFramebufferEXT				= NULL;
glIsRenderbufferEXTPROC					glIsRenderbufferEXT				= NULL;
glRenderbufferStorageEXTPROC			glRenderbufferStorageEXT		= NULL;
#endif

// FBO blit extensions
glBlitFramebufferEXTPROC				glBlitFramebufferEXT			= NULL;

#ifdef USE_FBO_EXTENSIONS
// OpenGL sync control extensions
PFNWGLSWAPINTERVALEXTPROC				wglSwapIntervalEXT				= NULL;
PFNWGLGETSWAPINTERVALEXTPROC			wglGetSwapIntervalEXT			= NULL;
#endif

// PBO extensions
#ifdef USE_PBO_EXTENSIONS
glGenBuffersPROC						glGenBuffersEXT					= NULL;
glDeleteBuffersPROC						glDeleteBuffersEXT				= NULL;
glBindBufferPROC						glBindBufferEXT					= NULL;
glBufferDataPROC						glBufferDataEXT					= NULL;
glMapBufferPROC							glMapBufferEXT					= NULL;
glUnmapBufferPROC						glUnmapBufferEXT				= NULL;
#endif



//-------------------
// Copy extensions
//-------------------
#ifdef USE_COPY_EXTENSIONS
PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData = NULL;
#endif

//---------------------------
// Context creation extension
//---------------------------
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

#endif

//
// Load the Nvidia-Extensions dynamically
//
bool loadInteropExtensions() {

#ifdef USE_GLEW
	if(WGLEW_NV_DX_interop)
		return true;
	else
		return false;
#else

	// Here we provide warnings for individual extensions

	wglDXOpenDeviceNV = (PFNWGLDXOPENDEVICENVPROC)wglGetProcAddress("wglDXOpenDeviceNV");
	if(!wglDXOpenDeviceNV) {
		SpoutLogWarning("loadInteropExtensions : wglDXOpenDeviceNV NULL");
		return false;
	}
	wglDXRegisterObjectNV = (PFNWGLDXREGISTEROBJECTNVPROC)wglGetProcAddress("wglDXRegisterObjectNV");
	if(!wglDXRegisterObjectNV) {
		SpoutLogWarning("loadInteropExtensions : wglDXRegisterObjectNV NULL");
		return false;
	}
	wglDXUnregisterObjectNV = (PFNWGLDXUNREGISTEROBJECTNVPROC)wglGetProcAddress("wglDXUnregisterObjectNV");
	if(!wglDXUnregisterObjectNV) {
		SpoutLogWarning("loadInteropExtensions : wglDXUnregisterObjectNV NULL");
		return false;
	}
	wglDXSetResourceShareHandleNV = (PFNWGLDXSETRESOURCESHAREHANDLENVPROC)wglGetProcAddress("wglDXSetResourceShareHandleNV");
	if(!wglDXSetResourceShareHandleNV) {
		SpoutLogWarning("loadInteropExtensions : wglDXSetResourceShareHandleNV NULL");
		return false;
	}
	wglDXLockObjectsNV = (PFNWGLDXLOCKOBJECTSNVPROC)wglGetProcAddress("wglDXLockObjectsNV");
	if(!wglDXLockObjectsNV)	{
		SpoutLogWarning("loadInteropExtensions : wglDXLockObjectsNV NULL");
		return false;
	}
	wglDXUnlockObjectsNV = (PFNWGLDXUNLOCKOBJECTSNVPROC)wglGetProcAddress("wglDXUnlockObjectsNV");
	if(!wglDXUnlockObjectsNV) {
		SpoutLogWarning("loadInteropExtensions : wglDXUnlockObjectsNV NULL");
		return false;
	}
	wglDXCloseDeviceNV = (PFNWGLDXCLOSEDEVICENVPROC)wglGetProcAddress("wglDXCloseDeviceNV");
	if(!wglDXCloseDeviceNV) {
		SpoutLogWarning("loadInteropExtensions : wglDXCloseDeviceNV NULL");
		return false;
	}

	return true;
#endif

}

bool loadFBOextensions() {

	// Here we use 'EXT_framebuffer_object'
	// But for OpenGL version >= 3, framebuffer objects are core.
	// Control this using the "legacyOpenGL" define in SpoutCommon.h

	// Thanks and credit to Menno Vink of Resolume for sharing the POSTFIX code
	
#ifdef legacyOpenGL
	#define FBO_EXTENSION_POSTFIX "EXT"
#else
	#define FBO_EXTENSION_POSTFIX
#endif

#ifdef USE_FBO_EXTENSIONS

	#ifdef USE_GLEW
	if(GLEW_EXT_framebuffer_object)
		return true;
	else
		return false;
	#else

	glBindFramebufferEXT                     = (glBindFramebufferEXTPROC)wglGetProcAddress("glBindFramebuffer" FBO_EXTENSION_POSTFIX);
	glBindRenderbufferEXT                    = (glBindRenderbufferEXTPROC)wglGetProcAddress("glBindRenderbuffer" FBO_EXTENSION_POSTFIX);
	glCheckFramebufferStatusEXT              = (glCheckFramebufferStatusEXTPROC)wglGetProcAddress("glCheckFramebufferStatus" FBO_EXTENSION_POSTFIX);
	glDeleteFramebuffersEXT                  = (glDeleteFramebuffersEXTPROC)wglGetProcAddress("glDeleteFramebuffers" FBO_EXTENSION_POSTFIX);
	glDeleteRenderBuffersEXT                 = (glDeleteRenderBuffersEXTPROC)wglGetProcAddress("glDeleteRenderbuffers" FBO_EXTENSION_POSTFIX);
	glFramebufferRenderbufferEXT             = (glFramebufferRenderbufferEXTPROC)wglGetProcAddress("glFramebufferRenderbuffer" FBO_EXTENSION_POSTFIX);
	glFramebufferTexture1DEXT                = (glFramebufferTexture1DEXTPROC)wglGetProcAddress("glFramebufferTexture1D" FBO_EXTENSION_POSTFIX);
	glFramebufferTexture2DEXT                = (glFramebufferTexture2DEXTPROC)wglGetProcAddress("glFramebufferTexture2D" FBO_EXTENSION_POSTFIX);
	glFramebufferTexture3DEXT                = (glFramebufferTexture3DEXTPROC)wglGetProcAddress("glFramebufferTexture3D" FBO_EXTENSION_POSTFIX);
	glGenFramebuffersEXT                     = (glGenFramebuffersEXTPROC)wglGetProcAddress("glGenFramebuffers" FBO_EXTENSION_POSTFIX);
	glGenRenderbuffersEXT                    = (glGenRenderbuffersEXTPROC)wglGetProcAddress("glGenRenderbuffers" FBO_EXTENSION_POSTFIX);
	glGenerateMipmapEXT                      = (glGenerateMipmapEXTPROC)wglGetProcAddress("glGenerateMipmap" FBO_EXTENSION_POSTFIX);
	glGetFramebufferAttachmentParameterivEXT = (glGetFramebufferAttachmentParameterivEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv" FBO_EXTENSION_POSTFIX);
	glGetRenderbufferParameterivEXT          = (glGetRenderbufferParameterivEXTPROC)wglGetProcAddress("glGetRenderbufferParameteriv" FBO_EXTENSION_POSTFIX);
	glIsFramebufferEXT                       = (glIsFramebufferEXTPROC)wglGetProcAddress("glIsFramebuffer" FBO_EXTENSION_POSTFIX);
	glIsRenderbufferEXT                      = (glIsRenderbufferEXTPROC)wglGetProcAddress("glIsRenderbuffer" FBO_EXTENSION_POSTFIX);
	glRenderbufferStorageEXT                 = (glRenderbufferStorageEXTPROC)wglGetProcAddress("glRenderbufferStorage" FBO_EXTENSION_POSTFIX);

	if	  ( glBindFramebufferEXT						!= NULL && 
			glBindRenderbufferEXT						!= NULL && 
			glCheckFramebufferStatusEXT					!= NULL && 
			glDeleteFramebuffersEXT						!= NULL && 
			glDeleteRenderBuffersEXT					!= NULL &&
			glFramebufferRenderbufferEXT				!= NULL && 
			glFramebufferTexture1DEXT					!= NULL && 
			glFramebufferTexture2DEXT					!= NULL && 
			glFramebufferTexture3DEXT					!= NULL && 
			glGenFramebuffersEXT						!= NULL &&
			glGenRenderbuffersEXT						!= NULL && 
			glGenerateMipmapEXT							!= NULL && 
			glGetFramebufferAttachmentParameterivEXT	!= NULL && 
			glGetRenderbufferParameterivEXT				!= NULL && 
			glIsFramebufferEXT							!= NULL &&
			glIsRenderbufferEXT							!= NULL && 
			glRenderbufferStorageEXT					!= NULL) {
		return true;
	}
	else {
		return false;
	}
	#endif
#else
	// FBO extensions defined elsewhere
	return true;
#endif
}


bool loadBLITextension() {

#ifdef USE_GLEW
	if(GLEW_EXT_framebuffer_blit)
		return true;
	else
		return false;
#else
	glBlitFramebufferEXT = (glBlitFramebufferEXTPROC) wglGetProcAddress("glBlitFramebufferEXT");
	return glBlitFramebufferEXT!=NULL;
#endif

}

bool loadSwapExtensions()
{
	wglSwapIntervalEXT    = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglGetSwapIntervalEXT = (PFNWGLGETSWAPINTERVALEXTPROC)wglGetProcAddress("wglGetSwapIntervalEXT");
	if(wglSwapIntervalEXT == NULL || wglGetSwapIntervalEXT == NULL) {
		return false;
	}

	return true;

}


// =================== PBO support 18.01.14 ==================
bool loadPBOextensions() 
{

#ifdef USE_PBO_EXTENSIONS

	#ifdef USE_GLEW
	if(glGenBuffersARB)
		return true;
	else
		return false;
	#else
	glGenBuffersEXT	   = (glGenBuffersPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffersEXT = (glDeleteBuffersPROC)wglGetProcAddress("glDeleteBuffers");
	glBindBufferEXT	   = (glBindBufferPROC)wglGetProcAddress("glBindBuffer");
	glBufferDataEXT	   = (glBufferDataPROC)wglGetProcAddress("glBufferData");
	glMapBufferEXT     = (glMapBufferPROC)wglGetProcAddress("glMapBuffer");
	glUnmapBufferEXT   = (glUnmapBufferPROC)wglGetProcAddress("glUnmapBuffer");

	if(glGenBuffersEXT != NULL && glDeleteBuffersEXT != NULL
	&& glBindBufferEXT != NULL && glBufferDataEXT    != NULL
	&& glMapBufferEXT  != NULL && glUnmapBufferEXT   != NULL) {
		return true;
	}
	else {
		return false;
	}
	#endif

#else
	// PBO extensions defined elsewhere
	return true;
#endif
}



bool loadCopyExtensions()
{

#ifdef USE_COPY_EXTENSIONS

#ifdef USE_GLEW
	if (glCopyImageSubData)
		return true;
	else
		return false;
#else

	// Copy extensions
	glCopyImageSubData = (PFNGLCOPYIMAGESUBDATAPROC)wglGetProcAddress("glCopyImageSubData");

	if (glCopyImageSubData != NULL) {
		return true;
	}
	else {
		return false;
	}
#endif

#else
	// COPY extensions defined elsewhere
	return true;
#endif

}


bool loadContextExtension()
{

#ifdef USE_CONTEXT_EXTENSION

	// Return if loadContextExtension() has been called before loading all the extensions
	if (wglCreateContextAttribsARB) {
		return true;
	}

#ifdef USE_GLEW
	if (wglCreateContextAttribsARB)
		return true;
	else
		return false;
#else

	// Context creation extension
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	if (wglCreateContextAttribsARB != NULL) {
		return true;
	}
	else {
		return false;
	}
#endif

#else
	// Context creation extension defined elsewhere
	return true;
#endif

}


bool InitializeGlew()
{
#ifdef USE_GLEW
	HGLRC glContext;
	GLenum glew_error;

	// Opengl context is necessary
	glContext = wglGetCurrentContext();
	if(glContext == NULL) {
		return false;
	}
	//
	// Note from Glew : GLEW obtains information on the supported extensions from the graphics driver.
	// Experimental or pre-release drivers, however, might not report every available extension through
	// the standard mechanism, in which case GLEW will report it unsupported. To circumvent this situation,
	// the glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(),
	// which ensures that all extensions with valid entry points will be exposed. 
	//
	glewExperimental = GL_TRUE;
	glew_error = glewInit();
	if (glew_error != GLEW_OK) {
		return false;
	}
	//
	// Glew should have loaded all the extensions and we can check for them
	//
	// http://glew.sourceforge.net/basic.html
	//
	return true;
#else
	// Glew usage not defined so cannot initialize
	return false;
#endif
}


//
// Load GL extensions
//
unsigned int loadGLextensions() {
	
	unsigned int caps = 0; // as per elio glextensions

	// wglGetProcAddress requires an OpenGL rendering context
	HGLRC glContext = wglGetCurrentContext();
	if (glContext == NULL) {
		spoututils::SpoutLogError("loadGLextensions : no OpenGL context");
		return 0;
	}

#ifdef USE_GLEW
	InitializeGlew(); // probably needs failure check
#endif

	/*
	// LJ DEBUG
	printf("LoadGLextensions()\n");

	printf("GL_version  = %s\n", (char *)glGetString(GL_VERSION));
	printf("GL_vendor   = %s\n", (char *)glGetString(GL_VENDOR));
	printf("GL_renderer = %s\n", (char *)glGetString(GL_RENDERER));
	// printf("gl_extensions = %s\n", reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
	int major, minor = -1;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("  Major : %d\n", major);
	printf("  Minor : %d\n", minor);
	int profile_mask = -1;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile_mask);
	printf("Profile mask : %d\n", profile_mask);
	// This can contain the bits GL_CONTEXT_CORE_PROFILE_BIT
	// or GL_CONTEXT_COMPATIBILITY_PROFILE_BIT, but not both at the same time.
	if (profile_mask & GL_CONTEXT_CORE_PROFILE_BIT)
		printf("    GL_CONTEXT_CORE_PROFILE_BIT\n");
	if (profile_mask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT)
		printf("    GL_CONTEXT_COMPATIBILITY_PROFILE_BIT\n");
	int context_flags = -1;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);
	printf("Context flags : %d\n", context_flags);
	if (context_flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT)
		printf("    GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT\n");
	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		printf("    GL_CONTEXT_FLAG_DEBUG_BIT\n");
	if (context_flags & GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT)
		printf("    GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT\n");
	if (context_flags & GL_CONTEXT_FLAG_NO_ERROR_BIT)
		printf("    GL_CONTEXT_FLAG_NO_ERROR_BIT\n");
	// printf("gl_shading_language = %s\n", reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (GL_VERSION >= 3.0)
		printf("OpenGL 3-4\n");
	else
		printf("OpenGL 2\n");
	*/

	// Check for FBO extensions first - no use continuing without them
	if (loadFBOextensions()) {
		caps |= GLEXT_SUPPORT_FBO;
	}
	else {
		spoututils::SpoutLogError("loadGLextensions : loadFBOextensions fail");
		return 0;
	}

	// Load other extensions
	if(loadBLITextension()) {
		caps |= GLEXT_SUPPORT_FBO_BLIT;
	}
	else {
		spoututils::SpoutLogWarning("loadGLextensions : loadBLITextensions fail");
	}

	if(loadSwapExtensions()) {
		caps |= GLEXT_SUPPORT_SWAP;
	}
	else {
		spoututils::SpoutLogWarning("loadGLextensions : loadSwapExtensions fail");
	}

	if(loadPBOextensions()) {
		caps |= GLEXT_SUPPORT_PBO;
	}
	else {
		spoututils::SpoutLogWarning("loadGLextensions : loadPBOextensions fail");
	}

	if (loadCopyExtensions()) {
		caps |= GLEXT_SUPPORT_COPY;
	}
	else {
		spoututils::SpoutLogWarning("loadGLextensions : loadCopyExtensions fail");
	}

	if (loadContextExtension()) {
		caps |= SUPPORT_CONTEXT_EXTENSION;
	}
	else {
		spoututils::SpoutLogWarning("loadGLextensions : loadContextExtension fail");
	}

	// Load wgl interop extensions
	if (loadInteropExtensions()) {
		caps |= GLEXT_SUPPORT_NVINTEROP;
	}
	else {
		spoututils::SpoutLogWarning("loadGLextensions : loadInteropExtensions fail");
	}

	spoututils::SpoutLogNotice("loadGLextensions : extensions loaded successfully");

	// Find out whether bgra extensions are supported at compile and runtime
#ifdef GL_EXT_bgra
	//
	// "isExtensionSupported" code yet to be fully tested for
	// various compilers, operating systems and environments.
	// De-activate this function if you have problems.
	// 
	if (isExtensionSupported("GL_EXT_bgra")) {
		caps |= GLEXT_SUPPORT_BGRA;
	}
#endif

	return caps;

}


//
// Used to determine support for GL_EXT_bgra extensions
//
bool isExtensionSupported(const char *extension)
{
	char * where = nullptr;
	char * terminator = nullptr;
	int n = 0;
	int i = 0;

	if (!extension || *extension == '\0')
		return false;

	// Extension names should not have spaces.
	if(strchr(extension, ' '))
		return false;

	const char * extensionsstr = (const char *)glGetString(GL_EXTENSIONS);
	if (extensionsstr) {
		std::string extensions = extensionsstr;
		std::size_t found = extensions.find(extension);
		if (found != std::string::npos) {
			SpoutLogNotice("extension [%s] found", extension, found);
			return true;
		}
		SpoutLogNotice("extension [%s] not found", extension);
		return false;
	}
	else {

		SpoutLogNotice("glGetString(GL_EXTENSIONS) not supported - using glGetStringi");

		//
		// glGetstring not supported
		// for a core GL context
		//
		// Code adapted from : https://bitbucket.org/Coin3D/coin/issues/54/support-for-opengl-3x-specifically
		// Also : http://www.opengl.org/resources/features/OGLextensions/
		//

		typedef GLubyte* (APIENTRY * COIN_PFNGLGETSTRINGIPROC)(GLenum enm, GLuint idx);
		COIN_PFNGLGETSTRINGIPROC glGetStringi = 0;
		glGetStringi = (COIN_PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
		if(glGetStringi != NULL) {
			glGetIntegerv(GL_NUM_EXTENSIONS, &n);
			if(n > 0) {
				const char * exc = nullptr;
				for (i = 0; i < n; i++) {
					exc = (const char *)glGetStringi(GL_EXTENSIONS, i);
					if(exc) {
						if(strcmp(exc, extension) == 0)
							break;
					}
				}
				if(exc && i < n) {
					SpoutLogNotice("extension [%s] found", extension);
					return true;
				}
				SpoutLogNotice("extension [%s] not found", extension);
				return false;
			}
			else {
				SpoutLogWarning("glGetIntegerv(GL_NUM_EXTENSIONS) did not return a value");
			}
		}
		else {
			SpoutLogWarning("glGetStringi not found");
		}
	} 

	SpoutLogNotice("unable to find extension [%s]", extension);

	return false;

}

