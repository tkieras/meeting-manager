#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Person{
public:
	
	Person(const string& fn, const string& ln, const string& pn): first_name{fn}, last_name{ln}, phone_number{pn}{};
	Person(){}; //for loading

	string get_last_name() const {return last_name;}

	friend ostream& operator<< (ostream& os, const Person& person);

	void save(ostream& os);
	void load(ifstream& is);

private:
	string first_name;
	string last_name;
	string phone_number;
};

class Meeting{
public:
	
	Meeting(int t, const string& tp): time{t}, topic{tp}{ convert_time_to_24hr(); };
	Meeting(){}; // for load

	int get_time() const {return time;}
	int get_time_12hr() const;
	void set_time(int new_time);

	bool check_Person_is_participant(const Person* person) const;

	friend ostream& operator<<(ostream& os, const Meeting& meeting);

	void add_participant(Person* new_participant);
	void delete_participant(vector<Person*>::iterator found_person);

	void save(ostream& os);
	void load(ifstream& is, vector<Person*>& people_list);

private:

	int time; // 24 hr time
	string topic;
	vector<Person*> participant_list;

	void convert_time_to_24hr(){
		//precondition: time is valid (9am to 5 pm)
		if(time <= 5 && time > 0)
			time += 12;
	}
};

class Room{
public:
	
	Room(int n): number{n}{};
	Room(){}; // for load

	int get_number() const { return number; }

	friend ostream& operator<<(ostream& os, const Room& room);

	void add_meeting(Meeting& new_meeting);
	void delete_meeting(vector<Meeting>::iterator found_meeting);

	vector<Meeting>::iterator get_meeting_itr_from_input();
	int get_meeting_tally() const { return meeting_list.size(); }

	bool check_Person_is_participant(const Person* person) const;

	void save(ostream& os);
	void load(ifstream& is, vector<Person*>& people_list);

private:
	int number;
	vector<Meeting> meeting_list;

};


//top level command functions
void print_individual(vector<Person*>& people_list);
void print_room(vector<Room>& room_list);
void print_meeting(vector<Room>& room_list);
void print_schedule(vector<Room>& room_list);
void print_group(vector<Person*>& people_list);
void print_allocation(vector<Person*>& people_list, vector<Room>& room_list);

void add_individual(vector<Person*>& people_list);
void add_room(vector<Room>& room_list);
void add_meeting(vector<Room>& room_list);
void add_participant(vector<Person*>& people_list, vector<Room>& room_list);

void reschedule_meeting(vector<Room>& room_list);

void delete_individual(vector<Person*>& people_list, vector<Room>& room_list);
void delete_room(vector<Room>& room_list);
void delete_meeting(vector<Room>& room_list);
void delete_participant(vector<Person*>& people_list, vector<Room>& room_list);
void delete_schedule(vector<Room>& room_list);
void delete_group(vector<Person*>& people_list, vector<Room>& room_list);
void delete_all(vector<Person*>& people_list, vector<Room>& room_list);

void save_data(vector<Person*>& people_list, vector<Room>& room_list);

void load_data(vector<Person*>& people_list, vector<Room>& room_list);

//midlevel helper functions

vector<Person*>::iterator get_person_itr_from_input(vector<Person*>& people_list);
vector<Room>::iterator get_room_itr_from_input(vector<Room>& room_list);

//bottom level helper functions
void error(const string& str);
bool time_range(const int time);

