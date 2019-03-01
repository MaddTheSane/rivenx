//
//  RXInterpolator.h
//  rivenx
//
//  Created by Jean-Francois Roy on 12/08/2009.
//  Copyright 2005-2012 MacStorm. All rights reserved.
//

#include <CoreGraphics/CGBase.h>
#import "Base/RXBase.h"

#import "Rendering/Animation/RXAnimation.h"

@protocol RXInterpolator <NSObject>
- (RXAnimation*)animation;
@property (readonly) CGFloat value;
@property (readonly, getter=isDone) BOOL done;
@end

@interface RXAnimationInterpolator : NSObject <RXInterpolator> {
  RXAnimation* _animation;
}

- (id)initWithAnimation:(RXAnimation*)a;

@end

@interface RXLinearInterpolator : RXAnimationInterpolator {
@public
  CGFloat start;
  CGFloat end;
}

- (id)initWithAnimation:(RXAnimation*)a start:(CGFloat)p0 end:(CGFloat)p1;

@end

@interface RXChainingInterpolator : NSObject <RXInterpolator> {
  NSMutableArray* _interpolators;
  id<RXInterpolator> _current;
}

- (void)addInterpolator:(id<RXInterpolator>)interpolator;

@end
