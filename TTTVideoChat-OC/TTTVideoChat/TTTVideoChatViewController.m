//
//  TTTVideoChatViewController.m
//  TTTVideoChat
//
//  Created by yanzhen on 2018/8/15.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import "TTTVideoChatViewController.h"
#import "TTTAVRegion.h"

@interface TTTVideoChatViewController ()<TTTRtcEngineDelegate>
@property (weak, nonatomic) IBOutlet UIImageView *meImgView;
@property (weak, nonatomic) IBOutlet UILabel *roomIDLabel;
@property (weak, nonatomic) IBOutlet UILabel *idLabel;
@property (weak, nonatomic) IBOutlet UILabel *audioStatsLabel;
@property (weak, nonatomic) IBOutlet UILabel *videoStatsLabel;
@property (weak, nonatomic) IBOutlet UIButton *voiceBtn;
@property (weak, nonatomic) IBOutlet UIView *avRegionsView;
@property (nonatomic, strong) NSMutableArray<TTTAVRegion *> *avRegions;
@property (nonatomic, strong) NSMutableArray<TTTUser *> *users;
@end

@implementation TTTVideoChatViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _avRegions = [NSMutableArray array];
    _users = [NSMutableArray array];
    _roomIDLabel.text = [NSString stringWithFormat:@"房号: %lld", TTManager.roomID];
    _idLabel.text = [NSString stringWithFormat:@"ID: %lld", TTManager.me.uid];
    for (UIView *subView in _avRegionsView.subviews) {
        if ([subView isKindOfClass:[TTTAVRegion class]]) {
            [_avRegions addObject:(TTTAVRegion *)subView];
        }
    }
    TTManager.rtcEngine.delegate = self;
    [TTManager.rtcEngine startPreview];
    TTTRtcVideoCanvas *videoCanvas = [[TTTRtcVideoCanvas alloc] init];
    videoCanvas.uid = TTManager.me.uid;
    videoCanvas.renderMode = TTTRtc_Render_Adaptive;
    videoCanvas.view = _meImgView;
    [TTManager.rtcEngine setupLocalVideo:videoCanvas];
}

- (IBAction)leftBtnsAction:(UIButton *)sender {
    if (sender.tag == 1001) {
        [TTManager.rtcEngine switchCamera];
    } else if (sender.tag == 1002) {
        sender.selected = !sender.isSelected;
        TTManager.me.mutedSelf = sender.isSelected;
        [TTManager.rtcEngine muteLocalAudioStream:sender.isSelected];
    }
}

- (IBAction)exitChannel:(id)sender {
    UIAlertController *alert  = [UIAlertController alertControllerWithTitle:@"提示" message:@"您确定要退出房间吗？" preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"取消" style:UIAlertActionStyleCancel handler:nil]];
    UIAlertAction *sureAction = [UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
        [TTManager.rtcEngine leaveChannel:nil];
    }];
    [alert addAction:sureAction];
    [self presentViewController:alert animated:YES completion:nil];
}

