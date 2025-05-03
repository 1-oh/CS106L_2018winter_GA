#include <iostream>
#include <fstream>
#include "SimpleGraph.h"
#include <string>
#include <cmath>
#include <ctime>

using namespace std;
using std::cout;	using std::endl;

const double PI = 3.14159265358979323;
#define k_repel 1e-3
#define k_attract 1e-3

void Welcome();
void InputPrompt(ifstream & myfile);
void InitialMap(SimpleGraph & g, ifstream & myfile);
void InitialNodePos(SimpleGraph & g);
double Distance(SimpleGraph & g, size_t index1, size_t index2);
double RepelForce(SimpleGraph & g, size_t index0, size_t index1);
double AttractForce(SimpleGraph & g,size_t IndexOfEdge);
double Theta(SimpleGraph & g, size_t index0, size_t index1);
void MoveNode(SimpleGraph & g);

// Main method
int main() {
    Welcome();

    /*The graph of our program*/
    SimpleGraph mygraph;
    /*the stream to read the file*/
    ifstream myFile;

    InputPrompt(myFile);
    InitialMap(mygraph, myFile);
    InitialNodePos(mygraph);

    InitGraphVisualizer(mygraph);
    DrawGraph(mygraph);

    time_t startTime = time(NULL);
    double elapsedTime=0;
    do{
        MoveNode(mygraph);
        DrawGraph(mygraph);
        elapsedTime = difftime(time(NULL), startTime);
    }while(elapsedTime < 15);

    DrawGraph(mygraph);
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl; 
}

/*Prompt the user to input the file's name that he wants to open*/
void InputPrompt(ifstream & myfile ){
    string fileName="";
    cout << "Please Input File Name:>" ;

    while (getline(cin, fileName)) {
       myfile.open(fileName);/*e.g. 10 grid, 50 line,....*/
       if(myfile.fail()) {
           cout << "Please reenter valid file name>" << endl;
        }
        else{
           break;
        }
    }
}

/*Initial the map from the file*/
void InitialMap(SimpleGraph & g, ifstream & myfile){
    int NumOfNode;
    myfile >> NumOfNode;
    g.nodes.resize(NumOfNode);

    size_t n1, n2;

    while(myfile >> n1 >>n2){
        g.edges.push_back(Edge{.start = n1, .end = n2});
    }
}

/*Initial the Position of Nodes on a circle*/
void InitialNodePos(SimpleGraph & g){
    size_t NumOfNode = g.nodes.size();

    for (size_t i = 0;i < NumOfNode;i += 1){
        g.nodes[i].x = cos(2 * PI * i/NumOfNode);
        g.nodes[i].y = sin(2 * PI * i/NumOfNode);
    }
}

/*Compute the distance between two nodes*/
double Distance(SimpleGraph & g, size_t index0, size_t index1){
    double dx = g.nodes[index0].x-g.nodes[index1].x;
    double dy = g.nodes[index0].y-g.nodes[index1].y;
    return sqrt(dx * dx + dy * dy);
}

double RepelForce(SimpleGraph & g, size_t index0, size_t index1){
    return k_repel/Distance(g, index0, index1);
}

double AttractForce(SimpleGraph & g,size_t IndexOfEdge){
    double x = Distance(g, g.edges[IndexOfEdge].start, g.edges[IndexOfEdge].end);
    return k_attract * x * x;
}

double Theta(SimpleGraph & g, size_t index0, size_t index1){
    return atan2(g.nodes[index1].y-g.nodes[index0].y, g.nodes[index1].x -g.nodes[index0].x);
}

void MoveNode(SimpleGraph & g){
    size_t NumOfNode = g.nodes.size();
    size_t NumOfEdge = g.edges.size();
    vector<Node> delta;
    delta.resize(NumOfNode);
    for(size_t i = 0; i < NumOfNode ; i++){
        delta[i].x = 0;
        delta[i].y = 0;
    }

    /*The Repel Force Impact*/
    for(size_t i = 0; i < NumOfNode; i++){
        for(size_t j = i + 1; j < NumOfNode; j++){
            double force = RepelForce(g, i, j);
            double theta = Theta(g, i ,j);

            delta[i].x -= force*cos(theta);
            delta[i].y -= force*sin(theta);
            delta[j].x += force*cos(theta);
            delta[j].y += force*sin(theta);
        }
    }

    /*The Attract Force Impact*/
    for(size_t i = 0; i < NumOfEdge; i++){
        double force = AttractForce(g, i);

        size_t index0 = g.edges[i].start;
        size_t index1 = g.edges[i].end;

        double theta =Theta(g, index0, index1);

        delta[index0].x += force*cos(theta);
        delta[index0].y += force*sin(theta);
        delta[index1].x -= force*cos(theta);
        delta[index1].y -= force*sin(theta);
    }

    for(size_t i = 0; i < NumOfNode ; i++){
        g.nodes[i].x += delta[i].x;
        g.nodes[i].y += delta[i].y;
    }

}
