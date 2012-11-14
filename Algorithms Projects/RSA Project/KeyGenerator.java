//NAME: Ben Isenberg
//DATE: 7/5/11
//EMAIL: bji6@pitt.edu


import java.util.*;
import java.io.*;
import java.math.BigInteger;

/*This class generates all the keys and numbers required for RSA encryption*/


public class KeyGenerator{


	private BigInteger x;
	private BigInteger y;
	private BigInteger phi;
	private BigInteger n;
	private BigInteger e;
	private BigInteger d;
	private Random R;

	public KeyGenerator(){

		R = new Random();

		//Genterate X and Y as random prime 512 bit numbers
		x = BigInteger.probablePrime(512, R);
		y = BigInteger.probablePrime(512, R);

		BigInteger one = new BigInteger("1");

		//x-1 used to find PHI
		BigInteger x2 = x.subtract(one);
		//y-1 used to find PHI
		BigInteger y2 = y.subtract(one);

		//PHI = (x-1)*(y-1)
		phi = x2.multiply(y2);

		while(true){

			e = BigInteger.probablePrime(512, R);

			//If E < PHI
			if(e.compareTo(phi) == -1){
				//If Greatest Common Divisor of E and PHI = 1 exit loop
				if((e.gcd(phi).compareTo(one))== 0){
					System.out.println("E = " + this.getE() + "\n");
					break;
				}

			}

		}


		//Find D
		d = e.modInverse(phi);
		System.out.println("D = " + this.getD() + "\n");

		//Find N
		n = x.multiply(y);
		System.out.println("N = " + this.getN() + "\n");


	}


	public BigInteger getX(){
		return x;
	}

	public BigInteger getY(){
			return y;
	}

	public BigInteger getPhi(){
			return phi;
	}

	public BigInteger getE(){
			return e;
	}

	public BigInteger getD(){
			return d;
	}

	public BigInteger getN(){
			return n;
	}



	public static void main(String[] args)throws FileNotFoundException{


		KeyGenerator key = new KeyGenerator();

		PrintWriter outputFile = new PrintWriter("keys.txt");

		outputFile.println("E = " + key.getE());
		outputFile.println("D = " + key.getD());
		outputFile.println("N = " + key.getN());

		outputFile.close();

	}

}