Name: Ben Isenberg

Email: bji6@pitt.edu

Course: CS 0445 Data Structures

Assignment 1: Traveling Salesman Problem

Date: September 26, 2010

Introductory Comments:  

My program follows the basic template given by Dr. Aronis.  I use much of the same
class names and method names as he did as well as basically the same driver to test it.  I use ArrayLists
to manipulate City objects in my program.  It takes about 1 minute to go through a tour of 100 cities.  Hitting
the X button on the java window closes the program.

Files: Tour.java, Tour.class, City.java, City.class, Map.java, Map.class, Test.java, Test.class

How to run program: 

I'm providing all the class files needed to run the program but if you want to recompile
my code you need to compile Tour.java, City.java, Map.java, and Test.java to run my test file.  When compiling 
Map.java the compiler will say that you are using "unchecked or unsafe operations" but it will still generate 
a Map.class file and run.

Problems:  

My algorithm does not manage to provide the best tour possible.  It uncrosses many of the lines in
the tour but it isn't perfect.  With a small amount of cities (10-15) it does work much better and many times 
produces the best Tour route.  But with a large number of cities like 100 it leaves around 5-10 crosses in the tour
route.  Otherwise the rest of the code works well.
