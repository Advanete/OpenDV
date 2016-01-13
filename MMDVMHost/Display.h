/*
 *   Copyright (C) 2016 by Jonathan Naylor G4KLX
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

#if !defined(DISPLAY_H)
#define	DISPLAY_H

#include <string>

class IDisplay
{
public:
  virtual ~IDisplay() = 0;

  virtual bool open() = 0;

  virtual void setIdle() = 0;

  virtual void setDStar() = 0;
  virtual void writeDStar(const std::string& call1, const std::string& call2) = 0;
  virtual void clearDStar() = 0;

  virtual void setDMR() = 0;
  virtual void writeDMR(unsigned int slotNo, unsigned int srdId, bool group, unsigned int dstId) = 0;
  virtual void clearDMR(unsigned int slotNo) = 0;

  virtual void setFusion() = 0;
  virtual void writeFusion(const std::string& callsign) = 0;
  virtual void clearFusion() = 0;

  virtual void close() = 0;

private:
};

#endif
