package j2me.lang;

import javolution.lang.MathLib;

public class MathEx {

    public static double pow(double a, double b) {
    	return MathLib.pow(a,b);
    }
    
    public static long round(double a) {
    	return (long)Math.floor(a + 0.5d);
    }
    
    public static double log(double a) {
    	return MathLib.log(a);
    }
    public static double cbrt(double x) {
        if (x < 0)
            return -pow(-x, 1/3.0);
        else
            return pow(x, 1/3.0);
    }

    public static double log10(double a) {
    	return MathLib.log10(a);
    }
    public static double log2(double a) {
        return log10(a)/log10(2.0);
    }
    
    public static double exp(double a) {
    	return MathLib.exp(a);
    }
    public static double cosh(double x) {
    	return MathLib.cosh(x);
    }
    public static double sinh(double x) {
    	return MathLib.sinh(x);
    }
    public static double tanh(double x) {
    	return MathLib.tanh(x);
    }
    public static double acos(double a) {
    	return MathLib.acos(a);
    }
    public static double asin(double a) {
    	return MathLib.asin(a);
    }
    public static double atan(double a) {
    	return MathLib.atan(a);
    }
    public static double atan2(double y, double x) {
    	return MathLib.atan2(y,x);
    }
    public static double hypot(double a, double b) 
    {
    	double result;
        if (Math.abs(a) > Math.abs(b)) {
            result = b / a;
            result = Math.abs(a) * Math.sqrt(1 + result * result);
        } else if (b != 0) {
            result = a / b;
            result = Math.abs(b) * Math.sqrt(1 + result * result);
        } else {
            result = 0;
        }
        
        return result;
    }
    
    public static int min(int a, int b) {
    	return (a <= b) ? a : b;
    }
    
    public static int max(int a, int b) {
    	return (a >= b) ? a : b;
    }
    
    private static double sign(double x, double y) {
        double abs = ((x < 0) ? -x : x);
        return (y < 0.0) ? -abs : abs;
    }

    private static final double ERFC_COEF[] = {
        -.490461212346918080399845440334e-1,
        -.142261205103713642378247418996e0,
        .100355821875997955757546767129e-1,
        -.576876469976748476508270255092e-3,
        .274199312521960610344221607915e-4,
        -.110431755073445076041353812959e-5,
        .384887554203450369499613114982e-7,
        -.118085825338754669696317518016e-8,
        .323342158260509096464029309534e-10,
        -.799101594700454875816073747086e-12,
        .179907251139614556119672454866e-13,
        -.371863548781869263823168282095e-15,
        .710359900371425297116899083947e-17,
        -.126124551191552258324954248533e-18
   };
    
    public static double erf(double value) {
        double  result;
        double  y = Math.abs(value);

        if (y <= 1.49012e-08) {
            result = 2 * value / 1.77245385090551602729816748334;
        } else if (y <= 1) {
            result = value * (1 + chebylevSerie(2 * value * value - 1, ERFC_COEF));
        } else if (y < 6.013687357) {
            result = sign(1 - erfc(y), value);
        } else {
            result = sign(1, value);
        }
        
        return result;
    }

    private static final double ERFC2_COEF[] = {
        -.69601346602309501127391508262e-1,
        -.411013393626208934898221208467e-1,
        .391449586668962688156114370524e-2,
        -.490639565054897916128093545077e-3,
        .715747900137703638076089414183e-4,
        -.115307163413123283380823284791e-4,
        .199467059020199763505231486771e-5,
        -.364266647159922287393611843071e-6,
        .694437261000501258993127721463e-7,
        -.137122090210436601953460514121e-7,
        .278838966100713713196386034809e-8,
        -.581416472433116155186479105032e-9,
        .123892049175275318118016881795e-9,
        -.269063914530674343239042493789e-10,
        .594261435084791098244470968384e-11,
        -.133238673575811957928775442057e-11,
        .30280468061771320171736972433e-12,
        -.696664881494103258879586758895e-13,
        .162085454105392296981289322763e-13,
        -.380993446525049199987691305773e-14,
        .904048781597883114936897101298e-15,
        -.2164006195089607347809812047e-15,
        .522210223399585498460798024417e-16,
        -.126972960236455533637241552778e-16,
        .310914550427619758383622741295e-17,
        -.766376292032038552400956671481e-18,
        .190081925136274520253692973329e-18
   };

   private static final double ERFCC_COEF[] = {
        .715179310202924774503697709496e-1,
        -.265324343376067157558893386681e-1,
        .171115397792085588332699194606e-2,
        -.163751663458517884163746404749e-3,
        .198712935005520364995974806758e-4,
        -.284371241276655508750175183152e-5,
        .460616130896313036969379968464e-6,
        -.822775302587920842057766536366e-7,
        .159214187277090112989358340826e-7,
        -.329507136225284321486631665072e-8,
        .72234397604005554658126115389e-9,
        -.166485581339872959344695966886e-9,
        .401039258823766482077671768814e-10,
        -.100481621442573113272170176283e-10,
        .260827591330033380859341009439e-11,
        -.699111056040402486557697812476e-12,
        .192949233326170708624205749803e-12,
        -.547013118875433106490125085271e-13,
        .158966330976269744839084032762e-13,
        -.47268939801975548392036958429e-14,
        .14358733767849847867287399784e-14,
        -.444951056181735839417250062829e-15,
        .140481088476823343737305537466e-15,
        -.451381838776421089625963281623e-16,
        .147452154104513307787018713262e-16,
        -.489262140694577615436841552532e-17,
        .164761214141064673895301522827e-17,
        -.562681717632940809299928521323e-18,
        .194744338223207851429197867821e-18
   };
    
    public static double erfc(double value) 
    {
        double  result;
        double  y = Math.abs(value);

        if (value <= -6.013687357) {
            result = 2;
        } else if (y < 1.49012e-08) {
            result = 1 - 2 * value / 1.77245385090551602729816748334;
        } else {
            double ysq = y*y;
            if (y < 1) {
                result = 1 - value * (1 + chebylevSerie(2 * ysq - 1, ERFC_COEF));
            } else if (y <= 4.0) {
                result = MathEx.exp(-ysq)/y*(0.5+chebylevSerie((8.0 / ysq - 5.0) / 3.0, ERFC2_COEF));
                if (value < 0) result = 2.0 - result;
                if (value < 0) result = 2.0 - result;
                if (value < 0) result = 2.0 - result;
            } else {
                result = MathEx.exp(-ysq) / y * (0.5 + chebylevSerie(8.0 / ysq - 1, ERFCC_COEF));
                if (value < 0) result = 2.0 - result;
            }
        }
        
        return result;
    }
    
    private static double chebylevSerie(double x, double coef[]) {
        double  b0, b1, b2, twox;
        int i;
        b1 = 0.0;
        b0 = 0.0;
        b2 = 0.0;
        twox = 2.0 * x;
        for (i = coef.length-1; i >= 0; i--) {
            b2 = b1;
            b1 = b0;
            b0 = twox * b1 - b2 + coef[i];
        }
        return 0.5*(b0 - b2);
    }
    
}
