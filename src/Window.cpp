#include "../include/Window.h"
#include "../include/Constants.h"
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <filesystem>

/**
 * @brief Construct a new SDL window and initialise TTF and renderer.
 */
WindowSDL::WindowSDL(int w, int h)
    : width(w), height(h), zoomFactor(1.0f), offset(0, 0), paused(false) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("N-Body Simulation", 
             SDL_WINDOWPOS_CENTERED, 
             SDL_WINDOWPOS_CENTERED, 
             width, height, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font     = TTF_OpenFont("../src/JetBrainsMonoNLNerdFontMono-Regular.ttf", 16); 
}

WindowSDL::~WindowSDL() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void WindowSDL::clear() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}


SDL_Point WindowSDL::worldToScreen(const Vector2D& pos) const {
    return {
        // Here we map the world coordinates to screen coordinates
        static_cast<int>(width  * 0.5f + ((pos.x - offset.x) * (width  * 0.5f) / (5.0f *zoomFactor))),
        static_cast<int>(height * 0.5f - ((pos.y - offset.y) * (height * 0.5f) / (5.0f *zoomFactor)))
    };
}

void WindowSDL::drawFilledCircle(SDL_Point center, int radius, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 200);

    int cx = center.x;
    int cy = center.y;
    for (int dy = -radius; dy <= radius; ++dy) {
        int dxLimit = static_cast<int>(std::sqrt(radius * radius - dy * dy));
        int y = cy + dy;
        SDL_RenderDrawLine(renderer, cx - dxLimit, y, cx + dxLimit, y);
    }
}


void WindowSDL::renderBodies(const std::vector<Body*>& bodies) {
    float maxSpeed = 0.13f;                                                     // Maximum speed for color interpolation
    for (const Body* body: bodies) {
        if (!body) continue;

        SDL_Point screenPos = worldToScreen(body->pos);                         // Get the body's screen position
        int radius = (body->isCentral) ? 30 / zoomFactor : 2 / zoomFactor;      // Assign propper radius for visualisation 

        if (body->isCentral) {                                                  // If body is central, draw it in black
            drawFilledCircle(screenPos, radius, 0, 0, 0); 
        } else {
            float t = body->vel.mag() / maxSpeed;                               // Normalize speed to [0, 1]
            SDL_Color color = interpolateColour(t);                             // Interpolate color based on speed 
            drawFilledCircle(screenPos, radius, color.r, color.g, color.b);     
        }
    }
}


// In WindowSDL.cpp
void WindowSDL::renderQuads(const std::vector<Node>& nodes) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);      // Light gray

    for (const Node& node : nodes) {
        float halfsize = node.quad.size / 2.0f;                // Get the halfsize of the node
        Vector2D c = node.quad.center;                         // Get the centre of the node

        // Draw the node!
        SDL_Point points[5] = {
            worldToScreen(c + Vector2D(-halfsize,  halfsize)), // Top left
            worldToScreen(c + Vector2D( halfsize,  halfsize)), // Top right
            worldToScreen(c + Vector2D(halfsize,  -halfsize)), // Bottom right
            worldToScreen(c + Vector2D(-halfsize, -halfsize)), // Bottom left
            worldToScreen(c + Vector2D(-halfsize,  halfsize))  // Close rectangle
        };
        SDL_RenderDrawLines(renderer, points, 5);              // You got it!
    }
}

SDL_Color WindowSDL::interpolateColour(float t) const {
    t = std::clamp(t, 0.0f, 1.0f);                             // Clamp between 0 and 1

    // Here I generate the colour gradient; we can set a general colour though. That will speed the rendering!!
    Uint8 r = static_cast<Uint8>(t * 255);
    Uint8 g = 64;
    Uint8 b = static_cast<Uint8>((1.0f - t) * 255);
    return { r, g, b, 255 };                                   // Gradient: blue → magenta → red
}


void WindowSDL::renderText(const std::string& text, int x, int y, SDL_Color colour) {
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), colour);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dst         = { x, y, surface->w, surface->h };

    SDL_RenderCopy(renderer, texture, nullptr, &dst);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void WindowSDL::display() {
    SDL_RenderPresent(renderer);

    if (isRecording) {
        SDL_Surface* frame = SDL_CreateRGBSurfaceWithFormat(0, width, height,
                                                           32, SDL_PIXELFORMAT_RGBA32);
        if (frame) {
            SDL_RenderReadPixels(renderer, nullptr, 
                                SDL_PIXELFORMAT_RGBA32, 
                                frame->pixels, frame->pitch);
            frames.push_back(frame);
            std::cout << "Frame captured, total frames: " << frames.size() << "\n";
        }
    }
}

