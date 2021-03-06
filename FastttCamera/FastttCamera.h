//
//  FastttCamera.h
//  FastttCamera
//
//  Created by Laura Skelton on 2/5/15.
//  Copyright (c) 2015 IFTTT. All rights reserved.
//

@import UIKit;
#import "FastttCapturedImage.h"

typedef NS_ENUM(NSInteger, FastttCameraDevice) {
    FastttCameraDeviceFront,
    FastttCameraDeviceRear
};

typedef NS_ENUM(NSInteger, FastttCameraFlashMode) {
    FastttCameraFlashModeOff,
    FastttCameraFlashModeOn,
    FastttCameraFlashModeAuto
};

@protocol FastttCameraDelegate;

@interface FastttCamera : UIViewController

#pragma mark - FastttCameraDelegate

/**
 *  The delegate of the FastttCamera instance.
 */
@property (nonatomic, weak) id <FastttCameraDelegate> delegate;


#pragma mark - Advanced Configuration Options

/**
 *  Default is YES. Set this to NO if you don't want to enable
 *  FastttCamera to manage tap-to-focus with its internal tap gesture recognizer.
 *  You can still send it manual focusAtPoint: calls from your own gesture recognizer.
 */
@property (nonatomic, assign) BOOL handlesTapFocus;

/**
 *  Default is YES. Set this to NO if you don't want the focus square to show when
 *  the camera is focusing at a point.
 */
@property (nonatomic, assign) BOOL showsFocusView;

/**
 *  Defaults to YES. Set this to NO if you want FastttCamera to return the full image
 *  captured by the camera instead of an image cropped to the view's aspect ratio. The
 *  image will be returned by the cameraController:didFinishCapturingImage: delegate method,
 *  in the fullImage property of the FastttCapturedImage object.
 *  cameraController:didFinishNormalizingCapturedImage: is the only other method that will
 *  be called, and only if normalizesImageOrientations == YES.
 */
@property (nonatomic, assign) BOOL cropsImageToVisibleAspectRatio;

/**
 *  Defaults to YES. Set this to NO if you don't want FastttCamera to return a scaled version of the
 *  full captured image. The scaled image will be returned in the scaledImage property of the
 *  FastttCapturedImage object, and will trigger the cameraController:didFinishScalingCapturedImage:
 *  delegate method when it is available.
 */
@property (nonatomic, assign) BOOL scalesImage;

/**
 *  Defaults to scaling the cropped image to fit within the size of the camera preview. If you'd like to
 *  set an explicit max dimension for scaling the image, set it here. This can be useful if you have specific
 *  requirements for uploading the image.
 */
@property (nonatomic, assign) CGFloat maxScaledDimension;

/**
 *  Defaults to YES. Set this to NO if you would like to only use the images initially returned by FastttCamera 
 *  and don't need the versions returned that have been rotated so that their orientation is UIImageOrientationUP.
 *  If true, normalized images will replace the initial images in the FastttCapturedImage object when they are finished
 *  processing in the background, and the cameraController:didFinishNormalizingCapturedImage: delegate method will
 *  notify you that they are ready.
 */
@property (nonatomic, assign) BOOL normalizesImageOrientations;

/**
 *  Defaults to YES. Set this to NO if you don't want to display the captured image preview to the user in the same orientation
 *  that it was captured, or if you are already rotating your interface to account for this.
 */
@property (nonatomic, assign) BOOL returnsRotatedPreview;

/**
 *  Defaults to YES. Set this to NO if your interface does not autorotate with device orientation to make sure that preview
 *  images are still displayed correctly when orientation lock is off but your interface stays in portrait.
 */
@property (nonatomic, assign) BOOL interfaceRotatesWithOrientation;


#pragma mark - Camera State

/**
 *  The current camera device.
 */
@property (nonatomic, assign) FastttCameraDevice cameraDevice;

/**
 *  The current flash mode.
 */
@property (nonatomic, assign) FastttCameraFlashMode cameraFlashMode;

/**
 *  Check if flash is available for the specified camera device.
 *
 *  @param cameraDevice The camera device to check for flash availability.
 *
 *  @return YES if flash is available, NO if not.
 */
+ (BOOL)isFlashAvailableForCameraDevice:(FastttCameraDevice)cameraDevice;

/**
 *  Check if point focus is available for the specified camera device.
 *
 *  @param cameraDevice The camera device to check for point focus availability.
 *
 *  @return YES if point focus is available, NO if not.
 */
+ (BOOL)isPointFocusAvailableForCameraDevice:(FastttCameraDevice)cameraDevice;

/**
 *  Check if the specified camera device is available on this device.
 *
 *  @param cameraDevice The camera device to check for.
 *
 *  @return YES if the specified camera is available, NO if it is not.
 */
+ (BOOL)isCameraDeviceAvailable:(FastttCameraDevice)cameraDevice;

