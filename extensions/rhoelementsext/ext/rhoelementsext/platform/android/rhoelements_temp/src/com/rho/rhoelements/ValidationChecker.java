/**
 * 
 */
package com.rho.rhoelements;

import android.app.AlertDialog;
import android.os.Build;

import com.rho.rhoelements.common.Oem;
import com.rhomobile.rhodes.extmanager.RhoExtManager;

/**
 * @author FPC843
 * This class provides the logic for the following checks:
 * - Device is T&V'ed; 
 * - Device is Symbol Solutions
 */
public class ValidationChecker
{
	/**
	 *@return true if the OEM string matches belongs to the OEM set
	 */
	public void validate()
	{
		/*
		 * if executed for unit testing don't carry out the validation check
		 * This should be secure as the compiler will not generate the code
		 * bypassing the validation checks if Oem.testmode = false
		 */
		if (Oem.testmode)
		{
			Common.elementsCore.initializeActivity();
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Running in test mode, platform validation will be bypassed!"));
			return;
		}
		
		//else
		int loops;
		for (loops = 0; loops < Oem.ValidatedProducts.length; loops++)
			if (Build.MODEL.compareToIgnoreCase(Oem.ValidatedProducts[loops]) == 0)
			{
				Common.elementsCore.initializeActivity();
				Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "Device Validation Passed."));
				return;
			}
		/*
		 * Products is not validated but it might be a Symbol device
		 */
		for (loops = 0; loops < Oem.AllowedOEM.length; loops++)
			if (Build.MANUFACTURER.compareToIgnoreCase(Oem.AllowedOEM[loops]) == 0)
				break;

		if (loops < Oem.AllowedOEM.length)
		{
			/*
			 * OEM is ok but we the device is not validated.
			 * Display a dialog to let the user choosing whether to continue or exit
			 */
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_WARNING, "Device Validation Failed but OEM is valid."));
			createValidationWarningDialog();
		}
		else
		{
			/*
			 * If the program gets here it's because the device is from a 3rd-party
			 * manufacturer. RhoElements doesn't have to run on such device. Display a dialog
			 * to the user notifying about this and with a button to confirm.
			 */
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "Device Validation Failed. Device is not from valid OEM."));
			createInvalidPlatformDialog();
		}
	}



	private void createValidationWarningDialog()
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(Common.mainActivity);
		builder.setMessage(RhoExtManager.getResourceId("string","validation_dialog_message"))
		.setIcon(RhoExtManager.getResourceId("drawable","alert_dialog_icon"))
		.setTitle(RhoExtManager.getResourceId("string","alert_dialog_title"))
		.setCancelable(true)
		.setPositiveButton(RhoExtManager.getResourceId("string","alert_dialog_ok") , Common.elementsCore)
		.setNegativeButton(RhoExtManager.getResourceId("string","alert_dialog_quit"), Common.elementsCore);
		AlertDialog alert = builder.create();
		alert.setCancelable(false);
		alert.show();
	}

	private void createInvalidPlatformDialog()
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(Common.mainActivity);
		builder.setMessage(RhoExtManager.getResourceId("string","invalidplat_dialog_message"))
		.setIcon(RhoExtManager.getResourceId("drawable","alert_dialog_icon"))
		.setTitle(RhoExtManager.getResourceId("string","alert_dialog_title"))
		.setCancelable(true)
		.setNegativeButton(RhoExtManager.getResourceId("string","alert_dialog_quit"), Common.elementsCore);
		AlertDialog alert = builder.create();
		alert.setCancelable(false);
		alert.show();
	}
}
