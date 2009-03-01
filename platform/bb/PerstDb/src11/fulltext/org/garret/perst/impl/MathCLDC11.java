package org.garret.perst.impl;

public class MathCLDC11
{
    /** ln(0.5) constant */
    final static public double LOGdiv2 = -0.6931471805599453094;
    
    static private double _log(double x) {
        double f = 0.0;
        int appendix=0;
        while (x > 0.0 && x <= 1.0)  {
            x*=2.0;
            appendix++;
        }
        x /= 2.0;
        appendix--;
        
        double y1 = x-1.;
        double y2 = x+1.;
        double y = y1/y2;
        
        double k = y;
        y2 = k*y;
        
        for (long i = 1; i < 50; i += 2)  {
            f += k/i;
            k *= y2;
        }
        
        f*=2.0;
        for(int i = 0; i < appendix; i++) {
            f += LOGdiv2;
        }
        return f;
    }
    
    static public double log(double x)
    {
        return (x <= 0.0) 
            ? Double.NaN
            : (x==1.0) 
              ? 0.0
              : (x > 1.0) 
                ? -_log(1.0/x) 
                : _log(x);
    }
}
