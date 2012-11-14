///Name: Ben Isenberg
///Date: December 2, 2010
///Email: bji6@pitt.edu

/*This class represents a Binary Search Tree.  I implemented it by using nodes that reference
each other through their children.  It has one data field that represents the root of the tree.*/


public class BinarySearchTree {

	private Node root;

	//Constructor
	public BinarySearchTree(){
		root = new Node();
	}

	//Method returns the root node of the tree
	public Node getRoot(){
		return root;
	}

	//Method adds a new node to the Binary Search Tree
	public void add(String key, Object value){

		if(root.key == null){
			root = new Node(key, value, null, null);
		}
		else{

			Node n = new Node(key, value, null, null);

			add(root, n);

		}


	}

	//Overridden add method that traverses the tree to make sure it adds a node to the right location
	public void add(Node a, Node b){

		int comparison = a.key.compareTo(b.key);

		//if a > b
		if(comparison > 0){

			if(a.getLeft() == null){
				a.setLeft(b);
			}
			else{
				add(a.getLeft(), b);
			}

		}

		//if b > a
		if(comparison < 0){

			if(a.getRight() == null){
				a.setRight(b);
			}
			else{
				add(a.getRight(), b);
			}

		}

	}


	//Method returns the contents of a node with the same key as the one passed in
	public Object getValue(String key){

		Node x = root;

		while(x != null){

			if(x.key.equals(key)){
				return x.contents;
			}

			int comparison = x.key.compareTo(key);

			if(comparison > 0){
				x = x.getLeft();
				continue;
			}

			if(comparison < 0){
				x = x.getRight();
				continue;
			}

		}

		//Returns null because no node in the tree has the specified key
		return null;

	}



}