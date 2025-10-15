struct File {
    String name;
    String content;
    bool used;
};

constexpr int MAX_FILES = 5;  //file limit
File files[MAX_FILES];

void clearScreen() {
    for (int i = 0; i < 50; ++i) {
        Serial.println();         //function that clears your screen (again this is stupid)
    }
}

void listFiles() {
    Serial.println("Files:");
    for (const File& file : files) {
        if (file.used) {                    //FUNCTION THAT MAKES LS WORK
            Serial.println("- " + file.name);
        }
    }
}

void writeFile(const String& name, const String& content) {
    for (File& file: files) {
        if (!file.used) {
            file.name = name;               //function that writes to file
            file.content = content;
            file.used = true;
            Serial.println("File written: " + name);
            return;
        }
    }
    Serial.println("Error: No space left in RAM!");
}

void readFile(const String& name) {
    for (const File& file : files) {
        if (file.used && file.name == name) {
            Serial.println("Content of " + name + ": " + file.content); //function that reads written files
            return;
        }
    }
    Serial.println("Error: File not found!");
}

void deleteFile(const String& name) {
    for (File& file : files) {
        if (file.used && file.name == name) {
            file.used = false;  // Marks files as deleted
            Serial.println("Deleted file: " + name);
            return;
        }
    }
    Serial.println("Error: File not found!"); // WAH WAH WAH
}

void showHelp() { //I think this is fairly self explanatory, but still, making sure. It's just the guts of the `help` command
    Serial.println("Available commands:");
    Serial.println("  clear - Clears the screen");
    Serial.println("  echo <message> - Prints the message");
    Serial.println("  format(\"text,text,text\") - Formats input");
    Serial.println("  write <filename> <content> - Writes content to a file");
    Serial.println("  read <filename> - Reads and prints the content of a file");
    Serial.println("  delete <filename> - Deletes a file");
    Serial.println("  ls - Lists all files");
    Serial.println("  help - Displays this help message");
    Serial.println("  calc <number> <operator> <number> - calculates command");
    Serial.println("  run <script.gs> - Runs a script file");
    Serial.println("  loop <number> <command> - Loops a command the inputted amout of times");
    //Serial.println("  high <pin-number> - sets the inputted pin to high");// 12
    //Serial.println("  low <pin-number> - sets the inputted pin to low"); //13
}

void calculate(const String& input) {
    int firstSpace = input.indexOf(' ');
    int secondSpace = input.indexOf(' ', firstSpace + 1); // this is likely not the best way to do this as It takes up a *lot* of space, but hey. It works. I'll see If I keep It.

    if (firstSpace == -1 || secondSpace == -1) {
        Serial.println("Usage: calc <number> <operator> <number>");
        return;
    }

    char op = input.charAT(firstSpace + 1);
    String num1Str = input.substring(0, firstSpace);
    String num2Str = input.substring(secondSpace + 1);

    float num1 = num1Str.toFloat();
    float num2 = num2Str.toFloat();
    float result;

    if (op == '+') {
        result = num1 + num2;
    }
    else if (op == '-') {
        result = num1 - num2;
    }
    else if (op == '*') {
        result = num1 * num2;
    }
    else if (op == '/') {
        if (num2 == 0) {
            Serial.println("Error: Division by zero");
            return;
        }
        result = num1 / num2;
    }
    else {
        Serial.println("Error: Unknown operator. Use +, -, *, or /");
        return;
    }

    Serial.println(String(result));
}

void runScript(const String& name) { //script interpreter!
    if (!name.endsWith(".gs")) {
        Serial.println("Must end with .gs!");
        return;
    }

    for (const File& file : files) {
        if (file.used && file.name == name) {
            Serial.println("Running script: " + name);
            String content = file.content;
            
            content.replace("\\n", "\n");

            int start = 0;
            while (start < content.length()) {
                int end = content.indexOf('\n', start);
                if (end == -1) end = content.length();

                String command = content.substring(start, end);
                command.trim();

                Serial.print("Extracted Command: [");
                Serial.print(command);
                Serial.println("]");

                if (command.length() > 0) {
                    Serial.println("> " + command);
                    processCommand(command);
                }

                start = end + 1;
            }
            return;
        }
    }

    Serial.println("Error: No script found.");
}

void loopCommand(const String& input) {
    int firstSpace = input.indexOf(' ');
    if (firstSpace == -1) {
        Serial.println("Use `help`");
        return;
    }

    String numStr = input.substring(0, firstSpace); // loop command !
    String command = input.substring(firstSpace + 1);

    int times = numStr.toInt();

    if (times <= 0) {
        Serial.println("Must be positive number");
        return;
    }

    for (int i = 0; i < times; i++) {
        processCommand(command);
    }
}

void setPinLow(const String& input) {
    int pin = input.toInt();
    if (pin >= 0) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
        Serial.println("Pin " + String(pin) + " set to LOW");
    } else {
        Serial.println("Invalid pin number");
    }
}

void setPinHigh(const String& input) {
  int pin = input.toInt();
  if (pin >= 0){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    Serial.println("Pin " + String(pin) + " set to HIGH");

  } else {
    Serial.println("Invalid pin number");
  }

}

void processCommand(const String& input) {
    input.trim();

    if (input == "clear") {
        clearScreen();
    }
    else if (input.startsWith("echo ")) {
        String message = input.substring(5);
        message.trim();
        Serial.println(message);
    }
    else if (input.startsWith("format(") && input.endsWith(")")) {
        input = input.substring(7, input.length() - 1);
        input.replace("\"", "");
        input.replace(",", " ");
        Serial.println(input);
    }
    else if (input.startsWith("write ")) {
        int space = input.indexOf(' ', 6);
        if (space != -1) {
            String name = input.substring(6, space);
            String content = input.substring(space + 1);
            writeFile(name, content);
        } else {
            Serial.println("use `help`");
        }
    }
    else if (input.startsWith("read ")) {
        String name = input.substring(5);
        readFile(name);
    }
    else if (input.startsWith("delete ")) {
        String name = input.substring(7);
        deleteFile(name);
    }
    else if (input == "ls") {
        listFiles();
    }
    else if (input == "help") {
        showHelp();
    }
    else if (input.startsWith("calc ")) {
        calculate(input.substring(5));
    }
    else if (input == "cat"){
        Serial.println(" /\\_/\\");
        Serial.println("( o.o )");
        Serial.println("/> ^ <\\");
    }
    else if (input.startsWith("loop ")) {
        loopCommand(input.substring(5));
    }
    else if (input.startsWith("high ")) {
      setPinHigh(input.substring(5));
    }
    else if (input.startsWith("low ")){
      setPinLow(input.substring(4));
    }
    else {
        Serial.println("Unknown command " + input); //changed this because I realized It makes more sense
    }
}

void setup() {
    Serial.begin(9600);
    delay(500);
    Serial.println("Welcome to GrainOS 1.8.4, `help` for a list of commands");
}

void loop() { //the script runner :)
    if (Serial.available()) {
        String input = Serial.readStringUntil(' \n');
        input.trim();

        if (input.startsWith("run ")) {
            String scriptName = input.substring(4);
            runScript(scriptName);
        } else {
            processCommand(input);
        }
    }
}
