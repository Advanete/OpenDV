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

#include "MMDVMHost.h"
#include "Log.h"
#include "Version.h"
#include "StopWatch.h"
#include "Defines.h"
#include "DMRControl.h"

#include "DStarEcho.h"
#include "YSFEcho.h"

#include <cstdio>

#if !defined(WIN32)
#include <unistd.h>
#include <signal.h>
#endif

static bool m_killed = false;

#if !defined(WIN32)
static void sigHandler(int)
{
  m_killed = true;
}
#endif

const char* HEADER = "This software is for use on amateur radio networks only. Its use on commercial networks is strictly prohibited\nCopyright (C) 2015,2016 by Jonathan Naylor, G4KLX\n";

int main(int argc, char** argv)
{
  if (argc == 1) {
    ::fprintf(stderr, "Usage: MMDVMHost <conf file>\n");
    return 1;
  }

#if !defined(WIN32)
  ::signal(SIGUSR1, sigHandler);
#endif

  CMMDVMHost* host = new CMMDVMHost(std::string(argv[1]));
  int ret2 = host->run();

  delete host;

  ::LogFinalise();

  return ret2;
}

CMMDVMHost::CMMDVMHost(const std::string& confFile) :
m_conf(confFile),
m_modem(NULL),
m_dmrNetwork(NULL),
m_dstarEnabled(false),
m_dmrEnabled(false),
m_ysfEnabled(false)
{
}

CMMDVMHost::~CMMDVMHost()
{
}

int CMMDVMHost::run()
{
	::fprintf(stdout, "MMDVMHost-%s\n%s", VERSION, HEADER);

	bool ret = m_conf.read();
	if (!ret) {
		::fprintf(stderr, "MMDVMHost: cannot read the .ini file\n");
		return 1;
	}

	ret = ::LogInitialise(m_conf.getLogPath(), m_conf.getLogRoot(), m_conf.getLogDisplay());
	if (!ret) {
		::fprintf(stderr, "MMDVMHost: unable to open the log file\n");
		return 1;
	}

	::LogSetLevel(m_conf.getLogLevel());

	LogInfo(HEADER);

	LogMessage("MMDVMHost-%s is starting", VERSION);

	readParams();

	ret = createModem();
	if (!ret)
		return 1;

	if (m_dmrEnabled) {
		ret = createDMRNetwork();
		if (!ret)
			return 1;
	}

	CStopWatch stopWatch;
	stopWatch.start();

	CDStarEcho* dstar = NULL;
	if (m_dstarEnabled)
		dstar = new CDStarEcho(2U, 10000U);

	CDMRControl* dmr = NULL;
	if (m_dmrEnabled)
		dmr = new CDMRControl(m_conf.getDMRId(), m_conf.getDMRColorCode(), m_conf.getTimeout(), m_modem, m_dmrNetwork);

	CYSFEcho* ysf = NULL;
	if (m_ysfEnabled)
		ysf = new CYSFEcho(2U, 10000U);

	unsigned char mode = MODE_IDLE;
	CTimer modeTimer(1000U, m_conf.getModeHang());

	while (!m_killed) {
		unsigned char data[200U];
		unsigned int len;
		bool ret;

		len = m_modem->readDStarData(data);
		if (dstar != NULL && len > 0U) {
			if (mode == MODE_IDLE && (data[0U] == TAG_HEADER || data[0U] == TAG_DATA)) {
				LogMessage("Mode set to D-Star");
				mode = MODE_DSTAR;
				m_modem->setMode(MODE_DSTAR);
				modeTimer.start();
			}
			if (mode != MODE_DSTAR) {
				LogWarning("D-Star data received when in mode %u", mode);
			} else {
				if (data[0U] == TAG_HEADER || data[0U] == TAG_DATA || data[0U] == TAG_EOT) {
					dstar->writeData(data, len);
					modeTimer.start();
				}
			}
		}

		len = m_modem->readDMRData1(data);
		if (dmr != NULL && len > 0U) {
			if (mode == MODE_IDLE) {
				bool ret = dmr->processWakeup(data);
				if (ret) {
					LogMessage("Mode set to DMR");
					mode = MODE_DMR;
					// This sets the mode to DMR within the modem
					m_modem->writeDMRStart(true);
					modeTimer.start();
				}
			} else if (mode == MODE_DMR) {
				dmr->writeModemSlot1(data);
				modeTimer.start();
			} else {
				LogWarning("DMR data received when in mode %u", mode);
			}
		}

		len = m_modem->readDMRData2(data);
		if (dmr != NULL && len > 0U) {
			if (mode == MODE_IDLE) {
				bool ret = dmr->processWakeup(data);
				if (ret) {
					LogMessage("Mode set to DMR");
					mode = MODE_DMR;
					// This sets the mode to DMR within the modem
					m_modem->writeDMRStart(true);
					modeTimer.start();
				}
			} else if (mode == MODE_DMR) {
				dmr->writeModemSlot2(data);
				modeTimer.start();
			} else {
				LogWarning("DMR data received when in mode %u", mode);
			}
		}

		len = m_modem->readYSFData(data);
		if (ysf != NULL && len > 0U) {
			if (mode == MODE_IDLE && data[0U] == TAG_DATA) {
				LogMessage("Mode set to System Fusion");
				mode = MODE_YSF;
				m_modem->setMode(MODE_YSF);
				modeTimer.start();
			}
			if (mode != MODE_YSF) {
				LogWarning("System Fusion data received when in mode %u", mode);
			} else {
				if (data[0U] == TAG_DATA) {
					data[1U] = 0x00U;		// FICH digest
					ysf->writeData(data, len);
					modeTimer.start();
				}
			}
		}

		if (modeTimer.isRunning() && modeTimer.hasExpired()) {
			LogMessage("Mode set to Idle");

			if (mode == MODE_DMR)
				m_modem->writeDMRStart(false);

			mode = MODE_IDLE;
			m_modem->setMode(MODE_IDLE);
			modeTimer.stop();
		}

		if (dstar != NULL) {
			ret = dstar->hasData();
			if (ret) {
				ret = m_modem->hasDStarSpace();
				if (ret) {
					len = dstar->readData(data);
					if (mode != MODE_DSTAR) {
						LogWarning("D-Star echo data received when in mode %u", mode);
					} else {
						m_modem->writeDStarData(data, len);
						modeTimer.start();
					}
				}
			}
		}

		if (dmr != NULL) {
			ret = m_modem->hasDMRSpace1();
			if (ret) {
				len = dmr->readModemSlot1(data);
				if (len > 0U && mode == MODE_IDLE)
					mode = MODE_DMR;
				if (len > 0U && mode == MODE_DMR) {
					m_modem->writeDMRData1(data, len);
					modeTimer.start();
				}
			}

			ret = m_modem->hasDMRSpace2();
			if (ret) {
				len = dmr->readModemSlot2(data);
				if (len > 0U && mode == MODE_IDLE)
					mode = MODE_DMR;
				if (len > 0U && mode == MODE_DMR) {
					m_modem->writeDMRData2(data, len);
					modeTimer.start();
				}
			}
		}

		if (ysf != NULL) {
			ret = ysf->hasData();
			if (ret) {
				ret = m_modem->hasYSFSpace();
				if (ret) {
					len = ysf->readData(data);
					if (mode != MODE_YSF) {
						LogWarning("System Fusion echo data received when in mode %u", mode);
					} else {
						m_modem->writeYSFData(data, len);
						modeTimer.start();
					}
				}
			}
		}

		unsigned int ms = stopWatch.elapsed();
		m_modem->clock(ms);
		modeTimer.clock(ms);
		if (dstar != NULL)
			dstar->clock(ms);
		if (dmr != NULL)
			dmr->clock(ms);
		if (ysf != NULL)
			ysf->clock(ms);
		stopWatch.start();

		if (ms < 5U) {
#if defined(WIN32)
			::Sleep(5UL);		// 5ms
#else
			::usleep(5000);		// 5ms
#endif
		}
	}

	LogMessage("MMDVMHost is exiting on receipt of SIGHUP1");

	m_modem->close();
	delete m_modem;

	if (m_dmrNetwork != NULL) {
		m_dmrNetwork->close();
		delete m_dmrNetwork;
	}

	delete dstar;
	delete dmr;
	delete ysf;

	return 0;
}

