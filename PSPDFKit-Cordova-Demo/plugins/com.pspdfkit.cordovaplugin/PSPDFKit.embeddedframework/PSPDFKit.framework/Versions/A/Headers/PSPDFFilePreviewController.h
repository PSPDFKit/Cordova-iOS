//
//  PSPDFFilePreviewController.h
//  PSPDFKit
//
//  Copyright (c) 2013-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import <QuickLook/QuickLook.h>

/// Use QuickLook to preview an item.
@interface PSPDFFilePreviewController : QLPreviewController <QLPreviewControllerDataSource, QLPreviewControllerDelegate>

/// Designated initializer.
- (instancetype)initWithPreviewURL:(NSURL *)previewURL;

/// URL to then item that should be previewed.
@property (nonatomic, copy) NSURL *previewURL;

/// Can be set to provide a better animation. Optional.
@property (nonatomic, assign) CGRect sourceRect;

@end
