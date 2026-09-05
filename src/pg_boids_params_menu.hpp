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

    // closes the menu and immediately re-locks the cursor
    void Close();

    // true while the menu's sliders are shown; callers should skip camera look/movement while true
    bool IsOpen() const;

    // handles slider dragging and the close-button/click-outside-to-close gestures; no-ops when closed
    void Update();

    // draws the menu box and its sliders; no-ops when closed. must be called in 2D
    void Draw() const;

private:
    static const constexpr u32 NUM_SLIDERS = 6;

    bool isOpen = false;
    Rectangle bounds{};
    Rectangle closeButtonBounds{};
    Slider sliders[NUM_SLIDERS];
};

}
