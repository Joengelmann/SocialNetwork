#include <filesystem>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include "socialnetworkwindow.h"
#include "ui_socialnetworkwindow.h"
#include <sstream>

//constructor for the social network window
//pre: none
//post: constructs the social network object and reads in the users and posts
SocialNetworkWindow::SocialNetworkWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkWindow)
{
    ui->setupUi(this);
    connect(ui->SubmitNameButton, &QPushButton::clicked, this, &SocialNetworkWindow::on_SubmitNameButton_clicked);
    connect(ui->ProfileButton, &QPushButton::clicked, this, &SocialNetworkWindow::on_ProfileButton_clicked);
    connect(ui->LogoutButton, &QPushButton::clicked, this, &SocialNetworkWindow::on_LogoutButton_clicked);
    connect(ui->AddFriendButton, &QPushButton::clicked, this, &SocialNetworkWindow::on_AddFriendButton_clicked);
    connect(ui->BackButton, &QPushButton::clicked, this, &SocialNetworkWindow::on_BackButton_clicked);

    //display and hide correct widgets
    ui->ProfileNameLabel->hide();
    ui->ProfileButton->hide();
    ui->PostsTitleLabel->hide();
    ui->PostsLabel->hide();
    ui->SuggestedLabel->hide();
    ui->SuggestedTable->hide();
    ui->FriendsLabel->hide();
    ui->FriendsTable->hide();
    ui->AddFriendButton->hide();
    ui->LogoutButton->hide();
    ui->BackButton->hide();
    //algorithm to get pathway to user and post files
    char buffer[PATH_MAX];
    (getcwd(buffer, sizeof(buffer)) != nullptr);

    std::string path(buffer);

    std::stringstream ss(path);
    std::string component;
    std::vector<std::string> components;

    while (std::getline(ss, component, '/')){ //splits the path into the components
        components.push_back(component);
    }

    //there are 4 directories in the pathway that we need to remove from the string
    std::size_t componentsToRemove = 4;

    // Check if we have enough components to remove
    if(components.size() > componentsToRemove){
        components.erase(components.end() - componentsToRemove, components.end());
        path = "";
        for (const std::string& comp : components){ //reconstructs the pathway
            path += '/' + comp;
        }
    }
    else{ //this should never run
        std::cerr << "Not enough components to remove in the file path." << std::endl;
    }

    std::string userFile;
    std::string postFile;

    userFile += path + "/SocialNetwork/social_network.txt";
    postFile += path + "/SocialNetwork/posts.txt";

    //there is an extra "/" at the begining of the pathway
    userFile.erase(0, 1);
    postFile.erase(0, 1);

    //call functions to read in the users and posts
    Net.readUsers(userFile);
    Net.readPosts(postFile);
}

//destructor
//pre: none
//post: the window has been deleted
SocialNetworkWindow::~SocialNetworkWindow()
{
    delete ui;
}

//sets up the user interface after the user submits a valid name
//pre: a name has been entered into the text box
//post: The profile for the name that was entered is shown
void SocialNetworkWindow::on_SubmitNameButton_clicked()
{
    QString me = ui->NameBox->toPlainText();
    std::size_t id = Net.getId(me.toStdString());
    std::size_t invalid = -1;
    if(id==invalid){
        ui->NameLabel->setText("Enter a valid Name ");
        return;
    }
    ui->ProfileNameLabel->setText(QString::fromStdString("My Profile"));
    //set Me
    Me = Net.getUser(id);
    Prev.push(Me); //push me to the previous stack

    //Display correct widgets
    ui->LoginLabel->hide();
    ui->NameLabel->hide();
    ui->NameBox->hide();
    ui->SubmitNameButton->hide();
    ui->ProfileNameLabel->show();
    ui->PostsTitleLabel->show();
    ui->PostsLabel->show();
    ui->SuggestedLabel->show();
    ui->SuggestedTable->show();
    ui->FriendsLabel->show();
    ui->FriendsTable->show();
    ui->LogoutButton->show();
    ui->BackButton->show();

    //set the right text for the posts, friends and suggested friends
    setPosts();
    setFriends();
    setSuggested();

}

