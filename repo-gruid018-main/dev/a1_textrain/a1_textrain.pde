/*
 * CSci-4611 Assignment #1 Text Rain
 */


/* Note: if Processing's video library does not support your particular combination of webcam and
   operating system, then the Sketch may hang in the setup() routine when the list of available
   image capture devices is requestd with "Capture.list()".  If this happens, you can skip all of
   the camera initilization code and just run in movie mode by setting the following global 
   variable to true. 
   
   If you having trouble accessing the cameara on a mac laptop due to a security issue.  Some
   students found this post helpful: 
   https://github.com/processing/processing-video/issues/134#issuecomment-664778394
 */
boolean forceMovieMode = false;

// Global vars used to access video frames from either a live camera or a prerecorded movie file
import processing.video.*;
String[] cameraModes;
Capture cameraDevice;
Movie inputMovie;
boolean initialized = false;
PFont f;
String rainPhrase = "rain rain go away come again some other day";
int threshold = 50;
boolean debug = false;

// Both modes of input (live camera and movie) will update this same variable with the lastest
// pixel data each frame.  Use this variable to access the new pixel data each frame!
PImage inputImage;
PImage mirrorImage;
Drop[] rain;

class Drop {
 int xpos;
 int ypos;
 char letter;
   Drop (char c, int x1) {
     xpos = x1;
     letter = c;
     ypos = 0;
  } 
}

// Called automatically by Processing, once when the program starts up
void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  // load pixels and threshold them to all black or all white
  
  
  f = createFont("Arial", 16, true);
  textFont(f);
  fill(255);
  
  rain = new Drop[rainPhrase.length()];
  for (int i = 0; i < rainPhrase.length(); i++) {
   rain[i] = new Drop(rainPhrase.charAt(i), ((1200/rainPhrase.length()) * i) + 40);
  } 
  
  if (!forceMovieMode) {
    println("Querying avaialble camera modes.");
    cameraModes = Capture.list();
    println("Found " + cameraModes.length + " camera modes.");
    for (int i=0; i<cameraModes.length; i++) {
      println(" " + i + ". " + cameraModes[i]); 
    }
    // if no cameras were detected, then run in offline mode
    if (cameraModes.length == 0) {
      println("Starting movie mode automatically since no cameras were detected.");
      initializeMovieMode(); 
    }
    else {
      println("Press a number key in the Processing window to select the desired camera mode.");
    }
  }
}



