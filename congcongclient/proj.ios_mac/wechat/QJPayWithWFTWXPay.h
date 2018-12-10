//
//  QJPayWithWFTWXPay.h
//  QJPayWithWFTWXPay
//
//  Created by linkyun on 17/2/23.
//  Copyright © 2017年 linkyun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface QJPayWithWFTWXPay : NSObject

+ (void)sendWeChatWFTMessage:(NSString *)orderInfo andRootViewController:(UIViewController *)rootVC resPonseResult:(void(^)(int statusCode))responseResultBlock;


+ (BOOL)handleOpenURL:(NSURL *)url;

@end
