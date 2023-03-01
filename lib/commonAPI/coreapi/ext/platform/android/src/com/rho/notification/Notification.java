package com.rho.notification;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.app.Dialog;
import android.app.NotificationManager;

import com.rhomobile.rhodes.osfunctionality.AndroidFunctionalityManager;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.res.Resources;
import android.graphics.BitmapFactory;
import android.graphics.drawable.Drawable;
import android.os.Build;
import android.app.Notification.Builder;
import android.view.Gravity;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;
import android.util.DisplayMetrics;
import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.R;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.RhodesApplication;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.util.ContextFactory;
import com.rhomobile.rhodes.util.PerformOnUiThread;

import android.annotation.TargetApi;

public class Notification {
    private static final String TAG = Notification.class.getSimpleName();

    private static final int DLG_MAIN_VIEW_ID = 1;
    
    static final String NOTIFICATION_ID = "nitification_id";
    static final String NOTIFICATION_CHANEL_ID = "rho_notification_chanel_id";
    static final String NOTIFICATION_CHANEL_NAME = "Rhodes notifications chanel";

    int id;
    IMethodResult result;
    String title;
    String message;
    ArrayList<ActionData> actions = new ArrayList<ActionData>();
    int iconResourceId;
    String iconPath;
    List<String> kinds = new ArrayList<String>();
    Context ctx = null;
    
    Dialog dialog;

    public Notification(int id, Map<String, Object> props, IMethodResult result, Context c) {
        this.id = id;
        this.result = result;
        
        if(c == null)
            ctx = ContextFactory.getUiContext();
        else
            ctx = c;
        
        String iconName = null;

        Object titleObj = props.get(NotificationSingleton.HK_TITLE);
        if (titleObj != null && (titleObj instanceof String))
            title = (String) titleObj;

        Object messageObj = props.get(NotificationSingleton.HK_MESSAGE);
        if (messageObj != null && (messageObj instanceof String))
            message = (String) messageObj;

        Object iconObj = props.get(NotificationSingleton.HK_ICON);
        if (iconObj != null && (iconObj instanceof String))
            iconName = (String) iconObj;

        Object buttonsObj = props.get(NotificationSingleton.HK_BUTTONS);
        if (buttonsObj != null && (buttonsObj instanceof List<?>)) {

            List<Object> btns = (List<Object>) buttonsObj;
            for (int i = 0; i < btns.size(); ++i) {
                String itemId = null;
                String itemTitle = null;

                Object btnObj = btns.get(i);
                Logger.D(TAG, "button object=" + btnObj.getClass().getName());
                if (btnObj instanceof String) {
                    itemId = (String) btnObj;
                    itemTitle = (String) btnObj;
                }
                else if (btnObj instanceof Map<?, ?>) {
                    Map<String, Object> btnHash = (Map<String, Object>) btnObj;
                    Object btnIdObj = btnHash.get("id");
                    if (btnIdObj != null && (btnIdObj instanceof String)) {
                        itemId = (String) btnIdObj;
                    }
                    Object btnTitleObj = btnHash.get("title");
                    if (btnTitleObj != null && (btnTitleObj instanceof String)) {
                        itemTitle = (String) btnTitleObj;
                    }
                }

                if (itemId == null || itemTitle == null) {
                    Logger.E(TAG, "Incomplete button item");
                    continue;
                }

                actions.add(new ActionData(i, itemId, itemTitle, result));
            }
        }
//        else
//        {
//            Logger.W(TAG, "Unknown button description object. Will not show dialog without buttons");
//            throw new RuntimeException("Unknown button description object. Will not show dialog without buttons");
//        }

        if (iconName != null)
        {
            Resources res = ctx.getResources();
            if (iconName.equalsIgnoreCase("alert")) {
                iconResourceId = android.R.drawable.ic_dialog_alert;
            }
            else if (iconName.equalsIgnoreCase("question")) {
                iconResourceId = R.drawable.alert_question;
            }
            else if (iconName.equalsIgnoreCase("info")) {
                iconResourceId = android.R.drawable.ic_dialog_info;
            }
            else {
                iconResourceId = -1;
                iconPath = RhoFileApi.normalizePath(iconName);
            }
        }
        else {
            iconResourceId = -1;
        }
        Object kindsObj = props.get(NotificationSingleton.HK_TYPES);
        if (kindsObj != null && kindsObj instanceof List<?>) {
            for (Object kind: (List<?>)kindsObj) {
                if (kind != null && kind instanceof String) {
                    kinds.add((String)kind);
                }
            }
        }
        else {
            kinds.add(INotificationSingleton.TYPE_DIALOG);
            kinds.add(INotificationSingleton.TYPE_NOTIFICATION);
        }
    }
    
