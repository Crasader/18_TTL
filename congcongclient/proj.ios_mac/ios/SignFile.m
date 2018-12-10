//
//  SignFile.m
//  testFOrdaoru
//
//  Created by linkyun on 16/9/21.
//  Copyright © 2016年 QianEn payment technology co., LTD. All rights reserved.
//

#import "SignFile.h"
#import <CommonCrypto/CommonDigest.h>


extern NSString* app_Key;



@implementation SignFile

#pragma mark -- 签名
- (NSString *)signparams:(NSDictionary *)params{
    
    NSArray * keyArr = params.allKeys;
    
    NSArray * newArr = [keyArr sortedArrayUsingComparator:^NSComparisonResult(id  _Nonnull obj1, id  _Nonnull obj2) {
        
        return [obj1 compare:obj2 options:NSCaseInsensitiveSearch];
        //return [obj1 compare:obj2];
    }];
    
    NSMutableString * String = [NSMutableString string];
    
    for (int j = 0; j< newArr.count;j++) {
        
        if (j == 0) {
                        
            if ([[[params objectForKey:newArr[j]] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] length]==0) {
            }else{
                [String appendFormat:@"%@=%@",newArr[j],[params objectForKey:newArr[j]]];
            }
        }
        else{
            if ([[[params objectForKey:newArr[j]] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] length]==0) {
            }else{
                [String appendFormat:@"&%@=%@",newArr[j],[params objectForKey:newArr[j]]];
            }
        }
    }
    
    NSLog(@"String------------->%@",String);
    
    
    
    
    NSString *sign = [NSString stringWithFormat:@"%@&key=%@",String,app_Key];
    
    NSLog(@"sign------------->%@",sign);
    
    return [self makeMD5WithString:sign];
}


/**md5*/
- (NSString *)makeMD5WithString:(NSString *)str{
    
    const char * cStr = [str UTF8String];
    unsigned char digest[CC_MD5_DIGEST_LENGTH];
    CC_MD5(cStr, strlen(cStr), digest);
    
    NSMutableString * output = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    
    for (int i = 0; i < CC_MD5_DIGEST_LENGTH; i++) {
        [output appendFormat:@"%02x",digest[i]];
    }
    return output;
}



@end
