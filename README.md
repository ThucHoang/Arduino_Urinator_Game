Arduino: Urinator Game
=====================

An attempt at gamification of peeing for men.

This was a project where I programmed this urinator game for my group at University of Oslo. Please note that I had zero knowledge of C/C++ when I created this. The course this was created was [INF1510](http://www.uio.no/studier/emner/matnat/ifi/INF1510/) Spring 2013! I know that my code could be a lot better, but it works as intended :)

I had a whole list of features I would like to add if I ever were to pick up this project again.

1. Add a highscore list - As of now it only shows the highest value
2. Better scoring algorithm - This algoritm has 2 possible outcomes. If it's a higher temperature than what it has measured add +30 points. If not add +10.
3. Sounds - It has no sound right now.

The idea of this project was to create something that would be useful in our daily lives. Peeing for men has always been a problem for pubs and public restrooms. If there were something (like this urinator game) that could make men focus more, while peeing the chance of peeing outside of the toilet would decrease rapidly. 

List of extra items you need for this project
===

1. 1x temperature sensor compatible with [OneWire](http://playground.arduino.cc/Learning/OneWire)
2. 2x 4 digit segment display - Here is some [instructions](http://www.hacktronics.com/Tutorials/arduino-and-7-segment-led.html) how to pin it correctly!