#include "pch.h"
#include "BindableBase.h"

using namespace TestMetro::Common;

using namespace Platform;
using namespace Windows::UI::Xaml::Data;

/// <summary>
/// Notifies listeners that a property value has changed.
/// </summary>
/// <param name="propertyName">Name of the property used to notify listeners.</param>
void BindableBase::OnPropertyChanged(String^ propertyName)
{
	PropertyChanged(this, ref new PropertyChangedEventArgs(propertyName));
}
