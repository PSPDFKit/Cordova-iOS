//
//  PSPDFAnnotation.h
//  PSPDFKit
//
//  Copyright (c) 2011-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import "PSPDFModel.h"
#import "PSPDFUndoProtocol.h"
#import "PSPDFJSONAdapter.h"

@class PSPDFDocument, PSPDFDocumentProvider;

/// List of available annotation types. Set in the `editableAnnotationTypes` set of `PSPDFDocument`.
extern NSString *const PSPDFAnnotationStringLink;
extern NSString *const PSPDFAnnotationStringHighlight;
extern NSString *const PSPDFAnnotationStringUnderline;
extern NSString *const PSPDFAnnotationStringStrikeOut;
extern NSString *const PSPDFAnnotationStringSquiggly;
extern NSString *const PSPDFAnnotationStringNote;
extern NSString *const PSPDFAnnotationStringFreeText;
extern NSString *const PSPDFAnnotationStringInk;
extern NSString *const PSPDFAnnotationStringSquare;
extern NSString *const PSPDFAnnotationStringCircle;
extern NSString *const PSPDFAnnotationStringLine;
extern NSString *const PSPDFAnnotationStringPolygon;
extern NSString *const PSPDFAnnotationStringPolyLine;
extern NSString *const PSPDFAnnotationStringSignature;  /// Signature is a `PSPDFAnnotationStringInk` annotation.
extern NSString *const PSPDFAnnotationStringStamp;

/// Sound annotations can be played back and recorded by default, but playback and recording will not work when the host app is in the background. If you want to enable background playback and recording, you'll need to add the "audio" entry to the `UIBackgroundModes` array in the app's Info.plist. If you do not add this, then recording will be stopped and playback will be silenced when your app is sent into the background.
extern NSString *const PSPDFAnnotationStringSound;

/// `UIImagePickerController` used in the image add feature will throw a `UIApplicationInvalidInterfaceOrientation` exception if your app does not include portrait in `UISupportedInterfaceOrientations` (Info.plist). For landscape only apps, we suggest enabling portrait orientation(s) in your Info.plist and rejecting these in `UIViewController's` auto-rotation methods. This way, you can be landscape only for your view controllers and still be able to use `UIImagePickerController`.
extern NSString *const PSPDFAnnotationStringImage;      // Image is a `PSPDFAnnotationStringStamp` annotation.

// Non-writable annotation types.
extern NSString *const PSPDFAnnotationStringWidget;     // Widget is currently handled similar to Link.
extern NSString *const PSPDFAnnotationStringFile;
extern NSString *const PSPDFAnnotationStringRichMedia;
extern NSString *const PSPDFAnnotationStringScreen;
extern NSString *const PSPDFAnnotationStringCaret;      // There's no menu entry for Caret.

// Placeholders. Not yet supported.
extern NSString *const PSPDFAnnotationStringPopup;
extern NSString *const PSPDFAnnotationStringWatermark;
extern NSString *const PSPDFAnnotationStringTrapNet;
extern NSString *const PSPDFAnnotationString3D;
extern NSString *const PSPDFAnnotationStringRedact;

/// PDF Annotations types.
typedef NS_OPTIONS(NSUInteger, PSPDFAnnotationType) {
    PSPDFAnnotationTypeNone        = 0,
    PSPDFAnnotationTypeUndefined   = 1 << 0,  /// Any annotation whose type couldn't be recognized.
    PSPDFAnnotationTypeLink        = 1 << 1,  /// Links and PSPDFKit multimedia extensions.
    PSPDFAnnotationTypeHighlight   = 1 << 2,
    PSPDFAnnotationTypeStrikeOut   = 1 << 17,
    PSPDFAnnotationTypeUnderline   = 1 << 18,
    PSPDFAnnotationTypeSquiggly    = 1 << 19,
    PSPDFAnnotationTypeFreeText    = 1 << 3,
    PSPDFAnnotationTypeInk         = 1 << 4,  /// Ink (includes Signatures)
    PSPDFAnnotationTypeSquare      = 1 << 5,
    PSPDFAnnotationTypeCircle      = 1 << 20,
    PSPDFAnnotationTypeLine        = 1 << 6,
    PSPDFAnnotationTypeNote        = 1 << 7,
    PSPDFAnnotationTypeStamp       = 1 << 8,  /// A stamp can be an image as well.
    PSPDFAnnotationTypeCaret       = 1 << 9,
    PSPDFAnnotationTypeRichMedia   = 1 << 10, /// Embedded PDF video
    PSPDFAnnotationTypeScreen      = 1 << 11, /// Embedded PDF video
    PSPDFAnnotationTypeWidget      = 1 << 12, /// Widget (includes PDF Forms)
    PSPDFAnnotationTypeFile        = 1 << 13, /// FileAttachment
    PSPDFAnnotationTypeSound       = 1 << 14,
    PSPDFAnnotationTypePolygon     = 1 << 15,
    PSPDFAnnotationTypePolyLine    = 1 << 16,
    PSPDFAnnotationTypePopup       = 1 << 21, /// Popup annotations are not yet supported.
    PSPDFANnotationTypeWatermark   = 1 << 22, /// Not supported.
    PSPDFAnnotationTypeTrapNet     = 1 << 23, /// Not supported.
    PSPDFAnnotationType3D          = 1 << 24, /// Not supported.
    PSPDFAnnotationTypeRedact      = 1 << 25, /// Not supported.
    PSPDFAnnotationTypeAll         = NSUIntegerMax
};

