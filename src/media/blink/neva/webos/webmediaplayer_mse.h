// Copyright (c) 2018 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef MEDIA_BLINK_NEVA_WEBOS_WEBMEDIAPLAYER_MSE_H_
#define MEDIA_BLINK_NEVA_WEBOS_WEBMEDIAPLAYER_MSE_H_

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include "media/base/media_log.h"
#include "media/base/neva/webos/media_platform_api_webos.h"
#include "media/blink/neva/video_frame_provider_impl.h"
#include "media/blink/webmediaplayer_impl.h"
#include "third_party/blink/public/platform/web_float_point.h"
#include "third_party/blink/public/platform/web_rect.h"
#include "ui/gfx/geometry/rect_f.h"

#if defined(USE_VIDEO_TEXTURE)
#include "ui/gl/neva/video_texture.h"
#endif

namespace media {
class MediaPlatformAPI;
class WebMediaPlayerImpl;

// The canonical implementation of blink::WebMediaPlayer that's backed by
// Pipeline. Handles normal resource loading, Media Source, and
// Encrypted Media.
class MEDIA_BLINK_EXPORT WebMediaPlayerMSE : public WebMediaPlayerImpl {
 public:
  // Constructs a WebMediaPlayer implementation using Chromium's media stack.
  // |delegate| may be null. |renderer_factory_selector| must not be null.
  WebMediaPlayerMSE(
      blink::WebLocalFrame* frame,
      blink::WebMediaPlayerClient* client,
      blink::WebMediaPlayerEncryptedMediaClient* encrypted_client,
      media::WebMediaPlayerDelegate* delegate,
      std::unique_ptr<media::RendererFactorySelector> renderer_factory_selector,
      UrlIndex* url_index,
      std::unique_ptr<VideoFrameCompositor> compositor,
      const StreamTextureFactoryCreateCB& stream_texture_factory_create_cb,
      std::unique_ptr<media::WebMediaPlayerParams> params,
      const blink::WebFloatPoint additional_contents_scale,
      const blink::WebString& app_id);
  ~WebMediaPlayerMSE() override;

  void Load(LoadType load_type,
            const blink::WebMediaPlayerSource& source,
            CORSMode cors_mode) override;

  void Play() override;

  void Pause() override;

  void SetRate(double rate) override;

  void SetVolume(double volume) override;

  void SetContentDecryptionModule(
      blink::WebContentDecryptionModule* cdm,
      blink::WebContentDecryptionModuleResult result) override;

  bool HasVisibility() const override;
  void SetVisibility(bool visible) override;

  // WebMediaPlayerDelegate::Observer interface stubs
  // TODO(neva): Below two methods changed to similar function name.
  //             Need to verify.
  void OnFrameHidden() override {}
  void OnFrameShown() override {}
  void OnFrameClosed() override {}
  void OnIdleTimeout() override {}
  void OnPlay() override {}
  void OnPause() override {}
  void OnVolumeMultiplierUpdate(double multiplier) override {}
  void OnBecamePersistentVideo(bool value) override {}
  void OnSuppressedMediaPlay(bool) override;

  void SetRenderMode(blink::WebMediaPlayer::RenderMode mode) override;

  void UpdateVideoHoleBoundary(bool forced = false);

  void OnDidCommitCompositorFrame() override;

  bool RenderTexture() const {
    return render_mode_ == blink::WebMediaPlayer::RenderModeTexture;
  }
  // Calculate the boundary rectangle of the media player (i.e. location and
  // size of the video frame).
  // Returns true if the geometry has been changed since the last call.
  bool UpdateBoundaryRect();

  scoped_refptr<VideoFrame> GetCurrentFrameFromCompositor() const override;

 private:
  enum StatusOnSuspended {
    UnknownStatus = 0,
    PlayingStatus,
    PausedStatus,
  };

  void Suspend();
  void Resume();

  void OnMetadata(PipelineMetadata metadata) override;

  std::unique_ptr<VideoFrameProviderImpl> video_frame_provider_;
  const blink::WebFloatPoint additional_contents_scale_;
  std::string app_id_;
  bool is_suspended_;
  StatusOnSuspended status_on_suspended_;

  scoped_refptr<media::MediaPlatformAPIWebOS> media_platform_api_;

  base::OneShotTimer throttleUpdateVideoHoleBoundary_;

  gfx::Rect source_rect_in_video_space_;
  gfx::Rect visible_rect_in_screen_space_;
  gfx::Rect last_computed_rect_in_view_space_;
  bool is_video_offscreen_;
  bool is_fullscreen_;

  blink::WebMediaPlayer::RenderMode render_mode_;

  DISALLOW_COPY_AND_ASSIGN(WebMediaPlayerMSE);
};

}  // namespace media

#endif  // MEDIA_BLINK_NEVA_WEBOS_WEBMEDIAPLAYER_MSE_H_
