//
// file:		stdout_analyzer_hook_unix.c
// path:		src/utils/stdout_analyzer_hook_unix.c
// created by:	Davit Kalantaryan (davit.kalataryan@desy.de)
// created on:	2022 Jun 14
//


#ifdef STDOUT_INVEST_USE_HOOK01
#if (!defined(_WIN32)) || defined(__INTELICENCE__)

#include <stdout_investigator/investigator01.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

CPPUTILS_BEGIN_C

#define READ_BUFFER_MAX_SIZE        4096

static pthread_t    s_redirectorThread = 0;
static int          s_nWork = 0;
static int          s_vPipesStdOut[2]={-1,-1},s_vPipesStdErr[2]={-1,-1};
static int          s_stdoutCopy=-1, s_stderrCopy=-1;

static void __attribute__((constructor)) ConstructStdoutInvestigateor(void);
static void __attribute__((destructor))  DestructStdoutInvestigateor(void);

static void HandleUserStdout(const char* a_buffer, size_t a_unBufferSize );
static void HandleUserStderr(const char* a_buffer, size_t a_unBufferSize );
static void SignalFunction(int a_sigNum){CPPUTILS_STATIC_CAST(void,a_sigNum);}

TypeWriter  g_stdoutHandler = &HandleUserStdout;
TypeWriter  g_stderrHandler = &HandleUserStderr;
bool        s_shouldStopCallbacks = false;

void StopStdoutInvestManually(void)
{
    DestructStdoutInvestigateor();
}


void WriteToStdOut(const char* a_buffer, size_t a_unBufferSize)
{
    write(s_stdoutCopy,a_buffer,a_unBufferSize);
}


void WriteToStdError(const char* a_buffer, size_t a_unBufferSize)
{
    write(s_stderrCopy,a_buffer,a_unBufferSize);
}


static void* RedirectorThreadFunction(void* a_p_data)
{
    char vcBufferToRead[READ_BUFFER_MAX_SIZE];
    fd_set rfds;
    int nSelectReturn;
    const int maxsd = s_vPipesStdOut[0]>s_vPipesStdErr[0] ? (s_vPipesStdOut[0]+1) : (s_vPipesStdErr[0]+1) ;
    ssize_t  sizeRead;
    struct sigaction aSigAction;

    sigemptyset(&aSigAction.sa_mask);
    aSigAction.sa_flags = 0;
    aSigAction.sa_restorer = CPPUTILS_NULL;
    aSigAction.sa_handler = &SignalFunction;
    sigaction(SIGUSR1,&aSigAction,CPPUTILS_NULL);

    CPPUTILS_STATIC_CAST(void,a_p_data);

    do{
        FD_ZERO( &rfds );
        FD_SET( s_vPipesStdOut[0], &rfds );
        FD_SET( s_vPipesStdErr[0], &rfds );

        nSelectReturn = select(maxsd, &rfds, CPPUTILS_NULL, CPPUTILS_NULL, CPPUTILS_NULL );

        switch(nSelectReturn)
        {
        case 0:	case -1:/* 0=> time out, -1 eintr */
            break;
        default:{
            // we can read
            if (FD_ISSET(s_vPipesStdOut[0], &rfds)){
                sizeRead = read(s_vPipesStdOut[0],vcBufferToRead,READ_BUFFER_MAX_SIZE);
                if(sizeRead>0){
                    if(s_shouldStopCallbacks){
                        write(s_stdoutCopy,vcBufferToRead,CPPUTILS_STATIC_CAST(size_t,sizeRead));
                    }
                    else{
                        (*g_stdoutHandler)(vcBufferToRead,CPPUTILS_STATIC_CAST(size_t,sizeRead));
                    }
                } // if(sizeRead>0){
            } // if (FD_ISSET(s_vPipesStdOut[0], &rfds)){
            if (FD_ISSET(s_vPipesStdErr[0], &rfds)){
                sizeRead = read(s_vPipesStdErr[0],vcBufferToRead,READ_BUFFER_MAX_SIZE);
                if(sizeRead>0){
                    if(s_shouldStopCallbacks){
                        write(s_stderrCopy,vcBufferToRead,CPPUTILS_STATIC_CAST(size_t,sizeRead));
                    }
                    else{
                        (*g_stderrHandler)(vcBufferToRead,CPPUTILS_STATIC_CAST(size_t,sizeRead));
                    }
                } // if(sizeRead>0){
            } // if (FD_ISSET(s_vPipesStdErr[0], &rfds)){
        }break;
        }  // switch(nTry)

    }while(s_nWork);

    pthread_exit(CPPUTILS_NULL);
}


