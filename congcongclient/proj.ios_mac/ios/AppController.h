#import <UIKit/UIKit.h>
#import "WXApi.h"
//#import "../zhangling/QJPaySDKDZF.h"

@class RootViewController;

@interface AppController : NSObject </*QJPayManagerDelegate,*/WXApiDelegate,UIApplicationDelegate> {
    UIWindow *window;
    
}

@property(nonatomic, readonly) RootViewController* viewController;

+ (AppController*) Instance;

@end

