/*
    Name: Karanvir Basson
    Title: Quick Hull Algorithm
    Date: 05-06-2022
*/

#include "quickHull.h"

Node* createNode(int x, int y) { //Given an x and y value, create and return a node struct

    Node* node = malloc(sizeof(Node)); //Allocate memory for node
    if (node == NULL) return NULL; //NULL ptr check

    //Initialize node
    node -> x = x;
    node -> y = y;
    node -> visited = 0;
    node -> dest1 = NULL;
    node -> dest2 = NULL;

    return node; //Return allocated node

}

void printNode(Node* node) { //Given a node, print its info

    if (node == NULL) return; //NULL ptr check

    //PRINT NODE INFO
    printf("\n(%d,%d)\n", node -> x, node -> y);
    printf("Visited = %d\n\n", node -> visited);

    if (node -> dest1 != NULL && node -> dest2 != NULL) { //If node on convex hull

        printf("DEST 1: (%d,%d)\n", node -> dest1 -> x, node -> dest1 -> y);
        printf("DEST 2: (%d,%d)\n", node -> dest2 -> x, node -> dest2 -> y);

    }

}

float getDist(Node* node1, Node* node2) { //Given 2 nodes, return distnace between them

    int x = ((node1 -> x) - (node2 -> x)) * ((node1 -> x) - (node2 -> x)); //Calculate (x1-x2)^2
    int y = ((node1 -> y) - (node2 -> y)) * ((node1 -> y) - (node2 -> y)); //Calculate (y1-y2)^2
    
    double res = sqrt(x + y); //Take sqrt of x + y
    return res; //Return result

}

int partition(Node* nodes[NUM_POINTS], int low, int high) { //Partitions array using pivot, used for quicksort

    Node* pivot = nodes[high]; //Pointer to pivot of array
    int i = low - 1;

    for (int j = low; j < high; j++) { //For all nodes

        //Compare nodes[j] and pivot
        int res = (nodes[j] -> x) - (pivot -> x);
        if (res == 0) res = (nodes[j] -> y) - (pivot -> y);

        if (res < 0) { //If nodes[j] < pivot

            i++;

            //swap nodes[i] and nodes[j]
            Node* temp = nodes[i];
            nodes[i] = nodes[j];
            nodes[j] = temp;

        }

    }

    //swap nodes[i+1] and nodes[high] (pivot)
    Node* temp = nodes[i + 1];
    nodes[i + 1] = nodes[high];
    nodes[high] = temp;

    return i + 1; //Return index of pivot

}

void quickSort(Node* nodes[NUM_POINTS], int low, int high) { //Sorts node array using quicksort algorithm

    if (low >= high || low < 0) {
        
        return;

    } else {

        int p = partition(nodes, low, high);
        quickSort(nodes, low, p - 1);
        quickSort(nodes, p + 1, high);

    }

}

int partitionHull(Node* nodes[NUM_POINTS], int low, int high, int sort_upper) { //Partition points into upper/lower hull

    Node* p1 = nodes[low]; //(x1, y1)
    Node* p2 = nodes[high]; //(x2, y2)

    int i = low;  

    for (int j = low + 1; j < high; j++) {

        Node* p3 = nodes[j]; //(x3, y3)

        //det = x1y2 + x3y1 + x2y3 − x3y2 − x2y1 − x1y3
        int det = ((p1 -> x) * (p2 -> y)) + ((p3 -> x) * (p1 -> y)) + ((p2 -> x) * (p3 -> y)) - ((p3 -> x) * (p2 -> y)) - ((p2 -> x) * (p1 -> y)) - ((p1 -> x) * (p3 -> y));
        
        if (sort_upper) {
            
            if (det > 0) { //If p3 in upper hull

                i++;

                //swap nodes[i] and p3
                Node* temp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp;

            }

        } else {
            
            if (det < 0) { //If p3 in lower hull

                i++;

                //swap nodes[i] and p3
                Node* temp = nodes[i];
                nodes[i] = nodes[j];
                nodes[j] = temp;

            }

        }

    }

    //swap nodes[i+1] and nodes[high] (pivot)
    Node* temp = nodes[i + 1];
    nodes[i + 1] = nodes[high];
    nodes[high] = temp;

    return i + 1; //Return index of pivot (p2)

}

void computeHull(Node* nodes[NUM_POINTS], int low, int high, int sort_upper) { //Get upper/lower hull

    Node* p1 = nodes[low]; //(x1, y1)
    Node* p2 = nodes[high]; //(x2, y2)

    int current = 0;
    int pMaxIndex = -1;

    for (int i = low + 1; i < high; i++) {

        Node* p3 = nodes[i]; //(x3, y3)
        int det = ((p1 -> x) * (p2 -> y)) + ((p3 -> x) * (p1 -> y)) + ((p2 -> x) * (p3 -> y)) - ((p3 -> x) * (p2 -> y)) - ((p2 -> x) * (p1 -> y)) - ((p1 -> x) * (p3 -> y));
        
        if (sort_upper) { //If getting upper hull
            
            if (det > current) {

                current = det;
                pMaxIndex = i;

            }

        } else { //If getting lower hull

            if (det < current) {

                current = det;
                pMaxIndex = i;

            }

        }

    }

    if (pMaxIndex == -1) { //If boundary line

        printf("(%d, %d) and (%d, %d) form boundary!\n", p1 -> x, p1 -> y, p2 -> x, p2 -> y);
        
        //ADD DESTINATION TO NODE
        if (p1 -> dest1 == NULL) p1 -> dest1 = p2;
        else p1 -> dest2 = p2;

        if (p2 -> dest1 == NULL) p2 -> dest1 = p1;
        else p2 -> dest2 = p1;

    } else { //If not boundary line

        pMaxIndex = partitionHull(nodes, low, pMaxIndex, sort_upper); //Get upper/lower hull of sub array
        computeHull(nodes, low, pMaxIndex, sort_upper); //Get upper/lower hull of p1 to pMax
        computeHull(nodes, pMaxIndex, high, sort_upper); //Get upper/lower hull of pMax to p2

    }

}
