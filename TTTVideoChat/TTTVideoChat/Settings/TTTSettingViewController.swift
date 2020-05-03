//
//  TTTSettingViewController.swift
//  TTTVideoChat
//
//  Created by yanzhen on 2018/9/11.
//  Copyright © 2018年 yanzhen. All rights reserved.
//

import UIKit
import TTTRtcEngineKit

private extension TTTRtcVideoProfile {
    func getProfileInfo() -> (String, String, Int, Int) {
        switch self {
        case ._VideoProfile_120P:
            return ("65", "160x120", 15, 0)
        case ._VideoProfile_180P:
            return ("140", "320x180", 15, 1)
        case ._VideoProfile_240P:
            return ("200", "320x240", 15, 2)
        case ._VideoProfile_480P:
            return ("1000", "848x480", 15, 4)
        case ._VideoProfile_640x480:
            return ("800", "640x480", 15, 5)
        case ._VideoProfile_960x540:
            return ("1600", "960x540", 24, 6)
        case ._VideoProfile_720P:
            return ("2400", "1280x720", 30, 7)
        case ._VideoProfile_1080P:
            return ("3000", "1920x1080", 30, 8)
        default:
            return ("600", "640x360", 15, 3)
        }
    }
}

class TTTSettingViewController: UIViewController {

    @IBOutlet private weak var videoTitleTF: UITextField!
    @IBOutlet private weak var videoSizeTF: UITextField!
    @IBOutlet private weak var videoBitrateTF: UITextField!
    @IBOutlet private weak var videoFpsTF: UITextField!
    @IBOutlet private weak var audioSwitch: UISwitch!
    @IBOutlet private weak var pickBGView: UIView!
    @IBOutlet private weak var pickView: UIPickerView!
    private let videoSizes = ["120P", "180P", "240P", "360P", "480P", "640x480", "960x540", "720P", "1080P", "自定义"]
    override func viewDidLoad() {
        super.viewDidLoad()
    
        audioSwitch.isOn = TTManager.isHighQualityAudio
        let isCustom = TTManager.videoCustomProfile.isCustom
        refreshState(isCustom, profile: TTManager.videoProfile)
        if isCustom {
            pickView.selectRow(9, inComponent: 0, animated: true)
            let custom = TTManager.videoCustomProfile
            videoSizeTF.text = "\(Int(custom.videoSize.width))x\(Int(custom.videoSize.height))"
            videoBitrateTF.text = custom.bitrate.description
            videoFpsTF.text = custom.fps.description
        } else {
            pickView.selectRow(TTManager.videoProfile.getProfileInfo().3, inComponent: 0, animated: true)
        }
    }
    
    @IBAction private func saveSettingAction(_ sender: Any) {
        if videoTitleTF.text == "自定义" {
            //videoSize必须以x分开两个数值
            if videoSizeTF.text == nil || videoSizeTF.text?.count == 0 {
                showToast("请输入正确视频参数")
                return
            }
            
            let sizes = videoSizeTF.text?.components(separatedBy: "x")
            if sizes?.count != 2 {
                showToast("请输入正确视频参数")
                return
            }
            
            guard let sizeW = Int(sizes![0]), sizeW > 0 else {
                showToast("请输入正确视频参数")
                return
            }
            
            if sizeW > 1920 {
                showToast("视频宽最大为1920")
                return
            }
            
            guard let sizeH = Int(sizes![1]), sizeH > 0 else {
                showToast("请输入正确视频参数")
                return
            }
            
            if sizeH > 1080 {
                showToast("视频高最大为1080")
                return
            }
            
            guard let bitrate = Int(videoBitrateTF.text!), bitrate > 0 else {
                showToast("请输入正确码率参数")
                return
            }
            
            if bitrate > 5000 {
                showToast("码率不能大于5000")
                return
            }
            
            guard let fps = Int(videoFpsTF.text!), fps > 0 else {
                showToast("请输入正确帧率参数")
                return
            }
            
            TTManager.videoCustomProfile = (true,CGSize(width: sizeW, height: sizeH),bitrate,fps)
        } else {
            TTManager.videoCustomProfile.isCustom = false
            let index = pickView.selectedRow(inComponent: 0)
            TTManager.videoProfile = getProfileIndex(index)
        }
        TTManager.isHighQualityAudio = audioSwitch.isOn
        dismiss(animated: true, completion: nil)
    }
    
    @IBAction private func showMoreVideoPara(_ sender: UIButton) {
        pickBGView.isHidden = false
    }
    
    @IBAction private func back(_ sender: Any) {
        dismiss(animated: true, completion: nil)
    }
    
    
    @IBAction private func cancelSetting(_ sender: Any) {
        pickBGView.isHidden = true
    }
    
    @IBAction private func sureSetting(_ sender: Any) {
        pickBGView.isHidden = true
        let index = pickView.selectedRow(inComponent: 0)
        let profile: TTTRtcVideoProfile = getProfileIndex(index)
        refreshState(index == 9, profile: profile)
        videoTitleTF.text = videoSizes[index]
    }
    
    override func touchesBegan(_ touches: Set<UITouch>, with event: UIEvent?) {
        view.endEditing(true)
    }
}

extension TTTSettingViewController: UIPickerViewDelegate, UIPickerViewDataSource {
    func numberOfComponents(in pickerView: UIPickerView) -> Int {
        return 1;
    }
    
    func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return videoSizes.count;
    }
    
    func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return videoSizes[row]
    }
}

private extension TTTSettingViewController {
    
    func refreshState(_ isCustom: Bool, profile: TTTRtcVideoProfile) {
        if isCustom {
            videoTitleTF.text = "自定义"
            videoSizeTF.isEnabled = true
            videoBitrateTF.isEnabled = true
            videoFpsTF.isEnabled = true
        } else {
            let index = profile.getProfileInfo().3
            videoTitleTF.text = videoSizes[index]
            videoSizeTF.isEnabled = false
            videoBitrateTF.isEnabled = false
            videoFpsTF.isEnabled = false
            videoSizeTF.text = profile.getProfileInfo().1
            videoBitrateTF.text = profile.getProfileInfo().0
            videoFpsTF.text = profile.getProfileInfo().2.description
        }
    }
    
    func getProfileIndex(_ index: Int) -> TTTRtcVideoProfile {
        if index == 0 {
            return ._VideoProfile_120P
        } else if index == 1 {
            return ._VideoProfile_180P
        } else if index == 2 {
            return ._VideoProfile_240P
        } else if index == 3 {
            return ._VideoProfile_360P
        } else if index == 4 {
            return ._VideoProfile_480P
        } else if index == 5 {
            return ._VideoProfile_640x480
        } else if index == 6 {
            return ._VideoProfile_960x540
        } else if index == 7 {
            return ._VideoProfile_720P
        } else if index == 8 {
            return ._VideoProfile_1080P
        }
        return ._VideoProfile_360P
    }
}
