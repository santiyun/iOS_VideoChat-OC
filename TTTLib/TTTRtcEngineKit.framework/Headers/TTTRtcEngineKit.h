#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#if TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
typedef UIView      TView;
typedef UIImage     TImage;
typedef UIImageView TImageView;
#elif TARGET_OS_MAC
#import <AppKit/AppKit.h>
typedef NSView      TView;
typedef NSImage     TImage;
typedef NSImageView TImageView;
#endif

/**
 *  错误代码
 */
typedef NS_ENUM(NSUInteger, TTTRtcErrorCode) {
    TTTRtc_Error_NoError            = 0,    // 没有错误
    // 进入房间错误
    TTTRtc_Error_InvalidChannelName = 9000, // 无效的房间名称
    TTTRtc_Error_Enter_TimeOut      = 9001, // 超时,10秒未收到服务器返回结果
    TTTRtc_Error_Enter_Failed       = 9002, // 无法连接服务器
    TTTRtc_Error_Enter_VerifyFailed = 9003, // 验证码错误
    TTTRtc_Error_Enter_BadVersion   = 9004, // 版本错误
    TTTRtc_Error_Enter_Unknown      = 9005, // 未知错误
    //
    TTTRtc_Error_Unknown            = 9999, // 未知错误
};

/**
 *  踢出房间原因
 */
typedef NS_ENUM(NSUInteger, TTTRtcKickedOutReason) {
    TTTRtc_KickedOut_KickedByHost      = 1, // 被主播踢出
    TTTRtc_KickedOut_PushRtmpFailed    = 2, // rtmp推流失败
    TTTRtc_KickedOut_ServerOverload    = 3, // 服务器过载
    TTTRtc_KickedOut_MasterExit        = 4, // 主播已退出
    TTTRtc_KickedOut_ReLogin           = 5, // 重复登录
    TTTRtc_KickedOut_NoAudioData       = 6, // 长时间没有上行音频数据
    TTTRtc_KickedOut_NoVideoData       = 7, // 长时间没有上行视频数据
    TTTRtc_KickedOut_NewChairEnter     = 8, // 其他人以主播身份进入
    TTTRtc_KickedOut_ChannelKeyExpired = 9, // Channel Key失效
};

/**
 *  频道模式
 */
typedef NS_ENUM(NSUInteger, TTTRtcChannelProfile) {
    TTTRtc_ChannelProfile_Communication    = 0, // 通信
    TTTRtc_ChannelProfile_LiveBroadcasting = 1, // 直播
    TTTRtc_ChannelProfile_Game_FreeMode    = 2, // 游戏（自由发言模式）
};

/**
 *  用户角色
 */
typedef NS_ENUM(NSUInteger, TTTRtcClientRole) {
    TTTRtc_ClientRole_Anchor      = 1, // 主播
    TTTRtc_ClientRole_Broadcaster = 2, // 副播
    TTTRtc_ClientRole_Audience    = 3, // 观众(默认)
};

/**
 *  视频编码属性
 */
typedef NS_ENUM(NSUInteger, TTTRtcVideoProfile) {
                                       //   res     fps   kbps
    TTTRtc_VideoProfile_120P    =  0,  // 160x120    15     65
    TTTRtc_VideoProfile_180P    = 10,  // 320x180    15    140
    TTTRtc_VideoProfile_240P    = 20,  // 320x240    15    200
    TTTRtc_VideoProfile_360P    = 30,  // 640x360    15    400
    TTTRtc_VideoProfile_480P    = 40,  // 640x480    15    500
    TTTRtc_VideoProfile_720P    = 50,  // 1280x720   15   1130
    TTTRtc_VideoProfile_1080P   = 60,  // 1920x1080  15   2080
    TTTRtc_VideoProfile_Default = TTTRtc_VideoProfile_360P
};

/**
 *  用户离线原因
 */
typedef NS_ENUM(NSUInteger, TTTRtcUserOfflineReason) {
    TTTRtc_UserOffline_Quit           = 0, // 用户主动离开
    TTTRtc_UserOffline_Dropped        = 1, // 因过长时间收不到对方数据包，超时掉线。
    TTTRtc_UserOffline_BecomeAudience = 2, // 当用户身份从主播切换为观众时触发
};

/**
 *  连麦主播离开原因
 */
typedef NS_ENUM(NSUInteger, TTTRtcAnchorExitReason) {
    TTTRtc_AnchorExit_Unlink    = 0, // 其他主播结束与“我”连麦，正常离开
    TTTRtc_AnchorExit_Timeout   = 1, // 心跳超时断开
    TTTRtc_AnchorExit_LinkClose = 2, // 网络异常断开
};

/**
 *  音频输出路由
 */
typedef NS_ENUM(NSUInteger, TTTRtcAudioOutputRouting)
{
    TTTRtc_AudioOutput_Headset   = 0, //耳机或蓝牙
    TTTRtc_AudioOutput_Speaker   = 1, //扬声器
    TTTRtc_AudioOutput_Headphone = 2  //手机听筒
};

/**
 *  日志过滤器
 */
typedef NS_ENUM(NSUInteger, TTTRtcLogFilter) {
    TTTRtc_LogFilter_Off      = 0,
    TTTRtc_LogFilter_Debug    = 0x080f,
    TTTRtc_LogFilter_Info     = 0x000f,
    TTTRtc_LogFilter_Warning  = 0x000e,
    TTTRtc_LogFilter_Error    = 0x000c,
    TTTRtc_LogFilter_Critical = 0x0008
};