//displays the recent posts on the left side of the screen
//pre: either Me or Them pointers in the Network obejct point to a user
//post: the most recent posts of the correct user's profiles are displayed on the left
void SocialNetworkWindow::setPosts(){
    QString posts;
    if(Them == nullptr){ //sets the posts for Me's profile
        posts = QString::fromStdString(Net.displayPosts(Me->getName(),5,false));
    }
    else{ //sets the post for Them's profile
        posts = QString::fromStdString(Net.displayPosts(Them->getName(),5,true));
    }
    ui->PostsLabel->setText(posts);
}

// function to handle clicking on a user's friend
// pre: none
// post: the user who was clicked on's profile is now displayed
void SocialNetworkWindow::friendButtonClicked(){
    // Determine which friend's button was clicked
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    if (clickedButton) {
        std::string friendName = clickedButton->text().toStdString();
        Them = Net.getUser(Net.getId(friendName)); //set Them to the user of the friend on the button
        Prev.push(Them); //push them to previous stack
        if(Them==Me){ //If the friend clicked is Me, simply call the profile button function
            ui->ProfileButton->click();
            return;
        }
        ui->ProfileNameLabel->setText(QString::fromStdString(friendName + " Profile"));

        //set the right text for the posts and friends
        setPosts();
        setFriends();

        //display correct widgets
        ui->SuggestedLabel->hide();
        ui->SuggestedTable->hide();
        setAddButtonText();
        ui->AddFriendButton->show();
        ui->ProfileButton->show();
    }
}

// function to handle clicking on a suggested friend
//pre: none
//post: the friend is added to the logged in user and the text displayed is changed
void SocialNetworkWindow::suggestedFriendButtonClicked(){
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (clickedButton) {
        std::string friendName = clickedButton->text().toStdString();
        Net.addConnection(Me->getName(),friendName); //adds the suggested friend
        clickedButton->setText(QString::fromStdString("Added!"));
        setFriends(); //displays the new friend on the friends list
    }
}

//displays all friends of the current User's profile
//pre: Either Me or Them pointers in the network class are pointing at a valid user
//post: displays the friends of the User who'se profile we are on
void SocialNetworkWindow::setFriends(){
    std::vector<std::size_t> friends;
    //determine which user's friends to get
    if(Them == nullptr){
        friends = Me->getFriends();
    }
    else{
        friends = Them->getFriends();
    }

    std::size_t rows = friends.size(); //set the amount of rows in the table to the number of friends

    ui->FriendsTable->setRowCount(rows);
    ui->FriendsTable->setColumnCount(1);

    for (std::size_t i = 0; i < rows; i++) { //create a button for each friend
        QPushButton *button = new QPushButton(QString::fromStdString((Net.getUser(friends[i]))->getName()));
        button->setFixedWidth(200);
        connect(button, &QPushButton::clicked, this, &SocialNetworkWindow::friendButtonClicked);
        ui->FriendsTable->setCellWidget(i,0,button); //add the button to the table
    }
}

//displays all suggested friends of the the logged in user
//pre: Me pointer is pointing at the user that is logged in
//post: displays the suggested friends of Me
void SocialNetworkWindow::setSuggested(){
    std::vector<std::size_t> suggestedFriends;
    std::size_t score; //this is unused for now
    suggestedFriends = Net.suggestFriends(Net.getId(Me->getName()),score); //get the suggested friends

    std::size_t rows = suggestedFriends.size(); //set the amount of rows in the table to the number of suggested friends
    ui->SuggestedTable->setRowCount(rows);
    ui->SuggestedTable->setColumnCount(1);

    for (std::size_t i = 0; i < rows; i++) { //create a button for each suggested friend
        QPushButton *button = new QPushButton(QString::fromStdString((Net.getUser(suggestedFriends[i]))->getName()));
        button->setFixedWidth(180);
        connect(button, &QPushButton::clicked, this, &SocialNetworkWindow::suggestedFriendButtonClicked);
        ui->SuggestedTable->setCellWidget(i,0,button); //add each button to the table
    }
}

