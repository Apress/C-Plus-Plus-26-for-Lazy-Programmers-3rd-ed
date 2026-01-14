//Program to test SSDL
//      -- adapted from the Hallowe'en candy program 
//         in _C++26 for Lazy Programmers_

/* I am not going to test these as all I provide are wrapper functions for SDL functions:

   void SSDL_FadeInMusic 	(SSDL_Music m,  int loops, int ms);
   void SSDL_FadeInMusicPos  	(SSDL_Music m,  int loops, int ms, double pos);
   void SSDL_RewindMusic	();	rewind music; works on some file types
   void SSDL_SetMusicPosition  	(double position);
   void SSDL_FadeOutMusic 	(int ms);	start music fading out over ms milliseconds
   Mix_MusicType SSDL_GetMusicType (const SSDL_Music music);
   Mix_Fading SSDL_FadingMusic	();

   int  SSDL_ExpireSound           	(SSDL_Sound snd, int ms);	cause the sound to halt after ms milliseconds
   int  SSDL_ExpireAllSounds     	(int ms);	cause all sounds to halt after ms milliseconds
   int  SSDL_FadeOutSound    	(SSDL_Sound snd, int ms);	fade out sound over ms milliseconds
   int  SSDL_FadeOutAllSounds 	(int ms);	fade out all sounds over ms milliseconds
   void SSDL_PauseSound       	(SSDL_Sound);	pause sound
   void SSDL_PauseAllSounds 	()	pause all sounds
   void SSDL_ResumeSound 	(SSDL_Sound);	resume sound if paused
   void SSDL_ResumeAllSounds 	();	resume all paused sounds
   void SSDL_HaltSound 	(SSDL_Sound);	halt sound
   void SSDL_HaltAllSounds 	();	halt all sounds
   bool SSDL_SoundPaused 	(SSDL_Sound)	...or paused
   Mix_Fading SSDL_FadingChannel   (SSDL_Sound);
   int  SSDL_VolumeSound      	(SSDL_Sound snd,      int volume=MIX_MAX_VOLUME);
   int  SSDL_VolumeAllSounds  	(int volume=MIX_MAX_VOLUME);	...or do the same for all sounds

*/

/* I'm not going to test these as they wrap simple access functions I wrote for SSDL_Sprite:

   int SSDL_GetSpriteWidth  (const SSDL_Sprite&);return sprite's width as it will appear on screen
   int SSDL_GetSpriteHeight (const SSDL_Sprite&);...and height
   int SSDL_GetSpriteOffsetX(const SSDL_Sprite&);return x part of sprite's offset (see Chapter 11 )
   int SSDL_GetSpriteOffsetY(const SSDL_Sprite&);...and its y component
*/

/* And I'm not going to test these because I'm just too lazy:


   void SSDL_FadeInSound     (SSDL_Sound& sound, int repeats, int ms);
   void SSDL_FadeInSoundTimed(SSDL_Sound& sound, int repeats, int ms, int duration);
   void SSDL_PlaySoundTimed  (SSDL_Sound sound, int repeats, int duration);

*/
#include <cmath> //for sqrt
#include <sstream>
#include <cstring> //for strcmp
#include <ranges>  //for transform
#include <algorithm>//for ranges::to, or std::ranges::copy
#include "SSDL.h" 

