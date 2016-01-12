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

#if !defined(CONF_H)
#define	CONF_H

#include <string>

class CConf
{
public:
  CConf(const std::string& file);
  ~CConf();

  bool read();

  // The General section
  std::string  getCallsign() const;
  unsigned int getTimeout() const;
  bool         getDuplex() const;
  unsigned int getModeHang() const;
  std::string  getDisplay() const;

  // The Info section
  unsigned int getRxFrequency() const;
  unsigned int getTxFrequency() const;
  unsigned int getPower() const;
  float        getLatitude() const;
  float        getLongitude() const;
  int          getHeight() const;
  std::string  getLocation() const;
  std::string  getDescription() const;
  std::string  getURL() const;

  // The Log section
  std::string  getLogPath() const;
  std::string  getLogRoot() const;
  unsigned int getLogLevel() const;
  bool         getLogDisplay() const;

  // The Modem section
  std::string  getModemPort() const;
  bool         getModemRXInvert() const;
  bool         getModemTXInvert() const;
  bool         getModemPTTInvert() const;
  unsigned int getModemTXDelay() const;
  unsigned int getModemRXLevel() const;
  unsigned int getModemTXLevel() const;
  bool         getModemDebug() const;

  // The D-Star section
  bool         getDStarEnabled() const;
  std::string  getDStarModule() const;

  // The DMR section
  bool         getDMREnabled() const;
  unsigned int getDMRId() const;
  unsigned int getDMRColorCode() const;

  // The System Fusion section
  bool         getFusionEnabled() const;

  // The D-Star Network section
  bool         getDStarNetworkEnabled() const;
  std::string  getDStarGatewayAddress() const;
  unsigned int getDStarGatewayPort() const;
  unsigned int getDStarLocalPort() const;
  bool         getDStarNetworkDebug() const;

  // The DMR Network section
  bool         getDMRNetworkEnabled() const;
  std::string  getDMRNetworkAddress() const;
  unsigned int getDMRNetworkPort() const;
  std::string  getDMRNetworkPassword() const;
  bool         getDMRNetworkDebug() const;

  // The System Fusion Network section
  bool         getFusionNetworkEnabled() const;
  std::string  getFusionNetworkAddress() const;
  unsigned int getFusionNetworkPort() const;
  bool         getFusionNetworkDebug() const;

  // The TFTSERIAL section
  std::string  getTFTSerialPort() const;

private:
  std::string  m_file;
  std::string  m_callsign;
  unsigned int m_timeout;
  bool         m_duplex;
  unsigned int m_modeHang;
  std::string  m_display;

  unsigned int m_rxFrequency;
  unsigned int m_txFrequency;
  unsigned int m_power;
  float        m_latitude;
  float        m_longitude;
  int          m_height;
  std::string  m_location;
  std::string  m_description;
  std::string  m_url;

  unsigned int m_logLevel;
  std::string  m_logPath;
  std::string  m_logRoot;
  bool         m_logDisplay;

  std::string  m_modemPort;
  bool         m_modemRXInvert;
  bool         m_modemTXInvert;
  bool         m_modemPTTInvert;
  unsigned int m_modemTXDelay;
  unsigned int m_modemRXLevel;
  unsigned int m_modemTXLevel;
  bool         m_modemDebug;

  bool         m_dstarEnabled;
  std::string  m_dstarModule;

  bool         m_dmrEnabled;
  unsigned int m_dmrId;
  unsigned int m_dmrColorCode;

  bool         m_fusionEnabled;

  bool         m_dstarNetworkEnabled;
  std::string  m_dstarGatewayAddress;
  unsigned int m_dstarGatewayPort;
  unsigned int m_dstarLocalPort;
  bool         m_dstarNetworkDebug;

  bool         m_dmrNetworkEnabled;
  std::string  m_dmrNetworkAddress;
  unsigned int m_dmrNetworkPort;
  std::string  m_dmrNetworkPassword;
  bool         m_dmrNetworkDebug;

  bool         m_fusionNetworkEnabled;
  unsigned int m_fusionNetworkPort;
  std::string  m_fusionNetworkAddress;
  bool         m_fusionNetworkDebug;

  std::string  m_tftSerialPort;
};

#endif
