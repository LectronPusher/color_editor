# color_editor
A simple editor for changing the colors in an image, written in C++ with Qt5.

This is a hobby project to make something I'll use a few times and to learn how to code using object oriented principles and with modern C++ design.

The editor uses a unique system of selections and color effects to allow for powerful editing capabilities. First a user selects a region, then they choose a color effect, then they apply it to the image and start again. The editor will update the effect on the image live as the user selects new regions and makes changes. The editor also features robust undo and redo abilities, allowing a user to say, "Hmm that brown from three effects ago wasn't very good, let me go back and change it." and will let them alter the color and propagate the change to the current image. There is also a custom image viewer that allows zooming and panning around the image with disappearing scrollbars.

There are three Selectors and three Color Effects to choose from:
##### Selectors
- Entire Image: selects the entire image, good for changing a background after excluding the foreground with draw
- Draw: with either a circle or square cursor of any pixel size, the user can use the mouse to draw on the image, the drawing can either select points, exclude them from future selections, or remove selected points from select and exclude
- Match Colors: the user chooses a color from the image, chooses how close a match they want, then they either select, exclude, or remove all points that match that color.
- Also a 'Clear' button is present for each selector and will clear all selected points
##### Color Effects
- Single Color: a single color, the user picks one, also offers an opacity slider and the option to overlay the effect on the image or to replace the selected points with the chosen color (only matters if transparent)
- Gradient: offers a horizontal or vertical gradient between two colors
- Pixellate: pixellates the image by sampling colors and painting them over their neighboring pixels, the user can choose the size of the pixels