/**
 *  视频显示模式
 */
typedef NS_ENUM(NSUInteger, TTTRtcRenderMode) {
    // 如果视频尺寸与显示视窗尺寸不一致，则视频流会按照显示视窗的比例进行周边裁剪或图像拉伸后填满视窗。
    TTTRtc_Render_Hidden   = 1,
    // 如果视频尺寸与显示视窗尺寸不一致，在保持长宽比的前提下，将视频进行缩放后填满视窗。
    TTTRtc_Render_Fit      = 2,
    // 如果自己和对方都是竖屏，或者如果自己和对方都是横屏，使用TTTRtc_Render_Hidden；如果对方和自己一个竖屏一个横屏，则使用TTTRtc_Render_Fit。
    TTTRtc_Render_Adaptive = 3
};

/**
 *  聊天消息类型
 */
typedef NS_ENUM(NSUInteger, TTTRtcChatType) {
    TTTRtc_ChatType_Text    = 1,
    TTTRtc_ChatType_Picture = 2,
    TTTRtc_ChatType_Audio   = 3,
    TTTRtc_ChatType_Custom  = 4
};

/**
 *  RTMP推流状态
 */
typedef NS_ENUM(NSUInteger, TTTRtcRtmpPublishStatus) {
    TTTRtc_RtmpPublishStatus_InitError     = 0, // 初始化RTMP发送器失败
    TTTRtc_RtmpPublishStatus_OpenError     = 1, // 打开RTMP链接失败
    TTTRtc_RtmpPublishStatus_AudioNoBuf    = 2, // 音频数据缓冲区空间不足
    TTTRtc_RtmpPublishStatus_VideoNoBuf    = 3, // 视频数据缓冲区空间不足
    TTTRtc_RtmpPublishStatus_LinkFailed    = 4, // 发送视频数据失败
    TTTRtc_RtmpPublishStatus_LinkSuccessed = 5, // 推流成功
};

/**
 *  视频流类型
 */
typedef NS_ENUM(NSUInteger, TTTRtcVideoStreamType) {
    TTTRtc_VideoStream_High = 0, // 视频大流
    TTTRtc_VideoStream_Low  = 1, // 视频小流
};

/**
 *  会话数据信息
 */
@interface TTTRtcStats : NSObject

@property (assign, nonatomic) NSUInteger duration;        // 通话时长，累计值
@property (assign, nonatomic) NSUInteger txBytes;         // 发送字节数，累计值
@property (assign, nonatomic) NSUInteger rxBytes;         // 接收字节数，累计值
@property (assign, nonatomic) NSUInteger txAudioKBitrate;
@property (assign, nonatomic) NSUInteger rxAudioKBitrate;
@property (assign, nonatomic) NSUInteger txVideoKBitrate;
@property (assign, nonatomic) NSUInteger rxVideoKBitrate;
@property (assign, nonatomic) NSUInteger users;

@end

/**
 *  本地视频统计信息
 */
@interface TTTRtcLocalVideoStats : NSObject

@property (assign, nonatomic) NSUInteger encodedBitrate;  // 编码的码率(kbps)
@property (assign, nonatomic) NSUInteger sentBitrate;     // 发送的码率(kbps)
@property (assign, nonatomic) NSUInteger sentFrameRate;   // 发送的帧率(fps)
@property (assign, nonatomic) NSUInteger receivedBitrate; // 接收的码率(kbps)
@property (assign, nonatomic) float sentLossRate;         // 发送的丢包率
@property (assign, nonatomic) int bufferDuration;         // 视频缓冲区大小

@end

/**
 *  本地音频统计信息
 */
@interface TTTRtcLocalAudioStats : NSObject

@property (assign, nonatomic) NSUInteger encodedBitrate;  // 编码的码率(kbps)
@property (assign, nonatomic) NSUInteger sentBitrate;     // 发送的码率(kbps)
@property (assign, nonatomic) NSUInteger receivedBitrate; // 接收的码率(kbps)

@end

/**
 *  远端视频统计信息
 */
@interface TTTRtcRemoteVideoStats : NSObject

@property (assign, nonatomic) int64_t uid;
@property (assign, nonatomic) NSUInteger delay;
@property (assign, nonatomic) NSUInteger width;
@property (assign, nonatomic) NSUInteger height;
@property (assign, nonatomic) NSUInteger receivedBitrate;
@property (assign, nonatomic) NSUInteger receivedFrameRate;
@property (assign, nonatomic) NSUInteger receivedFrames;    // 接收的帧数
@property (assign, nonatomic) NSUInteger lostFrames;        // 丢掉的帧数

@end

/**
 *  远端音频统计信息
 */
@interface TTTRtcRemoteAudioStats : NSObject

@property (assign, nonatomic) int64_t uid;
@property (assign, nonatomic) NSUInteger receivedBitrate;

@end

/**
 *  视频属性
 */
@interface TTTRtcVideoCanvas : NSObject

@property (strong, nonatomic) TImageView *view;            // 视频显示窗口。SDK不维护view的生命周期，应用程序应保证view在通话中是有效的。
@property (assign, nonatomic) TTTRtcRenderMode renderMode; // 视频显示模式
@property (assign, nonatomic) int64_t uid;                 // 用户ID

