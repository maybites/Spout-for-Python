//
// TODO : SpoutSender IsInitialized() necessary ?
// TODO : HoldFps necessary for a receiver ?
// TODO : Add IsFrameCountEnabled
//
//		SpoutReceive
//
//		High level functions for Spout 2.007
//
// ====================================================================================
//		Revisions :
//
//		11.11.18	- Add high level application functions
//		13.11.18	- Remove SetCPUmode, GetCPUmode
//		24.11.18	- Remove redundant GetImageSize
//		28.11.18	- Add IsFrameNew and HasNewFrame
//		11.12.18	- Add utility functions
//		05.01.19	- Change names for high level receiver functions for SpoutLibrary
//		16.01.19	- Initialize class variables
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
#include "SpoutReceive.h"

SpoutReceive::SpoutReceive()
{
	m_SenderNameSetup[0] = 0;
	m_SenderName[0] = 0;
	m_TextureID = 0;
	m_TextureTarget = 0;
	m_bInvert = false;
	m_bUpdate = false;
	m_bUseActive = false;
	m_Width = 0;
	m_Height = 0;

}

//---------------------------------------------------------
SpoutReceive::~SpoutReceive()
{
	CloseReceiver();
}

//
// 2.007 functions
//

//---------------------------------------------------------
void SpoutReceive::SetupReceiver(unsigned int width, unsigned int height, bool bInvert)
{
	// CreateReceiver will use the active sender
	// unless the user has specified a sender to 
	// connect to using SetReceiverName
	m_SenderNameSetup[0] = 0;
	m_SenderName[0] = 0;
	m_bUseActive = true;

	// Record details for subsequent functions
	m_Width = width;
	m_Height = height;
	m_bInvert = bInvert;
	m_bUpdate = false;

}

//---------------------------------------------------------
void SpoutReceive::SetReceiverName(const char * SenderName)
{
	if (SenderName && SenderName[0]) {
		strcpy_s(m_SenderNameSetup, 256, SenderName);
		strcpy_s(m_SenderName, 256, SenderName);
		m_bUseActive = false; // the user has specified a sender to connect to
	}
}

//---------------------------------------------------------
bool SpoutReceive::ReceiveTextureData(GLuint TextureID, GLuint TextureTarget, GLuint HostFbo)
{
	m_bUpdate = false;

	// Initialization is recorded in the spout class for sender or receiver
	if (!receiver.IsInitialized()) {
		if (receiver.CreateReceiver(m_SenderName, m_Width, m_Height, m_bUseActive)) {
			// Signal the application to update the receiving texture size
			// Retrieved with a call to the Updated function
			m_bUpdate = true;
			return true;
		}
	}
	else {
		// Save sender name and dimensions to test for change
		char name[256];
		strcpy_s(name, 256, m_SenderName);
		unsigned int width = m_Width;
		unsigned int height = m_Height;
		// Receive a shared texture but don't read it into the user texture yet
		if (receiver.ReceiveTexture(name, width, height)) {
			// Test for sender name or size change
			if (width != m_Width
				|| height != m_Height
				|| strcmp(name, m_SenderName) != 0) {
				// Update name
				strcpy_s(m_SenderName, 256, name);
				// Update class dimensions
				m_Width = width;
				m_Height = height;
				// Signal the application to update the receiving texture
				m_bUpdate = true;
				return true;
			}
			else {
				// Read the shared texture to the user texture
				return receiver.spout.interop.ReadTexture(TextureID, TextureTarget, m_Width, m_Height, false, HostFbo);
			}
		}
		else {
			// receiving failed
			CloseReceiver();
			return false;
		}
	}

	// No connection
	return false;

}

//---------------------------------------------------------
bool SpoutReceive::ReceiveImageData(unsigned char *pixels, GLenum glFormat, GLuint HostFbo)
{
	m_bUpdate = false;

	if (!receiver.IsInitialized()) {
		if (receiver.CreateReceiver(m_SenderName, m_Width, m_Height, m_bUseActive)) {
			m_bUpdate = true;
			return true;
		}
	}
	else {
		char sendername[256];
		strcpy_s(sendername, 256, m_SenderName);
		unsigned int width = m_Width;
		unsigned int height = m_Height;
		// Receive a shared image but don't read it into the user pixels yet
		if (receiver.ReceiveImage(sendername, width, height, NULL)) {
			// Test for sender name or size change
			if (width != m_Width
				|| height != m_Height
				|| strcmp(m_SenderName, sendername) != 0) {
				// Update the connected sender name
				strcpy_s(m_SenderName, 256, sendername);
				// Update class dimensions
				m_Width = width;
				m_Height = height;
				// Signal the application to update the receiving pixels
				m_bUpdate = true;
				return true;
			}
			else {
				// Read the shared texture or memory directly into the pixel buffer
				// Functions handle the formats supported
				return(receiver.spout.interop.ReadTexturePixels(pixels, width, height, glFormat, m_bInvert, HostFbo));
			}
		}
		else {
			// receiving failed
			CloseReceiver();
			return false;
		}
	}

	// No connection
	return false;

}

//---------------------------------------------------------
bool SpoutReceive::IsUpdated()
{
	// Return whether the application texture needs updating.
	// The application must update the receiving texture before
	// the next call to ReceiveTexture when the update flag is reset.
	return m_bUpdate;
}

//---------------------------------------------------------
bool SpoutReceive::IsInitialized()
{
	return receiver.spout.IsSpoutInitialized();
}

//---------------------------------------------------------
void SpoutReceive::CloseReceiver()
{
	receiver.ReleaseReceiver();
	// Restore the sender name that the user specified in SetupReceiver
	strcpy_s(m_SenderName, 256, m_SenderNameSetup);
	m_Width = 0;
	m_Height = 0;
}

//---------------------------------------------------------
void SpoutReceive::SelectSender()
{
	receiver.SelectSenderPanel();
}

//---------------------------------------------------------
char * SpoutReceive::GetReceiverName()
{
	return m_SenderName;
}

//---------------------------------------------------------
unsigned int SpoutReceive::GetSenderWidth()
{
	return m_Width;
}

//---------------------------------------------------------
unsigned int SpoutReceive::GetSenderHeight()
{
	return m_Height;
}

//---------------------------------------------------------
double SpoutReceive::GetSenderFps()
{
	return (receiver.spout.interop.frame.GetSenderFps());
}

//---------------------------------------------------------
long SpoutReceive::GetSenderFrame()
{
	return (receiver.spout.interop.frame.GetSenderFrame());
}

//---------------------------------------------------------
bool SpoutReceive::IsFrameNew()
{
	return (receiver.spout.interop.frame.IsFrameNew());
}

//---------------------------------------------------------
bool SpoutReceive::HasNewFrame()
{
	return (receiver.spout.interop.frame.HasNewFrame());
}

//---------------------------------------------------------
void SpoutReceive::DisableFrameCount()
{
	receiver.spout.interop.frame.DisableFrameCount();
}

void SpoutReceive::HoldFps(int fps)
{
	receiver.spout.interop.frame.HoldFps(fps);
}






