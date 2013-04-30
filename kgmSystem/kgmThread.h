// cThread.h: interface for the cThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#ifdef WIN32
 #include <windows.h>
 #include <process.h>
#else
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
#endif


class kgmThread{

public:
typedef void* Mutex;

#ifdef WIN32
 void* m_thread;
#else
 pthread_t m_thread;
#endif
 
public:
 kgmThread();
 ~kgmThread();

 bool exec();
 void exit();
 void join();

 static Mutex mutex();
 static void  free(Mutex);
 static void  lock(Mutex);
 static void  unlock(Mutex);
 static bool  lockable(Mutex);

protected: 
 virtual void run() = 0;

private:
 static void thread(kgmThread *p);
};

