Ben Isenberg - bji6@pitt.edu

My algorithm works as follows:

-First I initialize a 128 byte array to all values to be -1.  This array
is used to hold the rain drops that appear on screen.  Everytime a
random number is generated I set that element in the array to 0.
Then whenever I update the rain drops I just loop through the array
and all values that are greater than -1 are incremented by 1.  If the
value is greater than 7 then it reverts back to 0 so it wraps around
the screen.  So the value that is stored is the rain drop's y coordinate
and the position in the array is its x coordinate.

-After the array values are initialized I proceed into my spinloop
which checks for input from the user and moves the player accordingly.
It loops for as long as the word "delay" is set to.  To move the player
up and down you just offset the memory address by +32 or -32.  To move
left and right I use the formula: byte offset = (y*32) + (x/4).
Then I take the remainder of x/4 to determine which two bits in the 
byte I am at to change to "01".

-Then I draw the rain drops and update them and then return to the 
spinloop and this process continues until the player is hit by water
or his x coordinate = 127.

-If he gets to the otherside successfully, then I count how many rain
drops were stored in the array to determine how many gold pieces to 
display.  127 - # of rain drops is the equation I use.

-The rate of rain increases due to the fact that as the player moves to 
the right there are less available locations for rain to be placed.

-To generate the randomly placed gold pieces I just generate two random
numbers to represent the x and y coordinates for each piece.  I then
store each piece in the appropriate spot in their own array.

-Then I clear the entire display by setting every byte in memory to 0.

-I draw all the gold pieces and redraw the player back to his starting
position and allow him approximately 30 seconds to collect as many gold
pieces as possible.

-After time is up I end the game.

Bugs:

-If the player is in umbrella mode and rain is drawn over him, he
will appear to disappear but if you continue to move him with the 
arrow keys he will reappear.

-The amount of gold collected is not kept track off.  I couldn't
find a good way to do this so it displays at the end that you have
0 gold no matter how much you collected.

-The rain begins to fall from the second LED down so the top row of LEDs
never has rain on it.  Rain also wraps around to the second row.

-Sometimes if you hold down an arrow key for more than a few seconds
it can cause the program to freeze.  