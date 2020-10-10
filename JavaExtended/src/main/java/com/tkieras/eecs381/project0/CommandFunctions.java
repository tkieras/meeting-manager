package com.tkieras.eecs381.project0;

import java.util.*;
import java.io.*;
import java.nio.file.*;
import java.nio.charset.*;

import java.util.stream.*;


class CommandFunctions {


    public static WithMessage<MeetingManager> applyCommand(MeetingManager mm, String command) {

        List<String> commandList = List.of(command.split(" "));


        List<WithMessage<MeetingManager>> stagedResults = new ArrayList<>();

        stagedResults.add(quitCommand(mm, commandList));
        stagedResults.add(addIndividualCommand(mm, commandList));
        stagedResults.add(printIndividualCommand(mm, commandList));
        stagedResults.add(deleteIndividualCommand(mm, commandList));

        stagedResults.add(printGroupCommand(mm, commandList));
        stagedResults.add(deleteGroupCommand(mm, commandList));

        stagedResults.add(addRoomCommand(mm, commandList));
        stagedResults.add(deleteRoomCommand(mm, commandList));
        stagedResults.add(printRoomCommand(mm, commandList));

        stagedResults.add(printScheduleCommand(mm, commandList));
        stagedResults.add(deleteScheduleCommand(mm, commandList));

        stagedResults.add(deleteAllCommand(mm, commandList));
        stagedResults.add(printAllocationCommand(mm, commandList));

        stagedResults.add(addMeetingCommand(mm, commandList));
        stagedResults.add(printMeetingCommand(mm, commandList));
        stagedResults.add(deleteMeetingCommand(mm, commandList));
        stagedResults.add(rescheduleMeetingCommand(mm, commandList));

        stagedResults.add(addParticipantCommand(mm, commandList));
        stagedResults.add(deleteParticipantCommand(mm, commandList));

        stagedResults.add(saveDataCommand(mm, commandList));
        stagedResults.add(loadDataCommand(mm, commandList));
        
        MeetingManager manager = mm;

        String message = "Unrecognized command";

        for (var stagedResult : stagedResults) {

            if (stagedResult.hasObject()) {
                manager = stagedResult.getObject();

            }

            if (stagedResult.hasMessage()) {
                message = stagedResult.getMessage();
            }

        }
       

        return new WithMessage<MeetingManager>(manager, message);

    }

    private static WithMessage<MeetingManager> quitCommand(MeetingManager mm, List<String> commandList) {
        
        String commandString = "qq";
        Integer neededCommands = 1;
        String successMessage = "All rooms and meetings deleted\nDone";

        WithMessage<MeetingManager> result = new WithMessage<>();

        if (commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            
            MeetingManager newMeetingManager = MeetingManager.initializeEmptyMeetingManager();

            result = new WithMessage<>(newMeetingManager, successMessage);
            
        }

        return result;

    }

    private static WithMessage<MeetingManager> addIndividualCommand(MeetingManager mm, List<String> commandList) {
        
        String commandString = "ai";
        Integer neededCommands = 4;
        String successMessage = "Person %s added";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            
            WithMessage<Person> person = getOpenPerson(mm, commandList.get(2));

            result = result.collectMessage(person);

            if (!result.hasMessage()) {

                result = new WithMessage<>(MeetingManager.addIndividual(mm, commandList.get(1), commandList.get(2), commandList.get(3)), 
                                    String.format(successMessage, commandList.get(2)));
            }

        }


