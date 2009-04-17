package rhomobile;

import net.rim.device.api.ui.component.LabelField;
import net.rim.device.api.ui.container.MainScreen;
import net.rim.device.api.ui.component.ObjectChoiceField;
import net.rim.device.api.ui.component.EditField;
import com.rho.RhoLogger;
import com.rho.RhoLogConf;

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
