package com.rhomobile.rhodes.phonebook;

import java.util.List;
import java.util.ArrayList;

public class ContactList {
	
	private List<Contact> contacts = new ArrayList<Contact>();

	public List<Contact> getContacts() {
		return contacts;
	}

	public void setContacts(List<Contact> contacts) {
		this.contacts = contacts;
	}
	
	public void addContact(Contact contact)
	{
		this.contacts.add(contact);
	}
}
