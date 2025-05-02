#include "ComplexPlane.h"
#include <sstream> 
#include <iomanip>

//NOTE: You can adjust the window size and MAX_ITER to make it run faster or slower if needed for testing/performance

// Temporary inclusions for testing purposes
#include <iostream>
using namespace std;

// Constructor for ComplexPlane
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) 
{
    m_pixel_size = Vector2i(pixelWidth, pixelHeight);                   // Size of each pixel
    m_aspectRatio = (pixelWidth * 1.0) / (pixelHeight * 1.0);           // Calculates aspect ratio according to pixel size, used to avoid plane distortion
    m_plane_center = Vector2f(0.0, 0.0);                                // Center of plane object
    m_plane_size = Vector2f(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio);   // Size of plane object
    m_zoomCount = 0;                                                    // Zoom Count of plane object
    m_state = CALCULATING;                                              // State of plane object
    m_vArray.setPrimitiveType(Points);                                  // Sets primitive type of vertex array to Points
    m_vArray.resize(pixelWidth * pixelHeight);                          // Sets size of vertex array according to pixel size
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const 
{
target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    // Behavior for Calculating state
    if (m_state == CALCULATING)
    {
        // Iterate through all pixels on screen; j is x and i is y
        for (int j = 0; j < VideoMode::getDesktopMode().width / 2; j++)
        {
            for (int i = 0; i < VideoMode::getDesktopMode().height / 2; i++)
            {
                int index = j + i * m_pixel_size.x;
                
                // Set position variable in the element of VertexArray that corresponds to the screen coordinate j,i
                m_vArray[index].position = Vector2f((float)j, (float)i);

                //>> Map to complex plane
                Vector2f complexCoordinate = this->mapPixelToCoords(Vector2i(j, i));

                // Count iterations in current coordinate and store in variable
                size_t iterations = this->countIterations(complexCoordinate);

                //>> turn the iteration count to RGB values
                Uint8 r, g, b;
                this->iterationsToRGB(iterations, r, g, b);
                
                m_vArray[index].color = Color(r, g, b);
            }
        }

        m_state = DISPLAYING;

        std::cout << "Finished calculating!" << std::endl;
    }
 
}

void ComplexPlane::zoomIn()
{
    ++m_zoomCount;
    m_plane_size = Vector2f(BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount)), BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount)));
    m_state = CALCULATING;
}

void ComplexPlane::zoomOut() 
{
     --m_zoomCount;
    m_plane_size = Vector2f(BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount)), BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount)));
    m_state = CALCULATING;
}

void ComplexPlane::pan(Vector2f direction)
{
    if (m_state == DISPLAYING)
    {
        m_plane_center = Vector2f((m_plane_center.x + direction.x / (10 + pow(2, m_zoomCount))), (m_plane_center.y + direction.y / (10 + pow(2, m_zoomCount))));
        m_state = CALCULATING;
    }
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousPixel)
{
    m_mouseLocation = mapPixelToCoords(mousPixel);
}

void ComplexPlane::loadText(Text& text)
{
    string fullString;
    stringstream textStream;
    textStream << "Center: (" << m_plane_center.x << ',' << m_plane_center.y << ')' << endl;
    textStream << "Cursor: (" << m_mouseLocation.x << ',' << m_mouseLocation.y << ')' << endl;
    fullString = textStream.str();
    text.setString(fullString);
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
    //Count the number of iterations of the set for the given coordinate

    size_t count = 0;

    //>> initalize x and y coords, and x^2, y^2
    float x = 0.0f;
    float y = 0.0f;
    float x2 = 0.0f;
    float y2 = 0.0f;

            //>> check if abs(z) > 4, if z = x^2 + y^2
            //>> make sure the count is not going for ever
    while (x2 + y2 <= 4.0f && count < MAX_ITER)
    {
            //>> use old x and y to calculate new
            //>> y is imaginary component
        y = 2 * x * y + coord.y;
        x = x2 - y2 + coord.x;
            
            //>> recalculate x^2, y^2 every iteration of the loop
        x2 = x * x;
        y2 = y * y;

        ++count;
    }

    return count;   // return
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    /*Map the given iteration count to an r,g,b color and assign the given reference variables
          Color Mapping: SEE "https://www.w3schools.com/colors/colors_hsl.asp" */

    //>> if the count is at or exceeds the max iteration, the pixel should be black
    //>> this is exactly where we could implement a gradient, replacing the black pixels.
    //>> Im thinking their values would be decided by their position on the plane ~

    if (count >= MAX_ITER)
    {
        r = g = b = 0;

        return;
    }


    //>> make count a float somewhere between 0 and 1
    float t = static_cast<float>(count) / MAX_ITER;


    //>> if else to divide range into 5 color regions
    if (t < 0.2f)
    {
        //>> Purple->Blue
        r = static_cast<Uint8>(128 - 128 * (t / 0.2f));
        g = 0;
        b = static_cast<Uint8>(255 * (t / 0.2f));
    }
    else if (t < 0.4f)
    {
        //>> Blue->Turquoise
        r = 0;
        g = static_cast<Uint8>(255 * ((t - 0.2f) / 0.2f));
        b = 255;
    }
    else if (t < 0.6f)
    {
        //>>Turquoise->Green
        r = 0;
        g = 255;
        b = static_cast<Uint8>(255 - 255 * ((t - 0.4f) / 0.2f));
    }
    else if (t < 0.8f)
    {
        //>> Green->Yellow
        r = static_cast<Uint8>(255 * ((t - 0.4f) / 0.2f));
        g = 255;
        b = 0;
    }
    else
    {
        //>> Yellow->Red
        r = 255;
        g = static_cast<Uint8>(255 - 255 * ((t - 0.8f) / 0.2f));
        b = 0;
    }

}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    //>> Calculate the Boundaries of the complex plane given plane center and size
    float left = m_plane_center.x - m_plane_size.x / 2.0f;
    float right = m_plane_center.x + m_plane_size.x / 2.0f;
    float top = m_plane_center.y + m_plane_size.y / 2.0f;
    float bottom = m_plane_center.y - m_plane_size.y / 2.0f;

    //>> Map x pixel with given formula
    float real = ((mousePixel.x - 0.0f) / (m_pixel_size.x - 0.0f)) * (right - left) + left;
    //>> y pixel
    float imaginary = ((mousePixel.y - static_cast<float>(m_pixel_size.y)) / -m_pixel_size.y) * (top - bottom) + bottom;

    return Vector2f(real, imaginary);

}