@end

/**
 *  视频合成区域
 */
@interface TTTRtcVideoCompositingRegion : NSObject

@property (assign, nonatomic) int64_t uid;      // 待显示在该区域的主播用户 uid
@property (assign, nonatomic) double x;         // [0.0, 1.0] 屏幕里该区域的横坐标
@property (assign, nonatomic) double y;         // [0.0, 1.0] 屏幕里该区域的纵坐标
@property (assign, nonatomic) double width;     // [0.0, 1.0] 该区域的实际宽度
@property (assign, nonatomic) double height;    // [0.0, 1.0] 该区域的实际高度
@property (assign, nonatomic) NSInteger zOrder; // [0, 100]   用于定义图层。0表示该区域图像位于最下层，而100表示该区域图像位于最上层。
@property (assign, nonatomic) double alpha;     // [0.0, 1.0] 用于定义图像的透明度。0表示图像为透明的，1表示图像为完全不透明的。
@property (assign, nonatomic) TTTRtcRenderMode renderMode; // 视频显示模式

@end

/**
 *  视频合成布局
 */
@interface TTTRtcVideoCompositingLayout : NSObject

@property (assign, nonatomic) NSInteger canvasWidth;   // 整个屏幕(画布)的宽度
@property (assign, nonatomic) NSInteger canvasHeight;  // 整个屏幕(画布)的高度
@property (copy, nonatomic) NSString* backgroundColor; // 屏幕(画布)的背景颜色，可根据 RGB 填写所需颜色对应的6位符号。e.g. "#c0c0c0"
@property (strong, nonatomic) NSMutableArray *regions; // 视频合成区域列表
@property (copy, nonatomic) NSString* appData;         // 应用程序自定义的数据

@end

/**
 *  直播推流配置
 */
@interface TTTPublisherConfiguration : NSObject

@property (assign, nonatomic) NSInteger bitrate;       // 旁路直播输出码流的码率。默认设置为 500 Kbps
@property (copy, nonatomic) NSString *publishUrl;      // 合图推流地址
@property (assign, nonatomic) BOOL isPureAudio;        // 推送纯音频流

@end

/**
 *  直播推流配置生成器
 */
@interface TTTPublisherConfigurationBuilder : NSObject

- (TTTPublisherConfigurationBuilder *)setBitrate:(NSInteger)bitrate;
- (TTTPublisherConfigurationBuilder *)setPublisherUrl:(NSString *)url;
- (TTTPublisherConfigurationBuilder *)setPublishPureAudio:(BOOL)isPureAudio;
- (TTTPublisherConfiguration *)build;

@end

/**
 *  视频帧格式
 */
typedef NS_ENUM(NSUInteger, TTTRtcVideoFrameFormat) {
    TTTRtc_VideoFrameFormat_Texture = 0,
    TTTRtc_VideoFrameFormat_I420    = 1,
    TTTRtc_VideoFrameFormat_NV12    = 2,
    TTTRtc_VideoFrameFormat_NV21    = 3,
    TTTRtc_VideoFrameFormat_RGBA    = 4,
    TTTRtc_VideoFrameFormat_BGRA    = 5,
    TTTRtc_VideoFrameFormat_ARGB    = 6,
};

/**
 *  视频帧
 */
@interface TTTRtcVideoFrame : NSObject

@property (assign, nonatomic) TTTRtcVideoFrameFormat format;  // 视频帧的格式
@property (assign, nonatomic) CMTime time;                    // 视频帧的时间戳，以毫秒为单位。不正确的时间戳会导致丢帧或者音视频不同步
@property (assign, nonatomic) CVPixelBufferRef textureBuffer;
@property (strong, nonatomic) NSData *dataBuffer;             // raw data buffer. in case of ios texture, it is not used
@property (assign, nonatomic) int strideInPixels;             // 视频帧的行间距，单位为像素而不是字节。
@property (assign, nonatomic) int height;                     // how many rows of pixels, in case of ios texture, it is not used
@property (assign, nonatomic) int cropLeft;                   // how many pixels to crop on the left boundary
@property (assign, nonatomic) int cropTop;                    // how many pixels to crop on the top boundary
@property (assign, nonatomic) int cropRight;                  // how many pixels to crop on the right boundary
@property (assign, nonatomic) int cropBottom;                 // how many pixels to crop on the bottom boundary
@property (assign, nonatomic) int rotation;                   // 0, 90, 180, 270.

@end

/**
 *  聊天信息
 */
@interface TTTRtcChatInfo : NSObject

@property (assign, nonatomic) TTTRtcChatType chatType;  // 聊天类型
@property (copy, nonatomic) NSString *seqID;            // 唯一标识
@property (copy, nonatomic) NSString *chatData;         // 聊天内容
@property (assign, nonatomic) NSUInteger audioDuration; // 音频时长（单位“秒”，chatType为“Audio”）

@end

@protocol TTTRtcEngineDelegate;

@interface TTTRtcEngineKit : NSObject

/**
 *  应用ID，由连麦平台分配，用于区分不同的客户和应用。
 */
@property (nonatomic, copy) NSString *appID;

/**
 *  SDK回调对象，由调用方赋值。
 */
