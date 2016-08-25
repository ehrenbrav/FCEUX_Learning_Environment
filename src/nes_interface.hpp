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
#define NUM_NES_LEGAL_ACTIONS 7

// The amount of change in the position above which we disregard.
#define MAX_ALLOWED_X_CHANGE 100

// Define possible actions
enum Action {
    NOOP        		= 0, // 0
    A           		= 1, // 1
	B					= 2, // 2
    UP          		= 3, // 16
    RIGHT       		= 4, // 128
    LEFT        		= 5, // 64
    DOWN        		= 6, // 32
	A_UP				= 7, // 17
	A_RIGHT				= 8, // 129
	A_LEFT				= 9, // 65
	A_DOWN				= 10, // 33
	B_UP				= 11, // 18
	B_RIGHT				= 12, // 130
	B_LEFT				= 13, // 66
	B_DOWN				= 14, // 34
    RESET       		= 15, // note: we use SYSTEM_RESET instead to reset the environment.
    UNDEFINED   		= 16,
    RANDOM      		= 17,
    SAVE_STATE          = 18,
    LOAD_STATE          = 19,
    SYSTEM_RESET        = 20,
    SELECT              = 21, // 8
    LAST_ACTION_INDEX   = 22
};

// a list of NES actions
typedef std::vector<Action> ActionVect;

// type used to represent instantaneous reward
typedef int reward_t;

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
        reward_t act(Action action);

        /** Returns the number of legal actions. */
        int getNumLegalActions();

        /** Returns the vector of legal actions. */
        ActionVect getLegalActionSet();

        /** Returns a vector describing the minimal set of actions needed to play current game. */
        ActionVect getMinimalActionSet();

        /** Returns the frame number since the loading of the ROM. */
        int getFrameNumber() const;

        /** Set the frame limit for each episode. '0' means no limit. */
        void setMaxNumFrames(int newMax);

        /** Minimum possible instantaneous reward. */
        reward_t minReward() const;

        /** Maximum possible instantaneous reward. */
        reward_t maxReward() const;

        /** The remaining number of lives. */
        int lives() const;

        /** Returns the frame number since the start of the current episode. */
        int getEpisodeFrameNumber() const;

        /** Returns a handle to the current game screen. */
        const uint8_t *getScreen() const;

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
        void restoreSnapshot(const std::string& snapshot);
        
        /** Get the current version of the NES interface.
            Major versions indicate significant changes that might break backward compatibility.
            Minor versions indicate bug-fixes. */
        void getVersion(int &major, int &minor) const;

        /** Converts a pixel to its RGB value. */
        static void getRGB(
            unsigned char pixel, 
            unsigned char &red, 
            unsigned char &green, 
            unsigned char &blue
        );

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
