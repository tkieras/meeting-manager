package com.tkieras.eecs381.project0;

import java.util.*;
import java.io.*;

import java.util.stream.*;


public class Meeting implements Comparable<Meeting>{

	public static boolean timeIsMorning(Integer time) {

		if (time >= 9 && time < 12) {
			return true;
		}
		return false;
	}

	public static boolean timeIsAfternoon(Integer time) {

		if (time >= 1 && time < 5) {
			return true;
		} else if(time == 12) {
			return true;
		} 
		return false;
		
	}

	public static boolean isValidTime(Integer time) {

		return (timeIsMorning(time) || timeIsAfternoon(time));
	}

	public static Meeting updateTime(Meeting m, Integer newTime) {
		
		return new Meeting(m, newTime);
	}

	public static Meeting addParticipant(Meeting m, Person p) {

		List<Person> newParticipantList = new ArrayList<>(m.participantList);

		newParticipantList.add(p);

		Collections.sort(newParticipantList);
		
		return new Meeting(m, newParticipantList);
	}

	public static Meeting deleteParticipant(Meeting m, Person p) {

		List<Person> newParticipantList = new ArrayList<>(m.participantList);

		newParticipantList.remove(p);

		return new Meeting(m, newParticipantList);
	}

	public static Optional<Meeting> loadMeeting(BufferedReader in, List<Person> peopleList) throws IOException {

		List<String> meetingInfo = Arrays.asList(in.readLine().split(" "));

		Integer time = Integer.parseInt(meetingInfo.get(0));

		String topic = meetingInfo.get(1);
       
        Integer participantListCount = Integer.parseInt(meetingInfo.get(2));

        List<Person> participantList = new ArrayList<>();

        for (int i = 0; i < participantListCount; i++) {

            String participantName = in.readLine();

            Optional<Person> participant = peopleList.stream()
            										 .filter(p -> p.getLastName().equals(participantName))
            										 .findAny();

           	participant.ifPresent(p -> participantList.add(p));

        }

        Meeting newMeeting = Meeting.initializeFullMeeting(time, topic, participantList);

        if (newMeeting.getParticipantCount() != participantListCount) {
            return Optional.empty();
        }

        return Optional.of(newMeeting);
	}

	public static Meeting initializeEmptyMeeting(int time, String topic) {
		return new Meeting(time, topic);
	}

	public static Meeting initializeFullMeeting(int time, String topic, List<Person> participantList) {
		return new Meeting(time, topic, participantList);
	}

	private final int time;

	private final String topic;

	private final List<Person> participantList;

	private Meeting(int time, String topic) {
		this.time = time;
		this.topic = topic;
		this.participantList = Collections.unmodifiableList(new ArrayList<>());
	}

	private Meeting(Meeting m, List<Person> newParticipantList) {
		this.time = m.time;
		this.topic = m.topic;
		this.participantList = Collections.unmodifiableList(newParticipantList);
	}

	private Meeting(Meeting m, int time) {
		this.time = time;
		this.topic = m.topic;
		this.participantList = m.participantList;
	}

	private Meeting(int time, String topic, List<Person> participantList) {
		this.time = time;
		this.topic = topic;
		this.participantList = Collections.unmodifiableList(participantList);
	}

	public int getTime() {
		return time;
	}

	public String getTopic() {
		return topic;
	}

	public boolean personIsParticipant(Person person) {
		return participantList.contains(person);
	}

	public int getParticipantCount() {
		return participantList.size();
	}

	public String print() {

		String result = "Meeting time: " + time
						+ ", Topic: " + topic 
						+ "\nParticipants:";

		if (participantList.isEmpty()) {
			
			result += " None";
		
		}
		else {

			result += "\n" + participantList.stream()
									 .map(Person::print)
									 .collect(Collectors.joining("\n"));
		}

		return result;
				
	}

	public void save(PrintStream out) {

		out.println(time + " " + topic + " " + participantList.size());

		participantList.stream().forEach(p -> out.println(p.getLastName()));
		
	}


	@Override
	public boolean equals(Object obj) {
		if (obj == this) {
			return true;
		}
		if ((obj == null) || (getClass() != obj.getClass())) {
			return false;
		}
		Meeting that = (Meeting) obj;

		return that.time == time
			&& that.topic.equals(topic);
	}

	@Override
	public int hashCode() {
		int result = Integer.valueOf(time).hashCode();
		result += topic.hashCode() * 37;
		return result;
	}

	public int compareTo(Meeting that) {

		Integer thisTime = Integer.valueOf(this.time);
		Integer thatTime = Integer.valueOf(that.time);

		if (timeIsAfternoon(thisTime) && timeIsAfternoon(thatTime)) {
			return thisTime.compareTo(thatTime);
		}
		else if (timeIsMorning(thisTime) && timeIsMorning(thatTime)) {
			return thisTime.compareTo(thatTime);
		}
		else if (timeIsMorning(thisTime) && timeIsAfternoon(thatTime)) {
			//thisTime is less
			return -1;
		}
		else {
			//thisTime is greater
			return 1;
		}

	}
};