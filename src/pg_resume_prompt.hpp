#pragma once

namespace PG
{

// Prompt shown (web builds) whenever the locked cursor got released outside of any menu -
// e.g. the browser force-releasing pointer lock on Esc - and a fresh click is needed to
// re-engage it. See pg_update_frame.cpp and BoidsParamsMenu's own AwaitingRelock state.
class ResumePrompt
{
public:
    // draws the prompt centred on screen; must be called in 2D (outside of BeginMode3D/EndMode3D)
    static void Draw();
};

}
