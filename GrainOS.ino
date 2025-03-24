struct File {
    String name;
    String content;
    bool used;
};

const int MAX_FILES = 10;  //file limit
File files[MAX_FILES];     


void clearScreen() {
    for (int i = 0; i < 50; i++) {
        Serial.println();         //function that clears your screen (again this is stupid)
    }
}


void listFiles() {
    Serial.println("Files:");
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used) {                    //FUNCTION THAT MAKES LS WORK
            Serial.println("- " + files[i].name);
        }
    }
}


void writeFile(String name, String content) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) {  
            files[i].name = name;               //function that writes to file
            files[i].content = content;
            files[i].used = true;
            Serial.println("File written: " + name);
            return;
        }
    }
    Serial.println("Error: No space left in RAM!");
}

void readFile(String name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && files[i].name == name) {
            Serial.println("Content of " + name + ": " + files[i].content); //function that reads written files
            return;
        }
    }
    Serial.println("Error: File not found!");
}


void deleteFile(String name) {
    for (int i = 0; i < MAX_FILES; i++) {
        if (files[i].used && files[i].name == name) {
            files[i].used = false;  // Marks files as deleted
            Serial.println("Deleted file: " + name);
            return;
        }
    }
    Serial.println("Error: File not found!"); // WAH WAH WAH 
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        input.trim();

        if (input == "clear") {
            clearScreen(); //clears screen (very scuffed)
        }
        else if (input.startsWith("echo ")) {
            String message = input.substring(5); // It's just echo with less functionality

            message.trim();
            Serial.println(message);
        }
        else if (input.startsWith("format(") && input.endsWith(")")) {
            input = input.substring(7, input.length() - 1);
            input.replace("\"", ""); //Formatting for Yasmina :)
            input.replace(",", " ");
            Serial.println(input);
        }
        else if (input.startsWith("write ")) {
            int space = input.indexOf(' ', 6);
            if (space != -1) {
                String name = input.substring(6, space); // writes to files, I didn't know what to call this command so just write, fuck It
                String content = input.substring(space + 1);
                writeFile(name, content);
            } else {
                Serial.println("Usage: write <filename> <content>");
            }
        } 
        else if (input.startsWith("read ")) {
            String name = input.substring(5); // This is your cat, but this was simpler to write, fuck it
            readFile(name);
        } 
        else if (input.startsWith("delete ")) {
            String name = input.substring(7); //deletes files (has no flags)
            deleteFile(name);
        } 
        else if (input == "ls") { //list files (currently directories don't exist (and let's be real probably won't))
            listFiles();
        } 
        else {
            Serial.println("Unknown command.");
        }
    }
}
