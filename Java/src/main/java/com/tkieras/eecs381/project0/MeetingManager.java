package com.tkieras.eecs381.project0;

import java.util.*;
import java.io.*;
import java.nio.file.*;
import java.nio.charset.*;

import java.util.stream.*;
import java.util.Optional.*;
import java.util.regex.*;
import java.util.function.*;


public class MeetingManager {

    public static void main(String[] args) {

        if (args == null || args.length > 0) {

            throw new IllegalArgumentException();
        }

        MeetingManager manager = MeetingManager.initializeEmptyMeetingManager();
        
        Scanner in = new Scanner(System.in);

        String command;

        WithMessage<MeetingManager> output;

        do {
            System.out.print("\nEnter Command: ");

            command = in.nextLine();

            output = CommandFunctions.applyCommand(manager, command);

            System.out.println(output.getMessage());

            manager = output.getObject();
            

            // System.out.println("debugging:");

            // manager.peopleList.stream().forEach(p -> System.out.println(p.print()));

            // manager.roomList.stream().forEach(r -> System.out.println(r.print()));

            // System.out.println("end debugging");


            if (command.trim().startsWith("qq")) {
                return;
            }
        }
        while (true);

    }


    public static MeetingManager addRoom(MeetingManager mm, Integer number) {

        Room newRoom = Room.initializeEmptyRoom(number);

        List<Room> newRoomList = new ArrayList<Room>(mm.roomList);

        newRoomList.add(newRoom);

        Collections.sort(newRoomList);

        return new MeetingManager(mm.peopleList, newRoomList);

    }

    public static MeetingManager deleteRoom(MeetingManager mm, Room r) {

        List<Room> newRoomList = new ArrayList<Room>(mm.roomList);

        newRoomList.remove(r);

        return new MeetingManager(mm.peopleList, newRoomList);
    }

    public static MeetingManager updateRoom(MeetingManager mm, Room srcRoom, Room updatedSrcRoom) {

        List<Room> newRoomList = new ArrayList<Room>(mm.roomList);

        newRoomList.remove(srcRoom);
        
        newRoomList.add(updatedSrcRoom);

        Collections.sort(newRoomList);

        return new MeetingManager(mm.peopleList, newRoomList);

    }


    public static MeetingManager addIndividual(MeetingManager mm, String arg1, String arg2, String arg3) {
        Person newPerson = new Person(arg1, arg2, arg3);

        List<Person> newPeopleList = new ArrayList<Person>(mm.peopleList);

        newPeopleList.add(newPerson);

        Collections.sort(newPeopleList);
        
        return new MeetingManager(newPeopleList, mm.roomList);
    }

    public static MeetingManager deleteIndividual(MeetingManager mm, Person p) {
        List<Person> newPeopleList = new ArrayList<Person>(mm.peopleList);

        newPeopleList.remove(p);

        return new MeetingManager(newPeopleList, mm.roomList);
    }

    public static MeetingManager deleteGroup(MeetingManager mm) {

        return new MeetingManager(new ArrayList<Person>(), mm.roomList);
    }


    public static MeetingManager deleteSchedule(MeetingManager mm) {

        return new MeetingManager(mm.peopleList, new ArrayList<Room>());
    }

    public static MeetingManager addMeeting(MeetingManager mm, Room r, List<String> commandList) {

        Integer time = Integer.parseInt(commandList.get(2));

        Meeting newMeeting = Meeting.initializeEmptyMeeting(time, commandList.get(3));

        Room newRoom = Room.addMeeting(r, newMeeting);

        List<Room> newRoomList = new ArrayList<Room>(mm.roomList);

        newRoomList.remove(r);

        newRoomList.add(newRoom);

        Collections.sort(newRoomList);

        return new MeetingManager(mm.peopleList, newRoomList);

    }

    public static MeetingManager deleteMeeting(MeetingManager mm, Room r, Integer time) {

        Meeting m = r.getMeeting(time);

        Room newRoom = Room.deleteMeeting(r, m);

        List<Room> newRoomList = new ArrayList<Room>(mm.roomList);

        newRoomList.remove(r);

        newRoomList.add(newRoom);

        Collections.sort(newRoomList);

        return new MeetingManager(mm.peopleList, newRoomList);

    }

    public static MeetingManager addParticipant(MeetingManager mm, Room r, Meeting m, Person p) {

        Meeting newMeeting = Meeting.addParticipant(m, p);

        Room newRoom = Room.deleteMeeting(r, m);
        
        newRoom = Room.addMeeting(newRoom, newMeeting);

        List<Room> newRoomList = new ArrayList<Room>(mm.roomList);

        newRoomList.remove(r);

        newRoomList.add(newRoom);

        Collections.sort(newRoomList);

        return new MeetingManager(mm.peopleList, newRoomList);


    }

    public static MeetingManager deleteParticipant(MeetingManager mm, Room r, Meeting m, Person p) {

        Meeting newMeeting = Meeting.deleteParticipant(m, p);

        Room newRoom = Room.deleteMeeting(r, m);
        
        newRoom = Room.addMeeting(newRoom, newMeeting);

        List<Room> newRoomList = new ArrayList<Room>(mm.roomList);

        newRoomList.remove(r);

        newRoomList.add(newRoom);

        Collections.sort(newRoomList);

        return new MeetingManager(mm.peopleList, newRoomList);

    }


    public static MeetingManager initializeEmptyMeetingManager() {
        return new MeetingManager();
    }

    public static MeetingManager initializeFullMeetingManager(List<Person> peopleList, List<Room> roomList) {
        return new MeetingManager(peopleList, roomList);
    }

    private MeetingManager(){
        peopleList = Collections.unmodifiableList(new ArrayList<Person>());
        roomList = Collections.unmodifiableList(new ArrayList<Room>());
    }
    private MeetingManager(List<Person> newPeopleList, List<Room> newRoomList) {
        peopleList = Collections.unmodifiableList(newPeopleList);
        roomList = Collections.unmodifiableList(newRoomList);
    }


    private final List<Person> peopleList;

    private final List<Room> roomList;

    public boolean roomExists(int number) {
        return roomList.stream().anyMatch( r -> r.getNumber() == number);
    }

    public Room getRoom(int number) {
        return roomList.stream().filter( r -> r.getNumber() == number).findAny().get();
    }

    public boolean personExists(String lastName) {
        return peopleList.stream().anyMatch( p -> p.getLastName().equals(lastName));
    }

    public Person getPerson(String lastName) {
        return peopleList.stream().filter( p -> p.getLastName().equals(lastName)).findAny().get();
    }

    public List<Room> getRoomList() {
        return roomList;
    }

    public List<Person> getPeopleList() {
        return peopleList;
    }

}

            
        
  