@property (nonatomic, weak) id<TTTRtcEngineDelegate> delegate;

/**
 *  音视频信息统计间隔（默认：2秒）
 */
@property (nonatomic, assign) NSUInteger statsInterval;

/**
 *  初始化SDK，加载资源。
 *
 *  @param appId        应用ID，由连麦平台分配，用于区分不同的客户和应用，在同一个连麦平台内保证唯一。
 *  @param delegate     SDK回调代理
 *  @param enableChat   打开发送聊天功能
 *  @param enableSignal 打开发送信令功能
 *
 *  @return TTTRtc 对象(单例)
 */
+ (instancetype)sharedEngineWithAppId:(NSString *)appId delegate:(id<TTTRtcEngineDelegate>)delegate
                           enableChat:(BOOL)enableChat enableSignal:(BOOL)enableSignal;

+ (instancetype)sharedEngineWithAppId:(NSString *)appId delegate:(id<TTTRtcEngineDelegate>)delegate;

/**
 *  销毁引擎实例
 */
+ (void)destroy;

/**
 *  获取SDK版本号
 */
+ (NSString *)getSdkVersion;

/**
 *  设置服务器地址
 *
 *  @param ip   ip地址或域名
 *  @param port 端口
 */
- (void)setServerIp:(NSString*)ip port:(int)port;

/**
 *  设置频道模式
 *
 *  @param profile 频道模式
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setChannelProfile:(TTTRtcChannelProfile)profile;

/**
 *  设置用户角色
 *
 *  @param role          直播场景里的用户角色
 *  @param permissionKey 连麦鉴权密钥
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setClientRole:(TTTRtcClientRole)role withKey:(NSString *)permissionKey;

/**
 *  设置视频编码属性
 *
 *  @param profile            每个Profile对应一套视频参数，如分辨率、帧率、码率等。
 *  @param swapWidthAndHeight 是否交换宽和高
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setVideoProfile:(TTTRtcVideoProfile)profile swapWidthAndHeight:(BOOL)swapWidthAndHeight;

/**
 *  设置视频编码属性（自定义）
 *
 *  @param videoSize 视频分辨率
 *  @param frameRate 视频帧率
 *  @param bitRate   视频码率
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setVideoProfile:(CGSize)videoSize frameRate:(NSUInteger)frameRate bitRate:(NSUInteger)bitRate;

/**
 *  加入通话频道
 *
 *  @param channelKey        此为程序生成的Channel Key（当用户使用静态Key也即只使用App ID时, 该参数是可选的）
 *  @param channelName       标识通话的频道名称，长度在64字节以内的字符串。
 *  @param uid               用户ID
 *  @param joinSuccessBlock  用户加入成功回调
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)joinChannelByKey:(NSString *)channelKey
            channelName:(NSString *)channelName
                    uid:(int64_t)uid
            joinSuccess:(void(^)(NSString *channel, int64_t uid, NSInteger elapsed))joinSuccessBlock;

/**
 *  离开频道，即挂断或退出通话。
 *
 *  @param leaveChannelBlock 成功离开频道的回调
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)leaveChannel:(void(^)(TTTRtcStats* stat))leaveChannelBlock;

/**
 *  该方法用于开启视频模式。
 *  可以在加入频道前或者通话中调用，在加入频道前调用，则自动开启视频模式，在通话中调用则由音频模式切换为视频模式。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)enableVideo;

/**
 *  该方法用于关闭视频，开启纯音频模式。
 *  可以在加入频道前或者通话中调用，在加入频道前调用，则自动开启纯音频模式，在通话中调用则由视频模式切换为纯音频频模式。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)disableVideo;

/**
 *  禁用/启用本地视频功能。该方法用于只看不发的视频场景。该方法不需要本地有摄像头。
 *
 *  @param enabled YES: 启用本地视频（默认），NO: 禁用本地视频。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)enableLocalVideo:(BOOL)enabled;

/**
 *  启动本地视频预览
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startPreview;

/**
 *  停止本地视频预览
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)stopPreview;

/**
 *  设置本地视频显示属性
 *
 *  @param local 本地视频显示属性
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setupLocalVideo:(TTTRtcVideoCanvas*)local;

/**
 *  设置远端视频显示属性
 *
 *  @param remote 远端视频显示属性
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setupRemoteVideo:(TTTRtcVideoCanvas*)remote;

/**
 *  切换前置/后置摄像头
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)switchCamera;

/**
 *  暂停所有远端视频流
 *
 *  @param mute YES: 停止播放接收到的所有视频流，NO: 允许播放接收到的所有视频流。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteAllRemoteVideoStreams:(BOOL)mute;

/**
 *  允许/禁止播放指定的远端视频流
 *
 *  @param uid  用户ID
 *  @param mute YES: 停止播放接收到的视频流，NO: 允许播放接收到的视频流。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteRemoteVideoStream:(int64_t)uid mute:(BOOL)mute;

/**
 *  打开/关闭耳返功能，在插入耳机的情况下有效
 *
 *  @param enable YES: 打开耳返功能，NO: 关闭耳返功能。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)enableAudioEarBack:(BOOL)enable;

/**
 *  静音/取消静音。该方法用于允许/禁止往网络发送本地音频流。
 *
 *  @param mute YES: 麦克风静音, NO: 取消静音。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteLocalAudioStream:(BOOL)mute;

/**
 *  静音所有远端音频/对所有远端音频取消静音。该方法用于允许/禁止播放远端用户的音频流，即对所有远端用户进行静音与否。
 *
 *  @param mute YES: 停止播放所接收的音频流, NO: 恢复播放所接收的音频流。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteAllRemoteAudioStreams:(BOOL)mute;

/**
 *  静音指定远端用户/对指定远端用户取消静音。本方法用于允许/禁止播放远端用户的音频流。
 *
 *  @param mute YES: 停止播放指定用户的音频流, NO: 恢复播放指定用户的音频流。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteRemoteAudioStream:(int64_t)uid mute:(BOOL)mute;

/**
 *  禁言指定远端用户/对指定远端用户取消禁言。
 *
 *  @param mute YES: 禁止发言, NO: 允许发言。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)muteRemoteSpeaking:(int64_t)uid mute:(BOOL)mute;

#if TARGET_OS_IPHONE

/**
 *  切换音频输出方式：扬声器或听筒
 *
 *  @param enableSpeaker YES: 音频输出至扬声器，NO: 语音会根据默认路由出声。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setEnableSpeakerphone:(BOOL)enableSpeaker;

/**
 *  是否是扬声器状态
 *
 *  @return YES: 表明输出到扬声器，NO: 表明输出到非扬声器(听筒，耳机等)。
 */
