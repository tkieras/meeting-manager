package com.tkieras.meetingmanager;

import java.util.*;
import java.io.*;


public class Room implements Comparable<Room> {

	public static boolean roomNumberInRange(Integer roomNumber) {
        return roomNumber > 0;
	}

	public static Optional<Room> loadRoom(BufferedReader in, 
		Set<Person> peopleList) throws IOException {

		String[] roomInfo = in.readLine().split(" ");

		Room newRoom = new Room(Integer.parseInt(roomInfo[0]));

        int meetingListCount = Integer.parseInt(roomInfo[1]);

        for(int j = 0; j < meetingListCount; j++) {
            Optional<Meeting> newMeeting = Meeting.loadMeeting(in, peopleList);

            if(newMeeting.isEmpty()) {
                return Optional.empty();
            }

            newRoom.addMeeting(newMeeting.get());

        }

        if(newRoom.getMeetingCount() != meetingListCount) {
        	return Optional.empty();

        }

        return Optional.of(newRoom);

	}

	private final int number;
	
	private final Set<Meeting> meetingList = new TreeSet<>();

	public Room(int number) {
		this.number = number;
	}

	public int getNumber() {
		return number;
	}

	public void print(PrintStream out) {
		out.println("--- Room " + number + " ---");

		if(meetingList.isEmpty()) {
			out.println("No meetings are scheduled");
		}
		else {
			meetingList.forEach(m -> m.print(out));
		}
	}

	public boolean addMeeting(Meeting meeting) {
		return meetingList.add(meeting);
	}

	public Optional<Meeting> getMeeting(int time) {

		return meetingList.stream()
						  .filter(m -> m.getTime() == time)
						  .findAny();

	}

	public void deleteMeeting(Meeting meeting) {
		meetingList.remove(meeting);
	}

	public int getMeetingCount() {
		return meetingList.size();
	}

	public boolean personIsParticipantInRoom(Person person) {
		return meetingList.stream().noneMatch((Meeting m) -> m.personIsParticipant(person));
		
	}

	public boolean meetingAtTime(int time) {
		return meetingList.stream().anyMatch(m -> m.getTime() == time);
	}

	public void save(PrintStream out) {
		out.println(number + " " + meetingList.size());

		meetingList.forEach(m -> m.save(out));
		
	}

	@Override
	public boolean equals(Object obj) {
		if(obj == this) {
			return true;
		}
		if((obj == null) || (getClass() != obj.getClass())) {
			return false;
		}

		Room that = (Room) obj;

		return number == that.number;
	}

	@Override
	public int hashCode() {
		return Integer.valueOf(number).hashCode();
	}

	public int compareTo(Room that) {
		return Integer.compare(number, that.number);
	}

}