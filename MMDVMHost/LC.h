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

#if !defined(LC_H)
#define LC_H

#include "DMRDefines.h"

class CLC
{
public:
	CLC(FLCO flco, unsigned int srcId, unsigned int dstId);
	CLC(const unsigned char* bytes);
	CLC(const bool* bits);
	CLC();
	~CLC();

	void getData(unsigned char* bytes) const;
	void getData(bool* bits) const;

	bool getPF() const;
	void setPF(bool pf);

	FLCO getFLCO() const;
	void setFLCO(FLCO flco);

	unsigned char getFID() const;
	void setFID(unsigned char fid);

	unsigned int getSrcId() const;
	void setSrcId(unsigned int id);

	unsigned int getDstId() const;
	void setDstId(unsigned int id);

private:
	bool          m_PF;
	FLCO          m_FLCO;
	unsigned char m_FID;
	unsigned int  m_srcId;
	unsigned int  m_dstId;
};

#endif

