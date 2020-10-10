package com.tkieras.eecs381.project0;

import java.util.*;
import java.io.*;

import java.util.stream.*;

public class Room implements Comparable<Room> {

	public static boolean roomNumberInRange(Integer roomNumber) {
        return roomNumber > 0;
	}

	public static Optional<Room> loadRoom(BufferedReader in, 
		List<Person> peopleList) throws IOException {

		List<String> roomInfo = Arrays.asList(in.readLine().split(" "));

		Integer number = Integer.parseInt(roomInfo.get(0));

        Integer meetingListCount = Integer.parseInt(roomInfo.get(1));

        List<Meeting> meetingList = new ArrayList<>();

        for(int j = 0; j < meetingListCount; j++) {
            Optional<Meeting> newMeeting = Meeting.loadMeeting(in, peopleList);

            if (!newMeeting.isPresent()) {
                return Optional.empty();
            }

            meetingList.add(newMeeting.get());

        }

        Room newRoom = Room.initializeFullRoom(number, meetingList);

        if (newRoom.getMeetingCount() != meetingListCount) {
        	return Optional.empty();

        }

        return Optional.of(newRoom);

	}

	public static Room addMeeting(Room r, Meeting m) {

		List<Meeting> newMeetingList = new ArrayList<>(r.meetingList);

		newMeetingList.add(m);

		Collections.sort(newMeetingList);

		return new Room(r, newMeetingList);
	}

	public static Room deleteMeeting(Room r, Meeting m) {

		List<Meeting> newMeetingList = new ArrayList<>(r.meetingList);

		newMeetingList.remove(m);

		return new Room(r, newMeetingList);
	}

	public static Room initializeEmptyRoom(int number) {
		return new Room(number);
	}

	public static Room initializeFullRoom(int number, List<Meeting> meetingList) {
		return new Room(number, meetingList);
	}

	private final int number;
	
	private final List<Meeting> meetingList;

	private Room(Room r, List<Meeting> newMeetingList) {
		this.number = r.getNumber();
		this.meetingList = Collections.unmodifiableList(newMeetingList);
	}

	private Room(int number) {
		this.number = number;
		this.meetingList = Collections.unmodifiableList(new ArrayList<Meeting>());
	}

	private Room(int number, List<Meeting> meetingList) {
		this.number = number;
		this.meetingList = Collections.unmodifiableList(meetingList);
	}


	public int getNumber() {
		return number;
	}

	public String print() {
		String result = "--- Room " + number + " ---" + "\n";


		if (meetingList.isEmpty()) {
			
			result += "No meetings are scheduled" + "\n";

		} else {
			result += meetingList.stream().map(Meeting::print).collect(Collectors.joining("\n"));
		
		}
		
		return result;
	}

	public Meeting getMeeting(int time) {

		return meetingList.stream()
						  .filter(m -> m.getTime() == time)
						  .findAny().get();

	}

	public int getMeetingCount() {
		return meetingList.size();
	}

	public boolean personIsParticipantInRoom(Person person) {
		return meetingList.stream().anyMatch((Meeting m) -> m.personIsParticipant(person));
		
	}

	public boolean meetingAtTime(int time) {
		return meetingList.stream().anyMatch(m -> m.getTime() == time);
	}

	public void save(PrintStream out) {
		out.println(number + " " + meetingList.size());

		meetingList.stream().forEach(m -> m.save(out));
		
	}

	@Override
	public boolean equals(Object obj) {
		if (obj == this) {
			return true;
		}
		if ((obj == null) || (getClass() != obj.getClass())) {
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
		return Integer.valueOf(number).compareTo(Integer.valueOf(that.number));
	}


};