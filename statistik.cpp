#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <algorithm>

#include "statistik.hpp"

unsigned long long RowCount(const char * filename, const char seperator) {
    //creating filebuffer
    unsigned int data_len = 100000 * sizeof (char);
    char * data = (char *) std::malloc (data_len);

    //opening file and checking
    std::ifstream file (filename);
    if(!file.is_open()){
        std::cout << "File not open" << std::endl;
        return 0;
    }

    //countint the rows
    unsigned long long rows_count = 0;
    file >> data;
    for (unsigned long long i = 0; data[i] != '\0'; i++) {
        if (data[i] == seperator && data[i+1] != '\0'){
            rows_count++;
        }
    }
    rows_count+=2;

    //closing resource and return value
    file.close();
    std::free(data);
    return rows_count;
}

unsigned long long LineCount(const char * filename) {
    //opening file and checking
    std::ifstream file (filename);
    if(!file.is_open()){
        std::cout << "File not open" << std::endl;
        return 0;
    }

    //Counting the lines
    unsigned long long lines_count = std::count(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), '\n');

    //closing resource and return value
    file.close();
    return lines_count;
}

char *** CreateFileArray(const unsigned long long lines_count, const unsigned long long rows_count) {
    //creating the lines
    char *** filearray = (char ***) std::malloc(lines_count * sizeof(char **));

    //creting the rows for each line
    for(unsigned long long i = 0; i < lines_count; i++) {
        filearray[i] = (char **) std::malloc(rows_count * sizeof(char*));
    }

    return filearray;
}

void FillFileArray(const char * filename, const char seperator, char *** filearray, const unsigned long long lines_count) {
    //creating filebuffer
    unsigned int data_len = 100000 * sizeof (char);
    char * data = (char *) std::malloc (data_len);

    //opening file and checking
    std::ifstream file (filename);
    if(!file.is_open()){
        std::cout << "File not open" << std::endl;
        return;
    }

    //reading the file in
    for (unsigned long long lines = 0; file && lines < lines_count; lines++){
        file.getline(data, data_len);   //get the line
        unsigned long long last = 0;    //the last occurence of the seperator
        unsigned long long row = 0;     //the row iterated througn
        unsigned long long pos = 0;     //the pos of the courrent line
        for(pos = 0; data[pos] != '\0'; pos++){
            if(data[pos] == seperator && data[pos + 1] != '\0') {
                int len = 1 + pos - last;
                filearray[lines][row] = (char *) std::malloc((1 + len) * sizeof(char));
                std::memcpy(filearray[lines][row], &data[last], len);
                filearray[lines][row][len - 1] = '\0';
                last = pos + 1;
                row++;
            }
        }
        int len = 1 + pos - last;
        filearray[lines][row] = (char *) std::malloc((1 + len) * sizeof(char));
        std::memcpy(filearray[lines][row], &data[last], len);
        filearray[lines][row][len - 1] = '\0';
    }
    
    file.close();
}

bool * CheckThrough(const unsigned long long lines_count, const unsigned long long rows_count, char *** filearray) {
    //creating additional componentes
    unsigned long long * amount = (unsigned long long *) std::malloc(lines_count * sizeof(unsigned long long));
    bool * useless_row = (bool *) std::malloc(rows_count * sizeof(bool));
    for(unsigned long long i = 0; i < rows_count; i++) useless_row[i] = false;

    //go through row by row
    for (unsigned long long row = 0; row < rows_count; row++) {
        for(unsigned long long i = 2; i < lines_count; i++) amount[i] = 1; //reset amount 
        for (unsigned long long line = 1; line < lines_count; line++) {
            for (unsigned long long i = 1; i <= line; i++) {
                //the string compare
                if(std::strcmp(filearray[line][row], filearray[i][row]) == 0) {
                    amount[i]++;
                    break;
                }
            }
        }
        //checking if row is useless
        for(unsigned long long index = 1; index < lines_count; index++) {
            if(amount[index] >= 0.9 * lines_count) {
                useless_row[row] = 1;
                break;
            }
        }
    }

    std::free(amount);
    return useless_row;
}

void WriteFile(const char * filename, const char seperator, const unsigned long long lines_count, const unsigned long long rows_count, char *** filearray, bool * useless_row) {
    //opening file and checking
    std::ofstream file;
    file.open(filename, std::ios::out);
    if(!file.is_open()){
        std::cout << "File not open" << std::endl;
        return;
    }

    //writing line by line
    for(unsigned long long line = 0; line < lines_count; line++){
        for(unsigned long long row = 0; row < rows_count; row++){
            if(useless_row[row]) continue;      //skipp useless rows
            file << filearray[line][row];
            file << seperator;
        }
        file << "\n";
    }
    file.close();
}

void Analyze(const char * filename, const char * destfilename, const char seperator, const double percentage) {

    std::cout << "Getting dimensions of the file" << std::endl;
    const unsigned long long rows_count = RowCount(filename, seperator);
    const unsigned long long lines_max_count = LineCount(filename);
    const unsigned long long lines_count = lines_max_count * percentage;
    
    if(rows_count == 0 || lines_max_count == 0) {
        std::cout << "Error File not found or empty" << std::endl;
        return;
    }
    std::cout << "Allocating Memory for Array" << std::endl;
    char *** filearray = CreateFileArray(lines_max_count, rows_count);
    std::cout << "Filling file data into Array" << std::endl;
    FillFileArray(filename, seperator, filearray, lines_max_count);
    std::cout << "Checking for useless Rows" << std::endl;
    bool * useless_row = CheckThrough(lines_count, rows_count, filearray);
    std::cout << "Write into file " << destfilename << std::endl;
    WriteFile(destfilename, seperator, lines_max_count, rows_count, filearray, useless_row);
}