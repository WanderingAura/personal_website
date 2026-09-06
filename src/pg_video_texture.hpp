#pragma once
#include "raylib.h"

namespace PG
{

// Web only: streams a looping, muted <video> element's decoded frames into an existing raylib
// texture via WebGL's native <video>-to-texture upload (texImage2D accepts a <video> element
// directly), so the browser's built-in H264 decoder does the work - no C++ decoder needed.
// No-op on desktop builds (neither raylib nor this project has a video decoder there); the
// texture just keeps showing whatever static image it was loaded with.
class VideoTexture
{
public:
    VideoTexture() = default;

    // texture's underlying GL texture gets its pixels replaced by live video frames while
    // playing; texture must already be loaded and outlive this object. videoPath is a project
    // asset path (e.g. "assets/foo.mp4"); pass nullptr for no video (this becomes a no-op)
    VideoTexture(const char* videoPath, Texture2D texture);

    // starts playback (paused by default); loops automatically. No-op if there's no video
    void Play();

    // pauses playback, holding on the current frame
    void Pause();

    // uploads the current video frame into the texture; call every frame. No-op unless playing
    void Update();

private:
    unsigned int textureId = 0;
    bool hasVideo = false;
    bool isPlaying = false;
};

}
