# Blackberry HTML And CSS

## Introduction
The BlackBerry Browser version 4.2 - the version available to native BlackBerry applications - has extremely limited support for modern css.  Most notably, several key styles that are used in the standard views (like divs with floats) are not supported in the BlackBerry browser.  

In order to take advantage of the features found in the modern browsers on other supported devices, it is necessary to provide alternate view files with a structure that mitigates BlackBerry's limited support for modern css styles.  Instead, table-based view files are loaded when Rhodes applications are run in the BlackBerry device. 

View files customized for the BlackBerry are found in the same directory as their standard counterparts, and can be distinguished from standard view files by the replacement of the standard .erb extension with '.bb.erb'.  In order to load at runtime, these view files must have the same name as their standard counterparts.

As with the other platforms, the default styles of the BlackBerry stylesheet can be found in the <code>pageTitle</code>, <code>toolbar</code> and <code>content</code> divs.  

If you wish to include additional styles in your BlackBerry views, you may wish to refer to the [BlackBerry Browser Version 4.2 Content Developer Guide](http://docs.blackberry.com/en/developers/deliverables/1143/browser_devguide.pdf)


## HTML Structure

### pageTitle (\<div id="pageTitle"\>) 

The pageTitle div contains the title of the current page, wrapped in an h1 tag.  The page title will be displayed at the top of each application view.

	:::html
	<div id="pageTitle">
		<h1>Things</h1>
	</div>


### toolbar  (\<div id="toolbar"\>)

The toolbar div contains contains interaction elements that allow users to perform actions and navigate through the application.  On the BlackBerry, these elements are styled as a series of inline links. 

	:::html
	<div id="toolbar">
	  <%= link_to "Back", :action => :index %>
	  <%= link_to "Edit", :action => :edit, :id => @uiformdemo.object %>
	</div>

The toolbar div is displayed directly below the pageTitle div.

### Content (\<div id="content"\>)

As in standard smartphones, lists, forms, and other content must be placed inside the content div in order to make use of the styles contained in the generated stylesheets.

#### Lists

Simple lists of items or links can be displayed on the BlackBerry. 

	:::html 
	<ul>
		<li><%= url_for :controller => :Product %></li>
		<li><%= url_for :controller => :Inventory %></li>
		<li><%= url_for :controller => :Location %></li>
		<li><%= url_for :controller => :Purchase %></li>
	</ul>


Examples can be found in the generated application and model index files.

#### Tables

Any content that requires precise alignment of containers should be placed in tables.  

BlackBerry Browser 4.2 does not support defining table data cell widths through css.  If you wish to maintain evenly-spaced columns throughout your form, you must manually define the width of one column in every row.  Typically, setting the width of the label-containing cell to 125 (width="125") provides sufficient width for a descriptive label without compromising the space available for user input.

Examples of tables with a defined width can be found in generated model new.bb.erb files.  Examples of tables with no width defined can be seen in generated model show.bb.erb files.  


## Forms

### Creating a custom "New" Form for BlackBerry

You can create native-styled forms by organizing form fields in one or more tables inside a view's "content" div. 

Include a form tag inside the \<div id="content"\> tag. The form should use the post method to submit to the current controller's "create" action.

	:::html    
	<form method="POST" action="<%= url_for :action => :create %>">
	</form>


Add a \<table\> tag inside your form.  To break your fields into multiple groupings, include one \<table\> tag to contain each group as described above.<br/>
**NOTE: to append a label to a group of fields, include the label inside a \<h2 class="groupTitle"\> tag immediately before the table containing those fields. **


Add one \<tr\> tag inside the \<table\> tag for each input field/field label.  You must adhere to the syntax found in the examples below to access the appropriate styles for the BlackBerry.<br/>        
**NOTE: each checkbox and radio button must be contained inside its own row.  Further discussion of the formatting can be found later in this document. **


To include a "submit" button, immediately below the last closing \</table\> tag and immediately above the closing \</form\> tag.  
**NOTE: the button should ''not'' be contained inside an \<tr\> tag. **

	:::ruby
	<input type="submit" value="Create" />


### Creating a custom "Edit" Form for BlackBerry

You can create native-styled forms by organizing form fields in one or more tables inside a view's "content" div. 


Include a form tag inside the \<div id="content"\> tag. The form should use the post method to submit to the current controller's "create" or "update" actions.
    
	:::html
    <form method="POST" action="<%= url_for :action => : update %>">
    </form>

Add a \<table\> tag inside your form.  To break your fields into multiple groupings, include one \<table\> tag to contain each group as described above.<br/>
**NOTE: to append a label to a group of fields, include the label inside a \<h2 class="groupTitle"\> tag immediately before the table containing those fields. **


When creating an edit form, it should include a hidden field that contains the id of the  object being edited.

	:::html
	<input type="hidden" name="id" value="<%= @model_name.object %>"/>


Add one \<tr\> tag inside the \<table\> tag for each input field/field label. You must adhere to the syntax found in the examples below to access the appropriate styles for the BlackBerry.<br/>        
**NOTE: each checkbox and radio button must be contained inside its own row.  Further discussion of the formatting can be found later in this document. **


To include a "submit" button, immediately below the last closing <nowiki></table></nowiki> tag and immediately above the closing <nowiki></form></nowiki> tag.  
**NOTE: the button should ''not'' be contained inside an <nowiki><tr></nowiki> tag. **

	:::html
    <input type="submit" value="Update" />

## Form elements

Each form element will take up a full table row.  Note: each checkbox and radio button will need to be contained inside its own list item.

BlackBerry Browser 4.2 does not support defining table data cell widths through css.  If you wish to maintain evenly-spaced columns throughout your form, you must manually define the width of one column in every row.  Typically, setting the width of the label-containing cell to 125 (width="125") provides sufficient width for a descriptive label without compromising the space available for user input.

### Text fields

	:::html
	<tr>
	    <td width="125">Textbox1: </td>
	    <td><input type="text" name="uiformdemo[textbox1]"/></td>
	</tr>


### Checkboxes/Switches

	:::html
    <tr>
	    <td width="125">Checkbox1: </td>
	    <td><input type="text" name="uiformdemo[checkbox1]"/></td>
    </tr>

### Radio Buttons

	:::html
    <h2 class="groupTitle">Radio Button</h2>
    <table>
        <tr>
          <td width="125">Radiobutton1: </td>
          <td><input type="text" name="uiformdemo[radiobutton]"  value="radiobutton1" /></td>
        </tr>

        <tr>
          <td width="125">Radiobutton2: </td>
          <td><input type="text" name="uiformdemo[radiobutton]"  value="radiobutton2" /></td>
        </tr>

        <tr>
          <td width="125">Radiobutton3: </td>
          <td><input type="text" name="uiformdemo[radiobutton]"  value="radiobutton3" /></td>
        </tr>
    </table>

### Select boxes

	:::html
    <h2 class="groupTitle">Select Box</h2>
    <table>
        <tr>
            <td width="125">Select device</td>
            <td>
                <select name="uiformdemo[selectbox]">
                    <option selected>Please select</option>
                    <option value="apple">iPhone</option>
                    <option value="android">Android</option>
                    <option value="blackberry">BlackBerry</option>
                    <option value="winmo">Windows Mobile</option>
                </select>
            </td>
        </tr>
    </table

### Text areas

Each textarea tag must include attributes for the number of rows and columns. The recommended values for rows and columns can be found below.  Additionally, each textarea tag must have both an opening \<textarea\> and closing \</textarea\> tag: self-closing textarea tags will not be displayed properly in your application.


	:::html
    <tr>
        <td width="125">Textarea: </td>
        <td><textarea name="uiformdemo[textarea]" rows="5" cols="30"></textarea></td>
    </tr>

### Buttons

Buttons involved in form submission should be created as input tags, type submit.   Buttons should be placed directly between the last closing \</table\> tag and the closing \</form\> tag, and should not be contained inside an \<table\>,\<tr\>, or \<td\> tag. 

Links used to perform actions related to the form but not involved in form submission (e.g., Delete, Cancel) should be located in the top toolbar instead.

	:::html
	<input type="submit" class="standardButton" value="Create"/>

\<button\> elements are not supported on the BlackBerry platforms.