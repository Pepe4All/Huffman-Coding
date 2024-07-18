#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <string>
#include <cstring>
#include <set>
#include <cmath>
#include <bitset>
#include <list>

using namespace std;

// function to convert an integer to a string with the desired size
string int_to_binary_string(int value, int bits)
{
    string output;
    bitset<8> binary_value(value);
    string binary_to_str_val = binary_value.to_string();

    for (int k = binary_to_str_val.size() - bits; k < binary_to_str_val.size(); k++)
    {
        output += binary_to_str_val[k];
    }

    return output;
}

//---------------------------------------------------------------------------------------------------------

// function to quit the program
void quit()
{

    char input;

    while (true)
    {
        cout << "\nAre you sure? (Y/N): ";
        cin >> input;
        if (input == 'Y' || input == 'y')
        {
            cout << "The program will end\n";
            exit(0);
        }
        else if (input == 'N' || input == 'n')
        {
            cout << "The program will continue\n\n\n\n";
            return;
            break;
        }
        else
        {
            cout << "Wrong character, try again.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
        }
    }
} // end of quit function

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

int check_file_name_validity(string file_name, list<string> valid_extensions)
{

    int flag; // output

    string file_name_extension = ""; // variable to store the file extension

    // loop to get the file extension
    for (int i = 0; i < file_name.size(); i++)
    {
        // cout << file_name[i] << endl;
        if (file_name[i] == '.')
        {
            for (int k = i; k < file_name.size(); k++)
            {
                file_name_extension += file_name[k];
                // cout << file_name_extension;
            }
        }
    }

    //            cout << file_name << " | " << file_name_extension << endl;

    string bare_file_name;
    for (int i = 0; i < file_name.size(); i++)
    {
        if (file_name[i] == '.')
        {
            break;
        }
        bare_file_name += file_name[i];
    }

    if (bare_file_name.empty())
    {
        flag = 3;
        return flag;
    }

    // check if the extension is empty
    if (file_name_extension.empty())
    {
        flag = 2;
        return flag;
    }

    // check if the file extension is among the list
    for (auto i : valid_extensions)
    {
        // cout << i << endl;
        if (i == file_name_extension)
        {
            flag = 0;
            break;
        }
        else
        {
            flag = 1;
        }
    }

    // cout << bare_file_name << endl;
    // cout << file_name_extension << endl;
    return flag;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

// function to compress a file
void compress()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string file_name = "";
    cout << "\n\nTo quit, type in \"quit\"\nTo go back, type in \"back\"\n\n";
    cout << "Enter the file name: ";
    getline(cin, file_name); // use getline function in case there are spaces in the name
    // cout << file_name << endl;

    // cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // conditions when back or quit is typed in
    if (file_name == "back")
    {
        return;
    }

    if (file_name == "quit")
    {
        quit();
    }

    list<string> valid_extensions{".txt"};

    /* when
     flag = 0; everything's good
     flag = 1; extension not supported
     flag = 2; extension is blank
     flag = 3; file name is blank
    */

    int flag = check_file_name_validity(file_name, valid_extensions);
    while (true)
    {
        switch (flag)
        {
        case 0:
            cout << "The file name is valid\n";
            break;

        case 1:
            cout << "The program doesn't support the specified extension. Press enter to try again.\n";
            compress();
            break;

        case 2:
            cout << "The file extension cannot be blank. Press enter to try again.\n";
            compress();
            break;

        case 3:
            cout << "The file name cannot be blank. Press enter to try again. \n";
            compress();
            break;
        }
        break;
    }

    // open the file and read it's contents
    ifstream read_file;

    read_file.open(file_name);

    if (!read_file.is_open())
    {
        cout << "File doesn't exist. Press enter to try another one." << endl;
        compress();
    }

    // get the data from the file
    string line; // a variable to temporarily store each line

    // use a vector to store each line
    vector<string> input_data;
    while (getline(read_file, line))
    {
        input_data.push_back(line);
    }

    // create a map variable to store every unique character
    map<char, int> char_map;
    for (int i = 0; i < input_data.size(); i++)
    {
        string vector_line = input_data[i];
        for (char c : vector_line)
        {
            char_map.insert({c, 0});
        }
    }

    // calculate the required bits
    unsigned int bits = 0;
    for (int i = 0; i <= 8; i++)
    {
        if (pow(2, i) < char_map.size())
        { // when 2^i is enough to store every character in the map, stop assigning bits variable to i
            bits = i;
        }
    }
    bits++; // add one to the bit size

    // check if the file is worth compressing
    if (bits > 7)
    {
        cout << "Too many unique characters in this file. Choose another one." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        compress();
    }

    // assign a code to every unique character
    int var = 0; // temporary variable
    for (auto &i : char_map)
    {
        i.second = var;
        //    cout << i.first << " | " << i.second << endl;
        var++;
    }

    // create a new name for the output file
    string write_file_name;

    for (int i = 0; i < file_name.size(); i++)
    {
        if (file_name[i] == '.')
        {
            break;
        }
        write_file_name += file_name[i];
    }
    write_file_name += "_compressed.bin";

    // create the new file with the new name
    fstream write_file(write_file_name, ios::out | ios::binary);

    // display a message in case the file could not be created
    if (!write_file)
    {
        cout << "Error creating a new file. Try again." << endl;
        compress();
    }

    //  first byte is the number of bits
    //  second byte is the number of unique characters in the map
    //  after the 2 bytes, the following will be a character's ASCII values and then the binary codes
    //  to divide the data line-by-line write the line length in 2 bytes before the actual data
    //  lastly, the encoded data will be sent

    // write the first byte
    write_file.write(reinterpret_cast<char *>(&bits), 1);

    // write the second byte
    int char_map_size = char_map.size();
    write_file.write(reinterpret_cast<char *>(&char_map_size), 1);

    // write the map
    for (auto i : char_map)
    {
        write_file.write(&i.first, 1);
        write_file.write((reinterpret_cast<char *>(&i.second)), 1);
    }

    // write the data

    // loop through every line in the memory
    for (int i = 0; i < input_data.size(); i++)
    {
        string regular_data = input_data[i];
        string encoded_data;

        // loop through every character in the line
        for (int k = 0; k < regular_data.size(); k++)
        {
            encoded_data += int_to_binary_string(regular_data[k], bits);
        }

        int encoded_data_length = encoded_data.size();

        // Convert integer value to 16-bit unsigned integer
        uint16_t sixteenBitVal = static_cast<uint16_t>(encoded_data_length);

        // Split 16-bit value into two 8-bit values
        uint8_t highByte = static_cast<uint8_t>(sixteenBitVal >> 8);
        uint8_t lowByte = static_cast<uint8_t>(sixteenBitVal & 0xFF);

        // write 2 bytes to define number of characters in the line

        write_file.write((reinterpret_cast<char *>(&highByte)), 1);
        write_file.write((reinterpret_cast<char *>(&lowByte)), 1);

        if (encoded_data_length % 8 != 0)
        {
            int paddingLength = 8 - (encoded_data_length % 8);
            encoded_data.append(paddingLength, '0');
        }

        for (int n = 0; n < encoded_data_length; n += 8)
        {
            // convert the 8-bit binary string to an int and then to a char
            string byteString = encoded_data.substr(n, 8);
            unsigned char byte = static_cast<unsigned char>(stoi(byteString, nullptr, 2));
            write_file.write(reinterpret_cast<const char *>(&byte), 1);
        }
    }

    write_file.close();
    cout << "File compressed successfully\n\n\n";
    return;
} // end of compress function

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