- (BOOL)isSpeakerphoneEnabled;

/**
 *  设置默认的语音路由
 *
 *  @param defaultToSpeaker YES: 从扬声器出声，NO: 语音聊天：从听筒出声；视频聊天：从扬声器出声。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setDefaultAudioRouteToSpeakerphone:(BOOL)defaultToSpeaker;

#endif

/**
 *  启用说话者音量提示
 *
 *  @param interval 指定音量提示的时间间隔（<=0: 禁用音量提示功能；>0: 提示间隔，单位为毫秒。建议设置到大于200毫秒。）
 *  @param smooth   平滑系数。默认可以设置为3。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)enableAudioVolumeIndication:(NSInteger)interval smooth:(NSInteger)smooth;

/**
 *  设置日志文件
 *
 *  @param filePath 日志文件的完整路径。该日志文件为UTF-8编码。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setLogFile:(NSString*)filePath;

/**
 *  设置日志文件过滤器
 *
 *  @param filter 日志过滤器
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setLogFilter:(TTTRtcLogFilter)filter;

/**
 *  往日志文件中追加自定义日志（进入房间后才可使用）
 *
 *  @param content 日志内容
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)appendLogContent:(NSString *)content;

/**
 *  设置信令超时时间
 *
 *  @param seconds 超时时间（秒）
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setSignalTimeout:(NSUInteger)seconds;

/**
 *  设置音频高音质选项
 *
 *  @param fullband    全频带编解码器（48kHz采样率）
 *  @param stereo      立体声编解码器
 *  @param fullBitrate 高码率模式，建议仅在纯音频模式下使用。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setHighQualityAudioParametersWithFullband:(BOOL)fullband stereo:(BOOL)stereo fullBitrate:(BOOL)fullBitrate;

/**
 *  设置视频优化选项
 *
 *  @param preferFrameRateOverImageQuality True: 画质和流畅度里，优先保证流畅度，False: 画质和流畅度里，优先保证画质(默认)。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setVideoQualityParameters:(BOOL)preferFrameRateOverImageQuality;

/**
 *  配置旁路直播推流
 *
 *  @param config 直播推流配置
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)configPublisher:(TTTPublisherConfiguration *)config;

/**
 *  设置画中画布局
 *
 *  @param layout 画中画布局
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setVideoCompositingLayout:(TTTRtcVideoCompositingLayout*)layout;

/**
 *  取消画中画布局设置
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)clearVideoCompositingLayout;

/**
 *  配置外部视频源
 *  如果使用外部视频源，在“enableVideo/startPreview”之前调用本API。
 *
 *  @param enable     是否使用外部视频源
 *  @param useTexture 是否使用 Texture 作为输入
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setExternalVideoSource:(BOOL)enable useTexture:(BOOL)useTexture;

/**
 *  推送外部视频帧
 *
 *  @param videoFrame 包含待编码数据的视频帧
 *
 *  @return YES: 该帧推送成功，NO: 该帧推送失败。
 */
- (BOOL)pushExternalVideoFrame:(TTTRtcVideoFrame *)videoFrame;

