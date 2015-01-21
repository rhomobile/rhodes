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
package com.rhomobile.rhodes.ui;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.rhomobile.rhodes.BaseActivity;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhoConf;
import com.rhomobile.rhodes.RhoLogConf;
import com.rhomobile.rhodes.RhodesService;
import com.rhomobile.rhodes.mainview.MainView;

import android.app.Dialog;
import android.content.Context;
import android.database.DataSetObserver;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewTreeObserver.OnScrollChangedListener;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.AbsListView;
import android.widget.AbsListView.OnScrollListener;
import android.widget.Adapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.TextView;

public class LogViewDialog extends BaseActivity implements OnClickListener {
    private static final String TAG = LogViewDialog.class.getSimpleName();
    private static final int LOG_INIT_SIZE = 16384;
    private static final int LOG_CHUNK_SIZE = 4096;
    private static final int LINES_TO_TRIGGER_PRELOAD = 50;
    

    class LogAdapter extends BaseAdapter {

        private volatile boolean preloading = false;
        private volatile int referencePos;
        private volatile int preloadPos = -1;
        private volatile boolean fullyLoaded = false;
        private boolean notifyOnUpdate = false;

        synchronized void setPreloading(boolean working) {
            this.preloading = working;
        }

        synchronized boolean isPreloading() {
            return this.preloading;
        }

        private ArrayList<String> logList = new ArrayList<String>();
        private long idShift = 0;

        private List<String> preloadData(int sizeToPreload) {
            String strLog;
            try {
                int fullSize = RhoLogConf.getLogFileSize();
                if (fullSize <= sizeToPreload) {
                    strLog = RhoLogConf.getLogText();
                    fullyLoaded = true;
                } else {
                    
                    Logger.T(TAG, "preloadPos: " + preloadPos);
                    
                    
                    if (referencePos == 0) {
                        referencePos = RhoLogConf.getLogTextPos();
                    }
                    
                    if (preloadPos == -1) {
                        preloadPos = fullSize;
                    }
                    
                    int startPos = preloadPos - sizeToPreload;
                    strLog = RhoLogConf.getLogFileText(startPos, sizeToPreload, referencePos);
                    
                    fullyLoaded = strLog.length() == 0;
                    
                    preloadPos -= strLog.length();

                    //Logger.T(TAG, "Log has been preloaded, size: " + (strLog.length() / 1024) + "K");
                }
            } catch (Throwable err) {
                strLog = err.getMessage();
                if (strLog == null) {
                    strLog = err.toString();
                }
            }
            return (strLog != null) ? Arrays.asList(strLog.split("\n")) : null;
        }

        void reset() {
            Logger.T(TAG, "Foreground data reset!");
            setPreloading(true);
            logList.clear();
            idShift = 0;
            referencePos = 0;
            preloadPos = -1;
            logList.addAll(preloadData(LOG_INIT_SIZE));
            notifyDataSetChanged();
            setPreloading(false);
            
            Logger.T(TAG, "Data is preloaded: preloadPos: " + preloadPos);
        }
        
        void clear() {
            logList.clear();
            idShift = 0;
            referencePos = 0;
            preloadPos = -1;
            notifyDataSetChanged();
        }

        void preloadAtBackground() {
            if (fullyLoaded)
                return;
            
            setPreloading(true);
            Thread bgThread = new Thread() {
                @Override
                public void run() {
                    final List<String> preloadList = preloadData(LOG_CHUNK_SIZE);
                    logContainer.post(new Runnable() {
                        @Override
                        public void run() {
                            
                            // Concatinate broken line
                            preloadList.set(preloadList.size() - 1, preloadList.get(preloadList.size() - 1) + logList.get(0));
                            logList.remove(0);
                            
                            logList.addAll(0, preloadList);
                            idShift += preloadList.size();
                            if (notifyOnUpdate) {
                                notifyDataSetChanged();
                            }
                            setPreloading(false);
                        }
                    });
                }
            };
            bgThread.start();
        }

        @Override
        public int getCount() {
            return logList.size() > 0 ? Integer.MAX_VALUE : 0;
        }
        
        public int getCachedCount() {
            return logList.size();
        }

        @Override
        public Object getItem(int position) {
            return null;
        }

        @Override
        public long getItemId(int position) {
            return logList.size() - position + idShift;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            TextView logLineView = (convertView == null) ? new TextView(LogViewDialog.this) : (TextView) convertView;
            int idx = logList.size() - (Integer.MAX_VALUE - position);
            if (idx >= 0) {
                logLineView.setText(logList.get(idx));
            } else {
                logLineView.setText("");
                notifyOnUpdate = true;
            }
            return logLineView;
        }

        @Override
        public int getItemViewType(int position) {
            return Adapter.IGNORE_ITEM_VIEW_TYPE;
        }

        @Override
        public int getViewTypeCount() {
            return 1;
        }

        @Override
        public boolean hasStableIds() {
            return true;
        }

        @Override
        public boolean isEmpty() {
            return logList.isEmpty();
        }

        @Override
        public boolean areAllItemsEnabled() {
            return true;
        }

        @Override
        public boolean isEnabled(int position) {
            return true;
        }

    };

    private Button refreshButton;
    private Button clearButton;
    private Button sendButton;

    private ListView logContainer;
    private LogAdapter logAdapter;
    
    private boolean firstResume = true;

    @Override
    protected void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        
        Logger.T(TAG, "onCreate");

        //getWindow().requestFeature(Window.FEATURE_NO_TITLE);

        setContentView(R.layout.logview);
        refreshButton = (Button) findViewById(R.id.logviewRefreshButton);
        clearButton = (Button) findViewById(R.id.logviewClearButton);
        sendButton = (Button) findViewById(R.id.logviewSendButton);

        refreshButton.setOnClickListener(this);
        clearButton.setOnClickListener(this);
        sendButton.setOnClickListener(this);

        logContainer = (ListView) findViewById(R.id.logContainer);

        logContainer.setAdapter(logAdapter = new LogAdapter());
        logContainer.setOnScrollListener(new OnScrollListener() {
            @Override
            public void onScroll(AbsListView view, int firstVisibleItem, int visibleItemCount, int totalItemCount) {
                if (logAdapter.isEmpty()) {
                    return;
                }
                
                if ((logAdapter.getCachedCount() - (Integer.MAX_VALUE - firstVisibleItem)) <= LINES_TO_TRIGGER_PRELOAD && !logAdapter.isPreloading()) {
                    logAdapter.preloadAtBackground();
                }
            }

            @Override
            public void onScrollStateChanged(AbsListView view, int scrollState) {
            }

        });
        logAdapter.reset();
        logContainer.setSelection(logAdapter.getCount());

    }
    
    @Override
    protected void onResume() {
        super.onResume();
        
        if(firstResume) {
            logContainer.setSelection(logAdapter.getCount());
            firstResume = false;
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        switch (keyCode) {
        case KeyEvent.KEYCODE_BACK:
            logAdapter.reset();
            logContainer.setSelection(logAdapter.getCount());
        }

        return super.onKeyDown(keyCode, event);
    }

    public void onClick(View view) {
        switch (view.getId()) {
        case R.id.logviewRefreshButton:
            logAdapter.reset();
            logContainer.setSelection(logAdapter.getCount());
            break;
        case R.id.logviewClearButton:
            RhoLogConf.clearLog();
            logAdapter.clear();
            break;
        case R.id.logviewSendButton:
            RhoLogConf.sendLog();
            break;
        }
    }
}
