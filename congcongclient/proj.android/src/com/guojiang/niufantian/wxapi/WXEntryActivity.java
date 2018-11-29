package com.guojiang.niufantian.wxapi;

import java.io.File;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;

import org.cocos2dx.cpp.Native;
import org.cocos2dx.cpp.Util;

import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.modelmsg.SendAuth;

import com.tencent.mm.sdk.modelmsg.SendMessageToWX;
import com.tencent.mm.sdk.modelmsg.WXAppExtendObject;
import com.tencent.mm.sdk.modelmsg.WXImageObject;
import com.tencent.mm.sdk.modelmsg.WXMediaMessage;
import com.tencent.mm.sdk.modelmsg.WXTextObject;
import com.tencent.mm.sdk.modelmsg.WXWebpageObject;

import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler
{

	public static String Tag = "WXEntryActivity";
	public static boolean bLogonWX = false;
	public static String APP_ID = null;
	private static IWXAPI wxapi;

	private static final int SceneSession = 0;
	private static final int SceneTimeline = 1;

	public static String ReqAppID = "AppID";
	public static String ReqWxLogin = "ReqWxLogin";
	public static String ReqWxShareImg = "ReqWxShareImg";
	public static String ReqWxShareTxt = "ReqWxShareTxt";
	public static String ReqWxShareUrl = "ReqWxShareUrl";
	public static String ReqWXPay = "ReqWXPay";

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		Log.d(Tag, "onCreate");

		Intent intent = getIntent();

		if (APP_ID == null) {
			APP_ID = Native.getAppID();
		}

		if (APP_ID == null) {
			Log.e(Tag, "APP_ID is null !!!!");
		}

		wxapi = WXAPIFactory.createWXAPI(this, APP_ID, true);
		wxapi.registerApp(APP_ID);
		wxapi.handleIntent(intent, this);

		if (intent.hasExtra(ReqWxLogin)) {
			reqLogin();
		} else if (intent.hasExtra(ReqWxShareImg)) {
			String ImgPath = intent.getStringExtra("ImgPath");
			int nType = intent.getIntExtra("ShareType", 0);
			reqShareImg(ImgPath, nType);
		} else if (intent.hasExtra(ReqWxShareTxt)) {
			String ShareText = intent.getStringExtra("ShareText");
			int nType = intent.getIntExtra("ShareType", 0);
			reqShareTxt(ShareText, nType);
		} else if (intent.hasExtra(ReqWxShareUrl)) {
			String ShareUrl = intent.getStringExtra("ShareUrl");
			String ShareTitle = intent.getStringExtra("ShareTitle");
			String ShareDesc = intent.getStringExtra("ShareDesc");
			int nType = intent.getIntExtra("ShareType", 0);
			reqShareUrl(ShareUrl, ShareTitle, ShareDesc, nType);
		}

		finish();
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		this.finish();
	}

	@Override
	protected void onNewIntent(Intent intent)
	{
		super.onNewIntent(intent);

		setIntent(intent);
		wxapi.handleIntent(intent, this);
	}

	public void reqLogin()
	{
		SendAuth.Req req = new SendAuth.Req();
		req.scope = "snsapi_userinfo";
		req.state = "carjob_wx_login";
		wxapi.sendReq(req);
		Log.d(Tag, "reqLogin!!!!");
	}

	public void reqShareImg(String ImgPath, int nType)
	{
		File file = new File(ImgPath);
		if (!file.exists()) {
			Log.d(Tag, "reqShare file not exists:" + ImgPath);
			return;
		}

		Bitmap bmp = BitmapFactory.decodeFile(ImgPath);
		WXImageObject imgObj = new WXImageObject(bmp);

		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = imgObj;

		Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, 128, 72, true);
		bmp.recycle();
		msg.thumbData = Util.bmpToByteArray(thumbBmp, true);

		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("img");
		req.message = msg;
		if (nType == SceneTimeline) {
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		} else if (nType == SceneSession) {
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		wxapi.sendReq(req);
		Log.d(Tag, "reqShare Ok:" + ImgPath);
	}

	public void reqShareTxt(String text, int nType)
	{
		WXTextObject textObj = new WXTextObject();
		textObj.text = text;

		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		msg.description = text;

		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("text");
		req.message = msg;
		if (nType == SceneTimeline) {
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		} else if (nType == SceneSession) {
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		wxapi.sendReq(req);

		Log.d(Tag, "reqShareTxt Ok:" + text);
	}

	public void reqShareUrl(String url, String title, String desc, int nType)
	{
		WXWebpageObject textObj = new WXWebpageObject();
		textObj.webpageUrl = url;

		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		msg.title = title;
		msg.description = desc;

		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("webpage");
		req.message = msg;
		if (nType == SceneTimeline) {
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		} else if (nType == SceneSession) {
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		wxapi.sendReq(req);

		Log.d(Tag, "reqShareUrl url=" + url);
		Log.d(Tag, "reqShareUrl title=" + title);
		Log.d(Tag, "reqShareUrl desc=" + desc);
		Log.d(Tag, "reqShareUrl nType=" + nType);
	}

	public void reqShareTxtCB(String text, int nType)
	{
		// send appdata with no attachment
		WXAppExtendObject appdata = new WXAppExtendObject("lallalallallal", "filePath");

		boolean bValue = appdata.checkArgs();
		if (!bValue) {
			Log.d(Tag, "reqShareTxtCB Error:" + text);
		}

		WXMediaMessage msg = new WXMediaMessage();
		msg.title = "11";
		msg.description = "22";
		msg.mediaObject = appdata;

		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("appdata");
		req.message = msg;

		if (nType == SceneTimeline) {
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		} else if (nType == SceneSession) {
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		wxapi.sendReq(req);

		Log.d(Tag, "reqShareTxtCB Ok:" + text);
	}

	@Override
	public void onReq(BaseReq req)
	{
		Log.d(Tag, "public void onReq(BaseReq req) !!!!!!!");
		switch (req.getType()) {
			case ConstantsAPI.COMMAND_GETMESSAGE_FROM_WX :
				Log.d(Tag, "onReq:COMMAND_GETMESSAGE_FROM_WX");
				break;
			case ConstantsAPI.COMMAND_SHOWMESSAGE_FROM_WX :
				Log.d(Tag, "onReq:COMMAND_SHOWMESSAGE_FROM_WX");
				break;
			default :
				break;
		}
		Log.d(Tag, "onReq:" + req.getType());
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.tencent.mm.sdk.openapi.IWXAPIEventHandler#onResp(com.tencent.mm.sdk.
	 * modelbase.BaseResp)
	 */
	@Override
	public void onResp(BaseResp resp)
	{
		Log.d(Tag, "public void onResp(BaseResp resp)");
		Log.d(Tag, "resp.getType() = " + resp.getType());
		Log.d(Tag, "errCode:" + resp.errCode);

		switch (resp.getType()) {
			case ConstantsAPI.COMMAND_SENDMESSAGE_TO_WX :// 微信分享
				Native.onWxShareResult("" + resp.errCode);
				break;
			case ConstantsAPI.COMMAND_SENDAUTH :// 微信登录
				switch (resp.errCode) {
					case BaseResp.ErrCode.ERR_OK :
						String code = ((SendAuth.Resp) resp).code;
						String Url = code + ":" + "authorization_code";
						Log.d(Tag, Url);
						Native.WxLoginGetAccessToken(Url);
						break;
					case BaseResp.ErrCode.ERR_USER_CANCEL :
						Native.WxLoginGetAccessToken("ERR_USER_CANCEL");
						break;
					case BaseResp.ErrCode.ERR_AUTH_DENIED :
						Native.WxLoginGetAccessToken("ERR_AUTH_DENIED");
						break;
					default :
						Native.WxLoginGetAccessToken("REE_Unknow");
						break;
				}
				break;
		}
	}

	private String buildTransaction(final String type)
	{
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
}