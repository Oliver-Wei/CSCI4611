import processing.video.*;
// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
PImage destination;
boolean inputMethodSelected = false;
int threshold = 128;
boolean userView = true;
boolean grayMode = true;
PFont font;
char[] inputLetters;
Letter[][] drops;
int count = 3;
int savedTime;
int totalTime = 10;

String rainString = "rain afc on the dof green grass huj but not wmz on me";


class Letter {
  int x;
  int y;
  char textLetter;
  int upSpeed;
  int alpha = 255;
  PFont font = loadFont("Times-Roman-20.vlw");
  int stayTime;
  int allowTime = 1000;

  Letter(char inputText) {
    x = 100;
    y = int(random(-1280,0));
    //y = 100;
    textLetter = inputText;
    // textSize(16);
    textFont(font);
    upSpeed = 1;
    stayTime = 0;
  }
  void drawLetter() {
    fill(100, 0, 66, alpha);
    text(textLetter, x, y);
  }
 
  void letterFade() {
    alpha -= 50;
    // Letter goes to the above of the image after it fades away
    if(alpha <= 70) {
      y = int(random(-1280, 0));
      alpha = 255;
      stayTime = 0;
    }
  }
 
  void dropLetter() {
    y++;
    stayTime++;
    // Letter fades away if it is near the bottom of the image
    if (y > 550) {
      letterFade();
    }
    // Letter fades away if it stays in the image for a long time
    if ((y > 0) && (stayTime > allowTime)){
      letterFade();
    }
  }
 
  void liftLetter() {
    int newY = y - upSpeed;
    if (newY >= 0) {
      y = newY;
    } 
    else {
      y = 0;
    }
  }
}

char[] splitString(String inputString) {
  for (int i = 0; i < inputString.length(); i++) {
    inputLetters[i] = inputString.charAt(i); 
  }
  return inputLetters; 
}


void setup() {
  size(1280, 720);
  savedTime = millis();
  inputImage = createImage(width, height, RGB);
  drops = new Letter[count][rainString.length()];
  int gap = 1280/rainString.length();
  int startX = 5;
  inputLetters = new char[rainString.length()];
  inputLetters = splitString(rainString);
  int startYDiff = 0; 
  for (int i = 0; i < count; i++){
    for (int j = 0; j < inputLetters.length; j++){
      Letter dropLetter = new Letter(inputLetters[j]);
      dropLetter.x = startX;
      dropLetter.y -= startYDiff;
      drops[i][j] = dropLetter;
      startX += gap;
      startX += int(random(-10,10));
      if (startX >= inputImage.width-gap){
        startX = 5;
      }
    }
    startYDiff += 50;
  }
}


void draw() {
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9,cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }


  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.

  
  // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable
  
  if ((cam != null) && (cam.available())) {
    cam.read();
    inputImage.copy(cam, 0,0,cam.width,cam.height, 0,0,inputImage.width,inputImage.height);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
  }



  // Fill in your code to implement the rest of TextRain here..

  // Tip: This code draws the current input image to the screen
  // set(0, 0, inputImage);

  destination = createImage(inputImage.width, inputImage.height, RGB);
  inputImage.loadPixels();
  destination.loadPixels();

  // Flip the video image displayed on the screen
  if (userView == true){
    for (int x = 0; x < inputImage.width; x++) {
      for (int y = 0; y < inputImage.height; y++ ) {
        int loc = x + y*inputImage.width;
        int mirror_loc = inputImage.width - x - 1 + y*inputImage.width;
        // Test the brightness against the threshold
        destination.pixels[mirror_loc]  = inputImage.pixels[loc];  // White
      }
    }
  }
  else {
    for (int x = 0; x < inputImage.width; x++) {
      for (int y = 0; y < inputImage.height; y++ ) {
        int loc = x + y*inputImage.width;
        int mirror_loc = inputImage.width - x - 1 + y*inputImage.width;
        // Test the brightness against the threshold
        if (brightness(inputImage.pixels[loc]) > threshold) {
          destination.pixels[mirror_loc]  = color(255);  // White
        }  else {
          destination.pixels[mirror_loc]  = color(0);    // Black
        }
      }
    }
  }
  destination.updatePixels();
  if (grayMode == true) {
    destination.filter(GRAY);
  }
  set(0,0,destination);
  // Calculate how much time has passed
  // Make the rain fall based on a velocity specified in pixels per second
  int passedTime = millis() - savedTime;
  if (passedTime > totalTime) {
    for (int i = 0; i < count; i++) {
      for (int j = 0; j < inputLetters.length; j++) {
        if (drops[i][j].y < destination.height && drops[i][j].y > 0){
          //  Make rain doesn’t pass through black regions even if they are very thin
          int loc = drops[i][j].x + (drops[i][j].y*destination.width);
          float brightness = brightness(destination.pixels[loc]);
          if (brightness > threshold){
            drops[i][j].dropLetter();
            drops[i][j].upSpeed = 1;
          }
          else {
            if (drops[i][j].y > 0){
              // Lift the letter if the palm of the viewer rises up
              int upLoc = loc = drops[i][j].x + (drops[i][j].y-1)*destination.width;
              float upBrightness = brightness(destination.pixels[upLoc]);
              if (upBrightness < threshold){
                drops[i][j].liftLetter();
                drops[i][j].upSpeed = drops[i][j].upSpeed*2;
              }
            }
          }
        }
        else {
          // The letter drops if it's above the screen
          drops[i][j].dropLetter();
        }
        drops[i][j].drawLetter();
      }
    }
    savedTime = millis(); // Save the current time to restart the timer!
  }
}

void keyPressed() {
  
  if (!inputMethodSelected) {
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        mov = new Movie(this, "TextRainInput.mov");
        mov.loop();
        inputMethodSelected = true;
      }
      else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");           
        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }


  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // Fill in your code to handle keypresses here..
  
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed
      threshold += 1;
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed
      threshold -= 1;
    }
    if (threshold <=0) threshold = 1;
    else if (threshold >= 255) threshold = 254;
  }
  else if (key == ' ') {
    // space bar pressed
    userView = !userView;
  }
  else if (key == 'g') {
    grayMode = !grayMode;
  }
}
