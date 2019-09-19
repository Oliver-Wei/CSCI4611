# repo-wei00120

README

Yingxin Wei (5517682)

1. Pseudo-random selection of characters.
The sentence I choose is “rain on the green grass but not on me”. I insert several random letters in the sentence and it becomes "rain afc on the dof green grass huj but not wmz on me". I copy the sentence for a lot of times and split the sentences to letters. The x coordinates of all letters should be within a range. So rain looks random but there is a reasonable likelihood of catching a set of raindrops that spell out a whole word. 

2. Flipping the video image
I flip the video image displayed on the screen by changing the color information between (x,y) and (Imagewidth-1-x,y).

3. Control the threshold
User can change the value of the threshold used to determine foreground and background objects interactively by using the UP and DOWN arrow keys for situations with different lighting conditions. When UP is pressed, threshold increases by one; when DOWN is pressed, threshold decreases by one.

4. Control the user_view
When SPACEBAR is pressed, the program will toggle between displaying the final user view and a “debugging” view that shows a binary image for the foreground/background classification, where all pixels classified as foreground are pure black and all pixels classified as background are pure white. This is accomplished by changing the variable userView when SPACEBAR is pressed and displaying the view based on the value of userView. Similarly, When “g” button is pressed, the program will toggle between the gray view and natural view. A gray filter would be added to the displayed image, which makes it looks like Utterback’s program. 

5. Dropping and Rising of the Letter 
The letter drops if its new assigned position doesn’t belong to a dark object, that is, the brightness of the pixel is larger than the threshold. The letter rises up if the dark object moves up, that is, the pixel is less than the threshold and its above pixel is also less than the threshold. The variable “upSpeed” controls the height it rises up to.

7. Velocity of drops
The millis() function is related with real time and it helps control the velocity of the dropping of the letter. The program updates the image when the passed time (passedTime) is larger than the setting time (totalTime). In other words, the letter drops a pixel when the passed time (passedTime) is larger than the setting time (totalTime). So the velocity of dropping is actually 1/totalTime (pixel/ms) = 1000 /totalTime (pixel/second).

Wizards

1. Fade away
The letter fades away if it is near the bottom of the image or if it stays in the image for a long time.


