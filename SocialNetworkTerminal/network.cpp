#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include "network.h"

//gets the Id of a user from their name
//pre: user is the string name of the user
//post: the Id is returned of the user names user
std::size_t Network::getId(std::string user){
    for (auto e: Graph){
        if(e->getName()==user){
            return e->getId();
        }
    }
    return -1;
}

//gets the 
//pre: id is the size_t id of the user
//post: a pointer to the user is returned
User* Network::getUser(std::size_t id){
    for (auto e: Graph){
        if(e->getId()==id){
            return e;
        }
    }
    std::cout<<"No User found with that name/id... quitting"<<std::endl;
    exit(0);
}

//add user
//pre: newUser is a pointer pointing to the user we want to add
//post: a user is added to the network
void Network::addUser(User* newUser) {
    for (auto e: Graph){
        if(newUser->getName()==e->getName()){ //checks to determine if user being added already is in the network
            if(newUser->getBirth()==e->getBirth()){
                if(newUser->getZip()==e->getZip()){
                    std::cout<<"Cannot add the same user twice!"<<std::endl<<std::endl;
                    return;
                }
            }
        }
    }
    std::cout<<"User added!"<<std::endl<<std::endl;
    Graph.push_back(newUser);
}

//add connection between users
//pre: user1 and user2 are the names of the two users
//post: a connection is made between the 2 inputed users
std::size_t Network::addConnection(std::string user1, std::string user2){
    std::size_t id1, id2;

    //find the id's of the students
    id1 = getId(user1);
    id2 = getId(user2);

    //if an id wasn't found
    if(id1 == -1){
        std::cout<<user1<<" was not found"<<std::endl;
        return -1;
    }
    if(id2 == -1){
        std::cout<<user2<<" was not found"<<std::endl;
        return -1;
    }
    
    //loop again to make the connection
    for (auto e: Graph){
        if(e->getName()==user1){
            e->addFriend(id2);
        }
        if(e->getName()==user2){
            e->addFriend(id1);
        }
    }
    return 0;
}



//remove connection between users
//pre: user1 and user2 are the names of the two users
//post: a connection is removed between the 2 inputed users
std::size_t Network::removeConnection(std::string user1, std::string user2){
    std::size_t id1, id2;

    //find the id's of the students
    id1 = getId(user1);
    id2 = getId(user2);

    //if an id wasn't found
    if(id1 == -1){
        std::cout<<user1<<" was not found"<<std::endl;
        return -1;
    }
    if(id2 == -1){
        std::cout<<user2<<" was not found"<<std::endl;
        return -1;
    }

    //loop again to remove the connection
    for (auto e: Graph){
        if(e->getName()==user1){
            e->removeFriend(id2);
        }
        if(e->getName()==user2){
            e->removeFriend(id1);
        }
    }
    return 0;
}

//write the users to a file
//pre: filename is the desired name of the output file
//post: a file is written with all the user information
std::size_t Network::writeUsers(std::string filename){
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cout << "There was an error opening the file" << std::endl;
        return -1;
    }

    outputFile<<getSize()<<std::endl;
    for(auto e: Graph){ //iterate across the graph

        std::string adjustedZip;
        adjustedZip = std::to_string(e->getZip());
        for(std::size_t i = 0; i < 5-adjustedZip.length(); i++){ //re-add leading 0's to the zipcodes
            adjustedZip.insert(adjustedZip.begin(),'0');
        }

        //writes user's information to the file
        outputFile<<e->getId()<<std::endl<<"\t"<<e->getName()<<std::endl<<"\t"<<e->getBirth()<<std::endl<<"\t"<<adjustedZip<<std::endl<<"\t";
        for(auto f: e->getFriends()){ //writes the list of the user's friends id
            outputFile<<f<<" ";
        }
        outputFile<<std::endl;
    }
    outputFile.close();
    std::cout<<std::endl<<getSize()<< " users were added!"<<std::endl;
    return 0;
}

