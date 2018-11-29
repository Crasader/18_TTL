/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.net.URLConnection;

import org.cocos2dx.lib.Cocos2dxActivity;
import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.content.Intent;
import android.net.Uri;
import android.os.*;
import android.util.Log;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.os.Bundle;

public class AppActivity extends Cocos2dxActivity
{

	ProgressDialog mProgressDlg = null;
	private int downLoadFileSize;

	public native void setNetworkState(int state);

	public native void setbatteryLevel(int batteryL);

	public String room_id = "room_id";

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		Intent intt = getIntent();

		if ((getIntent().getFlags() & Intent.FLAG_ACTIVITY_BROUGHT_TO_FRONT) != 0) {
			finish();
			return;
		}
		IntentFilter filter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
		IntentFilter netstatefilter = new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION);

		BroadcastReceiver netstatereceiver = new BroadcastReceiver() {
			// @Override
			public void onReceive(Context context, Intent intent)
			{

				ConnectivityManager manager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
				manager.getActiveNetworkInfo();

			}
		};
		registerReceiver(netstatereceiver, netstatefilter);
		registerReceiver(netstatereceiver, filter);

		try {
			String action = intt.getAction();
			if (Intent.ACTION_VIEW.equals(action)) {
				Uri data = intt.getData();
				if (data != null) {
					String room_id = data.getQueryParameter("room_id");
					Native.onLoginWithRoomNO(room_id);
				}
			}
		} catch (Exception e) {
			Log.e("cocos2d-x", e.getMessage());
		}

//		try {
//			Bundle bnd = intt.getExtras();
//			if (bnd != null) {
//				Log.e("cocos2d-x", "bnd ok");
//			}
//			for (String key : bnd.keySet()) {
//				Log.e("cocos2d-x", "Key=" + key);
//				Object obj = bnd.get(key);
//				if (obj != null) {
//					Log.e("cocos2d-x", " content=" + bnd.get(key));
//				}
//			}
//		} catch (Exception e) {
//			Log.e("cocos2d-x", e.getMessage());
//		}

	}

	@Override
	protected void onNewIntent(Intent intent)
	{
		super.onNewIntent(intent);
		setIntent(intent);
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
	}

	private void openFile(File file)
	{
		Intent intent = new Intent();
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		intent.setAction(android.content.Intent.ACTION_VIEW);
		intent.setDataAndType(Uri.fromFile(file), "application/vnd.android.package-archive");
		startActivity(intent);
		this.finish();
	}

	public void downFile(String url, String path) throws IOException
	{
		String filename = url.substring(url.lastIndexOf("/") + 1);
		URL myURL = new URL(url);
		URLConnection conn = myURL.openConnection();
		conn.connect();
		InputStream inputstream = conn.getInputStream();
		final File file = new File(path + filename);
		if (inputstream == null)
			throw new RuntimeException("stream is null");
		FileOutputStream fileOutputStream = new FileOutputStream(file);
		byte buf[] = new byte[512];
		downLoadFileSize = 0;
		sendMsg(0);
		do {
			int numread = inputstream.read(buf);
			if (numread == -1) {
				break;
			}
			fileOutputStream.write(buf, 0, numread);
			downLoadFileSize += numread;
			sendMsg(1);
		} while (true);
		sendMsg(2);
		try {
			fileOutputStream.close();
			inputstream.close();
		} catch (Exception ex) {
			Log.e("tag", "error: " + ex.getMessage(), ex);
		}
	}

	@SuppressLint("HandlerLeak")
	private Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg)
		{
			if (!Thread.currentThread().isInterrupted()) {
				switch (msg.what) {
					case 1 :
						if (mProgressDlg != null) {
							mProgressDlg.setProgress(downLoadFileSize / 1024);
						}

						break;
					case 2 :
						String absPath = Environment.getExternalStorageDirectory() + "/Android/data/" + getApplicationInfo().packageName + "/files/";
						openFile(new File(absPath/* + filename */));
						break;
					case 3 :
						break;
					case -1 :
						// Cocos2dxHelper.ExitUpdate(0);
						break;
					case 4 :
						break;
				}
			}

		}
	};

	private void sendMsg(int flag, String url, int size, String info, int isUpdate)
	{
		Message msg = new Message();
		msg.what = flag;
		Bundle bundle = new Bundle();
		bundle.putInt("size", size);
		bundle.putString("url", url);
		bundle.putInt("isUpdate", isUpdate);
		bundle.putString("info", info);
		msg.setData(bundle);
		handler.sendMessage(msg);
	}

	private void sendMsg(int flag)
	{
		Message msg = new Message();
		msg.what = flag;
		handler.sendMessage(msg);
	}

	@Override
	public void onPause()
	{
		super.onPause();
	}

	@Override
	public void onResume()
	{
		super.onResume();

		Intent intt = getIntent();
		try {
			String action = intt.getAction();
			if (Intent.ACTION_VIEW.equals(action)) {
				Uri data = intt.getData();
				if (data != null) {
					String room_id = data.getQueryParameter("room_id");
					Native.onLoginWithRoomNO(room_id);
				}
			}
		} catch (Exception e) {
			Log.e("cocos2d-x", e.getMessage());
		}
	}

}
