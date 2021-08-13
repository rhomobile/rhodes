package com.rho.camera;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Map;
import java.io.FileNotFoundException;
import android.content.ContentResolver;
import com.rhomobile.rhodes.util.ContextFactory;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import java.io.OutputStream;
import android.content.ContentValues;
import android.provider.MediaStore.Images;
import android.graphics.Matrix;
import android.content.ContentUris;

import com.rhomobile.rhodes.Logger;
import com.rhomobile.rhodes.RhodesActivity;
import com.rhomobile.rhodes.api.IMethodResult;
import com.rhomobile.rhodes.file.RhoFileApi;
import com.rhomobile.rhodes.ui.FileList;

public class CameraSingleton implements ICameraSingleton {
    private static final String TAG = CameraSingleton.class.getSimpleName();
    private int mId = 0;

    static int getCameraIndex(String id) {
        return Integer.valueOf(id.substring(7)).intValue();
    }

    static String getCameraId(int idx) {
        return "camera#" + String.valueOf(idx);
    }

    public int getCameraCount() {
        Logger.T(TAG, "getCameraCount");
        return android.hardware.Camera.getNumberOfCameras();
    }


    public CameraSingleton() {
        int camera_count = getCameraCount();
        for (int i = 0 ; i < camera_count; i++) {
            android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
            android.hardware.Camera.getCameraInfo(i, info);
            if (info.facing == android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK) {
                setDefaultIndex(i);
                return;
            }
        }

    }

    @Override
    public String getDefaultID() {
        return getCameraId(mId);
    }

    @Override
    public void setDefaultID(String id) {
        mId = getCameraIndex(id);
    }

    public void setDefaultIndex(int id) {
        mId = id;
    }

    @Override
    public void enumerate(IMethodResult result) {
        Logger.T(TAG, "enumerate");
        int cameraCount = getCameraCount();
        Logger.T(TAG, "Number of cameras: " + cameraCount);
        for (int i = 0 ; i < cameraCount; i++) {
            result.collect(getCameraId(i));
        }
        result.set();
    }

    @Override
    public void getCameraByType(String cameraType, IMethodResult result) {
        int cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK;
        if (cameraType.equalsIgnoreCase(ICameraSingleton.CAMERA_TYPE_FRONT)) {
            Logger.T(TAG, "Requesting front camera.");
            cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_FRONT;
        } else if (cameraType.equalsIgnoreCase(ICameraSingleton.CAMERA_TYPE_BACK)) {
            Logger.T(TAG, "Requesting back camera.");
            cameraTypeId = android.hardware.Camera.CameraInfo.CAMERA_FACING_BACK;
        } else {
            Logger.E(TAG, "Unknown camera type requested: " + cameraType);
            result.setArgError("Unknown camera type requested: " + cameraType);
            return;
        }

        int cameraCount = getCameraCount();
        int i;
        for (i = 0 ; i < cameraCount; i++) {
            android.hardware.Camera.CameraInfo info = new android.hardware.Camera.CameraInfo();
            android.hardware.Camera.getCameraInfo(i, info);
            if (info.facing == cameraTypeId) {
                result.set(getCameraId(i));
                return;
            }
        }
    }

