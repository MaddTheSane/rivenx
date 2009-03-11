/*
 * InterThreadMessaging -- InterThreadMessaging.m
 * Created by toby on Tue Jun 19 2001.
 */

#import <pthread.h>
#import <mach/semaphore.h>
#import <mach/task.h>

#import "InterThreadMessaging.h"

@interface NSObject (LeopardOnly)
- (void)performSelector:(SEL)selector onThread:(NSThread*)thread withObject:(id)object waitUntilDone:(BOOL)wait;
@end

static BOOL useSystemThreadPerformSelector() {
	static BOOL useSystemVersion = NO;
	static BOOL checkedSystem = NO;

	if (!checkedSystem ) {
		useSystemVersion = [NSObject instancesRespondToSelector:@selector(performSelector:onThread:withObject:waitUntilDone:)];
		checkedSystem = YES;
	}

	return useSystemVersion;
}

typedef struct InterThreadMessage {
	SEL selector;
	id receiver;
	id arg;
	semaphore_t sem;
} InterThreadMessage;

static NSMapTable* pThreadMessagePorts = NULL;
static pthread_mutex_t pGate;

@interface InterThreadManager : NSObject
+ (void)threadDied:(NSNotification*)notification;
+ (void)handlePortMessage:(NSPortMessage*)msg;
@end

static void createMessagePortForThread(NSThread* thread, NSRunLoop* runLoop) {
	pthread_mutex_lock(&pGate);

	NSPort* port = NSMapGet(pThreadMessagePorts, thread);
	if (!port) {
		port = [NSPort new];
		[port setDelegate:[InterThreadManager class]];
		[port scheduleInRunLoop:runLoop forMode:NSDefaultRunLoopMode];

		NSMapInsertKnownAbsent(pThreadMessagePorts, thread, port);
		[port release];
	}

	pthread_mutex_unlock(&pGate);
}

static NSPort* messagePortForThread(NSThread* thread) {
	pthread_mutex_lock(&pGate);
	NSPort* port = NSMapGet(pThreadMessagePorts, thread);
	pthread_mutex_unlock(&pGate);

	if (!port) [NSException raise:NSInvalidArgumentException format:@"Thread %@ is not prepared to receive inter-thread messages. You must invoke +prepareForInterThreadMessages first.", thread];

	return port;
}

static void removeMessagePortForThread(NSThread* thread) {
	pthread_mutex_lock(&pGate);

	NSPort* port = (NSPort*)NSMapGet(pThreadMessagePorts, thread);
	if (port) {
		[port invalidate];
		NSMapRemove(pThreadMessagePorts, thread);
	}

	pthread_mutex_unlock(&pGate);
}

@implementation NSThread (InterThreadMessaging)
+ (void)prepareForInterThreadMessages {
	if (useSystemThreadPerformSelector()) return;
	[InterThreadManager class]; // Force the class initialization.
	createMessagePortForThread([NSThread currentThread], [NSRunLoop currentRunLoop]);
}
@end

@implementation InterThreadManager
+ (void)initialize {
	if (useSystemThreadPerformSelector()) return;
	if (!pThreadMessagePorts) {
		pthread_mutex_init(&pGate, NULL);
		pThreadMessagePorts = NSCreateMapTable(NSNonRetainedObjectMapKeyCallBacks, NSObjectMapValueCallBacks, 0);
		[[NSNotificationCenter defaultCenter] addObserver:[self class] selector:@selector(threadDied:) name:NSThreadWillExitNotification object:nil];
	}
}

+ (void)threadDied:(NSNotification*)notification {
	removeMessagePortForThread([notification object]);
}

+ (void)handlePortMessage:(NSPortMessage*)portMessage {
	NSArray* components = [portMessage components];
	NSData* data = [components objectAtIndex:0];
	InterThreadMessage* msg = *((InterThreadMessage**)[data bytes]);

	[msg->receiver performSelector:msg->selector withObject:msg->arg];

	[msg->receiver release];
	[msg->arg release];
	
	if (msg->sem != 0) semaphore_signal(msg->sem);
	free(msg);
}
@end

static void performSelector(SEL selector, id receiver, id object, NSThread* thread, BOOL wait) {	
	if (useSystemThreadPerformSelector()) {
		[receiver performSelector:selector onThread:thread withObject:object waitUntilDone:wait];
		return;
	}
	
	if (!thread || ([thread isEqual:[NSThread currentThread]] && wait)) {
		[receiver performSelector:selector withObject:object];
		return;
	}

	InterThreadMessage* msg = (InterThreadMessage*)malloc(sizeof(struct InterThreadMessage));
	memset(msg, 0, sizeof(struct InterThreadMessage));
	
	semaphore_t sem = 0;
	if (wait) semaphore_create(mach_task_self(), &(msg->sem), SYNC_POLICY_FIFO, 0);

	msg->selector = selector;
	msg->receiver = [receiver retain];
	msg->arg = [object retain];
	msg->sem = sem;

	NSPort* port = messagePortForThread(thread);
	NSData* data = [[NSData alloc] initWithBytes:&msg length:sizeof(void*)];
	NSMutableArray* components = [[NSMutableArray alloc] initWithObjects:&data count:1];
	NSPortMessage* portMessage = [[NSPortMessage alloc] initWithSendPort:port receivePort:nil components:components];

	[portMessage sendBeforeDate:[NSDate distantFuture]];

	[portMessage release];
	[components release];
	[data release];
	
	if (wait) {
		semaphore_wait(sem);
		semaphore_destroy(mach_task_self(), sem);
	}
}

@implementation NSObject (InterThreadMessaging)
- (void)performSelector:(SEL)selector inThread:(NSThread*)thread {
	performSelector(selector, self, nil, thread, NO);
}

- (void)performSelector:(SEL)selector inThread:(NSThread*)thread waitUntilDone:(BOOL)wait {
	performSelector(selector, self, nil, thread, wait);
}

- (void)performSelector:(SEL)selector withObject:(id)object inThread:(NSThread*)thread {
	performSelector(selector, self, object, thread, NO);
}

- (void)performSelector:(SEL)selector withObject:(id)object inThread:(NSThread*)thread waitUntilDone:(BOOL)wait {
	performSelector(selector, self, object, thread, wait);
}
@end