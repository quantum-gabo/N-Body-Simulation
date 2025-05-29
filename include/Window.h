#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "../include/Body.h"
#include "../include/Vector.h"
#include "../include/Quad.h"
#include "../include/Node.h"
#include <string>
#include <omp.h>

/**
 * @brief WindowSDL class for rendering an N-body simulation using SDL2.
 *
 * This class handles the creation of a window, rendering of bodies, quads,
 * text, and managing the view (zooming, panning). It uses SDL2 for graphics
 * rendering and SDL_ttf for text rendering.
 *
 * @note The class supports zooming and panning of the view, rendering bodies
 * as filled circles with colours based on their speed, and rendering quads
 * for the Barnes-Hut tree structure.
 *
 * @note The class also supports saving screenshots and video recording of the
 * simulation.
 *
 * @author J. Gabriel Balarezo, 2025
 */
class WindowSDL {
public:

    /**
     * @brief Constructor to initialize the SDL window and renderer.
     * @param width Width of the window.
     * @param height Height of the window.
     */
    WindowSDL(int width, int height);

    /**
     * @brief Destructor to clean up SDL resources.
     * Closes the font, destroys the renderer and window, and quits SDL and TTF.
     */
    ~WindowSDL();

    /**
     * @brief Clears the renderer with a white background. 
     */
    void clear();

    // Render methods for different elements

    /*
    * @brief Renders bodies as filled circles on the screen.
    * @param bodies Vector of pointers to Body objects to be rendered.
    * Each body is rendered at its position with a constant radius.
    * The color of the circle is determined by its speed, with a maximum speed
    * used for color interpolation.
    * If the body is a central body, it is rendered in black.
    */
    void renderBodies(const std::vector<Body*>& bodies);

    /**
     * @brief Renders quads for the Barnes-Hut tree structure.
     * @param nodes Vector of Node objects representing the quadtree nodes.
     * Each node is rendered as a rectangle on the screen.
     */
    void renderQuads(const std::vector<Node>& nodes);

    /**
     * @brief Renders text on the screen.
     * @param text The text to be rendered.
     * @param x The x-coordinate for the text position.
     * @param y The y-coordinate for the text position.
     * @param colour The color of the text.
     */
    void renderText(const std::string& text, int x, int y, SDL_Color colour);

    /**
     * @brief Clears the renderer and presents the current frame.
     * This method also captures frames for video recording if enabled.
     */
    void display();

        // Zoom and pan methods for the view
    
    /**
     * @brief Zooms in the view by increasing the zoom factor.
     */
    void zoomIn();

    /**
     * @brief Zooms out the view by decreasing the zoom factor.
     */
    void zoomOut();

    /**
     * @brief Pans the view to the left by adjusting the offset.
     */
    void panLeft();

    /**
     * @brief Pans the view to the right by adjusting the offset.
     */
    void panRight();

    /**
     * @brief Pans the view upwards by adjusting the offset.
     */
    void panUp();

    /**
     * @brief Pans the view downwards by adjusting the offset.
     */
    void panDown();

    /**
     * @brief Resets the view to the default zoom and offset.
     */
    void resetView();

    /**
     * @brief Saves a screenshot of the current window to the specified path.
     * @param path The file path where the screenshot will be saved.
     */
    void saveScreenshot(const std::string& path);

    
    /**
     * @brief Saves the recorded video frames to a file.
     * This method creates a directory for frames and saves each frame as a BMP file.
     */
    void saveVideo();

    /**
     * @brief Toggles the pause state of the simulation.
     * If paused, the simulation will not update until unpaused.
     */
    void togglePause();

    /**
     * @brief Checks if the simulation is currently paused.
     * @return True if paused, false otherwise.
     */
    bool isPaused() const;

    /**
     * @brief Handles SDL events such as keyboard and mouse input.
     * This method processes events for zooming, panning, pausing, and quitting the application.
     * @return True if the application should continue running, false if it should quit.
     */
    bool handleEvents();

private:
    SDL_Window* window;      ///< SDL window handle
    SDL_Renderer* renderer;  ///< SDL renderer for drawing
    TTF_Font* font;          ///< Font for text rendering

    int width;               ///< Window width in pixels
    int height;              ///< Window height in pixels
    float zoomFactor;        ///< Current zoom magnification factor
    Vector2D offset;         ///< Current view offset (panning)
    bool paused;             ///< Pause state flag
    bool isRecording;        ///< Video recording state flag
    std::vector<SDL_Surface*> frames; ///< Captured frames for video recording
    
    /**
     * @brief Converts world coordinates to screen coordinates.
     * @param pos The world position to convert.
     * @return The corresponding screen position as SDL_Point.
     */
    SDL_Point worldToScreen(const Vector2D& pos) const;

    /**
     * @brief Draws a filled circle on the screen.
     * @param center The center of the circle as SDL_Point.
     * @param radius The radius of the circle in pixels.
     * @param r Red component of the color (0-255).
     * @param g Green component of the color (0-255).
     * @param b Blue component of the color (0-255).
     */
    void drawFilledCircle(SDL_Point center, int radius, Uint8 r, Uint8 g, Uint8 b);

    /**
     * @brief Interpolates a color based on the speed of a body.
     * This method generates a color gradient from blue to magenta to red based on the body's speed.
     * @param t Normalized speed value (0.0 to 1.0).
     * @return The interpolated SDL_Color.
     */
    SDL_Color interpolateColour(float t) const;
};
