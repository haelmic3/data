#C++ Code

# Build instruction

make -j 5
to build all executables
j flag is optional but 5 is most number of threads that can speed up compile time.

make xen
for just xen
make hash
make comp


## xen an animated image viewer.

on a debian based linux distro the following are the required package names.

 -	libgraphicsmagick1-dev
 -	libsfml-dev

they must be installed for `make` to compile xen

xen takes a filestream as input for example

ls -d path/\*.jpg | ./xen

will open every image found through the `ls` call on one page.

you can seperate groups of files into pages by putting 2 newlines between files

./xen < file

Right and Left arrow keys move forward and back Spacebar also cycling foward.

Pressing `F` dissplays framerates as a debuging feature.

## hash is a file compare function

the attempt was  to analize files and give them a token that describes the file.
Files tend to only have same hashes if they are the same file.
Files that are simular sometimes have simular hashes.

This is mostly a demonstration of multi threaded batches for file parsing.

accepts a filestream of newline seperated filenames with hard or relitive paths.

ls -d path/\* |./hash > output.txt
./hash < file > output.txt

it's reccomended to pipe the output to a file because the program is slow.

## comp is a utility 

comp takes in output from hash and outputs double line seperated groups of matching hashes.

typically if there aren't many copies of files in the fed in hashed list then it will return nothing.

output of comp can be given to xen if the files are all and only images.
