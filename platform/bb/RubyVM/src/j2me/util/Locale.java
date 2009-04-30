package j2me.util;

public class Locale  {
	
    static public final Locale ENGLISH = new Locale("en","","");
    private final String language;
    private final String country;
    private final String variant;
    
    public Locale(String language, String country, String variant) {
        this.language = convertOldISOCodes(language);
        this.country = toUpperCase(country).intern();
        this.variant = variant.intern();
    }

    public Locale(String language, String country) {
        this(language, country, "");
    }
    
    public Locale(String language) {
        this(language, "", "");
    }
    
    public static Locale getDefault() {
    	return  ENGLISH;
    }
    
    public String getDisplayLanguage(Locale inLocale) {
    	return "English";
    }
    
    private String toLowerCase(String str) {
    	char[] buf = new char[str.length()];
            for (int i = 0; i < buf.length; i++) {
    	    buf[i] = Character.toLowerCase(str.charAt(i));
            }
            return new String( buf );
        }
    
    private String toUpperCase(String str) {
    	char[] buf = new char[str.length()];
            for (int i = 0; i < buf.length; i++) {
    	    buf[i] = Character.toUpperCase(str.charAt(i));
            }
            return new String( buf );
        }
    
    private String convertOldISOCodes(String language) {
        // we accept both the old and the new ISO codes for the languages whose ISO
        // codes have changed, but we always store the OLD code, for backward compatibility
        language = toLowerCase(language).intern();
        if (language == "he") {
            return "iw";
        } else if (language == "yi") {
            return "ji";
        } else if (language == "id") {
            return "in";
        } else {
            return language;
        }
    }
    
}
