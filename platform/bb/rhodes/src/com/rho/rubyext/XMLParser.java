package com.rho.rubyext;

import com.xruby.runtime.builtin.*;
import com.xruby.runtime.lang.*;
import com.rho.*;
import com.rho.net.URI;

import java.util.Vector;
import net.rim.device.api.xml.parsers.SAXParser;
import net.rim.device.api.xml.parsers.SAXParserFactory;
import java.io.*;

public class XMLParser extends RubyBasic {
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() : 
		new RhoLogger("XMLParser");

	RubyArray m_arEvents = new RubyArray();
	int       m_nCurPos = 0;
	
	XMLParser(RubyClass c) {
		super(c);
	}
	
    //@RubyAllocMethod
    private static RubyValue alloc(RubyValue receiver) {
    	return new XMLParser((RubyClass) receiver); 
    }
    
    RubyValue createEndDocumentEvent()
    {
    	RubyArray arEvent = new RubyArray();
    	arEvent.add(ObjectFactory.createSymbol("end_document"));
    	
    	return arEvent;
    }

    void addEvent(RubyValue valEvent)
    {
    	m_arEvents.add(valEvent);
    }
    
    void addEvent(String strName)
    {
    	RubyArray arEvent = new RubyArray();
    	arEvent.add(ObjectFactory.createSymbol(strName));
    	
    	m_arEvents.add(arEvent);
    }

    void addEvent(String strName, RubyValue arg1)
    {
    	RubyArray arEvent = new RubyArray();
    	arEvent.add(ObjectFactory.createSymbol(strName));
    	arEvent.add(arg1);
    	
    	m_arEvents.add(arEvent);
    }

    void addEvent(String strName, RubyValue arg1, RubyValue arg2)
    {
    	RubyArray arEvent = new RubyArray();
    	arEvent.add(ObjectFactory.createSymbol(strName));
    	arEvent.add(arg1);
    	arEvent.add(arg2);
    	
    	m_arEvents.add(arEvent);
    }
    
    static class XmlSAXHandler extends org.xml.sax.helpers.DefaultHandler 
	{
    	XMLParser m_parser;
    	XmlSAXHandler(XMLParser parser)
    	{
    		m_parser = parser;
    	}
    	
    	public void startDocument() 
    	{
    	} 
    	
    	public void endDocument()  
    	{
    		m_parser.addEvent( m_parser.createEndDocumentEvent() );
    	}
  
		public void startElement(String uri, String localName, String qname, org.xml.sax.Attributes attr)
		{
			RubyHash attrHash = new RubyHash();
		    int attrCount = attr.getLength();
	    	for ( int i = 0; i < attrCount; i++ ) 
	    	{
	    		attrHash.add( ObjectFactory.createString(attr.getQName(i)), 
	    				ObjectFactory.createString( URI.ampEncode(attr.getValue(i)) ));
	    	}
		    
			m_parser.addEvent( "start_element", ObjectFactory.createString(qname), attrHash );
		}
  
		public void endElement(String uri, String localName, String qname) 
		{
    		m_parser.addEvent( "end_element", ObjectFactory.createString(qname) );
		}
  
		public void characters(char[] ch, int start, int length) 
		{
    		m_parser.addEvent( "text", ObjectFactory.createString(
    				new String(ch, start, length) ) );
		}

		public void ignorableWhitespace(char[] ch, int start, int length) 
		{
			//System.out.println("Ignorable whitespace: " + new String(ch, start, length));
		}

		public void startPrefixMapping(String prefix, String uri) 
		{
			//System.out.println("Start \"" + prefix + "\" namespace scope. URI: " + uri); 
		}

		public void endPrefixMapping(String prefix) 
		{
			//System.out.println("End \"" + prefix + "\" namespace scope."); 
		}

		public void warning(org.xml.sax.SAXParseException spe) 
		{
			LOG.INFO("Warning at line "+spe.getLineNumber() + "Message: " + spe.getMessage());
		}

  		public void fatalError(org.xml.sax.SAXParseException spe) throws org.xml.sax.SAXException 
  		{
  			LOG.ERROR("XML parse failed : " + spe.getMessage() + "; Line: " + spe.getLineNumber(), spe);
  			throw spe;
  		}
	}
	
    private void parse(String strXML)throws Exception 
    {
		InputStream in = new ByteArrayInputStream(strXML.getBytes());
		
		parse(in);
    }

    private void parse(InputStream in)throws Exception 
    {
    	if ( in == null )
    		return;
    	
		SAXParserFactory factory = SAXParserFactory.newInstance();
		SAXParser parser = factory.newSAXParser();
		
		m_nCurPos = 0;
		parser.parse(in, new XmlSAXHandler(this));
		
		LOG.INFO("Parse finished");
    }
    
    private RubyValue pull() 
    {
    	if ( m_nCurPos >= m_arEvents.size() )
    		return createEndDocumentEvent();
    	
    	RubyValue res = m_arEvents.get(m_nCurPos);
    	m_nCurPos++;
    	return res;
    }
    
	public static void initMethods(RubyClass klass) {
		
		klass.defineAllocMethod(new RubyNoArgMethod(){
			protected RubyValue run(RubyValue receiver, RubyBlock block )	{
				return XMLParser.alloc(receiver);}
		});
		
		klass.defineMethod( "initialize", new RubyOneArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyValue arg, RubyBlock block )
			{
		    	try
		    	{
		    		if ( arg != null && arg instanceof RubyFile)
		    		{
		    			RubyFile file = (RubyFile)arg;
		    			((XMLParser)receiver).parse(file.getInputStream());
		    		}
		    		else if (arg != null && arg instanceof RubyString)
		    		{
		    			((XMLParser)receiver).parse(arg !=null && arg != RubyConstant.QNIL ? arg.toStr() : "");
		    		}else
		    	        throw new RubyException(RubyRuntime.ArgumentErrorClass, "in `" + this.getID() + "': wrong type of argument. should be String or File");
		    		
				}catch(Exception e)
				{
					LOG.ERROR("initialize failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
		    	
		        return receiver;
			}
		});
		klass.defineMethod( "pull", new RubyNoArgMethod(){ 
			protected RubyValue run(RubyValue receiver, RubyBlock block )
			{
		    	try
		    	{
		    		return ((XMLParser)receiver).pull();
				}catch(Exception e)
				{
					LOG.ERROR("initialize failed.", e);
					throw (e instanceof RubyException ? (RubyException)e : new RubyException(e.getMessage()));
				}
			}
		});
	}
	
}
