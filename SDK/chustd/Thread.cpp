///////////////////////////////////////////////////////////////////////////////
// This file is part of the chustd library
// Copyright (C) ChuTeam
// For conditions of distribution and use, see copyright notice in chustd.h
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Thread.h"

using namespace chustd;

////////////////////////////////////////////////
Thread::Thread()
{
	m_handle = nullptr;
}

////////////////////////////////////////////////
Thread::~Thread()
{
	WaitForExit();
}

////////////////////////////////////////////////
struct StartArg
{
	int (*pfn)(void*);
	void* userArg;
};

////////////////////////////////////////////////
#if defined(_WIN32)
static DWORD WINAPI ThreadProc(LPVOID arg)
#elif defined(__linux__)
static void* ThreadProc(void* arg)
#endif
{
	StartArg* pStartArg = reinterpret_cast<StartArg*>(arg);
	StartArg startArg = *pStartArg;
	delete pStartArg;
	int ret = startArg.pfn(startArg.userArg);
#if defined(_WIN32)
	return static_cast<DWORD>(ret);
#elif defined(__linux__)
	return reinterpret_cast<void*>(ret);
#endif
}

////////////////////////////////////////////////
bool Thread::Start(int (*pfn)(void*), void* userArg)
{
	StartArg* pStartArg = new StartArg;
	pStartArg->pfn = pfn;
	pStartArg->userArg = userArg;

#if defined(_WIN32)
	DWORD threadId = 0;
	m_handle = ::CreateThread(NULL, 0, ThreadProc, pStartArg, 0, &threadId);
	return m_handle != NULL;

#elif defined(__linux__)
	pthread_t th;
	int ret = pthread_create(&th, NULL, &ThreadProc, pStartArg);
	if( ret != 0 )
	{
		return false;
	}
	m_handle = reinterpret_cast<void*>(th);
	return true;
#endif
}

////////////////////////////////////////////////
bool Thread::IsStarted() const
{
	return m_handle != 0;
}

////////////////////////////////////////////////
void Thread::Suspend()
{
#if defined(_WIN32)
	::SuspendThread(m_handle);
#endif
}

////////////////////////////////////////////////
void Thread::WaitForExit()
{
	if( m_handle == nullptr )
	{
		return;
	}

#if defined(_WIN32)
	WaitForSingleObject(m_handle, INFINITE);
	CloseHandle(m_handle);

#elif defined(__linux__)
	void* threadRet = nullptr;
	pthread_t th = reinterpret_cast<pthread_t>(m_handle);
	pthread_join(th, &threadRet);
#endif

	m_handle = nullptr;
}

////////////////////////////////////////////////
void Thread::Sleep(int delayMs)
{
#if defined(_WIN32)
	::Sleep(delayMs);

#elif defined(__linux__)
	struct timespec ts;
	ts.tv_sec = delayMs / 1000;
	ts.tv_nsec = (delayMs % 1000) * 1000000;
	nanosleep(&ts, NULL);
#endif
}

