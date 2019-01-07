//
//  IosHelper.cpp
//  kkddz
//
//  Created by macbook110 on 13-6-4.
//
//

#include "IosHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <UIKit/UIDevice.h>
#include <sys/socket.h> // Per msqr
#include <sys/sysctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#import "WXApi.h"
#import "WXApiObject.h"

//#import "../proj.ios_mac/zhangling/QJPaySDKDZF.h"
#import "../proj.ios_mac/ios/AppController.h"

//extern AppDelegate s_sharedApplication;

NSString* appId =@"wx3cadb286cad49c5d";
NSString* app_Key =@"b298e9fafabc890e69a35eb88e24cb74";

#endif//

void IosHelper::sendAuthRequest()
{ 
    //构造SendAuthReq结构体 
    SendAuthReq* req =[[[SendAuthReq alloc ] init ] autorelease ];
    req.scope = @"snsapi_userinfo" ;
    req.state = @"123" ;
    req.openID = @"wx3cadb286cad49c5d";
    //第三方向微信终端发送一个SendAuthReq消息结构
    //[WXApi sendReq:req];
    
    if ([WXApi isWXAppInstalled]) {
        [WXApi sendReq:req];
    }else{
        [WXApi sendAuthReq:req viewController:[UIApplication sharedApplication].keyWindow.rootViewController delegate:id<WXApiDelegate>([UIApplication sharedApplication])];
    }
    
}

void IosHelper::shareWithWeixinCircleTxt(const char * pTxt,const char * pUrl){
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = txt;
    message.description = txt;
    message.messageExt = txt;
    message.messageAction = txt;
    
    [message setThumbImage:[UIImage imageNamed:@"weixin_share_icon.png"]];
    
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = [NSString stringWithCString:pUrl encoding:NSUTF8StringEncoding];
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message; 
    req.scene = WXSceneTimeline;

    [WXApi sendReq:req];
}



void IosHelper::shareWithWeixinFriendTxt(const char * pTxt,const char * pUrl){
    
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    message.title = txt;
    message.description = txt;
    message.messageExt = txt;
    message.messageAction = txt;
    
    WXWebpageObject *ext = [WXWebpageObject object];
    ext.webpageUrl = [NSString stringWithCString:pUrl encoding:NSUTF8StringEncoding];

    message.mediaObject = ext;

    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message; 
    req.scene = WXSceneSession;

    [WXApi sendReq:req];

}

void IosHelper::shareWithWeixinFriendTxt2(const char * pTxt){
    
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];

    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = YES;
    req.text = txt;
    req.scene = WXSceneSession;
    
    [WXApi sendReq:req];
    
}

void IosHelper::shareWithWeixinFriendImg(const char * pTxt,const char *FileName)
{
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    NSString *filePath = [NSString stringWithCString:FileName encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    
    message.title = txt;
    message.description = txt;
    [message setThumbImage:[UIImage imageNamed:@"weixin_share_icon.png"]];
    
    WXImageObject *ext = [WXImageObject object];
    ext.imageData = [NSData dataWithContentsOfFile:filePath];
    
    
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneSession;
    
    [WXApi sendReq:req];
}
void IosHelper::shareWithWeixinCircleImg(const char * pTxt,const char *FileName)
{
    NSString *txt = [NSString stringWithCString:pTxt encoding:NSUTF8StringEncoding];
    NSString *filePath = [NSString stringWithCString:FileName encoding:NSUTF8StringEncoding];
    WXMediaMessage *message = [WXMediaMessage message];
    
    message.title = txt;
    message.description = txt;
    [message setThumbImage:[UIImage imageNamed:@"weixin_share_icon.png"]];
    
    WXImageObject *ext = [WXImageObject object];
    ext.imageData = [NSData dataWithContentsOfFile:filePath];
    
    
    
    message.mediaObject = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText = NO;
    req.message = message;
    req.scene = WXSceneTimeline;
    
    [WXApi sendReq:req];
}
void IosHelper::startBrowserJni( const char * url)
{
    NSString *nsFineName = [NSString stringWithCString:url encoding:NSUTF8StringEncoding];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:nsFineName]];
}
 AVAudioRecorder *recorder = NULL;