/// Converts an annotation type into the string representation and back.
extern NSString *PSPDFStringFromAnnotationType(PSPDFAnnotationType annotationType);
extern PSPDFAnnotationType PSPDFAnnotationTypeFromString(NSString *string);

/// Annotation border style types.
typedef NS_ENUM(NSUInteger, PSPDFAnnotationBorderStyle) {
    PSPDFAnnotationBorderStyleNone,
    PSPDFAnnotationBorderStyleSolid,
    PSPDFAnnotationBorderStyleDashed,
    PSPDFAnnotationBorderStyleBelved,
    PSPDFAnnotationBorderStyleInset,
    PSPDFAnnotationBorderStyleUnderline,
    PSPDFAnnotationBorderStyleUnknown
};
extern NSString *const PSPDFBorderStyleTransformerName; // Global `NSValueTransformer` to convert between enum and string value.

/// A set of flags specifying various characteristics of the annotation.
/// PSPDFKit doesn't support all of those flag settings.
typedef NS_OPTIONS(NSUInteger, PSPDFAnnotationFlags) {
    PSPDFAnnotationFlagInvisible      = 1 << 0, // If set, ignore annotation AP stream if there is no handler available.
    PSPDFAnnotationFlagHidden         = 1 << 1, // If set, do not display or print the annotation or allow it to interact with the user.
    PSPDFAnnotationFlagPrint          = 1 << 2, // [IGNORED] If set, print the annotation when the page is printed. Default value.
    PSPDFAnnotationFlagNoZoom         = 1 << 3, // [IGNORED] If set, don't scale the annotation’s appearance to match the magnification of the page.
    PSPDFAnnotationFlagNoRotate       = 1 << 4, // [IGNORED] If set, don't rotate the annotation’s appearance to match the rotation of the page.
    PSPDFAnnotationFlagNoView         = 1 << 5, // If set, don't display the annotation on the screen. (But printing might be allowed)
    PSPDFAnnotationFlagReadOnly       = 1 << 6, // [IGNORED] If set, don't allow the annotation to interact with the user. Ignored for Widget.
    PSPDFAnnotationFlagLocked         = 1 << 7, // [IGNORED] If set, don't allow the annotation to be deleted or properties modified (except contents)
    PSPDFAnnotationFlagToggleNoView   = 1 << 8, // [IGNORED] If set, invert the interpretation of the NoView flag for certain events.
    PSPDFAnnotationFlagLockedContents = 1 << 9, // [IGNORED] If set, don't allow the contents of the annotation to be modified by the user.
};