//reads the users file and constructs a social network from it
//pre: filename is the name of the input file
//post: a network object is constructed
std::size_t Network::readUsers(std::string filename){
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "There was an error opening the file" << std::endl;
        return -1;
    }

    bool firstNumber = true; // created to skip first line (the size)

    //variables that will be set for each new user
    std::size_t id = 0;
    std::string fullName;
    std::size_t birthYear;
    std::size_t zip;
    int converter;

    //strings to store each line
    std::string line1;
    std::string line2;
    std::string line3;
    std::string line4;
    std::string line5;

    while (std::getline(inputFile,line1)) {

        if(firstNumber){//skips the size of the network
            firstNumber = false;
            continue;
        }

        //converts the string id to a size_t for user class
        converter = stoi(line1);
        id = converter;

        if(getline(inputFile,line2)){ //gets the name line
            line2.erase(0,1);
            fullName=line2;
        }
        if(getline(inputFile,line3)){ //gets the birthyear line
            line3.erase(0,1);
            converter = stoi(line3);
            birthYear = converter;
        }
        if(getline(inputFile,line4)){ //gets the zipcode line
            line4.erase(0, 1);
            converter = stoi(line4);
            zip = converter;
        }
        if(getline(inputFile,line5)){ //gets the friends list line
            line5.erase(0, 1);
            std::istringstream iss(line5);
            std::vector<size_t> v; //vector to store all friends
            std::string number;
            std::size_t num;

            while(iss>>number){ //iterate across the friends string
                converter = stoi(number);
                num = converter;
                v.push_back(num);
            }
            //create and add the new user to the Network
            User* u = new User(id, fullName , birthYear, zip , v);
            addUser(u);
        }
    }
    inputFile.close();
    return 0;
}

//finds the shortest path from user from to user to
//pre:user from and to are names of the users whose path we are trying to find
//post:a vector containing the shortest path of vertices is returned
std::vector<std::size_t> Network::shortestPath(std::size_t from, std::size_t to){
    std::queue<std::size_t> Q; //queue to store unprocessed vertices
    std::vector<bool> visited(getSize(), 0); //vector to mark vertices as visisted
    std::vector<std::size_t> prev(getSize(), -1); //vector to keep track of the vertex that was just processes
    std::vector<std::size_t> path; //vector to store the final path of from - to

    if(from == to){ //if same person
        path.push_back(-3);
        return path;
    }

    prev[from] = 0;
    Q.push(from); //push starting point to the queue
    while (Q.size() > 0) { //iterate until all users have been processed
        size_t currentId = Q.front();
        Q.pop();

        if (visited[currentId]) continue; //check if visited already
        visited[currentId] = true;

        for (std::size_t i = 0; i < getUser(currentId)->getFriends().size(); i++) { //iterate across friends
            size_t friendId = getUser(currentId)->getFriends()[i];
            Q.push(friendId); //push the friendId to the queue
            if(!(visited[friendId])){ 
                prev[friendId]=currentId; //set the prev vector at the friends integer, to the current integer
            }
            if(friendId == to) { //if connection has been found, build the final path vector
                path.push_back(friendId);
                path.push_back(currentId);
                std::size_t id;
                id = currentId;
                while(id != 0){ //add the full path
                    id = prev[id];
                    path.push_back(id);
                }
                std::reverse(path.begin(), path.end()); //reverse path to get in correct order
                return path; //return because user to has already been found
            }
        }
    }
    //below only runs if there is no connection from - to, so just return a vector with -2
    path.push_back(-2);
    return path;
}

