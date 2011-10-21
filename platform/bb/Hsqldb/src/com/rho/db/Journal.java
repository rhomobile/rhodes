/*------------------------------------------------------------------------
* (The MIT License)
*
* Copyright (c) 2008-2011 Rhomobile, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* http://rhomobile.com
*------------------------------------------------------------------------*/

package com.rho.db;

import com.rho.RhoEmptyLogger;
import com.rho.RhoLogger;
import com.rho.file.RandomAccessFile;
import java.io.IOException;
import java.io.EOFException;

public class Journal
{
	private static final RhoLogger LOG = RhoLogger.RHO_STRIP_LOG ? new RhoEmptyLogger() :
		new RhoLogger("DBJournal");

	final static byte BLOCK_CONFIRM = 0x55;

	String m_journalName;
	String m_dataName;
	RandomAccessFile m_dataFile;
	RandomAccessFile m_journalFile;
	boolean          m_bStarted = false;

	public Journal(){

	}

	public void create( String name )throws IOException{
		int nDot = name.lastIndexOf('.');
		if ( nDot > 0 )
			m_journalName = name.substring(0, nDot);

		m_journalName += ".journal";
		m_dataName = name;
		//m_dataFile = file;

		LOG.INFO("Create: " + m_journalName );

		rollback();
	}

	public void close()throws IOException{
		if ( m_journalFile != null ){
			m_journalFile.stopListenForSync(m_dataName);
			m_journalFile.close();
			m_journalFile = null;
		}
		if ( m_dataFile != null ){
			m_dataFile.close();
			m_dataFile = null;
		}

	}

	public void start()throws IOException{
		if ( FileUtilBB.getDefaultInstance().isStreamElement(m_journalName))
			throw new IOException("Cannot start journal: journal file already exist");

		m_bStarted = true;
	}

	void initJournal()throws IOException{
		if ( !m_bStarted )
			throw new IOException("Cannot init journal: start should be called first");

		if ( m_dataFile == null )
			m_dataFile = new RandomAccessFile(m_dataName,"r");

		if ( m_journalFile == null ){
			m_journalFile = new RandomAccessFile(m_journalName,"rw");
			m_journalFile.listenForSync(m_dataName);
			m_journalFile.writeLong(m_dataFile.length());
			m_journalFile.write(BLOCK_CONFIRM);
			m_journalFile.sync();
		}

	}

	public void stop()throws IOException{
		close();

		FileUtilBB.getDefaultInstance().removeElement(m_journalName);

		m_bStarted = false;
	}

	void rollback()throws IOException{
		if ( !FileUtilBB.getDefaultInstance().isStreamElement(m_journalName))
			return;

		LOG.INFO("Start rollback journal");

		RandomAccessFile jf = null;
		RandomAccessFile df = null;
		boolean bSuccess = false;
		try{
			jf = new RandomAccessFile(m_journalName, "r");
			long nDataSize = jf.readLong();
			byte byteConfirm = (byte)jf.read();

			if ( byteConfirm == BLOCK_CONFIRM )
			{
				LOG.INFO("Rollback: open database file");

				df = new RandomAccessFile(m_dataName, "rw");
				df.setLength(nDataSize);

				while(true){
					long nPos = 0;
					byte[] buf = null;

					try{
						nPos = jf.readLong();
						long nLen = jf.readLong();

						buf = new byte[(int)nLen];
						long nReaded = jf.read(buf);

						byteConfirm = (byte)jf.read();
						if ( byteConfirm != BLOCK_CONFIRM || nReaded != nLen )
							break;

					}catch(EOFException exc){
						break;
					}

					df.seek(nPos);
					df.write(buf);
				}

			}
			bSuccess = true;
		}catch(IOException exc)
		{
			LOG.ERROR("Rollback failed.Journal is empty or corrupted", exc);
			//throw exc;
		}finally{
			if ( df != null )
			{
				df.close();
				df = null;
			}

			if ( jf != null )
			{
				jf.close();
				jf = null;
			}

//			if ( bSuccess )
			{
				FileUtilBB.getDefaultInstance().removeElement(m_journalName);
				LOG.INFO("End rollback journal");
			}
		}
	}

	public void write( long nPos, long nLen)throws IOException{
		initJournal();

		//if ( nPos >= m_dataFile.length() )
		//	return;

		byte[] buf = new byte[(int)nLen];
		m_dataFile.seek(nPos);
		long nReaded = m_dataFile.read(buf);
		if ( nReaded <= 0 )
			return;

//		if ( nReaded != nLen )
//			throw new IOException("Cannot read '" + nLen + "' bytes from data file: only '" + nReaded +"' is available");

		m_journalFile.writeLong(nPos);
		m_journalFile.writeLong(nReaded);
		m_journalFile.write(buf);
		m_journalFile.write(BLOCK_CONFIRM);
		m_journalFile.sync();

		//throw new IOException("TEST");

	}

}
