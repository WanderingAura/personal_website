namespace PG
{

class Tooltip
{
public:
    // draws a tooltip box with centered text anchored to the bottom of the window.
    // must be called in 2D (i.e. outside of BeginMode3D/EndMode3D)
    static void DrawAtBottom(const char* text);
};

}
