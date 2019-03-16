/*

					SpoutFrameCount.h

	Copyright (c) 2019. Lynn Jarvis. All rights reserved.

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

// ====================================================================================
//		Revisions :
//
//		07.10.18 - Start class
//		13.11.18 - Clean up	
//
//
#pragma once

#ifndef __spoutFrameCount__
#define __spoutFrameCount__

#include <string>
#include "SpoutCommon.h"
#include "SpoutSharedMemory.h"
// #include <time.h>

#include <chrono> // c++11 timer
#include <thread>

// Warnings disabled in this project : C4250

#pragma comment(lib, "Winmm.lib")  // for timeGetTime

using namespace spoututils;

class SPOUT_DLLEXP spoutFrameCount {

	public:

	spoutFrameCount();
    ~spoutFrameCount();

	void EnableFrameCount(const char* SenderName);
	void DisableFrameCount();
	bool IsFrameCountEnabled();
	void CleanupFrameCount();
	void SetNewFrame(); // Sender increment the semaphore count
	bool GetNewFrame(); // Receiver read the semaphore count
	bool IsFrameNew(); // Is the received frame new
	bool HasNewFrame(); // Has the sender produced a new frame
	double GetSenderFps(); // Receiver get sender frame rate
	long GetSenderFrame(); // Receiver get sender frame number
	void HoldFps(int fps); // Target frame rate control


	// Mutex for shared texture access
	bool CreateAccessMutex(const char * SenderName);
	void CloseAccessMutex();
	bool CheckAccess();
	void AllowAccess();

protected :

	// Texture access mutex
	HANDLE m_hAccessMutex;

	// Frame count semaphore
	bool m_bFrameCount; // User selection of frame count in SpoutSettings
	bool m_bDisabled; // application disable
	bool m_bIsNewFrame; // received frame is new
	HANDLE m_hCountSemaphore; // semaphore handle
	char m_CountSemaphoreName[256]; // semaphore name
	char m_SenderName[256]; // sender currently connected to a receiver
	long m_FrameCount; // sender frame count
	long m_LastFrameCount; // receiver frame comparator
	double m_FrameTimeTotal;
	double m_FrameTimeNumber;

	// Sender frame timing
	double m_FrameEnd;
	double m_FrameLast;
	double m_FrameRate;
	double m_Fps;
	void UpdateSenderFps(long framecount = 0);
	double GetRefreshRate();

	// Frame rate control
	DWORD m_startTime;
	DWORD m_millisForFrame;
	unsigned int m_minTimerResolution;

	// Timer
	double PCFreq;
	__int64 CounterStart;
	void StartCounter();
	DWORD GetCounter();


};

#endif
