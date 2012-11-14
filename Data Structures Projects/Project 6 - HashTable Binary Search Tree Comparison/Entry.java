///Name: Ben Isenberg
///Date: December 2, 2010
///Email: bji6@pitt.edu

/*This class represents an entry into a Hash Table.  It has two data fields:
a key and a value.  I created this class to solve the problem that arises when
two items hash to the same spot in the array.  During retrieval I just compare
the key of the entry in the array to the key passed into the method to find out
if they are actually the same or just have the same code.*/


public class Entry{


	private String key;
	private Object value;

	//Constructor
	public Entry(String k, Object v){

		key = k;
		value = v;

	}

	//Method returns entry's key
	public String getKey(){
		return this.key;
	}

	//Method returns entry's value
	public Object getValue(){
		return this.value;
	}


}