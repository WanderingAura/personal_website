namespace PG
{

void UpdateDrawFrame(void);

// true once the user has asked to quit via keyboard (Esc while the boids params menu is
// closed - Esc while it's open closes the menu instead, see SetExitKey(KEY_NULL) in pg_main.cpp).
// Callers should also keep checking WindowShouldClose() for the OS close button/Alt+F4 etc,
// which this does not cover.
bool ShouldQuit(void);

}