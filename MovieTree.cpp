#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "MovieTree.hpp"

using namespace std;

MovieTree::MovieTree(){
	root = NULL;
}

MovieTree::~MovieTree(){
	DeleteAll(root);
}

void MovieTree::printMovieInventory(){
	printMovieInventory(root);
}

int MovieTree::countMovieNodes(){
	int *c = new int(0);
	countMovieNodes(root, c);
	return *c;
}
/*
When the user selects this option, they are prompted for the title of the
movie to delete. Then it searches the tree for the first letter of
that movie, and then search the singly linked list for the title. If the title is
found, it deletes it from the singly linked list. If it was the only title for that letter
in the BST, it also deletes the node in the BST and re-assigns the
parent and child pointers to bypass the deleted node, and free the memory
assigned to the node. If the movie is not found in the search process, it prints
“Movie not found” and doesn't attempt to delete.
A movie node also gets deleted when its quantity goes to 0.
*/
void MovieTree::deleteMovieNode(string title){
	MovieNodeBST *node = searchBST(root, title);
	if(node!=NULL){
		if(node->head->next==NULL){ //delete node, move b/c LL is empty
			if(node!=root){
				if(node->parent->letter > node->letter){ //left child
					if(node->rightChild==NULL && node->leftChild == NULL){ //0 children
						node->parent->leftChild = NULL;
					}else if(node->rightChild!=NULL && node->leftChild!=NULL){ //2 children
						MovieNodeBST *minimum = treeMinimum(node->rightChild);
						if(minimum==node->rightChild){
							node->parent->leftChild = minimum;
							minimum->parent = node->parent;
							minimum->leftChild = node->leftChild;
							minimum->leftChild->parent = minimum;
						}else{
							minimum->parent->leftChild = minimum->rightChild;
							minimum->rightChild->parent = minimum->parent;
							minimum->parent = node->parent;
							node->parent->leftChild = minimum;
							minimum->leftChild = node->leftChild;
							minimum->rightChild = node->rightChild;
							node->rightChild->parent = minimum;
							node->leftChild->parent = minimum;
						}
					}else{ //1 child
						if(node->leftChild!=NULL){ //left child;
							MovieNodeBST *temp = node->leftChild;
							node->parent->leftChild = temp;
							temp->parent = node->parent;
						}else{ //right child
							MovieNodeBST *temp = node->rightChild;
							node->parent->leftChild = temp;
							temp->parent = node->parent;
						}
					}
				}else{ //right child
					if(node->rightChild==NULL && node->leftChild==NULL){ //no children
						node->parent->rightChild = NULL;
					}else if(node->rightChild!=NULL && node->leftChild!=NULL){ //2 children
						MovieNodeBST *minimum = treeMinimum(node->rightChild);
						if(minimum==node->rightChild){
							node->parent->rightChild = minimum;
							minimum->parent = node->parent;
							minimum->leftChild = node->leftChild;
							minimum->leftChild->parent = minimum;
						}else{
							minimum->parent->rightChild = minimum->rightChild;
							minimum->rightChild->parent = minimum->parent;
							minimum->parent = node->parent;
							node->parent->leftChild = minimum;
							minimum->leftChild = node->leftChild;
							minimum->rightChild = node->rightChild;
							node->rightChild->parent = minimum;
							node->leftChild->parent = minimum;
						}
					}else{ //1 child
						if(node->leftChild!=NULL){ //left child
							MovieNodeBST *temp = node->leftChild;
							node->parent->rightChild = temp;
							temp->parent = node->parent;
						}else{ //right child
							MovieNodeBST *temp = node->rightChild;
							node->parent->rightChild = temp;
							temp->parent = node->parent;
						}
					}
				}
			}else{ //node = root
				if(node->rightChild==NULL && node->leftChild==NULL){ //0 children
					root = NULL;
				}else if(node->rightChild!=NULL && node->leftChild!=NULL){ //2 children
					MovieNodeBST *minimum = treeMinimum(node->rightChild);
					if(minimum==node->rightChild){
						minimum->leftChild = node->leftChild;
						node->leftChild->parent = minimum;
						minimum->parent = NULL;
						root = minimum;
					}else{
						minimum->parent->leftChild = minimum->rightChild;
						minimum->rightChild->parent = minimum->parent;
						minimum->parent = NULL;
						minimum->leftChild = node->leftChild;
						node->leftChild->parent = minimum;
						minimum->rightChild = node->rightChild;
						node->rightChild->parent = minimum;
						root = minimum;
					}
				}else{ //1 child
					if(node->leftChild!=NULL){ //left child;
						MovieNodeBST *temp = node->leftChild;
						temp->parent = NULL;
						temp = root;
					}else{ //right child
						MovieNodeBST *temp = node->rightChild;
						temp->parent = NULL;
						temp = root;
					}
				}
			}
			delete node;
			return;
		}else{ //delete from first letter's LL
			if(title==node->head->title){ //delete from head
				MovieNodeLL *temp = node->head;
				node->head = node->head->next;
				delete temp;
				return;
			}else{ //delete from the middle or tail
				MovieNodeLL *previous = node->head;
				MovieNodeLL *current = node->head->next;
				while(current!=NULL && current->title!=title){
					previous = current;
					current = current->next;
				}
				if(current!=NULL){
					previous->next = current->next;
					delete current;
					return;
				}
			}
		}
	}
	cout << "Movie not found." << endl;
}
/*
When the user starts the program they will pass it the name of the text file
that contains all movie information. Each line in the file shows the IMDB
ranking, title, year released, and quantity in stock. The program
handles the command line argument, opens the file, and read all movie data in
the file.
*/
void MovieTree::addMovieNode(int ranking, string title, int releaseYear, int quantity){
	MovieNodeLL *listNode = new MovieNodeLL(ranking, title, releaseYear, quantity);
	if(root==NULL){
		MovieNodeBST *addNode = new MovieNodeBST(title[0]);
		addNode->head = listNode;
		root = addNode;
	}else{
		MovieNodeBST *currNode = root;
		MovieNodeBST *parent = root;
		while(currNode!=NULL && currNode->letter!=title[0]){
			if(title[0] < currNode->letter){
				parent =  currNode;
				currNode = currNode->leftChild;
			}else{
				parent = currNode;
				currNode = currNode->rightChild;
			}
		}
		if(currNode!=NULL){
			if(currNode->head->title > listNode->title){
				listNode->next = currNode->head;
				currNode->head = listNode;
			}else{
				MovieNodeLL *currentMovie = currNode->head;
				while(currentMovie->next!=NULL && currentMovie->next->title < listNode->title){
					currentMovie = currentMovie->next;
				}
				listNode->next = currentMovie->next;
				currentMovie->next = listNode;
			}
		}else{
			MovieNodeBST *newNode = new MovieNodeBST(title[0]);
			newNode->head = listNode;
			newNode->parent = parent;
			if(title[0] < parent->letter){
				parent->leftChild = newNode;
			}else{
				parent->rightChild = newNode;
			}
		}
	}
}
/*
When the user selects this option from the menu, they are prompted
for the name of the movie. The program then searches the tree and
singly linked lists and displays all information for that movie. If the movie is
not found, the program displays, “Movie not found.”
*/
void MovieTree::findMovie(string title){
	MovieNodeBST *foundNode = searchBST(root, title);
	MovieNodeLL *nodeList;
	if(foundNode!=NULL){
		nodeList=searchLL(foundNode->head, title);
		if(nodeList!=NULL){
			cout<<"Movie Info:"<<endl<<"==========="<<endl<<"Ranking:"<<nodeList->ranking<<endl<<"Title:"<<nodeList->title<<endl<<"Year:"<<nodeList->year<<endl<<"Quantity:"<< nodeList->quantity<<endl; 
		}else{
			cout << "Movie not found." << endl;
		}
	}else{
		cout << "Movie not found." << endl;
	}
}
/*
When the user selects this option from the menu, they are prompted
for the name of the movie. If the movie is found in the data structure, the
program updates the Quantity in stock property of the movie and
displays the new information about the movie.
If the movie is not found, the program displays, “Movie not found.”
When the quantity reaches 0, the movie will be deleted from the singly
linked list. If that was the only node in the singly linked list, the node will
also be deleted from the BST for that letter.
*/
void MovieTree::rentMovie(string title){
	MovieNodeBST* foundNode = searchBST(root, title);
	MovieNodeLL* nodeList;
	if(foundNode!=NULL){
		nodeList = searchLL(foundNode->head, title);
		if(nodeList!=NULL){
			nodeList->quantity--;
			cout<<"Movie has been rented."<<endl<<"Movie Info:"<<endl<<"==========="<<endl<<"Ranking:"<<nodeList->ranking<<endl<<"Title:"<<nodeList->title<<endl<<"Year:"<<nodeList->year<<endl<<"Quantity:"<< nodeList->quantity<<endl;
            if(nodeList->quantity<=0){
				deleteMovieNode(title);
			}
		}else{
			cout<<"Movie not found."<<endl;
		}
	}else{
		cout<<"Movie not found."<<endl;	
	}
}

