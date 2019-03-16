/*

			SpoutReceiver.h

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
#pragma once

#ifndef __SpoutReceiver__
#define __SpoutReceiver__

#include "spoutSDK.h"

class SPOUT_DLLEXP SpoutReceiver {

public:

	SpoutReceiver();
	~SpoutReceiver();

	//---------------------------------------------------------
	// PYTHON WRAPPING Functions
	//---------------------------------------------------------
	bool pyCreateReceiver(const char* theName, unsigned int theWidth, unsigned int theHeight, bool bUseActive);
	bool pyReceiveTexture(const char* theName, unsigned int theWidth, unsigned int theHeight, GLuint TextureID, GLuint TextureTarget, bool bInvert, GLuint HostFBO);
	unsigned int GetWidth(const char* theName);
	unsigned int GetHeight(const char* theName);
	//---------------------------------------------------------

	/*
	// 2.007 functions
	void SetupReceiver(unsigned int width, unsigned int height, bool bInvert = false);
	void SetReceiverName(const char * SenderName);
	bool ReceiveTextureData(GLuint TextureID, GLuint TextureTarget, GLuint HostFbo = 0);
	bool ReceiveImageData(unsigned char *pixels, GLenum glFormat = GL_RGBA, GLuint HostFbo = 0);
	bool IsUpdated();
	void CloseReceiver();
	bool GetReceiverName(char * name);
	unsigned int GetSenderWidth();
	unsigned int GetSenderHeight();
	double GetSenderFps();
	long GetSenderFrame();
	bool IsFrameNew();
	bool HasNewFrame();
	void DisableFrameCount();
	void HoldFps(int fps = 0);
	*/

	// functions 2.006 and earlier
	bool OpenSpout();
	bool CreateReceiver(char* Sendername, unsigned int &width, unsigned int &height, bool bUseActive = false);
	void ReleaseReceiver();
	bool ReceiveTexture(char* Sendername, unsigned int &width, unsigned int &height, GLuint TextureID = 0, GLuint TextureTarget = 0, bool bInvert = false, GLuint HostFBO = 0);
#ifdef legacyOpenGL
	bool DrawSharedTexture(float max_x = 1.0, float max_y = 1.0, float aspect = 1.0, bool bInvert = true, GLuint HostFBO = 0);
#endif	
	bool ReceiveImage(char* Sendername, unsigned int &width, unsigned int &height, unsigned char* pixels, GLenum glFormat = GL_RGBA, bool bInvert = false, GLuint HostFBO = 0);
	void RemovePadding(const unsigned char *source, unsigned char *dest, unsigned int width, unsigned int height, unsigned int stride, GLenum glFormat = GL_RGBA);
	bool CheckReceiver(char* Sendername, unsigned int &width, unsigned int &height, bool &bConnected);
	bool IsInitialized();

	bool BindSharedTexture();
	bool UnBindSharedTexture();

	int  GetSenderCount();
	bool GetSenderName(int index, char* Sendername, int MaxSize = 256);
	bool GetSenderInfo(const char* Sendername, unsigned int &width, unsigned int &height, HANDLE &dxShareHandle, DWORD &dwFormat);
	bool GetActiveSender(char* Sendername);
	bool SetActiveSender(const char* Sendername);
	bool SelectSenderPanel(const char* message = NULL);

	bool GetDX9();
	bool SetDX9(bool bDX9 = true); // set to use DirectX 9 (default is DirectX 11)
	bool GetMemoryShareMode();
	bool SetMemoryShareMode(bool bMem = true);
	int  GetShareMode();
	bool SetShareMode(int mode);
	bool GetBufferMode();
	void SetBufferMode(bool bActive); // Set the pbo availability on or off

	bool GetDX9compatible();
	void SetDX9compatible(bool bCompatible = true);

	int  GetNumAdapters(); // Get the number of graphics adapters in the system
	bool GetAdapterName(int index, char *adaptername, int maxchars); // Get an adapter name
	int  GetAdapter(); // Get the current adapter index
	bool SetAdapter(int index = 0); // Set required graphics adapter for output

	int  GetMaxSenders(); // Get maximum senders allowed
	void SetMaxSenders(int maxSenders); // Set maximum senders allowed
	bool GetHostPath(const char *sendername, char *hostpath, int maxchars); // The path of the host that produced the sender
	int  GetVerticalSync();
	bool SetVerticalSync(bool bSync = true);

	Spout spout; // for access to all functions

protected:

	/*
	char m_SenderNameSetup[256];
	char m_SenderName[256];
	GLuint m_TextureID;
	GLuint m_TextureTarget;
	bool m_bInvert;
	bool m_bUpdate;
	bool m_bUseActive;
	unsigned int m_Width;
	unsigned int m_Height;
	*/

};

#endif
