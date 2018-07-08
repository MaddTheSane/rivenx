//
//  NSArray+RXArrayAdditions.h
//  rivenx
//
//  Created by Jean-François Roy on 26/12/2011.
//  Copyright (c) 2012 MacStorm. All rights reserved.
//

#import <Foundation/NSArray.h>

@interface NSArray<ObjectType> (RXArrayAdditions)

- (nullable ObjectType)objectAtIndexIfAny:(NSUInteger)index;

@end
