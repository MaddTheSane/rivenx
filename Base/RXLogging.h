/*
 *  RXLogging.h
 *  rivenx
 *
 *  Created by Jean-Francois Roy on 26/02/2008.
 *  Copyright 2005-2012 MacStorm. All rights reserved.
 *
 */

#if !defined(RXLOGGING_H)
#define RXLOGGING_H

#include <sys/cdefs.h>
#include <CoreFoundation/CoreFoundation.h>

__BEGIN_DECLS

/* facilities */
typedef const char* RXLoggingFacility CF_TYPED_ENUM;
extern RXLoggingFacility kRXLoggingBase;
extern RXLoggingFacility kRXLoggingEngine;
extern RXLoggingFacility kRXLoggingRendering;
extern RXLoggingFacility kRXLoggingScript;
extern RXLoggingFacility kRXLoggingGraphics;
extern RXLoggingFacility kRXLoggingAudio;
extern RXLoggingFacility kRXLoggingEvents;
extern RXLoggingFacility kRXLoggingAnimation;

/* levels */
extern const int kRXLoggingLevelDebug;
extern const int kRXLoggingLevelMessage;
extern const int kRXLoggingLevelError;
extern const int kRXLoggingLevelCritical;

extern void RXCFLog(RXLoggingFacility facility, int level, CFStringRef format, ...) CF_FORMAT_FUNCTION(3, 4);

#if defined(__OBJC__)

#import <Foundation/NSString.h>

extern void RXLog(RXLoggingFacility facility, int level, NSString* format, ...) NS_FORMAT_FUNCTION(3, 4);
extern void RXLogv(RXLoggingFacility facility, int level, NSString* format, va_list args) NS_FORMAT_FUNCTION(3, 0);

extern void _RXOLog(id object, RXLoggingFacility facility, int level, NSString* format, ...) NS_FORMAT_FUNCTION(4, 5);

#define RXOLog(format, ...) _RXOLog(self, kRXLoggingBase, kRXLoggingLevelMessage, format, ##__VA_ARGS__)
#define RXOLog2(facility, level, format, ...) _RXOLog(self, facility, level, format, ##__VA_ARGS__)

#endif // __OBJC__

__END_DECLS

#endif // RXLOGGING_H
