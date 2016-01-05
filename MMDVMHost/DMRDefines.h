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

#if !defined(DMRDefines_H)
#define	DMRDefines_H

const unsigned int DMR_FRAME_LENGTH_BITS  = 264U;
const unsigned int DMR_FRAME_LENGTH_BYTES = 33U;

const unsigned int DMR_SYNC_LENGTH_BITS  = 48U;
const unsigned int DMR_SYNC_LENGTH_BYTES = 6U;

const unsigned int DMR_EMB_LENGTH_BITS  = 8U;
const unsigned int DMR_EMB_LENGTH_BYTES = 1U;

const unsigned int DMR_SLOT_TYPE_LENGTH_BITS  = 8U;
const unsigned int DMR_SLOT_TYPE_LENGTH_BYTES = 1U;

const unsigned int DMR_EMBEDDED_SIGNALLING_LENGTH_BITS  = 32U;
const unsigned int DMR_EMBEDDED_SIGNALLING_LENGTH_BYTES = 4U;

const unsigned int DMR_AMBE_LENGTH_BITS  = 108U * 2U;
const unsigned int DMR_AMBE_LENGTH_BYTES = 27U;

const unsigned char BS_SOURCED_AUDIO_SYNC[]   = {0x07U, 0x55U, 0xFDU, 0x7DU, 0xF7U, 0x5FU, 0x70U};
const unsigned char BS_SOURCED_DATA_SYNC[]    = {0x0DU, 0xFFU, 0x57U, 0xD7U, 0x5DU, 0xF5U, 0xD0U};

const unsigned char MS_SOURCED_AUDIO_SYNC[]   = {0x07U, 0xF7U, 0xD5U, 0xDDU, 0x57U, 0xDFU, 0xD0U};
const unsigned char MS_SOURCED_DATA_SYNC[]    = {0x0DU, 0x5DU, 0x7FU, 0x77U, 0xFDU, 0x75U, 0x70U};

const unsigned char DIRECT_SLOT1_AUDIO_SYNC[] = {0x05U, 0xD5U, 0x77U, 0xF7U, 0x75U, 0x7FU, 0xF0U};
const unsigned char DIRECT_SLOT1_DATA_SYNC[]  = {0x0FU, 0x7FU, 0xDDU, 0x5DU, 0xDFU, 0xD5U, 0x50U};

const unsigned char DIRECT_SLOT2_AUDIO_SYNC[] = {0x07U, 0xDFU, 0xFDU, 0x5FU, 0x55U, 0xD5U, 0xF0U};
const unsigned char DIRECT_SLOT2_DATA_SYNC[]  = {0x0DU, 0x75U, 0x57U, 0xF5U, 0xFFU, 0x7FU, 0x50U};

const unsigned char SYNC_MASK[]               = {0x0FU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xF0U};

const unsigned char PAYLOAD_LEFT_MASK[]       = {0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xF0U};
const unsigned char PAYLOAD_RIGHT_MASK[]      = {0x0FU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

const unsigned char VOICE_LC_HEADER_CRC_MASK[]    = {0x96U, 0x96U, 0x96U};
const unsigned char TERMINATOR_WITH_LC_CRC_MASK[] = {0x99U, 0x99U, 0x99U};
const unsigned char CSBK_CRC_MASK[]               = {0xA5U, 0xA5U};

const unsigned int DMR_SLOT_TIME = 60U;
const unsigned int AMBE_PER_SLOT = 3U;

const unsigned char DT_MASK               = 0x0FU;
const unsigned char DT_VOICE_PI_HEADER    = 0x00U;
const unsigned char DT_VOICE_LC_HEADER    = 0x01U;
const unsigned char DT_TERMINATOR_WITH_LC = 0x02U;
const unsigned char DT_CSBK               = 0x03U;
const unsigned char DT_IDLE               = 0x09U;

const unsigned char DMR_IDLE_RX    = 0x80U;
const unsigned char DMR_SYNC_DATA  = 0x40U;
const unsigned char DMR_SYNC_AUDIO = 0x20U;

const unsigned char DMR_SLOT1      = 0x00U;
const unsigned char DMR_SLOT2      = 0x80U;

enum FLCO {
	FLCO_GROUP = 0,
	FLCO_USER_USER = 3
};

#endif
