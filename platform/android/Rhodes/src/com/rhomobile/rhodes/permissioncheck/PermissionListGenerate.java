package com.rhomobile.rhodes.permissioncheck;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PermissionInfo;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.view.Gravity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.rhomobile.rhodes.RhoConf;


import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.function.Consumer;

public class PermissionListGenerate {
    private Context mContext;
    private Activity mActivity;
    private AlertDialog mPermissionsDialog = null;
    private final Map<String, ArrayList<String>> permissionsMap = new HashMap<String, ArrayList<String>>();
    private AlertDialog.Builder ad;
    final static private Integer MY_PERMISSIONS_REQUEST = 343457842;
    public PermissionListGenerate(Context context, Activity activity){
        this.mContext = context;
        this.mActivity = activity;

        this.ad = new AlertDialog.Builder(context);

        try {
            init();
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    private void init() throws PackageManager.NameNotFoundException {
        PackageInfo info = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(),PackageManager.GET_PERMISSIONS);
        for(String p :info.requestedPermissions){
            PermissionInfo permissionInfo = mContext.getPackageManager().getPermissionInfo(p, 0);
            int protectionLevel = permissionInfo.protectionLevel;
            if (protectionLevel == PermissionInfo.PROTECTION_DANGEROUS)
                addToListPermission(p);
        }
    }



    public void addToListPermission(String permissionName){
        String nameKey = formatName(permissionName);
        if(permissionsMap.get(nameKey) != null)
        {
            ArrayList<String> i = permissionsMap.get(nameKey);
            if(!i.contains(permissionName)) {
                i.add(permissionName);
                permissionsMap.remove(nameKey);
                permissionsMap.put(nameKey, i);
            }
        }else{
            ArrayList<String> i = new ArrayList<String>();
            i.add(permissionName);
            permissionsMap.put(nameKey, i);
        }
    }


    @NonNull
    private String formatName(String name) {
        name = name.toLowerCase(Locale.ROOT);

        String[] splitNameToPoint = name.split("\\.");
        String[] splitName = splitNameToPoint[splitNameToPoint.length -1].split("_");

        StringBuilder returnName = new StringBuilder();
        for(int i = 0; i < splitName.length; i++){
            if(!splitName[i].equals("read") &&
               !splitName[i].equals("write"))
            {
                if(i != splitName.length -1)
                    returnName.append(" ");
                returnName.append(splitName[i]);
            }
        }
        return returnName.toString();
    }



    public boolean checkAllPermissionsStatus(){
        if(RhoConf.getInt("strict_permission_requirements") == 1){
            boolean returnStatus = true;
            for(Map.Entry<String, ArrayList<String>> entry : permissionsMap.entrySet()){
                ArrayList<String> entryValue = entry.getValue();
                for(String pNameManifest : entryValue){
                    if(!returnStatus)
                        break;
                    returnStatus = getPermissionStatus(pNameManifest);
                }
                if (!returnStatus)
                    break;
            }
            return returnStatus;
        }else
            return true;
    }

    private boolean getPermissionStatus(@NonNull String pNameManifest){
        return ContextCompat.checkSelfPermission(mContext, pNameManifest) == PackageManager.PERMISSION_GRANTED;
    }
    private boolean getPermissionStatus(@NonNull ArrayList<String> pNameManifest){
        boolean returnStatus = true;
        for (String value : pNameManifest) {
            returnStatus =  ContextCompat.checkSelfPermission(mContext, value) == PackageManager.PERMISSION_GRANTED;
        }
        return returnStatus;
    }



    public boolean isShowPermissionDialog(){
        return mPermissionsDialog != null && mPermissionsDialog.isShowing();
    }
    public void closePermissionsDialog(){
        if(mPermissionsDialog != null)
            mPermissionsDialog.cancel();
    }



    public void showPermissionList(){
        ad.setTitle("Permission check")
                .setCancelable(false);

        ////////////////////////////////////////////////////////////

        LinearLayout permissionLayout = new LinearLayout(mContext);
        permissionLayout.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        ));
        permissionLayout.setOrientation(LinearLayout.VERTICAL);
        permissionLayout.setBackgroundColor(Color.parseColor("#FFFFFF"));

