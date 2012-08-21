//
// App.xaml.cpp
// Implementation of the App.xaml class.
//
//#pragma comment(lib, "ruffbylib.lib")

#include "pch.h"
#include "BlankPage.xaml.h"
#include "ruby/ext/rho/rhoruby.h"
#include <windows.h>
#include <fileapi.h>
using namespace TestMetro;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

std::string wideCharToMultiByte( const wchar_t *lpwstr)
{
int size = WideCharToMultiByte( CP_UTF8, 0, lpwstr, -1, NULL, 0, NULL, NULL );

char *buffer = new char[ size+1];
WideCharToMultiByte( CP_UTF8, 0, lpwstr, -1, buffer, size, NULL, NULL );

std::string str( buffer);
delete []buffer;

return str;
}


extern "C" const char* rho_native_rhopath()
{
	std::wstring s = Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data();
	s.append(L"\\rho");
	std::string  result = wideCharToMultiByte(s.c_str());
	const char * d = result.c_str();
	return "C:\\Users\\kons\\AppData\\Local\\Packages\\f4159207-d2e4-44be-8519-5d35dbdf8e84_eeq4jmbew1eya\\LocalState\\rho\\";
}

extern "C" const char* rho_native_reruntimepath()
{
	return rho_native_rhopath();
}

// The Split Application template is documented at http://go.microsoft.com/fwlink/?LinkId=234228

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
#include <stdio.h>
#include <io.h>
#include <logging/RhoLogConf.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

App::App()
{
	InitializeComponent();
	Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
	std::wstring s = Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data();
	//HANDLE h = CreateFile2(L"C:\\Users\\kons\\AppData\\Local\\Packages\\f4159207-d2e4-44be-8519-5d35dbdf8e84_eeq4jmbew1eya\\LocalState\\NUL",GENERIC_ALL,0,OPEN_ALWAYS,0);
	//FILE* f = fopen("C:\\Users\\kons\\AppData\\Local\\Packages\\f4159207-d2e4-44be-8519-5d35dbdf8e84_eeq4jmbew1eya\\LocalState\\sample.txt","w");
	//f = fopen("C:\\Users\\kons\\AppData\\Local\\Packages\\f4159207-d2e4-44be-8519-5d35dbdf8e84_eeq4jmbew1eya\\sample.txt","r");
	//f = fopen("C:\\Users\\kons\\Documents\\sample.txt","r");
	//f = fopen("sdfsdf.txt","w");
	//int ret = open("C:\\Users\\kons\\AppData\\Local\\Packages\\f4159207-d2e4-44be-8519-5d35dbdf8e84_eeq4jmbew1eya\\LocalState\\rho\\lib\\rhoframework.iseq", _O_RDONLY);
	//ret = open("C:\\Users\\kons\\Documents\\sample.txt", _O_RDONLY);
	//ret = open("sdfsdf.txt", _O_CREAT);
	//int ret = open("/rho/lib/rhoframework.iseq", _O_CREAT);
	//ret = open("rho/lib/rhoframework.iseq", _A_RDONLY);
	//ret = open("/rhoframework.iseq", _A_RDONLY);
	//ret = open("rhoframework.iseq", _A_RDONLY);
	//LOGCONF().setLogToOutput(true);
	RhoRubyStart();
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used when the application is launched to open a specific file, to display
/// search results, and so forth.
/// </summary>
/// <param name="args">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs)
{
	if (pArgs->PreviousExecutionState == ApplicationExecutionState::Terminated)
	{
		//TODO: Load state from previously suspended application
	}

	// Create a Frame to act navigation context and navigate to the first page
	auto rootFrame = ref new Frame();
	TypeName pageType = { BlankPage::typeid->FullName, TypeKind::Metadata };
	rootFrame->Navigate(pageType);

	// Place the frame in the current Window and ensure that it is active
	Window::Current->Content = rootFrame;
	Window::Current->Activate();
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
	//TODO: Save application state and stop any background activity
}
