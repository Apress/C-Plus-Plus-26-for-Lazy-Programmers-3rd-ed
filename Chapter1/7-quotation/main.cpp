//Prints a haiku
//      -- from _C++26 for Lazy Programmers_

#include "SSDL.h" 

int main (int argc, char** argv)
{
    // Window setup
    SSDL_SetWindowTitle("Hit any key to end"); 
    // Always tell user what's expected...

    // Load a font and start using it: Neuton, bold, 24 point
    const SSDL_Font NEUTON = SSDL_OpenFont("Neuton-Bold.ttf", 24); 
    SSDL_SetFont (NEUTON);

    SSDL_SetCursor (0, 50);	// Start 50 pixels down

    // And now the poem
    sout << "\"The Sound of Water\"\n";
    sout << "\tby Matsuo Bashou\n\n";		
    // Tab over for author's name, then
    //  double new line at the end of the line

    sout << "Down at the old pond\n";
    sout << "A frog is jumping into it...\n"; 
    sout << "The sound of water.\n\n";

    const SSDL_Font NOTO  = SSDL_OpenFont("NotoSansJP-ExtraBold.ttf", 24);
    
    sout << "Original:\n";
    SSDL_SetFont (NOTO);
    sout << u8"古池や\n";
    sout << u8"蛙飛びこむ\n"; 
    sout << u8"水の音";

    // End when user hits a key
    SSDL_WaitKey();

    return 0;
}