        return result;

    }

    private static WithMessage<MeetingManager> deleteIndividualCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "di";
        Integer neededCommands = 2;
        String successMessage = "Person %s deleted";
        String errorMessage = "This person is a participant in a meeting!";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {


            WithMessage<Person> person = getExistingPerson(mm, commandList.get(1));

            result = result.collectMessage(person);

            if (!result.hasMessage()) {
            	
            	if (mm.getRoomList().stream().anyMatch( r -> r.personIsParticipantInRoom(person.getObject()))) {

            		result = new WithMessage<>(errorMessage);
            	}
            }

            if (!result.hasMessage()) {

                Person p = person.getObject();

                result = new WithMessage<>(MeetingManager.deleteIndividual(mm, p), 
                                            String.format(successMessage, p.getLastName()));
            }
            

        }

        return result;

    }

    private static WithMessage<MeetingManager> printIndividualCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "pi";
        Integer neededCommands = 2;

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            WithMessage<Person> person = getExistingPerson(mm, commandList.get(1));

            result = result.collectMessage(person);

            if (!result.hasMessage()) {

                result = new WithMessage<>(person.getObject().print());
            }
            
        }

        return result;

    }


    private static WithMessage<MeetingManager> printGroupCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "pg";
        Integer neededCommands = 1;
        String errorMessage = "List of people is empty";
        String successMessage = "Information for %d people:\n%s"; 

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            String message;

            if (mm.getPeopleList().isEmpty()) {

                message = errorMessage;

            } else {

                message = String.format(successMessage, mm.getPeopleList().size(),
                             mm.getPeopleList().stream()
                            .map(p -> p.print())
                            .collect(Collectors.joining("\n")));
            }

            result = new WithMessage<>(message);

        }

        return result;
    }


    private static WithMessage<MeetingManager> deleteGroupCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "dg";
        Integer neededCommands = 1;
        String errorMessage = "Cannot clear people list unless there are no meetings!";
        String successMessage = "All persons deleted";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            if(mm.getRoomList().stream().anyMatch( r -> r.getMeetingCount() > 0)) {
                result = new WithMessage<>(errorMessage);

            } else {

                result = new WithMessage<>(MeetingManager.deleteGroup(mm), successMessage);
            }
        }

        return result;
    }

    private static WithMessage<MeetingManager> addRoomCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "ar";
        Integer neededCommands = 2;
        String successMessage = "Room %d added!";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            
            WithMessage<Room> room = getOpenRoom(mm, commandList.get(1));

            result = result.collectMessage(room);

            if (!result.hasMessage()) {
                
                WithMessage<Integer> number = getRoomNumber(commandList.get(1));

                result = new WithMessage<>(MeetingManager.addRoom(mm, number.getObject()), 
                                        String.format(successMessage, number.getObject()));
            }

        }

        return result;

    }

    private static WithMessage<MeetingManager> deleteRoomCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "dr";
        Integer neededCommands = 2;
        String successMessage = "Room %d deleted";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            WithMessage<Room> room = getExistingRoom(mm, commandList.get(1));

            result = result.collectMessage(room);

            if (!result.hasMessage()) {

                result = new WithMessage<>(MeetingManager.deleteRoom(mm, room.getObject()), 
                                        String.format(successMessage, room.getObject().getNumber()));
            }

        }

        return result;

    }

    private static WithMessage<MeetingManager> printRoomCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "pr";
        Integer neededCommands = 2;


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            WithMessage<Room> room = getExistingRoom(mm, commandList.get(1));

            result = result.collectMessage(room);

            if (!result.hasMessage()) {
                
                result = new WithMessage<>(room.getObject().print());
            }

        }

        return result;

    }
   

    private static WithMessage<MeetingManager> printScheduleCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "ps";
        Integer neededCommands = 1;

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            String message = mm.getRoomList().isEmpty() 
                            ? "List of rooms is empty"
                            : "Information for " + mm.getRoomList().size() + " rooms:\n" + 
                             mm.getRoomList().stream()
                            .map(r -> r.print())
                            .collect(Collectors.joining("\n"));

            result = new WithMessage<>(message);


        }

        return result;
    }




    private static WithMessage<MeetingManager> deleteScheduleCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "ds";
        Integer neededCommands = 1;
        String successMessage = "All meetings deleted";

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            
            result = new WithMessage<>(MeetingManager.deleteSchedule(mm), successMessage);
               
        }

        return result;
    }


    private static WithMessage<MeetingManager> deleteAllCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "da";
        Integer neededCommands = 1;
        String successMessage = "All rooms and meetings deleted";

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            
            MeetingManager newMeetingManager = MeetingManager.initializeEmptyMeetingManager();

            result = new WithMessage<>(newMeetingManager, successMessage);
               
        }

        return result;
    }

    private static WithMessage<MeetingManager> printAllocationCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "pa";
        Integer neededCommands = 1;

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            Integer meetingCount = mm.getRoomList().stream()
                                              .map(Room::getMeetingCount)
                                              .reduce(0, Integer::sum);
    
            String message = "Memory allocations:\n";
        
            message += "Persons: " + mm.getPeopleList().size() + "\n"; 
        
            message += "Meetings: " + meetingCount + "\n";
        
            message += "Rooms: " + mm.getRoomList().size();

            result = new WithMessage<>(message);

        }
        return result;
    }

    private static WithMessage<MeetingManager> addMeetingCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "am";
        Integer neededCommands = 4;
        String successMessage = "Meeting added at %d";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            
            WithMessage<Meeting> meeting = getOpenMeetingTime(mm, commandList.get(1), commandList.get(2));

            result = result.collectMessage(meeting);

            if (!result.hasMessage()) {

                Room r = getExistingRoom(mm, commandList.get(1)).getObject();

                Integer t = getTime(commandList.get(2)).getObject();

                result = new WithMessage<>(MeetingManager.addMeeting(mm, r, commandList),
                                            String.format(successMessage, t));

            }

        }

        return result;
    }

    private static WithMessage<MeetingManager> deleteMeetingCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "dm";
        Integer neededCommands = 3;
        String successMessage = "Meeting at %d deleted";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            WithMessage<Meeting> meeting = getExistingMeeting(mm, commandList.get(1), commandList.get(2));

            result = result.collectMessage(meeting);

            if (!result.hasMessage()) {
                
                Room r = getExistingRoom(mm, commandList.get(1)).getObject();

                Integer t = meeting.getObject().getTime();

                result = new WithMessage<>(MeetingManager.deleteMeeting(mm, r, t),
                                            String.format(successMessage, t));

            }

        }

        return result;
    }
    
    private static WithMessage<MeetingManager> printMeetingCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "pm";
        Integer neededCommands = 3;

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
        
            WithMessage<Meeting> meeting = getExistingMeeting(mm, commandList.get(1), commandList.get(2));

            result = result.collectMessage(meeting);

            if (!result.hasMessage()) {
    
                result = new WithMessage<>(meeting.getObject().print());

            }

        }

        return result;
    }

   
    private static WithMessage<MeetingManager> rescheduleMeetingCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "rm";
        Integer neededCommands = 5;
        String successMessage = "Meeting rescheduled to room %d at %d";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            WithMessage<Room> srcRoom = getExistingRoom(mm, commandList.get(1));
            WithMessage<Meeting> srcMeeting = getExistingMeeting(mm, commandList.get(1), commandList.get(2));
            WithMessage<Room> dstRoom = getExistingRoom(mm, commandList.get(3));
            WithMessage<Meeting> dstMeeting = getOpenMeetingTime(mm, commandList.get(3), commandList.get(4));

            result = result.collectMessage(srcRoom);
            result = result.collectMessage(srcMeeting);
            result = result.collectMessage(dstRoom);
            result = result.collectMessage(dstMeeting);

            if (!result.hasMessage()) {

                Integer dstTime = getTime(commandList.get(4)).getObject();

                Room updatedSrcRoom = Room.deleteMeeting(srcRoom.getObject(), srcMeeting.getObject());

                Meeting updatedMeeting = Meeting.updateTime(srcMeeting.getObject(), dstTime);

                Room dstRoomObj;

                if (srcRoom.getObject().equals(dstRoom.getObject())) {

                    dstRoomObj = updatedSrcRoom;
                
                } else {

                    dstRoomObj = dstRoom.getObject();
                }

                Room updatedDstRoom = Room.addMeeting(dstRoomObj, updatedMeeting);

                MeetingManager updatedMeetingManager = MeetingManager.updateRoom(mm, srcRoom.getObject(), updatedSrcRoom);
                
                updatedMeetingManager = MeetingManager.updateRoom(updatedMeetingManager, dstRoomObj, updatedDstRoom);

                result = new WithMessage<>(updatedMeetingManager, 
                                            String.format(successMessage, updatedDstRoom.getNumber(), dstTime));

            }            

        }
        return result;
    }


    private static WithMessage<MeetingManager> addParticipantCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "ap";
        Integer neededCommands = 4;
        String successMessage = "Participant %s added";
        String errorMessage = "This person is already a participant!";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            WithMessage<Meeting> meeting = getExistingMeeting(mm, commandList.get(1), commandList.get(2));

            WithMessage<Person> person = getExistingPerson(mm, commandList.get(3));

            result = result.collectMessage(meeting);
            result = result.collectMessage(person);

            
            if (!result.hasMessage() && meeting.getObject().personIsParticipant(person.getObject())) {

                result = new WithMessage<>(errorMessage);

            }

            if (!result.hasMessage()) {

                Room r = getExistingRoom(mm, commandList.get(1)).getObject();

                result = new WithMessage<>(MeetingManager.addParticipant(mm, r, meeting.getObject(), person.getObject()), 
                                        String.format(successMessage, person.getObject().getLastName()));

            }
        }

        return result;
    }

    private static WithMessage<MeetingManager> deleteParticipantCommand(MeetingManager mm, List<String> commandList) {

        String commandString = "dp";
        Integer neededCommands = 4;
        String successMessage = "Participant %s deleted";
        String errorMessage = "This person is not a participant in the meeting!";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {

            WithMessage<Meeting> meeting = getExistingMeeting(mm, commandList.get(1), commandList.get(2));

            WithMessage<Person> person = getExistingPerson(mm, commandList.get(3));

            result = result.collectMessage(meeting);
            result = result.collectMessage(person);

            if (!result.hasMessage() && !meeting.getObject().personIsParticipant(person.getObject())) {

                result = new WithMessage<>(errorMessage);

            } 

            if (!result.hasMessage()) {

                Room r = getExistingRoom(mm, commandList.get(1)).getObject();

                result = new WithMessage<>(MeetingManager.deleteParticipant(mm, r, meeting.getObject(), person.getObject()), 
                                        String.format(successMessage, person.getObject().getLastName()));

            }
        }

        return result;
    }


    private static WithMessage<MeetingManager> saveDataCommand(MeetingManager mm, List<String> commandList) {
        String commandString = "sd";
        Integer neededCommands = 2;
        String successMessage = "Data saved";

        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            String filename = commandList.get(1);

            try(PrintStream outFile = new PrintStream(
                Files.newOutputStream(Paths.get(filename)))) {

                outFile.println(mm.getPeopleList().size());

                mm.getPeopleList().stream().forEach(p -> outFile.println(p.print()));

                outFile.println(mm.getRoomList().size());

                mm.getRoomList().stream().forEach(r -> r.save(outFile));
                
                result = new WithMessage<>(successMessage);

            } catch(IOException ioe) {

                throw new RuntimeException(ioe);
            }
        }

        return result;
    }

    private static WithMessage<MeetingManager> loadDataCommand(MeetingManager mm, List<String> commandList) {
        String commandString = "ld";
        Integer neededCommands = 2;
        String successMessage = "Data loaded";
        String errorMessage = "Invalid data found in file!";
        String noFileMessage = "Could not open file!";


        WithMessage<MeetingManager> result = new WithMessage<>();

        if(commandList.get(0).equals(commandString) && commandList.size() == neededCommands) {
            String filename = commandList.get(1);
        
            Charset cs = StandardCharsets.UTF_8;

            try(BufferedReader in = Files.newBufferedReader(
                Paths.get(filename), cs)) {

                String line = in.readLine();

                Integer peopleListCount = Integer.parseInt(line);

                List<Person> peopleList = new ArrayList<>();

                for(int i = 0; i < peopleListCount; i++) {
                    String[] personInfo = in.readLine().split(" ");

                    Person newPerson = new Person(personInfo[0], personInfo[1], personInfo[2]);

                    peopleList.add(newPerson);
                    
                }

                line = in.readLine();

                Integer roomListCount = Integer.parseInt(line);

                List<Room> roomList = new ArrayList<>();

                for(int i = 0; i < roomListCount; i++) { 

                    Optional<Room> newRoom = Room.loadRoom(in, peopleList);

                    newRoom.ifPresent((r) -> roomList.add(r));

                }

                if(roomList.size() == roomListCount && peopleList.size() == peopleListCount) {
                    
                    result = new WithMessage<>(MeetingManager.initializeFullMeetingManager(peopleList, roomList), 
                                                successMessage);

                } else {

                    result = new WithMessage<>(errorMessage);
                }

            }catch(NoSuchFileException f) {

                result = new WithMessage<>(noFileMessage);

            }catch(NumberFormatException e) {

                result = new WithMessage<>(errorMessage);
            
            }catch(IOException ioe) {
                
                throw new RuntimeException(ioe);
            }

        }

        return result;
    }

    private static WithMessage<Person> getOpenPerson(MeetingManager mm, String arg) {
        String errorMessage = "There is already a person with this last name!";

        WithMessage<Person> result;

        if (mm.personExists(arg)) {

            result = new WithMessage<>(mm.getPerson(arg), errorMessage);
        
        } else {
        
            result = new WithMessage<>();
        
        }

        return result;

    }

    private static WithMessage<Person> getExistingPerson(MeetingManager mm, String arg) {
        String errorMessage = "No person with that name!";

        WithMessage<Person> result;

        if (mm.personExists(arg)) {

            result = new WithMessage<>(mm.getPerson(arg));
        
        } else {
        
            result = new WithMessage<>(errorMessage);
        
        }

        return result;

    }

    private static WithMessage<Integer> getInteger(String arg) {
        String errorMessage = "Could not read an integer value!";


        WithMessage<Integer> result;

        try {
            Integer number = Integer.parseInt(arg);

            result = new WithMessage<>(number);
        
        } catch (NumberFormatException nfe) {

            result = new WithMessage<>(errorMessage);

        }

        return result;
    }

    private static WithMessage<Integer> getRoomNumber(String arg) {
        String errorMessage = "Room number is not in range!";


        WithMessage<Integer> result;

        result = getInteger(arg);

        if(result.hasObject() && !(Room.roomNumberInRange(result.getObject()))) {

            result = new WithMessage<>(errorMessage);
        }

        return result;
    }

    private static WithMessage<Room> getExistingRoom(MeetingManager mm, String arg) {
        String errorMessage = "No room with that number!";


        WithMessage<Room> result;

        WithMessage<Integer> number = getRoomNumber(arg);

        if (number.hasObject() && !(mm.roomExists(number.getObject()))) {
        
            result = new WithMessage<>(errorMessage);
        
        } else if (number.hasObject() && mm.roomExists(number.getObject())) {
            
            result = new WithMessage<>(mm.getRoom(number.getObject()));
        
        } else {

            result = new WithMessage<>(number.getMessage());
        }

        return result;
    }

    private static WithMessage<Room> getOpenRoom(MeetingManager mm, String arg) {
        String errorMessage = "There is already a room with this number!";


        WithMessage<Room> result;

        WithMessage<Integer> number = getRoomNumber(arg);

        if (number.hasObject() && mm.roomExists(number.getObject())) {

            result = new WithMessage<>(mm.getRoom(number.getObject()), errorMessage);
        
        } else if (number.hasObject() && !(mm.roomExists(number.getObject()))) {

            result = new WithMessage<>();
        
        } else {

            result = new WithMessage<>(number.getMessage());
        }

        return result;
    }



    private static WithMessage<Integer> getTime(String arg) {
        String errorMessage = "Time is not in range!";


        WithMessage<Integer> result;

        result = getInteger(arg);

        if (!result.hasMessage() && !(Meeting.isValidTime(result.getObject()))) {
        
            result = new WithMessage<>(errorMessage);
        
        } 

        return result;
    }

    private static WithMessage<Meeting> getExistingMeeting(MeetingManager mm, String arg1, String arg2) {
        String errorMessage = "No meeting at that time!";

        WithMessage<Meeting> result = new WithMessage<>();
        
        WithMessage<Room> room = getExistingRoom(mm, arg1);

        WithMessage<Integer> time = getTime(arg2);

        result = result.collectMessage(room);
        result = result.collectMessage(time);

        
        if (!result.hasMessage() && !room.getObject().meetingAtTime(time.getObject())) {

            result = new WithMessage<>(errorMessage);

        } 

        if (!result.hasMessage()) {

            result = new WithMessage<>(room.getObject().getMeeting(time.getObject()));
        }

        return result;
    }

    private static WithMessage<Meeting> getOpenMeetingTime(MeetingManager mm, String arg1, String arg2) {
        String errorMessage = "There is already a meeting at that time!";

        WithMessage<Meeting> result = new WithMessage<>();
        
        WithMessage<Room> room = getExistingRoom(mm, arg1);

        WithMessage<Integer> time = getTime(arg2);

        result = result.collectMessage(room);
        result = result.collectMessage(time);

        
        if (!result.hasMessage() && room.getObject().meetingAtTime(time.getObject())) {
        
            result = new WithMessage<>(room.getObject().getMeeting(time.getObject()), errorMessage);

        } 

        if (!result.hasMessage()) {
        
            result = new WithMessage<>();
        }
        
        return result;
    }


}