//
//  BZFSOperation.h
//  rivenx
//
//  Created by Jean-Francois Roy on 08/02/2008.
//  Copyright 2005-2012 MacStorm. All rights reserved.
//

#import "Base/RXBase.h"
#import <CoreServices/CoreServices.h>

@class NSRunLoop;

typedef NS_ENUM(uint32_t, BZFSOperationType) {
  BZFSOperationCopyOperation = 1,
  BZFSOperationMoveOperation
};

@interface BZFSOperation : NSObject {
  FSFileOperationRef _op;
  OptionBits _options;

  BZFSOperationType _type;

  NSString* _source;
  NSString* _destination;

  BOOL _cancelled;

  NSString* _item;
  FSFileOperationStage _stage;
  NSDictionary* _status;
  NSError* _error;
}

- (instancetype)initCopyOperationWithSource:(NSString*)source destination:(NSString*)destination;

@property (atomic) BOOL allowOverwriting;

- (BOOL)scheduleInRunLoop:(NSRunLoop*)aRunLoop forMode:(NSString*)mode error:(NSError**)error;
- (BOOL)start:(NSError**)error;
- (BOOL)cancel:(NSError**)error;

@property (atomic, readonly, copy) NSString *item;
@property (atomic, readonly) FSFileOperationStage stage;
@property (atomic, readonly, copy) NSDictionary *status;
@property (atomic, readonly, retain) NSError *error;
@property (atomic, readonly) BOOL cancelled;

@end
