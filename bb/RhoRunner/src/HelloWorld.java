/**
 * The sentinal sample!
 *
 * Copyright ¬ 1998-2006 Research In Motion Ltd.
 */

//package com.rim.samples.device.helloworld;
//import com.rim.samples.device.resource.*;
import net.rim.device.api.ui.*;
import net.rim.device.api.ui.component.*;
import net.rim.device.api.ui.container.*;
import net.rim.device.api.i18n.*;
import net.rim.device.api.system.*;

import net.rim.device.api.collection.util.*; //import org.apache.oro.text.*;
//import xruby.rhodbTest3.*;
//import org.garret.perst.*;
//import xruby.Test1.*;
//import java.io.InputStream;
import xruby.TestServe.*;
import com.rho.sync.SyncEngine;

/**
 * <p>
 * The HelloWorld sample demonstrates some of the most basic features of the RIM
 * UI and internationalization (i18n).
 */
public class HelloWorld extends UiApplication {
	// statics
	// ------------------------------------------------------------------
	public static void main(String[] args) {
		// com.xruby.runtime.lang.RubyRuntime.init(args);
		// String str = new String("");
		// InputStream stream =
		// str.getClass().getResourceAsStream("/resources/Rhosugar/index.html");

		main.main(args);
		SyncEngine.start(null);

		// com.xruby.runtime.lang.RubyRuntime.fini();
		HelloWorld theApp = new HelloWorld();
		// To make the application enter the event thread and start processing
		// messages, we invoke the enterEventDispatcher method
		theApp.enterEventDispatcher();
	}

	/**
	 * <p>
	 * the default constructor. Creates all the RIM UI components and pushes the
	 * application's root screen onto the UI stack
	 */
	public HelloWorld() {
		// Push the main screen instance onto the UI stack for rendering.
		pushScreen(new HelloWorldScreen());
	}

}

/* package */final class HelloWorldScreen extends MainScreen// implements
															// HelloWorldResResource
{
	// members
	// ------------------------------------------------------------------

	/**
	 * <p>
	 * It is RIM recommended practice that you internationalize your code from
	 * the beginning. In order to do so, you'll need to create an RRH file, and
	 * various RRC files, one for each language or dialect you intend to
	 * support. Review the associated RRH and RRC files included with this
	 * project. Notice that the HelloWorld class implements a resouce interface.
	 * That interface is automatically generated from the RRH file.
	 */
	// private static ResourceBundle _resources =
	// ResourceBundle.getBundle(BUNDLE_ID, BUNDLE_NAME);
	/**
	 * Add the title and a simple richtext field
	 */
	public HelloWorldScreen() {
		super(DEFAULT_MENU | DEFAULT_CLOSE);

		// Add a field to the title region of the screen. We use a simple
		// LabelField here. The ELLIPSIS option truncates
		// the label text with "..." if the text was too long for the space
		// available.
		// setTitle(new LabelField(_resources.getString(HELLOWORLD_TITLE),
		// LabelField.ELLIPSIS | LabelField.USE_ALL_WIDTH));
		setTitle(new LabelField("Hello", LabelField.ELLIPSIS
				| LabelField.USE_ALL_WIDTH));
		// Add a read only text field (RichTextField) to the field portion of
		// the screen. The RTF is focusable by default.
		// add(new
		// RichTextField(_resources.getString(HELLOWORLD_CONTENTSTRING)));
		add(new RichTextField("Hello text"));
	}

	public boolean onClose() {
		// Dialog.alert(_resources.getString(HELLOWORLD_EXITSTRING));
		System.exit(0);
		return true;
	}

	public boolean keyChar(char key, int status, int time) {
		// intercept the ESC key - exit the app on its receipt
		boolean retval = false;

		switch (key) {
		case Characters.ESCAPE:
			onClose();
			retval = true;
			break;
		}

		return retval;
	}
}

/*
 * public static class Player extends Persistent { String fname;
 *  // Serialize the object public void writeObject(IOutputStream out) {
 * out.writeString(fname); }
 *  // Deserialize the object public void readObject(IInputStream in) { fname =
 * in.readString(); }
 * 
 * public Player() {}
 * 
 * Player(Storage db, String row[]) throws NumberFormatException { super(db);
 * fname = row[0]; } }; public static class Root extends Persistent { Index
 * playerLName;
 *  // Deserialize the object public void readObject(IInputStream in) {
 * playerLName = (Index)in.readObject(); }
 *  // Serialize the object public void writeObject(IOutputStream out) {
 * out.writeObject(playerLName); }
 *  // Default constructor (constructor without parameters) should not be used
 * for object initialization since it is // used to instantiate object each time
 * when it is loaded from the database. // So class should either not have
 * constructor at all (in this case it // will be generated automatically by
 * compiler), either provide // empty default constructor and constructor used
 * for object // initialization (usually it is passed reference to the Storage //
 * since it is need to create Perst collections). public Root() {}
 *  // This constructor is called once while database initialization. // It
 * initialize root object and creates indices to access teams // and players.
 * public Root(Storage db) { super(db); // Create unique index for team
 * identifiers // Create non-unique index for last name of the players
 * playerLName = db.createIndex(Types.String, false); } }
 */
/*
 * Storage storage = StorageFactory.getInstance().createStorage(); try {
 * storage.open("RhoTest.dbs"); } catch (StorageError x) { //error("Failed to
 * open database"); return; } Root root = (Root)storage.getRoot(); if (root ==
 * null) { // if root object was not specified, then storage is not yet
 * initialized // Perform initialization: // ... create root object root = new
 * Root(storage); // ... register new root object storage.setRoot(root); // ...
 * and import data from resource files // importData(); }
 * 
 * String[] row = new String[1]; row[0] = "pasy"; Player player = new
 * Player(storage, row); root.playerLName.put(player.fname.toLowerCase(),
 * player); storage.commit();
 * 
 * Player player2 = (Player)root.playerLName.get(new Key("pasy"));
 */
