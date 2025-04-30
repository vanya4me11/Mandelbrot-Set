#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H
#endif

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum State {CALCULATING, DISPLAYING};
/*^^^^ When the program starts or when the user clicks the mouse to move, we will be CALCULATING the complex plane display
    This is very CPU intense, so we will set the state variable back to DISPLAYING after it is done so we only have to do it once per click*/

//TODO: Declare the rest of ComplexPlane as specified in the UML diagram

