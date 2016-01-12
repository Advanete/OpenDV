/*
 *   Copyright (C) 2015,2016 by Jonathan Naylor G4KLX
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

#if !defined(DMRSlot_H)
#define	DMRSlot_H

#include "HomebrewDMRIPSC.h"
#include "EmbeddedLC.h"
#include "RingBuffer.h"
#include "DMRSlot.h"
#include "DMRData.h"
#include "Timer.h"
#include "Modem.h"
#include "LC.h"

enum SLOT_STATE {
	SS_LISTENING,
	SS_LATE_ENTRY,
	SS_RELAYING_RF,
	SS_RELAYING_NETWORK
};

class CDMRSlot {
public:
	CDMRSlot(unsigned int slotNo, unsigned int timeout);
	~CDMRSlot();

	void writeModem(unsigned char* data);

	unsigned int readModem(unsigned char* data);

	void writeNetwork(const CDMRData& data);

	bool readNetwork(CDMRData& data);

	void clock(unsigned int ms);

	static void init(unsigned int colorCode, CModem* modem, CHomebrewDMRIPSC* network);

private:
	unsigned int                 m_slotNo;
	CRingBuffer<unsigned char>** m_txQueue;
	SLOT_STATE                   m_state;
	CEmbeddedLC                  m_embeddedLC;
	CLC*                         m_lc;
	unsigned char                m_seqNo;
	unsigned char                m_n;
	CTimer**                     m_playoutTimer;
	CTimer                       m_networkWatchdog;
	CTimer                       m_timeoutTimer;
	unsigned int                 m_writeQueue;
	unsigned int                 m_readQueue;
	FILE*                        m_fp;

	static unsigned int        m_colorCode;
	static CModem*             m_modem;
	static CHomebrewDMRIPSC*   m_network;

	static unsigned char*      m_idle;

	static FLCO                m_flco1;
	static unsigned char       m_id1;
	static FLCO                m_flco2;
	static unsigned char       m_id2;

	void writeQueue(const unsigned char* data);
	void writeNetwork(const unsigned char* data, unsigned char dataType);

	void writeEndOfTransmission();

	bool openFile();
	bool writeFile(const unsigned char* data);
	void closeFile();

	static void setShortLC(unsigned int slotNo, unsigned int id, FLCO flco = FLCO_GROUP);
};

#endif
