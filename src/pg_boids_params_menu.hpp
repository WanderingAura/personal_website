#pragma once
#include "raylib.h"
#include "pg_boids.hpp"
#include "pg_slider.hpp"
#include "pg_types.hpp"

namespace PG
{

// A 2D overlay menu of sliders bound directly to a Boids instance's movement parameters,
// so dragging a slider immediately affects the live simulation. Opening/closing it toggles
// OS cursor visibility (EnableCursor/DisableCursor) - callers should skip camera look/movement
// updates (e.g. UpdateCamera) while IsOpen() is true, so the cursor is free to use on the menu
// instead of driving the camera.
class BoidsParamsMenu
{
public:
    BoidsParamsMenu() = default;

    // sliders are bound directly to fields of params, which must outlive this menu
    explicit BoidsParamsMenu(BoidsParams& params);

    void Open();

    // begins closing the menu; on desktop this immediately re-locks the cursor, but on web
    // it first enters an "awaiting relock" state (see State) that Update() resolves once the
    // user clicks again
    void Close();

    // true while the menu's sliders are shown OR (web only) while awaiting the resume-click;
    // callers should treat both the same - camera look/movement should stay frozen throughout
    bool IsOpen() const;

    // handles slider dragging and the esc/click-outside-to-close gestures; no-ops when closed
    void Update();

    // draws the menu box and its sliders; no-ops when closed. must be called in 2D
    void Draw() const;

private:
    static const constexpr u32 NUM_SLIDERS = 6;

    // Open: sliders active. AwaitingRelock (web builds only): menu just closed but the OS
    // cursor is still visible, waiting for one more click before re-enabling the locked
    // cursor - browsers only grant pointer-lock re-acquisition from within a genuine click
    // event, so re-locking the instant the menu closes is unreliable (see pg_boids_params_menu.cpp).
    enum class State { Closed, Open, AwaitingRelock };

    State state = State::Closed;
    Rectangle bounds{};
    Slider sliders[NUM_SLIDERS];
};

}
