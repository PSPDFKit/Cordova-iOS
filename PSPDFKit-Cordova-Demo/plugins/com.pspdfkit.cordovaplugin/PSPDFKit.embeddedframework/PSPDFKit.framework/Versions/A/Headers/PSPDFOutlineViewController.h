//
//  PSPDFOutlineViewController.h
//  PSPDFKit
//
//  Copyright (c) 2011-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import "PSPDFStatefulTableViewController.h"
#import "PSPDFStyleable.h"
#import "PSPDFOverridable.h"

@class PSPDFDocument, PSPDFOutlineViewController, PSPDFOutlineElement, PSPDFOutlineCell;

/// Delegate for the `PSPDFOutlineViewController`.
@protocol PSPDFOutlineViewControllerDelegate <PSPDFOverridable>

/// Called when we tapped on a cell in the `outlineController`.
/// Return NO if event is not processed.
- (BOOL)outlineController:(PSPDFOutlineViewController *)outlineController didTapAtElement:(PSPDFOutlineElement *)outlineElement;

@end

/// Outline (Table of Contents) view controller.
@interface PSPDFOutlineViewController : PSPDFStatefulTableViewController <UISearchDisplayDelegate, PSPDFStyleable>

/// Designated initializer.
- (instancetype)initWithDocument:(PSPDFDocument *)document delegate:(id<PSPDFOutlineViewControllerDelegate>)delegate NS_DESIGNATED_INITIALIZER;

/// Allow to long-press to copy the title. Defaults to YES.
@property (nonatomic, assign) BOOL allowCopy;

/// Allows search. Defaults to YES.
/// The UISearchBar is updated internally during reloading. To customize, use UIAppearance:
/// `[[UISearchBar appearanceWhenContainedIn:PSPDFOutlineViewController.class, nil] setBarStyle:UIBarStyleBlack];`
@property (nonatomic, assign) BOOL searchEnabled;

/// Enables displaying page labels.
@property (nonatomic, assign) BOOL showPageLabels;

/// How many lines should be displayed for a cell. Defaults to 4. 0 means unlimited.
@property (nonatomic, assign) NSUInteger maximumNumberOfLines;

/// Left intent width. Defaults to 32.f.
@property (nonatomic, assign) CGFloat outlineIntentLeftOffset;

/// Intent multiplier (will be added x times the intent level). Defaults to 15.f.
@property (nonatomic, assign) CGFloat outlineIndentMultiplier;

/// Delegate to communicate with `PSPDFViewController`.
@property (nonatomic, weak) IBOutlet id<PSPDFOutlineViewControllerDelegate> delegate;

/// Attached document.
@property (nonatomic, weak) PSPDFDocument *document;

@end


@interface PSPDFOutlineViewController (SubclassingHooks)

// Cell delegate - expand/shrink content.
- (void)outlineCellDidTapDisclosureButton:(PSPDFOutlineCell *)cell;

// Used on iOS 8+ only, otherwise `searchDisplayController` is used.
@property (nonatomic, strong, readonly) UISearchController *searchController;

// The search bar used with `UISearchController` or `UISearchDisplayController`.
@property (nonatomic, strong, readonly) UISearchBar *searchBar;

@end
