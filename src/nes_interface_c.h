#ifndef NES_INTERFACE_C_H
#define NES_INTERFACE_C_H

/* This is a thin wrapper to nes_interface.hpp 
   that is pure C to allow it to be called by
   Python's ctypes.
*/

#include "nes_interface.hpp"

extern "C" {

        nes::NESInterface *NESInterface(char* ROM);

        void delete_NES(nes::NESInterface *nes);

        void resetGame(nes::NESInterface *nes);

        bool gameOver(nes::NESInterface *nes);

        int act(nes::NESInterface *nes, int action);
        
        int getNumLegalActions(nes::NESInterface *nes);

        void getLegalActionSet(nes::NESInterface *nes, int legal_actions[]);

        int getFrameNumber(nes::NESInterface *nes);

        void setMaxNumFrames(nes::NESInterface *nes, int max_frames);

        int minReward(nes::NESInterface *nes);

        int maxReward(nes::NESInterface *nes);

        int lives(nes::NESInterface *nes);

        int getEpisodeFrameNumber(nes::NESInterface *nes);

        void getScreen(nes::NESInterface *nes, unsigned char *screen, int screen_size);

        int getScreenHeight(nes::NESInterface *nes);

        int getScreenWidth(nes::NESInterface *nes);

        int getCurrentScore(nes::NESInterface *nes);

        void saveState(nes::NESInterface *nes);

        bool loadState(nes::NESInterface *nes);

        std::string getSnapshot(nes::NESInterface *nes);

        void restoreSnapshot(nes::NESInterface *nes, char *snapshot);

        void fillRGBfromPalette(nes::NESInterface *nes, unsigned char *raw_screen, unsigned char *rgb_screen, int raw_screen_size);

} // extern "C"

#endif // NES_INTERFACE_C_H
