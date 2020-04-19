# Statistik
This algorithm reads a csv(Char seperated Value) file, and checks for useless cloumns and writes the modified data into a new file

## Build
This software was run and build on linux x64 only.
It will build bin/statistik.out.

## Run
The software is run with args. Minimum argc is 2. Recommended is 3
the first arg is the seperator sign, the second arg is the file, the third is the output file.

### example:
bin/statistik.out "|" "a_ereignis_00.unl" "output.unl"
