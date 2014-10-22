//
//  PSPDFThumbnailBar.h
//  PSPDFKit
//
//  Copyright (c) 2013-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "PSPDFCache.h"
#import "PSPDFPresentationContext.h"

@class PSPDFThumbnailBar;

/// Delegate for thumbnail actions.
@protocol PSPDFThumbnailBarDelegate <NSObject>

@optional

/// A thumbnail has been selected.
- (void)thumbnailBar:(PSPDFThumbnailBar *)thumbnailBar didSelectPage:(NSUInteger)page;

@end

/// Bottom bar that shows a scrollable list of thumbnails.
@interface PSPDFThumbnailBar : UICollectionView <UICollectionViewDataSource, UICollectionViewDelegate>

/// Delegate for the thumbnail controller.
@property (nonatomic, weak) id<PSPDFThumbnailBarDelegate> thumbnailBarDelegate;

/// The data source.
@property (nonatomic, weak) id <PSPDFPresentationContext> thumbnailBarDataSource;

/// Scrolls to specified page in the grid and centers the selected page.
- (void)scrollToPage:(NSUInteger)page animated:(BOOL)animated;

/// Stops an ongoing scroll animation.
- (void)stopScrolling;

/// Reload and keep the selection
- (void)reloadDataAndKeepSelection;

/// Thumbnail size. Defaults to 88x125 on iPad and 53x75 on iPhone.
@property (nonatomic, assign) CGSize thumbnailSize;

/// Set the default height of the thumbnail bar. Defaults to 135 on iPad and 85 on iPhone.
/// @note Set this before the toolbar is displayed.
@property (nonatomic, assign) CGFloat thumbnailBarHeight;

/// Show page labels. Defaults to NO.
@property (nonatomic, assign) BOOL showPageLabels;

@end
