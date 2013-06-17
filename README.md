pca
===

Simple program for principal component analysis

This program calculates the eigenvectors and eigenvalues of the covariance
matrix of a series of samples of particle configurations. It is an analysis
tool for molecular dynamic simulations

Usage
=====

<particles_per_sample> <sample_filename> [ <eigenvector_filename> ] [
<eigenvalue_filename> ]

Input file
==========

The inputfiles must hold <particles_per_samples> lines with 3 tab or
whitespace seperated floating point numbers which a treated as components
of a position vector. There is no separation between the samples, all
have to have the same length and the same particle order.

What is calculated
==================

First the covariance matrix cov(i,j) is calculated, which is

cov(i,j) = < (x[i] - m[i] - com) * (x[j] - m[j] - com) >

where x[i] is the i-th position, m[i] is the mean of the i-th position. < . >
means "average over samples" and com_l = ( x[1]_l + x[2]_l + ... +
x[particles_per_sample]_l ) / particles_per_sample, l = x,y,z is the center of
mass.

The the eigenvalues and eigenvectors of cov(i,j) are calculated. They are
sorted in order of ascending eigenvalue.

Build
=====

The program depends on eigen3 [http://eigen.tuxfamily.org].
It can be build by

g++ -I/path/to/eigen3 pca.cc

Utils
=====

The file rewrite_blockfile.c contains a c-file to convert blockfiles of
the espresso simulation packge to input files of the programm.

It can be build by "gcc rewrite_blockfile.c"