/**
 *  设置本地视频帧采集格式
 *  如果不设置，回调rtcEngine:localVideoFrameCaptured:videoFrame:默认格式为“BGRA”。
 *
 *  @param format 视频帧的格式
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setLocalVideoFrameCaptureFormat:(TTTRtcVideoFrameFormat)format;

/**
 *  设置远端视频帧输出格式
 *  如果不设置，回调rtcEngine:remoteVideoFrameDecodedOfUid:videoFrame:默认格式为“NV12”。
 *
 *  @param format 视频帧的格式
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setRemoteVideoFrameOutputFormat:(TTTRtcVideoFrameFormat)format;

/**
 *  开始客户端本地混音
 *
 *  @param filePath 指定需要混音的本地音频文件名和文件路径
 *  @param loopback True: 只有本地可以听到混音或替换后的音频流，False: 本地和对方都可以听到混音或替换后的音频流。
 *  @param replace  True: 音频文件内容将会替换本地录音的音频流，False: 音频文件内容将会和麦克风采集的音频流进行混音。
 *  @param cycle    指定音频文件循环播放的次数
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startAudioMixing:(NSString *)filePath loopback:(BOOL)loopback replace:(BOOL)replace cycle:(NSInteger)cycle;

/**
 *  停止客户端本地混音
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)stopAudioMixing;

/**
 *  暂停播放伴奏
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)pauseAudioMixing;

/**
 *  恢复播放伴奏
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)resumeAudioMixing;

/**
 *  调节伴奏音量
 *
 *  @param volume 伴奏音量范围为0~100。默认100为原始文件音量。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)adjustAudioMixingVolume:(NSInteger)volume;

/**
 *  获取伴奏时长
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)getAudioMixingDuration;

/**
 *  获取伴奏播放进度
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)getAudioMixingCurrentPosition;

/**
 *  拖动语音进度条
 *
 *  @param pos 进度条位置，单位为毫秒。
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setAudioMixingPosition:(NSInteger)pos;

/**
 *  踢出房间
 *  角色为“TTTRtc_ClientRole_Anchor”调用有效
 *
 *  @param uid 被踢者userID
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)kickChannelUser:(int64_t)uid;

/**
 *  发送聊天消息
 *
 *  @param userID   用户ID
 *  @param chatType 聊天消息类型
 *  @param seqID    唯一标识
 *  @param data     消息内容
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)sendChatMessageWithUserID:(int64_t)userID chatType:(TTTRtcChatType)chatType seqID:(NSString *)seqID data:(NSString *)data;

/**
 *  发送信令
 *
 *  @param userID 用户ID
 *  @param seqID  唯一标识
 *  @param data   信令内容
 */
- (int)sendSignalWithUserID:(int64_t)userID seqID:(NSString *)seqID data:(NSString *)data;

/**
 *  开始采集语音消息
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startRecordChatAudio;

/**
 *  停止采集并开始发送消息
 *
 *  @param userID 目标用户ID，0表示发送给房间内的所有人。
 *  @param seqID  消息唯一标识
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)stopRecordAndSendChatAudioWithUserID:(int64_t)userID seqID:(NSString *)seqID;

/**
 *  取消语音消息录制
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)cancelRecordChatAudio;

/**
 *  开始播放语音消息
 *
 *  @param fileName 语音消息文件名
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startPlayChatAudioFileName:(NSString *)fileName;

/**
 *  停止播放语音消息
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)stopPlayChatAudio;

/**
 *  是否正在播放语音消息
 *
 *  @return YES: 正在播放，NO: 不在播放。
 */
- (BOOL)isChatAudioPlaying;

#if TARGET_OS_IPHONE

/**
 *  是否正在屏幕录制
 *
 *  @return YES: 是，NO: 否。
 */
- (BOOL)isScreenRecording;

/**
 *  开始录制屏幕用作视频源
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startRecordScreen NS_AVAILABLE_IOS(11_0);

/**
 *  开始录制屏幕并保存
 *
 *  @param path   保存路径，如果值为nil，保存到系统相册。
 *  @param width  视频宽度
 *  @param height 视频高度
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startRecordScreenAndSave:(NSString *)path width:(NSInteger)width height:(NSInteger)height NS_AVAILABLE_IOS(11_0);

/**
 *  停止录制屏幕
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)stopRecordScreen;

#endif

/**
 *  开始RTMP推流
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)startRtmpPublish:(NSString *)rtmpUrl;

/**
 *  停止RTMP推流
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)stopRtmpPublish;

/**
 *  暂停RTMP推流
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)pauseRtmpPublish;

/**
 *  恢复RTMP推流
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)resumeRtmpPublish;

/**
 *  设置SEI
 *
 *  @param SEI JSON格式的SEI
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setSEI:(NSString *)SEI;

/**
 * 向h.264码流中插入sei内容
 *
 * @param content 字符串内容
 *
 * @return 0:成功
 */
- (int)insertH264SeiContent:(NSString*)content;

/**
 *  设置是否启用视频双流模式
 *
 *  @param enabled 是否启用视频双流模式
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)enableDualStreamMode:(BOOL)enabled;

/**
 *  设置视频大小流
 *  如果远端用户选择发送双流(视频大流和小流), 该方法指定接收远端用户的视频流大小。
 *
 *  @param uid        用户ID
 *  @param streamType 视频流类型
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)setRemoteVideoStream:(int64_t)uid type:(TTTRtcVideoStreamType)streamType;

/**
 *  跨房间连麦，与其他主播连麦
 *
 *  @param sessionID 对方主播的会话ID
 *  @param userID    对方主播的用户ID
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)linkOtherAnchor:(int64_t)sessionID userID:(int64_t)userID;

/**
 *  跨房间连麦，结束与其他主播连麦
 *
 *  @param sessionID 对方主播的会话ID
 *  @param userID    对方主播的用户ID
 *
 *  @return 0: 方法调用成功，<0: 方法调用失败。
 */
- (int)unlinkOtherAnchor:(int64_t)sessionID userID:(int64_t)userID;

/**
 *  设置CDN推流视频参数，在加入房间之前调用
 *
 *  @param videoSize      视频分辨率
 *  @param videoFrameRate 视频帧率
 *  @param videoBitRate   视频码率
 *
 */
