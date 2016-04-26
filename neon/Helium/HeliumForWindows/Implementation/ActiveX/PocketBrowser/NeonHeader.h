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

/******************************************************************************/
struct NEONHEADER
{
	UINT uType;
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
