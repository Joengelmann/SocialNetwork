#ifndef NETWORK_H
#define NETWORK_H

#include <cstddef>
#include <string>
#include <vector>
#include "user.h"


class Network {
public:
    //default constructor
    //pre: none
    //post: an empty network is created
    Network() {}

    //gets the Id of a user from their name
    //pre: user is the string name of the user
    //post: the Id is returned of the user names user
    std::size_t getId(std::string user);

    //gets the user's id
    //pre: id is the size_t id of the user
    //post: a pointer to the user is returned
    User* getUser(std::size_t id);

    //gets the amount of people in the network
    //pre: none
    //post: returns the amount of people in the network
    std::size_t getSize() {return Graph.size();}

    //add user
    //pre: newUser is a pointer pointing to the user we want to add
    //post: a user is added to the network
    void addUser(User* newUser);

    //add connection between users
    //pre: user1 and user2 are the names of the two users
    //post: a connection is made between the 2 inputed users
    std::size_t addConnection(std::string user1, std::string user2);

    //remove connection between users
    //pre: user1 and user2 are the names of the two users
    //post: a connection is removed between the 2 inputed users
    std::size_t removeConnection(std::string user1, std::string user2);

    //write the users to a file
    //pre: filename is the desired name of the output file
    //post: a file is written with all the user information
    std::size_t writeUsers(std::string filename);

    //reads the users file and constructs a social network from it
    //pre: filename is the name of the input file
    //post: a network object is constructed
    std::size_t readUsers(std::string filename);

    //finds the shortest path from user from to user to
    //pre:user from and to are names of the users whose path we are trying to find
    //post:a vector containing the shortest path of vertices is returned
    std::vector<std::size_t> shortestPath(std::size_t from, std::size_t to);

    //finds a user at the given distance and sends the path
    //pre: from is the starting user, to is the address of an id, distance is the requested distance
    //post: to is pointing to the found student, and a vector is returned listing the path to that student
    std::vector<std::size_t> distanceUser(std::size_t from, std::size_t& to, std::size_t distance);

    //gives a friend suggestion with the maximum score
    //pre: who is the id of the student whose suggested friend we want to find, and score is a delcared size_t to store the max score
    //post: a vector containing the id's of the friends with the highest score is returned, and score is set to the highest score
    std::vector<std::size_t> suggestFriends(std::size_t who, std::size_t& score);

    //gives a list of vectors containing the id's of people in each group
    //pre: none
    //post: returns a vector containing the id's of people in each group
    std::vector<std::vector<std::size_t> > groups();

    //gets the total amount of posts in the network
    //pre: none
    //post: returns the number of posts in the network
    std::size_t getTotalPosts();

    // adds a post to the user
    // pre: all private member variables are the correct
    // post: a post is added to user id ownerId and it the correct type of post
    void addPost(std::size_t ownerId, std::string message, std::size_t likes, bool incoming, std::size_t author, bool isPublic);

    //prints the amount of recent posts requested for user name
    //pre: name is user, howMany is the amount of posts wanted, and showOnlyPublic is true or false depending on which posts wanted to be shown
    //post: howMany posts are printed for user name
    std::string displayPosts(std::string name, std::size_t howMany, bool showOnlyPublic);


    //reads the users file and contructs all the posts
    //pre: filename is the name of the input file
    //post: all posts are made from the file
    int readPosts(std::string filename);

private:
    //private member variable to store vector of users
    std::vector<User*> Graph;
};

#endif