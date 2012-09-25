#pragma once

namespace rhoruntime
{
	public delegate void WaitCallbackPointer(int timeout);
	public delegate void UpdateWebViewCallbackPointer(int counter);
	public delegate void ExitCallbackPointer();

    public ref class CRhoRuntime sealed
    {
    public:
		CRhoRuntime();

		// this callback is an example of calling the C# API methods from C++
		void SetWaitCallback(WaitCallbackPointer^ callback);
		// this callback is an example of updating UI controls from C++ non-UI thread
		void SetUpdateWebViewCallback(UpdateWebViewCallbackPointer^ callback);
		// this callback is an example of application exit technique
		void SetExitCallback(ExitCallbackPointer^ callback);

		// rhodes executed in a separate thread
		void Execute();

	private:
		WaitCallbackPointer^ m_WaitCallback;
		UpdateWebViewCallbackPointer^ m_UpdateWebViewCallback;
		ExitCallbackPointer^ m_ExitCallback;
    };
}