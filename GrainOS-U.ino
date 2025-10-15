// '''''''''''''''''''''''''''''''''''''''''
//  This is a rewrite for the Arduino Uno  .
//  which will (hopefully) improve memory  .
//  management and prevent ram exhaustion  .
//  to some extent.                        .
//                                         .
//  Changes:                               .
//  - String class object -> C-Strings     .
//  - Constant outputs wrapped with F()    .
//    macro to prevent copies into ram.    .
//  - Replaced else-if commang thingy      .
//    with a switch statement              .
// .........................................

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define MAX_FILES 5
#define MAX_NAME_LEN 20
#define MAX_CONTENT_LEN 180
#define MAX_CALCULATE_LEN 64

struct File {
    char name[MAX_NAME_LEN];
    char content[MAX_CONTENT_LEN];
    bool used;
};

File files[MAX_FILES];

void clearScreen() {
    Serial.print(F("\033[2J")); //Clears terminal
    Serial.print(F("\033[H")); //Return cursors to origin
}

void listFiles() {
    Serial.println(F("Files:"));
    for (const File& file : files) {
        if (file.used) {                    //FUNCTION THAT MAKES LS WORK
            Serial.println(F("- "));
            Serial.println(file.name);
        }
    }
}

void writeFile(char* input) {
    char *name, *content;
    name = strtok(input, " ");
    content = strtok(NULL, " ");
    for (File& file: files) {
        if (!file.used) {

            strncpy(file.name, name, MAX_NAME_LEN - 1);
            file.name[MAX_NAME_LEN - 1] = '\0';

            strncpy(file.content, content, MAX_CONTENT_LEN - 1);
            file.name[MAX_CONTENT_LEN - 1] = '\0';

            file.used = true;

            Serial.println(F("File written: "));
            Serial.println(name);
            return;
        }
    }
    Serial.println(F("Error: No space left in RAM!"));
}

void readFile(const char* name) {
    for (const File& file : files) {
        if (file.used && (strcmp(file.name, name) == 0)) {
            Serial.println(F("Content of "));
            Serial.println(name);
            Serial.println(F(": "));
            Serial.println(file.content); //function that reads written files
            return;
        }
    }
    Serial.println(F("Error: File not found!"));
}

void deleteFile(const char* name) {
    for (File& file : files) {
        if (file.used && (strcmp(file.name, name) == 0)) {
            file.used = false;  // Marks files as deleted
            Serial.println(F("Deleted file: "));
            Serial.println(name);
            return;
        }
    }
    Serial.println(F("Error: File not found!")); // WAH WAH WAH
}

void showHelp() { //I think this is fairly self explanatory, but still, making sure. It's just the guts of the `help` command
    Serial.println(F("Available commands:"));
    Serial.println(F("  clear - Clears the screen"));
    Serial.println(F("  echo <message> - Prints the message"));
    Serial.println(F("  write <filename> <content> - Writes content to a file"));
    Serial.println(F("  read <filename> - Reads and prints the content of a file"));
    Serial.println(F("  delete <filename> - Deletes a file"));
    Serial.println(F("  ls - Lists all files"));
    Serial.println(F("  help - Displays this help message"));
    Serial.println(F("  calc <number> <operator> <number> - calculates command"));
    Serial.println(F("  run <script.gs> - Runs a script file"));
    Serial.println(F("  loop <number> <command> - Loops a command the inputted amout of times"));
    //Serial.println(F("  high <pin-number> - sets the inputted pin to high"));// 12
    //Serial.println(F("  low <pin-number> - sets the inputted pin to low")); //13
}

void calculate(char* input) {
    char *num1_str, *num2_str, *op_str;
    char  op;
    double result;

    if ((num1_str = strtok(input, " ")) == NULL) goto bad_input_calc;

    if ((op_str = strtok(NULL, " ")) == NULL || strlen(op_str) != 1){
        Serial.println(F("Operator missing or greater than a single character!"));
        return;
    }

    if ((num2_str = strtok(NULL, " ")) == NULL) goto bad_input_calc;

    if (strtok(NULL, " ") != NULL){
        Serial.println(F("Too many arguments!"));
        return;
    }
    char* end_ptr;

    double num1 = strtod(num1_str, &end_ptr);
    if (*end_ptr != '\0'){
        Serial.println(F("Invalid first number format!"));
    }

    double num2 = strtod(num2_str, &end_ptr);
    if (*end_ptr != '\0'){
        Serial.println(F("Invalid second number format!"));
    }

    switch(op_str[0]){
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (fabs(num2) == 0.000001f){
                Serial.println(F("Error: Division by zero"));
                return;
            }
            result = num1 / num2;
            break;
        default:
            Serial.println(F("Error: Unknown operator. Use +, -, *, or /"));
            return;
    }

    Serial.println(String(result));

    bad_input_calc:
        Serial.println(F("Usage: calc <number> <operator> <number>"));
        return;
}

