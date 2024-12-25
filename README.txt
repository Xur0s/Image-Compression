Usage is to take in a digitized image and compress it via the distance transfrom algorithm 
and then de-compress the compressed file using the expansion pass algortihm.

Requires a digitized image with a header containing number of rows & columns and 
minimum and maximum values of the image. 

To run the file, pass in 6 parameters into the terminal: 
input file (digitized image), distance transform choice (either 4, 8, or 10), 
a print file (output), skeleton file (compressed image output), 
decompressed file (decompressed output of skeleton file), and log file

