#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
#include "user.h"
#include "network.h"

using namespace std;

/*
This implementation of social_network.cpp has added options 7,8,9 and 10 which are all features that
utilise different searching mechanisms.
*/

//function to run a social network simulator
//pre: none
//post: output file is created if user asked for one
void runNetwork() { //runs the program
    bool running = true;
    size_t id=0;
    Network N;

    cout<<endl<<"Welcome to the social network builder!"<<endl;
    cout<<"Enter 1 to use the prebuilt network. Enter anything else to start from scratch:"<<endl;
    char fileEntry;
    cin>>fileEntry;

    if (fileEntry == '1'){
        N.readUsers("social_network.txt"); //call network function to read in users
        cout<<endl<<"network built!"<<endl;
        
        id += N.getSize();

        N.readPosts("posts.txt"); //call network function to read in posts
        cout<<endl<<"Posts Added!"<<endl;
         
    }

    else{
        cout<<endl<<"starting from scratch!"<<endl;
    }

    while(running == true){ //display the user their options
        cout<<endl<<"type 1 to add a user"<<endl;
        cout<<"type 2 to add a friend connection"<<endl;
        cout<<"type 3 to remove a friend connection"<<endl;
        cout<<"type 4 to print all users"<<endl;
        cout<<"type 5 to print all friends of a user"<<endl;
        cout<<"type 6 to write user information to file"<<endl;
        cout<<"type 7 to find the shortest friendship path between 2 students"<<endl;
        cout<<"type 8 to find a path to a user at a requested distance"<<endl;
        cout<<"type 9 to find suggested friends for a requested user"<<endl;
        cout<<"type 10 to get all sets of users"<<endl;
        cout<<"type 11 to view the most recent posts of a user"<<endl;
        cout<<"type 0 to close the program"<<endl;

        string userInput;
        cin>>userInput;

        if(userInput == "1"){ //Add a user
            string firstname;
            string lastname;
            string birthyear;
            string zip;
        
            cout<<endl<<"enter the first name, last name, birth year, and zip code on the same line "<<endl;
            cout<<1<<" ";
            cin>>firstname>>lastname>>birthyear>>zip;
            
            string fullname;
            fullname = firstname + " " + lastname;

            //create new user with the input information and add them to the network
            User* u = new User(id, fullname, stoul(birthyear), stoul(zip), vector<size_t> {});
            id++;
            N.addUser(u);
        }

        else if(userInput == "2"){ // add friend connection
            string fname1;
            string lname1;
            string fname2;
            string lname2;

            cout<<endl<<"enter the names of each person on one line ex: Joe Smith Katie Li "<<endl;
            cout<<2<<" ";
            cin>>fname1>>lname1>>fname2>>lname2;

            string person1;
            string person2;
            person1 = fname1 + " " + lname1;
            person2 = fname2 + " " + lname2;

            if(N.addConnection(person1,person2) == 0){ //adds connection between the users
                cout<<endl<<"friendship added successfully!"<<endl<<endl;
            }
        }

        else if(userInput == "3"){ // remove friend connection
            string fname1;
            string lname1;
            string fname2;
            string lname2;

            cout<<endl<<"enter the names of each person on one line ex: Joe Smith Katie Li "<<endl;
            cout<<3<<" ";
            cin>>fname1>>lname1>>fname2>>lname2;

            string person1;
            string person2;
            person1 = fname1 + " " + lname1;
            person2 = fname2 + " " + lname2;

            if(N.removeConnection(person1,person2) == 0){ //removes connection from the users
                cout<<endl<<"friendship removed successfully"<<endl<<endl;
            }
        }

        else if(userInput == "4"){ // print users
            cout<<endl<<"ID   Name        Year   Zip   "<<endl<<"==============================="<<endl;
            for(size_t i = 0; i < N.getSize(); i++){ //iterate over the network
                
                string adjustedZip;
                adjustedZip = to_string(N.getUser(i)->getZip());
                for(size_t i = 0; i < 5-adjustedZip.length(); i++){ //add leading 0's to the zip that were lost
                    adjustedZip.insert(adjustedZip.begin(),'0');
                }

                //output the user's information to the terminal
                cout<<i<<" "<<N.getUser(i)->getName()<<" "<<N.getUser(i)->getBirth()<<" "<<adjustedZip<<endl;
            }
            cout<<endl;
        }

        else if(userInput == "5"){ //print friends
            string fname;
            string lname;
            
            cout<<"enter the name of the student whose friends list you want to see"<<endl;
            cout<<5<<" ";
            cin>>fname>>lname;

            string person;
            person = fname + " " + lname;

            cout<<endl<<"ID   Name        Year   Zip   "<<endl<<"==============================="<<endl;
            for(auto e: N.getUser(N.getId(person))->getFriends()){

                string adjustedZip;
                adjustedZip = to_string(N.getUser(e)->getZip());
                for(size_t i = 0; i < 5-adjustedZip.length(); i++){ //add leading 0's to the zip that were lost
                    adjustedZip.insert(adjustedZip.begin(),'0');
                }

                //output friend's information to the terminal
                cout<<e<<" "<<N.getUser(e)->getName()<<" "<<N.getUser(e)->getBirth()<<" "<<adjustedZip<<endl;
            }
            cout<<endl;
        }

        else if(userInput == "6"){ //write to file
            string filename;
            cout<<"enter your filename"<<endl;
            cout<<6<<" ";
            cin>>filename;

            //creates and writes the output file
            N.writeUsers(filename);
        }

        else if(userInput == "7"){ //shortest path
            string fname1;
            string lname1;
            string fname2;
            string lname2;

            cout<<endl<<"enter the names of each person on one line ex: Joe Smith Katie Li "<<endl;
            cout<<7<<" ";
            cin>>fname1>>lname1>>fname2>>lname2;

            string person1;
            string person2;
            person1 = fname1 + " " + lname1;
            person2 = fname2 + " " + lname2;

            //call shortest path function and set the vector to paths
            vector<size_t> paths = N.shortestPath(N.getId(person1),N.getId(person2));
            if(paths[0]==-2){ //no path was found between the 2 people
                cout<<endl<<"None"<<endl;
                continue;
            }
            if(paths[0]==-3){ // the 2 people are the same
                cout<<endl<<"distance: 0"<<endl;
                cout<<person1<<endl;
                continue;
            }

            cout<<endl;
            cout<<"distance: "<<paths.size()-1<<endl;
            for(auto e: paths){ // display the path from person 1 to person 2
                cout<<N.getUser(e)->getName();
                if(N.getUser(e)->getName() != person2){
                    cout<<" -> ";
                }
            }
            cout<<endl;
        }

        else if(userInput == "8"){ // user at distance x
            string fname1;
            string lname1;
            string dist;

            cout<<endl<<"enter the name of the person and the distance you want to explore. Ex: Katie Li 4"<<endl;
            cout<<8<<" ";
            cin>>fname1>>lname1>>dist;

            string person1;
            person1 = fname1 + " " + lname1;
            std::size_t distance;
            distance = stoul(dist);
            std::size_t to;

            //call the distance user function and set the returned vector to paths
            vector<size_t> paths = N.distanceUser(N.getId(person1),to, distance);

            if(to == -1){ //means that no users were found
                cout<<endl<<"No users were found at this distance"<<endl;
                continue;
            }

            string toName;
            toName = N.getUser(to)->getName();
            cout<<endl<<toName<<": ";
            for(auto e: paths){ //print the path to the person at distance x
                cout<<N.getUser(e)->getName();
                if(N.getUser(e)->getName() != toName){
                    cout<<", ";
                }
            }
            cout<<endl;
        }

        else if(userInput == "9"){ //find suggested friends
            string fname1;
            string lname1;

            cout<<endl<<"enter the name of the person for their suggested friends"<<endl;
            cout<<9<<" ";
            cin>>fname1>>lname1;

            string person1;
            person1 = fname1 + " " + lname1;
            std::size_t score;

            //call suggest friends function and set returned vector to suggestedFriends vector
            vector<size_t> suggestedFriends = N.suggestFriends(N.getId(person1), score);

            if(score == -1){ //means there are no suggested friends
                cout<<endl<<"None"<<endl;
                continue;
            }
            cout<<endl<<"The suggested friend(s) is/are: "<<endl;
            for(auto e: suggestedFriends){ //print the suggested friends
                cout<<N.getUser(e)->getName()<<" Score: "<<score<<endl;
            }
        }

        else if(userInput == "10"){ //Get a list containing all specific sets of users
            vector<vector<size_t>> groups;
            groups = N.groups();
            size_t counter = 1;

            cout<<endl;
            for(auto e: groups){ //iterate through the groups
                cout<<"Set "<<counter++<<" => ";
                for(std::size_t i = 0; i<e.size(); i++){ //iterate through each user in the group
                    cout<<N.getUser(e[i])->getName(); //print the user's name
                    if(i!=e.size()-1){
                        cout<<", ";
                    }
                }
                cout<<endl;
            }
        }

        else if(userInput == "11") { //print a certain amount of posts of a user
            string fname1;
            string lname1;
            string count;

            cout<<endl<<"enter the name of the person and amount of posts you want to see. Ex: Katie Li 4"<<endl;
            cout<<11<<" ";
            cin>>fname1>>lname1>>count;

            string person1;
            person1 = fname1 + " " + lname1;
            std::size_t Count;
            Count = stoul(count);

            string posts = N.displayPosts(person1,Count,false); // gets the posts from person 1
            cout<<endl<<posts;
        }

        else if (userInput == "0"){
            cout<<endl<<"exiting program... "<<endl;
            running = false;
        }
    }
}

int main(){
    runNetwork();
    return 0;
}