int main(){
	
	vector<Person*> people_list;
	vector<Room> room_list;

	char c1;
	char c2;

	while(true){
		cout<<"\nEnter command: ";
		cin>>c1>>c2;
		try{
			switch(c1) {
				case 'p':
					switch(c2){
						case 'i': print_individual(people_list); break;
						case 'r': print_room(room_list); break;
						case 'm': print_meeting(room_list); break;
						case 's': print_schedule(room_list); break;
						case 'g': print_group(people_list); break;
						case 'a': print_allocation(people_list, room_list); break;
						default:
							error("Unrecognized command!");
					}
				break;
				case 'a':
					switch(c2){
						case 'i': add_individual(people_list); break;
						case 'r': add_room(room_list); break;
						case 'm': add_meeting(room_list); break;
						case 'p': add_participant(people_list, room_list); break;
						default:
							error("Unrecognized command!");
					}
				break;
				case 'r':
					switch(c2){
						case 'm': reschedule_meeting(room_list); break;
						default:
							error("Unrecognized command!");
					}
				break;
				case 'd':
					switch(c2){
						case 'i': delete_individual(people_list, room_list); break;
						case 'r': delete_room(room_list); break;
						case 'm': delete_meeting(room_list); break;
						case 'p': delete_participant(people_list, room_list); break;
						case 's': delete_schedule(room_list); break;
						case 'g': delete_group(people_list, room_list); break;
						case 'a': delete_all(people_list, room_list); break;
						default:
							error("Unrecognized command!");
					}
				break;
				case 's':
					switch(c2){
						case 'd': save_data(people_list, room_list); break;
						default:
							error("Unrecognized command!");
					}
				break;
				case 'l':
					switch(c2){
						case 'd': load_data(people_list, room_list); break;
						default:
							error("Unrecognized command!");
					}
				break;
				case 'q':
					switch(c2){
						case 'q':
							delete_all(people_list, room_list);
				 			cout<<"Done"<<endl;
				 			return 0;
					default:
						error("Unrecognized command!");
					}
				break;
				default:
					error("Unrecognized command!");
			}
		}catch(runtime_error& e){
			cout<<e.what()<<endl;
			cin.clear();
			while(cin.get() != '\n');
		}
	}

	return 0;
}
//top level command functions
void print_individual(vector<Person*>& people_list){
	auto found_person = get_person_itr_from_input(people_list);

	cout<<**found_person;
}
void print_room(vector<Room>& room_list){
	auto found_room = get_room_itr_from_input(room_list);

	cout<<*found_room;
}
void print_meeting(vector<Room>& room_list){
	auto found_room = get_room_itr_from_input(room_list);
	auto found_meeting = (*found_room).get_meeting_itr_from_input();

	cout<<*found_meeting;
	
}
void print_schedule(vector<Room>& room_list){
	if(room_list.empty()){
		cout<<"List of rooms is empty"<<endl;
		return;
	}
	cout<<"Information for "<<room_list.size()<<" rooms:"<<endl;

	for(auto i : room_list)
		cout<<i;
}
void print_group(vector<Person*>& people_list){
	if(people_list.empty())
		error("List of people is empty");

	cout<<"Information for "<<people_list.size()<<" people:"<<endl;
	for(auto i : people_list)
		cout<<*i;
}
void print_allocation(vector<Person*>& people_list, vector<Room>& room_list){
	int person_tally = 0;
	int meeting_tally = 0;
	int room_tally = 0;

	person_tally = people_list.size();
	room_tally = room_list.size();

	for(auto i : room_list)
		meeting_tally += i.get_meeting_tally();

	cout<<"Memory allocations:"<<endl;
	cout<<"Persons: "<<person_tally<<endl; 
 	cout<<"Meetings: "<<meeting_tally<<endl;
	cout<<"Rooms: "<<room_tally<<endl;
}

