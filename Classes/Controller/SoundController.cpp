#include "SoundController.h"
#include "audio/include/AudioEngine.h"
#include "Constants/Constants.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace cocos2d::experimental;

SoundController* SoundController::instance = nullptr;

SoundController* SoundController::getInstance() {
    if (instance == nullptr) {
        instance = new SoundController();
        instance->musicVolume = 1.0f;
        instance->effectsVolume = 1.0f;
    }
    return instance;
}

void SoundController::preloadMusic(const std::string& filePath) {
    AudioEngine::preload(filePath);
}

int SoundController::playMusic(const std::string& filePath, bool loop) {
    Constants::currentSoundTrackPath = filePath;
    stopMusic(filePath);
    int audioId = AudioEngine::play2d(filePath, loop);
    AudioEngine::setVolume(audioId, musicVolume);
    playingMusic[filePath] = audioId;
    return audioId;
}

void SoundController::stopMusic() {
    AudioEngine::stopAll();
    playingMusic.clear();
}

void SoundController::stopMusic(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::stop(it->second);
        playingMusic.erase(it);
    }
}

bool SoundController::isMusicPlaying(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        return AudioEngine::getState(it->second) == AudioEngine::AudioState::PLAYING;
    }
    return false;
}

float SoundController::getMusicDuration(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        return AudioEngine::getDuration(it->second);
    }
    return 0.0f;
}

void SoundController::setMusicVolume(float volume) {
    musicVolume = volume;
    for (const auto& music : playingMusic) {
        AudioEngine::setVolume(music.second, volume);
    }
}

void SoundController::setMusicVolume(const std::string& filePath, float volume) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::setVolume(it->second, volume);
    }
}

void SoundController::pauseMusic(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::pause(it->second);
    }
}

void SoundController::resumeMusic(const std::string& filePath) {
    auto it = playingMusic.find(filePath);
    if (it != playingMusic.end()) {
        AudioEngine::resume(it->second);
    }
}

void SoundController::replayMusic(const std::string& filePath) {
    stopMusic(filePath);
    playMusic(filePath);
}

void SoundController::preloadSoundEffect(const std::string& filePath) {
    AudioEngine::preload(filePath);
}

int SoundController::playSoundEffect(const std::string& filePath, bool loop) {
    int audioId = AudioEngine::play2d(filePath, loop, effectsVolume);
    playingSoundEffects[audioId] = filePath;
    return audioId;
}

void SoundController::stopSoundEffect(int audioId) {
    AudioEngine::stop(audioId);
    playingSoundEffects.erase(audioId);
}

void SoundController::stopAllSoundEffects() {
    for (const auto& effect : playingSoundEffects) {
        AudioEngine::stop(effect.first);
    }
    playingSoundEffects.clear();
}

void SoundController::setEffectsVolume(float volume) {
    effectsVolume = volume;
    for (const auto& effect : playingSoundEffects) {
        AudioEngine::setVolume(effect.first, volume);
    }
}

void SoundController::pauseAllSoundEffects() {
    for (const auto& effect : playingSoundEffects) {
        AudioEngine::pause(effect.first);
    }
}

void SoundController::resumeAllSoundEffects() {
    for (const auto& effect : playingSoundEffects) {
        AudioEngine::resume(effect.first);
    }
}

void SoundController::scheduleSpawnEvents(const std::vector<float>& timestamps, const std::vector<std::function<void()>>& spawnFunctions) {
    spawnEvents.clear();
    for (size_t i = 0; i < timestamps.size(); ++i) {
        spawnEvents.emplace_back(timestamps[i], spawnFunctions[i]);
    }
    currentEventIndex = 0;
    elapsedTime = 0.0f;
}
