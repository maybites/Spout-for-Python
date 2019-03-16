//
//		SpoutSend
//
//		High level functions for Spout 2.007
//
// ====================================================================================
//		Revisions :
//
//		11.11.18	- Add high level application functions
//		13.11.18	- Remove CPU mode functions
//		27.11.18	- Add RemovePadding
//		01.12.18	- Add GetFps and GetFrame
//		11.12.18	- Add utility functions
//		14.12.18	- Clean up
//		16.01.19	- Initialize class variables
//		21.01.19	- Add Bind and UnBindSharedTexture
//		26.02.19	- Add IsFrameCountEnabled
//
// ====================================================================================
/*

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
#include "SpoutSend.h"

SpoutSend::SpoutSend()
{
	m_SenderName[0] = 0;
	m_TextureID = 0;
	m_TextureTarget = 0;
	m_Width = 0;
	m_Height = 0;
	m_bInvert = true;
}

//---------------------------------------------------------
SpoutSend::~SpoutSend()
{
	CloseSender();
}


//
// =================== SENDER =======================
//

//---------------------------------------------------------
bool SpoutSend::SetupSender(const char* SenderName,
	unsigned int width, unsigned int height,
	bool bInvert)
{
	strcpy_s(m_SenderName, 256, SenderName);
	m_bInvert = bInvert;
	m_Width = width;
	m_Height = height;
	return sender.CreateSender(m_SenderName, m_Width, m_Height);
}

//---------------------------------------------------------
bool SpoutSend::SendTextureData(GLuint TextureID, GLuint TextureTarget, GLuint HostFbo)
{
	if (sender.IsInitialized())
		return sender.SendTexture(TextureID, TextureTarget, m_Width, m_Height, m_bInvert, HostFbo);
	else
		return false;
}

//---------------------------------------------------------
bool SpoutSend::SendFboData(GLuint FboID)
{
	if (sender.IsInitialized())
		return sender.SendFboTexture(FboID, m_Width, m_Height, m_bInvert);
	else
		return false;
}

//---------------------------------------------------------
bool SpoutSend::SendImageData(const unsigned char* pixels, GLenum glFormat, GLuint HostFbo)
{
	if (sender.IsInitialized())
		return sender.SendImage(pixels, m_Width, m_Height, glFormat, m_bInvert, HostFbo);
	else
		return false;
}

//---------------------------------------------------------
void SpoutSend::Update(unsigned int width, unsigned int height)
{
	if (width != m_Width || height != m_Height) {
		sender.UpdateSender(m_SenderName, width, height);
		m_Width = width;
		m_Height = height;
	}
}

//---------------------------------------------------------
bool SpoutSend::IsInitialized()
{
	return sender.spout.IsSpoutInitialized();
}

//---------------------------------------------------------
void SpoutSend::CloseSender()
{
	if (sender.IsInitialized())
		sender.ReleaseSender();
	m_SenderName[0] = 0;
	m_bInvert = true;
	m_Width = 0;
	m_Height = 0;
}

//---------------------------------------------------------
unsigned int SpoutSend::GetWidth()
{
	return m_Width;
}

//---------------------------------------------------------
unsigned int SpoutSend::GetHeight()
{
	return m_Height;
}

//---------------------------------------------------------
double SpoutSend::GetFps()
{
	return (sender.spout.interop.frame.GetSenderFps());
}

//---------------------------------------------------------
long SpoutSend::GetFrame()
{
	return (sender.spout.interop.frame.GetSenderFrame());
}

//---------------------------------------------------------
void SpoutSend::DisableFrameCount()
{
	sender.spout.interop.frame.DisableFrameCount();
}

//---------------------------------------------------------
bool SpoutSend::IsFrameCountEnabled()
{
	return sender.spout.interop.frame.IsFrameCountEnabled();
}


void SpoutSend::HoldFps(int fps)
{
	sender.spout.interop.frame.HoldFps(fps);
}
