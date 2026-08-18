#pragma once

#include "Event.h"
#include "Platform.h"

#if defined(__has_include)
#  if __has_include(<SDL2/SDL.h>)
#    include <SDL2/SDL.h>
#    define BERKENGINE_HAS_SDL2 1
#  elif __has_include(<SDL.h>)
#    include <SDL.h>
#    define BERKENGINE_HAS_SDL2 1
#  endif
#endif

#ifndef BERKENGINE_HAS_SDL2
#  define BERKENGINE_HAS_SDL2 0
#endif

#include <cstdint>
#include <string>
#include <utility>

namespace BerkEngine {

class SDLPlatform : public IPlatform {
public:
    struct Config {
        std::string title{"BerkEngine"};
        std::int32_t width{1280};
        std::int32_t height{720};
    };

    SDLPlatform()
        : SDLPlatform(Config{})
    {}

    explicit SDLPlatform(Config config)
        : mConfig(std::move(config))
    {}

    void initialize() override {
#if BERKENGINE_HAS_SDL2
        if (mInitialized) {
            return;
        }

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            return;
        }

        mWindow = SDL_CreateWindow(
            mConfig.title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            mConfig.width,
            mConfig.height,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (mWindow == nullptr) {
            SDL_Quit();
            return;
        }
        mInitialized = true;
#endif
    }

    void pollEvents(EventQueue& events) override {
#if BERKENGINE_HAS_SDL2
        if (!mInitialized) {
            return;
        }

        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent) == 1) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    mShouldClose = true;
                    events.push(Event{EventType::Quit, false});
                    break;
                case SDL_WINDOWEVENT:
                    if (sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                        events.push(Event{EventType::WindowResized, false});
                    }
                    break;
                case SDL_KEYDOWN:
                    events.push(Event{EventType::KeyPressed, false});
                    break;
                case SDL_KEYUP:
                    events.push(Event{EventType::KeyReleased, false});
                    break;
                case SDL_MOUSEMOTION:
                    events.push(Event{EventType::MouseMoved, false});
                    break;
                default:
                    break;
            }
        }
#else
        (void)events;
#endif
    }

    bool shouldClose() const override {
        return mShouldClose;
    }

    void shutdown() override {
#if BERKENGINE_HAS_SDL2
        if (mWindow != nullptr) {
            SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
        }

        if (mInitialized) {
            SDL_Quit();
        }
#endif
        mShouldClose = false;
        mInitialized = false;
    }

    bool isInitialized() const {
        return mInitialized;
    }

private:
    Config mConfig{};
    bool mInitialized{false};
    bool mShouldClose{false};
#if BERKENGINE_HAS_SDL2
    SDL_Window* mWindow{nullptr};
#endif
};

} // namespace BerkEngine