    @Override
    public void choosePicture(Map<String, String> propertyMap, IMethodResult result) {
        CameraObject.CURRENT_SCREEN_AUTO_ROTATE_MODE = RhodesActivity.safeGetInstance().getScreenAutoRotateMode();
    	CameraObject.CURRENT_FULL_SCREEN_MODE = RhodesActivity.safeGetInstance().getFullScreenMode();


        if(propertyMap.get(ICameraSingleton.PROPERTY_USE_REAL_BITMAP_RESIZE) == null) {
            propertyMap.put(ICameraSingleton.PROPERTY_USE_REAL_BITMAP_RESIZE, "true");
        }
        if(propertyMap.get(ICameraSingleton.PROPERTY_USE_ROTATION_BITMAP_BY_EXIF) == null) {
            propertyMap.put(ICameraSingleton.PROPERTY_USE_ROTATION_BITMAP_BY_EXIF, "true");
        }

        Intent intent = null;
        String outputFormat = null;
        if(propertyMap.get(ICameraSingleton.PROPERTY_OUTPUT_FORMAT) == null){
            propertyMap.put(ICameraSingleton.PROPERTY_OUTPUT_FORMAT, ICameraSingleton.OUTPUT_FORMAT_IMAGE);
        }
        outputFormat = propertyMap.get(ICameraSingleton.PROPERTY_OUTPUT_FORMAT);

        CameraFactory factory = (CameraFactory)CameraFactorySingleton.getInstance();
        factory.getRhoListener().setMethodResult(result);
        factory.getRhoListener().setActualPropertyMap(propertyMap);
        RhodesActivity ra = RhodesActivity.safeGetInstance();
       


        /*
        Intent getIntent = new Intent(Intent.ACTION_GET_CONTENT);
        getIntent.setType("image/*");

        Intent pickIntent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        pickIntent.setType("image/*");

        Intent chooserIntent = Intent.createChooser(getIntent, "Select Image");
        chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, new Intent[] {pickIntent});

        intent =  chooserIntent;
        */

        intent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
        intent.setType("image/*");



        propertyMap.put("fromGallery", "true");
        String fileName = null;
        if(!propertyMap.containsKey(ICameraSingleton.PROPERTY_FILE_NAME)){
            fileName = "/sdcard/DCIM/Camera/";
        }
        else{
            fileName = propertyMap.get(ICameraSingleton.PROPERTY_FILE_NAME);
        }
        if (fileName != null && fileName.length() > 0) {
            if (outputFormat.equalsIgnoreCase(ICameraSingleton.OUTPUT_FORMAT_IMAGE)) {
                String tmpPath = getTemporaryLoc(fileName);
                if (tmpPath == null) {
                    throw new RuntimeException("Failed to access shared temporary folder");
                }

                Uri captureUri = Uri.fromFile(new File(tmpPath));
                propertyMap.put("captureUri", captureUri.toString());
                intent.putExtra(MediaStore.EXTRA_OUTPUT, captureUri);
            }
            else if(outputFormat.equalsIgnoreCase("dataUri")){
                intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(fileName + ".jpg")));
            }
            ra.startActivityForResult(intent, 0);
            propertyMap.put("ChoosePicture_Key", "ChoosePicture_Value");
        }
        else {
            result.setArgError("'fileName' parameter is missed");
        }
    }

    private String getTemporaryLoc(String targetPath) {
        if(Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            File externalRoot = Environment.getExternalStorageDirectory();
            if (! externalRoot.exists()){
                if (! externalRoot.mkdirs()){
                    Logger.E(TAG, "Failed to create directory: " + externalRoot);
                    return null;
                }
            }
            String filename = new File(targetPath).getName();
            return new File(externalRoot, filename).getAbsolutePath();
        } else {
            return null;
        }
    }

    /* @Override
    public void saveImageToDeviceGallery(String pathToImage, IMethodResult result) {
    // TODO Auto-generated method stub

    }
    */
    private static String insertImage(ContentResolver cr, String imageFullPath) {

            String filename =  imageFullPath.substring(imageFullPath.lastIndexOf("/")+1, imageFullPath.length());
            boolean isPNG = false;
            if (filename.indexOf("png") >= 0) {
                isPNG = true;
            }

            ContentValues values = new ContentValues();
            values.put(Images.Media.TITLE, filename);
            values.put(Images.Media.DISPLAY_NAME, filename);
            values.put(Images.Media.DESCRIPTION, filename);
            if (isPNG) {
                values.put(Images.Media.MIME_TYPE, "image/png");
            }
            else {
                values.put(Images.Media.MIME_TYPE, "image/jpeg");
            }

            // Add the date meta data to ensure the image is added at the front of the gallery
            values.put(Images.Media.DATE_ADDED, System.currentTimeMillis());
            values.put(Images.Media.DATE_TAKEN, System.currentTimeMillis());

            Uri url = null;
            String stringUrl = null;    /* value to be returned */

            try {
                url = cr.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);

                if (imageFullPath != null) {
                    OutputStream imageOut = cr.openOutputStream(url);
                    try {
                        //source.compress(Bitmap.CompressFormat.JPEG, 50, imageOut);
                        copyFile(imageFullPath, imageOut);
                    } finally {
                        imageOut.close();
                    }

                    long id = ContentUris.parseId(url);
                    // Wait until MINI_KIND thumbnail is generated.
                    Bitmap miniThumb = Images.Thumbnails.getThumbnail(cr, id, Images.Thumbnails.MINI_KIND, null);
                    // This is for backward compatibility.
                    storeThumbnail(cr, miniThumb, id, 50F, 50F,Images.Thumbnails.MICRO_KIND);
                } else {
                    cr.delete(url, null, null);
                    url = null;
                }
            } catch (Exception e) {
                Logger.T(TAG, "ERROR: can not insert image to gallery !");
                e.printStackTrace();
                if (url != null) {
                    cr.delete(url, null, null);
                    url = null;
                }
            }

            if (url != null) {
                stringUrl = url.toString();
            }

            return stringUrl;
        }

        /**
         * A copy of the Android internals StoreThumbnail method, it used with the insertImage to
         * populate the android.provider.MediaStore.Images.Media#insertImage with all the correct
         * meta data. The StoreThumbnail method is private so it must be duplicated here.
         * @see android.provider.MediaStore.Images.Media (StoreThumbnail private method)
         */

    private static Bitmap storeThumbnail(ContentResolver cr, Bitmap source, long id,
                float width, float height, int kind) {

            // create the matrix to scale it
            Matrix matrix = new Matrix();

            float scaleX = width / source.getWidth();
            float scaleY = height / source.getHeight();

            matrix.setScale(scaleX, scaleY);

            Bitmap thumb = Bitmap.createBitmap(source, 0, 0,
                source.getWidth(),
                source.getHeight(), matrix,
                true
            );

            ContentValues values = new ContentValues(4);
            values.put(Images.Thumbnails.KIND,kind);
            values.put(Images.Thumbnails.IMAGE_ID,(int)id);
            values.put(Images.Thumbnails.HEIGHT,thumb.getHeight());
            values.put(Images.Thumbnails.WIDTH,thumb.getWidth());

            Uri url = cr.insert(Images.Thumbnails.EXTERNAL_CONTENT_URI, values);

            try {
                OutputStream thumbOut = cr.openOutputStream(url);
                thumb.compress(Bitmap.CompressFormat.JPEG, 100, thumbOut);
                thumbOut.close();
                return thumb;
            } catch (FileNotFoundException ex) {
                return null;
            } catch (IOException ex) {
                return null;
            }

    }

    public static void copyImageFileToDeviceGallery(String imageFullPath) {

        String imageName = imageFullPath.substring(imageFullPath.lastIndexOf("/")+1, imageFullPath.length());
        String abspath = copyImageToDesired(imageFullPath, imageName);

        insertImage(RhodesActivity.getContext().getContentResolver(), abspath);
    }


    @Override
    public void copyImageToDeviceGallery(String pathToImage, IMethodResult result) {
        copyImageFileToDeviceGallery(pathToImage);
    }


    public void copyImageToDeviceGalleryOld(String pathToImage, IMethodResult result) {
        // TODO Auto-generated method stub
        String imageName = pathToImage.substring(pathToImage.lastIndexOf("/")+1, pathToImage.length());
        String abspath = copyImageToDesired(pathToImage, imageName);
        String strUri = null;
        try {
            strUri = MediaStore.Images.Media.insertImage(RhodesActivity.getContext().getContentResolver(), abspath, imageName, "Saving Image to Device Gallery through Camera");
        } catch (FileNotFoundException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

    private static void copyFile(String src, OutputStream dst) {

        InputStream finput= null;
        //FileOutputStream fout = null;
        try {
            finput= RhoFileApi.open(src);
            //fout = new FileOutputStream(mediafile);
            byte[] b = new byte[1024];
            int read = 0;
            while ((read = finput.read(b)) != -1) {
                //fout.write(b, 0, read);
                dst.write(b, 0, read);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if(finput != null){
                try {
                    finput.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            //if(fout != null){
            //    try {
            //        fout.close();
            //    } catch (IOException e) {
            //        e.printStackTrace();
            //    }
            //}
        }

    }

    private static String copyImageToDesired(String pathToImage, String imageName) {
        // TODO Auto-generated method stub
        File oldFile = new File(RhoFileApi.absolutePath(pathToImage));
        File mediafile  =  new File(RhoFileApi.getDbFilesPath(), imageName);

        if (oldFile.getAbsolutePath().equalsIgnoreCase(mediafile.getAbsolutePath())) {
            return RhoFileApi.absolutePath(pathToImage);
        }

        InputStream finput= null;
        FileOutputStream fout = null;
        try {
            finput= RhoFileApi.open(pathToImage);
            fout = new FileOutputStream(mediafile);
            byte[] b = new byte[1024];
            int read = 0;
            while ((read = finput.read(b)) != -1) {
                fout.write(b, 0, read);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if(finput != null){
                try {
                    finput.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            if(fout != null){
                try {
                    fout.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }

        return mediafile.getAbsolutePath();
    }
}
