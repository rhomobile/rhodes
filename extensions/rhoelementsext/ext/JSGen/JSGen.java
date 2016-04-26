import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import javax.imageio.metadata.IIOMetadataNode;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;
import org.omg.CORBA.Environment;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public class JSGen
{
	public static void main(String[] args) throws IOException
	{
		if(args.length != 3)
		{
			System.out.println("ERROR: Incorrect Parameters.");
			System.out.println("Expected: JSGen [XMLDirectory] [XSLFile] [OutputFile]");
			System.exit(0);
		}
		new JSGen(args[0], args[1], args[2]);
	}
	
	public JSGen(String directory, String inputXslFile, String inputOutputFile) throws IOException
	{
		URL directoryURL = new URL(new URL("file", null, System.getProperty("user.dir")), directory);
		URL xslURL = new URL(new URL("file", null, System.getProperty("user.dir")), inputXslFile);
		URL outputURL = new URL(new URL("file", null, System.getProperty("user.dir")), inputOutputFile);
		
		File directoryFile = new File(directoryURL.getPath());
		File xslFile = new File(xslURL.getPath());
		File outputFile = new File(outputURL.getPath());
		
		if(!directoryFile.exists())
		{
			System.out.println("ERROR: Directory does not exist. Exiting...");
			return;
		}
		
		if(!directoryFile.isDirectory())
		{
			System.out.println("ERROR: XMLDirectory is not a directory. Exiting...");
			return;
		}
		
		if(!xslFile.exists() || !xslFile.canRead())
		{
			System.out.println("ERROR: Could not read xsl file. Exiting...");
			System.exit(0);
		}
		
		if(outputFile.exists())
		{
			outputFile.delete();
		}
		
		try		//Check the file location
		{
			outputFile.createNewFile();
		}
		catch (IOException e)
		{
			System.out.println("ERROR: Could not create output file. Exiting...");
			System.exit(0);
		}
		
		File[] xmlFiles = directoryFile.listFiles(new FilenameFilter()
		{
			@Override
			public boolean accept(File dir, String name)
			{
				if(name.endsWith(".xml"))
				{
					return true;
				}
				return false;
			}
		});
		
		//Concatenate into one file		
		try
		{
			File tempFile = File.createTempFile("xmlgen", ".xml");
			PrintWriter writer = new PrintWriter(tempFile);
			writer.println("<root>");
			
			for(File xmlFile: xmlFiles)
			{
				BufferedReader bufferedReader = new BufferedReader(new FileReader(xmlFile));
				String line = bufferedReader.readLine();
				while(line != null)
				{
					if(!line.trim().startsWith("<?"))
					{
						writer.println(line);
					}
					line = bufferedReader.readLine();
				}
				bufferedReader.close();
			}
			writer.println("</root>");
			writer.close();
			
			//Parse and capture scanner things
			XMLGenHandler handler = new XMLGenHandler();
			SAXParserFactory parserFactory = SAXParserFactory.newInstance();
			SAXParser parser = parserFactory.newSAXParser();
			parser.parse(tempFile, handler);
			try
			{
				TransformerFactory tFactory = TransformerFactory.newInstance();
				Transformer transformer = tFactory.newTransformer(new StreamSource(xslFile));
				
				transformer.transform(new StreamSource(handler.getTempFile()), new StreamResult(new FileOutputStream(outputFile)));
				System.out.println("Javascript file successfully generated.");
			}
			catch (TransformerConfigurationException e)
			{
				System.err.println("ERROR: XSLT config exception: " + e.getMessage());
				System.exit(0);
			}
			catch (TransformerException e)
			{
				System.err.println("ERROR: XSLT exception: " + e.getMessage());
				System.exit(0);
			}
		}
		catch(IOException e)
		{
			System.err.println("ERROR: file concatenation: " + e.getMessage());
			System.exit(0);
		}
		catch (ParserConfigurationException e)
		{
			System.err.println("ERROR: parser config: " + e.getMessage());
			System.exit(0);
		}
		catch (SAXException e)
		{
			System.err.println("ERROR: parser exception: " + e.getMessage());
			System.exit(0);
		}
	}

	private class XMLGenHandler extends DefaultHandler
	{
		private File	outputFile;
		private PrintWriter outputWriter;
		private String	parentNode;
		private String	currentNode;
		private String	lastName;
		private ArrayList<String> hierarchy;
		
		private String nodeName;
		private ArrayList<String> nodeParams;
		private ArrayList<String> nodeMethods;
		private ArrayList<String> nodeReturns;
		private boolean isNodeScanner;
		private boolean isJSObjectFalse;
		private boolean isActiveX;
		private boolean isVsControls;
		
		private ArrayList<String> scannerParams;
		private ArrayList<String> scannerMethods;
		private ArrayList<String> scannerReturns;
		
		private String	text;


		public XMLGenHandler() throws IOException
		{
			outputFile = File.createTempFile("JSGen", null);
			outputWriter = new PrintWriter(outputFile);
			hierarchy = new ArrayList<String>();
			scannerParams = new ArrayList<String>();
			scannerMethods = new ArrayList<String>();
			scannerReturns = new ArrayList<String>();
			outputWriter.println("<ALLOBJECTS>");
		}
		
		@Override
		public void endDocument() throws SAXException
		{
			finishWriting();
		}

		@Override
		public void startElement(String uri, String localName, String qName, Attributes attributes) throws SAXException
		{
			hierarchy.add(qName.toLowerCase());
			if(qName.equalsIgnoreCase("func"))
			{
				nodeName = "";
				nodeParams = new ArrayList<String>();
				nodeMethods = new ArrayList<String>();
				nodeReturns = new ArrayList<String>();
				isNodeScanner = false;
				isJSObjectFalse = false;
				isVsControls = false;
				isActiveX = false;
			}
		}

		@Override
		public void endElement(String uri, String localName, String qName) throws SAXException
		{
			if(qName.equalsIgnoreCase("name") && hierarchy.size() >= 2 && hierarchy.get(hierarchy.size() - 2).equals("func"))
			{
				nodeName = text;
				if(nodeName.equalsIgnoreCase("scanner"))
				{
					isNodeScanner = true;
				}
			}
			else if(qName.equalsIgnoreCase("name") && hierarchy.size() >= 3 && hierarchy.get(hierarchy.size() - 2).equals("param") && hierarchy.get(hierarchy.size() - 3).equals("params_with_attributes"))
			{
				nodeParams.add(text);
			}
			else if(qName.equalsIgnoreCase("name") && hierarchy.size() >= 3 && hierarchy.get(hierarchy.size() - 2).equals("param") && hierarchy.get(hierarchy.size() - 3).equals("params_no_attributes"))
			{
				//is a method name
				nodeMethods.add(text);
			}
			else if(qName.equalsIgnoreCase("tag_name"))
			{
				//is a event
				nodeReturns.add(text);
			}
			else if(qName.equalsIgnoreCase("path"))
			{
				if(text.startsWith("Barcode Scanner\\"))
				{
					//Is a scanner tag
					isNodeScanner = true;
				}
				else if(text.startsWith("legacyObjects"))
				{
					isActiveX = true;
				}
			}
			else if(qName.equalsIgnoreCase("func"))
			{
				//Store Data
				if(isNodeScanner && !isJSObjectFalse)
				{
					scannerParams.addAll(nodeParams);
					scannerMethods.addAll(nodeMethods);
					scannerReturns.addAll(nodeReturns);
				}
				else if(!isJSObjectFalse && !isActiveX)
				{
					writeNode();
				}
			}
			else if(qName.equalsIgnoreCase("jsobject"))
			{
				if(text.equalsIgnoreCase("false"))
				{
					isJSObjectFalse = true;
				}
			}
			else if(qName.equalsIgnoreCase("vscontrols"))
			{
				nodeParams.remove(nodeParams.size() -1);
			}
			hierarchy.remove(hierarchy.size() -1);
			text = null;
		}

		@Override
		public void characters(char[] ch, int start, int length) throws SAXException
		{
			text = new String(ch, start, length);
		}
		
		public File getTempFile()
		{
			return outputFile;
		}
		
		private void writeNode()
		{
			outputWriter.println("\t<FUNC>");
			outputWriter.println("\t\t<NAME>" + nodeName + "</NAME>");
			for(String method: nodeMethods)
			{
				outputWriter.println("\t\t<METHOD><NAME>" + method + "</NAME></METHOD>");
			}
			for(String param: nodeParams)
			{
				outputWriter.println("\t\t<PARAM><NAME>" + param + "</NAME></PARAM>");
			}
			for(String param: nodeReturns)
			{
				outputWriter.println("\t\t<RETURNS><NAME>" + param + "</NAME></RETURNS>");
			}
			outputWriter.println("\t</FUNC>");
		}
		
		private void writeScanner()
		{
			outputWriter.println("\t<FUNC>");
			outputWriter.println("\t\t<NAME>scanner</NAME>");
			for(String method: scannerMethods)
			{
				outputWriter.println("\t\t<METHOD><NAME>" + method + "</NAME></METHOD>");
			}
			for(String param: scannerParams)
			{
				outputWriter.println("\t\t<PARAM><NAME>" + param + "</NAME></PARAM>");
			}
			for(String param: scannerReturns)
			{
				outputWriter.println("\t\t<RETURNS><NAME>" + param + "</NAME></RETURNS>");
			}
			outputWriter.println("\t</FUNC>");
		}
		
		private void finishWriting()
		{
			writeScanner();
			outputWriter.println("</ALLOBJECTS>");
			outputWriter.close();
		}
	}
}
