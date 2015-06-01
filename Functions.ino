void ATCmdSetup() {
  bt.listen();
  bt.print(atStart + Initialize + atSetEnd);    //initialize device (only has to be done once)
  bt.print(atStart + accesCode + atSetEnd);     //defines the accesCode this device shares with the others
  bt.print(atStart + deviceType + atSetEnd);    //defines device type (32 bit indicating device type and what is supported)
  bt.print(atStart + searchRSSI + atSetEnd);    //looks for "open" bluetooth devices in the area and lists them with RSSI
}

void listRSSI() {
  bt.listen();
  bt.print(atStart + receiveRSSI + atSetEnd); //looks for devices and their RSSI and lists them
}

void filterCode() {
  if (message.substring(0, 5) == "+INQ:") {
    deviceAddress = splitString(message, ",", 0).substring(5);
    deviceClass = splitString(message, ",", 1);
    deviceRSSI = splitString(message, ",", 2);
    if (RSSIVisible) {
      Serial.println(sqrt(-(int(hexToDec(deviceRSSI))-int(hexToDec("D666")))));
    }
  }
  message = "";
}

String splitString(String str, String devider, int arrayNumber) {
  int previousDevider = 0;
  int deviders = 1;
  for (int i = 0; i < str.length(); i++) {
    if (str.substring(i, i + devider.length()) == devider) {
      deviders++;
    }
  }
  int devideCounter = 0;
  String devidedString[deviders + 1];
  for (int i = 0; i < str.length(); i++) {
    if (str.substring(i, i + devider.length()) == devider) {
      if (devideCounter) {
        devidedString[devideCounter] = str.substring(devidedString[devideCounter - 1].length() + 1, i);
      } else {
        devidedString[0] = str.substring(0, i);
      }
      devideCounter++;
      previousDevider = i + devider.length();
    }
  }
  devidedString[devideCounter] = str.substring(previousDevider, str.length());
  if (arrayNumber > deviders) {
    return "ERROR; array number not found. Array number is to big.";
  } else {
    return devidedString[arrayNumber];
  }
}

unsigned int hexToDec(String hexString) { //converts HEX to DEC
  unsigned int decValue = 0;
  int nextInt;
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}

void showMenu() {
  Serial.println(" ");
  Serial.println(F("HC-05 BlueTooth Programmer"));
  Serial.println(F("1. AT test           a. previous Address + Class + RSSI"));
  Serial.println(F("2. Version           b. print RSSI"));
  Serial.println(F("3. Address           c. rename to 'hc-05'"));
  Serial.println(F("4. Name              d. set baut to 9600"));
  Serial.println(F("5. Role              m. Display this menu"));
  Serial.println(F("6. UART              p. Initializing (can only happen once)"));
  Serial.println(F("7. State             o. Inquire if bluetooth device has an acces code"));
  Serial.println(F("8. Password          i. Define bluetooth class"));
  Serial.println(F("9. Device Reset      u. Set inquire mode to read RSSI"));
  Serial.println(F("j. CmdSetup          y. getting RSSI list"));
  Serial.println(F("k. Role = Slave      l. Role = Master"));
  Serial.println(" ");
}

void commandControl() {
  switch (crud) {
    case 'm':
      showMenu();
      break;
    case '1':
      bt.write("AT\r\n");
      break;
    case '2':
      bt.print(atStart + Version + atQuestEnd);
      break;
    case '3':
      bt.print(atStart + Address + atQuestEnd);
      break;
    case '4':
      bt.print(atStart + Name + atQuestEnd);
      break;
    case '5':
      bt.print(atStart + Role + atQuestEnd);
      break;
    case '6':
      bt.print(atStart + UART + atQuestEnd);
      break;
    case '7':
      bt.print(atStart + State + atQuestEnd);
      break;
    case '8':
      bt.print(atStart + Password + atQuestEnd);
      break;
    case '9':
      bt.print(atStart + Reset + atSetEnd);
      break;
    case 'p':
      bt.print(atStart + Initialize + atSetEnd);
      break;
    case 'o':
      bt.print(atStart + accesCode + atSetEnd);
      break;
    case 'i':
      bt.print(atStart + deviceType + atSetEnd);
      break;
    case 'u':
      bt.print(atStart + searchRSSI + atSetEnd);
      break;
    case 'y':
      listRSSI();
      break;
    case 'l':
      bt.print(atStart + roleIsMaster + atSetEnd);
      break;
    case 'k':
      bt.print(atStart + roleIsSlave + atSetEnd);
      break;
    case 'a':
      printAddressClassRSSI();
      break;
    case 'b':
      Serial.println(log10(hexToDec(deviceRSSI)));
      break;
    case 'c':
      bt.print(atStart + "NAME=HC-05" + atSetEnd);
      break;
    case 'd':
      bt.print(atStart + "UART=9600,0,0" + atSetEnd);
      break;
    case 'j':
      ATCmdSetup();
      break;
  }
}

void printAddressClassRSSI() {
  Serial.print("Address:");
  Serial.print(deviceAddress);
  Serial.print("  Class:");
  Serial.print(deviceClass);
  Serial.print("  RSSI:");
  Serial.println(deviceRSSI);
}














