#include <string>
#include <cstdlib>

#include "post.h"

//function to print a post
//pre: none
//post: a string is returned of the posts information
std::string Post::displayPost(){
    std::string s;
    s= Message + "\n- Liked by "+std::to_string(Likes)+" people";
    return s;
}

//default constructor
//pre: none
//post: basic incomingPost is constructed
incomingPost::incomingPost(): Post(){
    isPublic = false; 
}

//primary constructor
//pre: the input id, message, likes, ispublic, name are the correct types
//post: basic incomingPost is constructed
incomingPost::incomingPost(std::size_t id, std::string message, std::size_t likes, bool ispublic, std::string name): Post(id,message + "\n- From user: " + name,likes){
    isPublic = ispublic; 
}

//function to print a Post
//pre: none
//post: a string is returned of the incoming Posts information
std::string incomingPost::displayPost(){
    std::string s;
    //add public or private to start of string depending on if the post is private or public
    if(isPublic){
        s+="Public: ";
    }
    else{ 
        s+="Private: ";
    }
    s += Post::displayPost();
    return s;
}

