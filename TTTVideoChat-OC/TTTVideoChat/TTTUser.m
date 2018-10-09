//
//  TTTUser.m
//  TTTVideoChat
//
//  Created by yanzhen on 2018/8/15.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import "TTTUser.h"

@implementation TTTUser

- (instancetype)initWith:(int64_t)uid {
    self = [super init];
    if (self) {
        _uid = uid;
    }
    return self;
}

@end
