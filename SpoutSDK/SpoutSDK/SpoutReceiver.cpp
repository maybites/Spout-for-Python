//
//		SpoutReceiver
//
//		Wrapper class so that a receiver object can be created independent of a sender
//
// ====================================================================================
//		Revisions :
//
//		27-07-14	- CreateReceiver - bUseActive flag instead of null name
//		03.09.14	- Cleanup
//		23.09.14	- return DirectX 11 capability in SetDX9
//		28.09.14	- Added Host FBO for ReceiveTexture
//		12.10.14	- changed SelectSenderPanel arg to const char
//		23.12.14	- added host fbo arg to ReceiveImage
//		08.02.15	- Changed default texture format for ReceiveImage in header to GL_RGBA
//		29.05.15	- Included SetAdapter for multiple adapters - Franz Hildgen.
//		02.06.15	- Added GetAdapter, GetNumAdapters, GetAdapterName
//		24.08.15	- Added GetHostPath to retrieve the path of the host that produced the sender
//		15.09.15	- Removed SetMemoryShareMode for 2.005 - now done globally by SpoutDirectX.exe
//		10.10.15	- Added transition flag to set invert true for 2.004 rather than default false for 2.005
//					- currently not used - see SpoutSDK.cpp CreateSender
//		14.11.15	- changed functions to "const char *" where required
//		18.11.15	- added CheckReceiver so that DrawSharedTexture can be used by a receiver
//		18.06.16	- Add invert to ReceiveImage
//		17.09.16	- removed CheckSpout2004() from constructor
//		13.01.17	- Add SetCPUmode, GetCPUmode, SetBufferMode, GetBufferMode
//					- Add HostFBO arg to DrawSharedTexture
//		15.01.17	- Add GetShareMode, SetShareMode
//		06.06.17	- Add OpenSpout
//		05.11.18	- Add IsSpoutInitialized
//
// ====================================================================================
/*
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
#include "SpoutReceiver.h"

SpoutReceiver::SpoutReceiver()
{

}

//---------------------------------------------------------
SpoutReceiver::~SpoutReceiver()
{
	ReleaseReceiver();
}

//---------------------------------------------------------
// PYTHON WRAPPING Functions
//---------------------------------------------------------
bool SpoutReceiver::pyCreateReceiver(const char* theName, unsigned int theWidth, unsigned int theHeight, bool bUseActive)
{
	char name[256];

	strcpy_s(name, 256, theName);
	unsigned int width = theWidth;
	unsigned int height = theHeight;

	bool bRet = spout.CreateReceiver(name, width, height, bUseActive);

	if (bRet) {
		//strcpy_s(theName, 256, name);
		theWidth = width;
		theHeight = height;
	}

	return bRet;
}

//---------------------------------------------------------
bool SpoutReceiver::pyReceiveTexture(const char* theName, unsigned int theWidth, unsigned int theHeight, GLuint TextureID, GLuint TextureTarget, bool bInvert, GLuint HostFBO)
{
	char name[256];

	strcpy_s(name, 256, theName);
	unsigned int width = theWidth;
	unsigned int height = theHeight;

	return spout.ReceiveTexture(name, width, height, TextureID, TextureTarget, bInvert, HostFBO);
}

//---------------------------------------------------------
unsigned int  SpoutReceiver::GetWidth(const char* theName) {
	unsigned int width;
	unsigned int height;
	HANDLE handle;
	DWORD dword;
	if (GetSenderInfo(theName, width, height, handle, dword)) {
		return width;
	}
	else {
		return 0;
	}
}

//---------------------------------------------------------
unsigned int  SpoutReceiver::GetHeight(const char* theName) {
	unsigned int width;
	unsigned int height;
	HANDLE handle;
	DWORD dword;
	if (GetSenderInfo(theName, width, height, handle, dword)) {
		return height;
	}
	else {
		return 0;
	}
}
//---------------------------------------------------------
//---------------------------------------------------------

//---------------------------------------------------------
bool SpoutReceiver::OpenSpout()
{
	return spout.OpenSpout();
}

//---------------------------------------------------------
bool SpoutReceiver::CreateReceiver(char* name, unsigned int &width, unsigned int &height, bool bUseActive)
{
	return spout.CreateReceiver(name, width, height, bUseActive);
}

//---------------------------------------------------------
void SpoutReceiver::ReleaseReceiver()
{
	spout.ReleaseReceiver();
}

//---------------------------------------------------------
bool SpoutReceiver::ReceiveTexture(char* name, unsigned int &width, unsigned int &height, GLuint TextureID, GLuint TextureTarget, bool bInvert, GLuint HostFBO)
{
	return spout.ReceiveTexture(name, width, height, TextureID, TextureTarget, bInvert, HostFBO);
}

#ifdef legacyOpenGL
//---------------------------------------------------------
bool SpoutReceiver::DrawSharedTexture(float max_x, float max_y, float aspect, bool bInvert, GLuint HostFBO)
{
	return spout.DrawSharedTexture(max_x, max_y, aspect, bInvert, HostFBO);
}
#endif

//---------------------------------------------------------
bool SpoutReceiver::ReceiveImage(char* Sendername,
	unsigned int &width,
	unsigned int &height,
	unsigned char* pixels,
	GLenum glFormat,
	bool bInvert,
	GLuint HostFBO)
{
	return spout.ReceiveImage(Sendername, width, height, pixels, glFormat, bInvert, HostFBO);
}

//---------------------------------------------------------
void SpoutReceiver::RemovePadding(const unsigned char *source, unsigned char *dest,
	unsigned int width, unsigned int height, unsigned int stride, GLenum glFormat)
{
	return spout.RemovePadding(source, dest, width, height, stride, glFormat);
}

//---------------------------------------------------------
bool SpoutReceiver::CheckReceiver(char* name, unsigned int &width, unsigned int &height, bool &bConnected)
{
	return spout.CheckReceiver(name, width, height, bConnected);
}

//---------------------------------------------------------
bool SpoutReceiver::IsInitialized()
{
	return spout.IsSpoutInitialized();

}

//---------------------------------------------------------
bool SpoutReceiver::SelectSenderPanel(const char* message)
{
	return spout.SelectSenderPanel(message);
}

//---------------------------------------------------------
bool SpoutReceiver::BindSharedTexture()
{
	return spout.BindSharedTexture();
}

//---------------------------------------------------------
bool SpoutReceiver::UnBindSharedTexture()
{
	return spout.UnBindSharedTexture();
}

//---------------------------------------------------------
int  SpoutReceiver::GetSenderCount()
{
	return spout.GetSenderCount();
}

//---------------------------------------------------------
bool SpoutReceiver::GetSenderName(int index, char* sendername, int MaxNameSize)
{
	return spout.GetSenderName(index, sendername, MaxNameSize);
}

//---------------------------------------------------------
bool SpoutReceiver::GetSenderInfo(const char* sendername, unsigned int &width, unsigned int &height, HANDLE &dxShareHandle, DWORD &dwFormat)
{
	return spout.GetSenderInfo(sendername, width, height, dxShareHandle, dwFormat);
}

//---------------------------------------------------------
bool SpoutReceiver::GetActiveSender(char* Sendername)
{
	return spout.GetActiveSender(Sendername);
}

//---------------------------------------------------------
bool SpoutReceiver::SetActiveSender(const char* Sendername)
{
	return spout.SetActiveSender(Sendername);
}

//---------------------------------------------------------
bool SpoutReceiver::GetMemoryShareMode()
{
	return spout.GetMemoryShareMode();
}

//---------------------------------------------------------
bool SpoutReceiver::SetMemoryShareMode(bool bMem)
{
	return spout.SetMemoryShareMode(bMem);
}

//---------------------------------------------------------
int SpoutReceiver::GetShareMode()
{
	return (spout.GetShareMode());
}

//---------------------------------------------------------
bool SpoutReceiver::SetShareMode(int mode)
{
	return (spout.SetShareMode(mode));
}

//---------------------------------------------------------
bool SpoutReceiver::GetBufferMode()
{
	return spout.GetBufferMode();
}

//---------------------------------------------------------
void SpoutReceiver::SetBufferMode(bool bActive)
{
	spout.SetBufferMode(bActive);
}

//---------------------------------------------------------
bool SpoutReceiver::GetDX9()
{
	return spout.interop.isDX9();
}

//---------------------------------------------------------
bool SpoutReceiver::SetDX9(bool bDX9)
{
	return spout.interop.UseDX9(bDX9);
}

//---------------------------------------------------------
bool SpoutReceiver::GetDX9compatible()
{
	if (spout.interop.DX11format == DXGI_FORMAT_B8G8R8A8_UNORM)
		return true;
	else
		return false;
}

//---------------------------------------------------------
void SpoutReceiver::SetDX9compatible(bool bCompatible)
{
	if (bCompatible) {
		// DX11 -> DX9 only works if the DX11 format is set to DXGI_FORMAT_B8G8R8A8_UNORM
		spout.interop.SetDX11format(DXGI_FORMAT_B8G8R8A8_UNORM);
	}
	else {
		// DX11 -> DX11 only
		spout.interop.SetDX11format(DXGI_FORMAT_R8G8B8A8_UNORM);
	}
}

//---------------------------------------------------------
int SpoutReceiver::GetAdapter()
{
	return spout.GetAdapter();
}

//---------------------------------------------------------
bool SpoutReceiver::SetAdapter(int index)
{
	return spout.SetAdapter(index);
}

//---------------------------------------------------------
int SpoutReceiver::GetNumAdapters()
{
	return spout.GetNumAdapters();
}

//---------------------------------------------------------
// Get an adapter name
bool SpoutReceiver::GetAdapterName(int index, char* adaptername, int maxchars)
{
	return spout.GetAdapterName(index, adaptername, maxchars);
}

//---------------------------------------------------------
int SpoutReceiver::GetMaxSenders()
{
	// Get the maximum senders allowed from the sendernames class
	return(spout.interop.senders.GetMaxSenders());
}

//---------------------------------------------------------
void SpoutReceiver::SetMaxSenders(int maxSenders)
{
	// Sets the maximum senders allowed
	spout.interop.senders.SetMaxSenders(maxSenders);
}

//---------------------------------------------------------
bool SpoutReceiver::GetHostPath(const char* sendername, char* hostpath, int maxchars)
{
	return spout.GetHostPath(sendername, hostpath, maxchars);
}

//---------------------------------------------------------
int SpoutReceiver::GetVerticalSync()
{
	return spout.interop.GetVerticalSync();
}

//---------------------------------------------------------
bool SpoutReceiver::SetVerticalSync(bool bSync)
{
	return spout.interop.SetVerticalSync(bSync);
}