/**
 *  Focus the camera at the specified point, if focus at point is available on the current camera device.
 *  You only need to worry about this if you set handlesTapFocus to NO, and want to manually control
 *  tap-to-focus.
 *
 *  @param touchPoint The point at which to focus the camera, if point focus is available.
 */
- (void)focusAtPoint:(CGPoint)touchPoint;

#pragma mark - Take a picture!

/**
 *  Triggers the camera to take a photo.
 */
- (void)takePicture;

#pragma mark - Process a photo

/**
 *  Scale the image to the given max dimension and trigger the delegate callbacks
 *  with a capturedImage object similarly to takePicture.
 *
 *  @note This will always trigger cameraController:didFinishCapturingImage: and cameraController:didFinishScalingCapturedImage:
 *  and will trigger cameraController:didFinishNormalizingCapturedImage: if normalizesImageOrientations is set to YES.
 *
 *  @param image The image to process.
 *  @param maxDimension The maximum dimension of the target size for aspect scaling the image.
 */
- (void)processImage:(UIImage *)image withMaxDimension:(CGFloat)maxDimension;

/**
 *  Crop the image to the given cropRect and trigger the delegate callbacks
 *  with a capturedImage object similarly to takePicture.
 *
 *  @param image The image to process.
 *  @param cropRect The CGRect to use for cropping the image.
 *
 *  @note This will always trigger cameraController:didFinishCapturingImage:, will never trigger cameraController:didFinishScalingCapturedImage:,
 *  and will trigger cameraController:didFinishNormalizingCapturedImage: if normalizesImageOrientations is set to YES.
 */
- (void)processImage:(UIImage *)image withCropRect:(CGRect)cropRect;

/**
 *  Crop the image to the given cropRect and scale the image to the given max dimension and trigger 
 *  the delegate callbacks with a capturedImage object similarly to takePicture.
 *
 *  @param image The image to process.
 *  @param maxDimension The maximum dimension of the target size for aspect scaling the image.
 *
 *  @note This will always trigger cameraController:didFinishCapturingImage: and cameraController:didFinishScalingCapturedImage:
 *  and will trigger cameraController:didFinishNormalizingCapturedImage: if normalizesImageOrientations is set to YES.
 */
- (void)processImage:(UIImage *)image withCropRect:(CGRect)cropRect maxDimension:(CGFloat)maxDimension;

@end

#pragma mark - FastttCameraDelegate

@protocol FastttCameraDelegate <NSObject>

@optional

/**
 *  Called when the camera controller has finished capturing a photo.
 *
 *  @param cameraController The FastttCamera instance that captured a photo.
 *
 *  @param capturedImage The FastttCapturedImage object, containing a full-resolution (UIImage *)fullImage that has not 
 *  yet had its orientation normalized (it has not yet been rotated so that its orientation is UIImageOrientationUp),
 *  and a (UIImage *)previewImage that has its image orientation set so that it is rotated to match the camera preview's 
 *  orientation as it was captured, so if the device was held landscape left, the image returned will be set to display so 
 *  that landscape left is "up". This is great if your interface doesn't rotate, or if the photo was taken with orientation lock on.
 *
 *  @note if you set returnsRotatedPreview=NO, there will be no previewImage here, and if you set cropsImageToVisibleAspectRatio=NO,
 *  the fullImage will be the raw image captured by the camera, while by default the fullImage will have been cropped to the visible
 *  camera preview's aspect ratio.
 */
- (void)cameraController:(FastttCamera *)cameraController didFinishCapturingImage:(FastttCapturedImage *)capturedImage;

/**
 *  Called when the camera controller has finished scaling the captured photo.
 *
 *  @param cameraController The FastttCamera instance that captured a photo.
 *
 *  @param capturedImage    The FastttCapturedImage object, which now also contains a scaled (UIImage *)scaledImage, that has not yet
 *  had its orientation normalized. The image by default is scaled to fit within the camera's preview window, but you can
 *  set a custom maxScaledDimension above.
 *
 *  @note This method will not be called if scalesImage is set to NO.
 */
- (void)cameraController:(FastttCamera *)cameraController didFinishScalingCapturedImage:(FastttCapturedImage *)capturedImage;

/**
 *  Called when the camera controller has finished normalizing the captured photo.
 *
 *  @param cameraController The FastttCamera instance that captured the photo.
 *
 *  @param capturedImage    The FastttCapturedImage object, with the (UIImage *)fullImage and (UIImage *)scaledImage (if any) replaced
 *  by images that have been rotated so that their orientation is UIImageOrientationUp. This is a slower process than creating the
 *  initial images that are returned, which have varying orientations based on how the phone was held, but the normalized images
 *  are more ideal for uploading or saving as they are displayed more predictably in different browsers and applications than the
 *  initial images which have an orientation tag set that is not UIImageOrientationUp.
 *
 *  @note This method will not be called if normalizesImageOrientations is set to NO.
 */
- (void)cameraController:(FastttCamera *)cameraController didFinishNormalizingCapturedImage:(FastttCapturedImage *)capturedImage;

@end