void runScript(const char* name) { //script interpreter!

    size_t name_len = strlen(name);
    if (name_len < 3 || strcmp(name + name_len - 3, ".gs") != 0) {
        Serial.println(F("Must end with .gs!"));
        return;
    }

    for (const File& file : files) {
        if (file.used && strcmp(file.name, name) == 0) {

            Serial.println(F("Running script: "));
            Serial.println(name);

            char temp_content[MAX_CONTENT_LEN];
            strncpy(temp_content, file.content, MAX_CONTENT_LEN - 1);

            char* command = strtok(temp_content, "\n");

            while(command != NULL){
                
                  size_t command_len = strlen(command);
                  if(command_len > 0 && command[command_len - 1] == '\r'){
                      command[command_len - 1] = '\0';
                  }
                  if (strlen(command) > 0){
                      Serial.print(F("Extracted Command: ["));
                      Serial.print(command);
                      Serial.println(F("]"));

                      Serial.print(F("> "));
                      Serial.println(command);
                  }
            }
        }
        return;
    }
    Serial.println(F("Error: No script found."));
}

void loopCommand(char* input) {
    char *command_str, *times_str, *end_ptr;
    uint16_t times;

    if ((times_str = strtok(input, " ")) == NULL) goto bad_input_loop;
    if ((command_str = strtok(NULL, " ")) == NULL) goto bad_input_loop;
    
    {
    if (*times_str == '-'){
        Serial.println(F("Must be positive number."));
        return;
    }
    long times_l = strtoul(times_str, &end_ptr, 10);
    if (*end_ptr != '\0') goto bad_input_loop;
    if (times_l == 0) goto bad_input_loop;
    times = (uint16_t) times_l;
    }



    for (uint16_t i = 0; i < times; ++i) {
        processCommand(command_str);
    }

    bad_input_loop:
        Serial.println(F("Bad input arguments. Number not greater than zero or invalid command."));
        return;
}

void setPinLow(const String& input) {
    int pin = input.toInt();
    if (pin >= 0) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        Serial.println(F("Pin ")); 
        Serial.println(String(pin));
        Serial.println(F(" set to LOW"));
    } else {
        Serial.println(F("Invalid pin number"));
    }
}

void setPinHigh(const String& input) {
  int pin = input.toInt();
  if (pin >= 0){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    Serial.println(F("Pin "));
    Serial.println(String(pin));
    Serial.println(F(" set to HIGH"));

  } else {
    Serial.println(F("Invalid pin number"));
  }
}

void processCommand(char* input) {

    if (strlen(input) < 3){
        Serial.println(F("Invalid input"));
        return;
    }

    uint32_t sig = 0x00;
    sig |= input[0]<<16;
    sig |= input[1]<<8;
    sig |= input[2];

    char* parameters;
    parameters = strtok(input, " ");
    uint16_t parSize= strlen(parameters)+1;

    if((*(parameters+parSize) != ' ') && (*(parameters+parSize) != '\r') && 
       (*(parameters+parSize) != '\0') && (*(parameters+parSize) != '\n') ){
        parameters += parSize;
    }
    else{
        Serial.println(F("Parameters missing"));
        return;
    }

    // FIRST PART:
    // number : [          0x00000000          ]
    // after  : [ 0x00 ][char 1][char 2][char 3]
    // which works as an identifier/a signature and
    // makes the following part more efficient
    
    // SECOND PART:
    // "command  <arg1> <arg2>" becomes
    // "command\0<arg1> <arg2>"
    // the pointer is then moved behind \0
    // resulting in "<arg1> <arg2>"

    switch(sig){
        case 0x00636C65: //clear
            clearScreen();
            break;
        case 0x00656368: //echo
            Serial.println(parameters);
            break;
        case 0x00777269: //write
            writeFile(input);
            break;
        case 0x00726561: //read
            readFile(input);
            break;
        case 0x0064656C: //delete
            deleteFile(input);
            break;
        case 0x006C7320: //list
            listFiles();
            break;
        case 0x0068656C: //help
            showHelp();
            break;
        case 0x0063616C: //calculate
            calculate(input);
            break;
        case 0x0072756E: //run
            runScript(input);
            break;
        case 0x006C6F6F: //loop
            loopCommand(input);
            break;
        case 0x00636174: //cat
            Serial.println(F("/\\_/\\"));
            Serial.println(F("( o.o )"));
            Serial.println(F("/> ^ <\\"));
            break;
        default:
            Serial.println(F("Unknown command "));
            Serial.println(input); //changed this because I realized It makes more sense
            break;
    }
}

void setup() {
    Serial.begin(9600);
    delay(500);
    Serial.println(F("Welcome to GrainOS 1.8.4U, `help` for a list of commands"));
}

void loop() { //the script runner :)
    static char input[MAX_CONTENT_LEN + 7];
    static uint8_t inputCounter = 0;

    if (Serial.available()) {
        char inputChar = Serial.read();
        
        if (inputChar == '\n'){
            input[inputCounter] = '\0';
            inputCounter = 0;

            processCommand(input);
        }
        else if (inputCounter < MAX_CONTENT_LEN - 1){
            if (inputChar != '\r'){
                input[inputCounter++] = inputChar;
            }
        }
        else {
            Serial.println(F("Error : Input line too long."));
            inputCounter = 0;
        }  
        memset(input, 0, MAX_CONTENT_LEN + 7);
    }
}
