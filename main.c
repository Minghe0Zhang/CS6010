//
//  main.c
//  CSE-6010 Assignment1
//
//  Created by Karl Gemayel on 8/17/17.
//  Copyright © 2017 Karl Gemayel. All rights reserved.
//
//  Student name: Minghe Zhang
//  Student username: mzhang388

#include <stdio.h>
#include <stdlib.h>     // for rand, srand ,exit
#include <time.h>       // for time, clock
#include <string.h>     // for strncmp

// Use these variables for simplicity of code
#define RED 'R'
#define GREEN 'G'
#define VACANT ' '

// Function Prototypes: generic
void usage(const char *programName);

// Function Prototypes: matrix specific
char**      malloc_matrix   (int n1, int n2);
void        free_matrix     (int n1, int n2, char **a);
void        init_matrix     (int n1, int n2, char **a);
int         simulate        (int n1, int n2, char **a, double f, int step_times);
void        print_matrix    (int n1, int n2, char **a);



// Main function
int main(int argc, const char * argv[]) {
    
    // Start by reading the input from the command line prompt.
    // Note: when no additional arguments are passed through the
    // the command prompt, argc equals 1, and argv[0] is the
    // name of our executable file.
    
    // In this case, the program takes in 3 addition arguments:
    // (1) number of rows, (2) number of columns, (3) number of
    // times to run 'simulate', and (4) value of f
    if (argc != 5) {
        usage(argv[0]);
        return 1;
    }
    
    int n1 = (int) strtol(argv[1], NULL, 10);                   // number of rows
    int n2 = (int) strtol(argv[2], NULL, 10);                   // number of columns
    int numSimulations = (int) strtol(argv[3], NULL, 10);       // number of times to run 'simulate'
    double f = strtod(argv[4], NULL);                   // the satisfiability fraction
    
    // TODO: Your code here.
    char** model;                                       //create neighborhood model
    printf("the f is : %f\n",f);
    model = malloc_matrix(n1, n2);
    init_matrix(n1, n2, model);
    print_matrix(n1, n2, model);                       //the original neighborhood

    int result = simulate(n1, n2, model, f , numSimulations);
    printf("\n\nThis time, the relocation status is : %d\n",result);
    printf("1 means success; 0 means failure\n\n\n");
    print_matrix(n1, n2, model);                       //the neighborhood after optimization
    free_matrix(n1, n2, model);
    //NOTICE
    
    // This part should actually run the simulate function over and over
    // and print the final result.
    
    return 0;
}

/**
 * Print out the usage message.
 *
 * @param programName the name of the executable file.
 */
void usage(const char *programName) {
    printf("usage: %s  n1 n2 s f\n", programName);
    printf("where:\n");
    printf("    n1      : number of rows\n");
    printf("    n2      : number of columns\n");
    printf("    s       : number of simulation iterations\n");
    printf("    f       : minimum fraction for satisfiability\n");
    printf("\nExample: %s 10 10 100 0.3\n", programName);
}

/****** Matrix Specific Functions ******\
 *                                     *
 * These functions define what matrix  *
 * operations can be performed.        *
 *                                     *
 \***************************************/


/**
 * Allocate memory for a 2D matrix
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @return a pointer to the matrix
 */
char **malloc_matrix(int n1, int n2) {
    
    char **mat = NULL;       // pointer to the matrix
    int i;                 // index of mat
    if(n1>0 && n2>0){
        mat = (char **)malloc (n1 * sizeof (char *));
        for(i=0 ; i<n1 ; i++)
            mat[i] = (char *)malloc (n2 * sizeof (char));
    }
    
    else{
        printf("You cannot set n1 or n2 to be negative!! Try again");
        exit(EXIT_FAILURE);
    }    // - check for invalid inputs!
    
    // - update the 'mat' variable
    return mat;
}


/**
 * Free the space allocated for the matrix.
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 */
void free_matrix(int n1, int n2, char **a) {
    
    int i;                                //index of mat
    if(n1>0 && n2>0 && a!= NULL){
        for(i = 0 ; i < n1; i++ ){
                free(a[i]);
        }
    }
    else{
        printf("You cannot set n1 or n2 to be negative!! Try again");
        exit(EXIT_FAILURE);
    }
    // - check for invalid inputs!
}


/**
 * Initialize the matrix.
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 */
void init_matrix(int n1, int n2, char **a) {
    
    // TODO: Your code here. Don't forget to:
    int i,j;                 // index of mat
    int temp;                // a temporary var for storing the random integer in [0,100)
    time_t t ;               // the time at this moment
    srand((unsigned) time(&t)); // generate a seed for generating random numbers
    if(n1 > 0 && n2 > 0){
        for(i = 0; i < n1; i++){
            for(j = 0; j< n2; j++){
                temp = rand() % 100 ;//temp's range is [0,100)
                if(temp>=0 && temp<40)          a[i][j] = RED;
                else if(temp>=40 && temp<80)    a[i][j] = GREEN;
                else                            a[i][j] = VACANT;
                //initialize the model with the help of random numbers
            }
        }
    }
    else{
        printf("You cannot set n1 or n2 to be negative!! Try again");
        exit(EXIT_FAILURE);
    }    // - check for invalid inputs!
    

}

