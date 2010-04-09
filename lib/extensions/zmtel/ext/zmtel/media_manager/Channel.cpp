#include "../stdafx.h"
#include "Channel.h"

#include <Winsock2.h>

CChannel::CChannel(void)
  : m_Id(INVALID_CHANNEL)
  , m_hMM(NULL)
  , m_bAutoFecEnable(0)
{
  m_LocalRtpAddr.ip.i32 = ADDR_ANY;
  m_LocalRtpAddr.port = 0;

  m_LocalRtcpAddr.ip.i32 = ADDR_ANY;
  m_LocalRtcpAddr.port = 0;

  m_RemoteRtpAddr.ip.i32 = ADDR_ANY;
  m_RemoteRtpAddr.port = 0;

  m_RemoteRtcpAddr.ip.i32 = ADDR_ANY;
  m_RemoteRtcpAddr.port = 0;
}

CChannel::~CChannel(void)
{
  Deinit();
}

bool CChannel::Init(tMMHandle hMM)
{
  if(NULL == hMM)
    return false;

  m_hMM = hMM;
  return true;
}
bool CChannel::Deinit()
{
  m_hMM = NULL;
  return true;
}

bool CChannel::SetLocalIp(const char* pIp)
{
  if(!pIp)
    return false;

  unsigned long addr = inet_addr(pIp);
  if(INADDR_NONE == addr)
    return false;

  m_LocalRtpAddr.ip.i32 = m_LocalRtcpAddr.ip.i32 = htonl(addr);
  return true;
}

bool CChannel::SetLocalRtpPort(unsigned short Port)
{
  m_LocalRtpAddr.port = Port;
  return true;
}

bool CChannel::SetLocalRtcpPort(unsigned short Port)
{
  m_LocalRtcpAddr.port = Port;
  return true;
}

bool CChannel::SetRemoteIp(const char* pIp)
{
  if(!pIp)
    return false;

  unsigned long addr = inet_addr(pIp);
  if(INADDR_NONE == addr)
    return false;

  m_RemoteRtpAddr.ip.i32 = m_RemoteRtcpAddr.ip.i32 = htonl(addr);
  return true;
}
bool CChannel::SetRemoteRtpPort(unsigned short Port)
{
  m_RemoteRtpAddr.port = Port;
  return true;
}

bool CChannel::SetRemoteRtcpPort(unsigned short Port)
{
  m_RemoteRtcpAddr.port = Port;
  return true;
}

uint32 CChannel::Id()
{
  return m_Id; 
}

bool CChannel::SetAutoFecEnable(uint32 val)
{
  m_bAutoFecEnable = val;
  return true;
}
