package com.tkieras.meetingmanager;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;

import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.NoSuchFileException;
import java.nio.file.Paths;
import java.util.Optional;
import java.util.Scanner;
import java.util.Set;
import java.util.TreeSet;
import java.util.regex.Pattern;

public class MeetingManager {

    public static void main(String[] args) {

        if (args == null || args.length > 0) {

            throw new IllegalArgumentException();
        }

        MeetingManager manager = new MeetingManager(System.in, System.out);
        
        manager.run();

    }

    public MeetingManager(InputStream in, PrintStream out){
        this.in = new Scanner(in);
        this.out = out;
    }


    private final Set<Person> peopleList = new TreeSet<>();

    private final Set<Room> roomList = new TreeSet<>();
  
    private final Scanner in;

    private final PrintStream out;

    public void run() {

        String s1, s2;
        char c1, c2;

        Pattern whitespace = Pattern.compile("\\s");
        Pattern character = Pattern.compile("\\w");
        Pattern defaultDelimiter = in.delimiter();

        while (true) {
           
            out.print("\nEnter command: ");
            
            // Read two non-whitespace characters with typeahead

            in.useDelimiter("");

            while (in.hasNext(whitespace)) {
                in.skip(whitespace);
            }

            s1 = in.next(character);

            while (in.hasNext(whitespace)) {
                in.skip(whitespace);
            }

            s2 = in.next(character);

            c1 = s1.charAt(0);
            c2 = s2.charAt(0);

            in.useDelimiter(defaultDelimiter);


            switch(c1){
                case 'p':
                    switch(c2){
                        case 'i': print_individual(); break;
                        case 'r': print_room(); break;
                        case 'm': print_meeting(); break;
                        case 's': print_schedule(); break;
                        case 'g': print_group(); break;
                        case 'a': print_allocation(); break;
                        default:
                            inputError("Unrecognized command!");
                    }
                break;
                case 'a':
                    switch(c2){
                        case 'i': add_individual(); break;
                        case 'r': add_room(); break;
                        case 'm': add_meeting(); break;
                        case 'p': add_participant(); break;
                        default:
                            inputError("Unrecognized command!");
                    }
                break;
                case 'r':
                    switch(c2){
                        case 'm': reschedule_meeting(); break;
                        default:
                            inputError("Unrecognized command!");
                    }
                break;
                case 'd':
                    switch(c2){
                        case 'i': delete_individual(); break;
                        case 'r': delete_room(); break;
                        case 'm': delete_meeting(); break;
                        case 'p': delete_participant(); break;
                        case 's': delete_schedule(); break;
                        case 'g': delete_group(); break;
                        case 'a': delete_all(); break;
                        default:
                            inputError("Unrecognized command!");
                    }
                break;
                case 's':
                    switch(c2){
                        case 'd': save_data(); break;
                        default:
                            inputError("Unrecognized command!");
                    }
                break;
                case 'l':
                    switch(c2){
                        case 'd': load_data(); break;
                        default:
                            inputError("Unrecognized command!");
                    }
                break;
                case 'q':
                    switch(c2){
                        case 'q':
                            delete_all();
                            out.println("Done");
                            return;
                    default:
                        inputError("Unrecognized command!");
                    }
                break;
                default:
                    inputError("Unrecognized command!");
            }

            
        }
    
    }

    private void inputError(String message) {
        out.printf("%s\n", message);

        if (in.hasNextLine()) {
            in.nextLine();
        }

    }

    private Optional<Integer> readNumber() {

        Optional<Integer> number = Optional.empty();

        if (in.hasNextInt()) {
            number = Optional.of(in.nextInt());
        } else {
            inputError("Could not read an integer value!");
        }

        return number;
    }

    private Optional<Integer> readRoomNumber() {

        Optional<Integer> number = readNumber();

        boolean reportNewError = number.isPresent();

        number = number.filter(Room::roomNumberInRange);

        if (reportNewError && number.isEmpty()) {
            inputError("Room number is not in range!");
        }

        return number;

    }


    private Optional<Integer> readTime() {

        Optional<Integer> time = readNumber();

        boolean reportNewError = time.isPresent();

        time = time.filter(Meeting::isValidTime);

        if (reportNewError && time.isEmpty()) {
            inputError("Time is not in range!");
        }

        return time;

    }

    private Optional<Person> readAndFindPerson() {

        String lastName = in.next();
        
        Optional<Person> person = peopleList.stream()
                  .filter(p -> p.getLastName().equals(lastName))
                  .findAny();

        if (person.isEmpty()) {
            inputError("No person with that name!");
        }
        
        return person;       
    }

