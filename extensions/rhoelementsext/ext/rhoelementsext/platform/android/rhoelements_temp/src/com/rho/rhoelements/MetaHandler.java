package com.rho.rhoelements;

import java.util.List;
import android.os.Handler;
import android.os.Message;

public class MetaHandler extends Handler
{
	private EMML emml;

	public MetaHandler()
	{
		super();
		emml = EMML.getEMML();
	}
	/**
	 * Parses 'http-equiv' to get plugin name and 'content' to get list of settings. Returns once the plugin has handled all the settings.
	 */
	@Override
	public void handleMessage(Message message)
	{
		MetaTag tag = (MetaTag) message.obj;

		// Ignore blank ones
		//TODO is this correct? cant a tag have an Equiv but no content?
		if (tag.getEquiv() == null || tag.getContent() == null)
			return;

		if (tag.getEquiv().equals("pageevent"))
		{
			if (tag.getContent().startsWith("started:"))
			{
				Common.pluginManager.pageStarted(tag.getContent().substring("started:".length()));
			}
			else if (tag.getContent().startsWith("finished:"))
			{
				Common.pluginManager.pageFinished(tag.getContent().substring("finished:".length()));
			}
		}
		// Events for background & Foreground focus events
		else if (tag.getEquiv().equals("focusevent"))
		{
			if (tag.getContent().startsWith("foreground:"))
			{
				Common.pluginManager.onForeground(tag.getContent().substring("foreground:".length()));
			}
			else if (tag.getContent().startsWith("background:"))
			{
				Common.pluginManager.onBackground(tag.getContent().substring("background:".length()));
			}
		}
		else
		{
			// Parse meta tag into plugin settings
			distributeSettings(emml.parseEMMLTag(tag.getEquiv(), tag.getContent()));
		}
	}

	public void distributeSettings(List<PluginSetting> settings)
	{
	    Common.pluginManager.applySettings(settings);
	}
	
	@SuppressWarnings("unused")
	private EMML getEMML()
	{
		return emml;
	}
}