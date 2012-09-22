#pragma once

namespace rhoruntime
{
	public delegate void CallbackPointer();

    public ref class CRhoRuntime sealed
    {
    public:
		CRhoRuntime();
		void SetCallback(CallbackPointer^ callback);
		void DoCallback();
	private:
		CallbackPointer^ m_Callback;
    };
}