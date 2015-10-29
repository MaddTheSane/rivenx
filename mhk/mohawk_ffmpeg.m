// Copyright 2014 Jean-Francois Roy. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#import "mhk/mohawk_ffmpeg.h"

#import <Foundation/NSBundle.h>
#import <Foundation/NSPathUtilities.h>
#import <dlfcn.h>

struct mhk_ffmpeg g_mhk_ffmpeg;

static inline void load_function(void* handle, const char* name, void** fnptr) {
  *fnptr = dlsym(handle, name);
  if (!*fnptr) {
    fprintf(stderr, "fatal: unable to bind symbol \"%s\": %s\n", name, dlerror());
    abort();
  }
}

#define LOADFN(HANDLE, FN) load_function(g_mhk_ffmpeg.HANDLE, #FN, (void**)&g_mhk_ffmpeg.FN)

static void load_functions() {
  // lookup symbols
  LOADFN(avu_handle, av_malloc);
  LOADFN(avu_handle, av_freep);
  LOADFN(avu_handle, av_log_set_level);
  LOADFN(avu_handle, av_dict_set);
  LOADFN(avu_handle, av_frame_alloc);
  LOADFN(avu_handle, av_frame_unref);
  LOADFN(avu_handle, av_frame_free);

  LOADFN(avc_handle, avcodec_register_all);
  LOADFN(avc_handle, avcodec_find_decoder);
  LOADFN(avc_handle, avcodec_alloc_context3);
  LOADFN(avc_handle, avcodec_open2);
  LOADFN(avc_handle, avcodec_close);
  LOADFN(avc_handle, av_init_packet);
  LOADFN(avc_handle, av_new_packet);
  LOADFN(avc_handle, av_free_packet);
  LOADFN(avc_handle, avcodec_flush_buffers);
  LOADFN(avc_handle, avcodec_decode_audio4);

  LOADFN(avf_handle, av_register_all);
  LOADFN(avf_handle, av_iformat_next);
  LOADFN(avf_handle, avformat_alloc_context);
  LOADFN(avf_handle, avformat_free_context);
  LOADFN(avf_handle, avformat_open_input);
  LOADFN(avf_handle, avformat_find_stream_info);
  LOADFN(avf_handle, av_read_frame);
  LOADFN(avf_handle, avformat_close_input);

  LOADFN(avf_handle, avio_alloc_context);
  LOADFN(avf_handle, avio_seek);

  // initialize ffmpeg
  g_mhk_ffmpeg.av_log_set_level(AV_LOG_DEBUG);
  g_mhk_ffmpeg.avcodec_register_all();
  g_mhk_ffmpeg.av_register_all();
}

static void* load_lib(NSString* base_path, NSString* name) {
  const char* path = [[base_path stringByAppendingPathComponent:name] fileSystemRepresentation];
  void* handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
  if (!handle) {
    fprintf(stderr, "fatal: %s", dlerror());
    abort();
  }
  return handle;
}

static void load_libs() {
  NSBundle* mhk_bundle = [NSBundle bundleWithIdentifier:@"org.macstorm.MHKKit"];
  NSString* resource_path = [mhk_bundle resourcePath];

  g_mhk_ffmpeg.avu_handle = load_lib(resource_path, @"libavutil.dylib");
  g_mhk_ffmpeg.avc_handle = load_lib(resource_path, @"libavcodec.dylib");
  g_mhk_ffmpeg.avf_handle = load_lib(resource_path, @"libavformat.dylib");

  if (!g_mhk_ffmpeg.avu_handle || !g_mhk_ffmpeg.avc_handle || !g_mhk_ffmpeg.avf_handle) {
    memset(&g_mhk_ffmpeg, 0, sizeof(g_mhk_ffmpeg));
    return;
  }

  load_functions();
}

void mhk_load_ffmpeg() {
  static dispatch_once_t once;
  dispatch_once(&once, ^{ load_libs(); });
}