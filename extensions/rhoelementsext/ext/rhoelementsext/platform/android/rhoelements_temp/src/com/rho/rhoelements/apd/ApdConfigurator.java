/**
 * 
 */
package com.rho.rhoelements.apd;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.*;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import com.rho.rhoelements.Common;
import com.rho.rhoelements.LogEntry;
import com.rho.rhoelements.apd.ApdConfiguration.Language;
import com.rho.rhoelements.apd.ApdConfiguration.Transport;

/**
 * @author FPC843
 *
 */
public class ApdConfigurator
{
	public static final String APD_CONFIGURATION_FILE = Common.getDataPath() + "/apdconfig.xml";
	
	
	public static ApdConfiguration readConfiguration() throws IOException
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		ApdConfiguration res = new ApdConfiguration();
		URL url;
		try
		{
			url = new URL("file://" + APD_CONFIGURATION_FILE); 
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(new InputSource(url.openStream()));
			doc.getDocumentElement().normalize();
			
			NodeList nodeList = doc.getElementsByTagName("path");
			Node node = nodeList.item(0);
			Element pathElement = (Element) node;
			res.setFormatPath(pathElement.getAttribute("formats").replace("%INSTALLDIR%", Common.internalDataPath));
			res.setTemplatePath(pathElement.getAttribute("templates").replace("%INSTALLDIR%", Common.internalDataPath));
			res.setLabelIdPath(pathElement.getAttribute("idlabels").replace("%INSTALLDIR%", Common.internalDataPath));
			
			nodeList = doc.getElementsByTagName("printer");
			node = nodeList.item(0);
			pathElement = (Element) node;
			if (pathElement.getAttribute("bluetooth").compareTo("") != 0)
				res.setBtMac(pathElement.getAttribute("bluetooth"));
			else
				res.setBtMac(null);
			
			if (pathElement.getAttribute("id").compareTo("0") != 0)
				res.setId(Integer.valueOf(pathElement.getAttribute("id")));
			else
				res.setId(0);
			
			if (pathElement.getAttribute("ipaddress").compareTo("") != 0)
				res.setIpAddress(pathElement.getAttribute("ipaddress"));
			else
				res.setIpAddress(null);
			
			if (pathElement.getAttribute("ipport").compareTo("0") != 0)
				res.setIpPort((Integer.valueOf(pathElement.getAttribute("ipport"))));
			else
				res.setIpPort(0);
			
			if (pathElement.getAttribute("language").compareTo("") != 0)
				res.setLanguage(Language.values()[pathElement.getAttribute("language").charAt(0) - 65]);
			else
				res.setLanguage(null);
			
			if (pathElement.getAttribute("message").compareTo("0") != 0)
				res.setMessage(Integer.valueOf(pathElement.getAttribute("message")));
			else
				res.setMessage(0);
			
			if (pathElement.getAttribute("model").compareTo("0") != 0)
				res.setModel(Integer.valueOf(pathElement.getAttribute("model")));
			else
				res.setMessage(0);
			
			if (pathElement.getAttribute("transport").length() == 1)
				res.setTransport(Transport.values()[Integer.valueOf(pathElement.getAttribute("transport"))]);
			else
				res.setLanguage(null);
			
		}
		catch (MalformedURLException e)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		} catch (ParserConfigurationException e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		} catch (SAXException e) {
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, e.getMessage()));
		} 
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return res;
	}
	
	public static boolean writeConfiguration(ApdConfiguration apdConfiguration) throws IOException
	{
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "Start"));
		if (apdConfiguration == null)
		{
			Common.logger.add(new LogEntry(LogEntry.PB_LOG_ERROR, "apdConfiguration not valid"));
			return false;
		}
		FileWriter apdConfigFileWriter = null;
		BufferedWriter fileBufferedWriter = null;
		//Carry out some sanity check on the configuration passed as parameter
		if ( (apdConfiguration.getFormatPath() == null) || (apdConfiguration.getFormatPath().length() == 0) )
			apdConfiguration.setFormatPath(ApdEngine.DEFAULT_FORMAT_PATH);
		if ( (apdConfiguration.getTemplatePath() == null) || (apdConfiguration.getTemplatePath().length() == 0) )
			apdConfiguration.setTemplatePath(ApdEngine.DEFAULT_TEMPLATE_PATH);
		if ( (apdConfiguration.getLabelIdPath() == null) || (apdConfiguration.getLabelIdPath().length() == 0) )
			apdConfiguration.setTemplatePath(ApdEngine.DEFAULT_ID_PATH);
		
		File apdConfigFile = new File(APD_CONFIGURATION_FILE);
		apdConfigFile.delete();
		apdConfigFileWriter = new FileWriter(apdConfigFile);
		fileBufferedWriter = new BufferedWriter(apdConfigFileWriter);
		fileBufferedWriter.write("<apdconfiguration>\r\n");
		
		fileBufferedWriter.write("\t<path ");
		fileBufferedWriter.write("formats=\"" + 
				(apdConfiguration.getFormatPath()==null?"":apdConfiguration.getFormatPath()) + "\"\r\n");
		fileBufferedWriter.write("\t\ttemplates=\"" + 
				(apdConfiguration.getTemplatePath()==null?"":apdConfiguration.getTemplatePath()) + "\"\r\n");
		fileBufferedWriter.write("\t\tidlabels=\"" + 
				(apdConfiguration.getLabelIdPath()==null?"":apdConfiguration.getLabelIdPath()) + "\" />\r\n");
		fileBufferedWriter.write("\t<printer ");
		fileBufferedWriter.write("bluetooth=\"" + 
				(apdConfiguration.getBtMac()==null?"":apdConfiguration.getBtMac()) + "\"\r\n");
		fileBufferedWriter.write("\tid=\"" + apdConfiguration.getId() + "\"\r\n");
		fileBufferedWriter.write("\tipaddress=\"" + 
				(apdConfiguration.getIpAddress()==null?"":apdConfiguration.getIpAddress()) + "\"\r\n");
		fileBufferedWriter.write("\tipport=\"" + apdConfiguration.getIpPort() + "\"\r\n");
		fileBufferedWriter.write("\tlanguage=\"" + 
				(apdConfiguration.getLanguage()==null?"":String.valueOf((char)(apdConfiguration.getLanguage().ordinal() + 65)) + "\"\r\n")); //65 = ASCII code of 'A'
		fileBufferedWriter.write("\tmessage=\"" + apdConfiguration.getMessage() + "\"\r\n");
		fileBufferedWriter.write("\tmodel=\"" + apdConfiguration.getModel() + "\"\r\n");
		fileBufferedWriter.write("\ttransport=\"" + apdConfiguration.getTransport().ordinal() + "\"/>\r\n");
		fileBufferedWriter.write("</apdconfiguration>\r\n");
		fileBufferedWriter.flush();
		fileBufferedWriter.close();
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_INFO, "apdconfig.xml written succesfully"));
		Common.logger.add(new LogEntry(LogEntry.PB_LOG_DEBUG, "End"));
		return true;
	}
}

