//
//  PSPDFResizableView.h
//  PSPDFKit
//
//  Copyright (c) 2012-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "PSPDFLongPressGestureRecognizer.h"

@class PSPDFResizableView, PSPDFAnnotation, PSPDFPageView;

typedef NS_ENUM(NSUInteger, PSPDFResizableViewOuterKnob) {
    PSPDFResizableViewOuterKnobUnknown,
    PSPDFResizableViewOuterKnobTopLeft,
    PSPDFResizableViewOuterKnobTopMiddle,
    PSPDFResizableViewOuterKnobTopRight,
    PSPDFResizableViewOuterKnobMiddleLeft,
    PSPDFResizableViewOuterKnobMiddleRight,
    PSPDFResizableViewOuterKnobBottomLeft,
    PSPDFResizableViewOuterKnobBottomMiddle,
    PSPDFResizableViewOuterKnobBottomRight
};

/// Delegate to be notified on session begin/end and frame changes.
@protocol PSPDFResizableViewDelegate <NSObject>

@optional

/// The editing session has begun.
- (void)resizableViewDidBeginEditing:(PSPDFResizableView *)resizableView;

/// Called after frame change.
- (void)resizableViewChangedFrame:(PSPDFResizableView *)resizableView outerKnobType:(PSPDFResizableViewOuterKnob)outerKnobType;

/// The editing session has ended.
- (void)resizableViewDidEndEditing:(PSPDFResizableView *)resizableView;

@end

/// If trackedView responds to this delegate, we will set it.
@protocol PSPDFResizableTrackedViewDelegate <NSObject>

/// The resizable tracker, if currently available.
@property (nonatomic, weak) PSPDFResizableView *resizableView;

/// Query the annotation of the tracked view.
@property (nonatomic, strong, readonly) PSPDFAnnotation *annotation;

@end

typedef NS_ENUM(NSUInteger, PSPDFResizableViewMode) {
    PSPDFResizableViewModeIdle,   /// Nothing is currently happening.
    PSPDFResizableViewModeMove,   /// The annotation is being moved.
    PSPDFResizableViewModeResize, /// The annotation is being resized.
    PSPDFResizableViewModeAdjust  /// The shape of the annotation is being adjusted (e.g. polyline shape)
};

typedef NS_ENUM(NSUInteger, PSPDFResizableViewLimitMode) {
    PSPDFResizableViewLimitModeNone,            /// The view can bee freely moved outside of it's superview.
    PSPDFResizableViewLimitModeContentFrame,    /// The content frame has to stay inside the superview bounds.
    PSPDFResizableViewLimitModeBoundingBox,     /// The bounding box (blue) has to stay inside the superview bounds.
    PSPDFResizableViewLimitModeViewFrame        /// The entire resizable view frame has to stay inside the superview bounds.
};

/// Handles view selection with resize knobs.
@interface PSPDFResizableView : UIView <PSPDFLongPressGestureRecognizerDelegate>

/// Designated initializer.
/// This will call `self.trackedView`, so `trackedView` is the place where you'd want to override to dynamically set `allowResizing`.
- (instancetype)initWithTrackedView:(UIView *)trackedView;

/// View that will be changed on selection change.
@property (nonatomic, copy) NSSet *trackedViews;

/// Set zoomscale to be able to draw the page knobs at the correct size.
@property (nonatomic, assign) CGFloat zoomScale;

/// The inner edge insets are used to create space between the bounding box (blue) and tracked view.
/// They will be applied to the contentFrame in additon to outerEdgeInsets to calculate frame. Use negative
/// values to add space around the tracked annotation view. Defaults to -20.f for top, bottom, right, and left.
@property (nonatomic, assign) UIEdgeInsets innerEdgeInsets;

/// The outer edge insets are used to create space between the bounding box (blue) and the view bounds.
/// They will be applied to the contentFrame in additon to innerEdgeInsets to calculate frame.
/// Use negative values to add space around the tracked annotation view.
/// Defaults to `-40.0f` for top, bottom, right, and left.
@property (nonatomic, assign) UIEdgeInsets outerEdgeInsets;

