//NAME: Ben Isenberg
//DATE: 03/18/12
//EMAIL: bji6@pitt.edu

/*This java class includes an implementation of a page table and 3 page replacement algorithms: Opt, Clock, and NRU*/

import java.util.*;
import java.io.*;


public class VMsim{

	//Inner class
	static class Table_Entry{
		//Dirty bit
		int d_bit = 0;
		//Reference bit
		int r_bit = 0;
		//Valid bit
		int v_bit = 0;
		//Frame number
		int frame_num = -1;
		//Page's hex id 
		String hex_id;

		//Default constuctor
		public Table_Entry(){}
		
		//Constructor used to copy table entry objects
		public Table_Entry(Table_Entry copy){
			this.d_bit = copy.d_bit;
			this.r_bit = copy.r_bit;
			this.v_bit = copy.v_bit;
			this.frame_num = copy.frame_num;
			this.hex_id = copy.hex_id;
		}
	}

	//Function initializes the clock data structure
	public LinkedList<Table_Entry> init_clock(){

		LinkedList<Table_Entry> clock = new LinkedList<Table_Entry>();

		return clock;
	}

	//page table
	public Hashtable<String, Table_Entry> page_table;
		

	public static void main(String[] args) throws IOException{

		VMsim test = new VMsim();
		//Circular queue used with CLOCK algorithm
		LinkedList<Table_Entry> clock = null;
		//Used to keep track of runtime
		Date date = new Date();
		System.out.println(date.toString());
		//Data structure used for OPT algorithm
		Hashtable<String, LinkedList<Integer>> opt_struct = new Hashtable<String, LinkedList<Integer>>();

		//Number of frames (entered at command line)
		int num_frames = 0;
		//page replacement algorithm (entered at command line)
		String alg;
		//time interval for nru to refresh (entered at command line)
		int nru_refresh = 0;

		if(args.length < 5 || args.length > 7){
			System.out.println("Error incorrect number of arguments");
			return;
		}

		num_frames = Integer.parseInt(args[1]);

		//Have a list of all currently valid hex ids so a search isn't required through the page table
		String[] valid_list = new String[num_frames];

		String filename = args[args.length-1];
		
		//System.out.println(num_frames);

		alg = args[3];

		//Read trace file
		Scanner fileScan = new Scanner(new FileInputStream(filename));

		//For 32-bit address space and 4KB page size you need 2^(20) page table entries
		int table_size = (int)java.lang.Math.pow(2, 20);

		//Create page table object
		test.page_table = new Hashtable<String, Table_Entry>();

		//Fill page table with entries
		for(int x = 0; x < table_size; x++){
			Table_Entry e = new Table_Entry();
			String id = Integer.toHexString(x);
			int padding = 5 - id.length();
			e.hex_id = id;
			for(int f = 0; f < padding; f++){
				e.hex_id = "0" + e.hex_id;
			}
			//if using OPT create linked list objects for every page in hash table
			if(alg.equals("opt")){	
				opt_struct.put(e.hex_id, new LinkedList<Integer>());
			}
			//System.out.println(e.hex_id);
			test.page_table.put(e.hex_id, e);
		}

		//System.out.println(test.page_table.size());

		//Initialize clock; should be nothing in it at this point
		if(alg.equals("clock")){
			clock = test.init_clock();
		}

		//Get nru refresh rate from command line if thats whats being used
		if(alg.equals("nru")){
			nru_refresh = Integer.parseInt(args[5]);
		}

		//Counter
		int ctr = 0;

		//Read through trace file once to fill up opt data structure with all future references for every page
		if(alg.equals("opt")){
			Scanner firstScan = new Scanner(new FileInputStream(filename));
			while(firstScan.hasNextLine()){
				ctr++;
				String line = firstScan.nextLine();
				String page_num = line.substring(0, 5);

				LinkedList<Integer> ll = opt_struct.get(page_num);
				//Add line number to page's list
				ll.add(new Integer(ctr));
				//Place it back in hash table
				opt_struct.put(page_num, ll);
			}
			firstScan.close();
		}

		ctr = 0;
		//page fault counter
		int pf_ctr = 0;
		//write to disk counter
		int w_ctr = 0;
		//Keeps track of how many valid pages their are
		int valid_ctr = 0;
		
		
		//Read through trace file
		while(fileScan.hasNextLine()){
			ctr++;
			//if NRU, refresh when the provided number of memory references has occurred
			if(alg.equals("nru") && (ctr % nru_refresh == 0)){
				//Set all r bits to 0
				for(Enumeration<Table_Entry> enu = test.page_table.elements(); enu.hasMoreElements();){
					Table_Entry entry = enu.nextElement();
					entry.r_bit = 0;
					test.page_table.put(entry.hex_id, entry);
				}
			}
			//System.out.println(ctr);;

			String line = fileScan.nextLine();
			//Only look at first 5 hex values (last 3 are for offsets)
			String page_num = line.substring(0, 5);
			
			//System.out.println("Hex = " + page_num + " " + line);
			//System.out.println("Decimal = " + Integer.parseInt(page_num,16));

			//remove page read in to update its fields
			Table_Entry e = test.page_table.get(page_num);
			
			//page was referenced so set r bit
			e.r_bit = 1;

			//if you are doing a write then set d bit
			if(line.endsWith("W")){
				e.d_bit = 1;
				//System.out.println("Hex = " + page_num + " " + line);
			}

			//whenever you see a new memory reference you need to prune the opt data structure
			//so that that page's next reference is the first element in its list
			if(alg.equals("opt")){
				LinkedList<Integer> ll = opt_struct.get(page_num);
				//remove first line number in list because you are on that number now
				ll.removeFirst();
				//Place it back in hash table
				opt_struct.put(page_num, ll);
			}


			//If page is not in memory and their is an open frame then place it into memory
			//Compulsory misses
			if(e.v_bit == 0 && valid_ctr < num_frames){
				//System.out.println("page fault - Compulsory miss " + page_num);
				e.v_bit = 1;
				e.frame_num = valid_ctr;
				//Add to valid list
				valid_list[e.frame_num] = e.hex_id;
				valid_ctr++;
				test.page_table.put(e.hex_id, e);
				//For clock, add page to circular queue
				if(alg.equals("clock")){
					clock.add(e);
				}
				//date = new Date();
				//System.out.println(date.toString());
			}
			else {
				//OPT
				//If referenced page is already in RAM don't do anything
				if(e.v_bit == 1 && alg.equals("opt")){
					continue;
				}
				//OPT
				//Page fault = remove the valid page that is furthest in the future
				if(alg.equals("opt")){
					//System.out.println("Page fault!");
					//Keep track of  number of page faults
					pf_ctr++;
					String furthest_page = null;
					Integer next_reference = new Integer(0);
					//Search through all pages in RAM
					for(int g = 0; g < num_frames; g++){
						String next_valid_page = valid_list[g];
						//System.out.println(next_valid_page);
						//If a page has no more memory references in the future get rid of it cause its taking up space
						if(opt_struct.get(next_valid_page).size() == 0){
							furthest_page = next_valid_page;
							break;
						}
						//If current page is referenced farther into the future than previous pages, it is new best candidate to evict
						if(opt_struct.get(next_valid_page).getFirst() > next_reference){
							next_reference = opt_struct.get(next_valid_page).getFirst();
							furthest_page = next_valid_page;
							//System.out.println(furthest_page + " is new best with " + next_reference);
						}
					}
					//If no valid page has any more references in the future just evict the page in the first frame
					if(furthest_page == null){
						furthest_page = valid_list[0];
					}
					Table_Entry te = test.page_table.get(furthest_page);
					e.frame_num = te.frame_num;
					//Write to disk
					if(te.d_bit == 1){
						//System.out.println("Evicted dirty " + te.hex_id);
						w_ctr++;
					}
					else{
						//System.out.println("Evicted clean " + te.hex_id);
					}
					//Add to valid list
					valid_list[e.frame_num] = e.hex_id;
					te.v_bit = 0;
					te.d_bit = 0;
					te.r_bit = 0;
					test.page_table.put(te.hex_id, te);
					e.v_bit = 1;
					test.page_table.put(e.hex_id, e);
					continue;
				}
				//NRU
				//If referenced page is already in RAM don't do anything
				if(e.v_bit == 1 && alg.equals("nru")){
					continue;
				}
				//NRU
				//Evict best page
				if(alg.equals("nru")){
					//Keep track of  number of page faults
					pf_ctr++;
					 
					int done_flag = 0;
					Table_Entry second_option = null;

					while(done_flag == 0){
						//Look thru all valid pages and find best option
						for(int w = 0; w < valid_list.length; w++){
							
							Table_Entry te = test.page_table.get(valid_list[w]);
							//System.out.println(te.frame_num);
							//Evict this kind of page first if found
							if(te.r_bit == 0 && te.d_bit == 0 && te.v_bit == 1){
								//System.out.println("found best option");
								//System.out.println(te.frame_num);
								e.frame_num = te.frame_num;
								//Write to disk
								if(te.d_bit == 1){
									w_ctr++;
								}
								//Add to valid list
								valid_list[e.frame_num] = e.hex_id;
								te.v_bit = 0;
								te.d_bit = 0;
								te.r_bit = 0;
								test.page_table.put(te.hex_id, te);
								e.v_bit = 1;
								test.page_table.put(e.hex_id, e);
								//Evicted best page 
								done_flag = 1;
								break;
							}
							else{
								if(te.r_bit == 0 && te.d_bit == 1 && te.v_bit == 1){
									//2nd best choice; keep it in mind
									//System.out.println("found second best option");
									//System.out.println(te.frame_num);
									second_option = new Table_Entry(te);
									continue;
								}
								else{
									if(te.r_bit == 1 && te.d_bit == 0 && te.v_bit == 1 && second_option == null){
										//3rd best choice; keep it in mind
										//System.out.println("found third best option");
										//System.out.println(te.frame_num);
										second_option = new Table_Entry(te);
										continue;
									}
									else{
										if(te.r_bit == 1 && te.d_bit == 1 && te.v_bit == 1 && second_option == null){
											//Worst option; keep it in mind in case there's nothing else
											//System.out.println("found worst option");
											//System.out.println(te.frame_num);
											second_option = new Table_Entry(te);
											continue;
										}
									}
								}
							}
						
						}
						//Break out of loop if you have the best page
						if(done_flag == 1){
							continue;
						}
						e.frame_num = second_option.frame_num;
						//Write to disk
						if(second_option.d_bit == 1){
							w_ctr++;
						}
						//Add to valid list
						valid_list[e.frame_num] = e.hex_id;
						second_option.v_bit = 0;
						second_option.d_bit = 0;
						second_option.r_bit = 0;
						test.page_table.put(second_option.hex_id, second_option);
						e.v_bit = 1;
						test.page_table.put(e.hex_id, e);
						//Evicted best page 
						done_flag = 1;
					}
					//Don't do stuff for other algorithms
					continue;
				}
				
				//CLOCK
				//set r bit to one in clock since a page in memory was just now referenced
				if(e.v_bit == 1 && clock.contains(e) && alg.equals("clock")){
					//System.out.println("hit " + e.hex_id);
					clock.get(clock.indexOf(e)).r_bit = 1;
					test.page_table.put(e.hex_id, clock.get(clock.indexOf(e)));
					continue;
				}

				//CLOCK
				//Evicts best page
				if(alg.equals("clock")){
					//Keep track of  number of page faults
					pf_ctr++;
					//System.out.println("PAGE FAULT");
					String frame_to_evict = null;

					//Search for best page to evict
					for(int x = 0; x <= num_frames; x++){
						//reset x if you have to wrap back around circular queue
						if(x == num_frames){
							x = 0;
						}
						Table_Entry y = clock.get(x);
						//System.out.println(Integer.toHexString(test.page_table.indexOf(y)) + " page r bit = " + y.r_bit);
						if(y.r_bit == 0){
							//System.out.println("Evicting Frame " + y.frame_num + " from page " + Integer.toHexString(test.page_table.indexOf(y)));
							frame_to_evict = y.hex_id;
							clock.remove(y);
							break;
						}
						else{
							//Give page a second chance
							clock.remove(y);
							y.r_bit = 0;
							clock.addLast(y);
							test.page_table.put(y.hex_id, y);
						}
					}

						
					Table_Entry i = test.page_table.get(frame_to_evict);
					
					//System.out.println(frame_to_evict);
					e.v_bit = 1;
					//Write to disk
					if(i.d_bit == 1){
						//System.out.println("page fault - evict dirty " + i.hex_id);
						w_ctr++;
					}
					
					//Place new page into RAM
					e.frame_num = i.frame_num;
					//System.out.println("Adding NEW Page to Frame " + e.frame_num);
					clock.addLast(e);
					test.page_table.put(e.hex_id, e);
					i.frame_num = -1;
					i.v_bit = 0;
					i.d_bit = 0;
					i.r_bit = 0;
					test.page_table.put(i.hex_id, i);
					
					continue;
				}
			}
			
		}

	
		//Close scanner
		fileScan.close();
		date = new Date();
		System.out.println(date.toString() + "\n\n");
		//System.out.println(ctr);
		
		//Print statistical data
		System.out.println("Number of Frames:\t " + num_frames);
		System.out.println("Total Memory Accesses:\t" + ctr);
		System.out.println("Total Page Faults:\t" + pf_ctr);
		System.out.println("Total Writes to Disk:\t" + w_ctr);

	}


}
