## User Guide

### Getting Started  
When you open the application for the first time, you will see a single option: **Open Image**. This allows you to select the initial image to begin working with. Ideally, this should be a scan of puzzle pieces.  

### Adding More Images  
Once the first image is opened, the **Add Image** button becomes enabled. Use this to add additional images, which should ideally be scans of puzzle pieces from the same puzzle as the original image. You can add as many images as needed.  

### Processing the Puzzle  
At any point after the first image is opened, you can click the **Process** button. This is the key step where our algorithm analyzes all the loaded images.  

Upon processing, a new **QGraphicsView** will open, displaying each puzzle piece as an independent, movable, and rotatable element. This interactive view allows you to freely manipulate the pieces to solve the puzzle.  

### Resetting the Application  
At any point, the **Reset** button can be clicked to return the application to its initial state, as if it had just been opened. This clears all loaded images and resets the interface. 

## Developer Notes

### Code Design
1. MainWindow handles all GUI button processing.
2. ImageViewer allows user to view unprocessed images.
3. PuzzlePiece is an object to store all information about a given puzzle piece.
4. PuzzleSolverLayout does all image processing tasks, outputting a binary matrix for each puzzle piece.
5. InteractivePiece allows user to move puzzle pieces around the screen.
6. PuzzleSolver does all algorithmic solving of the puzzle.

### Known Issues
Currently missing edge identification needed to pass to the solving algorithm in order to solve the puzzle
