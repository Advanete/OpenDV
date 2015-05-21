/*
 *   Copyright (C) 2015 by Jonathan Naylor G4KLX
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "Log.h"

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <ctime>
#include <cassert>

static std::string m_path;
static std::string m_root;

static FILE* m_fpLog = NULL;

static unsigned int m_level = 2U;

static struct tm m_tm;

static char LEVELS[] = " DMIWEF";

static bool LogOpen()
{
	time_t now;
	::time(&now);

	struct tm* tm = ::gmtime(&now);

	if (tm->tm_mday == m_tm.tm_mday && tm->tm_mon == m_tm.tm_mon && tm->tm_year == m_tm.tm_year) {
		if (m_fpLog != NULL)
		    return true;
	} else {
		if (m_fpLog != NULL)
			::fclose(m_fpLog);
	}

	char filename[50U];
#if defined(WIN32)
	::sprintf(filename, "%s\\%s-%04d-%02d-%02d.log", m_path.c_str(), m_root.c_str(), tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
#else
	::sprintf(filename, "%s/%s-%04d-%02d-%02d.log", m_path.c_str(), m_root.c_str(), tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
#endif

    m_fpLog = ::fopen(filename, "a+t");
	m_tm = *tm;

    return m_fpLog != NULL;
}

bool LogInitialise(const std::string& path, const std::string& root)
{
	m_path = path;
	m_root = root;
    return ::LogOpen();
}

void LogFinalise()
{
    if (m_fpLog != NULL)
        ::fclose(m_fpLog);
}

void LogSetLevel(unsigned int level)
{
    m_level = level;
}

void Log(unsigned int level, const char* fmt, ...)
{
    assert(level < 7U);
    assert(fmt != NULL);

    if (level < m_level)
        return;

    bool ret = ::LogOpen();
    if (!ret)
        return;

    time_t now;
    ::time(&now);

    struct tm* tm = ::gmtime(&now);

    ::fprintf(m_fpLog, "%c: %04d-%02d-%02d %02d:%02d:%02d ", LEVELS[level], tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);

    va_list vl;
    va_start(vl, fmt);
    vfprintf(m_fpLog, fmt, vl);
    va_end(vl);

    ::fprintf(m_fpLog, "\n");

    ::fflush(m_fpLog);

    if (level == 6U) {		// Fatal
        ::fclose(m_fpLog);
        exit(1);
    }
}
