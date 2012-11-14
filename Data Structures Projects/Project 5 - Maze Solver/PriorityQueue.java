///Name: Ben Isenberg
///Date: November 18, 2010
///Email: bji6@pitt.edu

/*This class represents a priority queue.  I use a min value heap to implement the priority queue.
The item in the queue with the smallest distance to the goal of the maze is the one that is placed
at the top of the heap for removal.  I use an ArrayList to implement a heap.*/


import java.util.*;



public class PriorityQueue {

	private ArrayList<Point> heapArray;

	private int goalRowNum;

	private int goalColNum;

	//Constructor
	public PriorityQueue(){

		heapArray = new ArrayList<Point>();

	}

	//Keeps track of where the goal is in the maze
	public void setGoalNum(int r, int c){
		goalRowNum = r;
		goalColNum = c;
	}


	//Method inserts p into the heap properly
	public void insert(Point p){

		heapArray.add(p);

		Point parent = getParent(heapArray.indexOf(p));

		while(parent != null && parent.getManDistance() >= p.getManDistance()){

			swap(p, parent);
			parent = getParent(heapArray.indexOf(parent));

		}


	}

	//Method swaps item a with item b
	public void swap(Point a, Point b){
		int index1 = heapArray.indexOf(a);
		int index2 = heapArray.indexOf(b);

		heapArray.set(index1, b);
		heapArray.set(index2, a);

	}

	//Returns parent of item at index n
	public Point getParent(int n){
		int num = (n-1)/2;

		//if item at n is the root return null because root doesnt have a parent
		if(n == 0){

			return null;
		}

		return heapArray.get(num);

	}

	//Removes the item in the heap with the smallest distance to the goal
	//Heapifies the list after the removal so every item is in its proper place again
	public Point removeWithPriority(){

		if(isEmpty() == false){

			//Swaps root item with the bottom right item in the heap
			Point lastItem = heapArray.get(heapArray.size() - 1);

			swap(heapArray.get(0), heapArray.get(heapArray.size() - 1));

			//Removes root from heap
			Point root = heapArray.remove(heapArray.size() - 1);


			Point firstChild = new Point();

			firstChild = getChild(heapArray.indexOf(lastItem));

			Point secondChild = new Point();

			secondChild = getSecondChild(heapArray.indexOf(lastItem));

			//Percolates bottom right item of heap into proper position
			while(firstChild != null){

				if(secondChild == null && lastItem.getManDistance() > firstChild.getManDistance()){
					swap(lastItem, firstChild);
					break;
				}

				if(secondChild == null && lastItem.getManDistance() <= firstChild.getManDistance()){
					break;
				}



				//Checks to make sure no child is greater than parent during removal process

				if(firstChild.getManDistance() < secondChild.getManDistance() && lastItem.getManDistance() > firstChild.getManDistance()){

					swap(lastItem, firstChild);
				}
				else{
					if(secondChild.getManDistance() <= firstChild.getManDistance() && lastItem.getManDistance() > secondChild.getManDistance()){

						swap(lastItem, secondChild);
					}
					else{
						if(lastItem.getManDistance() <= firstChild.getManDistance() && lastItem.getManDistance() <= secondChild.getManDistance()){

							break;
						}

					}
				}


				firstChild = getChild(heapArray.indexOf(lastItem));

				secondChild = getSecondChild(heapArray.indexOf(lastItem));


			}

			return root;
		}

		//returns null when the heap is empty
		return null;


	}

	//Method that finds the first child of the point at index n
	public Point getChild(int n){

		int num = (2*n) + 1;

		//Returns null if child doesn't exist
		if(num > heapArray.size() - 1 || num < 0){

			return null;
		}

		return heapArray.get(num);

	}

	//Method finds the second child of the point at index n
	public Point getSecondChild(int n){

		int num = (2*n) + 2;

		//Returns null if child doesn't exist
		if(num > heapArray.size() - 1 || num < 0){

			return null;
		}

		return heapArray.get(num);

	}


	//Checks if the heap is empty
	public boolean isEmpty(){

		if(heapArray.isEmpty()){
			return true;
		}

		return false;
	}

	//Displays items in heap
	public void display(){

		for(int x = 0; x < heapArray.size(); x++){
			System.out.println(heapArray.get(x).getManDistance());
		}

	}









}