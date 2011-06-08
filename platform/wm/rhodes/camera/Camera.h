#pragma once

class Camera
{
public:
	Camera(void);
	virtual ~Camera(void);

public: 
	HRESULT takePicture(HWND hwndOwner,LPTSTR pszFilename);
	HRESULT selectPicture(HWND hwndOwner,LPTSTR pszFilename);
};

extern "C" void take_picture(char* callback_url);
extern "C" void choose_picture(char* callback_url);
extern "C" void take_picture(char* callback_url, VALUE options_hash);
extern "C" VALUE get_camera_info(const char* camera_type);