- (void)setVideoMixerParams:(CGSize)videoSize videoFrameRate:(NSUInteger)videoFrameRate videoBitRate:(NSUInteger)videoBitRate;

/**
 *  设置CDN推流音频参数
 *
 *  @param samplerate 采样率
 *  @param channels   1单声道，2双声道
 *
 */
- (void)setAudioMixerParams:(NSUInteger)samplerate channels:(NSUInteger)channels;

/**
 *  设置混屏背景图片
 *  当前版本仅支持进房间前进行设置，退出房间清空，再次进房间需要重新设置。
 *
 *  @param url 背景图片所在的地址
 */
- (void)setVideoMixerBackgroundImgUrl:(NSString*)url;

@end

/**
 *  SDK回调代理，由调用方实现。
 */
@protocol TTTRtcEngineDelegate <NSObject>

@optional

/**
 *  发生错误回调
 *  通常情况下，SDK上报的错误意味着SDK无法自动恢复，需要应用程序干预或提示用户。
 *
 *  @param errorCode 错误代码
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didOccurError:(TTTRtcErrorCode)errorCode;

/**
 *  网络连接丢失回调
 */
- (void)rtcEngineConnectionDidLost:(TTTRtcEngineKit *)engine;

/**
 *  当网络异常断开后，将尝试重连，
 *  若在服务器容忍的超时范围外才重连上服务器，服务器将会拒绝，其房间状态将不可用。
 *  此时触发该回调，上层应该在收到此回调后退出房间。
 */
- (void)rtcEngineReconnectServerTimeout:(TTTRtcEngineKit *)engine;

/**
 *  加入频道成功回调
 *
 *  @param channel 频道名
 *  @param uid     用户ID
 *  @param elapsed 从joinChannel开始到该事件产生的延迟（毫秒）
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didJoinChannel:(NSString*)channel withUid:(int64_t)uid elapsed:(NSInteger) elapsed;

/**
 *  成功离开频道的回调
 *
 *  @param stats 统计数据
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didLeaveChannelWithStats:(TTTRtcStats*)stats;

/**
 *  用户加入回调
 *
 *  @param uid            用户ID
 *  @param clientRole     用户角色
 *  @param isVideoEnabled 是否启用本地视频
 *  @param elapsed        加入频道开始到该回调触发的延迟（毫秒)
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didJoinedOfUid:(int64_t)uid clientRole:(TTTRtcClientRole)clientRole
    isVideoEnabled:(BOOL)isVideoEnabled elapsed:(NSInteger)elapsed;

/**
 *  用户离线回调
 *
 *  @param uid    用户ID
 *  @param reason 离线原因
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didOfflineOfUid:(int64_t)uid reason:(TTTRtcUserOfflineReason)reason;

/**
 *  用户被踢出回调
 *
 *  @param uid    用户ID
 *  @param reason 用户被踢出的原因
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didKickedOutOfUid:(int64_t)uid reason:(TTTRtcKickedOutReason)reason;

/**
 *  禁止/允许用户发言回调
 *
 *  @param muted YES: 禁止发言，NO: 允许发言。
 *  @param uid   用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didSpeakingMuted:(BOOL)muted ofUid:(int64_t)uid;

/**
 *  用户音频静音回调
 *
 *  @param muted YES: 静音，NO: 取消静音。
 *  @param uid   用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didAudioMuted:(BOOL)muted byUid:(int64_t)uid;

/**
 *  用户启用/关闭视频回调
 *
 *  @param enabled YES: 该用户已启用了视频功能，NO: 该用户已关闭了视频功能。
 *  @param uid     用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didVideoEnabled:(BOOL)enabled byUid:(int64_t)uid;

/**
 *  音频输出路由发生变化
 *
 *  @param routing 当前音频输出路由
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine didAudioRouteChanged:(TTTRtcAudioOutputRouting)routing;

/**
 *  本地视频统计回调
 *
 *  @param stats 本地视频的统计信息
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine localVideoStats:(TTTRtcLocalVideoStats*)stats;

/**
 *  本地音频统计回调
 *
 *  @param stats 本地音频的统计信息
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine localAudioStats:(TTTRtcLocalAudioStats*)stats;

/**
 *  远端视频统计回调
 *
 *  @param stats 远端视频的统计信息
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine remoteVideoStats:(TTTRtcRemoteVideoStats*)stats;

/**
 *  远端音频统计回调
 *
 *  @param stats 远端音频的统计信息
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine remoteAudioStats:(TTTRtcRemoteAudioStats*)stats;

/**
 *  远端用户音量回调
 *  提示谁在说话及其音量，默认禁用。可通过enableAudioVolumeIndication方法设置。
 *
 *  @param userID              用户ID
 *  @param audioLevel          非线性区间[0,9]
 *  @param audioLevelFullRange 线性区间[0,32768]
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine reportAudioLevel:(int64_t)userID
       audioLevel:(NSUInteger)audioLevel audioLevelFullRange:(NSUInteger)audioLevelFullRange;

/**
 *  远端首次音频数据接收回调
 *
 *  @param uid     用户ID，指定是哪个用户的音频数据
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine firstRemoteAudioDataIncommingOfUid:(int64_t)uid;

/**
 *  本地首帧视频显示回调
 *
 *  @param size    视频流尺寸（宽度和高度）
 *  @param elapsed 加入频道开始到该回调触发的延迟（毫秒)
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine firstLocalVideoFrameWithSize:(CGSize)size elapsed:(NSInteger)elapsed;

/**
 *  远端首帧视频接收解码回调
 *
 *  @param uid     用户ID，指定是哪个用户的视频流
 *  @param size    视频流尺寸（宽度和高度）
 *  @param elapsed 加入频道开始到该回调触发的延迟（毫秒)
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine firstRemoteVideoFrameDecodedOfUid:(int64_t)uid size:(CGSize)size elapsed:(NSInteger)elapsed;

/**
 *  摄像头启用回调
 */
