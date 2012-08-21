#include "pch.h"
#include "LayoutAwarePage.h"

using namespace TestMetro::Common;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

/// <summary>
/// Initializes a new instance of the <see cref="LayoutAwarePage"/> class.
/// </summary>
LayoutAwarePage::LayoutAwarePage()
{
	if (Windows::ApplicationModel::DesignMode::DesignModeEnabled) return;

	// Map application view state to visual state for this page when it is part of the visual tree
	Loaded += ref new RoutedEventHandler(this, &LayoutAwarePage::StartLayoutUpdates);
	Unloaded += ref new RoutedEventHandler(this, &LayoutAwarePage::StopLayoutUpdates);

	// Establish the default view model as the initial DataContext
	DataContext = _defaultViewModel = ref new Map<String^, Object^>(std::less<String^>());
}

/// <summary>
/// Gets an implementation of <see cref="IObservableMap<String, Object>"/> set as the page's
/// default <see cref="DataContext"/>.  This instance can be bound and surfaces property change
/// notifications making it suitable for use as a trivial view model.
/// </summary>
IObservableMap<Platform::String^, Object^>^ LayoutAwarePage::DefaultViewModel::get()
{
	return _defaultViewModel;
}

/// <summary>
/// Gets a value indicating whether visual states can be a loose interpretation of the actual
/// application view state.  This is often convenient when a page layout is space constrained.
/// </summary>
/// <remarks>
/// The default value of false indicates that the visual state is identical to the view state,
/// meaning that Filled is only used when another application is snapped.  When set to true
/// FullScreenLandscape is used to indicate that at least 1366 virtual pixels of horizontal real
/// estate are available - even if another application is snapped - and Filled indicates a lesser
/// width, even if no other application is snapped.  On a smaller display such as a 1024x768
/// panel this will result in the visual state Filled whenever the device is in landscape
/// orientation.
/// </remarks>
bool LayoutAwarePage::UseFilledStateForNarrowWindow::get()
{
	return _useFilledStateForNarrowWindow;
}

/// <summary>
/// Sets a value indicating whether visual states can be a loose interpretation of the actual
/// application view state.  This is often convenient when a page layout is space constrained.
/// </summary>
/// <remarks>
/// The default value of false indicates that the visual state is identical to the view state,
/// meaning that Filled is only used when another application is snapped.  When set to true
/// FullScreenLandscape is used to indicate that at least 1366 virtual pixels of horizontal real
/// estate are available - even if another application is snapped - and Filled indicates a lesser
/// width, even if no other application is snapped.  On a smaller display such as a 1024x768
/// panel this will result in the visual state Filled whenever the device is in landscape
/// orientation.
/// </remarks>
void LayoutAwarePage::UseFilledStateForNarrowWindow::set(bool value)
{
	_useFilledStateForNarrowWindow = value;
	this->InvalidateVisualState();
}

/// <summary>
/// Invoked as an event handler to navigate backward in the page's associated <see cref="Frame"/>
/// until it reaches the top of the navigation stack.
/// </summary>
/// <param name="sender">Instance that triggered the event.</param>
/// <param name="e">Event data describing the conditions that led to the event.</param>
void LayoutAwarePage::GoHome(Object^ sender, RoutedEventArgs^ e)
{
	// Use the navigation frame to return to the topmost page
	if (Frame != nullptr)
	{
		while (Frame->CanGoBack) Frame->GoBack();
	}
}

/// <summary>
/// Invoked as an event handler to navigate backward in the page's associated <see cref="Frame"/>
/// to go back one step on the navigation stack.
/// </summary>
/// <param name="sender">Instance that triggered the event.</param>
/// <param name="e">Event data describing the conditions that led to the event.</param>
void LayoutAwarePage::GoBack(Object^ sender, RoutedEventArgs^ e)
{
	// Use the navigation frame to return to the previous page
	if (Frame != nullptr && Frame->CanGoBack) Frame->GoBack();
}

/// <summary>
/// Invoked as an event handler, typically on the <see cref="Loaded"/> event of a
/// <see cref="Control"/> within the page, to indicate that the sender should start receiving
/// visual state management changes that correspond to application view state changes.
/// </summary>
/// <param name="sender">Instance of <see cref="Control"/> that supports visual state management
/// corresponding to view states.</param>
/// <param name="e">Event data that describes how the request was made.</param>
/// <remarks>The current view state will immediately be used to set the corresponding visual state
/// when layout updates are requested.  A corresponding <see cref="Unloaded"/> event handler
/// connected to <see cref="StopLayoutUpdates"/> is strongly encouraged.  Instances of
/// <see cref="LayoutAwarePage"/> automatically invoke these handlers in their Loaded and Unloaded
/// events.</remarks>
/// <seealso cref="DetermineVisualState"/>
/// <seealso cref="InvalidateVisualState"/>
void LayoutAwarePage::StartLayoutUpdates(Object^ sender, RoutedEventArgs^ e)
{
	auto control = safe_cast<Control^>(sender);
	if (_layoutAwareControls == nullptr) {
		// Start listening to view state changes when there are controls interested in updates
		_layoutAwareControls = ref new Vector<Control^>();
		_viewStateEventToken = ApplicationView::GetForCurrentView()->ViewStateChanged += ref new TypedEventHandler<ApplicationView^,ApplicationViewStateChangedEventArgs^>(this, &LayoutAwarePage::ViewStateChanged);
		_windowSizeEventToken = Window::Current->SizeChanged += ref new WindowSizeChangedEventHandler(this, &LayoutAwarePage::WindowSizeChanged);
	}
	_layoutAwareControls->Append(control);

	// Set the initial visual state of the control
	VisualStateManager::GoToState(control, DetermineVisualState(ApplicationView::Value), false);
}

