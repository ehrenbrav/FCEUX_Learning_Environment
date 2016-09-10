#include "nes_interface.hpp"
#include "drivers/sdl/config.h"
#include "drivers/sdl/sdl.h"
#include "driver.h"
#include "fceu.h"
#include "cheat.h"
#include "video.h"
#include <stdio.h>
#include <SDL/SDL.h>

// Global configuration info.
extern Config *g_config;
extern int noGui;
extern uint8_t *XBuf;

namespace nes {

// Return the RGB value of a given pixel we got from XBuf.
void NESInterface::getRGB(
    unsigned char pixel,
    unsigned char *red,
    unsigned char *green,
    unsigned char *blue) {
	FCEUD_GetPalette(pixel, red, green, blue);
}

class NESInterface::Impl {

    public:

        // create an NESInterface. This routine is not threadsafe!
        Impl(const std::string &rom_file);
        ~Impl();

        // Resets the game
        void reset_game();

        // Indicates if the game has ended
        bool game_over();

        // Applies an action to the game and returns the reward. It is the user's responsibility
        // to check if the game has ended and reset when necessary - this method will keep pressing
        // buttons on the game over screen.
        int act(int action);

        // Returns the number of legal actions.
        int getNumLegalActions();

        // Returns the vector of legal actions.
        void getLegalActionSet(int legal_actions[]);

        // Minimum possible instantaneous reward.
        int minReward() const;

        // Maximum possible instantaneous reward.
        int maxReward() const;

        // The remaining number of lives.
        int lives() const;

        // Returns the frame number since the loading of the ROM
        int getFrameNumber() const;

        // Returns the frame number since the start of the current episode
        int getEpisodeFrameNumber() const;

        // Sets the episodic frame limit
        void setMaxNumFrames(int newMax);

        // Returns the current game screen
        void getScreen(unsigned char *screen, int screen_size);

        // Return screen height.
        const int getScreenHeight() const;

        // Return screen width.
        const int getScreenWidth() const;

        // Returns the current score.
        const int getCurrentScore() const;

        // Saves the state of the system
        void saveState();

        // Loads the state of the system
        bool loadState();

        // Gets current state as string
        std::string getSnapshot() const;

        // restores state from a string
        void restoreSnapshot(const std::string snapshot);

        // Get the RGB data from the raw screen.
        void fillRGBfromPalette(unsigned char *raw_screen, unsigned char *rgb_screen, int raw_screen_size);

    private:

