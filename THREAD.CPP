/*--------------------------------------------------------------------------*/
/*                                                                          */
/*   Main-Module   : none                                                   */
/*                                                                          */
/*   Version       : V1.00                                                  */
/*                                                                          */
/*   Date          : 15.10.92                                               */
/*                                                                          */
/*   Written       : MH                                                     */
/*                                                                          */
/*   Dependency    :                                                        */
/*							                                                */
/*	 Revision History :

	15.10.92	MH	Entfernen von PM-Threads
	18.10.92	RF	erbt ERROR
							                                                */
/*--------------------------------------------------------------------------*/
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#include <thread.hpp>
#include <stdlib.h>

//---------------- Methods for class thread ----------------------------------
HSEM	thread_sem=0L;			// protection semaphore
THREAD	*newthread;

static void ThreadStarter (void);
static void DataThreadStarter (void);

THREAD::THREAD	()
	{
	PIDINFO pidinfo;
	
	DosGetPID (&pidinfo);
	pid = pidinfo.pid;
	tid = pidinfo.tid;

	threadfunc		= NULL;
	threadfuncdata	= NULL;
	started			= TRUE;
	running			= TRUE;
	stacksize		= DEF_STACKSIZE;
	error			= OK;
	starter			= NULL;
	}
	
THREAD::THREAD	(THREADFUNC func, BOOL start)
	{
	PIDINFO pidinfo;
	
	threadfunc		= func;
	threadfuncdata	= NULL;
	started			= start;
	running			= FALSE;
	stacksize		= DEF_STACKSIZE;
	error			= OK;
	data			= NULL;
	starter 		= ThreadStarter;
	tid				= 0;
	
	DosGetPID (&pidinfo);
	pid = pidinfo.pid;
	
	if (start)
		Start (func);
	}
	
THREAD::THREAD	(void *adata, unsigned adatalen,
        		 THREADFUNCDATA func, BOOL start)
	{
	PIDINFO pidinfo;
	
	threadfunc		= NULL;
	threadfuncdata	= func;
	started			= FALSE;
	running			= FALSE;
	stacksize		= DEF_STACKSIZE;
	error			= OK;
	data			= adata;
	datalen			= adatalen;
	starter			= DataThreadStarter;
	tid				= 0;
	
	DosGetPID (&pidinfo);
	pid = pidinfo.pid;
	
	if (start)
		Start (data, datalen, func);
	}
	
void THREAD::Suspend ()
	{
	if (!started || !running)	
		return;
		
	DosSuspendThread (tid);
	running = FALSE;
	}
	
void THREAD::Resume ()
	{
	if (running)
		return;
		
	DosResumeThread (tid);
	running = TRUE;
	}

USHORT THREAD::Start (THREADFUNC func)
	{
	PBYTE	stack;	
	
	if (started)
		return (error = OK);

	if (func)
		threadfunc = func;
	if (!threadfunc)
		return (error = ENOFUNC);

	stack = (PBYTE)malloc (stacksize);
	if (!stack)
		return (error = ENOMEM);
		
	DosSemRequest ((HSEM)&thread_sem, -1L);	
	newthread = this;
	if (DosCreateThread (starter, &tid, stack+stacksize))
		{
		free (stack);
		return (error = ESTARTTHREAD);
		}
	started = TRUE;
	running = TRUE;
	DosSemWait (&thread_sem, -1L);
		
	return (error = OK);
	}
	
USHORT THREAD::Start (void *adata, unsigned adatalen, THREADFUNCDATA func)
	{
	PBYTE			stack;	
	
	if (started)
		return (error = OK);

	if (func)
		threadfuncdata = func;
	if (!threadfuncdata)
		return (error = ENOFUNC);
		
	if (adata)
		{
		data = adata;
		datalen = adatalen;
		}
	starter	= DataThreadStarter;

	stack = (PBYTE)malloc (stacksize);
	if (!stack)
		return (error = ENOMEM);
		
	DosSemRequest (&thread_sem, -1L);	
	newthread = this;
	if (DosCreateThread (starter, &tid, stack+stacksize))
		{
		free (stack);
		return (error = ESTARTTHREAD);
		}
	started = TRUE;
	running = TRUE;
	DosSemWait (&thread_sem, -1L);
		
	return (error = OK);
	}
	
static void ThreadStarter (void)
	{
	THREAD *found = newthread;
	
	DosSemClear (&thread_sem);
	found->threadfunc ();
	found->running = found->started = FALSE;
	}

static void DataThreadStarter (void)
	{
	THREAD *found = newthread;
	
	DosSemClear (&thread_sem);
	found->threadfuncdata (found->data, found->datalen);
	found->running = found->started = FALSE;
	}
//============================================================================