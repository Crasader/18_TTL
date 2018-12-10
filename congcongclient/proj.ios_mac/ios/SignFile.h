//
//  SignFile.h
//  testFOrdaoru
//
//  Created by linkyun on 16/9/21.
//  Copyright © 2016年 QianEn payment technology co., LTD. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface SignFile : NSObject
/**
 签名
 */
- (NSString *)signparams:(NSDictionary *)params;
@end
