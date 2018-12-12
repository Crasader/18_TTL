//
//  IosHelper.h
//  kkddz
//
//  Created by macbook110 on 13-6-4.
//
//

#ifndef __kkddz__IosHelper__
#define __kkddz__IosHelper__

#include <iostream>
using namespace std;

class  IosHelper {
    
    
public:
	//微信登录
	static void sendAuthRequest();
	
    static void startBrowserJni( const char * url);
    //分享到微信朋友
    static void shareWithWeixinFriendTxt(const char * pTxt,const char * pUrl);
    //分享到微信朋友圈
    static void shareWithWeixinCircleTxt(const char * pTxt,const char * pUrl);
    
    static void shareWithWeixinFriendTxt2(const char * pTxt);
	
    static void shareWithWeixinFriendImg(const char * pTxt,const char *FileName);
    
    static void shareWithWeixinCircleImg(const char * pTxt,const char *FileName);
    static void sharedComplete();
	
    static void sendCongCongPay(const char* appkey,
                                const char* appsign,
                                const char* appip,
                                const char* subject, //∂©µ•±ÍÃ‚
                                const char* body,    //∂©µ•√Ë ˆ–≈œ¢
                                const char* mchntOrderNo,
                                const char* notifyUrl,
                                const char* cpChannel,
                                const char* description,
                                const char* extra,
                                const char* payStyle,
                                int payType,        //÷ß∏∂∑Ω Ω£¨“ªπ≤Àƒ÷÷
                                int amount);
    
	static void beginRecord(const char *_fileName);
    
    static const char * endRecord();
    
};

#endif /* defined(__kkddz__IosHelper__) */
