//
//  QJPayWithWXPay.h
//  QJPayWithWXPay
//
//  Created by linkyun on 17/3/10.
//  Copyright © 2017年 linkyun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "WXApi.h"
#import <UIKit/UIKit.h>



@interface QJPayWithWXPay : NSObject
+ (void)sendWeChatMessage:(NSString *)orderInfo andAppScheme:(NSString *)weChatAppScheme resPonseResult:(void(^)(int statusCode))responseResultBlock;


+(BOOL) handleOpenURL:(NSURL *) url delegate:(id<WXApiDelegate>) delegate;

@end
