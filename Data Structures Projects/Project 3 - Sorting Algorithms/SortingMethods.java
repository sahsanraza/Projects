///Name: Ben Isenberg
///Date: October 20, 2010
///Email: bji6@pitt.edu


/*This file includes my code for a selection sort method, merge sort method, and quick sort
method. They sort a array of objects. They all work with any object that implements the
Comparable interface.*/



import java.util.*;




public class SortingMethods {




	//Selection sort
	public static <T extends Comparable<T>> void selectionSort(T[] A){
		int min;

		for(int i = 0; i < A.length; i++){

			//Min starts out as the first element in the array
			min = i;

			for(int j = i; j < A.length; j++){

				//Scans array and if a smaller min is found that element is the new min
				if(A[j].compareTo(A[min]) < 0){

					min = j;

				}
				else{
				}

			}

			//Swaps new found min value with the element that was previously thought to be min
			swap(A, i, min);

		}

	}

	//Swapping method for selection sort
	private static <T extends Comparable<T>>  void swap(T[] data, int i, int min) {

	    T temp ;
	    temp = data[i];

	  	data[i] = data[min];
	    data[min] = temp;


	}




	//MergeSort method
	public static <T extends Comparable<T>> void mergeSort(T[] A){

		//Ends recursion when only one element is left in each half of the array
		if(A.length <= 1){

			return;
		}

		//Finds middle to divide arrays
		int middle = A.length/2;

		//Two new arrays are made that are copies of halves of original array
		Comparable[] left = new Comparable[middle];
		Comparable[] right = new Comparable[A.length - middle];

		//Copy over all elements from left half of original array into new Left array
		for(int x = 0; x < middle; x++){
			left[x] = A[x];

		}

		//Needed for following for loop
		int index = 0;

		//Copies over all elements from right half of original array into new Right array
		for(int x = middle; x < A.length; x++){
			right[index] = A[x];

			index++;
		}


		//Recursively continue dividing arrays until base case is reached
		mergeSort(left);
		mergeSort(right);

		//Begin merging arrays until original array is completely sorted
		merge(left, right, A);



		return;


	}



	public static <T extends Comparable<T>> void merge(T[] left, T[] right, T[] A){

		int indexArray = 0;
		int indexLeft = 0;
		int indexRight = 0;

		//Keep merging while arrays left and right have elements in them
		while(left.length != indexLeft && right.length != indexRight){

			//Place element in left array in new array if its left than element in right array
			if(left[indexLeft].compareTo(right[indexRight]) < 0){

				A[indexArray] = left[indexLeft];

				indexLeft++;
				indexArray++;

			}
			//Otherwise the element in the right array goes in first
			else{


				A[indexArray] = right[indexRight];

				indexRight++;
				indexArray++;

			}


		}

		//If all the elements from the left array have been merged, put the rest of the right array into the new array
		if(indexLeft == left.length){

			for(int x = indexRight; x < right.length; x++){
				A[indexArray] = right[x];

				indexArray++;

			}

		}

		//If all the elements from the right array have been merged, put the rest of the left side into the new array
		if(indexRight == right.length){

			for(int x = indexLeft; x < left.length; x++){
				A[indexArray] = left[x];

				indexArray++;

			}

		}



	}


	//Overloaded quickSort
	public static <T extends Comparable<T>> void quickSort(T[] array){
		int left = 0;

		int right = (array.length - 1);

		quickSort(array, left, right);

	}


	//QuickSort
	public static <T extends Comparable<T>> void quickSort(T[] array, int left, int right){


		//While the whole array hasnt been sorted keep the recursion going
		if(right > left){

			int pivotIndex = right;

			//Final location of pivot is found by calling partion
			int newPivotIndex = partition(array, left, right, pivotIndex);

			//QuickSort is called again to recursively partition the left and right sides until every element is sorted
			quickSort(array, left, newPivotIndex-1);
			quickSort(array, newPivotIndex +1, right);

		}


	}

	//Partitions
	public static <T extends Comparable<T>> int partition(T[] array, int left, int right, int pivotIndex){

		int smaller = left;

		T i;

		//Goes through array and puts all values less than or equal to pivot left of smaller
		for(int x = left; x < right; x++){

			i = array[x];

			if(i.compareTo(array[pivotIndex]) < 0){

				T temp = array[smaller];

				array[smaller] = i;

				array[x] = temp;
				smaller++;

			}

		}

		//Swaps pivot and smaller so that pivot is now in final location
		T temp = array[smaller];
		array[smaller] = array[pivotIndex];
		array[pivotIndex] = temp;

		//returns final location of pivot
		int newPivotIndex = smaller;

		return newPivotIndex;

	}






}
















