package org.cocos2dx.cpp;

import java.io.File;
import java.io.IOException;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxHelper;

import com.guojiang.niufantian.wxapi.WXEntryActivity;

import android.content.Intent;
import android.media.MediaRecorder;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;

public class Native extends Cocos2dxHelper
{
	public static native void WxLoginGetAccessToken(String kUrl);

	public static native void WxLoginGetFailToken(String Error);

	public static native void onLoginWithRoomNO(String RoomInfo);

	public static native void onWxShareResult(String Error);

	public static native String getAppID();

	private static final String TAG = "Native";

	public static void LoginWX(String APP_ID, String AppSecret)
	{
		Intent intent = new Intent(Cocos2dxActivity.getContext(), WXEntryActivity.class);
		intent.putExtra(WXEntryActivity.ReqAppID, APP_ID);
		intent.putExtra(WXEntryActivity.ReqWxLogin, "wxlogin");
		Cocos2dxActivity.getContext().startActivity(intent);
	}

	public static void onLoginError(int errorCode)
	{
		// 42001 access_token expired access_token超时
		// 42002 refresh_token expired refresh_token超时
		// 42003 code expired code超时
		Log.e("onLoginError", "errorcode = " + errorCode);
		if (errorCode == 42001 || errorCode == 42002 || errorCode == 42003) {
			Intent intent = new Intent(Cocos2dxActivity.getContext(), WXEntryActivity.class);
			intent.putExtra(WXEntryActivity.ReqWxLogin, "wxlogin");
			Cocos2dxActivity.getContext().startActivity(intent);
		}
	}

	public static void ShareImageWX(String ImgPath, int nType)
	{
		Intent intent = new Intent(Cocos2dxActivity.getContext(), WXEntryActivity.class);
		intent.putExtra(WXEntryActivity.ReqWxShareImg, "ReqWxShareImg");
		intent.putExtra("ImgPath", ImgPath);
		intent.putExtra("ShareType", nType);
		Cocos2dxActivity.getContext().startActivity(intent);
	}

	public static void ShareTextWX(String text, int nType)
	{
		Intent intent = new Intent(Cocos2dxActivity.getContext(), WXEntryActivity.class);
		intent.putExtra(WXEntryActivity.ReqWxShareTxt, "ReqWxShareTxt");
		intent.putExtra("ShareText", text);
		intent.putExtra("ShareType", nType);
		Cocos2dxActivity.getContext().startActivity(intent);
	}

	public static void ShareUrlWX(String url, String title, String desc, int nType)
	{
		Intent intent = new Intent(Cocos2dxActivity.getContext(), WXEntryActivity.class);
		intent.putExtra(WXEntryActivity.ReqWxShareUrl, "ReqWxShareUrl");
		intent.putExtra("ShareUrl", url);
		intent.putExtra("ShareTitle", title);
		intent.putExtra("ShareDesc", desc);
		intent.putExtra("ShareType", nType);
		Cocos2dxActivity.getContext().startActivity(intent);
	}

	public static void showWebView(String kUrl)
	{
		Log.d("showWebView", kUrl);
		Uri uri = Uri.parse(kUrl);
		Intent it = new Intent(Intent.ACTION_VIEW, uri);
		Cocos2dxActivity.getContext().startActivity(it);
	}

	public static void versionUpdate(final String url, final String info, final int size, final int isUpdate)
	{
		Log.d("versionUpdate", "1111");
		QYFun.getInstance().setContext(Cocos2dxActivity.getContext());

		Log.d("versionUpdate", "2222");
		new Thread() {
			@Override
			public void run()
			{
				Log.d("versionUpdate", "3333");
				QYFun.getInstance().showVersionUpdate(url, info, size, isUpdate);
			}
		}.start();

	}

	private static MediaRecorder recorder = null;
	private static String filePath = null;

	public static void startSoundRecord(String SoundFileName)
	{
		String SoundFilePath = Environment.getExternalStorageDirectory().getAbsolutePath();

		if (filePath != null) {
			File file = new File(filePath);
			if (file != null && file.exists()) {
				file.delete();
			}
		}
		filePath = SoundFilePath + "/" + SoundFileName;
		recorder = new MediaRecorder();
		recorder.setAudioSource(MediaRecorder.AudioSource.MIC);
		recorder.setOutputFormat(MediaRecorder.OutputFormat.RAW_AMR);
		recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
		recorder.setOutputFile(SoundFilePath + "/" + SoundFileName);
		try {
			recorder.prepare();//
			recorder.start();//
		} catch (IllegalStateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public static String stopSoundRecord()
	{
		try {
			recorder.setOnErrorListener(null);  
			recorder.setOnInfoListener(null);    
			recorder.setPreviewDisplay(null);  
			recorder.stop();
			recorder.release();
			recorder = null;
		}catch(Exception e) {
			e.printStackTrace();
			return "";
		}
		return filePath;
	}
}
