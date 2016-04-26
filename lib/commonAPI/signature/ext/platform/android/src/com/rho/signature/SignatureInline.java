package com.rho.signature;

import android.view.View;
import android.view.ViewGroup;
import android.widget.AbsoluteLayout;
import android.os.Handler;

import com.rho.signature.SignatureSingleton.SignatureProperties;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.extmanager.RhoExtManager;
import com.rhomobile.rhodes.util.ContextFactory;

/**
 * A control object for an inline Signature
 * @author Ben Kennedy (NCVT73)
 */
public class SignatureInline extends Signature
{
	private static final String LOGTAG = "MotoSignature";
	private SignatureView signatureView;
	private Handler handler =new Handler();
	
	/**
	 * Creates a new inline signature capture control object 
	 * @param properties the Signature Capture area properties parsed from SignatureSingleton
	 * @param result The IMethodResult from the creating call (not important for this class)
	 * @param singleton the singleton class that created the object.
	 * @author Ben Kennedy (NCVT73)
	 */
	public SignatureInline(SignatureProperties properties, IMethodResult result, SignatureSingleton singleton)
	{
		super(properties, result, singleton);
	}
	
	@Override
	public void capture(IMethodResult result)
	{
		if (signatureView != null)
		{
			String imageFormat;
			if(properties.outputFormat.equalsIgnoreCase("datauri")) imageFormat = "datauri";
			else imageFormat = properties.compressionFormat;
			ImageCapture.takeSignature(result, imageFormat, signatureView.makeBitmap(), properties.filePath, properties.penColor, properties.bgColor);
		}
	}

	@Override
	public void clear(IMethodResult result)
	{
		if (signatureView != null)
		{
			signatureView.doClear();
		}
	}

	@Override
	public void hide(IMethodResult result)
	{
		if (signatureView != null)
		{
			//...We need to clear the previous signature state..so that the signature wont appear on show
			signatureView.doClear();
			signatureView.setVisibility(View.GONE);
		}
	}

	@Override
	public void destroy()
	{
		if (signatureView != null)
		{
			Object webView = RhoExtManager.getInstance().getWebView().getView();
			if (webView instanceof ViewGroup)
			{
				ViewGroup wv = (ViewGroup) webView;
				if ((wv != null) && (signatureView != null))
				{
					wv.removeView(signatureView);
					signatureView = null;
				}
			}
			else
			{
				ViewGroup wv = (ViewGroup) (((View) webView).getParent());
				if ((wv != null) && (signatureView != null))
				{
					wv.removeView(signatureView);
					signatureView = null;
				}
			}
		}
	}
	
	/**
	 * Sets up the inline Signature capture graphical view
	 */
	private void setupView()
	{
		signatureView.setupView(properties.penColor, properties.penWidth, properties.bgColor, properties.border);
	}

	@Override
	public void setBgColor(int bgColor)
	{
		super.setBgColor(bgColor);
		clear(null);
		setupView();
	}

	@Override
	public void setBorder(boolean border)
	{
		super.setBorder(border);
		signatureView.setBorder(border);
		//setupView();
	}

	@Override
	public void setPenColor(int penColor)
	{
		super.setPenColor(penColor);
		clear(null);
		setupView();
	}

	@Override
	public void setPenWidth(int penWidth)
	{
		super.setPenWidth(penWidth);
		clear(null);
		setupView();
	}

	@Override
	public void setTop(int top)
	{
		super.setTop(top);
		if(handler!=null)
		{
			handler.post(new Runnable(){

							@Override
							public void run() {
								signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
								
							}
			
						    }
				       );	
		}
	
		else
			signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
	}

	@Override
	public void setLeft(int left)
	{
		super.setLeft(left);
		if(handler!=null)
		{
				handler.post(new Runnable(){

			@Override
			public void run() {
				signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
				
			}
			
		});	
		}
		else
			signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
	}
	
	@Override
	public void setHeight(int height)
	{
		super.setHeight(height);
		clear(null);
		if(handler!=null)
		{
			handler.post(new Runnable(){

			@Override
			public void run() {
				signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
				
			}
			
		});	
		}
		else	
			signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
	}

	@Override
	public void setWidth(int width)
	{
		super.setWidth(width);
		clear(null);
		if(handler!=null)
		{
				handler.post(new Runnable(){

			@Override
			public void run() {
				signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
				
			}
			
		});
		}
		else
			signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
	}

	@Override
	public void show()
	{
		Logger.D(Signature.LOGTAG, "$$$ Start make of Signature View");

		Object webView = RhoExtManager.getImplementationInstance().getWebView().getView();

		if(signatureView == null)
		{
			signatureView = new SignatureView(ContextFactory.getUiContext(), null);
			signatureView.setSingletonId(singleton.hashCode());
			signatureView.setupView(properties.penColor, properties.penWidth, properties.bgColor, properties.border);
			signatureView.setLayoutParams(new AbsoluteLayout.LayoutParams(properties.width,	properties.height, properties.left, properties.top));
			//During scrolling, the view was moving when app_type="rhoelements". Hence provided this fix	
			ViewGroup wv = (ViewGroup) (((View) webView).getParent());
			if ((wv != null) && (signatureView != null))
			{
				wv.addView(signatureView);
				wv.bringChildToFront(signatureView);
				signatureView.requestFocus();
				signatureView.bringToFront();
				signatureView.invalidate();
			}
	
	
		/*	if (webView instanceof ViewGroup)
			{
				ViewGroup wv = (ViewGroup) webView;
				if ((wv != null) && (signatureView != null))
				{
					wv.addView(signatureView);
					wv.bringChildToFront(signatureView);
					signatureView.requestFocus();
					signatureView.bringToFront();
					signatureView.invalidate();
				}
			}
			else
			{
				ViewGroup wv = (ViewGroup) (((View) webView).getParent());
				if ((wv != null) && (signatureView != null))
				{
					wv.addView(signatureView);
					wv.bringChildToFront(signatureView);
					signatureView.requestFocus();
					signatureView.bringToFront();
					signatureView.invalidate();
				}
			}
			*/
		}
		signatureView.setVisibility(View.VISIBLE);
		Logger.D(LOGTAG, " $$$ Finish make of Signature View");
	}
}
