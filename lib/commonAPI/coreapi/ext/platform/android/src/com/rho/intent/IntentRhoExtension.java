package com.rho.intent;

import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Environment;

import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.extmanager.AbstractRhoExtension;
import com.rhomobile.rhodes.extmanager.IRhoConfig;
import com.rhomobile.rhodes.extmanager.IRhoExtManager;

public class IntentRhoExtension extends AbstractRhoExtension{

	public static int RECEIVER_REGISTER_STATUS = 0;
	
	@Override
	public boolean onNewConfig(IRhoExtManager extManager, IRhoConfig config,
			String name, boolean res) {
		if (config.isExist("enablereceiver")) {
			String EnableReceiver = config.getString("enablereceiver");
			int reg_receiver = 0;
			IntentFilter intentfilter = new IntentFilter();
			IntentReceiver mReceiver = new IntentReceiver();
			if (EnableReceiver != null) {
				reg_receiver = Integer.parseInt(EnableReceiver);
				if(reg_receiver == 1){
					String externalSharedPath = Environment.getExternalStorageDirectory().toString();
                    String configPath = new File(externalSharedPath, "/Android/data/com.symbol.enterprisebrowser/Config.xml").getAbsolutePath();
                    File fXmlFile = new File(configPath);
                    DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
                    DocumentBuilder dBuilder = null;
                    Document doc = null;
					try {
						dBuilder = dbFactory.newDocumentBuilder();
						doc = dBuilder.parse(fXmlFile);
					} catch (ParserConfigurationException e1) {
						e1.printStackTrace();
					} catch (SAXException e) {
						e.printStackTrace();
					} catch (IOException e) {
						e.printStackTrace();
					}
                    doc.getDocumentElement().normalize();
                    NodeList nList = doc.getElementsByTagName("IntentAction");
                    for (int temp = 0; temp < nList.getLength(); temp++) {
                    	intentfilter.addAction(nList.item(temp).getAttributes().getNamedItem("value").getTextContent());
                    	RhodesActivity.getContext().registerReceiver(mReceiver, intentfilter);
                    	RhodesActivity.getContext().sendBroadcast(new Intent(nList.item(temp).getAttributes().getNamedItem("value").getTextContent()));
                    	RECEIVER_REGISTER_STATUS = 1;
                    }
                    NodeList nList_cat = doc.getElementsByTagName("IntentCategory");
                    for (int i = 0; i < nList_cat.getLength(); i++) {
                    	intentfilter.addCategory(nList_cat.item(i).getAttributes().getNamedItem("value").getTextContent());
                    	RhodesActivity.getContext().registerReceiver(mReceiver, intentfilter);
                    	RhodesActivity.getContext().sendBroadcast(new Intent(nList_cat.item(i).getAttributes().getNamedItem("value").getTextContent()));
                    	RECEIVER_REGISTER_STATUS = 1;
					}
				}
			}
			
		}
		return super.onNewConfig(extManager, config, name, res);
	}
}
