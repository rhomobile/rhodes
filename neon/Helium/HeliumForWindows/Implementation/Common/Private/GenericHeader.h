/******************************************************************************/
#define NEON_TO_HOST		L"NeonToHost"
#define NEON_FROM_HOST		L"NeonFromHost"
#define NEON_MAX_MESSAGE_LENGTH		1024

/******************************************************************************
* Values for uType in NEONHEADER
******************************************************************************/
#define NHT_RESPONSE		1

// Invoke MetaFunc: request consists of NEONHEADER followed by null-terminated 'http-equiv' string,
// followed by null-terminated 'content' string. Response passed in NEONRESPONSE.lValue will be
// returned to Javascript calling function.
#define NHT_INVOKE			2

// Hourglass control: request consists of NEONHEADER only, no response required.
#define NHT_HIDE_HOURGLASS	3
#define NHT_SHOW_HOURGLASS	4

// Log: request consists of NEONHEADER followed by NEONLOG with severity,
// followed by null-terminated comment string.
#define NHT_LOG				5

// Write user setting: request consists of NEONHEADER followed by null-terminated 'setting' string,
// followed by null-terminated 'value' string. Response passed in NEONRESPONSE.iValue will be
// zero for success, non-zero otherwise
#define NHT_WRITE_USER_SETTING		6

// Read user setting: request consists of NEONHEADER followed by null-terminated 'setting' string.
// Response consists of NEONRESPONSE with iValue indicating success (0 = OK), followed by null-terminated 'value' string.
#define NHT_READ_USER_SETTING		7

// Write config setting: request consists of NEONHEADER followed by null-terminated 'setting' string,
// followed by null-terminated 'value' string, followed by null-terminated 'app' string (GLOBAL or app name).
// Response passed in NEONRESPONSE.iValue will be zero for success, non-zero otherwise.
#define NHT_WRITE_CONFIG_SETTING	8

// Read config setting: request consists of NEONHEADER followed by null-terminated 'setting' string,
// followed by null-terminated 'app' string (GLOBAL or app name).
// Response consists of NEONRESPONSE with iValue indicating success (0 = OK), followed by null-terminated 'value' string.
#define NHT_READ_CONFIG_SETTING		9

/******************************************************************************/
struct NEONHEADER
{
	UINT uType;
};

/******************************************************************************/
struct NEONLOG
{
	UINT uSeverity;
};

/******************************************************************************/
struct NEONRESPONSE
{
	NEONHEADER Header;

	union
	{
		LONG lValue;
		int iValue;
	};
};
