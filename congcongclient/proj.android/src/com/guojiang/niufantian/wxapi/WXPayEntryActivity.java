package com.guojiang.niufantian.wxapi;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import com.mtdl.dlpaysdk.activity.DLPayManager;
import com.mtdl.dlpaysdk.activity.DLwebViewActivity;
import com.tencent.mm.sdk.constants.ConstantsAPI;
import com.tencent.mm.sdk.modelbase.BaseReq;
import com.tencent.mm.sdk.modelbase.BaseResp;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.sdk.openapi.WXAPIFactory;

public class WXPayEntryActivity extends Activity implements IWXAPIEventHandler
{
	private IWXAPI api;

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		api = WXAPIFactory.createWXAPI(this, DLwebViewActivity.getPayAppid());
		api.handleIntent(getIntent(), this);
	}

	@Override
	protected void onNewIntent(Intent intent)
	{
		super.onNewIntent(intent);
		setIntent(intent);
	}

	@Override
	public void onReq(BaseReq req)
	{
	}

	@Override
	protected void onDestroy()
	{
		super.onDestroy();
		this.finish();
	}

	@Override
	public void onResp(BaseResp resp)
	{
		if (resp.getType() == ConstantsAPI.COMMAND_PAY_BY_WX) {
			switch (resp.errCode) {
				case 0 :
					DLPayManager.isPaySuccess(true);// 该接口必须调用,支付成功则为true,否则为false
					System.out.println("支付成功");
					break;
				case -1 :
					// DLPayManager.isPaySuccess(false);
					System.out.println("支付失败");
					DLPayManager.isPaySuccess(false);
					// break;
				case -2 :
					// DLPayManager.isPaySuccess(false);
					// System.out.println("取消");
					// break;
				default :
					DLPayManager.isPaySuccess(false);
					// System.out.println("失败");
					break;
			}
			finish();
		}

	}
}