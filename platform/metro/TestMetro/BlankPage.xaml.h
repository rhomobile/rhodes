//
// BlankPage.xaml.h
// Declaration of the BlankPage.xaml class.
//

#pragma once

#include "pch.h"
#include "BlankPage.g.h"

namespace TestMetro
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class BlankPage sealed
	{
	public:
		BlankPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	};
}