// function to decompress a file
void decompress()
{
    cout << "decompress\n\n\n\n";
} // end of decompress function

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{

    // display the welcome text
    string WelcomeMessage = "************* file compressor v1.2 *************\nThis program uses Huffman coding to compress and\ndecompress text files without any data loss\n\nWritten by: Akaki Gvelesiani           14/4/2023\n************************************************\n\n\n";
    cout << WelcomeMessage;

top:

    // ask the user to choose what to do

    while (true)
    {

        cout << "To compress a file, input:    1\nTo decompress a file, input:  2\nTo quit the program, input:   3\n";

        int select_mode;
        cout << "Enter the input: ";
        while (!(cin >> select_mode))
        { // loop until an integer is entered
            cout << "Invalid input. Please enter an integer.\n\n\n\n";
            cin.clear(); // clear the error flag on cin

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard the input buffer
            goto top;
        }

        switch (select_mode)
        {
        case 1:
            compress();
            break;
        case 2:
            decompress();
            break;
        case 3:
            quit();
            return 0;
        default:
            cout << "Wrong input, try again.\n";
            continue;
        }

        // If the switch statement was executed, the program goes back to the top of the loop
        // Otherwise, it loops back to getting input
    }

    goto top;

    cout << "\nEnd of scope\n";
    return 0;
}