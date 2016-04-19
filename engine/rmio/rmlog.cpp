#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(once: 4290)

#include "rmlog.h"
#include <errno.h>
#include <cstdio>


RMLogger::RMLogger()
{
    m_logHandle = NULL;
    m_logging = false;
    m_isSTD = false;
    m_flushInterval = 1;
    m_logCount = 0;
}


RMLogger::~RMLogger()
{
    CloseLog();
}


void RMLogger::SetFlushInterval(int fi)
{
    m_flushInterval = fi;
    m_logCount = 0;
}


bool RMLogger::OpenLogSTDERR()
{
    m_logHandle = stderr;
    m_logging = true;
    m_isSTD = true;
    return true;
}


bool RMLogger::OpenLogSTDOUT()
{
    m_logHandle = stdout;
    m_logging = true;
    m_isSTD = true;
    return true;
}


bool RMLogger::OpenLog(const char *file)
{
    m_logHandle = fopen(file, "w");
    if (m_logHandle == NULL)
    {
        m_logging = false;
        return false;
    }
    else
    {
        m_logging = true;
        return true;
    }
}

void RMLogger::CloseLog()
{
    if (m_logging && m_logHandle != NULL && !m_isSTD)
    {
        fclose(m_logHandle);
    }
    m_logging = false;
}

void RMLogger::logmsg(const char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);
    logmsg_args_processed(fmt, arg);
    va_end(arg);
}

void RMLogger::logmsg_args_processed(const char* fmt, va_list arg)
{
    if (m_logHandle == NULL)
        return;

    vfprintf(m_logHandle, fmt, arg);

    if (++m_logCount >= m_flushInterval)
    {
        fflush(m_logHandle);
        m_logCount = 0;
    }
}




RMLoggerImpl::RMLoggerImpl()
{
    m_loggerObject = NULL;
}

void RMLoggerImpl::logmsg(const char* fmt, ...)
{
    if (m_loggerObject == NULL)
        return;

    va_list arg;
    va_start(arg, fmt);
    m_loggerObject->logmsg_args_processed(fmt, arg);
    va_end(arg);
}

void RMLoggerImpl::SetLogger(RMLogger* obj)
{
    m_loggerObject = obj;
}