/// Trigger events for certain viewer actions. See PDF Reference 1.7, 423ff.
typedef NS_ENUM(UInt8, PSPDFAnnotationTriggerEvent) {
    PSPDFAnnotationTriggerEventCursorEnters,  /// Cursor Enters. (Unsupported) E (0)
    PSPDFAnnotationTriggerEventCursorExits,   /// Cursor Exits. (Unsupported) X  (1)
    PSPDFAnnotationTriggerEventMouseDown,     /// Triggered on `touchesBegan:` D  (2)
    PSPDFAnnotationTriggerEventMouseUp,       /// Triggered on `touchesEnded:` U  (3)
    PSPDFAnnotationTriggerEventReceiveFocus,  /// Triggers when the annotation is tapped. Fo (4)
    PSPDFAnnotationTriggerEventLooseFocus,    /// Triggers when the annotation is tapped. Bl (5)
    PSPDFAnnotationTriggerEventPageOpened,    /// Page opens. (Unsupported) PO (6)
    PSPDFAnnotationTriggerEventPageClosed,    /// Page closes. (Unsupported) PC (7)
    PSPDFAnnotationTriggerEventPageVisible,   /// Page becomes visible. (Unsupported) PV (8)

    // Form extensions
    PSPDFAnnotationTriggerEventFormChanged,   /// Form value changes. K  (9)
    PSPDFAnnotationTriggerEventFieldFormat,   /// Form is formatted. F (10)
    PSPDFAnnotationTriggerEventFormValidate,  /// Form is validated. V (11)
    PSPDFAnnotationTriggerEventFormCalculate, /// Form is calculated. C (12)
};

/// Border effect names. See PDF Reference 1.5, 1.6. (Table 167).
typedef NS_ENUM(NSInteger, PSPDFAnnotationBorderEffect) {
    PSPDFAnnotationBorderEffectNoEffect = 0,
    PSPDFAnnotationBorderEffectCloudy,
};

/**
 `PSPDFAnnotation` is the base class for all PDF annotations and forms.

 Don't directly make an instance of this class, use subclasses like `PSPDFNoteAnnotation` or `PSPDFLinkAnnotation`. This class will return `nil` if initialized directly, unless with the type `PSPDFAnnotationTypeUndefined`.

 `PSPDFAnnotationManager` searches the runtime for subclasses of `PSPDFAnnotation` and builds up a dictionary using `supportedTypes`.

 @note on Thread safety:
 Annotation objects should only ever be edited on the main thread. Modify properties on the main thread only if they are already active (for creation, it doesn't matter which thread creates them). Before rendering, obtain a copy of the annotation to ensure it's not mutated while properties are read.
 Once the `documentProvider` is set, modifying properties on a background thread will throw an exception.

 @warning Annotations are mutable objects. Do not store them into NSSet or other objects that require a hash-value that does not change.
*/
@interface PSPDFAnnotation : PSPDFModel <PSPDFUndoProtocol, PSPDFJSONSerializing>

/// Converts JSON representation back into `PSPDFAnnotation` subclasses.
/// Will return nil for invalid JSON or not recognized types.
/// `document` is optional and if given the override dictionary will be honored (to return your custom `PSPDFAnnotation*` subclasses)
+ (PSPDFAnnotation *)annotationFromJSONDictionary:(NSDictionary *)JSONDictionary document:(PSPDFDocument *)document error:(NSError *__autoreleasing*)error;

/// Use this to create custom user annotations.
- (instancetype)initWithType:(PSPDFAnnotationType)annotationType;

/// Returns YES if PSPDFKit has support to write this annotation type back into the PDF.
+ (BOOL)isWriteable;

/// Returns YES if PSPDFKit has support to delete this annotation type back into the PDF.
+ (BOOL)isDeletable;

/// Returns YES if this annotation type has a fixed size, no matter the internal bounding box.
+ (BOOL)isFixedSize;

/// Returns YES if this annotation requires an implicit popup annotation.
+ (BOOL)requriesPopupAnnotation;

/// Returns YES if the annotation wants a selection border. Defaults to YES.
+ (BOOL)wantsSelectionBorder;

/// Returns YES if this annotation type is moveable.
- (BOOL)isMovable;

/// Returns YES if this annotation type is resizable (all but note annotations usually are).
- (BOOL)isResizable;

/// Returns YES if the annotation should maintain its aspect ratio when resized.
/// Defaults to NO for most annotations, except for the `PSPDFStampAnnotation`.
- (BOOL)shouldMaintainAspectRatio;

/// Returns the minimum size that an annotation can properly display. Defaults to (32.f, 32.f).
- (CGSize)minimumSize;

/// Check if `point` is inside the annotation area, while making sure that the hit area is at least `minDiameter` wide.
/// The default implementation performs hit testing based on the annotation bounding box, but concrete subclasses can (and do)
/// override this behavior in order to perform custom checks (e.g., path-based hit testing).
/// @note The usage of `minDiameter` is annotation specific.
- (BOOL)hitTest:(CGPoint)point minDiameter:(CGFloat)minDiameter;