void MovieTree::DeleteAll(MovieNodeBST *node){
	if(node!=NULL){
		DeleteAll(node->leftChild);
		DeleteAll(node->rightChild);
		MovieNodeLL *current = node->head;
		MovieNodeLL *next;
		while(current!=NULL){
			next = current->next;
			cout<<"Deleting: "<<current->title<<endl;
			delete current;
			current = next;
		}
		delete node;
	}
}
/*
When the user selects this option from the menu, the program will
display all movie titles and the quantity available in sorted order by title.
*/
void MovieTree::printMovieInventory(MovieNodeBST *node){
	if(node!=NULL){
		printMovieInventory(node->leftChild);
		MovieNodeLL *current = node->head;
		while(current!=NULL){
			cout<<"Movie: "<<current->title<<" "<<current->quantity<<endl;
			current = current->next;
		}
		printMovieInventory(node->rightChild);
	}
}
/*
When the user selects this option, the program will traverse the tree and
singly linked lists and count the total movie nodes in the tree and print the
count.
*/
void MovieTree::countMovieNodes(MovieNodeBST *node, int *c) {
	if(node!=NULL){
    	countMovieNodes(node->leftChild, c);
    	MovieNodeLL *current = node->head;
    	while(current != NULL){
    		current = current->next;
    		*c = *c + 1;
    	}
    	countMovieNodes(node->rightChild, c);
	}
}

MovieNodeBST* MovieTree::searchBST(MovieNodeBST *node, string title){
	if(node==NULL || node->letter==title[0]){
		return node;
	}else if(node->letter > title[0]){
		return searchBST(node->leftChild, title);
	}else{
		return searchBST(node->rightChild, title);
	}
}

MovieNodeLL* MovieTree::searchLL(MovieNodeLL* head, string title){
	MovieNodeLL *curr =  head;
	while(curr != NULL){
		if(curr->title == title){
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}

MovieNodeBST* MovieTree::treeMinimum(MovieNodeBST *node){
	while(node->leftChild!=NULL){
		node = node->leftChild;
	}
	return node;
}