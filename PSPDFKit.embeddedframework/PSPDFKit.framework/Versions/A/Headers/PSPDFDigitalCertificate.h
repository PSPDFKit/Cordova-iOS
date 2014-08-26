//
//  PSPDFDigitalCertificate.h
//  PSPDFKit
//
//  Copyright (c) 2011-2014 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY INTERNATIONAL COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import "PSPDFKitGlobal.h"

typedef NS_ENUM(NSUInteger, PSPDFDigitalCertificateErrorCode) {
    PSPDFDigitalCertificateErrorNone = noErr,
    // X509 errors are within 0 and 50, see x509_vfy.h (OpenSSL)
    PSPDFDigitalCertificateErrorCannotParseSignature = 1000
};

extern NSString *const PSPDFDigitalCertificateErrorDomain;

/// @note Requires the `PSPDFFeatureMaskDigitalSignatures` feature flag and OpenSSL.
@interface PSPDFDigitalCertificate : NSObject

+ (instancetype)certificateFromData:(NSData *)certificateData;
- (instancetype)initWithData:(NSData *)certificateData error:(NSError **)error;

@property (nonatomic, strong, readonly) NSData *certificateData;

@end