//button that returns user to login screem
//pre: none
//post: me pointer is set to null and the login screan is displayed
void SocialNetworkWindow::on_LogoutButton_clicked()
{
    //reset Me and Them pointers
    Me = nullptr;
    Them = nullptr;

    //reset the Prev stack
    while(!Prev.empty()){
        Prev.pop();
    }

    //display logout screen widgets
    ui->ProfileNameLabel->hide();
    ui->ProfileButton->hide();
    ui->PostsTitleLabel->hide();
    ui->PostsLabel->hide();
    ui->SuggestedLabel->hide();
    ui->SuggestedTable->hide();
    ui->FriendsLabel->hide();
    ui->FriendsTable->hide();
    ui->AddFriendButton->hide();
    ui->LogoutButton->hide();
    ui->BackButton->hide();
    ui->LoginLabel->show();
    ui->NameLabel->show();
    ui->NameBox->show();
    ui->SubmitNameButton->show();
}

//button that brings the user back to their profile
//pre: Someone else's profile is displayed
//post: The loged in user's profile is redisplayed
void SocialNetworkWindow::on_ProfileButton_clicked()
{
    Them = nullptr; //reset Them pointer
    Prev.push(Me);

    //reset the text for friends, posts and suggested friends
    setFriends();
    setPosts();
    setSuggested();

    //display Me's profile widgets
    ui->AddFriendButton->hide();
    ui->ProfileButton->hide();
    ui->SuggestedLabel->show();
    ui->SuggestedTable->show();
    ui->ProfileNameLabel->setText(QString::fromStdString("My Profile"));
}

//button that adds a displayed user as friends to the logged in user
//pre: Someone else's profile is displayed
//post: the logged in user and displayed user are now friends and calls the function to set the button's text
void SocialNetworkWindow::on_AddFriendButton_clicked()
{
    if(ui->AddFriendButton->text() == QString::fromStdString("Add as Friend")){
        Net.addConnection(Me->getName(),Them->getName()); //add connection with the user
    }
    else{
        Net.removeConnection(Me->getName(),Them->getName()); //remove connection with the user
    }
    setFriends();
    setAddButtonText(); //display the correct text
}

// Sets the text of the add friend button depending on if the user is already friends with the logged in user
// pre: We are on a profile that is not the logged in user
// post: The text of the add friend button displays the correct text
void SocialNetworkWindow::setAddButtonText(){
    size_t id = Me->getId();
    for(auto e: Them->getFriends()){
        if(e == id){ //check if the users are already friends
            ui->AddFriendButton->setText(QString::fromStdString("Remove as Friend"));
            return;
        }
    }
    //set the text to add as friend. This only runs if the user's aren't already friends
    ui->AddFriendButton->setText(QString::fromStdString("Add as Friend"));
}

//button that goes back a page, similar to a browser back button
//pre: none
//post: the previous user profile or login page is displayed
void SocialNetworkWindow::on_BackButton_clicked(){
    Prev.pop(); //pop the most recent person added to stack
    if(Prev.size() == 0){
        ui->LogoutButton->click();
        return;
    }
    Them = Prev.top(); //set Them to the user of the friend on the button
    if(Them==Me){ //If Top of Prev is Me, simply use the profile button algorithm without pushing to Prev
        Them = nullptr; //reset Them pointer

        //reset the text for friends, posts and suggested friends
        setFriends();
        setPosts();
        setSuggested();

        //display Me's profile widgets
        ui->AddFriendButton->hide();
        ui->ProfileButton->hide();
        ui->SuggestedLabel->show();
        ui->SuggestedTable->show();
        ui->ProfileNameLabel->setText(QString::fromStdString("My Profile"));
        return;
    }
    //friend button click algorithm:
    ui->ProfileNameLabel->setText(QString::fromStdString(Them->getName() + " Profile"));

    //set the right text for the posts and friends
    setPosts();
    setFriends();

    //display correct widgets
    ui->SuggestedLabel->hide();
    ui->SuggestedTable->hide();
    setAddButtonText();
    ui->AddFriendButton->show();
    ui->ProfileButton->show();
}