        int m_episode_score; // Score accumulated throughout the course of an episode
        bool m_display_active;    // Should the screen be displayed or not
        int m_max_num_frames;     // Maximum number of frames for each episode
        int nes_input; // Input to the emulator.
        int current_game_score;
        int current_x;
        int remaining_lives;
        int game_state;
        int episode_frame_number;
};


NESInterface::Impl::~Impl() {

	CloseGame();
	FCEUI_Kill();
	SDL_Quit();
}

bool NESInterface::Impl::loadState() {

	// TODO implement
	printf("loadState is not implemented...");
	//FCEUI_LoadState();
	return false;
}

bool NESInterface::Impl::game_over() {

	// Update game state.
	game_state = FCEU_CheatGetByte(0x0770);

	// Return true only if this byte is 1.
	if (game_state == 1) return false;

	// Reset the score and position.
	current_game_score = 0;
	current_x = 0;
	return true;
}

void NESInterface::Impl::reset_game() {

	// Pretty simple...
	ResetNES();

	// Initialize the score,position, and frame counter.
	current_game_score = 0;
	current_x = 0;
	episode_frame_number = 0;

	// Run a few frames first to get to the startup screen.
	for (int i = 0; i<60; i++) {
		NESInterface::Impl::act(ACT_NOOP);
	}

	// Hit the start button...
	for (int i = 0; i<10; i++) {
		NESInterface::Impl::act(ACT_SELECT);
	}
}

void NESInterface::Impl::saveState() {

	// TODO implement
	printf("saveState NOT IMPLEMENTED.\n");
	//		FCEUD_SaveStateAs ();
}

std::string NESInterface::Impl::getSnapshot() const {

	// TODO implement
	printf("getSnapshot NOT IMPLEMENTED.\n");
	/*
    const ALEState* state = m_emu->environment->cloneState();
    std::string snapshot = state->getStateAsString();
    m_emu->environment->destroyState(state);
    return snapshot;
    */
	return "";
}

void NESInterface::Impl::restoreSnapshot(const std::string snapshot) {

	// TODO implement
	printf("restoreSnapshot NOT IMPLEMENTED.\n");
    //ALEState state(snapshot);
    //m_emu->environment->restoreState(state);
}

void NESInterface::Impl::getScreen(unsigned char *screen, int screen_size) {
        memcpy(screen, XBuf, screen_size);
}

const int NESInterface::Impl::getScreenHeight() const {
	return FSettings.LastSLine - FSettings.FirstSLine+1;
}

const int NESInterface::Impl::getScreenWidth() const {
	return NES_SCREEN_WIDTH;
}

void NESInterface::Impl::fillRGBfromPalette(unsigned char *raw_screen, unsigned char *rgb_screen, int raw_screen_size) {

        unsigned long i;
        for (i = 0; i<raw_screen_size; i++) {
                unsigned char r, g, b;
                NESInterface::getRGB(raw_screen[i], &r, &g, &b);

                // Man, this bastard took a long time to figure out!
                rgb_screen[3*i] = b;
                rgb_screen[(3*i)+1] = g;
                rgb_screen[(3*i)+2] = r;
        }
}

void NESInterface::Impl::setMaxNumFrames(int newMax) {
    m_max_num_frames = newMax;
}

int NESInterface::Impl::getEpisodeFrameNumber() const {
	return episode_frame_number;
}

int NESInterface::Impl::getFrameNumber() const {

	// TODO implement
	printf("getFrameNumber NOT IMPLEMENTED.\n");
    //return m_emu->environment->getFrameNumber();
	return 0;
}

int NESInterface::Impl::getNumLegalActions() {
	return NUM_NES_LEGAL_ACTIONS;
}

void NESInterface::Impl::getLegalActionSet(int legal_actions[]) {
    
	// All the things you can do in SMB...
	legal_actions[0] = ACT_NOOP;
	legal_actions[1] = ACT_A;
	legal_actions[2] = ACT_B;
	legal_actions[3] = ACT_UP;
	legal_actions[4] = ACT_RIGHT;
	legal_actions[5] = ACT_LEFT;
	legal_actions[6] = ACT_DOWN;
	legal_actions[7] = ACT_A_UP;
	legal_actions[8] = ACT_A_RIGHT;
	legal_actions[9] = ACT_A_LEFT;
	legal_actions[10] = ACT_A_DOWN;
	legal_actions[11] = ACT_B_UP;
	legal_actions[12] = ACT_B_RIGHT;
	legal_actions[13] = ACT_B_LEFT;
	legal_actions[14] = ACT_B_DOWN;
}

int NESInterface::Impl::minReward() const {

	// TODO implement.
	printf("minReward NOT IMPLEMENTED.\n");
    //return m_rom_settings->minReward();
	return 0;
}

int NESInterface::Impl::maxReward() const {

	// TODO implement.
	printf("maxReward NOT IMPLEMENTED.\n");
    //return m_rom_settings->maxReward();
	return 0;
}

int NESInterface::Impl::lives() const {
	return remaining_lives;
}

const int NESInterface::Impl::getCurrentScore() const {
	return current_game_score;
}

int NESInterface::Impl::act(int action) {

	// Calculate lives.
	remaining_lives = FCEU_CheatGetByte(0x075a);

	// Update game state.
	game_state = FCEU_CheatGetByte(0x0770);

	// Set the action. No idea whether this will work with other input configurations!
	switch (action) {

		case ACT_NOOP:
			nes_input = 0;
			break;

		case ACT_A:
			nes_input = 1;
			break;

		case ACT_B:
			nes_input = 2;
			break;

		case ACT_UP:
			nes_input = 16;
			break;

		case ACT_RIGHT:
			nes_input = 128;
			break;

		case ACT_LEFT:
			nes_input = 64;
			break;

		case ACT_DOWN:
			nes_input = 32;
			break;

		case ACT_A_UP:
			nes_input = 17;
			break;

		case ACT_A_RIGHT:
			nes_input = 129;
			break;

		case ACT_A_LEFT:
			nes_input = 65;
			break;

		case ACT_A_DOWN:
			nes_input = 33;
			break;

		case ACT_B_UP:
			nes_input = 18;
			break;

		case ACT_B_RIGHT:
			nes_input = 130;
			break;

		case ACT_B_LEFT:
			nes_input = 66;
			break;

		case ACT_B_DOWN:
			nes_input = 34;
			break;

		case ACT_SELECT:
			nes_input = 8;
			break;

		default:
			nes_input = 0;
			printf("ERROR: Undefined action sent to nes_act!");
			break;
	}

	uint8 *gfx;
	int32 *sound;
	int32 ssize;
	static int fskipc = 0;

	// Main loop.
	episode_frame_number++;
	FCEUI_Emulate(&gfx, &sound, &ssize, fskipc);
	FCEUD_Update(gfx, sound, ssize);

	// Get score...
	int new_score = (FCEU_CheatGetByte(0x07dd) * 1000000) +
			(FCEU_CheatGetByte(0x07de) * 100000) +
			(FCEU_CheatGetByte(0x07df) * 10000) +
			(FCEU_CheatGetByte(0x07e0) * 1000) +
			(FCEU_CheatGetByte(0x07e1) * 100) +
			(FCEU_CheatGetByte(0x07e2) * 10);

	// Calculate the change in x (this is the x position on the screen, not in the level).
	int new_x = FCEU_CheatGetByte(0x0086);
	int deltaX = new_x - current_x;
	deltaX = deltaX * 5;

	// Handle resets of level, etc.
	if (abs(deltaX) > MAX_ALLOWED_X_CHANGE) {
		deltaX = 0;
		current_x = 0;
	} 
        current_x = new_x;

	// Calculate the reward based on score.
	int reward = new_score - current_game_score;

	// Handle negative scores.
	if (reward < 0) {
			reward = 0;
	}
	current_game_score = new_score;

	// Add reward based on position.
        // Oh wow - now sure we want to do this :(
	reward = reward + deltaX;

	return reward;
}

NESInterface::Impl::Impl(const std::string &rom_file) :
    m_episode_score(0),
    m_display_active(false),
	nes_input(0),
	current_game_score(0),
	current_x(0),
	remaining_lives(0),
	game_state(0),
	episode_frame_number(0)
{

	// Initialize some configuration variables.
	static int inited = 0;
	noGui = 1;

	if(SDL_Init(SDL_INIT_VIDEO)) {
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
	}
	SDL_GL_LoadLibrary(0);

	// Initialize the configuration system
	g_config = InitConfig();

	if(!g_config) {
		SDL_Quit();
		printf("Could not initialize configuration.\n");
	}

	int error = FCEUI_Initialize();
	if(error != 1) {
		SDL_Quit();
		printf("Error initializing FCEUI.\n");
	}

	// Specify some arguments - may want to do this dynamically at some point...
	int argc = 2;
	char** argv = new char*[argc];
        for (int i=0; i < argc; i++) {
                argv[i] = new char[200+rom_file.length()];
        }
        strcpy(argv[0], "./fceux");
        strcpy(argv[1], rom_file.c_str());

    // Parse the args.
	int romIndex = g_config->parse(argc, argv);
	if(romIndex < 1) {
		printf("ERROR: romIndex less than 1.\n");
	}

	// Now set some configuration options.
	int ret = g_config->setOption("SDL.NoConfig", 1);
	if (ret != 0) {
		printf("Error setting --no-config option.");
	}

	// Update input devices.
	UpdateInput(g_config);

    // Update the emu core with the config options.
	UpdateEMUCore(g_config);

	// load the specified game.
	error = LoadGame(argv[romIndex]);

	// If something went wrong, kill the driver stuff.
	if(error != 1) {
		if(inited&2)
			KillJoysticks();
		if(inited&4)
			KillVideo();
		if(inited&1)
			KillSound();
		inited=0;
		printf("Error loading ROM.\n");
		SDL_Quit();
	}

	// Set the emulation speed.
	FCEUD_SetEmulationSpeed(EMUSPEED_FASTEST);

	// Set the emulator to read from nes_input instead of the Gamepad :)
	FCEUI_SetInput(0, (ESI) SI_GAMEPAD, &nes_input, 0);
	FCEUI_SetInput(1, (ESI) SI_GAMEPAD, &nes_input, 0);
}

/* --------------------------------------------------------------------------------------------------*/

/* begin PIMPL wrapper */

bool NESInterface::loadState() {
    return m_pimpl->loadState();
}

bool NESInterface::gameOver() {
    return m_pimpl->game_over();
}

void NESInterface::resetGame() {
    m_pimpl->reset_game();
}

void NESInterface::saveState() {
    m_pimpl->saveState();
}

std::string NESInterface::getSnapshot() const {
    return m_pimpl->getSnapshot();
}

void NESInterface::restoreSnapshot(const std::string snapshot) {
    m_pimpl->restoreSnapshot(snapshot);
}

void NESInterface::getScreen(unsigned char *screen, int screen_size) {
         m_pimpl->getScreen(screen, screen_size);
}

const int NESInterface::getScreenHeight() const {
	return m_pimpl->getScreenHeight();
}

const int NESInterface::getScreenWidth() const {
	return m_pimpl->getScreenWidth();
}

void NESInterface::fillRGBfromPalette(unsigned char *raw_screen, unsigned char *rgb_screen, int raw_screen_size) {
        m_pimpl->fillRGBfromPalette(raw_screen, rgb_screen, raw_screen_size);
}

void NESInterface::setMaxNumFrames(int newMax) {
    m_pimpl->setMaxNumFrames(newMax);
}

int NESInterface::getEpisodeFrameNumber() const {
    return m_pimpl->getEpisodeFrameNumber();
}

int NESInterface::getFrameNumber() const {
    return m_pimpl->getFrameNumber();
}

int NESInterface::getNumLegalActions() {
	return m_pimpl->getNumLegalActions();
}

void NESInterface::getLegalActionSet(int legal_actions[]) {
        m_pimpl->getLegalActionSet(legal_actions);
}

int NESInterface::minReward() const {
    return m_pimpl->minReward();
}

int NESInterface::maxReward() const {
    return m_pimpl->maxReward();
}

int NESInterface::lives() const {
    return m_pimpl->lives();
}

const int NESInterface::getCurrentScore() const {
	return m_pimpl->getCurrentScore();
}

int NESInterface::act(int action) {
    return m_pimpl->act(action);
}

NESInterface::NESInterface(const std::string &rom_file) :
    m_pimpl(new NESInterface::Impl(rom_file)) {

}

NESInterface::~NESInterface() {
    delete m_pimpl;
}

} // namespace nes

