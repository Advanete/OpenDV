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

#if !defined(DMRControl_H)
#define	DMRControl_H

#include "HomebrewDMRIPSC.h"
#include "DMRSlot.h"
#include "DMRData.h"
#include "Modem.h"


class CDMRControl {
public:
	CDMRControl(unsigned int id, unsigned int colorCode, unsigned int timeout, CModem* modem, CHomebrewDMRIPSC* network);
	~CDMRControl();

	bool processWakeup(const unsigned char* data);

	void writeModemSlot1(unsigned char* data);
	void writeModemSlot2(unsigned char* data);

	unsigned int readModemSlot1(unsigned char* data);
	unsigned int readModemSlot2(unsigned char* data);

	void clock(unsigned int ms);

private:
	unsigned int      m_id;
	unsigned int      m_colorCode;
	CModem*           m_modem;
	CHomebrewDMRIPSC* m_network;
	CDMRSlot          m_slot1;
	CDMRSlot          m_slot2;
};

#endif
