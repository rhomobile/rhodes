package com.rhomobile.rhodes.phonebook;

public class ContactField {

	private String key;
	private String value;
	
	ContactField(String k, String v) {
		key = k;
		value = v;
	}
	
	String getKey() {
		return key;
	}
	
	String getValue() {
		return value;
	}
	
}