//finds a user at the given distance and sends the path
//pre: from is the starting user, to is the address of an id, distance is the requested distance
//post: to is pointing to the found student, and a vector is returned listing the path to that student
std::vector<std::size_t> Network::distanceUser(std::size_t from, std::size_t& to, std::size_t distance){
    std::queue<std::size_t> Q; //queue to store unprocessed vertices
    std::vector<bool> visited(getSize(), 0); //vector to mark vertices as visited
    std::vector<std::size_t> prev(getSize(), -1); //vector to keep track of the vertex that was just processed
    std::vector<std::size_t> path; //vector to store the final path of from to the found vertex to
    std::vector<std::size_t> dist(getSize(), -1); //vector to store the distance each vertex is away from user from

    prev[from] = 0;
    dist[from] = 0;
    Q.push(from); //add user from to the queue

    while (Q.size() > 0) {//iterate until all users have been processed
        size_t currentId = Q.front();
        Q.pop();

        if (visited[currentId]) continue; //check if visited already
        visited[currentId] = true;

        for (std::size_t i = 0; i < getUser(currentId)->getFriends().size(); i++) {//iterate across friends
            size_t friendId = getUser(currentId)->getFriends()[i];
            Q.push(friendId);
            if(!(visited[friendId])){
                prev[friendId]=currentId; //set the prev vector at the friends integer, to the current integer
            }
            if (dist[friendId] == -1) {
                dist[friendId] = dist[currentId] + 1; //set the distance vector to the previous vertex's distance + 1
            }
            if(dist[friendId] == distance) { //if user has been found at correct distance
                to = friendId; //set to the friends Id
                path.push_back(friendId); //construct the final paths vector
                path.push_back(currentId);
                std::size_t id;
                id = currentId;
                while(id != 0){ //add full path of from user to the user at the correct distance
                    id = prev[id];
                    path.push_back(id);
                }
                std::reverse(path.begin(), path.end()); //reverse path to get in correct order
                return path; //return because a user at distance "distance" has been found
            }
        }
    }
    //below only runs if no user was found at the correct distance, so set "to" to -1 and return the empty path vector
    to = -1;
    return path;
}

//gives a friend suggestion with the maximum score
//pre: who is the id of the student whose suggested friend we want to find, and score is a delcared size_t to store the max score
//post: a vector containing the id's of the friends with the highest score is returned, and score is set to the highest score
std::vector<std::size_t> Network::suggestFriends(std::size_t who, std::size_t& score){
    std::vector<std::size_t> scores(getSize(), 0); //vector to store the scores of all users

    for(auto e: getUser(who)->getFriends()){ //initilize the scores of people that are already friends of who to -1
        scores[e]=-1;
    }

    for(auto e: getUser(who)->getFriends()){ //iterate through all friends of who
        for(auto f: getUser(e)->getFriends()){ //iterate through all friends of friends of who
            if(scores[f]!=0 or f == who ){ //avoids any people that have already been considered 
                continue;
            }
            for(auto g: getUser(f)->getFriends()){ //iterate through one more layer of friends to determine how count shared friends 
                if(scores[g]==-1){ //if a shared friend is found
                    scores[f]++; //increase score of person considered
                }
            }
        }
    }

    std::vector<std::size_t> matches; //create vector to store the best matches
    std::size_t max = 0;
    for(auto e: scores){ //loop through scores vector to find max score
        if(e > max and e != -1){
            max = e;
        }
    }
    for(std::size_t i = 0; i<scores.size(); i++){ //loop through scores vector again to find the friends with that max score
        if(scores[i] == max){
            matches.push_back(i); //add the max score friends to the vector
        }
    }
    score = max; //set the score
    if(max == 0){ //change the score to -1 if no shared friends for anyone
        score = -1;
    }
    return matches; //return vector of users with best score
}


//gives a list of vectors containing the id's of people in each group
//pre: none
//post: returns a vector containing the id's of people in each group
std::vector<std::vector<std::size_t> > Network::groups(){
    std::vector<std::vector<std::size_t>> set; //create a set that is a vector of vectors of user id's
    std::vector<std::size_t> stck; //create a stack implemented as a vector for dfs
    std::vector<bool> visited(getSize(), 0); //vector to mark vertices as visited

    for(std::size_t i = 0; i<getSize(); i++){ //iterate through all user's
        if(visited[Graph[i]->getId()]){ //don't consider user's that have already been visited in the dfs
            continue;
        }
        std::vector<std::size_t> group; //create a new group
        stck.push_back(Graph[i]->getId()); //add the user

        while (stck.size() > 0) { //begin dfs
            size_t currentId = stck[stck.size() - 1]; //Visit the next user
            stck.pop_back();

            if (visited[currentId]) { continue; } //skip user if they have already been visited
            visited[currentId] = true; //mark them as visisted

            group.push_back(currentId); // Add current user to group

            for (auto e: getUser(currentId)->getFriends()) { //iterate through friends
                stck.push_back(e); //push the friend to the stack
            }
        }
        set.push_back(group); //add the vector to the set
    }
    return set; //return the set
}

