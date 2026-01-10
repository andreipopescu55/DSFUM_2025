#include <SPI.h>
#include <SD.h>


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // va asteptam sa deschideti consola seriala
  }
  if (!SD.begin(4)) { // 4 este pinul SS al cardului SD
    Serial.println("Nu putem accesa cardul... aveti un card acolo ? este formatat FAT16 sau FAT32 ?");    
    return; // iese si nu mai face nimic
  }  
  Serial.println("Accesare card -> OK");
}

String command = "";
String currentDir = "/";

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    command += c;
  }

  command.trim();

  if (command.startsWith("cd ")) {
    String path = command.substring(3);
    String newDir =  "/" + path;

    if (SD.exists(newDir)) {
      currentDir = newDir;
      Serial.println("Director curent: " + currentDir);
    } else Serial.println("Director inexistent: " + newDir);
  } 
  else if (command.startsWith("mkdir ")) {
    String name = command.substring(6);
    String fullPath =  "/" + name;

    if (SD.mkdir(fullPath)) 
      Serial.println("Director creat: " + fullPath);
    else
      Serial.println("Eroare la crearea directorului");
  } 
  else if (command.startsWith("rmdir ")) {
    String name = command.substring(6);
    String fullPath = "/" + name;
    if (SD.exists(fullPath)) {
      SD.rmdir(fullPath);
      Serial.println("Director sters: " + fullPath);
    } else Serial.println("Director inexistent: " + fullPath);
  } 
  else if (command == "ls") {
    File dir = SD.open(currentDir);
    File file;
    
    while (file) {
      Serial.print(file.name()); 
      Serial.println(file.size());
      file.close();
    }
    dir.close();
  } 
  else if (command.startsWith("cat ")) {
    String fullPath =  "/" + command.substring(4);
    if (!SD.exists(fullPath)) Serial.println("Fisier inexistent");
    else {
      File f = SD.open(fullPath);
    
      while (f.available()) 
        Serial.write(f.read());
      f.close();
    }
  }
  else if (command.startsWith("echo ")) {
      int idx = command.indexOf('>');
      String text = command.substring(5, idx);
      text.trim();
      if (text.startsWith("\"") && text.endsWith("\"")) 
        text = text.substring(1,text.length()-1);

      String filename = command.substring(idx+1);
      filename.trim();
      String fullPath = currentDir + "/" + filename;
      File f = SD.open(fullPath, FILE_WRITE);

      if (f) {
        f.println(text);
        f.close();
        Serial.println("Am scirs in fisier ");
      } else Serial.println("Eroare la scriere");
    
  }
  else if (command == "cd .") {
    Serial.println("Director curent: " + currentDir);
  }
  
  command = ""; 
}