void IosHelper::beginRecord(const char *_fileName)
{
    //if (recorder == nil)
    //{
        //设置文件名和录音路径
        NSString *recordFilePath = [NSString stringWithCString:_fileName encoding:NSUTF8StringEncoding];
        
        NSDictionary *recordSetting = [[NSDictionary alloc] initWithObjectsAndKeys:
                                       [NSNumber numberWithFloat: 8000.0],AVSampleRateKey, //采样率
                                       [NSNumber numberWithInt: kAudioFormatLinearPCM],AVFormatIDKey,
                                       [NSNumber numberWithInt:16],AVLinearPCMBitDepthKey,//采样位数 默认 16
                                       [NSNumber numberWithInt: 1], AVNumberOfChannelsKey,//通道的数目
                                       nil];
        //初始化录音
        NSError *error = nil;
        recorder = [[ AVAudioRecorder alloc] initWithURL:[NSURL URLWithString:recordFilePath] settings:recordSetting error:&error];
    //}
    recorder.meteringEnabled = YES;
    [recorder prepareToRecord];
    //开始录音
    UInt32 sessionCategory = kAudioSessionCategory_PlayAndRecord;
    AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
    
    // 扬声器播放
    UInt32 audioRouteOverride = kAudioSessionOverrideAudioRoute_Speaker;
    AudioSessionSetProperty (kAudioSessionProperty_OverrideAudioRoute, sizeof(audioRouteOverride), &audioRouteOverride);
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayAndRecord error:nil];
    [[AVAudioSession sharedInstance] setActive:YES error:nil];
    [recorder record];
}
/*
void IosHelper::sendCongCongPay(const char* appkey,
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
                                int amount)
{
    //初始化的参数值
    NSArray * typeArray = @[[NSString stringWithFormat:@"%d",amount],//1],//
                            appId,
                            [NSString stringWithFormat:@"%s",body],
                            [NSString stringWithFormat:@"%s",appip],
                            [NSString stringWithFormat:@"%s",mchntOrderNo],
                            [NSString stringWithFormat:@"%s",subject],
                            [NSString stringWithFormat:@"%s",cpChannel],
                            [NSString stringWithFormat:@"%s",extra],
                            [NSString stringWithFormat:@"%s",notifyUrl]//"http://120.26.165.100:9999/"]//notifyUrl]
                            ];
    
    NSMutableDictionary * _handleDic;
    UIViewController* ctrol;
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        NSArray* array=[[UIApplication sharedApplication]windows];
        UIWindow* win=[array objectAtIndex:0];
        
        UIView* ui=[[win subviews] objectAtIndex:0];
        ctrol=(UIViewController*)[ui nextResponder];
    }
    else
    {
        ctrol= [UIApplication sharedApplication].keyWindow.rootViewController;
    }
    
    //NSLog(@"%@,%@,%@,%@,%@,%@,%@,%@",typeArray[0],typeArray[1],typeArray[2],typeArray[3],typeArray[4],typeArray[5],[QJPaySDKDZF PAY_PONITCART],typeArray[7], typeArray[8] );
    
    _handleDic = [NSMutableDictionary dictionaryWithObjects:@[typeArray[0],
                                                              typeArray[1],
                                                              @"用户充值金币",
                                                              typeArray[3],
                                                              typeArray[4],
                                                              @"金币",
                                                              [QJPaySDKDZF PAY_BANKCART],
                                                              typeArray[7],
                                                              typeArray[8]]
                                                    forKeys:@[@"amount",
                                                              @"appid",
                                                              @"body",
                                                              @"clientIp",
                                                              @"mchntOrderNo",
                                                              @"subject",
                                                              @"payChannelId",
                                                              @"extra",
                                                              @"notifyUrl"]];
    
    [QJPaySDKDZF QJPayStart:_handleDic AppScheme:@"QJPaySDKGUI" appKey:app_Key  andCurrentViewController:ctrol andDelegate:[AppController Instance] Flag:0x80];
}
*/
const char * IosHelper::endRecord()
{
    if (recorder == nil)
        return "";
    if (recorder.isRecording)
        [recorder stop];
	return "";
}