    private Optional<Room> readAndFindRoom() {
        
        return readRoomNumber().flatMap((n) -> {
            Optional<Room> room = roomList.stream()
                                          .filter(r -> r.getNumber() == n)
                                          .findAny();

            if (room.isEmpty()) {
              inputError("No room with that number!");
            }
            return room;
        });
       
    }

    private Optional<Meeting> readAndFindMeeting(Room room) {

        return readTime().flatMap((t) -> {
            if (!room.meetingAtTime(t)) {
                inputError("No meeting at that time!");
                return Optional.empty();
            }
            return room.getMeeting(t);
            });
    }

    private Optional<Meeting> readAndFindMeeting() {

        return readAndFindRoom().flatMap(this::readAndFindMeeting);
    }

    private void print_individual(){ 

       readAndFindPerson().ifPresent((p) -> p.print(out));
    }

    private void print_room(){

        readAndFindRoom().ifPresent((r) -> r.print(out));  
    }
    private void print_meeting(){

        readAndFindMeeting().ifPresent((m) -> m.print(out));
    }

    private void print_schedule(){

        if (!roomList.isEmpty()) {
        
            out.println("Information for " + roomList.size() + " rooms:");

            roomList.forEach((r) -> r.print(out));

        } else {
            out.println("List of rooms is empty");
        }

    }

    private void print_group(){

        if (!peopleList.isEmpty()){

           out.println("Information for " + peopleList.size() + " people:");

           peopleList.forEach((p) -> p.print(out));

        } else {
            out.println("List of people is empty");
        }

    }

    private void print_allocation(){

        Integer meetingCount = roomList.stream()
            .map(Room::getMeetingCount)
            .reduce(0, Integer::sum);
    
        out.println("Memory allocations:");
        
        out.println("Persons: " + peopleList.size()); 
        
        out.println("Meetings: " + meetingCount);
        
        out.println("Rooms: " + roomList.size());
    }

    private void add_individual(){

        String firstName = in.next();
        
        String lastName = in.next();
        
        String phoneNumber = in.next();

        Person newPerson = new Person(firstName, lastName, phoneNumber);

        if (peopleList.contains(newPerson)) {
            inputError("There is already a person with this last name!");

        } else {
            
            peopleList.add(newPerson);

            out.println("Person " + newPerson.getLastName() + " added");   

        }
    }
    
    private void add_room(){

        Optional<Integer> readRoomNumberResult = readRoomNumber();

        if (readRoomNumberResult.isEmpty()) {
            return;
        }

        int number = readRoomNumberResult.get();

        Optional<Room> findRoomResult = roomList.stream()
                                                .filter(r -> r.getNumber() == number)
                                                .findAny();

        if (findRoomResult.isPresent()) {
            inputError("There is already a room with this number!");
            return;
        }

        roomList.add(new Room(number));

        out.println("Room " + number + " added!");

    }
    
    private void add_meeting(){

        Optional<Room> readRoomResult = readAndFindRoom();

        if (readRoomResult.isEmpty()) {
            return;
        }

        Room room = readRoomResult.get();

        Optional<Integer> readTimeResult = readTime();

        if (readTimeResult.isEmpty()) {
            return;
        }

        int time = readTimeResult.get();

        if (room.meetingAtTime(time)) {
            inputError("There is already a meeting at that time!");
            return;
        }

        String topic = in.next();
        room.addMeeting(new Meeting(time, topic));
        out.println("Meeting added at " + time);


    }
    
    private void add_participant(){
        
        Optional<Meeting> readMeetingResult = readAndFindMeeting();

        if (readMeetingResult.isEmpty()) {
            return;
        }

        Meeting meeting = readMeetingResult.get();

        Optional<Person> readPersonResult = readAndFindPerson();

        if (readPersonResult.isEmpty()) {
            return;
        }

        Person person = readPersonResult.get();

        if (meeting.personIsParticipant((person))) {
            inputError("This person is already a participant!");
            return;
        }

        meeting.addParticipant(person);
        out.println("Participant " + person.getLastName() + " added");

    }
    
