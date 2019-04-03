// Author: Jacob Christiansen
// Class: CSCI 2270
// Course Instructor: Rhonda Hoenigman
// Recitation: 105
//
// Assignment 6

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "MovieTree.hpp"
using namespace std;

int main(int argc, char *argv[]){
    MovieTree movT;
    MovieNodeLL movNLL;
    char *filename =argv[1];
    string line;
    int i = 0;
    ifstream fr;
    fr.open(filename);
    if(!fr.eof() ){
        while(getline(fr, line)){
            stringstream ss(line);
            getline(ss, line, ',');
            movNLL.ranking = stoi(line);
            getline(ss, line, ',');
            movNLL.title = line;
            getline(ss, line, ',');
            movNLL.year = stoi(line);
            getline(ss, line, ',');
            movNLL.quantity = stoi(line);
            movT.addMovieNode(movNLL.ranking, movNLL.title, movNLL.year, movNLL.quantity);   
        }
    }

    while(i!=6){
    	cout<<"======Main Menu======"<<endl<<"1. Find a movie"<<endl<<"2. Rent a movie"<<endl<<"3. Print the inventory"<<endl<<"4. Delete a movie"<<endl<<"5. Count the movies"<<endl<<"6. Quit"<<endl;
    	cin>>i;
	    if(i==1){
	    	string title;
	    	cin.ignore (100,'\n');
	   		cout<<"Enter title:"<<endl;
	   		getline (cin, title);
	   		movT.findMovie(title);
	    }
	    else if(i==2){
	    	string title;
	    	cin.ignore(100, '\n');
	   		cout<<"Enter title:"<<endl;
	   		getline(cin, title);
	   		movT.rentMovie(title);
	    }
	    else if(i==3){
	    	movT.printMovieInventory();
	    }
	    else if(i==4){
	    	string title;
	    	cin.ignore(100, '\n');
	   		cout<<"Enter title:"<<endl;
	   		getline(cin, title);
	   		movT.deleteMovieNode(title); 	
	    }
	    else if(i==5){
	    	cout<<"Tree contains: "<<movT.countMovieNodes()<<" movies."<<endl;
	    }
	    else if(i==6){
	    	cout<<"Goodbye!"<<endl;
	    	break;
	    }
    }
    return 0;
}