#ifndef POST_H
#define POST_H

#include <string>
#include <cstdlib>
#include <vector>

class Post{
public:
    //default constructor
    //pre: none
    //post: basic post is constructed
    Post(): Id(-1), Message("default1"), Likes(0) {}

    //primary constructor
    //pre: the input id, message, and likes are the correct types
    //post: Post is created with inputed member variable
    Post(size_t id, std::string message, std::size_t likes): Id(id), Message(message), Likes(likes){}

    //getter functions
    //pre: none
    //post: private member variable is returned
    std::size_t getId() const {return Id;}
    std::string getMessage() const {return Message;}
    std::size_t getLikes() const {return Likes;}

    virtual std::string displayPost();

private:
    //private member variables
    std::size_t Id;
    std::string Message;
    std::size_t Likes;
};

class incomingPost : public Post{
public:
    //default constructor
    //pre: none
    //post: basic incomingPost is constructed
    incomingPost();

    //primary constructor
    //pre: the input id, message, likes, ispublic, name are the correct types
    //post: basic incomingPost is constructed
    incomingPost(std::size_t id, std::string message, std::size_t likes, bool ispublic, std::string name);

    //getter functions
    //pre: none
    //post: private member variable is returned
    bool getisPublic() const {return isPublic;}

    //function to print a Post
    //pre: none
    //post: a string is returned of the incoming Posts information
    std::string displayPost();

private:
    //private member variables
    bool isPublic;
};

#endif