#include "nes_interface_c.h"

nes::NESInterface *NESInterface(char* ROM) {
        return new nes::NESInterface(ROM);
}

void delete_NES(nes::NESInterface *nes) {
        delete nes;
}

void resetGame(nes::NESInterface *nes) {
        nes->resetGame();
}

bool gameOver(nes::NESInterface *nes) {
        return nes->gameOver();
}

int act(nes::NESInterface *nes, int action) {
        return nes->act(action);
}

int getNumLegalActions(nes::NESInterface *nes) {
        return nes->getNumLegalActions();
}

void getLegalActionSet(nes::NESInterface *nes, int legal_actions[]) {
        nes->getLegalActionSet(legal_actions);
}

int getFrameNumber(nes::NESInterface *nes) {
        return nes->getFrameNumber();
}

void setMaxNumFrames(nes::NESInterface *nes, int max_frames) {
        nes->setMaxNumFrames(max_frames);
}

int minReward(nes::NESInterface *nes) {
        return nes->minReward();
}

int maxReward(nes::NESInterface *nes) {
        return nes->maxReward();
}

int lives(nes::NESInterface *nes) {
        return nes->lives();
}

int getEpisodeFrameNumber(nes::NESInterface *nes) {
        return nes->getEpisodeFrameNumber();
}

void getScreen(nes::NESInterface *nes, unsigned char *screen, int screen_size) {
        nes->getScreen(screen, screen_size);
}

int getScreenHeight(nes::NESInterface *nes) {
        return nes->getScreenHeight();
}

int getScreenWidth(nes::NESInterface *nes) {
        return nes->getScreenWidth();
}

int getCurrentScore(nes::NESInterface *nes) {
        return nes->getCurrentScore();
}

void saveState(nes::NESInterface *nes) {
        nes->saveState();
}

bool loadState(nes::NESInterface *nes) {
        return nes->loadState();
}

std::string getSnapshot(nes::NESInterface *nes) {
        return nes->getSnapshot();
}

void restoreSnapshot(nes::NESInterface *nes, char *snapshot) {
        nes->restoreSnapshot(snapshot);
}

void fillRGBfromPalette(nes::NESInterface *nes, unsigned char *raw_screen, unsigned char *rgb_screen, int raw_screen_size) {
        nes->fillRGBfromPalette(raw_screen, rgb_screen, raw_screen_size);
}
