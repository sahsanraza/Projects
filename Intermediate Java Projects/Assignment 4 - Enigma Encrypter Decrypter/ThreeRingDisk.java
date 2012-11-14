import java.util.*;
import java.lang.*;   //for toString() method

/*This data type is meant to represent a triple rotor coding device.
It is meant to be used with the Enigma machine to let users encrypt and
decrypt messages. It receives messages and can encrypt them into cipher
text or decrypt them back into plain text.  After a character is encrypted
or decrypted the inner rotor rotates clockwise once.  Once the inner rotor has
rotated a full revolution, the middle rotor rotates once.  The three rotors are
represented by ArrayLists.  Only the creator can change the outer rotor.
The user can change the middler and inner rotors, but all the rotors
must include the same characters and be the same length.*/

//by Ben Isenberg



public class ThreeRingDisk {

	//The three rotors are represented as ArrayLists
	private ArrayList<String> outRotor = new ArrayList<String>();

	//The middle and inner rotors are given an initial capacity
	private ArrayList<String> midRotor = new ArrayList<String>(10);

	private ArrayList<String> inRotor = new ArrayList<String>(10);


	//This variable keeps track of the inner rotor's rotations
	//It is used to tell the middle rotor when to rotate
	private int count = 0;



	//only contructor available to a client
	//Default constructor
	public ThreeRingDisk() {

		//Hard coded outer rotor from the assignment sheet
		this("#BDFHJLNPRTVXZACEGIKMOQSUWY");


	}

	//can only be called by the creator
	//Constructor with String parameter
	private ThreeRingDisk(String outr) {

		//Loops through the String argument given in the method call
		for (int index = 0; index < outr.length(); index++) {

			/*Reads each character in the String and converts
			each read character back into a string so it can be
			placed in the rotor*/
			String s1 = Character.toString(outr.charAt(index));

			outRotor.add(s1);

		}


	}

	//Method sets inner rotor
	public void setInner(String innr) {

		//Loops through the String argument given in the method call
		for (int index = 0; index < innr.length(); index++) {

			/*Reads each character in the String and converts
			each read character back into a string so it can be
			placed in the rotor*/
			String s1 = Character.toString(innr.charAt(index));

			inRotor.add(index, s1);
		}



	}

	//Method sets middle rotor
	public void setMiddle(String midl) {

		//Loops through the String argument given in the method call
		for (int index = 0; index < midl.length(); index++) {

			/*Reads each character in the String and converts
			each read character back into a string so it can be
			placed in the rotor*/
			String s1 = Character.toString(midl.charAt(index));

			midRotor.add(index, s1);
		}



	}

	//Method sets outer rotor
	//only creator can set the outer rotor
	private void setOuter(String outr) {

		//Loops through the String argument given in the method call
		for (int index = 0; index < outr.length(); index++) {

			/*Reads each character in the String and converts
			each read character back into a string so it can be
			placed in the rotor*/
			String s1 = Character.toString(outr.charAt(index));

			outRotor.add(s1);
		}


	}

	//Method used to erase the contents in the inner and middle rotor
	//Needed so users can change the rotors through the Enigma machine
	public void eraseRotors() {

		//Resets count so rotors dont rotate early
		count = 0;

		//Erases contents in rotors and trims their capacity
		inRotor.clear();
		inRotor.trimToSize();

		midRotor.clear();
		midRotor.trimToSize();

	}

	//manages rotating the inner and middle rotor
	public void rotate() {

		//Holds first element in inner rotor
		String firstIn = inRotor.get(0);
		//Holds last element in inner rotor
		String last = inRotor.get(inRotor.size() - 1);
		//Holds first element in middle rotor
		String firstMid = midRotor.get(0);

		//Last element in inner rotor is placed at the beginning of rotor
		inRotor.add(0, last);
		//Last element in inner rotor is removed since its now at the beginning
		inRotor.remove(inRotor.size() - 1);
		//count is incremented by one; represents one rotation of the inner rotor
		count = count + 1;


		//if count equals the size of the inner rotor, meaning a full rotation has occurred,
		//then the last element of the middle rotor is placed as the beginning of the middle rotor
		//then the last element is deleted and count returns back to zero
		if (count == inRotor.size()) {

			//Variables used to hold first and last element of middle rotor
			String f = midRotor.get(0);
			String l = midRotor.get(midRotor.size() - 1);

			midRotor.add(0, l);

			midRotor.remove(midRotor.size() - 1);

			count = 0;

		}



	}


	//Method used to encrypt the character c argument used when the method is called
	public char encrypt(char c) {

		//String variable used to convert the character c to a string so it can be found in the rotors
		String s2 = Character.toString(c);

		//Sequential Search Algorithm is used here to find s2 in the inner rotor and the index its at
		int index = 0;
		int element = -1;
		boolean found = false;

		while (!found && index < inRotor.size()) {

			if (inRotor.get(index).equals(s2)) {
				found = true;
				element = index;

			}

			index++;
		}

		//s2 is now the element in the outer rotor corresponding to the index found in the inner rotor
		s2 = outRotor.get(element);

		//Sequential Search Algorithm is used again to now find s2 in the middle rotor and its index
		//Variables are reset
		index = 0;
		element = -1;
		found = false;

		while (!found && index < midRotor.size()) {
			if (midRotor.get(index).equals(s2)) {
				found = true;
				element = index;
			}
			index++;
		}

		//s2 is now the element in the outer rotor corresponding to the index found in the middle rotor
		s2 = outRotor.get(element);

		//s2 is converted back into a character so it can be returned
		char en = s2.charAt(0);

		//rotors rotate after every encryption
		rotate();


		return en;

	}

	//Method used to decrypt the character c argument used when the method is called
	//Reverses the process in the encrypt method
	public char decrypt(char c) {

		//String variable used to convert the character c to a string so it can be found in the rotors
		String s2 = Character.toString(c);

		//Sequential Search Algorithm is used here to find s2 in the outer rotor and the index its at
		int index = 0;
		int element = -1;
		boolean found = false;

		while (!found && index < outRotor.size()) {

			if (outRotor.get(index).equals(s2)) {
				found = true;
				element = index;

			}

			index++;
		}

		//s2 is now the element in the middle rotor corresponding to the index found in the outer rotor
		s2 = midRotor.get(element);

		//Sequential Search Algorithm is used again to now find s2 in the outer rotor and its index
		//Variables are reset
		index = 0;
		element = -1;
		found = false;

		while (!found && index < outRotor.size()) {
			if (outRotor.get(index).equals(s2)) {
				found = true;
				element = index;
			}
			index++;
		}

		//s2 is now the element in the inner rotor corresponding to the index found in the outer rotor
		s2 = inRotor.get(element);

		//rotors rotate after every encryption
		rotate();

		//s2 is converted back into a character so it can be returned
		char en = s2.charAt(0);

		return en;

	}

	//Method used to display outer, middle, and inner rotors using their toString method
	public String toString() {

		String s = outRotor.toString() + "\n" + midRotor.toString() + "\n" + inRotor.toString();

		return s;


	}





}
