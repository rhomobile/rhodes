/**
 * 
 */
package com.rho.rhoelements.apd;

/**
 * @author FPC843
 * This class contains the commands constants indicating the external function to perform.
 */
public class ApdCommands
{
	public static final int Cmd_PSBuildLabel      = 0x0001;
	public static final int Cmd_PSEndLabel        = 0x0002;
	public static final int Cmd_PSEndSession      = 0x0003;
	public static final int Cmd_PSGetPorts        = 0x0004;
	public static final int Cmd_PSNewLabel        = 0x0005;
	public static final int Cmd_PSSelectPrinter   = 0x0006;
	public static final int Cmd_PSSendLabel       = 0x0007;
	public static final int Cmd_PSPrinterStat     = 0x0008;
	public static final int Cmd_PSStartSession    = 0x0009;
	public static final int Cmd_PSCatLabel        = 0x000a;
	public static final int Cmd_WakeupCOMTEC      = 0x000b;
	public static final int Cmd_PSSetParameter    = 0x000c;
	public static final int Cmd_PSPrinterReady    = 0x000d;
	public static final int Cmd_PSLink            = 0x000e;
	public static final int Cmd_PSComputeComtecID = 0x000f;
	public static final int Cmd_PSHashComtecID    = 0x0010;
	public static final int Cmd_PSConnectType     = 0x0011;
	public static final int Cmd_PSPrnType         = 0x0012;
	public static final int Cmd_PSPrnPort         = 0x0013;
	public static final int Cmd_PSSetDeliveryMode = 0x0014;

	public static final int Cmd_PSSendFormat      = 0x0100;
	public static final int Cmd_PSSendData        = 0x0101;
	public static final int Cmd_PSDisplaySetup    = 0x0102;
	public static final int Cmd_PSSetupPrinter    = 0x0103;
	public static final int Cmd_PSGetVersion      = 0x0104;
	public static final int Cmd_PSSetPrinter      = 0x0105;
	public static final int Cmd_PSGetPrinterList  = 0x0106;
	public static final int Cmd_PSGetLastMessage  = 0x0107;
	public static final int Cmd_PSPrintIdLabel    = 0x0108;
	public static final int Cmd_PSGetResponse     = 0x0109;
	public static final int	Cmd_PSSendString	  = 0x010a;
	public static final int Cmd_PSSetTemplatePath = 0x010b;
	public static final int Cmd_PSSetFormatPath	  = 0x010c;
	public static final int Cmd_PSSetIDLabelPath  = 0x010d;
	public static final int Cmd_PSOpenPort        = 0x010e;
	public static final int Cmd_PSClosePort       = 0x010f;
	public static final int Cmd_PSPowerOn         = 0x0110;
	public static final int Cmd_PSPowerOff        = 0x0111;
	public static final int Cmd_PSHideProgress    = 0x0112;
	public static final int Cmd_PSShowProgress    = 0x0113;
	public static final int Cmd_PSBthPowerOn      = 0x0114;
	public static final int Cmd_PSBthPowerOff     = 0x0115;

	// Alternative definitions
	public static final int Cmd_PSetTemplatePath  = 0x010b;
	public static final int Cmd_PSetFormatPath    = 0x010c;
	public static final int Cmd_PSetIDLabelPath   = 0x010d;
}
