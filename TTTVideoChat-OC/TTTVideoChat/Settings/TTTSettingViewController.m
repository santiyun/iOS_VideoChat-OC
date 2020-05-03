//
//  TTTSettingViewController.m
//  TTTVideoChat
//
//  Created by yanzhen on 2018/9/12.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import "TTTSettingViewController.h"

@interface TTTSettingViewController ()<UIPickerViewDelegate, UIPickerViewDataSource>
@property (weak, nonatomic) IBOutlet UITextField *videoTitleTF;
@property (weak, nonatomic) IBOutlet UITextField *videoSizeTF;
@property (weak, nonatomic) IBOutlet UITextField *videoBitrateTF;
@property (weak, nonatomic) IBOutlet UITextField *videoFpsTF;
@property (weak, nonatomic) IBOutlet UISwitch *audioSwitch;
@property (weak, nonatomic) IBOutlet UIView *pickBGView;
@property (weak, nonatomic) IBOutlet UIPickerView *pickView;
@property (nonatomic, strong) NSArray<NSString *> *videoSizes;
@end

@implementation TTTSettingViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _videoSizes = @[@"120P", @"180P", @"240P", @"360P", @"480P", @"640x480", @"960x540", @"720P", @"1080P", @"自定义"];
    _audioSwitch.on = TTManager.isHighQualityAudio;
    BOOL isCustom = TTManager.videoCustomProfile.isCustom;
    [self refreshState:isCustom profile:TTManager.videoProfile];
    if (isCustom) {
        [_pickView selectRow:9 inComponent:0 animated:YES];
        TTTCustomVideoProfile custom = TTManager.videoCustomProfile;
        _videoSizeTF.text = [NSString stringWithFormat:@"%.0fx%.0f", custom.videoSize.width, custom.videoSize.height];
        _videoBitrateTF.text = [NSString stringWithFormat:@"%lu", custom.videoBitRate];
        _videoFpsTF.text = [NSString stringWithFormat:@"%lu", custom.fps];
    } else {
        [_pickView selectRow:[self getVideoInfo:TTManager.videoProfile][3].integerValue inComponent:0 animated:YES];
    }
}

- (void)refreshState:(BOOL)isCustom profile:(TTTRtcVideoProfile)profile {
    if (isCustom) {
        _videoTitleTF.text = @"自定义";
        _videoSizeTF.enabled = YES;
        _videoBitrateTF.enabled = YES;
        _videoFpsTF.enabled = YES;
    } else {
        NSArray<NSString *> *info = [self getVideoInfo:profile];
        _videoTitleTF.text = _videoSizes[info[3].integerValue];
        _videoSizeTF.enabled = NO;
        _videoBitrateTF.enabled = NO;
        _videoFpsTF.enabled = NO;
        _videoSizeTF.text = info[1];
        _videoBitrateTF.text = info[0];
        _videoFpsTF.text = info[2];
    }
}

- (IBAction)saveSettingAction:(id)sender {
    if ([_videoTitleTF.text isEqualToString:@"自定义"]) {
        NSArray<NSString *> *sizes = [_videoSizeTF.text componentsSeparatedByString:@"x"];
        if (sizes.count != 2) {
            [self showToast:@"请输入正确的视频参数"];
            return;
        }
        if (sizes[0].longLongValue <= 0 || sizes[1].longLongValue <= 0) {
            [self showToast:@"请输入正确的视频参数"];
            return;
        }
        
        if (sizes[0].longLongValue > 1920) {
            [self showToast:@"视频宽最大为1920"];
            return;
        }
        
        if (sizes[1].longLongValue > 1080) {
            [self showToast:@"视频高最大为1080"];
            return;
        }
        
        if (_videoBitrateTF.text.longLongValue <= 0) {
            [self showToast:@"请输入正确码率参数"];
            return;
        }
        
        if (_videoBitrateTF.text.longLongValue > 5000) {
            [self showToast:@"码率不能大于5000"];
            return;
        }
        
        if (_videoFpsTF.text.longLongValue <= 0) {
            [self showToast:@"请输入正确帧率参数"];
            return;
        }
        
        TTTCustomVideoProfile profile = {YES, CGSizeMake(sizes[0].longLongValue, sizes[1].longLongValue), _videoBitrateTF.text.longLongValue, _videoFpsTF.text.longLongValue};
        TTManager.videoCustomProfile = profile;
    } else {
        TTTCustomVideoProfile profile = {NO, CGSizeZero, 0, 0};
        TTManager.videoCustomProfile = profile;
        NSInteger index = [_pickView selectedRowInComponent:0];
        TTManager.videoProfile = [self getProfileIndex:index];
    }
    TTManager.isHighQualityAudio = _audioSwitch.isOn;
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)showMoreVideoPara:(id)sender {
    _pickBGView.hidden = NO;
}

- (IBAction)back:(id)sender {
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (IBAction)cancelSetting:(id)sender {
    _pickBGView.hidden = YES;
}

- (IBAction)sureSetting:(id)sender {
    _pickBGView.hidden = YES;
    NSInteger index = [_pickView selectedRowInComponent:0];
    TTTRtcVideoProfile profile = [self getProfileIndex:index];
    [self refreshState:index == 9 profile:profile];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [self.view endEditing:YES];
}

#pragma mark - pickerView
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView {
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component {
    return _videoSizes.count;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component {
    return _videoSizes[row];
}

#pragma mark - help
- (NSArray<NSString *> *)getVideoInfo:(TTTRtcVideoProfile)profile {
    switch (profile) {
        case TTTRtc_VideoProfile_120P:
            return @[@"65", @"160x120", @"15", @"0"];
            break;
        case TTTRtc_VideoProfile_180P:
            return @[@"140", @"320x180", @"15", @"1"];
            break;
        case TTTRtc_VideoProfile_240P:
            return @[@"200", @"320x240", @"15", @"2"];
            break;
        case TTTRtc_VideoProfile_480P:
            return @[@"1000", @"848x480", @"15", @"4"];
            break;
        case TTTRtc_VideoProfile_640x480:
            return @[@"800", @"640x480", @"15", @"5"];
            break;
        case TTTRtc_VideoProfile_960x540:
            return @[@"1600", @"960x540", @"24", @"6"];
            break;
        case TTTRtc_VideoProfile_720P:
            return @[@"2400", @"1280x720", @"30", @"7"];
            break;
        case TTTRtc_VideoProfile_1080P:
            return @[@"3000", @"1920x1080", @"30", @"8"];
            break;
        default:
            return @[@"600", @"640x360", @"15", @"3"];
            break;
    }
}

- (TTTRtcVideoProfile)getProfileIndex:(NSInteger)index {
    switch (index) {
        case 0:
            return TTTRtc_VideoProfile_120P;
            break;
        case 1:
            return TTTRtc_VideoProfile_180P;
            break;
        case 2:
            return TTTRtc_VideoProfile_240P;
            break;
        case 4:
            return TTTRtc_VideoProfile_480P;
            break;
        case 5:
            return TTTRtc_VideoProfile_640x480;
            break;
        case 6:
            return TTTRtc_VideoProfile_960x540;
            break;
        case 7:
            return TTTRtc_VideoProfile_720P;
            break;
        case 8:
            return TTTRtc_VideoProfile_1080P;
            break;
        default:
            return 3;
            break;
    }
}
@end
