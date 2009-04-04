#pragma once

#if defined(_WIN32_WCE)

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

public: 
	HRESULT takePicture(HWND hwndOwner,LPTSTR pszFilename);
	HRESULT selectPicture(HWND hwndOwner,LPTSTR pszFilename);
};

#endif //_WIN32_WCE

extern "C" void take_picture(char* callback_url);
extern "C" void choose_picture(char* callback_url);