        ////////////////////////////////////////////////////////////

        TextView textView = new TextView(mContext);
        textView.setTextSize(23);
        textView.setText("This app requires some additional permissions in order to make its job properly:");
        textView.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        ));

        ////////////////////////////////////////////////////////////

        View space = new View(mContext);
        space.setLayoutParams(new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                (int)convertDpToPixels(20)
        ));

        ////////////////////////////////////////////////////////////

        permissionLayout.addView(textView);
        permissionLayout.addView(space);

        for(Map.Entry<String, ArrayList<String>> entry : permissionsMap.entrySet()){
            permissionLayout.addView(permissionLine(entry.getValue()));
        }

        ad.setView(permissionLayout);
        mPermissionsDialog = ad.create();
        mPermissionsDialog.show();

    }
    @NonNull
    private LinearLayout permissionLine(@NonNull final ArrayList<String> pItem){
        LinearLayout layout = new LinearLayout(mContext);
        layout.setOrientation(LinearLayout.HORIZONTAL);
        layout.setBackgroundColor(Color.parseColor("#F3F3F3"));
        layout.setMinimumHeight(48);
        layout.setMinimumWidth(88);

        LinearLayout.LayoutParams lParams = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.MATCH_PARENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        );
        lParams.setMargins(3,3,3,3);
        layout.setLayoutParams(lParams);

        ////////////////////////////////////////////////////////////

        TextView tv = new TextView(mContext);

        LinearLayout.LayoutParams tvParams = new LinearLayout.LayoutParams(
                LinearLayout.LayoutParams.WRAP_CONTENT,
                LinearLayout.LayoutParams.WRAP_CONTENT
        );
        tvParams.setMargins(10,0,0,0);

        tv.setLayoutParams(tvParams);
        tv.setText(formatName(pItem.get(0)));
        tv.setTextSize(20);
        tv.setGravity(Gravity.CENTER);

        ////////////////////////////////////////////////////////////

        View space = new View(mContext);
        space.setLayoutParams(new LinearLayout.LayoutParams(0,0,1));

        ////////////////////////////////////////////////////////////

        layout.addView(tv);
        layout.addView(space);

        ////////////////////////////////////////////////////////////

        if(getPermissionStatus(pItem)){
            ImageView checkImg = new ImageView(mContext);

            LinearLayout.LayoutParams imgParams = new LinearLayout.LayoutParams(
                    (int)convertDpToPixels(24),
                    (int)convertDpToPixels(24)
            );
            imgParams.gravity = Gravity.CENTER;
            imgParams.rightMargin = (int) convertDpToPixels(10);

            checkImg.setLayoutParams(imgParams);
            Drawable dr = new CheckDrawable();
            checkImg.setImageDrawable(dr);

            layout.addView(checkImg);
        }
        else
        {
            Button btn = new Button(mContext);

            LinearLayout.LayoutParams btnParams = new LinearLayout.LayoutParams(
                    (int) convertDpToPixels(130),
                    (int) convertDpToPixels(40)
            );
            btnParams.setMargins(0,0,(int)convertDpToPixels(3),0);

            btn.setLayoutParams(btnParams);
            btn.setText("Settings...");
            btn.setTextColor(Color.parseColor("#FFFFFF"));
            btn.setTextSize(15);
            btn.setGravity(Gravity.CENTER);
            btn.setBackgroundColor(Color.parseColor("#5CD9FF"));
            btn.setOnClickListener(new View.OnClickListener(){
                @Override
                public void onClick(View v) {
                    ActivityCompat.requestPermissions(mActivity, pItem.toArray(new String[0]), MY_PERMISSIONS_REQUEST);
                    mPermissionsDialog.cancel();
                }
            });

            layout.addView(btn);

        }

        ////////////////////////////////////////////////////////////

        return layout;
    }



    private float convertDpToPixels(float dp) {
        return dp * mContext.getResources().getDisplayMetrics().density;
    }
}
