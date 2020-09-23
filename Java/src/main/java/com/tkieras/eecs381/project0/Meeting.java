package com.tkieras.eecs381.project0;

import java.util.*;
import java.io.*;


public class Meeting implements Comparable<Meeting>{

	public static boolean timeIsMorning(Integer time) {
		if(time >= 9 && time < 12) {
			return true;
		}
		return false;
	}
	public static boolean timeIsAfternoon(Integer time) {
		if(time >= 1 && time < 5) {
			return true;
		}else if(time == 12) {
			return true;
		}else {
			return false;
		}
	}

	public static boolean isValidTime(Integer time) {
		return (timeIsMorning(time) || timeIsAfternoon(time));
	}

	public static Optional<Meeting> loadMeeting(BufferedReader in, 
		Set<Person> peopleList) throws IOException {

		List<String> meetingInfo = Arrays.asList(in.readLine().split(" "));

		Meeting newMeeting = new Meeting(new Integer(meetingInfo.get(0)), meetingInfo.get(1));
       
        Integer participantListCount = new Integer(meetingInfo.get(2));

        for(int z = 0; z < participantListCount; z++) {

            String participantName = in.readLine();

            Optional<Person> participant = peopleList.stream()
            										 .filter(p -> p.getLastName().equals(participantName))
            										 .findAny();

           	participant.ifPresent(p -> newMeeting.addParticipant(p));

        }

        if(newMeeting.getParticipantCount() != participantListCount) {
            return Optional.empty();
        }

        return Optional.of(newMeeting);
	}

	private int time;
	private final String topic;
	private final Set<Person> participantList = new TreeSet<>();

	public Meeting(int time, String topic) {
		this.time = time;
		this.topic = topic;
	}

	public int getTime() {
		return time;
	}

	public String getTopic() {
		return topic;
	}

	public void setTime(int newTime) {
		time = newTime;
	}

	public boolean addParticipant(Person person) {
		return participantList.add(person);

	}

	public boolean deleteParticipant(Person person) {
		return participantList.remove(person);

	}

	public boolean personIsParticipant(Person person) {
		return participantList.contains(person);
	}

	public int getParticipantCount() {
		return participantList.size();
	}

	public void print(PrintStream out) {
		out.print("Meeting time: " + time
			+ ", Topic: " + topic 
			+ "\nParticipants:" );

		if(participantList.isEmpty()) {
			out.println(" None");
		}
		else {
			out.println("");

			participantList.stream().forEach(p -> out.println(p.toString()));
			
		}
	}

	public void save(PrintStream out) {
		out.println(time + " " + topic + " " + participantList.size());

		participantList.stream().forEach(p -> out.println(p.getLastName()));
		
	}

	public void load() {};


	@Override
	public boolean equals(Object obj) {
		if(obj == this) {
			return true;
		}
		if((obj == null) || (getClass() != obj.getClass())) {
			return false;
		}
		Meeting that = (Meeting) obj;

		return that.time == time
			&& that.topic.equals(topic);
	}

	@Override
	public int hashCode() {
		int result = new Integer(time).hashCode();
		result += topic.hashCode() * 37;
		return result;
	}

	public int compareTo(Meeting that) {

		Integer thisTime = new Integer(this.time);
		Integer thatTime = new Integer(that.time);


		if(timeIsAfternoon(thisTime) && timeIsAfternoon(thatTime)) {
			return thisTime.compareTo(thatTime);
		}
		else if(timeIsMorning(thisTime) && timeIsMorning(thatTime)) {
			return thisTime.compareTo(thatTime);
		}
		else if(timeIsMorning(thisTime) && timeIsAfternoon(thatTime)) {
			//thisTime is less
			return -1;
		}
		else {
			//thisTime is greater
			return 1;
		}

	}
};