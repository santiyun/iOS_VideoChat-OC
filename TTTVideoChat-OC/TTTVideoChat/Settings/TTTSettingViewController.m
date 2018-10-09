//
//  TTTSettingViewController.m
//  TTTVideoChat
//
//  Created by yanzhen on 2018/9/12.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

#import "TTTSettingViewController.h"

static NSString *videoSizeStr[] = {
    [TTTRtc_VideoProfile_120P]  = @"160X120",
    [TTTRtc_VideoProfile_180P]  = @"320X180",
    [TTTRtc_VideoProfile_240P]  = @"320X240",
    [TTTRtc_VideoProfile_360P]  = @"640x360",
    [TTTRtc_VideoProfile_480P]  = @"640x480",
    [TTTRtc_VideoProfile_720P]  = @"1280x720",
    [TTTRtc_VideoProfile_1080P] = @"1920x1080"
};

static NSString *videoBitrateStr[] = {
    [TTTRtc_VideoProfile_120P]  = @"65",
    [TTTRtc_VideoProfile_180P]  = @"140",
    [TTTRtc_VideoProfile_240P]  = @"200",
    [TTTRtc_VideoProfile_360P]  = @"400",
    [TTTRtc_VideoProfile_480P]  = @"500",
    [TTTRtc_VideoProfile_720P]  = @"1130",
    [TTTRtc_VideoProfile_1080P] = @"2080"
};

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
    _videoSizes = @[@"120P", @"180P", @"240P", @"360P", @"480P", @"720P", @"1080P", @"自定义"];
    _audioSwitch.on = TTManager.isHighQualityAudio;
    BOOL isCustom = TTManager.videoCustomProfile.isCustom;
    [self refreshState:isCustom profile:TTManager.videoProfile];
    if (isCustom) {
        [_pickView selectRow:7 inComponent:0 animated:YES];
        TTTCustomVideoProfile custom = TTManager.videoCustomProfile;
        _videoSizeTF.text = [NSString stringWithFormat:@"%.0fx%.0f", custom.videoSize.width, custom.videoSize.height];
        _videoBitrateTF.text = [NSString stringWithFormat:@"%lu", custom.videoBitRate];
        _videoFpsTF.text = [NSString stringWithFormat:@"%lu", custom.fps];
    } else {
        [_pickView selectRow:TTManager.videoProfile / 10 inComponent:0 animated:YES];
    }
}

- (void)refreshState:(BOOL)isCustom profile:(TTTRtcVideoProfile)profile {
    if (isCustom) {
        _videoTitleTF.text = @"自定义";
        _videoSizeTF.enabled = YES;
        _videoBitrateTF.enabled = YES;
        _videoFpsTF.enabled = YES;
    } else {
        _videoTitleTF.text = _videoSizes[profile / 10];
        _videoSizeTF.enabled = NO;
        _videoBitrateTF.enabled = NO;
        _videoFpsTF.enabled = NO;
        _videoSizeTF.text = videoSizeStr[profile];
        _videoBitrateTF.text = videoBitrateStr[profile];
    }
}

- (IBAction)saveSettingAction:(id)sender {
    if ([_videoTitleTF.text isEqualToString:@"自定义"]) {
        NSArray<NSString *> *sizes = [_videoSizeTF.text componentsSeparatedByString:@"x"];
        if (sizes.count != 2) {
            [self showToast:@"请输入正确的视频尺寸"];
            return;
        }
        if (sizes[0].longLongValue <= 0 || sizes[1].longLongValue <= 0) {
            [self showToast:@"请输入正确的视频尺寸"];
            return;
        }
        
        if (_videoBitrateTF.text.longLongValue <= 0) {
            [self showToast:@"请输入正确的码率"];
            return;
        }
        
        if (_videoFpsTF.text.longLongValue <= 0) {
            [self showToast:@"请输入正确的帧率"];
            return;
        }
        TTTCustomVideoProfile profile = {YES, CGSizeMake(sizes[0].longLongValue, sizes[1].longLongValue), _videoBitrateTF.text.longLongValue, _videoFpsTF.text.longLongValue};
        TTManager.videoCustomProfile = profile;
    } else {
        TTTCustomVideoProfile profile = {NO, CGSizeZero, 0, 0};
        TTManager.videoCustomProfile = profile;
        NSInteger index = [_pickView selectedRowInComponent:0];
        TTManager.videoProfile = index * 10;
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
    TTTRtcVideoProfile profile = index * 10;
    [self refreshState:index == 7 profile:profile];
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
@end
