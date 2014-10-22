//
//  PSPDFRenderQueue.h
//  PSPDFKit
//
//  Copyright (c) 2012-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

@class PSPDFDocument, PSPDFRenderJob, PSPDFRenderQueue, PSPDFRenderReceipt;

/// Notification that will be thrown when we enqueue a job.
extern NSString *const PSPDFRenderQueueDidEnqueueJob;

/// Notification that will be thrown after a job finished. (in addition to the delegate)
extern NSString *const PSPDFRenderQueueDidFinishJob;

/// Notification that will be thrown when we cancel a job.
extern NSString *const PSPDFRenderQueueDidCancelJob;

/// Absolute limit for image rendering (memory constraint)
extern CGSize const PSPDFRenderSizeLimit;

/// Implement this delegate to get rendered pages. (Most of the times, you want to use `PSPDFCache` instead)
@protocol PSPDFRenderDelegate <NSObject>

/// Called when a render job finished. Guaranteed to be called from the main thread.
- (void)renderQueue:(PSPDFRenderQueue *)renderQueue jobDidFinish:(PSPDFRenderJob *)job;

@end

typedef NS_ENUM(NSUInteger, PSPDFRenderQueuePriority) {
    PSPDFRenderQueuePriorityVeryLow,  /// Used to re-render annotation changes.
    PSPDFRenderQueuePriorityLow,      /// Low and ReallyLow are used from within `PSPDFCache`.
    PSPDFRenderQueuePriorityNormal,   /// Life page renderings.
    PSPDFRenderQueuePriorityHigh,     /// Zoomed renderings.
    PSPDFRenderQueuePriorityVeryHigh, /// Highest priority. Unused.
};

typedef void(^PSPDFRenderJobCompletionBlock)(PSPDFRenderJob *renderJob, PSPDFRenderQueue *renderQueue);

/// Render Queue. Does not cache. Used for rendering pages/page parts in `PSPDFPageView`.
@interface PSPDFRenderQueue : NSObject

/// Render Queue is a singleton.
+ (instancetype)sharedRenderQueue;

/// @name Requests

/// Requests a (freshly) rendered image from a specified document. Does not use the file cache.
/// For options, see `PSPDFPageRender`.
/// IF `queueAsNext` is set, the request will be processed ASAP, skipping the current queue.
- (PSPDFRenderJob *)requestRenderedImageForDocument:(PSPDFDocument *)document page:(NSUInteger)page size:(CGSize)size clippedToRect:(CGRect)clipRect annotations:(NSArray *)annotations options:(NSDictionary *)options priority:(PSPDFRenderQueuePriority)priority queueAsNext:(BOOL)queueAsNext delegate:(id<PSPDFRenderDelegate>)delegate completionBlock:(PSPDFRenderJobCompletionBlock)completionBlock;

/// Return all queued jobs for the current `document` and `page`. (bound to `delegate`)
- (NSArray *)renderJobsForDocument:(PSPDFDocument *)document page:(NSUInteger)page delegate:(id<PSPDFRenderDelegate>)delegate;

/// Returns YES if currently a RenderJob is scheduled or running for delegate.
- (BOOL)hasRenderJobsForDelegate:(id<PSPDFRenderDelegate>)delegate;

/// Return how many jobs are currently queued.
- (NSUInteger)numberOfQueuedJobs;

/// @name Cancellation

/// Cancel a single render job.
/// @return YES if cancellation was successful, NO if not found.
- (BOOL)cancelJob:(PSPDFRenderJob *)job onlyIfQueued:(BOOL)onlyIfQueued;

/// Cancel all queued and running jobs.
- (void)cancelAllJobs;

/// Cancel job. Must be the identical object to the job queued.
/// Use `NSNotFound` for `page` to delete all requests for the document.
- (void)cancelJobsForDocument:(PSPDFDocument *)document page:(NSUInteger)page delegate:(id<PSPDFRenderDelegate>)delegate includeRunning:(BOOL)includeRunning;

/// Cancels all queued render-calls.
- (void)cancelJobsForDelegate:(id<PSPDFRenderDelegate>)delegate;

/// @name Settings

/// The minimum priority for requests. Defaults to `PSPDFRenderQueuePriorityVeryLow`.
/// Set to `PSPDFRenderQueuePriorityNormal` to temporarily pause cache requests.
@property (nonatomic, assign) PSPDFRenderQueuePriority minimumProcessPriority;

/// Amount of render requests that run at the same time. Defaults to 1.
/// @note Apple's PDF renderer has concurrency issues, increasing this value might reduce the framework stability.
@property (atomic, assign) NSUInteger concurrentRunningRenderRequests;

@end

// A render job is designed to be created and then treated as immutable.
// The internal hash is cached and you'll get weird results if renderJob is changed after being added to the queue.
@interface PSPDFRenderJob : NSObject

@property (nonatomic, strong, readonly) PSPDFDocument *document;
@property (nonatomic, assign, readonly) NSUInteger page;
@property (nonatomic, assign, readonly) CGSize size;
@property (nonatomic, assign, readonly) CGRect clipRect;
@property (nonatomic, assign, readonly) float zoomScale;
@property (nonatomic, copy,   readonly) NSArray *annotations;
@property (nonatomic, assign, readonly) PSPDFRenderQueuePriority priority;
@property (nonatomic, copy,   readonly) NSDictionary *options;
@property (nonatomic, weak,   readonly) id<PSPDFRenderDelegate> delegate;
@property (nonatomic, copy)             PSPDFRenderJobCompletionBlock completionBlock;
@property (nonatomic, strong, readonly) UIImage *renderedImage;
@property (nonatomic, strong, readonly) PSPDFRenderReceipt *renderReceipt;
@property (nonatomic, assign, readonly) uint64_t renderTime;

@end

/// Gets a 'receipt' of the current render operation, allows to compare different renders of the same page.
@interface PSPDFRenderReceipt : NSObject <NSSecureCoding>

- (instancetype)initWithDocument:(PSPDFDocument *)document page:(NSUInteger)page size:(CGSize)size
        clipRect:(CGRect)clipRect annotations:(NSArray *)annotations options: (NSDictionary *)options;

@property (nonatomic, copy) NSString *renderFingerprint;
@property (nonatomic, assign) double timeInNanoseconds; // Not persisted. Statistic feature only.
@end
