package com.rho.barcode.emdk3;

import android.os.Environment;
import com.symbol.emdk.barcode.ScannerConfig;

public class EMDK3ScannerSettings
{
	public static class Decoder
	{
		public static class AustralianPostal extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.australianPostal.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.australianPostal.enabled;
			}
		}

		public static class Aztec extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.aztec.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.aztec.enabled;
			}
		}

		public static class CanadianPostal extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.canadianPostal.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.canadianPostal.enabled;
			}
		}

		public static class Chinese2of5 extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.chinese2of5.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.chinese2of5.enabled;
			}
		}

		public static class CodaBar extends EnabledSetting
		{
			public static class ClsiEditing extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.codaBar.clsiEditing = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.codaBar.clsiEditing;
				}
			}
			
			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;

				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.codaBar.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.codaBar.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;

				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.codaBar.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.codaBar.length2;
				}
			}
			
			public static class NotisEditing extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.codaBar.notisEditing = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.codaBar.notisEditing;
				}
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.codaBar.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.codaBar.redundancy;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.codaBar.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.codaBar.enabled;
			}
		}

		public static class Code11 extends EnabledSetting
		{
			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code11.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code11.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code11.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code11.length2;
				}
				
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code11.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code11.redundancy;
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code11.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code11.reportCheckDigit;
				}
			}
			
			public static class VerifyCheckDigit extends EnumSetting
			{
				private final static String NONE = "none";
				private final static String ONE = "one";
				private final static String TWO = "two";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NONE))
					{
						config.decoderParams.code11.verifyCheckDigit = ScannerConfig.VerifyCheckDigit.NO;
						return true;
					}
					else if(value.equalsIgnoreCase(ONE))
					{
						config.decoderParams.code11.verifyCheckDigit = ScannerConfig.VerifyCheckDigit.ONE;
						return true;
					}
					else if(value.equalsIgnoreCase(TWO))
					{
						config.decoderParams.code11.verifyCheckDigit = ScannerConfig.VerifyCheckDigit.TWO;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.code11.verifyCheckDigit)
					{
						case NO:	return NONE;
						case ONE:	return ONE;
						case TWO:	return TWO;
						default:	return null;
					}
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.code11.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.code11.enabled;
			}
		}

		public static class Code128 extends EnabledSetting
		{
			public static class CheckIsbtTable extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code128.checkIsbtTable = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code128.checkIsbtTable;
				}
			}

			public static class EnableEan128 extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code128.enableEan128 = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code128.enableEan128;
				}
			}

			public static class EnableIsbt128 extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code128.enableIsbt128 = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code128.enableIsbt128;
				}
			}
			
			public static class EnablePlain extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code128.enablePlain = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code128.enablePlain;
				}
			}

			public static class Isbt128ConcatMode extends EnumSetting
			{
				private final static String NEVER = "never";
				private final static String ALWAYS = "always";
				private final static String AUTO = "auto";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NEVER))
					{
						config.decoderParams.code128.isbt128ConcatMode = ScannerConfig.Isbt128ContactMode.NEVER;
						return true;
					}
					else if (value.equalsIgnoreCase(ALWAYS))
					{
						config.decoderParams.code128.isbt128ConcatMode = ScannerConfig.Isbt128ContactMode.ALWAYS;
						return true;
					}
					else if (value.equalsIgnoreCase(AUTO))
					{
						config.decoderParams.code128.isbt128ConcatMode = ScannerConfig.Isbt128ContactMode.AUTO;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.code128.isbt128ConcatMode)
					{
						case NEVER:		return NEVER;
						case ALWAYS:	return ALWAYS;
						case AUTO:		return AUTO;
						default: 	return null;
					}
				}
			}

			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code128.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code128.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				private final static int MIN = 0;
				private final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code128.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code128.length2;
				}
				
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code128.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code128.redundancy;
				}
			}
			
			public static class SecurityLevel extends EnumSetting
			{
				private final static String ZERO = "0";
				private final static String ONE = "1";
				private final static String TWO = "2";
				private final static String THREE = "3";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equals(ZERO))
					{
						config.decoderParams.code128.securityLevel = ScannerConfig.SecurityLevel.LEVEL_0;
						return true;
					}
					else if (value.equals(ONE))
					{
						config.decoderParams.code128.securityLevel = ScannerConfig.SecurityLevel.LEVEL_1;
						return true;
					}
					else if (value.equals(TWO))
					{
						config.decoderParams.code128.securityLevel = ScannerConfig.SecurityLevel.LEVEL_2;
						return true;
					}
					else if (value.equals(THREE))
					{
						config.decoderParams.code128.securityLevel = ScannerConfig.SecurityLevel.LEVEL_3;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.code128.securityLevel)
					{
						case LEVEL_0: 	return ZERO;
						case LEVEL_1: 	return ONE;
						case LEVEL_2:	return TWO;
						case LEVEL_3: 	return THREE;
						default:		return null;
					}
				}
			}
			
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.code128.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.code128.enabled;
			}
		}

		public static class Code39 extends EnabledSetting
		{
			public static class ConvertToCode32 extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code39.convertToCode32 = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code39.convertToCode32;
				}
			}
			
			public static class FullAscii extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code39.fullAscii = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code39.fullAscii;
				}
			}

			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code39.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code39.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code39.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code39.length2;
				}				
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code39.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code39.redundancy;
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code39.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code39.reportCheckDigit;
				}
			}
			
			public static class ReportCode32Prefix extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code39.reportCode32Prefix = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code39.reportCode32Prefix;
				}
			}
			
			public static class SecurityLevel extends EnumSetting
			{
				private final static String ZERO = "0";
				private final static String ONE = "1";
				private final static String TWO = "2";
				private final static String THREE = "3";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equals(ZERO))
					{
						config.decoderParams.code39.securityLevel = ScannerConfig.SecurityLevel.LEVEL_0;
						return true;
					}
					else if(value.equals(ONE))
					{
						config.decoderParams.code39.securityLevel = ScannerConfig.SecurityLevel.LEVEL_1;
						return true;
					}
					else if(value.equals(TWO))
					{
						config.decoderParams.code39.securityLevel = ScannerConfig.SecurityLevel.LEVEL_2;
						return true;
					}
					else if(value.equals(THREE))
					{
						config.decoderParams.code39.securityLevel = ScannerConfig.SecurityLevel.LEVEL_3;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.code39.securityLevel)
					{
						case LEVEL_0: 	return ZERO;
						case LEVEL_1: 	return ONE;
						case LEVEL_2:	return TWO;
						case LEVEL_3: 	return THREE;
						default:		return null;
					}
				}
			}
			
			public static class VerifyCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code39.verifyCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code39.verifyCheckDigit;
				}
			}
			
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.code39.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.code39.enabled;
			}
		}

		public static class Code93 extends EnabledSetting
		{
			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code93.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code93.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.code93.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.code93.length2;
				}
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.code93.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.code93.redundancy;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.code93.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.code93.enabled;
			}
		}

		public static class CompositeAB extends EnabledSetting
		{
			public static class UccLinkMode extends EnumSetting
			{
				private final static String NEVER = "never";
				private final static String ALWAYS = "always";
				private final static String AUTO = "auto";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NEVER))
					{
						config.decoderParams.compositeAB.uccLinkMode = ScannerConfig.UccLinkMode.LINK_FLAG_IGNORED;
						return true;
					}
					else if (value.equalsIgnoreCase(ALWAYS))
					{
						config.decoderParams.compositeAB.uccLinkMode = ScannerConfig.UccLinkMode.ALWAYS_LINKED;
						return true;
					}
					else if (value.equalsIgnoreCase(AUTO))
					{
						config.decoderParams.compositeAB.uccLinkMode = ScannerConfig.UccLinkMode.AUTO_DISCRIMINATE;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.compositeAB.uccLinkMode)
					{
						case LINK_FLAG_IGNORED:	return NEVER;
						case ALWAYS_LINKED:		return ALWAYS;
						case AUTO_DISCRIMINATE:	return AUTO;
						default: 				return null;
					}
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.compositeAB.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.compositeAB.enabled;
			}
		}

		public static class CompositeC extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.compositeC.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.compositeC.enabled;
			}
		}

		public static class D2of5 extends EnabledSetting
		{
			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.d2of5.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.d2of5.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.d2of5.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.d2of5.length2;
				}
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.d2of5.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.d2of5.redundancy;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.d2of5.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.d2of5.enabled;
			}
		}

		public static class DataMatrix extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.dataMatrix.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.dataMatrix.enabled;
			}
		}

		public static class DutchPostal extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.dutchPostal.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.dutchPostal.enabled;
			}
		}

		public static class Ean13 extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.ean13.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.ean13.enabled;
			}
		}

		public static class Ean8 extends EnabledSetting
		{
			public static class ConvertToEan13 extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.ean8.convertToEan13 = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.ean8.convertToEan13;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.ean8.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.ean8.enabled;
			}
		}

		public static class Gs1Databar extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.gs1Databar.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.gs1Databar.enabled;
			}			
		}

		public static class Gs1DatabarExp extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.gs1DatabarExp.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.gs1DatabarExp.enabled;
			}
		}

		public static class Gs1DatabarLim extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.gs1DatabarLim.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.gs1DatabarLim.enabled;
			}
		}

		public static class I2of5 extends EnabledSetting
		{
			public static class ConvertToEan13 extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.i2of5.convertToEan13 = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.i2of5.convertToEan13;
				}
			}
			
			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.i2of5.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.i2of5.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.i2of5.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.i2of5.length2;
				}
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.i2of5.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.i2of5.redundancy;
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.i2of5.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.i2of5.reportCheckDigit;
				}
			}
			
			public static class SecurityLevel extends EnumSetting
			{
				private final static String ZERO = "0";
				private final static String ONE = "1";
				private final static String TWO = "2";
				private final static String THREE = "3";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equals(ZERO))
					{
						config.decoderParams.i2of5.securityLevel = ScannerConfig.SecurityLevel.LEVEL_0;
						return true;
					}
					else if (value.equals(ONE))
					{
						config.decoderParams.i2of5.securityLevel = ScannerConfig.SecurityLevel.LEVEL_1;
						return true;
					}
					else if (value.equals(TWO))
					{
						config.decoderParams.i2of5.securityLevel = ScannerConfig.SecurityLevel.LEVEL_2;
						return true;
					}
					else if (value.equals(THREE))
					{
						config.decoderParams.i2of5.securityLevel = ScannerConfig.SecurityLevel.LEVEL_3;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.i2of5.securityLevel)
					{
						case LEVEL_0: 	return ZERO;
						case LEVEL_1: 	return ONE;
						case LEVEL_2:	return TWO;
						case LEVEL_3: 	return THREE;
						default:		return null;
					}
				}
			}
			
			public static class VerifyCheckDigit extends EnumSetting
			{
				private final static String NONE = "none";
				private final static String OPCC = "opcc";
				private final static String USS = "uss";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NONE))
					{
						config.decoderParams.i2of5.verifyCheckDigit = ScannerConfig.CheckDigitType.NO;
						return true;
					}
					else if(value.equalsIgnoreCase(OPCC))
					{
						config.decoderParams.i2of5.verifyCheckDigit = ScannerConfig.CheckDigitType.OPCC;
						return true;
					}
					else if(value.equalsIgnoreCase(USS))
					{
						config.decoderParams.i2of5.verifyCheckDigit = ScannerConfig.CheckDigitType.USS;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.i2of5.verifyCheckDigit)
					{
						case NO:	return NONE;
						case OPCC:	return OPCC;
						case USS:	return USS;
						default:	return null;
					}
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.i2of5.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.i2of5.enabled;
			}
		}

		public static class JapanesePostal extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.japanesePostal.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.japanesePostal.enabled;
			}
		}

		public static class Korean3of5 extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.korean3of5.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.korean3of5.enabled;
			}
		}

		public static class Matrix2of5 extends EnabledSetting
		{
			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.matrix2of5.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.matrix2of5.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.matrix2of5.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.matrix2of5.length2;
				}
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.matrix2of5.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.matrix2of5.redundancy;
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.matrix2of5.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.matrix2of5.reportCheckDigit;
				}
			}
			
			public static class VerifyCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.matrix2of5.verifyCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.matrix2of5.verifyCheckDigit;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.matrix2of5.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.matrix2of5.enabled;
			}
		}

		public static class MaxiCode extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.maxiCode.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.maxiCode.enabled;
			}
		}

		public static class MicroPdf extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.microPDF.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.microPDF.enabled;
			}
		}

		public static class MicroQr extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.microQR.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.microQR.enabled;
			}
		}

		public static class Msi extends EnabledSetting
		{
			public static class CheckDigits extends EnumSetting
			{
				private static final String ONE = "one";
				private static final String TWO = "two";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(ONE))
					{
						config.decoderParams.msi.checkDigits = ScannerConfig.CheckDigit.ONE;
						return true;
					}
					if(value.equalsIgnoreCase(TWO))
					{
						config.decoderParams.msi.checkDigits = ScannerConfig.CheckDigit.TWO;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.msi.checkDigits)
					{
						case ONE: 	return ONE;
						case TWO: 	return TWO;
						default:	return null;
					}
				}
			}
			
			public static class CheckDigitScheme extends EnumSetting
			{
				private static final String MOD10 = "mod10";
				private static final String MOD11 = "mod11";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(MOD10))
					{
						config.decoderParams.msi.checkDigitScheme = ScannerConfig.CheckDigitScheme.MOD_10_10;
						return true;
					}
					if(value.equalsIgnoreCase(MOD11))
					{
						config.decoderParams.msi.checkDigitScheme = ScannerConfig.CheckDigitScheme.MOD_11_10;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.msi.checkDigitScheme)
					{
						case MOD_10_10:	return MOD10;
						case MOD_11_10:	return MOD11;
						default:		return null;
					}
				}
			}
			
			public static class Length1 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.msi.length1 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.msi.length1;
				}
			}
			
			public static class Length2 extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 55;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.msi.length2 = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.msi.length2;
				}
			}
			
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.msi.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.msi.redundancy;
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.msi.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.msi.reportCheckDigit;
				}
			}
			
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.msi.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.msi.enabled;
			}
		}

		public static class Pdf417 extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.pdf417.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.pdf417.enabled;
			}
		}

		public static class QrCode extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.qrCode.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.qrCode.enabled;
			}
		}

		public static class Signature extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.signature.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.signature.enabled;
			}
		}

		public static class Tlc39 extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.tlc39.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.tlc39.enabled;
			}
		}

		public static class TriOptic39 extends EnabledSetting
		{
			public static class Redundancy extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.triOptic39.redundancy = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.triOptic39.redundancy;
				}
			}
			
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.triOptic39.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.triOptic39.enabled;
			}
		}

		public static class UkPostal extends EnabledSetting
		{
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.ukPostal.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.ukPostal.reportCheckDigit;
				}
			}
			
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.ukPostal.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.ukPostal.enabled;
			}
		}

		public static class Upca extends EnabledSetting
		{
			public static class Preamble extends EnumSetting
			{
				private static final String NONE = "none";
				private static final String SYSTEM = "systemChar";
				private static final String COUNTRY = "countryAndSystemChars";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NONE))
					{
						config.decoderParams.upca.preamble = ScannerConfig.Preamble.NONE;
						return true;
					}
					else if(value.equalsIgnoreCase(SYSTEM))
					{
						config.decoderParams.upca.preamble = ScannerConfig.Preamble.SYS_CHAR;
						return true;
					}
					else if(value.equalsIgnoreCase(COUNTRY))
					{
						config.decoderParams.upca.preamble = ScannerConfig.Preamble.COUNTRY_AND_SYS_CHAR;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.upca.preamble)
					{
						case NONE:					return NONE;
						case SYS_CHAR:				return SYSTEM;
						case COUNTRY_AND_SYS_CHAR:	return COUNTRY;
						default:					return null;
					}
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upca.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upca.reportCheckDigit;
				}
			}
			
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.upca.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.upca.enabled;
			}
		}

		public static class Upce0 extends EnabledSetting
		{
			public static class ConvertToUpca extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upce0.convertToUpca = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upce0.convertToUpca;
				}
			}
			
			public static class Preamble extends EnumSetting
			{
				private static final String NONE = "none";
				private static final String SYSTEM = "systemChar";
				private static final String COUNTRY = "countryAndSystemChars";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NONE))
					{
						config.decoderParams.upce0.preamble = ScannerConfig.Preamble.NONE;
						return true;
					}
					else if(value.equalsIgnoreCase(SYSTEM))
					{
						config.decoderParams.upce0.preamble = ScannerConfig.Preamble.SYS_CHAR;
						return true;
					}
					else if(value.equalsIgnoreCase(COUNTRY))
					{
						config.decoderParams.upce0.preamble = ScannerConfig.Preamble.COUNTRY_AND_SYS_CHAR;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.upce0.preamble)
					{
						case NONE:					return NONE;
						case SYS_CHAR:				return SYSTEM;
						case COUNTRY_AND_SYS_CHAR:	return COUNTRY;
						default:					return null;
					}
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upce0.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upce0.reportCheckDigit;
				}
			}
			
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.upce0.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.upce0.enabled;
			}
		}

		public static class Upce1 extends EnabledSetting
		{
			public static class ConvertToUpca extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upce1.convertToUpca = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upce1.convertToUpca;
				}
			}
			
			public static class Preamble extends EnumSetting
			{
				private static final String NONE = "none";
				private static final String SYSTEM = "systemChar";
				private static final String COUNTRY = "countryAndSystemChars";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NONE))
					{
						config.decoderParams.upce1.preamble = ScannerConfig.Preamble.NONE;
						return true;
					}
					else if(value.equalsIgnoreCase(SYSTEM))
					{
						config.decoderParams.upce1.preamble = ScannerConfig.Preamble.SYS_CHAR;
						return true;
					}
					else if(value.equalsIgnoreCase(COUNTRY))
					{
						config.decoderParams.upce1.preamble = ScannerConfig.Preamble.COUNTRY_AND_SYS_CHAR;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.upce1.preamble)
					{
						case NONE:					return NONE;
						case SYS_CHAR:				return SYSTEM;
						case COUNTRY_AND_SYS_CHAR:	return COUNTRY;
						default:					return null;
					}
				}
			}
			
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upce1.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upce1.reportCheckDigit;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.upce1.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.upce1.enabled;
			}
		}

		public static class UpcEanParams
		{
			public static class BooklandCode extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.booklandCode = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.booklandCode;
				}
			}

			public static class BooklandFormat extends EnumSetting
			{
				private final static String ISBN10 = "isbn10";
				private final static String ISBN13 = "isbn13";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(ISBN10))
					{
						config.decoderParams.upcEanParams.booklandFormat = ScannerConfig.BooklandFormat.ISBN_10;
						return true;
					}
					if(value.equalsIgnoreCase(ISBN13))
					{
						config.decoderParams.upcEanParams.booklandFormat = ScannerConfig.BooklandFormat.ISBN_13;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.upcEanParams.booklandFormat)
					{
						case ISBN_10: 	return ISBN10;
						case ISBN_13:	return ISBN13;
						default:		return null;
					}
				}
			}

			public static class ConvertDataBarToUpcEan extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.convertDataBarToUpcEan = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.convertDataBarToUpcEan;
				}
			}

			public static class CouponCode extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.couponCode = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.couponCode;
				}
			}

			public static class CouponReport extends EnumSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					// TODO Auto-generated method stub
					return false;
				}

				public static String get(ScannerConfig config)
				{
					// TODO Auto-generated method stub
					return null;
				}
			}

			public static class EanZeroExtend extends BooleanSetting //TODO
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.eanZeroExtend = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.eanZeroExtend;
				}
			}

			public static class LinearDecode extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.linearDecode = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.linearDecode;
				}
			}

			public static class RandomWeightCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.randomWeightCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.randomWeightCheckDigit;
				}
			}

			public static class SecurityLevel extends EnumSetting
			{
				private final static String ZERO = "0";
				private final static String ONE = "1";
				private final static String TWO = "2";
				private final static String THREE = "3";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equals(ZERO))
					{
						config.decoderParams.upcEanParams.securityLevel = ScannerConfig.SecurityLevel.LEVEL_0;
						return true;
					}
					else if (value.equals(ONE))
					{
						config.decoderParams.upcEanParams.securityLevel = ScannerConfig.SecurityLevel.LEVEL_1;
						return true;
					}
					else if (value.equals(TWO))
					{
						config.decoderParams.upcEanParams.securityLevel = ScannerConfig.SecurityLevel.LEVEL_2;
						return true;
					}
					else if (value.equals(THREE))
					{
						config.decoderParams.upcEanParams.securityLevel = ScannerConfig.SecurityLevel.LEVEL_3;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.upcEanParams.securityLevel)
					{
						case LEVEL_0: 	return ZERO;
						case LEVEL_1: 	return ONE;
						case LEVEL_2:	return TWO;
						case LEVEL_3: 	return THREE;
						default:		return null;
					}
				}
			}

			public static class Supplemental2 extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.supplemental2 = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.supplemental2;
				}
			}

			public static class Supplemental5 extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.upcEanParams.supplemental5 = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.supplemental5;
				}
			}

			public static class SupplementalMode extends EnumSetting
			{
				private final static String NONE 	= "none";
				private final static String ALWAYS	= "always";
				private final static String AUTO 	= "auto";
				private final static String SMART	= "smart";
				private final static String S378 	= "378or379";
				private final static String S414 	= "414or419or434or439";
				private final static String S977 	= "977";
				private final static String S978 	= "978or979";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(NONE))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.NO;
						return true;
					}
					if(value.equalsIgnoreCase(ALWAYS))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.ALWAYS;
						return true;
					}
					if(value.equalsIgnoreCase(AUTO))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.AUTO;
						return true;
					}
					if(value.equalsIgnoreCase(SMART))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.SMART;
						return true;
					}
					if(value.equalsIgnoreCase(S378))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.S_378_379;
						return true;
					}
					if(value.equalsIgnoreCase(S414))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.S_414_419_434_439;
						return true;
					}
					if(value.equalsIgnoreCase(S977))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.S_977;
						return true;
					}
					if(value.equalsIgnoreCase(S978))
					{
						config.decoderParams.upcEanParams.supplementalMode = ScannerConfig.SupplementalMode.S_978_979;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.decoderParams.upcEanParams.supplementalMode)
					{
						case NO: return NONE;
						case ALWAYS: return ALWAYS;
						case AUTO: return AUTO;
						case SMART: return SMART;
						case S_378_379: return S378;
						case S_414_419_434_439: return S414;
						case S_977: return S977;
						case S_978_979: return S978;
						default: return null;
					}
				}
			}

			public static class SupplementalRetries extends IntSetting
			{
				final static int MIN = 2;
				final static int MAX = 20;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{
					config.decoderParams.upcEanParams.supplementalRetries = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.decoderParams.upcEanParams.supplementalRetries;
				}
			}
		}

		public static class Us4State extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.us4State.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.us4State.enabled;
			}
		}

		public static class Us4StateFics extends EnabledSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.us4StateFics.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.us4StateFics.enabled;
			}
		}

		public static class UsPlanet extends EnabledSetting
		{
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.usPlanet.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.usPlanet.reportCheckDigit;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.usPlanet.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.usPlanet.enabled;
			}
		}

		public static class UsPostNet extends EnabledSetting
		{
			public static class ReportCheckDigit extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.usPostNet.reportCheckDigit = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.usPostNet.reportCheckDigit;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.usPostNet.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.usPostNet.enabled;
			}
		}

		public static class WebCode extends EnabledSetting
		{
			public static class SubType extends BooleanSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						config.decoderParams.webCode.subType = parsedValue;
						return true;
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.decoderParams.webCode.subType;
				}
			}

			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.decoderParams.webCode.enabled = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.decoderParams.webCode.enabled;
			}
		}
	}
	
	public static class Reader
	{
		public static class Camera
		{
			public static class BeamTimer extends IntSetting
			{
				final static int MIN = 0;
				final static int MAX = 60000;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{	
					config.readerParams.readerSpecific.cameraSpecific.beamTimer = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.readerParams.readerSpecific.cameraSpecific.beamTimer;
				}
			}

			public static class IlluminationMode extends EnumSetting
			{
				private final static String OFF = "alwaysOff";
				private final static String ON = "alwaysOn";

				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(OFF))
					{
						config.readerParams.readerSpecific.cameraSpecific.illuminationMode = ScannerConfig.IlluminationMode.OFF;
						return true;
					}
					if(value.equalsIgnoreCase(ON))
					{
						config.readerParams.readerSpecific.cameraSpecific.illuminationMode = ScannerConfig.IlluminationMode.ON;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.cameraSpecific.illuminationMode)
					{
						case OFF: return OFF;
						case ON: return ON;
						default: return null;
					}
				}
			}
			
			public static class Inverse1DMode extends EnumSetting
			{
				private final static String DISABLED = "disabled";
				private final static String ENABLED = "enabled";
				private final static String AUTO = "auto";

				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(DISABLED))
					{
						config.readerParams.readerSpecific.cameraSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.DISABLED;
						return true;
					}
					if(value.equalsIgnoreCase(ENABLED))
					{
						config.readerParams.readerSpecific.cameraSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.ENABLED;
						return true;
					}
					if(value.equalsIgnoreCase(AUTO))
					{
						config.readerParams.readerSpecific.cameraSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.AUTO;	
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.cameraSpecific.inverse1DMode)
					{
						case AUTO:		return AUTO;
						case DISABLED:	return DISABLED;
						case ENABLED: 	return ENABLED;
						default: 		return null;
					}
				}
			}
			//TODO for all, correct case and change to equalsIgnoreCase
			public static class LinearSecurityLevel extends EnumSetting
			{
				//private final static String	REDUND	= "redundancyandlength";
				private final static String	SHORT	= "shortOrCodabar";
				private final static String	LONG	= "longAndShort";
				private final static String	TWICE	= "allTwice";
				private final static String	THRICE	= "allThrice";
						
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(SHORT))
					{
						config.readerParams.readerSpecific.cameraSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.SHORT_OR_CODABAR;
						return true;
					}
					if(value.equalsIgnoreCase(LONG))
					{
						config.readerParams.readerSpecific.cameraSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.LONG_AND_SHORT;
						return true;
					}
					if(value.equalsIgnoreCase(TWICE))
					{
						config.readerParams.readerSpecific.cameraSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.ALL_TWICE;	
						return true;
					}
					if(value.equalsIgnoreCase(THRICE))
					{
						config.readerParams.readerSpecific.cameraSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.ALL_THRICE;	
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.cameraSpecific.linearSecurityLevel)
					{
						case ALL_THRICE:		return THRICE;
						case ALL_TWICE:			return TWICE;
						case LONG_AND_SHORT: 	return LONG;
						case SHORT_OR_CODABAR: 	return SHORT;
						default: 				return null;
					}
				}
			}

			public static class ViewFinderMode extends EnumSetting
			{
				private final static String ENABLED = "enabled";
				private final static String STATIC = "staticReticle";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(ENABLED))
					{
						config.readerParams.readerSpecific.cameraSpecific.viewfinderMode = ScannerConfig.ViewFinderMode.ENABLED;
						return true;
					}
					if(value.equalsIgnoreCase(STATIC))
					{
						config.readerParams.readerSpecific.cameraSpecific.viewfinderMode = ScannerConfig.ViewFinderMode.STATIC_RECTICLE;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.cameraSpecific.viewfinderMode)
					{
						case ENABLED:			return ENABLED;
						case STATIC_RECTICLE:	return STATIC;
						default: 				return null;
					}
				}
			}
		}
		
		public static class Imager
		{
			public static class BeamTimer extends IntSetting //Cam, im, las
			{
				final static int MIN = 0;
				final static int MAX = 60000;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{	
					config.readerParams.readerSpecific.imagerSpecific.beamTimer = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.readerParams.readerSpecific.imagerSpecific.beamTimer;
				}
			}
			
			public static class IlluminationBrightness extends IntSetting //Im
			{
				final static int MIN = 1;
				final static int MAX = 10;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{	
					config.readerParams.readerSpecific.imagerSpecific.illuminationBrightness = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.readerParams.readerSpecific.imagerSpecific.illuminationBrightness;
				}
			}
			
			public static class Inverse1DMode extends EnumSetting
			{
				private final static String ENABLED = "enabled";
				private final static String DISABLED = "disabled";
				private final static String AUTO = "auto";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(ENABLED))
					{
						config.readerParams.readerSpecific.imagerSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.ENABLED;
						return true;
					}
					if(value.equalsIgnoreCase(DISABLED))
					{
						config.readerParams.readerSpecific.imagerSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.DISABLED;
						return true;
					}
					if(value.equalsIgnoreCase(AUTO))
					{
						config.readerParams.readerSpecific.imagerSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.AUTO;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.imagerSpecific.inverse1DMode)
					{
						case ENABLED:	return ENABLED;
						case DISABLED:	return DISABLED;
						case AUTO:		return AUTO;
						default: 		return null;
					}
				}
			}

			public static class LcdMode extends BooleanSetting //TODO is this okay, changed from Enum
			{			
				public static boolean set(ScannerConfig config, String value)
				{
					Boolean parsedValue = parse(value);
					if(parsedValue != null)
					{
						if(parsedValue)
						{
							config.readerParams.readerSpecific.imagerSpecific.lcdMode = ScannerConfig.LcdMode.ENABLED;
						}
						else
						{
							config.readerParams.readerSpecific.imagerSpecific.lcdMode = ScannerConfig.LcdMode.DISABLED;
						}
						return true;						
					}
					return false;
				}

				public static boolean get(ScannerConfig config)
				{
					return config.readerParams.readerSpecific.imagerSpecific.lcdMode == ScannerConfig.LcdMode.ENABLED;
				}
			}
			
			public static class LinearSecurityLevel extends EnumSetting
			{
				//private final static String	REDUND	= "redundancyandlength";
				private final static String	SHORT	= "shortOrCodabar";
				private final static String	LONG	= "longAndShort";
				private final static String	TWICE	= "allTwice";
				private final static String	THRICE	= "allThrice";
						
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(SHORT))
					{
						config.readerParams.readerSpecific.imagerSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.SHORT_OR_CODABAR;
						return true;
					}
					if(value.equalsIgnoreCase(LONG))
					{
						config.readerParams.readerSpecific.imagerSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.LONG_AND_SHORT;
						return true;
					}
					if(value.equalsIgnoreCase(TWICE))
					{
						config.readerParams.readerSpecific.imagerSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.ALL_TWICE;	
						return true;
					}
					if(value.equalsIgnoreCase(THRICE))
					{
						config.readerParams.readerSpecific.imagerSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.ALL_THRICE;	
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.imagerSpecific.linearSecurityLevel)
					{
						case ALL_THRICE:		return THRICE;
						case ALL_TWICE:			return TWICE;
						case LONG_AND_SHORT: 	return LONG;
						case SHORT_OR_CODABAR: 	return SHORT;
						default: 				return null;
					}
				}
			}

			public static class PickList extends EnumSetting
			{
				private final static String	OFF		= "disabled";
				private final static String	HARD	= "hardwareReticle";
				//private final static String	SOFT	= "softwareReticle";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(OFF))
					{
						config.readerParams.readerSpecific.imagerSpecific.pickList = ScannerConfig.PickList.DISABLED;
						return true;
					}
					if(value.equalsIgnoreCase(HARD))
					{
						config.readerParams.readerSpecific.imagerSpecific.pickList = ScannerConfig.PickList.ENABLED;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.imagerSpecific.pickList)
					{
						case DISABLED:	return OFF;
						case ENABLED:	return HARD;
						default: 		return null;
					}
				}
			}
		}
		
		public static class Laser
		{
			public static class BeamTimer extends IntSetting //Cam, im, las
			{
				final static int MIN = 0;
				final static int MAX = 60000;
				
				public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
				{	
					config.readerParams.readerSpecific.laserSpecific.beamTimer = parse(value, MIN, MAX);
					return true;
				}

				public static int get(ScannerConfig config)
				{
					return config.readerParams.readerSpecific.laserSpecific.beamTimer;
				}
			}

			public static class Inverse1DMode extends EnumSetting
			{
				private final static String ENABLED = "enabled";
				private final static String DISABLED = "disabled";
				private final static String AUTO = "auto";
				
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(ENABLED))
					{
						config.readerParams.readerSpecific.laserSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.ENABLED;
						return true;
					}
					if(value.equalsIgnoreCase(DISABLED))
					{
						config.readerParams.readerSpecific.laserSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.DISABLED;
						return true;
					}
					if(value.equalsIgnoreCase(AUTO))
					{
						config.readerParams.readerSpecific.laserSpecific.inverse1DMode = ScannerConfig.Inverse1DMode.AUTO;
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.laserSpecific.inverse1DMode)
					{
						case ENABLED:	return ENABLED;
						case DISABLED:	return DISABLED;
						case AUTO:		return AUTO;
						default: 		return null;
					}
				}
			}

			public static class LinearSecurityLevel extends EnumSetting
			{
				private final static String	SHORT	= "shortOrCodabar";
				private final static String	LONG	= "longAndShort";
				private final static String	TWICE	= "allTwice";
				private final static String	THRICE	= "allThrice";
						
				public static boolean set(ScannerConfig config, String value)
				{
					if(value.equalsIgnoreCase(SHORT))
					{
						config.readerParams.readerSpecific.laserSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.SHORT_OR_CODABAR;
						return true;
					}
					if(value.equalsIgnoreCase(LONG))
					{
						config.readerParams.readerSpecific.laserSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.LONG_AND_SHORT;
						return true;
					}
					if(value.equalsIgnoreCase(TWICE))
					{
						config.readerParams.readerSpecific.laserSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.ALL_TWICE;	
						return true;
					}
					if(value.equalsIgnoreCase(THRICE))
					{
						config.readerParams.readerSpecific.laserSpecific.linearSecurityLevel = ScannerConfig.LinearSecurityLevel.ALL_THRICE;	
						return true;
					}
					return false;
				}

				public static String get(ScannerConfig config)
				{
					switch(config.readerParams.readerSpecific.imagerSpecific.linearSecurityLevel)
					{
						case ALL_THRICE:		return THRICE;
						case ALL_TWICE:			return TWICE;
						case LONG_AND_SHORT: 	return LONG;
						case SHORT_OR_CODABAR: 	return SHORT;
						default: 				return null;
					}
				}
			}

			//TODO unknown setting...
			public static class PowerMode extends EnumSetting
			{
				public static boolean set(ScannerConfig config, String value)
				{
					// TODO Auto-generated method stub
					//config.readerParams.readerSpecific.laserSpecific.powerMode 
					return false;
				}

				public static String get(ScannerConfig config)
				{
					// TODO Auto-generated method stub
					return null;
				}				
			}
		}
	}
	
	public static class Scan
	{
		public static class AudioStreamType extends EnumSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				//TODO new parameter
//				/config.scanParams.audioStreamType = // TODO Auto-generated method stub
				return false;
			}

			public static String get(ScannerConfig config)
			{
				// TODO Auto-generated method stub
				return null;
			}
		}

		public static class CodeIdType extends EnumSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				//TODO unknown parameter
				// TODO Auto-generated method stub
				return false;
			}

			public static String get(ScannerConfig config)
			{
				// TODO Auto-generated method stub
				return null;
			}
		}

		public static class DecodeAudioFeedbackUri
		{
			public static boolean set(ScannerConfig config, String value)
			{
				String[] values = value.split("file:[/]*", 1);
				if(values.length == 1) value = values[0];
				else
				{
					if(values[0].endsWith("/")) value = "/" + values[1];
					else value = values[1];
				}
				value = value.substring(5);
				if(!value.equalsIgnoreCase(Environment.getExternalStorageDirectory().getAbsolutePath() + "/"+"decode.wav"))
				config.scanParams.decodeAudioFeedbackUri = value;
				return true;
			}

			public static String get(ScannerConfig config)
			{
				return config.scanParams.decodeAudioFeedbackUri;
			}
		}

		public static class DecodeHapticFeedback extends BooleanSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.scanParams.decodeHapticFeedback = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.scanParams.decodeHapticFeedback;
			}
		}

		public static class DecodeLEDFeedback extends BooleanSetting
		{
			public static boolean set(ScannerConfig config, String value)
			{
				Boolean parsedValue = parse(value);
				if(parsedValue != null)
				{
					config.scanParams.decodeLEDFeedback = parsedValue;
					return true;
				}
				return false;
			}

			public static boolean get(ScannerConfig config)
			{
				return config.scanParams.decodeLEDFeedback;
			}
		}

		public static class DecodeLEDTime extends IntSetting
		{
			final static int MIN = 0;
			final static int MAX = 1000;

			public static boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException
			{
				config.scanParams.decodeLEDTime = parse(value, MIN, MAX);
				return true;
			}

			public static int get(ScannerConfig config)
			{
				return config.scanParams.decodeLEDTime;
			}
		}
	}
	
	public static abstract class IntSetting
	{
		public static int parse(String value, int min, int max) throws NumberFormatException, IllegalArgumentException
		{
			int parsedValue = Integer.parseInt(value);
			if(parsedValue < min || parsedValue > max)
			{
				throw new IllegalArgumentException("Value out of range, must be between " + min + " and " + max);
			}
			return parsedValue;
		}
//		public abstract boolean set(ScannerConfig config, String value) throws NumberFormatException, IllegalArgumentException;
//		public abstract int get(ScannerConfig config);
	}
	
	public static abstract class BooleanSetting
	{
		public static boolean parse(String value)
		{
			return Boolean.parseBoolean(value);
		}
//		public abstract boolean set(ScannerConfig config, String value);
//		public abstract boolean get(ScannerConfig config);
	}
	
	public static abstract class EnabledSetting
	{
		public static Boolean parse(String value)
		{
			if(value.equalsIgnoreCase("enabled") || value.equalsIgnoreCase("true"))
			{
				return true;
			}
			if(value.equalsIgnoreCase("disabled") || value.equalsIgnoreCase("false"))
			{
				return false;
			}
			return null;
		}
		
//		public abstract boolean set(ScannerConfig config, String value);
//		public abstract boolean get(ScannerConfig config);
	}
	
	public static abstract class EnumSetting
	{
//		public abstract boolean set(ScannerConfig config, String value);
//		public abstract String get(ScannerConfig config);
	}
	
		
	/**
	 Gets Alldecodes value set in EMDK3Scanner and returns true or false
	 */
	
	public static abstract class AllDecoders
	{
		public static class AllDecodersDefault extends EnabledSetting
		{
			public static boolean get(String value)
			{
				
				//Boolean parsedValue = parse(value);
				if(value != null)
				{
					//config.decoderParams.australianPostal.enabled = parsedValue;
					return Boolean.valueOf(value);
				}
				return true;
			}
		}
	}
}
