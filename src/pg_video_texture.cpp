#include "pg_video_texture.hpp"
#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten.h>
#endif

namespace PG
{

#if defined(PLATFORM_WEB)

VideoTexture::VideoTexture(const char* videoPath, Texture2D texture) :
    textureId(texture.id),
    hasVideo(videoPath != nullptr)
{
    if (!hasVideo) return;

    int dataSize = 0;
    unsigned char* data = LoadFileData(videoPath, &dataSize);
    if (!data)
    {
        TraceLog(LOG_WARNING, "VideoTexture: failed to load %s", videoPath);
        hasVideo = false;
        return;
    }

    // a <video> element needs a fetchable URL, not a path into Emscripten's in-memory
    // filesystem (where --preload-file puts the .mp4) - so read the bytes ourselves and hand
    // them to JS as a Blob URL instead
    EM_ASM({
        var bytes = HEAPU8.slice($0, $0 + $1);
        var url = URL.createObjectURL(new Blob([bytes], { type: 'video/mp4' }));

        var video = document.createElement('video');
        video.src = url;
        video.loop = true;
        video.muted = true;
        video.playsInline = true;
        video.preload = 'auto';
        // off-screen rather than display:none - some browsers stop decoding frames for a
        // display:none video, but keep decoding one that's merely positioned off-screen
        video.style.position = 'fixed';
        video.style.top = '-9999px';
        document.body.appendChild(video);

        if (!window.PG_videos) window.PG_videos = {};
        window.PG_videos[$2] = video;
    }, data, dataSize, textureId);

    UnloadFileData(data);
}

void VideoTexture::Play()
{
    if (!hasVideo) return;
    isPlaying = true;

    EM_ASM({
        var video = window.PG_videos[$0];
        if (video) video.play().catch(function(e) { console.warn('VideoTexture: play() failed', e); });
    }, textureId);
}

void VideoTexture::Pause()
{
    if (!hasVideo) return;
    isPlaying = false;

    EM_ASM({
        var video = window.PG_videos[$0];
        if (video) video.pause();
    }, textureId);
}

void VideoTexture::Update()
{
    if (!hasVideo || !isPlaying) return;

    // GLctx/GL are Emscripten's internal WebGL context and GL-object registry; GL.textures[id]
    // maps the same integer id raylib's Texture2D.id already holds to the real WebGLTexture
    EM_ASM({
        var video = window.PG_videos[$0];
        if (!video || video.paused || video.readyState < video.HAVE_CURRENT_DATA) return;

        var gl = GLctx;
        gl.bindTexture(gl.TEXTURE_2D, GL.textures[$0]);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, video);
        gl.generateMipmap(gl.TEXTURE_2D);
    }, textureId);
}

#else

VideoTexture::VideoTexture(const char*, Texture2D texture) : textureId(texture.id) {}
void VideoTexture::Play() {}
void VideoTexture::Pause() {}
void VideoTexture::Update() {}

#endif

}
