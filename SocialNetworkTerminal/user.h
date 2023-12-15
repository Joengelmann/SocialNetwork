#ifndef USER_H
#define USER_H

#include <cstddef>
#include <string>
#include <vector>
#include "post.h"

class User {
public:
    //default constructor
    //pre: none
    //post: basic user is constructed
    User(): Id(-1), Name("-1"), Birth(-1), Zip(-1) {}

    //primary constructor
    //pre: the input id, name, birth, zip are all the correct object types
    //post: user is created with inputed member variable
    User(size_t id, std::string name, std::size_t birth, std::size_t zip, std::vector<std::size_t> friends)
    : Id(id), Name(name), Birth(birth), Zip(zip), Friends(friends){}

    //getter functions
    //pre: none
    //post: private member variable is returned
    std::size_t getId() const {return Id;}
    std::string getName() const {return Name;}
    std::size_t getBirth() const {return Birth;}
    std::size_t getZip() const {return Zip;}
    std::vector<std::size_t> getFriends() const {return Friends;}
    std::size_t getMessagesCount() const {return Messages.size();}

    //add a friend to the user
    //pre: FriendID is the number Id of the friend trying to be added
    //post: a friend is added to the user's friends away if they aren't already friends
    void addFriend(std::size_t friendId);

    //remove a friend from the user
    //pre: FriendID is the number Id of the friend trying to be removed
    //post: a friend is removed from the user if they were friends to begin with
    void removeFriend(std::size_t friendId);

    //adds a post to the user
    //pre: Post is a pointer to a post
    //post: a post is added to the user
    void addPost(Post* Post) {Messages.push_back(Post);}

    //prints the amount of recent posts requested
    //pre: howMany is the amount of posts wanted, and showOnlyPublic is true or false depending on which posts wanted to be shown
    //post: howMany posts are printed
    std::string displayPosts(std::size_t howMany, bool showOnlyPublic);

private:
    //private member variables
    std::size_t Id;
    std::string Name;
    std::size_t Birth;
    std::size_t Zip;
    std::vector<std::size_t> Friends;
    std::vector<Post*> Messages;
};

#endif