    public void showDialog() {
        RhodesApplication.runWhen(RhodesApplication.AppState.AppActivated, new RhodesApplication.StateHandler(true) {
            @Override public void run() {
                PerformOnUiThread.exec(new Runnable() {
                    @Override public void run() {
                            try {
                                doShowDialog();
                            } catch (Throwable e) {
                                Logger.E(TAG, e);
                                result.setError(e.getLocalizedMessage());
                            }
                        }
                    });
            }
        });
        
        
    }
    
    private synchronized void doShowDialog() {
        if (result == null) {
            Logger.T(TAG, "Notification action is already processed - no dialog will be shown");
            return;
        }
        
        Logger.T(TAG, "Dialog: title: " + title + ", message: " + message + ", buttons: " + actions.size());
        
        if(ctx == null)        
           ctx = ContextFactory.getUiContext();
        int nTopPadding = 10;

        dialog = new Dialog(ctx);
        if (title == null || title.length() == 0)
        {
            dialog.getWindow();
            dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        }
        else
        {
            dialog.setTitle(title);
            if (Build.VERSION.SDK_INT < Build.VERSION_CODES.HONEYCOMB) nTopPadding = 0;
        }

        dialog.setCancelable(false);
        dialog.setCanceledOnTouchOutside(false);

        LinearLayout main = new LinearLayout(ctx);
        main.setOrientation(LinearLayout.VERTICAL);
        main.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        main.setPadding(10, nTopPadding, 10, 10);
        main.setId(DLG_MAIN_VIEW_ID);

        LinearLayout top = new LinearLayout(ctx);
        top.setOrientation(LinearLayout.HORIZONTAL);
        top.setGravity(Gravity.CENTER);
        top.setPadding(10, nTopPadding, 10, 10);
        top.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        main.addView(top);

        if (iconResourceId != -1)
        {
            ImageView imgView = new ImageView(ctx);
            imgView.setImageDrawable(ctx.getResources().getDrawable(iconResourceId));
            imgView.setScaleType(ImageView.ScaleType.CENTER);
            imgView.setPadding(10, nTopPadding, 10, 10);
            top.addView(imgView);
        }
        else if (iconPath != null) {
            ImageView imgView = new ImageView(ctx);
            imgView.setImageDrawable(Drawable.createFromPath(iconPath));
            imgView.setScaleType(ImageView.ScaleType.CENTER);
            imgView.setPadding(10, nTopPadding, 10, 10);
            top.addView(imgView);
        }

        if (message != null)
        {
            TextView textView = new TextView(ctx);
            textView.setText(message);
            textView.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
            textView.setGravity(Gravity.CENTER);
            top.addView(textView);
        }

        if (actions.size() > 0) {
            LinearLayout bottom = new LinearLayout(ctx);
            bottom.setOrientation(actions.size() > 3 ? LinearLayout.VERTICAL : LinearLayout.HORIZONTAL);
            bottom.setGravity(Gravity.CENTER);
            bottom.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
            main.addView(bottom);
    
            for (ActionData btn: actions)
            {
                Button button = new Button(ctx);
                OnClickListener clickListener = new DialogActionListener(dialog);
                float btnWeight = 1f/actions.size() ;
                DisplayMetrics metrics = RhodesActivity.safeGetInstance().getContext().getResources().getDisplayMetrics();
            	float dp = 8f;
            	float fpixels = metrics.density * dp;
            	int pixels = (int) (fpixels + 0.5f);
                if(actions.size() == 3){
                	button.setMaxLines(2);
                	button.setTextSize(12);
                	button.setText(btn.title);
                }else{
                	button.setText(btn.title);
                }
                button.setTag(btn);
                button.setOnClickListener(clickListener);
                //Button text cutoff on Notification pop up
                if(actions.size() > 3){
                button.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT, 1));
                }
                else{
                	button.setLayoutParams(new LinearLayout.LayoutParams(0, LayoutParams.MATCH_PARENT, btnWeight));
                }
                bottom.addView(button);
            }
        }

        NotificationManager notificationManager = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.cancel(id);

        dialog.setContentView(main);
        dialog.show();
    }

    public void showNotification() {
        Logger.T(TAG, "Notification: title: " + title + ", message: " + message);
        
        if(ctx == null) ctx = ContextFactory.getContext();


        Builder builder = AndroidFunctionalityManager.getAndroidFunctionality().getNotificationBuilder(ctx,NOTIFICATION_CHANEL_ID,NOTIFICATION_CHANEL_NAME);

        builder.setTicker(message);
        if (title != null) {
            builder.setContentTitle(title);
        }
        else {
            builder.setContentTitle(ctx.getText(R.string.app_name));
        }
        builder.setContentText(message);
        builder.setAutoCancel(true);
        
        if (iconResourceId != -1 || iconPath != null) {
            if (iconResourceId != -1) {
                builder.setLargeIcon(BitmapFactory.decodeResource(ctx.getResources(), iconResourceId));
            }
            else {
                InputStream iconStream = RhoFileApi.open(iconPath);
                builder.setLargeIcon(BitmapFactory.decodeStream(iconStream));
                try {
                    iconStream.close();
                } catch (Throwable e) {
                    Logger.E(TAG, e);
                }
            }
        }

        builder.setSmallIcon(R.drawable.ic_notification);

        builder.setContentIntent(PendingIntent.getActivity(ctx, id, new Intent(ctx, RhodesActivity.class), 
        PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE));

        builder.setDefaults(android.app.Notification.DEFAULT_VIBRATE | android.app.Notification.DEFAULT_SOUND);

        if (kinds.contains(INotificationSingleton.TYPE_NOTIFICATION_DIALOG)) {
            for (ActionData action: actions) {
                
                Logger.T(TAG, "Adding action: " + action.index + ", " + action.id + ", " + action.title);
                
                Intent actionIntent = new Intent(ctx, NotificationIntentService.class);
                
                //Next two lines are needed in order to make intents unique
                actionIntent.setAction(action.id);
                actionIntent.addCategory(String.valueOf(id));
                
                actionIntent.putExtra(INotificationSingleton.HK_BUTTON_INDEX, action.index);
                actionIntent.putExtra(NOTIFICATION_ID, id);
    
                
                int resId = R.drawable.ic_action_star;
                if (action.id.equalsIgnoreCase("accept")) {
                    resId = R.drawable.ic_action_accept;
                }
                else if (action.id.equalsIgnoreCase("cancel")) {
                    resId = R.drawable.ic_action_cancel;
                }
                
                builder.addAction(resId, action.title, PendingIntent.getService(ctx, id, actionIntent, 
                    PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE));
            }
        }
        
        NotificationManager notificationManager = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
        android.app.Notification notification = builder.build();
        notificationManager.notify(id, notification);
    }

    synchronized void onAction(int actionIdx) {
        final ActionData action = actions.get(actionIdx);
        final IMethodResult result = this.result;

        this.result = null;
        
        PerformOnUiThread.exec(new Runnable() {
            @Override public void run() {
                Map<String, Object> data = new HashMap<String, Object>();
                data.put(NotificationSingleton.HK_BUTTON_ID, action.id);
                data.put(NotificationSingleton.HK_BUTTON_TITLE, action.title);
                data.put(NotificationSingleton.HK_BUTTON_INDEX, action.index + "");
                
                result.set(data);
            }
        });
    }
    
    public void showForegroundToast() {
        PerformOnUiThread.exec(new Runnable() {
            @Override public void run() {
                Toast.makeText(ContextFactory.getContext(), message, Toast.LENGTH_SHORT).show();
            }
        });
    }
    
    public void dismiss() {
        if (dialog != null) {
            PerformOnUiThread.exec(new Runnable() {
                @Override public void run() {
                    doDismissDialog();
                }
            });
        }

        if(ctx == null)
            ctx = ContextFactory.getContext();
        NotificationManager notificationManager = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.cancel(id);
    }
    
    private synchronized void doDismissDialog() {
        if (dialog != null) {
            dialog.dismiss();
            dialog = null;
            result.release();
            result = null;
        }
    }

    public boolean isDialogNeeded() {
        return kinds.contains(INotificationSingleton.TYPE_DIALOG) || kinds.contains(INotificationSingleton.TYPE_NOTIFICATION_DIALOG);
    }
    
    public boolean isForegroundToastNeeded() {
        boolean foregroundDialog = RhodesApplication.canHandleNow(RhodesApplication.AppState.AppActivated) && isDialogNeeded();
        return kinds.contains(INotificationSingleton.TYPE_TOAST) && !foregroundDialog;
    }
    
    public boolean isNotificationAreaNeeded() {
        return (kinds.contains(INotificationSingleton.TYPE_NOTIFICATION) || kinds.contains(INotificationSingleton.TYPE_NOTIFICATION_DIALOG));
    }

    private static class ActionData
    {
        public int index;
        public String id;
        public String title;
        public IMethodResult result;
        public ActionData(int index, String id, String title, IMethodResult result) {
            this.index = index;
            this.id = id;
            this.title = title;
            this.result = result;
        }
    };

    private static class DialogActionListener implements OnClickListener
    {
        private Dialog dialog;
        
        public DialogActionListener(Dialog dialog) {
            this.dialog = dialog;
        }

        public synchronized void onClick(View button)
        {
            ActionData buttonData = (ActionData)button.getTag();
            
            Map<String, Object> data = new HashMap<String, Object>();
            data.put(NotificationSingleton.HK_BUTTON_ID, buttonData.id);
            data.put(NotificationSingleton.HK_BUTTON_TITLE, buttonData.title);
            data.put(NotificationSingleton.HK_BUTTON_INDEX, buttonData.index + "");
            
            if (buttonData.result != null) {
                buttonData.result.set(data);
            }
            
            dialog.dismiss();
        }
    };
}