bool CMMDVMHost::createModem()
{
    std::string port       = m_conf.getModemPort();
    bool rxInvert          = m_conf.getModemRXInvert();
    bool txInvert          = m_conf.getModemTXInvert();
    bool pttInvert         = m_conf.getModemPTTInvert();
    unsigned int txDelay   = m_conf.getModemTXDelay();
    unsigned int rxLevel   = m_conf.getModemRXLevel();
    unsigned int txLevel   = m_conf.getModemTXLevel();
    bool debug             = m_conf.getModemDebug();
	unsigned int colorCode = m_conf.getDMRColorCode();

	m_modem = new CModem(port, rxInvert, txInvert, pttInvert, txDelay, rxLevel, txLevel, debug);
	m_modem->setModeParams(m_dstarEnabled, m_dmrEnabled, m_ysfEnabled);
	m_modem->setDMRParams(colorCode);

	bool ret = m_modem->open();
	if (!ret) {
		delete m_modem;
		m_modem = NULL;
		return false;
	}

	return true;
}

bool CMMDVMHost::createDMRNetwork()
{
	if (!m_conf.getDMRNetworkEnabled())
		return false;

	std::string address  = m_conf.getDMRNetworkAddress();
	unsigned int port    = m_conf.getDMRNetworkPort();
	unsigned int id      = m_conf.getDMRId();
	std::string password = m_conf.getDMRNetworkPassword();
	bool debug           = m_conf.getDMRNetworkDebug();
	m_dmrNetwork = new CHomebrewDMRIPSC(address, port, id, password, VERSION, "MMDVMHost", debug);

	std::string callsign     = m_conf.getCallsign();
	unsigned int rxFrequency = m_conf.getRxFrequency();
	unsigned int txFrequency = m_conf.getTxFrequency();
	unsigned int power       = m_conf.getPower();
	unsigned int colorCode   = m_conf.getDMRColorCode();
	float latitude           = m_conf.getLatitude();
	float longitude          = m_conf.getLongitude();
	int height               = m_conf.getHeight();
	std::string location     = m_conf.getLocation();
	std::string description  = m_conf.getDescription();
	std::string url          = m_conf.getURL();
	m_dmrNetwork->setConfig(callsign, rxFrequency, txFrequency, power, colorCode, latitude, longitude, height, location, description, url);

	bool ret = m_dmrNetwork->open();
	if (!ret) {
		delete m_dmrNetwork;
		m_dmrNetwork = NULL;
		return false;
	}

	return true;
}

void CMMDVMHost::readParams()
{
	m_dstarEnabled = m_conf.getDStarEnabled();
	m_dmrEnabled   = m_conf.getDMREnabled();
	m_ysfEnabled   = m_conf.getFusionEnabled();

	if (!m_conf.getDuplex() && m_dmrEnabled) {
		LogWarning("DMR operation disabled because system is not duplex");
		m_dmrEnabled = false;
	}
}
