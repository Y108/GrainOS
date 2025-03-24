void setup() {
  Serial.begin(9600);
  bool Userregistered = false;
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); 
    input.trim();

    if (input == "clear") {
      for (int i = 0; i < 50; i++) {
        Serial.println(); // very, very, VERY scuffed clear command. Adds 50 blank likes to the 'terminal'
      }
    } 
    else if (input.startsWith("echo")) {
      String message = input.substring(5); //simple echo command (absolutely stolen from linux and made even more scuffed!)
      message.trim(); 
      
      if (message.length() > 0) {
        Serial.println(message); 
      }
    }
    else if (input.startsWith("format(")) {
      if (input.endsWith(")")) {
        input = input.substring(7, input.length() - 1); 
        input.replace("\"", ""); 
        input.replace(",", " "); 
        Serial.println(input); //Happy, Yasmina?
      }
    }
  }
}