/// Calculates the exact annotation position in the current page.
- (CGRect)boundingBoxForPageRect:(CGRect)pageRect;

/// The annotation type.
@property (nonatomic, assign, readonly) PSPDFAnnotationType type;

/// Page for current annotation. Page is relative to the `documentProvider`.
/// @warning Only set the page at creation time and don't change it later on. This would break internal caching. If you want to move an annotations to a different page, copy an annotation, add it again and then delete the original.
@property (atomic, assign) NSUInteger page;

/// Page relative to the document.
/// @note Will be calculated each time from `page` and the current `documentProvider` and will change `page` if set.
@property (nonatomic, assign) NSUInteger absolutePage;

/// Corresponding `PSPDFDocumentProvider`.
@property (nonatomic, weak) PSPDFDocumentProvider *documentProvider;

/// Document is inferred from the `documentProvider` (Convenience method)
@property (nonatomic, assign, readonly) PSPDFDocument *document;

/// If this annotation isn't backed by the PDF, it's dirty by default.
/// After the annotation has been written to the file, this will be reset until the annotation has been changed.
@property (nonatomic, assign, getter=isDirty) BOOL dirty;

/// If YES, the annotation will be rendered as a overlay. If NO, it will be statically rendered within the PDF content image.
/// Rendering as overlay is more performant if you frequently change it, but might delay page display a bit.
/// @note `PSPDFAnnotationTypeLink` and `PSPDFAnnotationTypeNote` currently are rendered as overlay.
/// If `overlay` is set to YES, you must also register the corresponding *annotationView class to render (override `PSPDFAnnotationManager's` `defaultAnnotationViewClassForAnnotation:`)
@property (nonatomic, assign, getter=isOverlay) BOOL overlay;

/// Per default, annotations are editable when `isWriteable` returns YES.
/// Override this to lock certain annotations. (menu won't be shown)
@property (nonatomic, assign, getter=isEditable) BOOL editable;

/// Indicator if annotation has been soft-deleted (Annotation may already be deleted locally, but not written back.)
/// @note Doesn't check for the `isDeletable` property. Use `removeAnnotations:` on `PSPDFDocument` to delete annotations.
@property (nonatomic, assign, getter=isDeleted) BOOL deleted;

/// Annotation type string as defined in the PDF.
/// Usually read from the annotDict. Don't change this unless you know what you're doing.
@property (nonatomic, copy) NSString *typeString;

/// Alpha value of the annotation color.
@property (nonatomic, assign) CGFloat alpha;

/// Color associated with the annotation or nil if there is no color.
/// Note: use .alpha for transparency, not the alpha value in color.
@property (nonatomic, strong) UIColor *color;

/// Border Color usually redirects to color, unless overridden to have a real backing ivar.
/// (`PSPDFWidgetAnnotation` has such a backing store)
@property (nonatomic, strong) UIColor *borderColor;

/// Fill color. Only used for certain annotation types. ("IC" key, e.g. Shape Annotations)
/// Fill color might be nil - treat like clearColor in that case.
/// @note Fill color will *share* the alpha value set in the .alpha property, and will ignore any custom alpha value set here.
/// Apple Preview.app will not show you transparency in the `fillColor`.
@property (nonatomic, strong) UIColor *fillColor;

/// Various annotation types may contain text. Optional.
@property (nonatomic, copy) NSString *contents;

/// Subject property (corresponding to "Subj" key).
@property (nonatomic, copy) NSString *subject;

/// Dictionary for additional action types.
@property (nonatomic, copy) NSDictionary *additionalActions;

/// (Optional; inheritable) The field’s value, whose format varies depending on the field type. See the descriptions of individual field types for further information.
@property (nonatomic, copy) id value;

/// Annotation flags.
@property (nonatomic, assign) PSPDFAnnotationFlags flags;

/// Shortcut that checks for `PSPDFAnnotationFlagHidden` in `flags`.
@property (nonatomic, assign, getter=isHidden) BOOL hidden;

/// The annotation name, a text string uniquely identifying it among all the annotations on its page.
/// (Optional; PDF1.4, "NM" key)
@property (nonatomic, copy) NSString *name;