void add_individual(vector<Person*>& people_list){
	string first_name;
	string last_name;
	string phone_number;

	cin>>first_name>>last_name>>phone_number;

	//check for existence and insert-in-order	

	for(auto i = people_list.begin(); i != people_list.end(); ++i){
		if((*i)->get_last_name() == last_name)
			error("There is already a person with this last name!");
		else if((*i)->get_last_name() > last_name){
			//insert 
			Person* new_person = new Person(first_name, last_name, phone_number);
			people_list.insert(i, new_person);
			cout<<"Person "<<new_person->get_last_name()<<" added"<<endl;	

			return;
		}
	}

	// if person belongs at 'end' (including 1st call to add_individual)
	Person* new_person = new Person(first_name, last_name, phone_number);
	people_list.push_back(new_person);
	cout<<"Person "<<new_person->get_last_name()<<" added"<<endl;	

}
void add_room(vector<Room>& room_list){
	int room_number;

	cin>>room_number;
	if(cin.fail())
		error("Could not read an integer value!");

	if(room_number <= 0)
		error("Room number is not in range!");

	//check for existence and insert-in-order

	for(auto i = room_list.begin(); i != room_list.end(); ++i){
		if((*i).get_number() == room_number)
			error("There is already a room with this number!");
		else if((*i).get_number() > room_number){
			Room new_room{room_number}; // don't need to use new, will copy to vector
			room_list.insert(i, new_room);
			cout<<"Room "<<new_room.get_number()<<" added"<<endl;
			return;
		}
	}

	//if room is the first to be added, or belongs at end of list
	Room new_room{room_number}; // don't need to use new, will copy to vector
	room_list.push_back(new_room);
	cout<<"Room "<<new_room.get_number()<<" added"<<endl;

}
void add_meeting(vector<Room>& room_list){
	auto found_room = get_room_itr_from_input(room_list);

	int time;
	cin>>time;

	if(cin.fail())
		error("Could not read an integer value!");

	if(!time_range(time))
		error("Time is not in range!");

	string topic;
	cin>>topic;

	Meeting new_meeting{time, topic};
	(*found_room).add_meeting(new_meeting);

	cout<<"Meeting added at "<<new_meeting.get_time_12hr()<<endl;

}
void add_participant(vector<Person*>& people_list, vector<Room>& room_list){
	auto found_room = get_room_itr_from_input(room_list);
	auto found_meeting = (*found_room).get_meeting_itr_from_input();	
	auto found_person = get_person_itr_from_input(people_list);
	
	(*found_meeting).add_participant(*found_person);
	cout<<"Participant "<<(*found_person)->get_last_name()<<" added"<<endl;

}

void reschedule_meeting(vector<Room>& room_list){
	auto old_found_room = get_room_itr_from_input(room_list);
	auto old_found_meeting = (*old_found_room).get_meeting_itr_from_input();

	auto new_found_room = get_room_itr_from_input(room_list);

	// new meeting
	int time;
	cin>>time;

	if(cin.fail())
		error("Could not read an integer value!");

	if(!time_range(time))
		error("Time is not in range!");

	//change time 
	(*old_found_meeting).set_time(time);

	//if need be, change room
	if((*old_found_room).get_number() != (*new_found_room).get_number()){
		(*new_found_room).add_meeting(*old_found_meeting);
		(*old_found_room).delete_meeting(old_found_meeting);
	}

	cout<<"Meeting rescheduled to room "<<(*new_found_room).get_number()<<" at "<<(*old_found_meeting).get_time_12hr()<<endl;

}

void delete_individual(vector<Person*>& people_list, vector<Room>& room_list){
	auto found_person = get_person_itr_from_input(people_list);

	//check if scheduled
	for(auto i : room_list){
		if(i.check_Person_is_participant(*found_person))
			error("This person is a participant in a meeting!");
		
	}

	cout<<"Person "<<(*found_person)->get_last_name()<<" deleted"<<endl;

	people_list.erase(found_person);
	delete (*found_person);

}
void delete_room(vector<Room>& room_list){
	auto found_room = get_room_itr_from_input(room_list);
	
	cout<<"Room "<<(*found_room).get_number()<<" deleted"<<endl;
	room_list.erase(found_room);

}
void delete_meeting(vector<Room>& room_list){
	auto found_room = get_room_itr_from_input(room_list);
	auto found_meeting = (*found_room).get_meeting_itr_from_input();

	cout<<"Meeting at "<<(*found_meeting).get_time_12hr()<<" deleted"<<endl;
	(*found_room).delete_meeting(found_meeting);

}
void delete_participant(vector<Person*>& people_list, vector<Room>& room_list){
	auto found_room = get_room_itr_from_input(room_list);
	auto found_meeting = (*found_room).get_meeting_itr_from_input();

	auto found_participant = get_person_itr_from_input(people_list);

	(*found_meeting).delete_participant(found_participant);
	cout<<"Participant "<<(*found_participant)->get_last_name()<<" deleted"<<endl;

}
void delete_schedule(vector<Room>& room_list){
	room_list.clear();
	cout<<"All meetings deleted"<<endl;
}
void delete_group(vector<Person*>& people_list, vector<Room>& room_list){
	for(auto i : room_list){
		if(i.get_meeting_tally() > 0)
			error("Cannot clear people list unless there are no meetings!");
	}
	for(auto i : people_list)
		delete i;

	people_list.clear();
	cout<<"All persons deleted"<<endl;
}
void delete_all(vector<Person*>& people_list, vector<Room>& room_list){
	room_list.clear();
	for(auto i : people_list)
		delete i;

	people_list.clear();

	cout<<"All rooms and meetings deleted"<<endl;
}

