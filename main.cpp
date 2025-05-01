
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

// Seems to be a test function to aid in development. Not fully sure what it entails but it probably makes more sense when looking more into how the Mandelbrot works.
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
    // Initialize engine related objects
    VideoMode vm(650, 450);                                     // Creates a VideoMode object (common default is 1920 x 1080)
    RenderWindow window(vm, "Mandelbrot", Style::Default);      // Creates and opens a window for the game
    ComplexPlane plane(2, 2);                                   // ComplexPlane object
    Text text;                                                  // Text object to display text
    Font berlinSans;                                            // Font object to store font
        if (!berlinSans.loadFromFile("BRLNSDB.TTF"))
        {cout << "Error: Font cannot be loaded" << endl;}

    // Starter user prompt code copied from Chaos Game project, mostly here for quick reference for how to perform operations to it and to be replaced or reworked
    text.setFont(berlinSans);
    text.setString("Placeholder Text");
    text.setCharacterSize(50);
    text.setFillColor(Color::Cyan);
    text.setStyle(Text::Regular);
    text.setPosition(50, 50);
    
    // Initializes doubles, likely used to construct Mandelbrot?
    double re = -0.77568377;
    double im = 0.13646737;
    complex<double> c (re, im);

    // Tests convergence and prints to the console
    testConvergence(c);
    cout << endl;

    // Sets c to different values and tests again
    c = {-0.77568377, 0.23646737};
    testConvergence(c);

	// Runtime loop
	while (window.isOpen())
	{
		/*
		****************************************
		Handle the players input
		****************************************
		*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				// Quit the game when the window is closed
				window.close();
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				// Left Click
				if (event.mouseButton.button == sf::Mouse::Left)
				{
                    // Zoom in on the plane
                    cout << "+" << endl;
                    plane.zoomIn();
				}
				// Right Click
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
                    // Zoom out of the plane
                    cout << "-" << endl;
                    plane.zoomOut();
				}
			}
            // Constantly calls and updates mouse location whenever it moves
            if (event.type == sf::Event::MouseMoved)
            {
                plane.setMouseLocation(Vector2i(event.mouseButton.x, event.mouseButton.y));
            }
		}
		// Escape key; closes the window
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

        /*
        ****************************************
        Update Scene
        ****************************************
        */

        // Calls updateRender and loadText on ComplexPlane object
        plane.updateRender();
        plane.loadText(text);


        /*
        ****************************************
        Window Rendering
        ****************************************
        */

		// Clear window display
		window.clear();

		// Draws plane and text to window
        plane.draw(window, sf::RenderStates::Default);
        window.draw(text);

        // Show constructed window display
		window.display();
	}

    return 0;
}
