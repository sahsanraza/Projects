//NAME: Ben Isenberg
//DATE: 7/5/11
//EMAIL: bji6@pitt.edu


import java.util.*;
import java.io.*;
import java.math.BigInteger;

/*This is a test program for RSA encryption*/


public class Test{


	public static void main(String[] args)throws Exception{

		//Enter 1 to sign a message, 2 to corrupt a message, or 3 to verify a message
		System.out.println("RSA: 1. Sign a Message\t2. Corrupt a Message\t3. Verify a Message");

		Scanner keyboard = new Scanner(System.in);

		int num = keyboard.nextInt();

		//Sign a message
		if(num == 1){

			Scanner fileScan = new Scanner(new FileInputStream("keys.txt"));

			//Get data from keys.txt
			fileScan.skip("E = ");
			String s = fileScan.next();
			BigInteger E = new BigInteger(s);

			fileScan.nextLine();
			fileScan.skip("D = ");
			s = fileScan.next();
			BigInteger D = new BigInteger(s);

			fileScan.nextLine();
			fileScan.skip("N = ");
			s = fileScan.next();
			BigInteger N = new BigInteger(s);

			s = "SHA-256";
			//Creat SignedMessage object
			SignedMessage sm = new SignedMessage(E, N, s);

			fileScan = new Scanner(new FileInputStream("test.txt"));

			s = fileScan.nextLine();
			//Read in entire file as one string
			while(fileScan.hasNext()){

				s = s + fileScan.nextLine();

			}

			//Set message to read in string
			sm.setMessage(s);

			//sign the message
			sm.signMessage(D);

			FileOutputStream f_out = new FileOutputStream("test.txt.signed");
			ObjectOutputStream objOut = new ObjectOutputStream(f_out);

			objOut.writeObject(sm);

			objOut.close();
			f_out.close();
			fileScan.close();

			return;

		}
		else{
			//Corrupt the message
			if(num == 2){

				System.out.println("Enter file name:");
				String str = keyboard.next();

				FileInputStream f_in = new FileInputStream(str);
				ObjectInputStream objIn = new ObjectInputStream(f_in);

				SignedMessage sm = (SignedMessage) objIn.readObject();
				String s = sm.getMessage();

				//Replace all instances of the letter a with the $ symbol
				s = s.replace('a', '$');
				sm.setMessage(s);

				FileOutputStream f_out = new FileOutputStream(str);
				ObjectOutputStream objOut = new ObjectOutputStream(f_out);

				objOut.writeObject(sm);

				objIn.close();
				f_in.close();
				objOut.close();
				f_out.close();

				return;


			}
			else{
				//Verify the message
				if(num == 3){

					System.out.println("Enter file name:");
					String str = keyboard.next();

					FileInputStream f_in = new FileInputStream(str);
					ObjectInputStream objIn = new ObjectInputStream(f_in);

					SignedMessage sm = (SignedMessage) objIn.readObject();

					boolean tester = sm.verifyMessage();

					if(tester){
						System.out.println("THIS MESSAGE WAS NOT CORRUPTED: " + sm.getMessage());
					}
					else{
						System.out.println("THIS MESSAGE WAS TAMPERED WITH: " + sm.getMessage());
					}

					return;



				}
				else{
					//User entered an invalid command
					System.out.println("Invalid Command");
					return;
				}

			}

		}


	}


}