/// Returns the edge insets that are currently in effect. This is either UIEdgeInsetsZero or innerEdgeInsets.
- (UIEdgeInsets)effectiveInnerEdgeInsets;

/// Returns the edge insets that are currently in effect. This is outerEdgeInsets / zoomScale.
- (UIEdgeInsets)effectiveOuterEdgeInsets;

/// If set to NO, won't show selection knobs and dragging. Defaults to YES.
@property (nonatomic, assign) BOOL allowEditing;

/// Allows view resizing, shows resize knobs.
/// If set to NO, view can only be moved or adjusted, no resize knobs will be displayed. Depends on allowEditing. Defaults to YES.
@property (nonatomic, assign) BOOL allowResizing;

/// Allows view adjusting, shows knobs to move single points.
/// If set to NO, view can only be moved or resized, no adjust knobs will be displayed. Depends on allowEditing. Defaults to YES.
@property (nonatomic, assign) BOOL allowAdjusting;

/// Enables resizing helper so that aspect ration can be preserved easily.
/// Defaults to YES.
@property (nonatomic, assign) BOOL enableResizingGuides;

/// Shows the bounding box. Defaults to YES.
@property (nonatomic, assign) BOOL showBoundingBox;

/// Defines how aggressively the guide works. Defaults to 20.f
@property (nonatomic, assign) CGFloat guideSnapAllowance;

/// Override the minimum allowed width. This value is ignored if the view is smaller to begin with
/// or the annotation specifies a bigger minimum width. Default is 0.f.
@property (nonatomic, assign) CGFloat minWidth;

/// Override the minimum allowed height. This value is ignored if the view is smaller to begin with
/// or the annotation specifies a bigger minimum height. Default is 0.f.
@property (nonatomic, assign) CGFloat minHeight;

/// Defines the reziable view behavor when dragged outside of it's superview.
/// Defaults to PSPDFResizableViewLimitModeContentFrame.
@property (nonatomic, assign) PSPDFResizableViewLimitMode limitMode;

/// Border color. Defaults to `[UIColor.pspdf_selectionColor colorWithAlphaComponent:0.6f]`.
@property (nonatomic, strong) UIColor *selectionBorderColor UI_APPEARANCE_SELECTOR;

/// Border size. Defaults to 1.f
@property (nonatomic, assign) CGFloat selectionBorderWidth UI_APPEARANCE_SELECTOR;

/// Guide color. Defaults to `UIColor.pspdf_guideColor`.
@property (nonatomic, strong) UIColor *guideBorderColor UI_APPEARANCE_SELECTOR;

/// Corner radius size. Defaults to 2.f.
@property (nonatomic, assign) NSUInteger cornerRadius UI_APPEARANCE_SELECTOR;

// forward parent gesture recognizer longPress action.
- (BOOL)longPress:(UILongPressGestureRecognizer *)recognizer;

/// Delegate called on frame change.
@property (nonatomic, weak) IBOutlet id<PSPDFResizableViewDelegate> delegate;

/// The frame of the resizable content. This might be smaller than the frame of the view.
/// Changing the content frame affects the frame.
///
/// @warning Always change the view's frame by setting this property. Do not use the frame property directly!
@property (nonatomic, assign) CGRect contentFrame;

/// The mode that the resizable view is currently in.
@property (nonatomic, assign) PSPDFResizableViewMode mode;

/// The associated pageView.
@property (nonatomic, weak) PSPDFPageView *pageView;

@end

@interface PSPDFResizableView (SubclassingHooks)

// All knobs. Can be hidden individually.
// Note that properties like `allowEditing`/`allowResizing` will update the hidden property.
// To properly hide a knob, remove it from the superview.
- (UIImageView *)outerKnobOfType:(PSPDFResizableViewOuterKnob)knobType;

// Allows to customize the position for a knob.
- (CGPoint)centerPointForOuterKnob:(PSPDFResizableViewOuterKnob)knobType;

@property (nonatomic, strong, readonly) UIImage *outerKnobImage;
@property (nonatomic, strong, readonly) UIImage *innerKnobImage;

@property (nonatomic, strong, readonly) PSPDFAnnotation *trackedAnnotation;

// Update the knobs.
- (void)updateKnobsAnimated:(BOOL)animated;

@end