static void HandleUserStdout(const char* a_buffer, size_t a_unBufferSize )
{
    //dprintf(s_stdoutCopy,"readSize=%d, buffer=%." READ_BUFFER_MAX_SIZE_STR "s", static_cast<int>(a_unBufferSize),static_cast<const char*>(a_buffer) );
    write(s_stdoutCopy,a_buffer,a_unBufferSize);
}


static void HandleUserStderr(const char* a_buffer, size_t a_unBufferSize )
{
    //dprintf(s_stderrCopy,"readSize=%d, buffer=%." READ_BUFFER_MAX_SIZE_STR "s", static_cast<int>(a_unBufferSize),static_cast<const char*>(a_buffer) );
    write(s_stderrCopy,a_buffer,a_unBufferSize);
}



static void __attribute__((constructor)) ConstructStdoutInvestigateor(void)
{
#ifdef DEBUG_APP
    printf("Press any key, then press Enter to continue\n");fflush(stdout);
    getchar();
#endif

    if(pipe(s_vPipesStdOut)){
        fprintf(stderr,"Unable to open the pipes!\n");
        exit(1);
    }

    if(pipe(s_vPipesStdErr)){
        fprintf(stderr,"Unable to open the pipes!\n");
        exit(1);
    }

    s_stdoutCopy = dup(STDOUT_FILENO);
    s_stderrCopy = dup(STDERR_FILENO);

    if(dup2(s_vPipesStdOut[1],STDOUT_FILENO)==-1){
        exit(1);
    }
    if(dup2(s_vPipesStdErr[1],STDERR_FILENO)==-1){
        exit(1);
    }

    s_nWork=1;
    pthread_create(&s_redirectorThread,CPPUTILS_NULL,&RedirectorThreadFunction,CPPUTILS_NULL);
}


static void __attribute__((destructor))  DestructStdoutInvestigateor(void)
{
    s_nWork = 0;
    if(s_redirectorThread){
        pthread_kill(s_redirectorThread,SIGUSR1);
        pthread_join(s_redirectorThread,CPPUTILS_NULL);
        s_redirectorThread = 0;
    }

    if(s_stdoutCopy>=0){
        dup2(s_stdoutCopy,STDOUT_FILENO);
        close(s_stdoutCopy);
        s_stdoutCopy = -1;
    }
    if(s_stderrCopy>=0){
        dup2(s_stderrCopy,STDERR_FILENO);
        close(s_stderrCopy);
        s_stderrCopy = -1;
    }
    if(s_vPipesStdOut[0]>=0){
        close(s_vPipesStdOut[1]);
        close(s_vPipesStdOut[0]);
        s_vPipesStdOut[1]=-1;
        s_vPipesStdOut[0]=-1;
    }
    if(s_vPipesStdErr[0]>=0){
        close(s_vPipesStdErr[1]);
        close(s_vPipesStdErr[0]);
        s_vPipesStdErr[1]=-1;
        s_vPipesStdErr[0]=-1;
    }
}


CPPUTILS_END_C


#endif  // #if (!defined(_WIN32)) || defined(__INTELICENCE__)
#endif  // #ifdef STDOUT_INVEST_USE_HOOK01