//gets the total amount of posts in the network
//pre: none
//post: returns the number of posts in the network
std::size_t Network::getTotalPosts(){
    std::size_t total = 0;
    for(auto e: Graph){ //iteratre across all friends
        total+=e->getMessagesCount(); //add all of their posts
    }
    return total;
}

//adds a post to the user
// pre: all private member variables are the correct
// post: a post is added to user id ownerId and it the correct type of post
void Network::addPost(std::size_t ownerId, std::string message, std::size_t likes, bool incoming, std::size_t author, bool isPublic){
    if(Graph.size()<ownerId){ //checks to make sure owner id is valid
        std::cout<<std::endl<<"invalid owner ID"<<std::endl;
        return;
    }
    if(Graph.size()<author and author != -1){ //checks to make sure author id is valid (an author id of -1 means it is a self post)
        std::cout<<std::endl<<"invalid author ID"<<std::endl;
        return;
    }
    
    std::size_t postId;
    postId = getTotalPosts();
    if(incoming){ //if the post is an incoming post
        Post* p = new incomingPost(postId,message,likes,isPublic,getUser(author)->getName());
        getUser(ownerId)->addPost(p); //add the post to the user
    }
    else{ //the post is regular post
        Post* p = new Post(postId,message,likes);
        getUser(ownerId)->addPost(p); //add the post to the user
    }
}

//prints the amount of recent posts requested for user name
//pre: name is user, howMany is the amount of posts wanted, and showOnlyPublic is true or false depending on which posts wanted to be shown
//post: howMany posts are printed for user name
std::string Network::displayPosts(std::string name, std::size_t howMany, bool showOnlyPublic){
    std::string s;
    s = getUser(getId(name))->displayPosts(howMany,showOnlyPublic); //display the user's posts
    return s;
}

//reads the users file and contructs all the posts
//pre: filename is the name of the input file
//post: all posts are made from the file
int Network::readPosts(std::string filename){
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cout << "There was an error opening the file" << std::endl;
        return -1;
    }

    bool firstNumber = true; // created to skip first line (the size)

    //variables that will be set for each new post
    std::size_t ownerId;
    std::string message;
    std::size_t likes;
    bool incoming;
    std::size_t author;
    bool isPublic;

    //helper variable
    int converter;

    //strings to store each line
    std::string line1;
    std::string line2;
    std::string line3;
    std::string line4;
    std::string line5;
    std::string line6;

    while (std::getline(inputFile,line1)){ //iterates across the entire file

        if(firstNumber){//skips the size of the network
            firstNumber = false;
            continue;
        }

        //DO NOTHING WITH LINE 1

        if(getline(inputFile,line2)){ //gets the message line
            line2.erase(0,1);
            message=line2;
        }
        if(getline(inputFile,line3)){ //gets the Owner ID line
            line3.erase(0,1);
            converter = stoi(line3);
            ownerId = converter;
        }
        if(getline(inputFile,line4)){ //gets the Number of Likes line
            line4.erase(0, 1);
            converter = stoi(line4);
            likes = converter;
        }
        if(getline(inputFile,line5)){ //gets the either empty line or public private line
            std::size_t found1 = line5.find("public");

            if(found1 != std::string::npos){ //if "public" is in the line
                isPublic=true;
                incoming = true;

                if(getline(inputFile,line6)){ //gets the author line
                    line6.erase(0, 1);
                    author = getId(line6);
                    addPost(ownerId,message,likes,incoming,author,isPublic);
                    continue;
                }
            } 

            std::size_t found2 = line5.find("private");
            if(found2 != std::string::npos){ // if "private" is in the line
                isPublic = false;
                incoming = true;

                if(getline(inputFile,line6)){ //gets the author line
                    line6.erase(0, 1);
                    author = getId(line6);
                    addPost(ownerId,message,likes,incoming,author,isPublic);
                    continue;
                }
            } 

            //If this code runs then line5 was empty
            isPublic = true;
            incoming = false;
            author = -1;
            addPost(ownerId,message,likes,incoming,author,isPublic);
        }
    }
    inputFile.close();
    return 0;
}