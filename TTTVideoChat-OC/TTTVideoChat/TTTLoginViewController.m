//
//  TTTLoginViewController.m
//  TTTVideoChat
//
//  Created by yanzhen on 2018/8/15.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import "TTTLoginViewController.h"

@interface TTTLoginViewController ()<TTTRtcEngineDelegate>
@property (weak, nonatomic) IBOutlet UITextField *roomIDTF;
@property (weak, nonatomic) IBOutlet UILabel *websiteLabel;
@property (nonatomic, assign) int64_t uid;

@end

@implementation TTTLoginViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    NSString *dateStr = NSBundle.mainBundle.infoDictionary[@"CFBundleVersion"];
    _websiteLabel.text = [TTTRtcEngineKit.getSdkVersion stringByAppendingFormat:@"__%@", dateStr];
    _uid = arc4random() % 100000 + 1;
    int64_t roomID = [[NSUserDefaults standardUserDefaults] stringForKey:@"ENTERROOMID"].longLongValue;
    if (roomID == 0) {
        roomID = arc4random() % 1000000 + 1;
    }
    _roomIDTF.text = [NSString stringWithFormat:@"%lld", roomID];
}

- (IBAction)enterChannel:(id)sender {
    if (_roomIDTF.text.integerValue == 0 || _roomIDTF.text.length >= 19) {
        [self showToast:@"请输入大于0，19位以内的房间ID"];
        return;
    }
    [NSUserDefaults.standardUserDefaults setValue:_roomIDTF.text forKey:@"ENTERROOMID"];
    [NSUserDefaults.standardUserDefaults synchronize];
    TTManager.me.uid = _uid;
    TTManager.me.mutedSelf = NO;
    TTManager.roomID = _roomIDTF.text.longLongValue;
    [TTProgressHud showHud:self.view];
    TTTRtcEngineKit *rtcEngine = TTManager.rtcEngine;
    rtcEngine.delegate = self;
    [rtcEngine enableVideo];
    [rtcEngine muteLocalAudioStream:NO];
    [rtcEngine setChannelProfile:TTTRtc_ChannelProfile_Communication];
    [rtcEngine enableAudioVolumeIndication:1000 smooth:3];
    //settings
    if (TTManager.isHighQualityAudio) {
        [rtcEngine setPreferAudioCodec:TTTRtc_AudioCodec_AAC bitrate:96 channels:1];
    }
    BOOL swapWH = UIInterfaceOrientationIsPortrait(UIApplication.sharedApplication.statusBarOrientation);
    if (TTManager.videoCustomProfile.isCustom) {
        TTTCustomVideoProfile profile = TTManager.videoCustomProfile;
        CGSize videoSize = swapWH ? CGSizeMake(profile.videoSize.height, profile.videoSize.width) : profile.videoSize;
        [rtcEngine setVideoProfile:videoSize frameRate:profile.fps bitRate:profile.videoBitRate];
    } else {
        [rtcEngine setVideoProfile:TTManager.videoProfile swapWidthAndHeight:swapWH];
    }
    [rtcEngine joinChannelByKey:nil channelName:_roomIDTF.text uid:_uid joinSuccess:nil];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self.view endEditing:YES];
}

#pragma mark - TTTRtcEngineDelegate
-(void)rtcEngine:(TTTRtcEngineKit *)engine didJoinChannel:(NSString *)channel withUid:(int64_t)uid elapsed:(NSInteger)elapsed {
    [TTProgressHud hideHud:self.view];
    [self performSegueWithIdentifier:@"VideoChat" sender:nil];
}

-(void)rtcEngine:(TTTRtcEngineKit *)engine didOccurError:(TTTRtcErrorCode)errorCode {
    NSString *errorInfo = @"";
    switch (errorCode) {
        case TTTRtc_Error_Enter_TimeOut:
            errorInfo = @"超时,10秒未收到服务器返回结果";
            break;
        case TTTRtc_Error_Enter_Failed:
            errorInfo = @"该直播间不存在";
            break;
        case TTTRtc_Error_Enter_BadVersion:
            errorInfo = @"版本错误";
            break;
        case TTTRtc_Error_InvalidChannelName:
            errorInfo = @"Invalid channel name";
            break;
        default:
            errorInfo = [NSString stringWithFormat:@"未知错误：%zd",errorCode];
            break;
    }
    [TTProgressHud hideHud:self.view];
    [self showToast:errorInfo];
}
@end
