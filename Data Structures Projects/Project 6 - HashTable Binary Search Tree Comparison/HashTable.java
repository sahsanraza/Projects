///Name: Ben Isenberg
///Date: December 2, 2010
///Email: bji6@pitt.edu

/*This class represents a Hash Table.  I use an array to actually contain items.
I use the String class's hashCode() method to hash keys and then I use my method
hashIndex to find the right spot to place an item in the array.*/


public class HashTable {


	public Object[] list;
	private int size;


	//Constructor
	public HashTable(int capacity){

		//Makes an array based on the size of the capacity
		size = capacity;

		list = new Object[size];

	}

	//Method that takes the hash code of the key and finds the right index in the array to place the item
	public int hashIndex(int hashCode){

		//Index is the remainder left by dividing hashCode by the size of the array
		int index = hashCode % size;

		//To prevent a negative index I add size to it if it's less than zero
		if(index < 0){
			return (index + size);
		}

		return index;

	}

	//Method retrieves an Object that has the same key as the parameter key
	public Object getValue(String key){

		int i = key.hashCode();

		int index = hashIndex(i);

		boolean checker = true;

		//Accounts for linear probing during retrieval so it compares keys
		while(checker && list[index] != null){

			//Cast Object back into Entry
			if( ((Entry)list[index]).getKey().equals(key)){

				return ((Entry)list[index]).getValue();
			}

			index++;

			//Starts searching back at the begining of the array if it reaches the end and hasn't found the item
			if(index == size){
				index = 0;
			}

		}

		//returns null
		return list[index];

	}


	//Method adds an entry to hash table with the key and value passed in
	public void add(String key, Object value){


		Entry e = new Entry(key, value);

		int i = hashIndex(e.getKey().hashCode());

		//if the spot the key hashes to is empty enter the item there
		if(list[i] == null){
			list[i] = e;
			return;
		}

		int x = i;

		//Linear probing to take care of more than one item hashing to the same spot
		//I search in order for the next available spot in the hash table
		while(list[x] != null){

			x++;

			//Start back at the beginning of the array once it reaches the end
			if(x == size){
				x = 0;
			}

		}

		list[x] = e;

		return;
	}




}