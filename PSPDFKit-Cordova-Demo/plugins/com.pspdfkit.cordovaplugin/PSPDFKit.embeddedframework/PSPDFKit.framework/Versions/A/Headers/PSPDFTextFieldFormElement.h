//
//  PSPDFTextFieldFormElement.h
//  PSPDFKit
//
//  Copyright (c) 2013-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import "PSPDFFormElement.h"

/// The text field flags. Most flags aren't currently supported.
/// Query `fieldFlags` from the `PSPDFFormElement` base class.
typedef NS_OPTIONS(NSUInteger, PSPDFTextFieldFlag) {
    PSPDFTextFieldFlagMultiline       = 1 << (13-1),
    PSPDFTextFieldFlagPassword        = 1 << (14-1),
    PSPDFTextFieldFlagFileSelect      = 1 << (21-1),
    PSPDFTextFieldFlagDoNotSpellCheck = 1 << (23-1),
    PSPDFTextFieldFlagDoNotScroll     = 1 << (24-1),
    PSPDFTextFieldFlagComb            = 1 << (25-1),
    PSPDFTextFieldFlagRichText        = 1 << (26-1)
};

typedef NS_ENUM(NSUInteger, PSPDFTextInputFormat) {
    PSPDFTextInputFormatNormal,
    PSPDFTextInputFormatNumber,
    PSPDFTextInputFormatDate,
    PSPDFTextInputFormatTime
};

/// Text field form element.
@interface PSPDFTextFieldFormElement : PSPDFFormElement

/// If set, the field may contain multiple lines of text; if clear, the field’s text shall be restricted to a single line.
/// @note Evaluates `PSPDFTextFieldFlagMultiline` in the `fieldFlags`.
- (BOOL)isMultiline;

/// If set, the field is intended for entering a secure password that should not be echoed visibly to the screen.
/// @note Evaluates `PSPDFTextFieldFlagPassword` in the `fieldFlags`.
- (BOOL)isPassword;

/// Handles Keystroke, Validate and Calculate actions that follow from user text input automatically.
/// `isFinal` defines if the user is typing (NO) or if the string should be committed (YES).
/// The change is the change in text.
/// Returns the new text contents (possibly different from the passed change) to be applied. Otherwise, if failed, returns nil.
- (NSString *)textFieldChangedWithContents:(NSString *)contents change:(NSString *)change range:(NSRange)range isFinal:(BOOL)isFinal error:(NSError * __autoreleasing *)validationError;

/// Returns the contents formatted based on rules in the annotation (including JavaScript)
- (NSString *)formattedContents;

/// The input format. Some forms are number/date/time specific.
@property (nonatomic, assign, readonly) PSPDFTextInputFormat inputFormat;

@end
