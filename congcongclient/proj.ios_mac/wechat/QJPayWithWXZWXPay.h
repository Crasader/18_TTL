//
//  QJPayWithWXZWXPay.h
//  QJPayWithWXZWXPay
//
//  Created by linkyun on 16/11/28.
//  Copyright © 2016年 linkyun. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>



@interface QJPayWithWXZWXPay : NSObject

+ (void)sendWeChatZWXMessage:(NSString *)orderInfo andRootViewController:(UIViewController *)rootVC resPonseResult:(void(^)(BOOL statusCode))responseResultBlock;

@end
