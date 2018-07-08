/*
 *  MHKErrors.h
 *  MHKKit
 *
 *  Created by Jean-Francois Roy on 06/23/2005.
 *  Copyright 2005-2012 MacStorm. All rights reserved.
 *
 */

#if !defined(MHK_ERRORS_H)
#define MHK_ERRORS_H

#include <sys/cdefs.h>

__BEGIN_DECLS

// MHK errors
enum {
  errFileTooLarge = 1,
  errBadArchive,
  errResourceNotFound,
  errDamagedResource,
  errInvalidChannelCount,
  errInvalidFrameCount,
  errLibavNotAvailable,
  errInvalidSoundDescriptor,
  errInvalidBitmapCompression,
  errInvalidBitmapCompressorInstruction
};

#if defined(__OBJC__)

#import <Foundation/NSString.h>
#import <Foundation/NSError.h>

extern NSErrorDomain const MHKErrorDomain;

typedef NS_ERROR_ENUM(MHKErrorDomain, MHKErrors) {
  MHKErrorsFileTooLarge = 1,
  MHKErrorsBadArchive,
  MHKErrorsResourceNotFound,
  MHKErrorsDamagedResource,
  MHKErrorsInvalidChannelCount,
  MHKErrorsInvalidFrameCount,
  MHKErrorsLibavNotAvailable,
  MHKErrorsInvalidSoundDescriptor,
  MHKErrorsInvalidBitmapCompression,
  MHKErrorsInvalidBitmapCompressorInstruction
};


@interface MHKError : NSError
@end

#endif // __OBJC__

__END_DECLS

#endif // MHK_ERRORS_H
