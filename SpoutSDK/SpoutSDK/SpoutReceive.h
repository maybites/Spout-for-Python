/*

			SpoutReceive.h

	Copyright (c) 2019, Lynn Jarvis. All rights reserved.

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
#pragma once

#ifndef __SpoutReceive__
#define __SpoutReceive__

#include "SpoutReceiver.h"

class SPOUT_DLLEXP SpoutReceive {

	public:

	SpoutReceive();
    ~SpoutReceive();

	// 2.007 functions
	void SetupReceiver(unsigned int width, unsigned int height, bool bInvert = false);
	void SetReceiverName(const char * SenderName);
	bool ReceiveTextureData(GLuint TextureID, GLuint TextureTarget, GLuint HostFbo = 0);
	bool ReceiveImageData(unsigned char *pixels, GLenum glFormat = GL_RGBA, GLuint HostFbo = 0);
	bool IsUpdated();
	bool IsInitialized();
	void CloseReceiver();
	void SelectSender();
	// TODO : naming ?
	char *GetReceiverName();
	unsigned int GetSenderWidth();
	unsigned int GetSenderHeight();
	double GetSenderFps();
	long GetSenderFrame();
	bool IsFrameNew();
	bool HasNewFrame();
	void DisableFrameCount();
	void HoldFps(int fps = 0);

	SpoutReceiver receiver; // for access to all receiver functions

protected :

	char m_SenderNameSetup[256];
	char m_SenderName[256];
	GLuint m_TextureID;
	GLuint m_TextureTarget;
	bool m_bInvert;
	bool m_bUpdate;
	bool m_bUseActive;
	unsigned int m_Width;
	unsigned int m_Height;

};

#endif