// Called automatically by Processing, once per frame
void draw() {
  // start each frame by clearing the screen
  background(0);
    
  if (!initialized) {
    // IF NOT INITIALIZED, DRAW THE INPUT SELECTION MENU
    drawMenuScreen();      
  }
  else {
    // IF WE REACH THIS POINT, WE'RE PAST THE MENU AND THE INPUT MODE HAS BEEN INITIALIZED

    
    // GET THE NEXT FRAME OF INPUT DATA FROM LIVE CAMERA OR MOVIE  
    if ((cameraDevice != null) && (cameraDevice.available())) {
      // Get image data from cameara and copy it over to the inputImage variable
      cameraDevice.read();
      inputImage.copy(cameraDevice, 0,0,cameraDevice.width,cameraDevice.height, 0,0,inputImage.width,inputImage.height);
    }
    else if ((inputMovie != null) && (inputMovie.available())) {
      // Get image data from the movie file and copy it over to the inputImage variable
      inputMovie.read();
      inputImage.copy(inputMovie, 0,0,inputMovie.width,inputMovie.height, 0,0,inputImage.width,inputImage.height);
    }


    // DRAW THE INPUTIMAGE ACROSS THE ENTIRE SCREEN
    // Note, this is like clearing the screen with an image.  It will cover up anything drawn before this point.
    // So, draw your text rain after this!
    mirrorImage = inputImage.copy();
    mirrorImage.loadPixels();
    inputImage.loadPixels();
    
    
    for(int x = 0; x < mirrorImage.width; x++) {
      for(int y = 0; y < mirrorImage.height; y++) {
        mirrorImage.pixels[(mirrorImage.width * y) + x] = inputImage.pixels[(mirrorImage.width - (x+1)) + (y*mirrorImage.width)];
    }
  }
    
    mirrorImage.updatePixels();
    // inputImage = mirrorImage.copy();
    
    /*
    for(int x = 0; x < inputImage.width; x++) {
      for(int y = 0; y < inputImage.height; y++) {
        int loc = (inputImage.width * y) + x;
        if(red(inputImage.pixels[loc]) > threshold*255) {
          inputImage.pixels[loc] = color(255,255,255);
      } else {
        inputImage.pixels[loc] = color(0,0,0);
      } 
    }
   } 
   */
  
   
    set(0, 0, mirrorImage);
    if(debug){
    filter(THRESHOLD, threshold*0.01);
    fill(127,0,0);
       text("Threshold: " + threshold + "%", 40, 40);
    }
    
    
    // DRAW THE TEXT RAIN, ETC.
    // TODO: Much of your implementation code should go here.  At this point, the latest pixel data from the
    // live camera or movie file will have been copied over to the inputImage variable.  So, if you access
    // the pixel data from the inputImage variable, your code should always work, no matter which mode you run in.
     
        for(int i = 0; i < rain.length; i++) {
          int index1D = (mirrorImage.width * rain[i].ypos) + rain[i].xpos;
          
          println(mirrorImage.width + rain[i].ypos + rain[i].xpos);
             if(rain[i].ypos > mirrorImage.height){
               rain[i].ypos = 0; 
          } else if(rain[i].ypos < 0) {
               rain[i].ypos = 0; 
          }
           
              if(brightness(mirrorImage.pixels[index1D]) >= (255*(threshold*0.01))) {
        rain[i].ypos -=10;
        fill(127,0,0);
        text(rain[i].letter, rain[i].xpos, rain[i].ypos);
      
          } else {
        rain[i].ypos +=2;
        fill(127,0,0);
        text(rain[i].letter, rain[i].xpos, rain[i].ypos);
        
      }  
    }
  }
 }



// Called automatically by Processing once per frame
void keyPressed() {
  if (!initialized) {
    // CHECK FOR A NUMBER KEY PRESS ON THE MENU SCREEN    
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        initializeMovieMode();
      }
      else if ((input >= 1) && (input <= 9)) {
        initializeLiveCameraMode(input);
      }
    }
  }
  else {
    // CHECK FOR KEYPRESSES DURING NORMAL OPERATION
    // TODO: Fill in your code to handle keypresses here..
    if (key == CODED) {
      if (keyCode == UP) {
        // up arrow key pressed
       threshold += 2;
      }
      else if (keyCode == DOWN) {
        // down arrow key pressed
       threshold -=2;
      }
    }
    else if (key == ' ') {
      // spacebar pressed
       debug = !debug;
    } 
  }
}



// Loads a movie from a file to simulate camera input.
void initializeMovieMode() {
  String movieFile = "TextRainInput.mov";
  println("Simulating camera input using movie file: " + movieFile);
  inputMovie = new Movie(this, movieFile);
  inputMovie.loop();
  initialized = true;
}


// Starts up a webcam to use for input.
void initializeLiveCameraMode(int cameraMode) {
  println("Activating camera mode #" + cameraMode + ": " + cameraModes[cameraMode-1]);
  cameraDevice = new Capture(this, cameraModes[cameraMode-1]);
  cameraDevice.start();
  initialized = true;
}


// Draws a quick text-based menu to the screen
void drawMenuScreen() {
  int y=10;
  text("Press a number key to select an input mode", 20, y);
  y += 40;
  text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
  y += 40; 
  for (int i = 0; i < min(9,cameraModes.length); i++) {
    text(i+1 + ": " + cameraModes[i], 20, y);
    y += 40;
  }  
}
