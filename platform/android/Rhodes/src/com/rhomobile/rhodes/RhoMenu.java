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

package com.rhomobile.rhodes;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.rhomobile.rhodes.mainview.MainView;

import android.view.Menu;
import android.view.MenuItem;

public class RhoMenu {
    
    private static enum ItemType {
        ItemTypeBack("Back") {
            @Override public boolean action() {
                WebView.navigateBack();
                return true;
            }
        },
        ItemTypeHome("Home") {
            @Override public boolean action() {
                WebView.navigateHome();
                return true;
            }
        },
        ItemTypeRefresh("Refresh") {
            @Override public boolean action() {
                WebView.refresh();
                return true;
            }
        },
        ItemTypeSync("Sync") {
            @Override public boolean action() {
                RhodesService.getInstance().doSyncAllSources(true);
                return true;
            }
        },
        ItemTypeOptions("Options") {
            @Override public boolean action() {
                WebView.navigateOptions();
                return true;
            }
        },
        ItemTypeExit("Exit") {
            @Override public boolean action() {
                RhodesService.exit();
                return true;
            }
        },
        ItemTypeClose("Close") {
            @Override public boolean action() {
                RhodesService.exit();
                return true;
            }
        },
        ItemTypeFullScreen("FullScreen") {
            @Override public boolean action() {
                RhodesActivity.setFullScreenMode(!RhodesActivity.getFullScreenMode());
                return true;
            }
        },
        ItemTypeLog("Log") {
            @Override public boolean action() {
                RhodesService.showLogView();
                return true;
            }
        },
        ItemTypeUrl("URL");
        
        private String mStrType;
        
        private ItemType(String type) {
            mStrType = type;
        }
        @Override
        public String toString() {
            return mStrType;
        }
        
        public boolean action() {
            return false;
        }
    }

    private static final String TAG = "Menu";

    private static class Item
    {
        public String title;
        public String url;
        public ItemType type;

        public Item(String title, String url, ItemType type) {
            this.title = title;
            this.type = type;
            if (type == ItemType.ItemTypeUrl)
                this.url = url;
        }
    };
    
    private static ItemType getItemType(String str) {
        if (str.equalsIgnoreCase(ItemType.ItemTypeBack.toString())) {
            return ItemType.ItemTypeBack;
        } else if (str.equalsIgnoreCase(ItemType.ItemTypeHome.toString())) {
            return ItemType.ItemTypeHome;
        } else if (str.equalsIgnoreCase(ItemType.ItemTypeRefresh.toString())) {
            return ItemType.ItemTypeRefresh;
        } else if (str.equalsIgnoreCase(ItemType.ItemTypeSync.toString())) {
            return ItemType.ItemTypeSync;
        } else if (str.equalsIgnoreCase(ItemType.ItemTypeOptions.toString())) {
            return ItemType.ItemTypeOptions;
        } else if (str.equalsIgnoreCase(ItemType.ItemTypeExit.toString())) {
            return ItemType.ItemTypeExit;
        } else if (str.equalsIgnoreCase(ItemType.ItemTypeClose.toString())) {
            return ItemType.ItemTypeClose;
        } else if (str.equalsIgnoreCase(ItemType.ItemTypeLog.toString())) {
            return ItemType.ItemTypeLog;
        }
        return ItemType.ItemTypeUrl;
    }

    private boolean mNewMenu = false;
    private List<Item> mItems = null;
    private Map<MenuItem, Item> mItemMap = new HashMap<MenuItem, Item>();

    public synchronized List<Object> getMenuDescription() {
        List<Object> menuDescr = new ArrayList<Object>();
        if (mItems != null) {
            for (Item item: mItems) {
                Map<String, Object> itemDescr = new HashMap<String, Object>();
                itemDescr.put(item.title, item.url);
                menuDescr.add(itemDescr);
            }
        }
        return menuDescr;
    }

    public synchronized void setMenu(List<Map<String, String>> items) {
        mNewMenu = true;
        mItems = new ArrayList<Item>();
        
        for(Map<String, String> itemDescription: items) {
            String label = itemDescription.get("label");
            
            if (label == null) {
                Logger.E(TAG, "Menu item with label: <null>");
                continue;
            }
            
            if (label.equals("separator")) {
                continue;
            }
            
            String action = itemDescription.get("action");
            ItemType type = getItemType(action);

            Item item = new Item(label, action, type);
            mItems.add(item);
        }
    }

    public synchronized void enumerateMenu(Menu menu) {
        if (mNewMenu) {
            menu.clear();
            mItemMap.clear();
            for(Item item: mItems) {
                MenuItem menuItem = menu.add(item.title);
                mItemMap.put(menuItem, item);
            }
        }
    }

    public synchronized boolean onMenuItemSelected(MenuItem menuItem) {
        Item item = mItemMap.get(menuItem);
        if (item == null) {
            Logger.E(TAG, "Unknown MenuItem");
            return false;
        }

        ItemType type = item.type;
        if (!type.action()){
            MainView mainView = RhodesActivity.safeGetInstance().getMainView();
            mainView.navigate(RhodesService.getInstance().normalizeUrl(item.url), mainView.activeTab());
        }
        return true;
    }
}
