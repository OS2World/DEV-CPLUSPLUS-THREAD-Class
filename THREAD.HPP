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
	19.10.92	RF	Funcs sind pascal
							                                                */
/*--------------------------------------------------------------------------*/
#ifndef THREAD_INC
#define THREAD_INC

/*
#define INCL_ERRTHREAD
#include <errdef.h>
#include <error.hpp>
*/
#include <os2pm.h>

#define DEF_STACKSIZE (10*1024)		// default stacksize

#define ENOFUNC      100
#define ENOMEM       101
#define ESTARTTHREAD 102

typedef void (pascal *THREADFUNC	 ) () ;
typedef void (pascal *THREADFUNCDATA) (void *, unsigned) ;

class THREAD // : public ERROR
	{
    public:                      /* define error-var here, since */
        unsigned error;          /* we don't inherit it!!        */
    public:

		THREAD  ();
        THREAD  (THREADFUNC func, BOOL start=FALSE);
        THREAD	(void *adata, unsigned adatalen,
        		 THREADFUNCDATA func, BOOL start=FALSE);

		USHORT Start (THREADFUNC func=NULL);
		USHORT Start (void *adata, unsigned adatalen,
					  THREADFUNCDATA func=NULL);
						
		void Suspend	();
		void Resume		();
		void StackSize	(unsigned size)
			{
			stacksize = size;
			}
		BOOL 	Running () {return running;};
		BOOL 	Started () {return started;};
		TID		Tid		() {return tid;};
		PID		Pid		() {return pid;};
		
	protected:
		THREADFUNC		threadfunc;
		void 			(*starter) (void);
		THREADFUNCDATA	threadfuncdata;
		
		BOOL			running;
		BOOL			started;
		void			*data;
		unsigned		datalen;

	private:
		
		friend static void ThreadStarter (void);
		friend static void DataThreadStarter (void);

		
		TID				tid;
		PID				pid;
		unsigned		stacksize;
	};
//----------------------------------------------------------------------------
#endif

