/*
 * InterThreadMessaging -- InterThreadMessaging.h
 * Created by toby on Tue Jun 19 2001.
 */

#import "Base/RXBase.h"

#import <Foundation/NSThread.h>

@interface NSObject (InterThreadMessaging)
- (void)performSelector:(SEL)selector inThread:(NSThread*)thread;
- (void)performSelector:(SEL)selector inThread:(NSThread*)thread waitUntilDone:(BOOL)wait;
- (void)performSelector:(SEL)selector withObject:(id)object inThread:(NSThread*)thread;
- (void)performSelector:(SEL)selector withObject:(id)object inThread:(NSThread*)thread waitUntilDone:(BOOL)wait;
@end
