
/*Get the desktop resolution using:
    VideoMode::getDesktopMode().width;
    VideoMode::getDesktopMode().height;
    Note:  you may want to divide these values by 2 to start with a smaller screen so your program runs faster
Construct the RenderWindow
Construct the ComplexPlane
Construct the Font and Text objects*/

/*Begin the main loop
    Handle Input segment
        Poll Windows queue events
            Handle Event::Closed event to close the window
            Handle Event::MouseButtonPressed
                Right click will zoomOut and call setCenter on the ComplexPlane object with the (x,y) pixel location of the mouse click
                Left click will zoomIn and call setCenter on the ComplexPlane object with the (x,y) pixel location of the mouse click
            Handle Event::MouseMoved
                Call setMouseLocation on the ComplexPlane object to store the (x,y) pixel location of the mouse click
                This will be used later to display the mouse coordinates as it moves
        Check if Keyboard::isKeyPressed(Keyboard::Escape) to close the window
    Update Scene segment
        Call updateRender on the ComplexPlane object
        Call loadText on the ComplexPlane object
    Draw Scene segment
        Clear the RenderWindow object
        draw the ComplexPlane object
        draw the Text object
        Display the RenderWindow object*/


//EVERYTHING BELOW HERE IS AN EXAMPLE DIRECTLY FROM PROJECT PAGE

#include <iostream>     // std::cout
#include <complex>      // std::complex, std::abs
#include "ComplexPlane.h"  //THIS LINE IS NOT FROM EXAMPLE; ADDED LATER.
using namespace std;
using namespace sf;

void testConvergence(complex<double> c)
{
    cout << "c = " << c << endl;
    cout << "Real component of c: " << c.real() << endl;
    cout << "Imaginary component of c: " << c.imag() << endl;
    complex<double> z  = c;
    int i = 0;
    while(abs(z) < 2.0 && i < 64)
    {
        z = z*z + c;
        //cout << "z_" << i << "= " << z << endl;
        //cout << "|z| = " << abs(c) << endl;
        i++;
    }
    if(i == 64) cout << c << " converges!" << endl;
    else cout << c << " escapes after " << i << " iterations" << endl;
}

int main ()
{
    double re = -0.77568377;
    double im = 0.13646737;
    complex<double> c (re, im);
    testConvergence(c);
    cout << endl;
    c = {-0.77568377, 0.23646737};
    testConvergence(c);
    return 0;
}
