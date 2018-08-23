//
//  TTTAVRegion.m
//  TTTVideoChat
//
//  Created by yanzhen on 2018/8/15.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import "TTTAVRegion.h"

@interface TTTAVRegion ()
@property (strong, nonatomic) IBOutlet UIView *backgroundView;
@property (weak, nonatomic) IBOutlet UIImageView *videoView;
@property (weak, nonatomic) IBOutlet UILabel *idLabel;
@property (weak, nonatomic) IBOutlet UILabel *audioStatsLabel;
@property (weak, nonatomic) IBOutlet UILabel *videoStatsLabel;
@property (weak, nonatomic) IBOutlet UIButton *voiceBtn;

@end

@implementation TTTAVRegion
- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        [[NSBundle mainBundle] loadNibNamed:@"TTTAVRegion" owner:self options:nil];
        _backgroundView.frame = self.bounds;
        _backgroundView.backgroundColor = UIColor.clearColor;
        _backgroundView.alpha = 0.9;
        [self addSubview:_backgroundView];
    }
    return self;
}

- (UIImage *)getVoiceImage:(NSUInteger)audioLevel {
    UIImage *image = nil;
    if (audioLevel < 4) {
        image = [UIImage imageNamed:@"voice_small"];
    } else if (audioLevel < 7) {
        image = [UIImage imageNamed:@"voice_middle"];
    } else {
        image = [UIImage imageNamed:@"voice_big"];
    }
    return image;
}

#pragma mark - public
- (void)configureRegion:(TTTUser *)user {
    self.user = user;
    [_voiceBtn setImage:[UIImage imageNamed:@"voice_small"] forState:UIControlStateNormal];
    _idLabel.hidden = NO;
    _voiceBtn.hidden = NO;
    _audioStatsLabel.hidden = NO;
    _videoStatsLabel.hidden = NO;
    _idLabel.text = [NSString stringWithFormat:@"%lld", user.uid];
    
    TTTRtcVideoCanvas *videoCanvas = [[TTTRtcVideoCanvas alloc] init];
    videoCanvas.uid = user.uid;
    videoCanvas.renderMode = TTTRtc_Render_Adaptive;
    videoCanvas.view = _videoView;
    [TTManager.rtcEngine setupRemoteVideo:videoCanvas];
    
    if (user.mutedSelf) {
        [self mutedSelf:YES];
    }
}

- (void)closeRegion {
    _idLabel.hidden = YES;
    _voiceBtn.hidden = YES;
    _audioStatsLabel.hidden = YES;
    _videoStatsLabel.hidden = YES;
    _user = nil;
    _videoView.image = [UIImage imageNamed:@"video_head"];
}

- (void)reportAudioLevel:(NSUInteger)level {
    if (_user.mutedSelf) { return; }
    [_voiceBtn setImage:[self getVoiceImage:level] forState:UIControlStateNormal];
}

- (void)setRemoterAudioStats:(NSUInteger)stats {
    _audioStatsLabel.text = [NSString stringWithFormat:@"A-↓%ldkbps",stats];
}

- (void)setRemoterVideoStats:(NSUInteger)stats {
    _videoStatsLabel.text = [NSString stringWithFormat:@"V-↓%ldkbps",stats];
}

- (void)mutedSelf:(BOOL)mute {
    [_voiceBtn setImage:[UIImage imageNamed:mute ? @"speaking_closed" : @"voice_small"] forState:UIControlStateNormal];
}

@end
