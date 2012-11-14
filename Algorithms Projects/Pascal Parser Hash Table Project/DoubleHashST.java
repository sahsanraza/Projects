import java.util.*;

//NAME: Ben Isenberg
//EMAIL: bji6@pitt.edu
//DATE: June 10, 2011



public class DoubleHashST<Key, Value>{


	private static final int INIT_CAPACITY = 4;
	Random R = new Random();


	private int size;           // size of linear probing table
    public String[] vals;    // the values



	// create an empty hash table - use 16 as default size
	public DoubleHashST() {
	      this(INIT_CAPACITY);
	}

	 // create linear proving hash table of given capacity
	 public DoubleHashST(int capacity) {
	        size = capacity;

			//check if M is prime, makes it prime if not
	        size = primeCheck(size);

	        vals = new String[size];

     }

	//Function checks to see if m is prime and if it isnt it finds the next prime number after it
     public int primeCheck(int m){

		 int n = m;
		int i;

		 boolean check = true;

		//While prime number hasnt been found
		 while(check){


			//For all numbers up to square root of n
		 	for(i = 2; i < n; i++){

				//if i divides evenly into n its not prime so increment n and restart process
				 if((n % i) == 0){
					 n++;
					 break;
				 }

			 }

			if(i == n){
				check = false;
			}

		 }

		 return n;

	 }




    // return the number of key-value pairs in the symbol table
	    public int size() {
	        return vals.length;
	    }


	 // is the symbol table empty?
    public boolean isEmpty() {
        return size() == 0;
    }

	public boolean contains(String value) {

	    for(int x = 0; x < vals.length; x++){
			if(vals[x] != null && vals[x].equals(value)){
				return true;
			}
		}
		return false;
	}

	// hash function for keys - returns value between 0 and size-1
	private int hash(String value) {
	   return (value.hashCode() & 0x7fffffff) % size;
	}


	//Method retrieves an Object that has the same key as the parameter key
			public String get(String value){

				int index = hash(value);

				if(index >= vals.length){
					index = index + doubleHash(index, value);
					if(index >= size){
						index = index - size;
					}
					if(index >= size){
						return null;
					}
				}

				boolean checker = true;

				//Accounts for linear probing during retrieval so it compares keys
				while(checker && vals[index] != null){

					//Cast Object back into Entry
					if(vals[index].equals(value)){

						return vals[index];
					}
					else{
						//check double hashed value
						index = index + doubleHash(index, value);
					}


					//Starts searching back at the begining of the array if it reaches the end and hasn't found the item
					if(index >= size){
						index = index - size;
					}

				}

				//returns null
				return vals[index];

	}






	    // resize the hash table to the given capacity by re-hashing all of the keys
	    private void resize(int capacity) {

	        //make sure the new table size is a prime number
	        capacity = primeCheck(capacity);
	        System.out.println("Resizing Hash Table to size " + capacity);


	        DoubleHashST temp = new DoubleHashST(capacity);


	        for (int i = 0; i < size; i++) {
	            if (vals[i] != null) {
	                temp.add(vals[i]);
	            }
	        }

	        vals = temp.vals;
	        size    = temp.size;
	    }

		//Add function
	    public void add(String value){

				double r = vals.length;
				double j = 0.0;

				for(int e = 0; e < vals.length; e++){
					if(vals[e] != null){
						j++;
					}
				}

				if(j/r >= 0.8){
					resize(2*size);
				}

				int i = hash(value);

				//if the spot the key hashes to is empty enter the item there
				if(vals[i] == null){
					vals[i] =  value;
					return;
				}

				int x = i;

				//Linear probing to take care of more than one item hashing to the same spot
				//I search in order for the next available spot in the hash table
				while(vals[x] != null){

					//double hash
					x = x + doubleHash(i, value);
					//Start back at the beginning of the array once it reaches the end
					if(x >= size){
						x = x - size;
					}

				}

				vals[x] = value;


				return;

	}

	//Double hash function based on random int
	private int doubleHash(int i, String value){

		R.setSeed(i);

		int hash = R.nextInt(size-1) + 1;  // Get h2;


		return hash;

	}





}