//
//  TTTRtcManager.h
//  TTTVideoChat
//
//  Created by yanzhen on 2018/8/15.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <TTTRtcEngineKit/TTTRtcEngineKit.h>
#import "TTTUser.h"

@interface TTTRtcManager : NSObject
@property (nonatomic, strong) TTTRtcEngineKit *rtcEngine;
@property (nonatomic, strong) TTTUser *me;
@property (nonatomic, assign) int64_t roomID;

+ (instancetype)manager;
@end
