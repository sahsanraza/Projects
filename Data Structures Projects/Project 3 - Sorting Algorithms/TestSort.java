///Name: Ben Isenberg
///Date: October 20, 2010
///Email: bji6@pitt.edu


/*This is a small test file that tests my slection sort, merge sort, and quick sort
methods on an unorganized array of Strings*/




public class TestSort {


	public static void main(String[] args){

		String[] a = new String[5];


		System.out.println("SELECTION SORT");
		System.out.println("Unsorted Array: ");

		a[0] = "A";
		a[1] = "r";
		a[2] = "k";
		a[3] = "z";
		a[4] = "F";

		for(int x = 0; x < a.length; x++){
			System.out.print(a[x] + " ");
		}

		//Selection Sort
		SortingMethods.selectionSort(a);

		System.out.println();
		System.out.println("Array After Sorting:");

		for(int x = 0; x < a.length; x++){
			System.out.print(a[x] + " ");
		}

		String[] b = new String[5];


		System.out.println("\n\nMERGE SORT");
		System.out.println("Unsorted Array: ");

		b[0] = "A";
		b[1] = "r";
		b[2] = "k";
		b[3] = "z";
		b[4] = "F";

		for(int x = 0; x < b.length; x++){
			System.out.print(b[x] + " ");
		}

		//Merge Sort
		SortingMethods.mergeSort(b);

		System.out.println();
		System.out.println("Array After Sorting:");

		for(int x = 0; x < b.length; x++){
			System.out.print(b[x] + " ");
		}

		String[] c = new String[5];


		System.out.println("\n\nQUICK SORT");
		System.out.println("Unsorted Array: ");

		c[0] = "A";
		c[1] = "r";
		c[2] = "k";
		c[3] = "z";
		c[4] = "F";

		for(int x = 0; x < c.length; x++){
			System.out.print(c[x] + " ");
		}

		//Quick Sort
		SortingMethods.quickSort(c);

		System.out.println();
		System.out.println("Array After Sorting:");

		for(int x = 0; x < c.length; x++){
			System.out.print(c[x] + " ");
		}




	}







}