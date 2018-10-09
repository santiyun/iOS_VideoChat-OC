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

typedef struct {
    BOOL isCustom;
    CGSize videoSize;
    NSUInteger videoBitRate;
    NSUInteger fps;
}TTTCustomVideoProfile;

@interface TTTRtcManager : NSObject
@property (nonatomic, strong) TTTRtcEngineKit *rtcEngine;
@property (nonatomic, strong) TTTUser *me;
@property (nonatomic, assign) int64_t roomID;
//settings
@property (nonatomic, assign) BOOL isHighQualityAudio;
@property (nonatomic, assign) TTTRtcVideoProfile videoProfile;//set default is 360P
@property (nonatomic, assign) TTTCustomVideoProfile videoCustomProfile;

+ (instancetype)manager;
@end