void LayoutAwarePage::ViewStateChanged(ApplicationView^ sender, ApplicationViewStateChangedEventArgs^ e)
{
	InvalidateVisualState(e->ViewState);
}

void LayoutAwarePage::WindowSizeChanged(Object^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ e)
{
	if (_useFilledStateForNarrowWindow) InvalidateVisualState();
}

/// <summary>
/// Invoked as an event handler, typically on the <see cref="Unloaded"/> event of a
/// <see cref="Control"/>, to indicate that the sender should start receiving visual state
/// management changes that correspond to application view state changes.
/// </summary>
/// <param name="sender">Instance of <see cref="Control"/> that supports visual state management
/// corresponding to view states.</param>
/// <param name="e">Event data that describes how the request was made.</param>
/// <remarks>The current view state will immediately be used to set the corresponding visual state
/// when layout updates are requested.</remarks>
/// <seealso cref="StartLayoutUpdates"/>
void LayoutAwarePage::StopLayoutUpdates(Object^ sender, RoutedEventArgs^ e)
{
	auto control = safe_cast<Control^>(sender);
	unsigned int index;
	if (_layoutAwareControls != nullptr && _layoutAwareControls->IndexOf(control, &index))
	{
		_layoutAwareControls->RemoveAt(index);
		if (_layoutAwareControls->Size == 0)
		{
			// Stop listening to view state changes when no controls are interested in updates
			ApplicationView::GetForCurrentView()->ViewStateChanged -= _viewStateEventToken;
			Window::Current->SizeChanged -= _windowSizeEventToken;
			_layoutAwareControls = nullptr;
		}
	}
}

/// <summary>
/// Translates <see cref="ApplicationViewState"/> values into strings for visual state management
/// within the page.  The default implementation uses the names of enum values.  Subclasses may
/// override this method to control the mapping scheme used.
/// </summary>
/// <param name="viewState">View state for which a visual state is desired.</param>
/// <returns>Visual state name used to drive the <see cref="VisualStateManager"/></returns>
/// <seealso cref="InvalidateVisualState"/>
Platform::String^ LayoutAwarePage::DetermineVisualState(ApplicationViewState viewState)
{
	auto actualViewState = viewState;
	if (_useFilledStateForNarrowWindow &&
		(viewState == ApplicationViewState::Filled ||
		viewState == ApplicationViewState::FullScreenLandscape))
	{
		// Allow pages to request that the Filled state be used only for landscape layouts narrower
		// than 1366 virtual pixels
		auto windowWidth = Window::Current->Bounds.Width;
		actualViewState = windowWidth >= 1366 ? ApplicationViewState::FullScreenLandscape : ApplicationViewState::Filled;
	}
	switch (actualViewState)
	{
	case ApplicationViewState::Filled: return "Filled";
	case ApplicationViewState::Snapped: return "Snapped";
	case ApplicationViewState::FullScreenPortrait: return "FullScreenPortrait";
	default: case ApplicationViewState::FullScreenLandscape: return "FullScreenLandscape";
	}
}

/// <summary>
/// Updates all controls that are listening for visual state changes with the correct visual
/// state.
/// </summary>
/// <remarks>
/// Typically used in conjunction with overriding <see cref="DetermineVisualState"/> to
/// signal that a different value may be returned even though the view state has not changed.
/// </remarks>
void LayoutAwarePage::InvalidateVisualState()
{
	InvalidateVisualState(ApplicationView::Value);
}

/// <summary>
/// Updates all controls that are listening for visual state changes with the correct visual
/// state.
/// </summary>
/// <remarks>
/// Typically used in conjunction with overriding <see cref="DetermineVisualState"/> to
/// signal that a different value may be returned even though the view state has not changed.
/// </remarks>
/// <param name="viewState">The desired view state, or null if the current view state should be
/// used.</param>
void LayoutAwarePage::InvalidateVisualState(ApplicationViewState viewState)
{
	if (_layoutAwareControls != nullptr)
	{
		String^ visualState = DetermineVisualState(viewState);
		auto controlIterator = _layoutAwareControls->First();
		while (controlIterator->HasCurrent)
		{
			auto control = controlIterator->Current;
			VisualStateManager::GoToState(control, visualState, false);
			controlIterator->MoveNext();
		}
	}
}
