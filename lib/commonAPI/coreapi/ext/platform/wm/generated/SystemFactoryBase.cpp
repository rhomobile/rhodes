#include "ISystem.h"

rho::common::CAutoPtr< rho::CSystemFactoryBase> rho::CSystemFactoryBase::m_pInstance;

//string constants definiton 
const wchar_t rho::ISystem::PLATFORM_WM_CE[] = L"WINDOWS"; 
const wchar_t rho::ISystem::PLATFORM_ANDROID[] = L"ANDROID"; 
const wchar_t rho::ISystem::PLATFORM_IOS[] = L"APPLE"; 
const wchar_t rho::ISystem::PLATFORM_WP8[] = L"WP8"; 
const wchar_t rho::ISystem::PLATFORM_WINDOWS_DESKTOP[] = L"WINDOWS_DESKTOP"; 
const wchar_t rho::ISystem::SCREEN_PORTRAIT[] = L"portrait"; 
const wchar_t rho::ISystem::SCREEN_LANDSCAPE[] = L"landscape"; 
