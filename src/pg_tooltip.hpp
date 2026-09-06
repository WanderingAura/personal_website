namespace PG
{

class Tooltip
{
public:
    // draws a tooltip box anchored to the bottom of the window, with each line of text (split
    // on '\n') centred independently. Must be called in 2D (i.e. outside of BeginMode3D/EndMode3D)
    static void DrawAtBottom(const char* text);
};

}