void save_data(vector<Person*>& people_list, vector<Room>& room_list){
	string filename;
	cin>>filename;
	ofstream output_file(filename);

	if(!output_file.is_open())
		error("Could not open file!");

	output_file<<people_list.size()<<endl;

	for(auto i : people_list){
		i->save(output_file);
	}

	output_file<<room_list.size()<<endl;

	for(auto i : room_list){
		i.save(output_file);
	}

	cout<<"Data saved"<<endl;
	output_file.close();

}

void load_data(vector<Person*>& people_list, vector<Room>& room_list){
	string filename;
	cin>>filename;

	ifstream input_file(filename);

	if(!input_file.is_open())
		error("Could not open file!");

	int person_tally;
	input_file>>person_tally;

	if(input_file.fail() || person_tally < 0)
		error("Invalid data found in file!");

	vector<Person*> new_people_list;
	for(int i=0; i<person_tally; ++i){
		Person* new_person = new Person;
		new_person->load(input_file);
		new_people_list.push_back(new_person);
	}

	int room_tally;
	input_file>>room_tally;

	if(input_file.fail() || room_tally < 0)
		error("Invalid data found in file!");

	vector<Room> new_room_list;

	for(int i=0; i<room_tally; ++i){
		Room new_room;
		new_room.load(input_file, new_people_list);
		new_room_list.push_back(new_room);
	}

	for(auto i : people_list){
		delete i;
	}

	people_list = new_people_list;
	room_list = new_room_list;

	input_file.close();
	cout<<"Data loaded"<<endl;	
}

//member functions & operators

void Person::save(ostream& os){
	os<<first_name<<" "<<last_name<<" "<<phone_number<<endl;
}
void Person::load(ifstream& is){
	is>>first_name>>last_name>>phone_number;
	if(is.eof())
		error("Invalid data found in file!");
}

ostream& operator<< (ostream& os, const Person& person){
	os<<person.first_name<<" ";
	os<<person.last_name<<" ";
	os<<person.phone_number<<endl;

	return os;
}

void Room::add_meeting(Meeting& new_meeting){

	for(auto i = meeting_list.begin(); i != meeting_list.end(); ++i){
		if((*i).get_time() == new_meeting.get_time())
			error("There is already a meeting at that time!");
		else if((*i).get_time() > new_meeting.get_time()){
			meeting_list.insert(i, new_meeting);
			return;
		}
	}

	meeting_list.push_back(new_meeting);
}

bool Room::check_Person_is_participant(const Person* person) const {
		for(auto i : meeting_list){
			if(i.check_Person_is_participant(person))
				return true;
		}
		return false;
}

void Room::delete_meeting(vector<Meeting>::iterator found_meeting){
	meeting_list.erase(found_meeting);

}

ostream& operator<<(ostream& os, const Room& room){
	os<<"--- Room "<<room.number<<" ---"<<endl;

	if(!room.meeting_list.empty()){
		for(auto i : room.meeting_list)
			os<<i;
	}
	else{
		os<<"No meetings are scheduled"<<endl;
	}

	return os;
}

vector<Meeting>::iterator Room::get_meeting_itr_from_input(){
	int time;

	cin>>time;

	if(cin.fail())
		error("Could not read an integer value!");

	if(!time_range(time))
		error("Time is not in range!");

	if(time <= 5 && time > 0)
		time += 12;

	for(auto i = meeting_list.begin(); i != meeting_list.end(); ++i){
		if(time == (*i).get_time())
			return i;
	}
	error("No meeting at that time!");
	return meeting_list.end(); // never executed

}

