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

#if !defined(HOMEBREWDMRIPSC_H)
#define	HOMEBREWDMRIPSC_H

#include "UDPSocket.h"
#include "Timer.h"
#include "RingBuffer.h"
#include "DMRData.h"

#include <string>

#if defined(WIN32)
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
#else
#include <cstdint>
#endif

class CHomebrewDMRIPSC
{
public:
	CHomebrewDMRIPSC(const std::string& address, unsigned int port, unsigned int id, const std::string& password, const char* version);
	~CHomebrewDMRIPSC();

	void setConfig(const std::string& callsign, unsigned int rxFrequency, unsigned int txFrequency, unsigned int power, unsigned int colorCode, float latitude, float longitude, int height, const std::string& location, const std::string& description, const std::string& url);

	bool open();

	bool read(CDMRData& data);

	bool write(const CDMRData& data);

	void clock(unsigned int ms);

	void close();

private: 
	in_addr      m_address;
	unsigned int m_port;
	uint8_t*     m_id;
	std::string  m_password;
	const char*  m_version;
	CUDPSocket   m_socket;

	enum STATUS {
		DISCONNECTED,
		WAITING_LOGIN,
		WAITING_AUTHORISATION,
		WAITING_CONFIG,
		RUNNING
	};

	STATUS         m_status;
	CTimer         m_retryTimer;
	CTimer         m_timeoutTimer;
	CTimer         m_pingTimer;
	unsigned char* m_buffer;
	unsigned char* m_salt;
	uint32_t*      m_streamId;
	uint8_t*       m_seqNo;

	CRingBuffer<unsigned char> m_rxData;

	std::string    m_callsign;
	unsigned int   m_rxFrequency;
	unsigned int   m_txFrequency;
	unsigned int   m_power;
	unsigned int   m_colorCode;
	float          m_latitude;
	float          m_longitude;
	int            m_height;
	std::string    m_location;
	std::string    m_description;
	std::string    m_url;

	bool writeLogin();
	bool writeAuthorisation();
	bool writeConfig();
	bool writePing();
};

#endif
