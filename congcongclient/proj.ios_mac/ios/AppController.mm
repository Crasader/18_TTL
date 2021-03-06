/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
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

#import "AppController.h"
#import "platform/ios/CCEAGLView-ios.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "UserInfo.h"
#import "GameManagerBase.h"
#include "MissionWeiXin.h"

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

static AppDelegate s_sharedApplication;

static AppController* instance;

+ (AppController*) Instance{
    return instance;
}

// cocos2d application instance

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
// Override point for customization after application launch.
    
    cocos2d::Application *app = cocos2d::Application::getInstance();
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [window bounds]
                                         pixelFormat: (NSString*)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Use RootViewController manage CCEAGLView
    _viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    _viewController.wantsFullScreenLayout = YES;
    _viewController.view = eaglView;
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: _viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:_viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];//prefersStatusBarHidden
    
    [UIApplication sharedApplication].idleTimerDisabled=YES;

    
    // IMPORTANT: Setting the GLView should be done after creating the RootViewController
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView(eaglView);
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    app->run();

    [WXApi registerApp:@"wx3cadb286cad49c5d" withDescription:@"com-ttd-test"];
    
    instance = self;
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->pause(); */
    instance = self;
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    //We don't need to call this method any more. It will interupt user defined game pause&resume logic
    /* cocos2d::Director::getInstance()->resume(); */
    //[[NSNotificationCenter defaultCenter] postNotificationName:[QJPaySDKDZF WETCHAR] object:nil];
    instance = self;
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::Application::getInstance()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::Application::getInstance()->applicationWillEnterForeground();
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
}

#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

- (void)dealloc {
    [window release];
    [super dealloc];
}

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
    //if(!MissionWeiXin::Instance().isLogin())
    {
        return  [WXApi handleOpenURL:url delegate:self];
    }
    //else
    {
        //return  [QJPaySDKDZF handleOpenWechatURL:url];
    }
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
//    if(!MissionWeiXin::Instance().isLogin())
//    {
        BOOL isSuc = [WXApi handleOpenURL:url delegate:self];
        NSLog(@"url %@ isSuc %d",url,isSuc == YES ? 1 : 0);
        return  isSuc;
//    }
//    else
//    {
//        //[QJPaySDKDZF handleOpenWechatURL:url];
//        return [QJPaySDKDZF handleOpenURL:url];
//    }
}

-(void) onReq:(BaseReq*)req
{
    
}

-(void) onResp:(BaseResp*)resp
{
    if([resp isKindOfClass:[SendMessageToWXResp class]])
    {
        if(resp.errCode == 0)
        {
        }
    }
    if([resp isKindOfClass:[SendAuthResp class]])
    {
        if(resp.errCode == 0)
        {
            SendAuthResp *authResp = (SendAuthResp *)resp;
            NSString *strMsg = [NSString stringWithFormat:@"%@:authorization_code", authResp.code];
            MissionWeiXin::Instance().Req_Access_token([strMsg cStringUsingEncoding:NSUTF8StringEncoding]);
        }
    }
}

- (BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<NSString*, id> *)options
{
    //if(!MissionWeiXin::Instance().isLogin())
    {
        NSLog(@"openURL longin");
        return  [WXApi handleOpenURL:url delegate:self];
    }
    //else
    {
        NSLog(@"openURL QJPaySDKDZF");
        //[QJPaySDKDZF handleOpenWechatURL:url];
        //return [QJPaySDKDZF handleOpenURL:url];
    }
}

- (void)QJPayResponseResult:(int)response
{
    NSString* result = NULL;
    
//    [NSThread sleepForTimeInterval:1.0f];
//    
//    UserInfo::Instance().reqAccountInfo();
//    GameManagerBase::InstanceBase().RequreRefreshTableInfo();
//    
//    switch (response) {
//            
//        case 0:
//            NSLog(@"成功＝＝＝＝＝＝＝＝＝＝＝＝＝");
//            result=@"成功";
//            break;
//            
//        case 1:
//            NSLog(@"取消＝＝＝＝＝＝＝＝＝＝＝＝＝");
//            result=@"取消";
//            break;
//            
//        case -1:
//            NSLog(@"失败＝＝＝＝＝＝＝＝＝＝＝＝＝");
//            result=@"失败";
//            
//            break;
//
//        default:
//            break;
//    }
}
@end