bool WindowSDL::handleEvents() {
    static bool dragging = false;
    static int lastX = 0, lastY = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: return false;
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE  : togglePause(); break;
                    case SDLK_r      : resetView(); break;
                    case SDLK_s      : saveScreenshot("../results/screenshot.bmp"); break;
                    case SDLK_v      : 
                        if (!isRecording) {
                            isRecording = true;
                            frames.clear();
                            std::cout << "Recording started.\n";
                        } else {
                            isRecording = false;
                            std::cout << "Recording stopped" << std::endl;
                            saveVideo(); // Save recorded frames as video
                        }
                }
            break;

            case SDL_MOUSEWHEEL: {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                float scaleX = (width * 0.5f) / (2.0f * zoomFactor);
                float scaleY = (height * 0.5f) / (2.0f * zoomFactor);

                // World coords before zoom
                float worldXBefore = offset.x + (mouseX - width / 2.0f) / scaleX;
                float worldYBefore = offset.y - (mouseY - height / 2.0f) / scaleY;

                // Update zoom factor (zoom in = zoomFactor smaller)
                if (event.wheel.y > 0) zoomFactor /= 1.1f;
                else if (event.wheel.y < 0) zoomFactor *= 1.1f;

                // Recompute scale after zoom
                scaleX = (width * 0.5f) / (2.0f * zoomFactor);
                scaleY = (height * 0.5f) / (2.0f * zoomFactor);

                // World coords after zoom
                float worldXAfter = offset.x + (mouseX - width / 2.0f) / scaleX;
                float worldYAfter = offset.y - (mouseY - height / 2.0f) / scaleY;

                // Adjust offset so the point under the mouse stays fixed
                offset.x += worldXBefore - worldXAfter;
                offset.y += worldYBefore - worldYAfter;

                break;
            }

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = true;
                    lastX = event.button.x;
                    lastY = event.button.y;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    dragging = false;
                }
                break;

            case SDL_MOUSEMOTION:
                if (dragging) {
                    int dx = event.motion.x - lastX;
                    int dy = event.motion.y - lastY;
                    offset.x -= dx * 0.02 ;
                    offset.y += dy * 0.02 ;  

                    lastX = event.motion.x;
                    lastY = event.motion.y;  
                }
                break;
        }
    }
    return true;
}

//====== View manipulation methods ======

void WindowSDL::zoomIn()        { zoomFactor *= 1.1f; }
void WindowSDL::zoomOut()       { zoomFactor /= 1.1f; }
void WindowSDL::panLeft()       { offset.x   += 0.02f; }
void WindowSDL::panRight()      { offset.x   -= 0.02f; }
void WindowSDL::panUp()         { offset.y   -= 0.02f; }
void WindowSDL::panDown()       { offset.y   += 0.02f; }
void WindowSDL::resetView()     { zoomFactor = 1.0f; offset.x = offset.y = 0.f; }
void WindowSDL::togglePause()   { paused = !paused; }
bool WindowSDL::isPaused() const { return paused; }

/*
* Capture a screenshot of the current window and save it to the specified path.
*/
void WindowSDL::saveScreenshot(const std::string& path) {
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height,
                                                          32, SDL_PIXELFORMAT_RGBA32);

    SDL_RenderReadPixels(renderer, nullptr, 
                        SDL_PIXELFORMAT_RGBA32, 
                        surface->pixels, surface->pitch);

    SDL_SaveBMP(surface, path.c_str());
    SDL_FreeSurface(surface);
}


void WindowSDL::saveVideo() {
    std::filesystem::create_directories("../results/frames");
    for (size_t i = 0; i < frames.size(); ++i) {
        std::stringstream ss;
        ss << "../results/frames/frame_" << std::setw(4) << std::setfill('0') << i << ".bmp";
        SDL_SaveBMP(frames[i], ss.str().c_str());
        SDL_FreeSurface(frames[i]);
    }
    frames.clear();
}


