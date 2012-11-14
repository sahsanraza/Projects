//NAME: Ben Isenberg
//DATE: 7/5/11
//EMAIL: bji6@pitt.edu

// CS 1501 Summer 2011
// Specifications for the SignedMessage class for Assignment 4.  You must complete
// this class and use it with your digital signature testing program

import java.security.*;
import java.math.*;
import java.io.Serializable;

public class SignedMessage implements Serializable
{
	private String theMessage;  	// Plaintext message
	private String hashAlgo;		// String name of hash algorithm
	private boolean signed;			// is message signed
	private BigInteger theSignature;	// signature for message
	private BigInteger E;			// public key exponent
	private BigInteger N;			// public key mod value


	// initialize this SignedMessage object using BigIntegers for the
	// E and N values and a String for the name of the hash algorithm used.
	public SignedMessage(BigInteger E, BigInteger N, String h)
	{
		this.E = E;
		this.N = N;
		this.hashAlgo = h;
	}

	// Assign theMessage from the parameter
	public void setMessage(String s)
	{
		theMessage = s;
	}

	// Return the message within this SignedMessage object
	public String getMessage()
	{
		return theMessage;
	}

	// return true if the message has been signed
	public boolean isSigned()
	{
		return signed;
	}

	// Sign the message using theMessage, parameter D (the private
	// key), and N and hashAlgo.  Store the result in theSignature
	// and mark the message as being signed.
	public void signMessage(BigInteger D)throws Exception
	{

		MessageDigest m1 = MessageDigest.getInstance(this.hashAlgo);

		byte[] b1 = theMessage.getBytes();
		m1.update(theMessage.getBytes());

		//Get cryptographic hash signature of the message
		byte[] digest1 = m1.digest();

		//Convert hashed signature into a BigInteger to perform RSA on it
		BigInteger orig1Int = new BigInteger(1, digest1);

		//Decrypt signature using the private key D
		theSignature = orig1Int.modPow(D, this.N);

		signed = true;

	}



	// Verify the message as discussed in lecture:
	// Run the message digest algorithm on theMessage, obtaining
	// a message digest for the "received" message
	// "Encrypt" theSignature and convert back to an array of
	// bytes to restore the message digest of the original message
	// Return true if the message is ok, false otherwise
	public boolean verifyMessage()throws Exception
	{
		MessageDigest m1 = MessageDigest.getInstance(this.hashAlgo);
		MessageDigest m2 = MessageDigest.getInstance(this.hashAlgo);

		byte[] b1 = theMessage.getBytes();
		m1.update(b1);

		//Get cryptographic hash signature of the message
		byte[] digest1 = m1.digest();

		//Convert hashed signature into a BigInteger to perform RSA on it
		BigInteger orig1Int = new BigInteger(1, digest1);

		//Encrypt the decrypted signature to get back hashed signature
		BigInteger encrypted = theSignature.modPow(this.E, this.N);

		//If the messages are equal then it wasn't corrupted
		if(orig1Int.equals(encrypted)){
			return true;
		}

		return false;

	}
}

