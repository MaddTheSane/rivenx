//
//  RXMovie.h
//  rivenx
//
//  Created by Jean-Francois Roy on 08/09/2005.
//  Copyright 2005-2012 MacStorm. All rights reserved.
//

#import <libkern/OSAtomic.h>

#import <AVKit/AVKit.h>
#import <CoreMedia/CoreMedia.h>

#import "Rendering/RXRendering.h"

extern NSNotificationName const RXMoviePlaybackDidEndNotification;

@class RXMovie;

@interface RXMovie : NSObject <RXRenderingProtocol> {
  id _owner;

  AVMovie* _movie;
  AVComposition *_vc;

  long _hints;
  CGSize _current_size;
  CMTime _original_duration;

  BOOL _looping;
  BOOL _seamless_looping_hacked;
  BOOL _playing_selection;
  int32_t _goto_end_notification_state;

  GLuint _vao;
  GLfloat _coordinates[16];
  CGRect _render_rect;

  GLuint _texture;
  void* _texture_storage;
  CVImageBufferRef _image_buffer;

  CMTime _current_time;
  OSSpinLock _current_time_lock;

  OSSpinLock _render_lock;
}

- (id)initWithMovie:(void*)movie disposeWhenDone:(BOOL)disposeWhenDone owner:(id)owner;
- (id)initWithURL:(NSURL*)movieURL owner:(id)owner;

- (id)owner;

- (CGSize)currentSize;
- (CMTime)duration;
- (CMTime)videoDuration;

- (BOOL)looping;
- (void)setLooping:(BOOL)flag;

- (float)volume;
- (void)setVolume:(float)volume;

- (BOOL)isPlayingSelection;
- (void)setPlaybackSelection:(CMTimeRange)selection;
- (void)clearPlaybackSelection;

- (void)setExpectedReadAheadFromDisplayLink:(CVDisplayLinkRef)displayLink;

- (void)setWorkingColorSpace:(CGColorSpaceRef)colorspace;
- (void)setOutputColorSpace:(CGColorSpaceRef)colorspace;

- (CGRect)renderRect;
- (void)setRenderRect:(CGRect)rect;

- (void)play;
- (void)stop;
- (float)rate;
- (void)setRate:(float)rate;

- (void)gotoEnd;

- (CMTime)_noLockCurrentTime;

- (void)reset;

@end
