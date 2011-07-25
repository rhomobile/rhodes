/*------------------------------------------------------------------------
* (The MIT License)
* 
* Copyright (c) 2008-2011 Rhomobile, Inc.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
* 
* http://rhomobile.com
*------------------------------------------------------------------------*/

package rhomobile;

import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.container.MainScreen;
import net.rim.device.api.ui.component.ObjectChoiceField;
import net.rim.device.api.ui.component.EditField;
import com.rho.RhoLogger;

public class LogOptionsScreen extends MainScreen {

	ObjectChoiceField m_cbxLevels;
	EditField         m_enabledCat;
	EditField         m_disabledCat;
	
	public LogOptionsScreen(){
		setTitle( new LabelField( "Log Options", LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH ) );
		
		String levels[] = {"Trace","Info","Warning","Error"};
		m_cbxLevels = new ObjectChoiceField("Log level: ", levels);
		m_cbxLevels.setSelectedIndex(RhoLogger.getLogConf().getMinSeverity());
		add(m_cbxLevels);
		
		m_enabledCat = new EditField("Message classes(* for all):",
				RhoLogger.getLogConf().getEnabledCategories());
		m_disabledCat = new EditField("Exclude message classes:",
				RhoLogger.getLogConf().getDisabledCategories()); 
		
		add(m_enabledCat);
		add(m_disabledCat);
	}

	protected boolean onSave() {
		RhoLogger.getLogConf().setMinSeverity(m_cbxLevels.getSelectedIndex());
		RhoLogger.getLogConf().setEnabledCategories(m_enabledCat.getText());
		RhoLogger.getLogConf().setDisabledCategories(m_disabledCat.getText());
		
		RhoLogger.getLogConf().saveToFile();
		
		return super.onSave();
	}
}
