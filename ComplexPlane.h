#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp> // Includes graphics definitions, also added after example
using namespace std;
using namespace sf;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum State {CALCULATING, DISPLAYING};
/*^^^^ When the program starts or when the user clicks the mouse to move, we will be CALCULATING the complex plane display
    This is very CPU intense, so we will set the state variable back to DISPLAYING after it is done so we only have to do it once per click*/

//TODO: Declare the rest of ComplexPlane as specified in the UML diagram

class ComplexPlane
{
    public:
            // Public functions
        ComplexPlane(int pixelWidth, int pixelHeight);
        void draw(RenderTarget& target, RenderStates states) const;
        void zoomIn();
        void zoomOut();
        void setCenter(Vector2i mousePixel);
        void setMouseLocation(Vector2i mousePixel);
        void loadText(Text& text);
        void updateRender();
    private:
            // Private variables
        VertexArray m_vArray;
        State m_state;
        Vector2f m_mouseLocation;
        Vector2i m_pixel_size;
        Vector2f m_plane_size;
        int m_zoomCount;
        float m_aspectRatio;
            // Private functions
        size_t countIterations(Vector2f coord);
        void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
        Vector2f mapPixelToCoords(Vector2i mousePixel);
};

#endif