//Note to library maintainer: don't take this out -- it's a way the casual user can play
// with the numbers
SSDL_SoundSystemInitializer initializer(88020, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

//dimensions of screen and screen locations
constexpr int SCREEN_WIDTH=675, SCREEN_HEIGHT=522; //dimensions of bkgd

constexpr int CANDY_START_HEIGHT = 15;  //where candy falls from

constexpr int MARGIN             = 25;  //As close to the left/right edges 
                                 // of the screen as moving objects
                                 // are allowed to get

constexpr int BOTTOM_LINE        = 480; //Where last line of text is printed
                                 //on instruction & splash screens

//dimensions of important objects
constexpr int  CANDY_WIDTH  = 60, CANDY_HEIGHT  = 20 ;
constexpr int  BASKET_WIDTH = 70, BASKET_HEIGHT = 90 ;

//how many candies you can catch or miss before winning/losing
constexpr int MAX_CAUGHT         = 10,  MAX_MISSED= 10;
//If you change this, change 
// printInstructions too
// because it specifies this

//fonts for splash screens and catch/miss statistics
constexpr int SMALL_FONT_SIZE  = 12,
      MEDIUM_FONT_SIZE = 24,
      LARGE_FONT_SIZE  = 36;

const SSDL_Font SMALL_FONT 
  = SSDL_OpenFont ("../test/media/Sinister-Fonts_Werewolf-Moon/Werewolf Moon.ttf",
                 SMALL_FONT_SIZE);
const SSDL_Font MEDIUM_FONT
  = SSDL_OpenFont ("../test/media/Sinister-Fonts_Werewolf-Moon/Werewolf Moon.ttf", 
                 MEDIUM_FONT_SIZE);
const SSDL_Font LARGE_FONT 
  = SSDL_OpenFont ("../test/media/Sinister-Fonts_Werewolf-Moon/Werewolf Moon.ttf", 
                 LARGE_FONT_SIZE);

const SSDL_Font GODZILLA 
    = SSDL_OpenFont("../test/media/NotoSansJP-VariableFont_wght.ttf",
      LARGE_FONT_SIZE);

const SSDL_Font NORMAL = SSDL_OpenFont("../fonts/Noto_Sans/static/NotoSans_Condensed-Bold.ttf", 14);

//how far our victory/defeat messages are from left side of screen
constexpr int  FINAL_SCREEN_MESSAGE_OFFSET_X = 40 ;

//background
const SSDL_Image BKGD_IMAGE     
  = SSDL_LoadImage("../test/media/haunted-house.jpg");

//sounds and music
const SSDL_Music BKGD_MUSIC
  = SSDL_LoadMUS("../test/media/159509__mistersherlock__halloween-graveyd-short.mp3");
const SSDL_Sound THUNK_SOUND
  = SSDL_LoadWAV("../test/media/457741__osiruswaltz__wall-bump-1.wav");
const SSDL_Sound DROP_SOUND   
  = SSDL_LoadWAV("../test/media/388284__matypresidente__water-drop-short.wav");

//structs
struct Point2D { int x_=0, y_=0; };

using Vector2D = Point2D; 

struct Object
{
  Point2D     loc_;
  int         rotation_      = 0; 

  Vector2D    velocity_;
  int         rotationSpeed_ = 0;

  SSDL_Sprite sprite_;
};

//major functions called by the main program
bool playGame            ();

//startup/ending screens to communicate with user
void printInstructions   ();
void displayVictoryScreen();
void displayDefeatScreen ();

void fontsTest();

int average(int a, int b, int c) { return (a + b + c) / 3; }
SSDL_Color greyscale(const SSDL_Color& c); // Gets a greyscale color for a given r, g, b -- adapted from Chapter 7

int main (int argc, char** argv)
{
    assert(!SSDL_IsQuit());         //Too hard to test when it succeeds! But it's simple enough I'll let that go.

    //This tests SSDL_ProcessibleImage
    SSDL_ProcessibleImage original   = SSDL_LoadProcessibleImage("../test/media/fuji.bmp");
    SSDL_ProcessibleImage monochrome; monochrome = original;
    //ctor that takes SDL_Texture is called by SSDL_LoadProcessibleImage so it's tested here
    
    //didn't test width, height, size, but I trust them from Chapter 24 code
    auto newPixelsView = monochrome.getPixels() | std::views::transform(greyscale);
    //auto newPixels     = std::ranges::to<std::vector<SSDL_Color>>(newPixelsView); -- not Debian-compatible, so I do the next 2 lines
    std::vector<SSDL_Color> newPixels;
    std::ranges::copy(newPixelsView, std::back_inserter(newPixels));
    monochrome.setPixels(newPixels);

    SSDL_RenderImage(original, 0, 0);     SSDL_RenderText("original", 0, 150);
    SSDL_RenderImage(monochrome, 200, 0); SSDL_RenderText("monochrome", 200, 150);

    monochrome.save("../test/media/monochrome.bmp");
    SSDL_SetCursor(0, 170);
    sout << "Just saved to monochrome.bmp.\n";
    sout << "Hit a key to continue.";
    SSDL_WaitKey();

    //This tests SSDL_IsQuitMessage ()
    const int WHERE_IT_IS = SSDL_GetWindowWidth() / 2;
    while (!SSDL_IsQuitMessage() && SSDL_GetMouseX() < WHERE_IT_IS)
    {
        SSDL_RenderClear();
        SSDL_SetCursor(0, 0);
        sout << "Move mouse to right half of screen to continue.";
    }
    sout << "\nDone. Moving on...\n\n";

    //Now test fonts...
    fontsTest(); sout << "Moving on...\n\n";

    SSDL_Color PUCE = SSDL_CreateColor(228, 145, 166);
    sout << "This is a color: " << PUCE << ". Hit a key to continue.\n";
    SSDL_WaitKey();

    //Test window functions
    SSDL_SetWindowTitle ("Window should be in top left corner of screen");
    assert(!strcmp(SSDL_GetWindowTitle(), "Window should be in top left corner of screen"));
    SSDL_SetWindowPosition(100, 100);
    int x, y; SSDL_GetWindowPosition(x, y); //assert(x == 100); assert(y == 100);
                                            //2021-06-15: Some distros Unix won't set y correctly
                                            //Should either find solution or submit bug report to SDL2 community
    auto [xp, yp] = SSDL_GetWindowPosition(); 
    //assert(x == 100 && y == 100); 2025-05-05: Unix gets this wrong -- moves the window, but not to 100 100.
    //File a bug report with SDL community WSB

    SSDL_SetWindowSize  (SCREEN_WIDTH, SCREEN_HEIGHT);
    assert(SSDL_GetWindowWidth() == SCREEN_WIDTH); assert(SSDL_GetWindowHeight() == SCREEN_HEIGHT);
    auto [xd, yd] = SSDL_GetWindowSize();
    assert(xd == SCREEN_WIDTH && yd == SCREEN_HEIGHT);
    SSDL_SetWindowPosition(5, 5);

    
    //SDL in Unix has been flaking on minimize/maximize, so I'm commenting this out
    // maybe eventually it'll be fixed.
    //Tests the min/restore/max functions, plus SSDL_Dleay
    //sout << "Hit a key; watch the window disappear, reappear, maximize, then come back to this size.\n";
    //SSDL_WaitKey();
    //SSDL_MinimizeWindow(); SSDL_Delay(500);
    //SSDL_RestoreWindow (); SSDL_Delay(500);
    //SSDL_MaximizeWindow(); SSDL_Delay(500); 
    //SSDL_RestoreWindow();
    
  
    ////Test exception
    try 
    {
        SSDL_Image img = SSDL_LoadImage("nonexistentFile.jpg");
        sout << "Should never reach this line!\n";
    }
    catch (SSDL_Exception& e) 
    {
        std::ostringstream os; 
        os << e.what(); 
        //assert(os.str() == std::string("Couldn't open nonexistentFile.jpg")); //Not all distros give same error msg, so no
    }

    //Test image
    assert(BKGD_IMAGE.width() == 675); assert(BKGD_IMAGE.height() == 522); assert(BKGD_IMAGE.size() == 675 * 522);

    //Test GetMouse*: click, x, y; SSDL_ToggleEscapeIsQuit
    SSDL_ToggleEscapeIsQuit();
    SSDL_SetCursor(0,0);
    sout << "Click mouse. Escape won't kill the program on this screen!\n";
    SSDL_WaitMouse();
    switch (SSDL_GetMouseClick())
    {
    case SDL_BUTTON_LEFT: sout << "Left button clicked.\n"; break;
    case SDL_BUTTON_RIGHT:sout << "Right button clicked.\n"; break;
    default: sout << "Unknown button clicked -- not left or right!\n";
    }
    sout << "You clicked at point (" << SSDL_GetMouseX() << ", " << SSDL_GetMouseY() << "). Click again!\n";
    SSDL_WaitMouse();
    auto [xm, ym] = SSDL_GetMouseLocation();
    sout << "This time you clicked at point (" << xm << ", " << ym << ").  Moving on in a moment...\n";
    SSDL_Delay(1000); //wait 1 second

    //Now some music
    SSDL_VolumeMusic (int (MIX_MAX_VOLUME * 0.1));
    assert(!SSDL_PlayingMusic());
    SSDL_PlayMusic   (BKGD_MUSIC);
    assert(SSDL_PlayingMusic());
	
    //Test drawing, SSDL_ToggleEscapeIsQuit, RenderClear using erase color
    SSDL_SetRenderEraseColor(GREEN); SSDL_RenderClear();

    SSDL_SetRenderDrawColor(WHITE);	SSDL_RenderFillCircle(100, 300, 49);
    SSDL_SetRenderDrawColor(RED);   SSDL_RenderDrawCircle(100, 300, 50);
    SSDL_RenderDrawPoint(100, 400);

    SSDL_SetRenderDrawColor(WHITE);	SSDL_RenderFillRect(200, 300, 90, 90);
    SSDL_SetRenderDrawColor(RED);   SSDL_RenderDrawRect(200, 300, 90, 90);
    SSDL_RenderDrawPoint(245, 345);

    SDL_Point points[5] = { {300,300},{390,300},{390,390},{300,390},{300,300} };

    SSDL_RenderDrawLines(points, 5);
    SDL_Rect rects[3] = { {300,300,30,20},{330,335,30,20},{360,370,30,20} };
    SSDL_SetRenderDrawColor(WHITE); SSDL_RenderFillRects(rects, 3);
    SSDL_SetRenderDrawColor(RED); SSDL_RenderDrawRects(rects, 3);
    SSDL_RenderDrawLine(0, 450, SCREEN_WIDTH, 450);

    SSDL_SetRenderDrawColor(WHITE);
    SDL_Rect rect = { 400,300,90,90 };
    SDL_Point measles[] = { {410,310},{450,350},{490,390} };
    SSDL_RenderDrawRect(rect);
    SSDL_RenderDrawPoints(measles, 3);

    //Test erase color, pause/resume music
    std::string str;

    SSDL_SetCursor(0, 0);
    sout << "Window is near upper left of screen.\n";
    sout << "Check out these figures : bordered circles and rects.\n";
    sout << "Default font, GREEN background.\n";
    sout << "Enter text. Backspace to see BLUE\n"; 
    sout << "Hit Esc all you want, until you enter the text.\n";
    sout << "Music will be audible when you go on.\n>";
    sout << "At the upper right, there's a number! Yay!\n";
    SSDL_RenderText(42, 630, 5);

    SSDL_SetRenderDrawColor(RED);
    SSDL_SetFont(GODZILLA);
    SSDL_RenderTextCentered(u8"ゴジラが大きいね！", SCREEN_WIDTH/2, 400); //Here I test printing Unicode, with SSDL_RenderText*
    sout << u8"書いてください。\n";                                       //and sout
    SSDL_SetRenderDrawColor(WHITE);
    SSDL_SetFont(NORMAL);
    TTF_SetFontStyle(SSDL_GetCurrentFont(), TTF_STYLE_BOLD);

    SSDL_SetRenderEraseColor(BLUE);
    assert(SSDL_GetRenderEraseColor() == BLUE);
    SSDL_PauseMusic(); assert(SSDL_PausedMusic());
    ssin >> str;
    SSDL_ResumeMusic(); assert(!SSDL_PausedMusic());

    SSDL_ToggleEscapeIsQuit();

    //initial splash screen
    SSDL_SetRenderEraseColor(BLACK); SSDL_RenderClear();
    printInstructions ();

    //The game itself
    bool isVictory = playGame ();

    //final screen:  victory or defeat
    SSDL_RenderClear (BLACK);
    SSDL_HaltMusic   ();
    
    if (isVictory) displayVictoryScreen ();
    else           displayDefeatScreen  ();

    SSDL_RenderTextCentered("Click mouse to end", 
			    SCREEN_WIDTH/2, BOTTOM_LINE, SMALL_FONT);
    SSDL_WaitMouse();  //Inherently tests SSDL_RenderPresent and SSDL_WaitEvent
    return 0;
}

void fontsTest()
{   
    //constexpr int N = 6;
    //const SSDL_Font MY_FONTS[N] = { 
    //                                SSDL_OpenFont("../fonts/Noto_Sans/static/NotoSans_Condensed-Bold.ttf", 16),
    //                                SSDL_OpenFont("../fonts/Noto_Sans/static/NotoSans_Condensed-Black.ttf", 16),
    //                                SSDL_OpenFont("../fonts/Noto_Sans/static/NotoSans-Bold.ttf", 16),
    //                                SSDL_OpenFont("../fonts/Noto_Sans/static/NotoSans-Bold.ttf", 16),
    //                                SSDL_OpenFont("../fonts/Noto_Sans/static/NotoSans_SemiCondensed-Black.ttf", 16),
    //                                SSDL_OpenFont("../fonts/Noto_Sans/static/NotoSans_SemiCondensed-Bold.ttf", 16),
    //};

    //for (int i = 0; i < N; ++i)
    //{
    //    SSDL_SetFont(MY_FONTS[i]);
    //    TTF_SetFontHinting(MY_FONTS[i], TTF_HINTING_LIGHT);
    //    sout << "Let's test SSDL_OpenSystemFont. It's iffy. 1234567980\n";
    //}

    //SSDL_SetFont(MY_FONTS[0]);
    //TTF_SetFontHinting(MY_FONTS[0], TTF_HINTING_LIGHT);
    //sout << "Let's test SSDL_OpenSystemFont. It's iffy. 1234567980\n";
    //TTF_SetFontHinting(MY_FONTS[0], TTF_HINTING_NONE);
    //sout << "Let's test SSDL_OpenSystemFont. It's iffy. 1234567980\n";
    //TTF_SetFontHinting(MY_FONTS[0], TTF_HINTING_MONO);
    //sout << "Let's test SSDL_OpenSystemFont. It's iffy. 1234567980\n";
    //TTF_SetFontHinting(MY_FONTS[0], TTF_HINTING_NORMAL);
    //sout << "Let's test SSDL_OpenSystemFont. It's iffy. 1234567980\n";

    //SSDL_WaitKey(); SSDL_WaitKey();

    SSDL_Font f;
    bool ok = false;
    try { f = SSDL_OpenSystemFont("Arial.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //Will work on Windows
    try { f = SSDL_OpenSystemFont("Arial.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //Will work on Windows
    //try { f = SSDL_OpenSystemFont("Arial.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //I have no idea what would work WSB
    try { f = SSDL_OpenSystemFont("dejavu/DejaVuSans.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //Works on my basic Debian install
    try { f = SSDL_OpenSystemFont("dejavu-sans-fonts/DejaVuSans.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //Works on my basic Fedora install
    try { f = SSDL_OpenSystemFont("dejavu/DejaVuSans-Bold.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //Works on my basic Manjaro install
    try { f = SSDL_OpenSystemFont("dejavu/DejaVuSans-Bold.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //Works on openSuSe after sudo zypper install dejavu-fonts
    try { f = SSDL_OpenSystemFont("truetype/dejavu/DejaVuSans.ttf", 16); ok=true; } catch (SSDL_Exception&) {} //Works on my basic Ubuntu install

    if (ok) sout << "Looks like SSDL_OpenSystemFont works!\n";
    else sout << "SSDL_OpenSystemFont failed. It may be that you don't have to expected system fonts.\n";
}
//// Startup/ending screens to communicate with user ////

void printInstructions ()
{
  enum { LINE_HEIGHT = 40 };
  SSDL_SetRenderDrawColor (WHITE);
  SSDL_RenderTextCentered ("Catch 10 treats in ", 
			   SCREEN_WIDTH/2,              0, MEDIUM_FONT);
  SSDL_RenderTextCentered("your basket to win",
			  SCREEN_WIDTH/2, LINE_HEIGHT   , MEDIUM_FONT); 
  SSDL_RenderTextCentered ("Miss 10 treats and",
			   SCREEN_WIDTH/2, LINE_HEIGHT*3 , MEDIUM_FONT);
  SSDL_RenderTextCentered("the next treat is YOU",
			  SCREEN_WIDTH/2, LINE_HEIGHT*4 , MEDIUM_FONT);
    
  SSDL_RenderTextCentered ("Use arrow keys to move",
			   SCREEN_WIDTH/2, LINE_HEIGHT*6 , MEDIUM_FONT);
  SSDL_RenderTextCentered("left and right",
			  SCREEN_WIDTH/2, LINE_HEIGHT*7 , MEDIUM_FONT);
    
  SSDL_RenderTextCentered ("Click mouse to",
			   SCREEN_WIDTH/2, LINE_HEIGHT*9 , MEDIUM_FONT);
  SSDL_RenderTextCentered("toggle stats display",
			  SCREEN_WIDTH/2, LINE_HEIGHT*10, MEDIUM_FONT);
    
  SSDL_RenderTextCentered ("Hit any key to continue",
			   SCREEN_WIDTH/2, BOTTOM_LINE,    SMALL_FONT);

  SSDL_WaitKey      (); ///Inherently tests SSDL_RenderPresent and SSDL_WaitEvent
}

void displayVictoryScreen ()
{
  //sound and picture
  static const SSDL_Sound VICTORY_SOUND
    = SSDL_LoadWAV("../test/media/342153__robcro6010__circus-theme-short.wav");
  SSDL_PlaySound(VICTORY_SOUND);
  static const SSDL_Image GOOD_PUMPKIN
    = SSDL_LoadImage("../test/media/goodPumpkin.png");
  SSDL_RenderImage(GOOD_PUMPKIN, SCREEN_WIDTH / 4, 0); 

  //victory message
  SSDL_SetRenderDrawColor(WHITE);
  SSDL_RenderText ("Hooah!"  ,
		   FINAL_SCREEN_MESSAGE_OFFSET_X, SCREEN_HEIGHT/4, 
		   LARGE_FONT); 
  enum { LINE_DISTANCE_Y = 96 }; //an arbitrarily chosen number...
  SSDL_RenderText ("You won!",
		   FINAL_SCREEN_MESSAGE_OFFSET_X,
		   SCREEN_HEIGHT/4+LINE_DISTANCE_Y,
		   LARGE_FONT); 
}

void displayDefeatScreen ()
{
  //sound and picture
  static const SSDL_Sound DEFEAT_SOUND 
    = SSDL_LoadWAV("../test/media/326813__mrose6__echoed-screams-short.wav");
  SSDL_PlaySound(DEFEAT_SOUND);
  static const SSDL_Image SAD_PUMPKIN
    = SSDL_LoadImage("../test/media/sadPumpkin.png");
  SSDL_RenderImage(SAD_PUMPKIN, SCREEN_WIDTH / 4, 0);

  //defeat message
  SSDL_SetRenderDrawColor (WHITE);
  SSDL_RenderText ("Oh, no!", FINAL_SCREEN_MESSAGE_OFFSET_X,
		   SCREEN_HEIGHT/4, LARGE_FONT); 
}

///////////////////// Initializing /////////////////////////

void resetCandyPosition(Object& candy);

void initializeObjects (Object& basket, Object& candy, Object& yumMessage)
{
  //load those images
  SSDL_SetSpriteImage(candy.sprite_,
		      SSDL_LoadImage("../test/media/candy.png"));
  SSDL_SetSpriteImage(basket.sprite_,
		      SSDL_LoadImage("../test/media/jack-o-lantern.png"));
  SSDL_SetSpriteImage(yumMessage.sprite_,
		      SSDL_LoadImage("../test/media/yum.png"));


  //two images are the wrong size; we resize them.
  SSDL_SetSpriteSize (candy.sprite_,   CANDY_WIDTH,  CANDY_HEIGHT);
  SSDL_SetSpriteSize (basket.sprite_, BASKET_WIDTH, BASKET_HEIGHT);
  assert(SSDL_GetSpriteWidth(basket.sprite_) == BASKET_WIDTH);
  assert(SSDL_GetSpriteHeight(basket.sprite_) == BASKET_HEIGHT);

  //basket is 166x214. No, it's now 216x314. Clip out the extra to test clipping
  //clip basket
  SSDL_SetSpriteClipLocation(basket.sprite_, 50, 100);
  SSDL_SetSpriteClipSize(basket.sprite_, 166, 214);
  assert(SSDL_GetSpriteClipX(basket.sprite_) == 50);
  assert(SSDL_GetSpriteClipY(basket.sprite_) ==100);
  assert(SSDL_GetSpriteClipWidth(basket.sprite_) == 166);
  assert(SSDL_GetSpriteClipHeight(basket.sprite_) == 214);

  //flip basket
  SSDL_SpriteFlipVertical(basket.sprite_);
  assert(SSDL_GetSpriteFlipVertical(basket.sprite_));
  //I wont' test SSDL_SpriteFlipHorizontal -- assume it works the same way

  //move 'em so they're centered on the coords we set for them
  SSDL_SetSpriteOffset(candy.sprite_,
		       CANDY_WIDTH/2,  CANDY_HEIGHT/2);
  SSDL_SetSpriteOffset(basket.sprite_,
		       BASKET_WIDTH/2, BASKET_HEIGHT/2);

  //put the objects in their starting positions
  basket.loc_.x_ = SCREEN_WIDTH / 2;
  basket.loc_.y_ = SCREEN_HEIGHT - BASKET_HEIGHT/2;
  SSDL_SetSpriteLocation(basket.sprite_,
			 basket.loc_.x_, basket.loc_.y_);
  candy.loc_.x_ = basket.loc_.x_ + BASKET_WIDTH/2+CANDY_WIDTH/2 - 1; //just inside basket
  candy.loc_.y_ = basket.loc_.y_;
  SSDL_SetSpriteLocation(candy.sprite_, candy.loc_.x_, candy.loc_.y_);
  assert(  SSDL_SpriteHasIntersection(basket.sprite_, candy.sprite_));
  candy.loc_.x_ += 2; //now it's just outside the basket
  SSDL_SetSpriteLocation(candy.sprite_, candy.loc_.x_, candy.loc_.y_);
  assert(! SSDL_SpriteHasIntersection(basket.sprite_, candy.sprite_));
  resetCandyPosition(candy);

  //We don't care about yumMessage position till we make one

  //And set velocities
  //basket's can't be specified till we check inputs
  enum { CANDY_SPEED = 11 };            //11 pixels per frame, straight down
  candy.velocity_.y_ = CANDY_SPEED;     //11 per frame straight down
  //Increase speeds for faster game
  yumMessage.velocity_ = { 1, -1 };     //Up and to the right

  //And rotational speeds
  candy.rotationSpeed_ = 1;             //Candy spins slightly

  //Just to test = and == for SSDL_Sprite:
  SSDL_Sprite s = candy.sprite_; 
  assert(s == candy.sprite_);
}

/////////////////////////// Drawing /////////////////////////////

//Display all 3 objects (2 if yumMessage is currently not visible)
void renderObjects (Object basket, Object candy, Object yumMessage,
                    bool showYumMessage)
{
  SSDL_RenderSprite (basket.sprite_); //also calls SSDL_RenderImageEx
  SSDL_RenderSprite ( candy.sprite_); 
  if (showYumMessage) SSDL_RenderSprite (yumMessage.sprite_);
}

void renderStats(int Caught, int Missed)
{
  //Stats boxes, for reporting how many candies caught and missed
  SSDL_SetRenderDrawColor(BLACK);
  enum { BOX_WIDTH = 90, BOX_HEIGHT = 25 };
  SSDL_RenderFillRect(0, 0,                        //Left box
		      BOX_WIDTH, BOX_HEIGHT);
  SSDL_RenderFillRect(SCREEN_WIDTH - BOX_WIDTH, 0, //Right box
		      SCREEN_WIDTH - 1, BOX_HEIGHT);

  //Statistics themselves
  SSDL_SetRenderDrawColor(WHITE);
  SSDL_SetFont(SMALL_FONT);						//Tests SetFont

  SSDL_SetCursor(0, 0);                           //Tests SetCursor
  sout << "Caught: " << Caught;                   //Tests sout

  SSDL_SetCursor(SCREEN_WIDTH - BOX_WIDTH, 0);     //Right box
  sout << "Missed: " << Missed;
}

//////////////// Moving objects in the world ///////////////////

void resetCandyPosition (Object& candy)  //When it's time to drop 
// another candy...
{
  //Put it at a random X location
  candy.loc_.x_ = MARGIN + rand() % (SCREEN_WIDTH - MARGIN); 
  candy.loc_.y_ = CANDY_START_HEIGHT;    //at the top of the screen

  SSDL_SetSpriteLocation(candy.sprite_, candy.loc_.x_, candy.loc_.y_);
}

void moveObject(Object& object)       
{
  object.loc_.x_ += object.velocity_.x_; //Every frame, move object
  object.loc_.y_ += object.velocity_.y_; //  as specified
  SSDL_SetSpriteLocation(object.sprite_, object.loc_.x_, object.loc_.y_);
  assert(SSDL_GetSpriteX(object.sprite_) == object.loc_.x_);
  assert(SSDL_GetSpriteY(object.sprite_) == object.loc_.y_);

  //...and spin as specified
  object.rotation_ += object.rotationSpeed_; 
  object.rotation_ %= 360;               //angle shouldn't go over 360
  SSDL_SetSpriteRotation(object.sprite_, object.rotation_);
  assert(SSDL_GetSpriteRotation(object.sprite_) == object.rotation_);
}

void moveBasket(Object& basket, int basketSpeed)
{
  //Let user move basket with left and right arrows
  if (SSDL_IsKeyPressed (SDLK_LEFT )) basket.loc_.x_ -= basketSpeed;  
  if (SSDL_IsKeyPressed (SDLK_RIGHT)) basket.loc_.x_ += basketSpeed;  

  //..but don't let the user touch the sides of the screen
  if (basket.loc_.x_ < MARGIN)
    basket.loc_.x_ = MARGIN;
  if (basket.loc_.x_ > SCREEN_WIDTH - MARGIN)
    basket.loc_.x_ = SCREEN_WIDTH - MARGIN;

  //Tell the sprite about our changes on X
  SSDL_SetSpriteLocation(basket.sprite_,
			 basket.loc_.x_, basket.loc_.y_);
}

////////What happens when a candy is caught or missed ////////

//Some math functions we need a lot...
int sqr(int num) { return num * num; }

double distance(Point2D a, Point2D b)
{
  return sqrt(sqr(b.x_ - a.x_) + sqr(b.y_ - a.y_));
}

//Circular collision detection, better for round-ish objects
bool inCollision(Point2D a, Point2D b, int aSize, int bSize)
{
  return (distance(a, b) < aSize/2 + bSize/2);
}

//Detect and handle collisions between basket and candy, 
// and update numberCaught
bool handleCatchingCandy (Object basket, Object& candy, Object& yumMessage,
                          int& numberCaught)
{
  if (inCollision (basket.loc_, candy.loc_, CANDY_WIDTH, BASKET_WIDTH))
    {
      SSDL_PlaySound(THUNK_SOUND); assert(SSDL_SoundPlaying(THUNK_SOUND));

      ++numberCaught;

      resetCandyPosition (candy);

      yumMessage.loc_.x_    = basket.loc_.x_;
      yumMessage.loc_.y_    = basket.loc_.y_;

      return true;
    }
  else return false;
}

//Detect and handle when candy goes off bottom of screen, 
// and update numberMissed
void handleMissingCandy (Object& candy, int& numberMissed)
{
  //you missed it: it went off screen
  if (candy.loc_.y_ >= SCREEN_HEIGHT)  
    {
      SSDL_PlaySound (DROP_SOUND); 

      ++numberMissed;

      resetCandyPosition (candy);
    }
}

///////////////////// Events /////////////////////

//Have not tested SSDL_DefaultEventHandler, but tested something close: the eventhandler in this program
void myEventHandler(bool& mouseClicked)
{
  SSDL_Event event;

  while (SSDL_PollEvent(event)) //test PollEvent
    switch (event.type)
      {
      case SDL_QUIT:            SSDL_DeclareQuit(); break; //test DeclareQuit
      case SDL_KEYDOWN:         if (SSDL_IsKeyPressed(SDLK_ESCAPE)) //test IsKeyPresse
	  SSDL_DeclareQuit(); 
	break;
      case SDL_MOUSEBUTTONDOWN: mouseClicked = true;
      }
}

///// ** The game itself ** ////

bool playGame ()
{
  bool isVictory          = false;      //Did we win?  Not yet
  bool isDefeat           = false;      //Did we lose? Not yet
  bool letsDisplayStats   = true;       //Do we show stats on screen?
  //  Yes, for now

  int numberCaught = 0,                 //So far no candies
    numberMissed = 0;                 //  caught or missed

  //Initialize sprites
  Object basket, candy, yumMessage;
  initializeObjects (basket, candy, yumMessage);

  SSDL_Color DARK_RED = SSDL_CreateColor(128, 0, 0, 64);

  SSDL_SetFramesPerSecond(50); 

  assert(!SSDL_SoundPlaying(THUNK_SOUND)); //of course it's not playing YET...
  //Main game loop
  while (SSDL_IsNextFrame () && ! isVictory && ! isDefeat) 
    {
      enum {FRAMES_FOR_YUM_MESSAGE = 60};
      static int framesLeftTillYumDisappears = 0;

      //Handle input events
      bool mouseClick = false; myEventHandler (mouseClick);
      if (mouseClick) letsDisplayStats = !letsDisplayStats;

      //Display the scene
      SSDL_RenderImage(BKGD_IMAGE, 0, 0); //test RenderImage
      SSDL_SetRenderDrawColor(DARK_RED);
      assert(SSDL_GetRenderDrawColor() == DARK_RED);
      SSDL_Color tempColor(DARK_RED); assert(DARK_RED == tempColor);
      tempColor = GREEN;              assert(GREEN == tempColor);

      SSDL_SetFont(NORMAL); 
      SSDL_RenderTextCentered("This banner is centered and transparent dark red and in Noto Sans font", SCREEN_WIDTH / 2, 20);
      SSDL_SetRenderDrawColor(BLUE);
      SSDL_RenderText("But this is blue, left-justified, and in creepy font", 0, SCREEN_HEIGHT / 2 + 40, SMALL_FONT);

      renderObjects (basket, candy, yumMessage,
		     framesLeftTillYumDisappears>0);
      if (letsDisplayStats) renderStats (numberCaught, numberMissed);

      //Updates:

      //Move objects in the scene
      enum { BASKET_SPEED = 7 };    //7 pixels per frame, left or right
      moveBasket(basket, BASKET_SPEED);
      moveObject(candy); moveObject(yumMessage);

      //Did you catch a candy?
      if (handleCatchingCandy(basket, candy, yumMessage, numberCaught))
	framesLeftTillYumDisappears = FRAMES_FOR_YUM_MESSAGE;

      if (numberCaught >= MAX_CAUGHT) 
	isVictory = true; 
      else                                  //...or did it go off screen?
        {
	  handleMissingCandy (candy, numberMissed);
	  if (numberMissed >= MAX_MISSED) 
	    isDefeat = true; //You just lost!
        }

      //Update yum message
      if (framesLeftTillYumDisappears > 0)  //if yumMessage is active
	--framesLeftTillYumDisappears;    //  keep counting down
    }

  return isVictory;
}

SSDL_Color greyscale(const SSDL_Color& c) // Gets a greyscale color for a given r, g, b
{
    int rgbAverage = average(c.r, c.g, c.b);

    SSDL_Color result
        = SSDL_CreateColor(rgbAverage, rgbAverage, rgbAverage);

    return result;
}