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

package com.rhomobile.rhodes.osfunctionality;

import android.accounts.Account;
import android.accounts.AccountManager;
import android.content.Context;


//Android 2.0
class AndroidFunctionality05 extends AndroidFunctionality04 implements AndroidFunctionality {

	 private Account AccessOwnerInfo_getAccount(AccountManager accountManager) {
		    Account[] accounts = accountManager.getAccountsByType("com.google");
	    Account account;
	    if (accounts.length > 0) {
	      account = accounts[0];
	    } else {
	      account = null;
	    }
	    return account;
	  }
	
	  public String AccessOwnerInfo_getEmail(Context context) {
	    AccountManager accountManager = AccountManager.get(context);
	    Account account = AccessOwnerInfo_getAccount(accountManager);
	
	    if (account == null) {
	      return null;
	    } else {
	      return account.name;
	    }
	  }
	
	  public String AccessOwnerInfo_getUsername(Context context) {
	    // String email;
		AccountManager manager = AccountManager.get(context);
		Account account = AccessOwnerInfo_getAccount(manager);
		if (account == null) {
		  return "";
		} else {
		  String email = account.name;
		  String[] parts = email.split("@");
		  if (parts.length > 0 && parts[0] != null)
		    return parts[0];
		  else
		    return "";
	    }
	  }	
	
}