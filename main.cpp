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
    VideoMode vm(650, 450);                                                                             // Creates a VideoMode object (common default is 1920 x 1080)
    RenderWindow window(vm, "Mandelbrot", Style::Default);                                              // Creates and opens a window for the game
    ComplexPlane plane(VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height / 2);          // ComplexPlane object
    Text text;                                                                                          // Text object to display text
    Font berlinSans;                                                                                    // Font object to store font
        if (!berlinSans.loadFromFile("BRLNSDB.TTF"))
        {cout << "Error: Font cannot be loaded" << endl;}

    // Starter user prompt code copied from Chaos Game project, mostly here for quick reference for how to perform operations to it and to be replaced or reworked
    text.setFont(berlinSans);
    text.setString("Placeholder Text");
    text.setCharacterSize(20);
    text.setFillColor(Color::White);
    text.setStyle(Text::Regular);
    text.setPosition(10, 10);

    vector<Text> controlsList(3);
    controlsList.at(0).setString("Left Click: Zoom In");
    controlsList.at(1).setString("Right Click: Zoom Out");
    controlsList.at(2).setString("WASD / Arrow Keys: Pan");
    for (int i = 0; i < controlsList.size(); i++)
    {
        controlsList.at(i).setFont(berlinSans);
        controlsList.at(i).setCharacterSize(15);
        controlsList.at(i).setFillColor(Color::White);
        controlsList.at(i).setStyle(Text::Regular);
        controlsList.at(i).setPosition(8, (VideoMode::getDesktopMode().height / 2) - (98 + 15 * (3 - i)));
    }
    
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
                    plane.zoomIn();
                    plane.setCenter(sf::Mouse::getPosition(window));
				}
				// Right Click
				else if (event.mouseButton.button == sf::Mouse::Right)
				{
                    // Zoom out of the plane
                    plane.zoomOut();
                    plane.setCenter(sf::Mouse::getPosition(window));
				}
			}
            // Constantly calls and updates mouse location whenever it moves
            if (event.type == sf::Event::MouseMoved)
            {
                plane.setMouseLocation(sf::Mouse::getPosition(window));
            }
		}
		// Escape key; closes the window
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
        // WASD and Arrow Keys to pan the camera around
        if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
        {
            plane.pan(Vector2f(0.0, 1.0));
        }
        if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
        {
            plane.pan(Vector2f(-1.0, 0.0));
        }
        if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
        {
            plane.pan(Vector2f(0.0, -1.0));
        }
        if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
        {
            plane.pan(Vector2f(1.0, 0.0));
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
        for (Text controls : controlsList)
        {
            window.draw(controls);
        }

        // Show constructed window display
		window.display();
	}

    return 0;
}