    private void reschedule_meeting(){

        Optional<Room> oldRoomResult = readAndFindRoom();

        if (oldRoomResult.isEmpty()) {
            return;
        }

        Room oldRoom = oldRoomResult.get();

        Optional<Meeting> meetingResult = readAndFindMeeting(oldRoom);

        if (meetingResult.isEmpty()) {
            return;
        }

        Meeting meeting = meetingResult.get();

        Optional<Room> newRoomResult = readAndFindRoom();

        if (newRoomResult.isEmpty()) {
            return;
        }

        Room newRoom = newRoomResult.get();

        Optional<Integer> newTimeResult = readTime();

        if (newTimeResult.isEmpty()) {
            return;
        }

        int newTime = newTimeResult.get();

        if (newRoom.meetingAtTime(newTime)) {
            inputError("There is already a meeting at that time!");
            return;
        }

        oldRoom.deleteMeeting(meeting);
        meeting.setTime(newTime);
        newRoom.addMeeting(meeting);

        out.println("Meeting rescheduled to room " + newRoom.getNumber() + " at " + newTime);

    }

    
    private void delete_individual(){

        readAndFindPerson().ifPresent(p -> {
            if (roomList.stream().noneMatch(r -> r.personIsParticipantInRoom(p))) {
                peopleList.remove(p);
                out.println("Person " + p.getLastName() + " deleted");
            } else {
                inputError("This person is a participant in a meeting!");
            }
        });

    }
    
    private void delete_room(){
        readAndFindRoom().ifPresent((r) -> {
                roomList.remove(r);
                out.println("Room " + r.getNumber() + " deleted");
            });
    }
    
    private void delete_meeting(){
        Optional<Room> readRoomResult = readAndFindRoom();

        if (readRoomResult.isEmpty()) {
            return;
        }

        Room room = readRoomResult.get();

        Optional<Meeting> meetingResult = readAndFindMeeting(room);

        if (meetingResult.isEmpty()) {
            return;
        }

        Meeting meeting = meetingResult.get();

        room.deleteMeeting(meeting);

        out.println("Meeting at " + meeting.getTime() + " deleted");

    }

    private void delete_participant(){
        Optional<Meeting> meetingResult = readAndFindMeeting();

        if (meetingResult.isEmpty()) {
            return;
        }

        Meeting meeting = meetingResult.get();

        Optional<Person> readPersonResult = readAndFindPerson();

        if (readPersonResult.isEmpty()) {
            return;
        }

        Person person = readPersonResult.get();

        if (!meeting.personIsParticipant(person)) {
            inputError("This person is not a participant in the meeting!");
            return;
        }

        meeting.deleteParticipant(person);
        out.println("Participant " + person.getLastName() + " deleted");

    }

    private void delete_schedule(){
        
        roomList.clear();

        out.println("All meetings deleted");
    }

    private void delete_group(){
                                       
        if (roomList.stream().allMatch( r -> r.getMeetingCount() == 0)) {
            peopleList.clear();
        
            out.println("All persons deleted");

        } else {
            inputError("Cannot clear people list unless there are no meetings!");
        }

    }

    private void delete_all(){

        roomList.clear();

        peopleList.clear();
        
        out.println("All rooms and meetings deleted");
    }

    private void save_data(){

        String filename = in.next();

        try (PrintStream outFile = new PrintStream(
            Files.newOutputStream(Paths.get(filename)))) {

            outFile.println(peopleList.size());

            peopleList.forEach(outFile::println);

            outFile.println(roomList.size());

            roomList.forEach(r -> r.save(outFile));
            
            out.println("Data saved");

        } catch(IOException ioe) {

            throw new RuntimeException(ioe);
        }

    }

    private void load_data(){

        String filename = in.next();
        
        Charset cs = StandardCharsets.UTF_8;

        try (BufferedReader in = Files.newBufferedReader(Paths.get(filename), cs)) {

            String line = in.readLine();

            int peopleListCount = Integer.parseInt(line);

            for (int i = 0; i < peopleListCount; i++) {
                String[] personInfo = in.readLine().split(" ");

                Person newPerson = new Person(personInfo[0], personInfo[1], personInfo[2]);

                peopleList.add(newPerson);
                
            }

            line = in.readLine();

            int roomListCount = Integer.parseInt(line);

            for (int i = 0; i < roomListCount; i++) {

                Optional<Room> newRoom = Room.loadRoom(in, peopleList);

                newRoom.ifPresent(roomList::add);

            }

            if (roomList.size() != roomListCount || peopleList.size() != peopleListCount) {
                
                inputError("Invalid data found in file!");
                return;
            }

            out.println("Data loaded");

        } catch (NoSuchFileException f) {

            inputError("Could not open file!");

        } catch (NumberFormatException e) {

            inputError("Invalid data found in file!");
        
        } catch (IOException ioe) {
            
            throw new RuntimeException(ioe);
        }

    }

}
            

            
        
  