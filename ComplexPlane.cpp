#include "ComplexPlane.h"
#include <sstream> 

//NOTE: You can adjust the window size and MAX_ITER to make it run faster or slower if needed for testing/performance

// Constructor for ComplexPlane
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) 
{
    m_pixel_size = Vector2i(pixelWidth, pixelHeight);                   // Size of each pixel
    m_aspectRatio = (pixelWidth * 1.0) / (pixelHeight * 1.0);           // Calculates aspect ratio according to pixel size, used to avoid plane distortion
    m_plane_center = Vector2f(0.0, 0.0);                                // Center of plane object
    m_plane_size = Vector2f(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio);   // Size of plane object
    m_zoomCount = 0;                                                    // Zoom Count of plane object
    m_state = CALCULATING;                                              // State of plane object
    // NOTE: Instructions said to initialize VertexArray. Maybe it's already fine, but noting this in case any weird errors happen. Initialization may just mean what is below.
    m_vArray.setPrimitiveType(Points);                                  // Sets primitive type of vertex array to Points
    m_vArray.resize(pixelWidth * pixelHeight);                          // Sets size of vertex array according to pixel size
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const 
{
target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
/*If m_State is CALCULATING
    Create a double for loop to loop through all pixels in the screen height and width
        Use j for x and i for y
            Note:  be careful not to transpose these!
        Set the position variable in the element of VertexArray that corresponds to the screen coordinate j,i
            This involves mapping the two-dimensional position at j,i to its one-dimensional array index:
                vArray[j + i * pixelWidth].position = { (float)j,(float)i };
        Use ComplexPlane::mapPixelToCoords to find the Vector2f coordinate in the complex plane that corresponds to the screen pixel location at j,i
        Call ComplexPlane::countIterations with the Vector2f coordinate as its argument and store the number of iterations
        Declare three local Uint8 variables r,g,b to store the RGB values for the current pixel
            Uint8 is an alias for unsigned char
        Pass the number of iterations and the RGB variables into ComplexPlane::iterationsToRGB
            This will assign the RGB values by reference
        Set the color variable in the element of VertexArray that corresponds to the screen coordinate j,i
            vArray[j + i * pixelWidth].color = { r,g,b };
    Set the state to DISPLAYING*/
  
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

void ComplexPlane::setCenter(Vector2i mousePixel)
{
    //>> HAS NOT BEEN TESTED
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousPixel)
{
    //>> HAS NOT BEEN TESTED
    m_mouseLocation = mapPixelToCoords(mousPixel);
}

void ComplexPlane::loadText(Text& text)
{
    string fullString;
    stringstream textStream;
    textStream << "Mandelbrot Set" << endl;
    //TODO: MAKE CENTER ONLY UPDATE UPON CLICKING.
    textStream << "Center: (" << m_plane_center.x << ',' << m_plane_center.y << ')' << endl;
    textStream << "Cursor: (" << m_mouseLocation.x << ',' << m_mouseLocation.y << ')' << endl;
    textStream << "Left-click to Zoom in" << endl;
    textStream << "Right-click to Zoom out" << endl;
    fullString = textStream.str();
    text.setString(fullString);
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
//Count the number of iterations of the set for the given coordinate as specified above
    return 0;   // Placeholder return
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) 
{
/*Map the given iteration count to an r,g,b color and assign the given reference variables
  You are free to create and experiment with your own color scheme
  You may want to start with gray scale, where r,g,b are always the same value in the range [0,255]
  I used the following strategy:
      At MAX_ITER I colored the pixel black {0,0,0}
      Between [0 : MAX_ITER) I divided the colors into 5 regions:
          Purple / blue for low iteration counts
          Turquoise
          Green
          Yellow
          Red for high iteration counts
      You can create a color "sliding" effect to differentiate more colors by adding or subtracting the iteration count to one color within a region
  You can experiment with HSL color mapping to see which values to assign for each region
  Set S to 100% and L to 50% and slide the H:
      Color Mapping: SEE "https://www.w3schools.com/colors/colors_hsl.asp" */

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