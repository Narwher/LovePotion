#include "common/runtime.h"

using namespace love;

void Logger::Initialize()
{
    Logger::m_file = freopen("love.log", "w", stderr);
    Logger::m_enabled = true;
}

/*
** {Function Name}:{Line}
** {Resolved printf stuff}
** {Newline}
** {Start of next Output}
*/
void Logger::LogOutput(const char * func, size_t line, const char * format, ...)
{
    if (!m_enabled || !m_file)
        return;

    thread::Lock lock(m_mutex);

    va_list args;
    va_start(args, format);

    fprintf(m_file, "%s:%zu:\n", func, line);
    vfprintf(m_file, format, args);
    fprintf(m_file, "\n\n");

    fflush(m_file);
}

void Logger::Exit()
{
    if (!m_enabled || !m_file)
        return;

    fclose(m_file);
}