- (void)rtcEngineCameraDidReady:(TTTRtcEngineKit *)engine;

/**
 *  视频功能停止回调
 */
- (void)rtcEngineVideoDidStop:(TTTRtcEngineKit *)engine;

/**
 *  本地视频采集回调
 *
 *  @param videoFrame 视频帧
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine localVideoFrameCaptured:(TTTRtcVideoFrame *)videoFrame;

/**
 *  远端视频接收解码回调
 *
 *  @param uid        用户ID，指定是哪个用户的视频流
 *  @param videoFrame 视频帧
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine remoteVideoFrameDecodedOfUid:(int64_t)uid videoFrame:(TTTRtcVideoFrame *)videoFrame;

/**
 *  设置SEI的回调
 *
 *  @param SEI JSON格式的SEI
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onSetSEI:(NSString *)SEI;

/**
 *  发送聊天消息的回调
 *
 *  @param chatInfo  聊天信息
 *  @param errorCode 错误代码
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine chatMessageSentOfChatInfo:(TTTRtcChatInfo *)chatInfo errorCode:(TTTRtcErrorCode)errorCode;

/**
 *  收到聊天消息的回调
 *
 *  @param userID   用户ID
 *  @param chatInfo 聊天信息
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine chatMessageReceivedOfUserID:(int64_t)userID chatInfo:(TTTRtcChatInfo *)chatInfo;

/**
 *  发送信令的回调
 *
 *  @param seqID     唯一标识
 *  @param errorCode 错误代码
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine signalSentOfSeqID:(NSString *)seqID data:(NSString *)data errorCode:(TTTRtcErrorCode)errorCode;

/**
 *  收到信令的回调
 *
 *  @param userID 用户ID
 *  @param seqID  唯一标识
 *  @param data   信令内容
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine signalReceivedOfUserID:(int64_t)userID seqID:(NSString *)seqID data:(NSString *)data;

/**
 *  语音消息播放完成的回调
 *
 *  @param fileName 语音消息文件名
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine chatAudioDidFinishPlaying:(NSString *)fileName;

/**
 *  RTMP推流回调
 *
 *  @param status RTMP推流状态
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onStatusOfRtmpPublish:(TTTRtcRtmpPublishStatus)status;


- (void)rtcEngine:(TTTRtcEngineKit *)engine reportH264SeiContent:(NSString*)content uid:(int64_t)uid;

- (void)rtcEngineOnMediaSending:(TTTRtcEngineKit *)engine;

/**
 *  远端用户是否启用双流的回调
 *
 *  @param enabled 是否启用了双流
 *  @param userID  用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine dualStreamModeEnabled:(BOOL)enabled userID:(int64_t)userID;

/**
 *  跨房间连麦，与其他主播连麦成功或失败的回调
 *  调用linkOtherAnchor后触发
 *
 *  @param sessionID 对方主播的会话ID
 *  @param userID    对方主播的用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onAnchorEnter:(int64_t)sessionID userID:(int64_t)userID error:(TTTRtcErrorCode)error;

/**
 *  跨房间连麦，结束与其他主播连麦的回调
 *  调用unlinkOtherAnchor后触发
 *
 *  @param sessionID 对方主播的会话ID
 *  @param userID    对方主播的用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onAnchorExit:(int64_t)sessionID userID:(int64_t)userID;

/**
 *  跨房间连麦，其他主播向“我”发起连麦的回调
 *
 *  @param sessionID 对方主播的会话ID
 *  @param userID    对方主播的用户ID
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onAnchorLinkResponse:(int64_t)sessionID userID:(int64_t)userID;

/**
 *  跨房间连麦，其他主播结束与“我”连麦的回调
 *
 *  @param sessionID 对方主播的会话ID
 *  @param userID    对方主播的用户ID
 *  @param reason    连麦主播离开原因
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine onAnchorUnlinkResponse:(int64_t)sessionID userID:(int64_t)userID reason:(TTTRtcAnchorExitReason)reason;

/**
 *  请求改变视频编码参数
 *  当网络发生拥塞时，会请求下调码率，反之会请求上调码率
 *
 *  @param frameRate 帧率
 *  @param bitRate   码率
 */
- (void)rtcEngine:(TTTRtcEngineKit *)engine changeVideoEncodeParams:(NSUInteger)frameRate bitRate:(NSUInteger)bitRate;

/**
 *  跟视频媒体服务器连接断开（会尝试重连）
 *
 *  @param mediaID 视频设备ID
 */

- (void)rtcEngine:(TTTRtcEngineKit *)engine videoConnectFailed:(NSString *)mediaID;

@end