/// User (title) flag. ("T" property)
@property (nonatomic, copy) NSString *user;

/// Annotation group key. Allows to have multiple annotations that behave as single one, if their `group` string is equal. Only works within one page.
/// This is a proprietary extension and saved into the PDF as "PSPDF:GROUP" key.
@property (nonatomic, copy) NSString *group;

/// Date when the annotation was created. Might be nil.
/// PSPDFKit will set this for newly created annotations.
@property (nonatomic, strong) NSDate *creationDate;

/// Date where the annotation was last modified.
/// Saved into the PDF as the "M" property (Optional, since PDF 1.1)
/// Will be updated when a property is changed.
@property (atomic, strong) NSDate *lastModified;

/// Border Line Width (only used in certain annotations)
@property (nonatomic, assign) CGFloat lineWidth;

/// Annotation border style.
@property (nonatomic, assign) PSPDFAnnotationBorderStyle borderStyle;

/// If borderStyle is set to `PSPDFAnnotationBorderStyleDashed`, we expect a `dashStyle` array here (int-values)
@property (nonatomic, copy) NSArray *dashArray;

/// Border effect. See PDF Reference 1.5, 1.6 (Table 167).
@property (nonatomic, assign) PSPDFAnnotationBorderEffect borderEffect;

/// (Optional; valid only if the value of borderEffect is PSPDFAnnotationBorderEffectCloudy)
/// A number describing the intensity of the effect, in the range 0 to 2. Default value: 0.
@property (nonatomic, assign) CGFloat borderEffectIntensity;

/// Rectangle of specific annotation. (PDF coordinates)
/// @note Other properties might be adjusted, depending what `shouldTransformOnBoundingBoxChange` returns.
@property (nonatomic, assign) CGRect boundingBox;

/// Rotation property (should be a multiple of 90, but there are exceptions, e.g. for stamp annotations)
/// Defaults to 0. Allowed values are between 0 and 360.
@property (nonatomic, assign) NSUInteger rotation;

/// Certain annotation types like highlight can have multiple rects.
@property (nonatomic, copy) NSArray *rects;

/// Line, Polyline and Polygon annotations have points.
@property (nonatomic, copy) NSArray *points;

/// If `indexOnPage` is set, it's a native PDF annotation.
/// If this is -1, it's not yet saved in the PDF or saved externally.
@property (atomic, readonly) NSInteger indexOnPage;

/// Allows to save arbitrary data (e.g. a CoreData Object ID)
/// Will be preserved within app sessions and copy, but NOT serialized to disk or within the PDF.
@property (atomic, copy) NSDictionary *userInfo;

/// Returns YES if a custom appearance stream is attached to this annotation.
/// @note An appearance stream is a custom representation for annotations, much like a PDF within a PDF.
@property (nonatomic, assign, readonly) BOOL hasAppearanceStream;

/// Returns `self.contents` or something appropriate per annotation type to describe the object.
- (NSString *)localizedDescription;

/// Return icon for the annotation, if there's one defined.
- (UIImage *)annotationIcon;

/// Compare.
- (BOOL)isEqualToAnnotation:(PSPDFAnnotation *)otherAnnotation;

@end

@interface PSPDFAnnotation (Drawing)

/// Options to use for `drawInContext:withOptions:`
extern NSString *const PSPDFAnnotationDrawFlattenedKey;

/// Set to YES to not render the small note indicator for objects that contain text.
extern NSString *const PSPDFAnnotationIgnoreNoteIndicatorIconKey;

/// Draw current annotation in context. Coordinates here are in PDF coordinate space.
/// Use `PSPDFConvertViewRectToPDFRect:` to convert your coordinates accordingly.
/// (For performance considerations, you want to do this once, not every time `drawInContext:` is called)
/// `options is currently used to allow different annotation drawings during the annotation flattening process.
- (void)drawInContext:(CGContextRef)context withOptions:(NSDictionary *)options;

extern NSString *const PSPDFAnnotationDrawCenteredKey; // CGFloat, draw in the middle of the image, if size has a different aspect ratio.
extern NSString *const PSPDFAnnotationMarginKey;       // `UIEdgeInsets`.

/// Renders annotation into an image.
- (UIImage *)imageWithSize:(CGSize)size withOptions:(NSDictionary *)options;

// Point for the note icon. Override to customize.
- (CGPoint)noteIconPoint;