/**
 * Print the matrix to standard output
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 */
void print_matrix(int n1, int n2, char **a) {
    
    // TODO: Your code here. Don't forget to:
    int i,j;                                                //index
    if(n1 > 0 && n2 >0 && a!=NULL)
    {
        for(i = 0 ; i < n1 ; i ++){
            for(j = 0; j < n2 ; j++){
                printf("%c ", a[i][j]);
            }
            printf("\n");
        }
    }
    // - check for invalid inputs!
}


/**
 * Simulate one time-step using matrix 'a', where f determines
 * the minimum number of desired neighbors similar to the occupant.
 * @param n1 the number of rows
 * @param n2 the number of columns
 * @param a the pointer to the matrix
 * @param f the fraction indicating the minimum number of desired similar neighbours
 * @return the number of citizens that moved or left the game in this time-step; -1 if
 * function failed.
 */
int simulate(int n1, int n2, char **a, double f, int step_times) {
    
    // TODO: Your code here. Don't forget to:
    int count = step_times;                             //the number of completed steps
    int dissatisfied = 100;
    int i,j;                                            //index of the block
    int k_row,k_col;                                    //var for counting
    time_t t;                                           //t is to generate random
    if(n1<=0||n2<=0||f<=0||f>1||a==NULL){
        printf("ERROR happens!");
        exit(EXIT_FAILURE);
    }
    //malloc some memory for stroing block information
    int **vacant_block = (int**) malloc(n1 * sizeof(int *));
    for(i=0; i<n1 ; i++)   vacant_block[i]=(int*)malloc(sizeof(int)*n2);
    
    int **neighbor_number = (int**) malloc(n1*sizeof(int*));
    for(i=0; i<n1; i++)    neighbor_number[i]=(int*)malloc(sizeof(int)*n2);
    
    int **neighbor_red = (int**) malloc(n1 * sizeof(int *));
    for(i=0; i<n1 ; i++)   neighbor_red[i]=(int*)malloc(sizeof(int)*n2);
    
    int **neighbor_green = (int**) malloc(n1*sizeof(int*));
    for(i=0; i<n1; i++)    neighbor_green[i]=(int*)malloc(sizeof(int)*n2);
    
    //initialize:
    for(i=0;i<n1;i++)
        for(j=0;j<n2;j++){
            vacant_block[i][j]=0;
            neighbor_number[i][j]=0;
            neighbor_red[i][j]=0;
            neighbor_green[i][j]=0;
        }

    int loc_row,loc_col;          //the location of the dissatisfied man
    
    while(count > 0 && dissatisfied>0){
        count--;                    //substract step_times
        /*First step: update vacant_block,Negighbor's number, green and red number*/
        for(i=0; i<n1; i++)
            for(j=0 ; j<n2 ; j++){
                if(a[i][j]==VACANT) vacant_block[i][j]=1;
                else                vacant_block[i][j]=0;  //set the vacant block first
                
                //reset the neighbor's info(green or red)
                neighbor_number[i][j]=0;
                neighbor_red[i][j]=0;
                neighbor_green[i][j]=0;
                
                //update neighbor's info(green or red)
                for(k_row=-1;k_row<=1;k_row++)
                    for(k_col=-1;k_col<=1;k_col++)
                        if(!(k_row==0&&k_col==0)) //delete itself to get 8 neighbors
                            if(i+k_row>=0&&i+k_row<=n1-1&&j+k_col>=0&&j+k_col<=n2-1){
                                if(a[i+k_row][j+k_col]==RED)
                                    neighbor_red[i][j]++;
                                else if(a[i+k_row][j+k_col]==GREEN)
                                    neighbor_green[i][j]++;
                            }//then update the property of its neighbors
                //update the number of its neighbors
                if(i>0 && i<n1-1 && j>0 && j<n2-1) // means the block is inside
                    neighbor_number[i][j]=8;
                //the block is in the corner, having 3 neighbors
                else if((i==0||i==n1-1)&&(j==0&&j==n2-1))
                    neighbor_number[i][j]=3;
                else    neighbor_number[i][j] = 5;
                
            }
        
        
        /*Second step: find the first dissatisfied man*/
        loc_row=-1;loc_col=-1;
        int find = 0;               // flag for break the first loop
        for(i=0; i<n1 ; i++){
            
            for(j=0 ; j<n2 ; j++){
                if(a[i][j]==VACANT)
                    continue;
                else if(a[i][j]==RED){
                    if(neighbor_number[i][j]*f>neighbor_red[i][j]){//dissatisfied
                        loc_row=i;
                        loc_col=j;
                        find = 1;
                        break;
                    }
                }
                else{
                    if(neighbor_number[i][j]*f>neighbor_green[i][j]){//dissatisfied
                        loc_row=i;
                        loc_col=j;
                        find = 1;
                        break;
                    }
                }
            }
            if(find == 1) break;
        }
        /*Third step:randomly selecting a starting block*/
        int start_row,start_col;                    //starting location
        int present_loc_row,present_loc_col;        //present location
        srand((unsigned) time(&t)+count); // generate a seed for generating random numbers
        start_row = rand()%n1;
        start_col = rand()%n2;
        printf("starting location is %d,%d\n",start_row,start_col);
        
        /*Fourth step: find a vancant and also satisfied block, then move in*/
        int flag;                   //flag is to mark the Green/Red property of the dissatisfied man
        if (a[loc_row][loc_col] == RED)    flag = 0;           //0 presents RED
        else                               flag = 1;           //1 presents GREEN
        
        a[loc_row][loc_col]=VACANT; //leave the original block first
        vacant_block[loc_row][loc_col] = 1;
        
        present_loc_col = start_col;//starting at start block
        present_loc_row = start_row;
        
        int find_nice = 0;
        //from starting location to the bottommost and rightmost corner
        while(find_nice==0&&(!(present_loc_row==n1-1)&&(present_loc_col==n2-1))){
            if(present_loc_row!=n1-1)
                present_loc_row++;
            else{
                present_loc_row = 0;
                present_loc_col++;
            }
            if(vacant_block[present_loc_row][present_loc_col] == 0)
                continue;
            //judge whether this vancant block can be satisfied
            else{
                if(flag==0){                    //RED
                    if(neighbor_number[present_loc_row][present_loc_col]*f<=neighbor_red[present_loc_row][present_loc_col])
                    {
                        find_nice = 1;
                        a[present_loc_row][present_loc_col] = RED;
                        vacant_block[present_loc_row][present_loc_col] = 0;
                        break;
                    }
                    else                        continue; //dissatisfied
                }
                else{                           //GREEN
                    if(neighbor_number[present_loc_row][present_loc_col]*f<=neighbor_green[present_loc_row][present_loc_col])
                    {
                        find_nice = 1;
                        a[present_loc_row][present_loc_col] = GREEN;
                        vacant_block[present_loc_row][present_loc_col] = 0;
                        break;
                    }
                    else                        continue; //dissatisfied
                    
                }
            }
        }
        
        //Now we are at the rightmost and bottommost corner, return to left top
        present_loc_row=0;
        present_loc_col=0;
        //repeat the searching process
        while(find_nice==0&&(!(present_loc_row!=loc_row&&present_loc_col!=loc_col))){
            if(present_loc_row!=n1-1)
                present_loc_row++;
            else{
                present_loc_row = 0;
                present_loc_col++;
            }
            if(vacant_block[present_loc_row][present_loc_col] == 0)
                continue;
            //judge whether this vancant block can be satisfied
            else{
                if(flag==0){                    //RED
                    if(neighbor_number[present_loc_row][present_loc_col]*f<=neighbor_red[present_loc_row][present_loc_col])
                    {
                        find_nice = 1;
                        a[present_loc_row][present_loc_col] = RED;
                        vacant_block[present_loc_row][present_loc_col] = 0;
                        break;
                    }
                    else                        continue; //dissatisfied
                }
                else{                           //GREEN
                    if(neighbor_number[present_loc_row][present_loc_col]*f<=neighbor_green[present_loc_row][present_loc_col])
                    {
                        find_nice = 1;
                        a[present_loc_row][present_loc_col] = GREEN;
                        vacant_block[present_loc_row][present_loc_col] = 0;
                        break;
                    }
                    else                        continue; //dissatisfied
                }
            }
            
        }
        
        printf("the dissatisfied location is %d, %d\n",loc_row,loc_col);
        
        /*Fifth step: update the number of dissatisfied man*/
        dissatisfied = 0;
        for(i=0;i<n1;i++)
            for(j=0;j<n2;j++){
                if(a[i][j]==VACANT)
                    continue;
                else if(a[i][j]==RED){
                    if(neighbor_number[i][j]*f>neighbor_red[i][j])//dissatisfied
                        dissatisfied++;
                }
                else{
                    if(neighbor_number[i][j]*f>neighbor_green[i][j])//dissatisfied
                        dissatisfied++;
                }
            }
        printf("the number of dissatisfied man is: %d\n",dissatisfied);
    
        }
    /*Final step: free memory*/
    for(i = 0 ; i < n1; i++ ){
        free(vacant_block[i]);
        free(neighbor_green[i]);
        free(neighbor_red[i]);
        free(neighbor_number[i]);
    }

    // - update the return value based on success/failure
    if(dissatisfied==0)
        return 1;               //success
    else
        return 0;               //failure
    
}


