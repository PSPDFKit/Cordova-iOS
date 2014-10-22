//
//  PSPDFSegmentedControl.h
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

// Add support to replace images as the selection changes.
@interface PSPDFSegmentedControl : UISegmentedControl

// Allows to extend the hit testing area, if a negative inset is set.
@property (nonatomic, assign) UIEdgeInsets hitTestEdgeInsets;

@end

@interface PSPDFCustomImagesSegmentedControl : PSPDFSegmentedControl

// Allows to set images that are replaced when selected.
- (void)setSelectedImage:(UIImage *)image forSegmentAtIndex:(NSUInteger)segment;
- (UIImage *)selectedImageForSegmentAtIndex:(NSUInteger)segment;

@end