void Room::save(ostream& os){
	os<<number<<" "<<meeting_list.size()<<endl;

	for(auto i : meeting_list){
		i.save(os);
	}
}
void Room::load(ifstream& is, vector<Person*>& people_list){

	is>>number;
	if(is.fail() || is.eof())
		error("Invalid data found in file!");

	int meeting_tally;
	is>>meeting_tally;
	if(is.fail() || meeting_tally < 0)
		error("Invalid data found in file!");

	for(int i=0; i<meeting_tally; ++i){
		Meeting new_meeting;
		new_meeting.load(is, people_list);
		meeting_list.push_back(new_meeting);
	}

}

int Meeting::get_time_12hr() const {
	if(time <= 17 && time >= 13)
		return time - 12;
	else return time;
}

void Meeting::set_time(int new_time) {
	time = new_time;
	convert_time_to_24hr();
}

bool Meeting::check_Person_is_participant(const Person* person) const {
	for(const auto i : participant_list){
		if(person->get_last_name() == (*i).get_last_name())
			return true;
	}
	return false;
}


void Meeting::add_participant(Person* new_participant){
	for(auto i = participant_list.begin(); i != participant_list.end(); ++i){

		if((*i)->get_last_name() == new_participant->get_last_name())
			error("This person is already a participant!");
		else if((*i)->get_last_name() > new_participant->get_last_name()){	
			participant_list.insert(i, new_participant);
			return;
		}
	}
	participant_list.push_back(new_participant);
}
void Meeting::delete_participant(vector<Person*>::iterator found_person){
	for(auto i = participant_list.begin(); i != participant_list.end(); ++i){
		if((*i)->get_last_name() == (*found_person)->get_last_name()){
			participant_list.erase(i);
			return;
		}
	}
	error("This person is not a participant in the meeting!");
}

ostream& operator<<(ostream& os, const Meeting& meeting){

	os<<"Meeting time: "<<meeting.get_time_12hr()<<", Topic: "<<meeting.topic<<"\nParticipants:";

	if(meeting.participant_list.empty())
		os<<" None"<<endl;
	else{
		os<<endl;
		for(auto i : meeting.participant_list){
			os<<*i;
		}
	}

	return os;
}

void Meeting::save(ostream& os){
	
	os<<get_time_12hr()<<" "<<topic<<" "<<participant_list.size()<<endl;

	for(auto j : participant_list){
		os<<j->get_last_name()<<endl;
	}
}
void Meeting::load(ifstream& is, vector<Person*>& people_list){
	is>>time;
	if(is.fail() || is.eof())
		error("Invalid data found in file!");

	if(time <= 5 && time > 0)
		time += 12;

	is>>topic;
	int participant_tally;
	is>>participant_tally;
	if(is.fail())
		error("Invalid data found in file!");

	for(int i=0; i<participant_tally; ++i){
		string last_name;
		is>>last_name;
		if(is.eof())
			error("Invalid data found in file!");
		bool found_person = false;

		for(auto i = people_list.begin(); i != people_list.end(); ++i){
			if(last_name == (*i)->get_last_name()){
				found_person = true;
				participant_list.push_back(*i);
			}
				
		}
		if(!found_person)
			error("No person with that name!");
	}
}
//midlevel helper functions

vector<Person*>::iterator get_person_itr_from_input(vector<Person*>& people_list){
	string last_name;
	cin>>last_name;

	for(auto i = people_list.begin(); i != people_list.end(); ++i){
		if(last_name == (*i)->get_last_name())
			return i;
	}

	error("No person with that name!");
	return people_list.end(); // never executed
}

vector<Room>::iterator get_room_itr_from_input(vector<Room>& room_list){
	int room_number;
	cin>>room_number;

	if(cin.fail())
		error("Could not read an integer value!");

	if(room_number <= 0)
		error("Room number is not in range!");

	for(auto i = room_list.begin(); i != room_list.end(); ++i){
		if(room_number == (*i).get_number())
			return i;
	}

	error("No room with that number!");
	return room_list.end(); // never executed
}


//bottom level helper functions

void error(const string& str){
	throw runtime_error(str);
}
bool time_range(const int time){
	if((time >=9) && (time<=12))
		return true;
	else if((time >=1) && (time<5))
		return true;
	return false;
}