#ifndef __NES_INTERFACE_HPP__
#define __NES_INTERFACE_HPP__

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <SDL/SDL.h>

namespace nes {

// NES screen width.
#define NES_SCREEN_WIDTH 256

// Number of normal game actions we want to test.
#define NUM_NES_LEGAL_ACTIONS 15

// The amount of change in the position above which we disregard.
#define MAX_ALLOWED_X_CHANGE 100

// Define possible actions
#define ACT_NOOP      0  // 0
#define ACT_A         1  // 1
#define ACT_B	      2  // 2
#define ACT_UP        3  // 16
#define ACT_RIGHT     4  // 128
#define ACT_LEFT      5  // 64
#define ACT_DOWN      6  // 32
#define ACT_A_UP      7  // 17
#define ACT_A_RIGHT   8  // 129
#define ACT_A_LEFT    9  // 65
#define ACT_A_DOWN    10 // 33
#define ACT_B_UP      11 // 18
#define ACT_B_RIGHT   12 // 130
#define ACT_B_LEFT    13 // 66
#define ACT_B_DOWN    14 // 34
#define ACT_RESET     15 // note: we use SYSTEM_RESET instead to reset the environment.
#define ACT_UNDEFINED 16
#define ACT_RANDOM    17
#define ACT_SELECT    18 // 8

typedef unsigned char byte_t;
typedef unsigned char pixel_t;

// This class provides a simplified interface to NES.
class NESInterface {

    public:

        /** create a NESInterface. This routine is not threadsafe!
            One also has the option of creating a single NES session
            that will randomly (uniform) alternate between a number of
            different ROM files. The syntax is:  
                <rom path>+<rom path>+... */
        NESInterface(const std::string &rom_file);
        
        /** Unload the emulator. */
        ~NESInterface();

        /** Resets the game. */
        void resetGame();

        /** Indicates if the game has ended. */
        bool gameOver();

        /** Applies an action to the game and returns the reward. It is the user's responsibility
            to check if the game has ended and reset when necessary - this method will keep pressing
            buttons on the game over screen. */
        int act(int action);

        /** Returns the number of legal actions. */
        int getNumLegalActions();

        /** Returns the vector of legal actions. */
        void getLegalActionSet(int legal_actions[]);

        /** Returns the frame number since the loading of the ROM. */
        int getFrameNumber() const;

        /** Set the frame limit for each episode. '0' means no limit. */
        void setMaxNumFrames(int newMax);

        /** Minimum possible instantaneous reward. */
        int minReward() const;

        /** Maximum possible instantaneous reward. */
        int maxReward() const;

        /** The remaining number of lives. */
        int lives() const;

        /** Returns the frame number since the start of the current episode. */
        int getEpisodeFrameNumber() const;

        /** Returns a handle to the current game screen. */
        void getScreen(unsigned char *screen, int screen_size);

        /** Returns width and height. */
        const int getScreenHeight() const;
        const int getScreenWidth() const;

        /** Returns the score. */
        const int getCurrentScore() const;

        /** Saves the state of the emulator system, overwriting any 
            previously saved state. */
        void saveState();

        /** Restores a previously saved state of the emulator system,
            returns false if no such state exists (and makes no changes
            to the emulator system). */
        bool loadState();

        /** Gets a state as a string. */
        std::string getSnapshot() const;

        /** Sets the state from a string*/
        void restoreSnapshot(const std::string snapshot);
        
        /** Converts a pixel to its RGB value. */
        static void getRGB(
            unsigned char pixel, 
            unsigned char *red, 
            unsigned char *green, 
            unsigned char *blue
        );

        /** Get the full RGB screen from the raw pixel data. */
        void fillRGBfromPalette(unsigned char *raw_screen, unsigned char *rgb_screen, int raw_screen_size);

    private:

        /** Copying is explicitly disallowed. */
        NESInterface(const NESInterface &);

        /** Assignment is explicitly disallowed. */
        NESInterface &operator=(const NESInterface &);

        class Impl;
        Impl *m_pimpl;
};

} // namespace nes

#endif // __NES_INTERFACE_HPP__