@end

@interface PSPDFAnnotation (Advanced)

/// Some annotations might change their points/lines/size when the bounding box changes.
/// This returns NO by default.
- (BOOL)shouldUpdatePropertiesOnBoundsChange;
- (BOOL)shouldUpdateOptionalPropertiesOnBoundsChange;

- (void)updatePropertiesWithTransform:(CGAffineTransform)transform isSizeChange:(BOOL)isSizeChange meanScale:(CGFloat)meanScale;
- (void)updateOptionalPropertiesWithTransform:(CGAffineTransform)transform isSizeChange:(BOOL)isSizeChange meanScale:(CGFloat)meanScale;

/// Manually controls if with setting the `boundingBox` it should be transformed as well.
- (void)setBoundingBox:(CGRect)boundingBox transform:(BOOL)transform includeOptional:(BOOL)optionalProperties;

/// Copy annotation object to `UIPasteboard` (multiple formats).
- (void)copyToClipboard;

/// Ask if we may remove an annotation. Only called if `+isDeletable` returns YES.
- (BOOL)shouldDeleteAnnotation;

@end

// Key for vertical text alignment in `fontAttributes`.
extern NSString * const PSPDFVerticalAlignmentName;

/// Vertical alignment setting.
typedef NS_ENUM(NSUInteger, PSPDFVerticalAlignment) {
    PSPDFVerticalAlignmentTop    = 0, /// Align at the top.
    PSPDFVerticalAlignmentCenter = 1, /// Align at the vertical center.
    PSPDFVerticalAlignmentBottom = 2, /// Align at the bottom.
};

// This defines shortcuts that will edit the `fontAttributes` dictionary.
// Valid for PSPDFFreeTextAnnotation, PSPDFChoiceFormElement and PSPDF
@interface PSPDFAnnotation (Fonts)

/// Supports attributes for the text rendering, similar to the attributes in `NSAttributedString`.
/// @note Supported keys are:
/// `NSUnderlineStyleAttributeName` and `NSStrikethroughStyleAttributeName`, valid values `NSUnderlineStyleNone` and `NSUnderlineStyleSingle`.
/// A font can either be underline or strikethrough, not both.
/// `UIFontDescriptorTraitsAttribute` takes a boxed value of `UIFontDescriptorSymbolicTraits`, valid options are `UIFontDescriptorTraitItalic` and `UIFontDescriptorTraitBold`.
/// Setting `NSForegroundColorAttributeName` will also update the `color` property.
/// Setting `NSFontAttributeName` will update `fontName`.
/// Setting `PSPDFFontSizeName` will update `fontSize`.
/// Further attributes might be rendered and saved, but are not persisted in the PDF.
@property (nonatomic, copy) NSDictionary *fontAttributes;

/// The font name, if defined.
/// @note Shortcut for `[self.fontAttributes[NSFontAttributeName] familyName]`.
@property (nonatomic, copy) NSString *fontName;

/// Font size, if defined. Setting this to 0 will use the default size or (for forms) attempt auto-sizing the text.
/// @note Shortcut for `self.fontAttributes[PSPDFFontSizeName]`.
@property (nonatomic, assign) CGFloat fontSize;

/// Text justification. Allows `NSTextAlignmentLeft`, `NSTextAlignmentCenter` and `NSTextAlignmentRight`.
/// @note This is a shortcut for the data saved in `fontAttributes` (`NSParagraphStyleAttributeName`) and will modify `fontAttributes`.
@property (nonatomic, assign) NSTextAlignment textAlignment;

/// Vertical text alignment. Defaults to `PSPDFVerticalAlignmentTop`.
/// @note Shortcut for `self.fontAttributes[PSPDFVerticalAlignmentName]`.
/// @warning This is not defined in the PDF spec. (PSPDFKit extension)
@property (nonatomic, assign) PSPDFVerticalAlignment verticalTextAlignment;

// Return a default font size if not defined in the annotation.
- (CGFloat)defaultFontSize;

// Return a default font name (Helvetica) if not defined in the annotation.
- (NSString *)defaultFontName;

// Returns the currently set font (calculated from defaultFontSize)
- (UIFont *)defaultFont;

@end

extern void PSPDFAnnotationRegisterOverrideClasses(NSKeyedUnarchiver *unarchiver, PSPDFDocument *document);
