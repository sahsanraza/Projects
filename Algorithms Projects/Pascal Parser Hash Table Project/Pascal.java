import java.util.*;
import java.io.*;

//NAME: Ben Isenberg
//EMAIL: bji6@pitt.edu
//DATE: June 10, 2011


public class Pascal{


	public static void main(String[] args) throws IOException{

		DoubleHashST t1 = new DoubleHashST(5);
		DoubleHashST t2 = new DoubleHashST(11);
		DoubleHashST t3 = new DoubleHashST(7);


		//add all reserved words
		t1.add("and");
		t1.add("begin");
		t1.add("case");
		t1.add("div");
		t1.add("do");
		t1.add("else");
		t1.add("end");
		t1.add("for");
		t1.add("if");
		t1.add("mod");
		t1.add("not");
		t1.add("of");
		t1.add("or");
		t1.add("program");
		t1.add("repeat");
		t1.add("then");
		t1.add("to");
		t1.add("until");
		t1.add("var");
		t1.add("while");

		//add 4 predefined types
		t2.add("integer");
		t2.add("char");
		t2.add("boolean");
		t2.add("real");


		//add other predefined types
		t3.add("false");
		t3.add("true");
		t3.add("read");
		t3.add("readln");
		t3.add("write");
		t3.add("writeln");
		t3.add("sqr");
		t3.add("sqrt");
		t3.add("round");
		t3.add("trunc");
		t3.add("input");
		t3.add("output");


		//Use scanner to read file name specified by the user
		Scanner keyboard = new Scanner(System.in);

		String fileName;
		String l = "";
		StringBuilder string = new StringBuilder("");

		int lineNum = 0;

		ArrayList<Integer> numberlist = new ArrayList<Integer>();

		ArrayList<String> variables = new ArrayList<String>();
		ArrayList<String> types = new ArrayList<String>();


		fileName = args[0];

		File file = new File(fileName);

		Scanner inputFile = new Scanner(file).useDelimiter("\\s");

		//Find in file where variable declarations begin
		while(inputFile.hasNextLine()){

			l = inputFile.next();


			if(l.equals("var")){
				break;
			}
		}





		//parse variable names and types from code to make sure they are valid
		while(inputFile.next().equals("begin") != true){

			l = inputFile.nextLine();

			string = new StringBuilder("");

			for(int d = 0; d < l.length(); d++){


				if(l.charAt(d) != ' '){

					//skip numbers
					if((int)l.charAt(d) > 47 && (int)l.charAt(d) < 58){
						d++;
					}

					string.append(l.charAt(d));

					if(string.charAt(string.length()-1) == ':'){
						string.deleteCharAt(string.length()-1);
						variables.add(string.toString());


						if(d+1 != l.length() && l.charAt(d+1) == ' '){


							do{
								d++;
							}while(d < l.length() && l.charAt(d) == ' ');
							d--;
							string = new StringBuilder("");
						}

					}
					else{
						if(string.charAt(string.length()-1) == ';'){
							string.deleteCharAt(string.length()-1);
							types.add(string.toString());

							if(d+1 != l.length() && l.charAt(d+1) == ' '){

								do{
									d++;
								}while(d < l.length() && l.charAt(d) == ' ');
								d--;
								string = new StringBuilder("");
							}

						}
					}
				}



			}



		}


		int next = 0;

		//add valid variables to t3
		while(next != variables.size()){

			if(t2.contains(types.get(next))){

				if(t1.contains(variables.get(next)) == false && t2.contains(variables.get(next)) == false
					&& t3.contains(variables.get(next)) == false )
				{
					t3.add(variables.get(next));
				}

				next++;
			}
			else{
				next++;
			}

		}


		int counter = 0;

		StringBuilder str = new StringBuilder("");
		String[] list = new String[5000];
		ArrayList<String> errors = new ArrayList<String>();

		int x = 0;
		l = inputFile.nextLine();


		//parse all identifiers from code, replace all symbols with spaces
		while(inputFile.hasNextLine()){

			str = new StringBuilder("");

			counter++;

			if(x >= l.length()){
				l = inputFile.nextLine();

			}

			for( x = 0; x < l.length(); x++){



				//ignore single quote strings
				if((int)l.charAt(x) == 39){

					do{

						if( x >= l.length()-1){
							break;
						}
						x++;
					}while((int)l.charAt(x) != 39);
				}


				//ignore symbols
				if( ((int)l.charAt(x) < 65) || ((int)l.charAt(x) > 122)){
					str.append(' ');

					continue;
				}



				if((int)l.charAt(x) > 90 && (int)l.charAt(x) < 97){

					str.append(' ');
					continue;
				}

				str.append(l.charAt(x));



			}

			list[counter] = str.toString();


		}

		str = new StringBuilder("");

		//go thru strings and extract identifiers
		for(int k = 0; k < list.length; k++){

			if(list[k] == null){
				continue;
			}

			for(int h = 0; h < list[k].length(); h++){


				if(list[k].charAt(h) != ' '){

					str.append(list[k].charAt(h));

					if(h+1 != list[k].length() && list[k].charAt(h+1) == ' '){
						errors.add(str.toString());

						do{
							h++;
						}while(h < list[k].length() && list[k].charAt(h) == ' ');
						h--;
						str = new StringBuilder("");
					}

				}


			}
			if(str.toString().length() != 0){

				errors.add(str.toString());
			}
			str = new StringBuilder("");


		}




		file = new File(fileName);

		inputFile = new Scanner(file).useDelimiter("\\s");

		//skip to code section of file
		while(inputFile.hasNextLine()){

			l = inputFile.nextLine();
			lineNum++;

			if(l.contains("begin")){
				break;
			}
		}

		String previous = "";
		int prev_num = 0;

		StringTokenizer token = new StringTokenizer("");

		PrintWriter outputFile;

		string = new StringBuilder(fileName);
		string.deleteCharAt(string.length()-1);
		string.deleteCharAt(string.length()-1);
		string.append(".txt");
		fileName = string.toString();

		outputFile = new PrintWriter(fileName);

		//print out where errors are in code
		while(inputFile.hasNext()){

			l = inputFile.nextLine();

			token = new StringTokenizer(l);

			lineNum++;

			while(token.hasMoreTokens()){
				l = token.nextToken();

			for(int k = 0; k < errors.size(); k++){

				if(errors.get(k) == null || (errors.get(k).equals(previous) && prev_num == lineNum)){
					continue;
				}

				//ignore single quote strings
				if(l.contains("'") && token.hasMoreTokens()){
					l = token.nextToken("'");

					if(token.hasMoreTokens()){
						l = token.nextToken();
					}
				}



				if(t1.contains(errors.get(k)) == false && t3.contains(errors.get(k)) == false && l.contains(errors.get(k))){
					previous = errors.get(k);
					prev_num = lineNum;
					//System.out.println("Undeclared identifier " + errors.get(k) + " found on line " + lineNum);
					outputFile.println("Undeclared identifier " + errors.get(k) + " found on line " + lineNum);
				}


			}

			}

		}


		outputFile.close();






	}










}