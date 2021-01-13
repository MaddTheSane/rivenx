//
//  RXMovie.h
//  rivenx
//
//  Created by Jean-Francois Roy on 08/09/2005.
//  Copyright 2005-2012 MacStorm. All rights reserved.
//

#import <os/lock.h>

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
  os_unfair_lock _current_time_lock;

  os_unfair_lock _render_lock;
}

- (instancetype)initWithMovie:(void*)movie disposeWhenDone:(BOOL)disposeWhenDone owner:(id)owner;
- (instancetype)initWithURL:(NSURL*)movieURL owner:(id)owner;

- (id)owner;

@property (readonly) CGSize currentSize;
@property (readonly, nonatomic) CMTime duration;
@property (readonly, nonatomic) CMTime videoDuration;

@property BOOL looping;

@property float volume;

@property (readonly, getter=isPlayingSelection) BOOL playingSelection;
- (void)setPlaybackSelection:(CMTimeRange)selection;
- (void)clearPlaybackSelection;

- (void)setExpectedReadAheadFromDisplayLink:(CVDisplayLinkRef)displayLink;

- (void)setWorkingColorSpace:(CGColorSpaceRef)colorspace;
- (void)setOutputColorSpace:(CGColorSpaceRef)colorspace;

@property CGRect renderRect;

- (void)play;
- (void)stop;
@property float rate;

- (void)gotoEnd;

- (CMTime)_noLockCurrentTime;

- (void)reset;

@end
