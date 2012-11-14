///Name: Ben Isenberg
///Date: December 2, 2010
///Email: bji6@pitt.edu

/*This class represents a node in a binary search tree.  Each node has a key,
a value, and refrences to left and right children.*/


public class Node{

	String key;
	Object contents;
	Node left, right;


	//No-arg constructor
	public Node(){
		this.key = null;
	}

	//Constructor
	public Node(String key, Object value, Node l, Node r){
		this.key = key;
		this.contents = value;
		this.left = l;
		this.right = r;
	}

	//Method that assigns node l as a left child of the calling node
	public void setLeft(Node l){
		left = l;
	}

	//Method that assigns node r as a right child of the calling node
	public void setRight(Node r){
		right = r;
	}

	//Returns calling node's left child
	public Node getLeft(){
		return this.left;
	}

	//Returns calling node's right child
	public Node getRight(){
		return this.right;
	}


}