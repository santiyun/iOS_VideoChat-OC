//
//  TTTRtcManager.m
//  TTTVideoChat
//
//  Created by yanzhen on 2018/8/15.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import "TTTRtcManager.h"

@interface TTTRtcManager ()<TTTRtcEngineDelegate>

@end

@implementation TTTRtcManager
static id _manager;
+ (instancetype)manager {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _manager = [[self alloc] init];
    });
    return _manager;
}

+ (instancetype)allocWithZone:(struct _NSZone *)zone {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _manager = [super allocWithZone:zone];
    });
    return _manager;
}

- (id)copyWithZone:(NSZone *)zone {
    return _manager;
}

- (instancetype)init {
    self = [super init];
    if (self) {
        //输入申请的AppId
        _rtcEngine = [TTTRtcEngineKit sharedEngineWithAppId:<#AppId#> delegate:self];
        _me = [[TTTUser alloc] initWith:0];
        _videoProfile = TTTRtc_VideoProfile_Default;
    }
    return self;
}
@end