#pragma mark - TTTRtcEngineDelegate
-(void)rtcEngine:(TTTRtcEngineKit *)engine didJoinedOfUid:(int64_t)uid clientRole:(TTTRtcClientRole)clientRole isVideoEnabled:(BOOL)isVideoEnabled elapsed:(NSInteger)elapsed {
    TTTUser *user = [[TTTUser alloc] initWith:uid];
    [_users addObject:user];
    [[self getAvaiableAVRegion] configureRegion:user];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine didOfflineOfUid:(int64_t)uid reason:(TTTRtcUserOfflineReason)reason {
    TTTUser *user = [self getUser:uid];
    if (!user) { return; }
    [[self getAVRegion:uid] closeRegion];
    [_users removeObject:user];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine reportAudioLevel:(int64_t)userID audioLevel:(NSUInteger)audioLevel audioLevelFullRange:(NSUInteger)audioLevelFullRange {
    if (userID == TTManager.me.uid) {
        [_voiceBtn setImage:[self getVoiceImage:audioLevel] forState:UIControlStateNormal];
    } else {
        [[self getAVRegion:userID] reportAudioLevel:audioLevel];
    }
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine didAudioMuted:(BOOL)muted byUid:(int64_t)uid {
    TTTUser *user = [self getUser:uid];
    if (!user) { return; }
    user.mutedSelf = muted;
    [[self getAVRegion:uid] mutedSelf:muted];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine localAudioStats:(TTTRtcLocalAudioStats *)stats {
    _audioStatsLabel.text = [NSString stringWithFormat:@"A-↑%ldkbps", stats.sentBitrate];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine localVideoStats:(TTTRtcLocalVideoStats *)stats {
    _videoStatsLabel.text = [NSString stringWithFormat:@"V-↑%ldkbps", stats.sentBitrate];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine remoteAudioStats:(TTTRtcRemoteAudioStats *)stats {
    [[self getAVRegion:stats.uid] setRemoterAudioStats:stats.receivedBitrate];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine remoteVideoStats:(TTTRtcRemoteVideoStats *)stats {
    [[self getAVRegion:stats.uid] setRemoterVideoStats:stats.receivedBitrate];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine firstRemoteVideoFrameDecodedOfUid:(int64_t)uid size:(CGSize)size elapsed:(NSInteger)elapsed {
    //解码远端用户第一帧
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine didLeaveChannelWithStats:(TTTRtcStats *)stats {
    [engine stopPreview];
    [self dismissViewControllerAnimated:true completion:nil];
}

- (void)rtcEngineConnectionDidLost:(TTTRtcEngineKit *)engine {
    [TTProgressHud showHud:self.view message:@"网络链接丢失，正在重连..."];
}

- (void)rtcEngineReconnectServerTimeout:(TTTRtcEngineKit *)engine {
    [TTProgressHud hideHud:self.view];
    [self.view.window showToast:@"网络丢失，请检查网络"];
    [engine leaveChannel:nil];
    [engine stopPreview];
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)rtcEngineReconnectServerSucceed:(TTTRtcEngineKit *)engine {
    [TTProgressHud hideHud:self.view];
}

- (void)rtcEngine:(TTTRtcEngineKit *)engine didKickedOutOfUid:(int64_t)uid reason:(TTTRtcKickedOutReason)reason {
    NSString *errorInfo = @"";
    switch (reason) {
        case TTTRtc_KickedOut_KickedByHost:
            errorInfo = @"被主播踢出";
            break;
        case TTTRtc_KickedOut_PushRtmpFailed:
            errorInfo = @"rtmp推流失败";
            break;
        case TTTRtc_KickedOut_MasterExit:
            errorInfo = @"主播已退出";
            break;
        case TTTRtc_KickedOut_ReLogin:
            errorInfo = @"重复登录";
            break;
        case TTTRtc_KickedOut_NoAudioData:
            errorInfo = @"长时间没有上行音频数据";
            break;
        case TTTRtc_KickedOut_NoVideoData:
            errorInfo = @"长时间没有上行视频数据";
            break;
        case TTTRtc_KickedOut_NewChairEnter:
            errorInfo = @"其他人以主播身份进入";
            break;
        case TTTRtc_KickedOut_ChannelKeyExpired:
            errorInfo = @"Channel Key失效";
            break;
        default:
            errorInfo = @"未知错误";
            break;
    }
    [self.view.window showToast:errorInfo];
}

#pragma mark - helper mehtod
- (TTTAVRegion *)getAvaiableAVRegion {
    __block TTTAVRegion *region = nil;
    [_avRegions enumerateObjectsUsingBlock:^(TTTAVRegion * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (!obj.user) {
            region = obj;
            *stop = YES;
        }
    }];
    return region;
}

- (TTTAVRegion *)getAVRegion:(int64_t)uid {
    __block TTTAVRegion *region = nil;
    [_avRegions enumerateObjectsUsingBlock:^(TTTAVRegion * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.user.uid == uid) {
            region = obj;
            *stop = YES;
        }
    }];
    return region;
}

- (TTTUser *)getUser:(int64_t)uid {
    __block TTTUser *user = nil;
    [_users enumerateObjectsUsingBlock:^(TTTUser * _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        if (obj.uid == uid) {
            user = obj;
            *stop = YES;
        }
    }];
    return user;
}

- (UIImage *)getVoiceImage:(NSUInteger)level {
//    BOOL speakerphone = _routing != TTTRtc_AudioOutput_Headset;
    BOOL speakerphone = YES;
    if (TTManager.me.mutedSelf) {
        return [UIImage imageNamed:speakerphone ? @"voice_close" : @"tingtong_close"];
    }
    UIImage *image = nil;
    if (level < 4) {
        image = [UIImage imageNamed:speakerphone ? @"voice_small" : @"tingtong_small"];
    } else if (level < 7) {
        image = [UIImage imageNamed:speakerphone ? @"voice_middle" : @"tingtong_middle"];
    } else {
        image = [UIImage imageNamed:speakerphone ? @"voice_big" : @"tingtong_big"];
    }
    return image;
}

@end
