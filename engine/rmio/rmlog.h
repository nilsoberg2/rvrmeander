#ifndef RMLOG_H__20100401__
#define RMLOG_H__20100401__

#include <cstdio>
#include <cstdarg>
#include "rmio.h"


class LIBRMIO_DLL_IMP_EXP RMLogger
{
public:
    RMLogger();
    ~RMLogger();

    virtual void logmsg(const char* fmt, ...);
    virtual void logmsg_args_processed(const char* fmt, va_list arg);
    virtual bool OpenLog(const char* file);
    virtual bool OpenLogSTDERR();
    virtual bool OpenLogSTDOUT();
    virtual void CloseLog();
    virtual void SetFlushInterval(int fi);
protected:
    FILE* m_logHandle;
    bool m_logging;
    int m_flushInterval;
    int m_logCount;
    bool m_isSTD;
};


class LIBRMIO_DLL_IMP_EXP RMLoggerImpl
{
public:
    RMLoggerImpl();
    virtual void SetLogger(RMLogger* obj);
    virtual void logmsg(const char* fmt, ...);
protected:
    RMLogger* m_loggerObject;
};


#endif//RMLOG_H__20100401__
