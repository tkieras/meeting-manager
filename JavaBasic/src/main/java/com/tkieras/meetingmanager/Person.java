package com.tkieras.meetingmanager;

import java.io.*;

public class Person implements Comparable<Person> {
	private final String firstName;
	
	private final String lastName;
	
	private final String phoneNumber;

	public Person(String firstName, String lastName, String phoneNumber) {
		this.firstName = firstName;
		this.lastName = lastName;
		this.phoneNumber = phoneNumber;
	}

	public String getFirstName() {
		return firstName;
	}

	public String getLastName() {
		return lastName;
	}

	public String getPhoneNumber() {
		return phoneNumber;
	}

	public void print(PrintStream out) {
		 out.printf("%s %s %s%n", firstName, lastName, phoneNumber);
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) {
			return true;
		}

		if((obj == null) || (getClass() != obj.getClass())) {
			return false;
		}

		Person that = (Person) obj;

		return firstName.equals(that.firstName) 
			&& lastName.equals(that.lastName)
			&& phoneNumber.equals(that.phoneNumber);
	}

	@Override
	public int hashCode() {
		int result = firstName.hashCode() * 37;
		result += lastName.hashCode() * 37;
		result += phoneNumber.hashCode() * 37;
		return result;
	}

	public int compareTo(Person that) {
		return lastName.compareTo(that.lastName);
	}
}