#pragma once

#ifndef DEF_GAUSS
#define DEF_GAUSS


// C++ program to demonstrate working of Gaussian Elimination
// method
//#include<bits/stdc++.h>
#include<iostream>
using namespace std;

#define N 2        // Number of unknowns

// function to reduce matrix to r.e.f.  Returns a value to 
// indicate whether matrix is singular or not
int forwardElim(double mat[N][N + 1]);

// function to calculate the values of the unknowns
void backSub(double mat[N][N + 1], double y[N]);

// function to get matrix content
void gaussianElimination(double mat[N][N + 1], double y[N]);

// function for elementary operation of swapping two rows
void swap_row(double mat[N][N + 1], int i, int j);

// function to print matrix content at any stage
void print(double mat[N][N + 1]);


#endif // !DEF_GAUSS
