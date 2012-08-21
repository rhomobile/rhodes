#include "pch.h"
#include "RichTextColumns.h"

using namespace TestMetro::Common;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Interop;

static Windows::UI::Xaml::DependencyProperty^ _richTextContentProperty;
static Windows::UI::Xaml::DependencyProperty^ _columnTemplateProperty;

/// <summary>
/// Initializes a new instance of the <see cref="RichTextColumns"/> class.
/// </summary>
RichTextColumns::RichTextColumns()
{
	HorizontalAlignment = ::HorizontalAlignment::Left;
}

/// <summary>
/// Gets the initial rich text content to be used as the first column.
/// </summary>
DependencyProperty^ RichTextColumns::ColumnTemplateProperty::get()
{
	if (_columnTemplateProperty == nullptr)
	{
		TypeName thisType = { RichTextColumns::typeid->FullName, TypeKind::Custom };
		TypeName dataTemplateType = { DataTemplate::typeid->FullName, TypeKind::Metadata };
		_columnTemplateProperty = DependencyProperty::Register("ColumnTemplate",
			dataTemplateType, thisType, ref new PropertyMetadata(nullptr,
			ref new PropertyChangedCallback(&RichTextColumns::ResetOverflowLayout)));
	}
	return _columnTemplateProperty;
}

/// <summary>
/// Gets the template used to create additional
/// <see cref="RichTextBlockOverflow"/> instances.
/// </summary>
DependencyProperty^ RichTextColumns::RichTextContentProperty::get()
{
	if (_richTextContentProperty == nullptr)
	{
		TypeName thisType = { RichTextColumns::typeid->FullName, TypeKind::Custom };
		TypeName richTextBlockType = { RichTextBlock::typeid->FullName, TypeKind::Metadata };
		_richTextContentProperty = DependencyProperty::Register("RichTextContent",
			richTextBlockType, thisType, ref new PropertyMetadata(nullptr,
			ref new PropertyChangedCallback(&RichTextColumns::ResetOverflowLayout)));
	}
	return _richTextContentProperty;
}

/// <summary>
/// Invoked when the content or overflow template is changed to recreate the column layout.
/// </summary>
/// <param name="d">Instance of <see cref="RichTextColumns"/> where the change
/// occurred.</param>
/// <param name="e">Event data describing the specific change.</param>
void RichTextColumns::ResetOverflowLayout(DependencyObject^ d, DependencyPropertyChangedEventArgs^ e)
{
	auto target = dynamic_cast<RichTextColumns^>(d);
	if (target != nullptr)
	{
		// When dramatic changes occur, rebuild layout from scratch
		target->_overflowColumns = nullptr;
		target->Children->Clear();
		target->InvalidateMeasure();
	}
}

/// <summary>
/// Determines whether additional overflow columns are needed and if existing columns can
/// be removed.
/// </summary>
/// <param name="availableSize">The size of the space available, used to constrain the
/// number of additional columns that can be created.</param>
/// <returns>The resulting size of the original content plus any extra columns.</returns>
Size RichTextColumns::MeasureOverride(Size availableSize)
{
	if (RichTextContent == nullptr)
	{
		Size emptySize(0, 0);
		return emptySize;
	}

	// Make sure the RichTextBlock is a child, using the lack of
	// a list of additional columns as a sign that this hasn't been
	// done yet
	if (_overflowColumns == nullptr)
	{
		// Appending to the child collection currently returns S_FALSE on success,
		// which results in an exception when called from C++
		try { Children->Append(RichTextContent); } catch (COMException^ ex) { if (ex->HResult != 1) throw ex; }
		_overflowColumns = ref new Vector<RichTextBlockOverflow^>();
	}

	// Start by measuring the original RichTextBlock content
	RichTextContent->Measure(availableSize);
	auto maxWidth = RichTextContent->DesiredSize.Width;
	auto maxHeight = RichTextContent->DesiredSize.Height;
	auto hasOverflow = RichTextContent->HasOverflowContent;

	// Make sure there are enough overflow columns
	unsigned int overflowIndex = 0;
	while (hasOverflow && maxWidth < availableSize.Width && ColumnTemplate != nullptr)
	{
		// Use existing overflow columns until we run out, then create
		// more from the supplied template
		RichTextBlockOverflow^ overflow;
		if (_overflowColumns->Size > overflowIndex)
		{
			overflow = _overflowColumns->GetAt(overflowIndex);
		}
		else
		{
			overflow = safe_cast<RichTextBlockOverflow^>(ColumnTemplate->LoadContent());
			_overflowColumns->Append(overflow);
			// Appending to the child collection currently returns S_FALSE on success,
			// which results in an exception when called from C++
			try { Children->Append(overflow); } catch (COMException^ ex) { if (ex->HResult != 1) throw ex; }
			if (overflowIndex == 0)
			{
				RichTextContent->OverflowContentTarget = overflow;
			}
			else
			{
				_overflowColumns->GetAt(overflowIndex - 1)->OverflowContentTarget = overflow;
			}
		}

		// Measure the new column and prepare to repeat as necessary
		Size remainingSize(availableSize.Width - maxWidth, availableSize.Height);
		overflow->Measure(remainingSize);
		maxWidth += overflow->DesiredSize.Width;
		maxHeight = __max(maxHeight, overflow->DesiredSize.Height);
		hasOverflow = overflow->HasOverflowContent;
		overflowIndex++;
	}

	// Disconnect extra columns from the overflow chain, remove them from our private list
	// of columns, and remove them as children
	if (_overflowColumns->Size > overflowIndex)
	{
		if (overflowIndex == 0)
		{
			RichTextContent->OverflowContentTarget = nullptr;
		}
		else
		{
			_overflowColumns->GetAt(overflowIndex - 1)->OverflowContentTarget = nullptr;
		}
		while (_overflowColumns->Size > overflowIndex)
		{
			_overflowColumns->RemoveAt(overflowIndex);
			Children->RemoveAt(overflowIndex + 1);
		}
	}

	// Report final determined size
	Size resultingSize(maxWidth, maxHeight);
	return resultingSize;
}

/// <summary>
/// Arranges the original content and all extra columns.
/// </summary>
/// <param name="finalSize">Defines the size of the area the children must be arranged
/// within.</param>
/// <returns>The size of the area the children actually required.</returns>
Size RichTextColumns::ArrangeOverride(Size finalSize)
{
	float maxWidth = 0;
	float maxHeight = 0;
	auto childrenIterator = Children->First();
	while (childrenIterator->HasCurrent)
	{
		auto child = childrenIterator->Current;
		Rect childRect(maxWidth, 0, child->DesiredSize.Width, finalSize.Height);
		child->Arrange(childRect);
		maxWidth += child->DesiredSize.Width;
		maxHeight = __max(maxHeight, child->DesiredSize.Height);
		childrenIterator->MoveNext();
	}
	Size resultingSize(maxWidth, maxHeight);
	return resultingSize;
}
