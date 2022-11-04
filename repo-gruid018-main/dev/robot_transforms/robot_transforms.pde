

float t = 0.0;

void setup()
{
  size(600, 310);
  smooth();
}

void draw() {
  background(255);
  drawFloor();
  drawRobot();
  t += 0.05;
}


void drawRobot() {

}


// HELPER ROUTINES FOR DRAWING THE ROBOT'S BODY PARTS

// 50x65 rectangle with origin at bottom center (pelvis area)
void drawTorso() {
  noStroke();
  fill(38, 38, 200);
  rect(-25, -65, 50, 65); // body
}

// 38x30 rectangle with origin at bottom center
void drawHead() {
  noStroke();
  fill(38, 38, 200);
  rect(-19, -30, 38, 30);
  fill(222, 222, 249);
  ellipse(-8, -18, 12, 12); // left eye
  ellipse( 8, -18, 12, 12); // right eye
}

// 12x26 rectangle with origin at top center
void drawArmPart() {
  noStroke();
  fill(38, 38, 200);
  rect(-6, 0, 12, 26);  
}

// 12x20 ellipse with origin at the top center
void drawHand() {
  noStroke();
  fill(122, 122, 249);
  ellipse(0, 10, 12, 20);
}

// 16x40 rectangle with origin at top center
void drawLegPart() {
  noStroke();
  fill(38, 38, 200);  
  rect(-8, 0, 16, 40);
}

// 26x12 ellipse with origin at top center
void drawFoot() {
  noStroke();
  fill(122, 122, 249);
  ellipse(0, 6, 26, 12);
}



void drawFloor()
{
  noStroke();
  fill(100, 255, 100);
  rect(0, 300, 600, 10);   
}
