#pragma once

#include "pch.h"

namespace TestMetro
{
	namespace Common
	{
		/// <summary>
		/// Value converter that translates true to <see cref="Visibility.Visible"/> and false
		/// to <see cref="Visibility.Collapsed"/>.
		/// </summary>
		public ref class BooleanToVisibilityConverter sealed : Windows::UI::Xaml::Data::IValueConverter
		{
		public:
			virtual Object^ Convert(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^);
			virtual Object^ ConvertBack(Object^ value, Windows::UI::Xaml::Interop::TypeName targetType, Object^ parameter, Platform::String^);
		};
	}
}
