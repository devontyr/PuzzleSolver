#include "imageprocess.h"
#include "interactivepiece.h"
#include "puzzlepiece.h"

#include <QtWidgets>
#include <imageviewer.h>

/*
Main method to process an image into seperate and display all puzzle pieces
    input: an image
    output: displays all puzzle pieces and org image on the screen
*/
ImageProcess::ImageProcess(const QImage &_image):image(_image) {
    QImage imageToProcess = image.copy();
    processImage(imageToProcess);
    pieceSeperator(image, redImage);

    // LAYOUTS
    // set the horizontal box
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    setLayout(mainLayout);

    // add the ImageViewer to the top
    // ImageViewer *imageViewer = new ImageViewer(imageToProcess);
    //mainLayout->addWidget(imageViewer);

    // add QGraphicsView to the bottom
    solverInterface = new QGraphicsView;
    // mainLayout->addWidget(imageViewer);

    scene = new QGraphicsScene(this);
    solverInterface->setScene(scene);
    interactivePiece* interactivePieceLayout = new interactivePiece(pieces);
    mainLayout->addWidget(interactivePieceLayout);
}

/*
Pre-processes an image red. Looks at the 8 surrounding pixels for each pixel to fill in unexpected gaps
    input: image
    output: updates redImage to a processed version
*/
void ImageProcess::processImage(QImage &image){
    redImage = QImage(image.width(), image.height(), QImage::Format_RGB32);

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QRgb pixel = image.pixel(x, y);
            if (!isShadeOfBlack(pixel)) {
                redImage.setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as a puzzle piece
            }
            else{
                redImage.setPixelColor(QPoint(x,y), Qt::black); //mark the pixel as a puzzle piece

            }
        }
    }


    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            if(isSurroundedRed(x, y)){
                redImage.setPixelColor(QPoint(x,y), Qt::red); //mark the pixel as red
            }
        }
    }
}

/*
Helper method to check if a color is part of the white background
    input: a color
    output: true or false
*/
bool ImageProcess::isShadeOfWhite(const QRgb &color) {
    int threshold = 10; // larger num if we want to accept more gray colors as "white"
    int minBrightness = 169; // smaller num allows more gray colors as "white"

    //values are between 0-255
    int r = qRed(color);
    int g = qGreen(color);
    int b = qBlue(color);

    // Check if r/g/b are right hue (pure white/black would be 0)
    if (abs(r - g) > threshold || abs(r - b) > threshold || abs(g - b) > threshold) {
        return false;
    }
    // Check if r/g/b is right shade of white-gray-black
    return r >= minBrightness && g >= minBrightness && b >= minBrightness;
}

/*
Helper method to check if a color is part of the black background
    input: a color
    output: true or false
*/
bool ImageProcess::isShadeOfBlack(const QRgb &color) {
    //values are between 0-255
    int r = qRed(color);
    int g = qGreen(color);
    int b = qBlue(color);

    int combo = r + g + b;

    if (combo < 100){
        return true;
    }

    if ((qAbs(r - 45) <= 15) && (qAbs(g - 40) <= 15) && (qAbs(b - 48) <= 15)){
        return true;
    }
    else {
        return false;
    }
}

/*
Helper method to checks if surrounding 8 pixels of a given pixel coord are red
    input: a color
    output: true or false
*/
bool ImageProcess::isSurroundedRed(int pixelX, int pixelY){
    int redCount = 0;
    if ((pixelX <= 1) || (pixelY <=1) || (pixelX >= redImage.width() - 2) || (pixelY >= redImage.height() - 2)){
        return false;
    }

    QList<QRgb> surroundingPix(25);

    int index = 0;
    for (int row = -2; row <= 2; ++row){
        for (int col = -2; col <= 2; ++col){
            surroundingPix[index] = redImage.pixel(pixelX + col, pixelY + row);
            ++index;
        }
    }

    for (int i = 0; i < 25; ++i){
        if (qRed(surroundingPix[i]) == 255){
            ++redCount;
        }
    }

    return (redCount >= 13);
}

/*
BFS on the redImage to cut out each individual puzzle piece.
    input: image, redImage
    output:
            pieces -- a list of QImage puzzle pieces
*/
void ImageProcess::pieceSeperator(QImage &image, QImage &redImage) {
    //QSet<QPoint> PuzzlePixels; QVector<QSet<QPoint>> PuzzlePieces;
    QList<QPoint> toDo;
    toDo.reserve(100000);
    int minPieceSize = 30000;
    int C = image.width(), R = image.height();
    QRgb white = 0xffffffff;
    QRgb red = 0xffff0000;


    // loop through possible start positions to find the beginning of a piece
    for (int iStartRow=0; iStartRow < R; ++iStartRow) {
        for (int iStartCol=0; iStartCol < C; ++iStartCol) {

            QPoint curPoint = QPoint(iStartCol, iStartRow);
            if (redImage.pixel(curPoint) != red) {
                continue;
            }

            //create a new puzzle piece
            QImage piece(C, R, QImage::Format_ARGB32);
            piece.fill(0x00ffffff);
            int curPiecePixels = 0;
            int minX = C; int minY = R; int maxX = 0; int maxY = 0;
            toDo << curPoint;

            while (!toDo.empty()) {

                QPoint topPoint = toDo.back();
                toDo.pop_back(); // pop top pixel off the stack

                QRgb orgColor = image.pixel(topPoint);

                redImage.setPixel(topPoint, white); // make it white to avoid duplicate processes

                // add it to the current puzzle piece
                piece.setPixel(topPoint, orgColor);
                //PuzzlePixels.insert(topPoint);
                minX = min(minX, topPoint.x()); minY = min(minY, topPoint.y());
                maxX = max(maxX, topPoint.x()); maxY = max(maxY, topPoint.y());

                ++curPiecePixels; // add to the number of pixels

                // add any valid colored neighbors to toDo

                QPoint lNeighbor(topPoint.x() - 1, topPoint.y());
                QPoint rNeighbor(topPoint.x() + 1, topPoint.y());
                QPoint tNeighbor(topPoint.x(), topPoint.y() + 1);
                QPoint bNeighbor(topPoint.x(), topPoint.y() - 1);
                if (redImage.valid(lNeighbor) && redImage.pixel(lNeighbor) == red) toDo << lNeighbor;
                if (redImage.valid(rNeighbor) && redImage.pixel(rNeighbor) == red) toDo << rNeighbor;
                if (redImage.valid(tNeighbor) && redImage.pixel(tNeighbor) == red) toDo << tNeighbor;
                if (redImage.valid(bNeighbor) && redImage.pixel(bNeighbor) == red) toDo << bNeighbor;
            }
            // once toDo empty, we have found a full piece
            // check that it has enough pixels and then add that pieces to the collection of pieces
            if (curPiecePixels >= minPieceSize) {
                //PuzzlePieces.append(PuzzlePixels);

                //crop piece and add it to pieces
                QRect cropRect(minX, minY, maxX-minX, maxY-minY);
                QImage croppedPiece = piece.copy(cropRect);
                pieces.append(croppedPiece);
                qDebug() << "Piece of size" << curPiecePixels;
            }
        }
    }
}

/*
Stores each puzzle piece found as a binary matrix.
Takes the list of QImage puzzle pieces and turns it into a list of matricies
*/
void ImageProcess::pieceOutput(){
    for (int iImg=0; iImg < pieces.size(); iImg++) {
        pieceMatricies.append(imageToMatrix(pieces[iImg]));
    }
}

/*
Helper function to turns each individual QImage into a matrix of 0's for background and 1's for peices
    input: QImage of a piece
    output: Matrix representaion of that piece
*/
QVector<QVector<int>> ImageProcess::imageToMatrix(QImage &pieceImg) {
    int C = pieceImg.width(), R = pieceImg.height();
    QVector<QVector<int>> matrix(R, QVector<int>(C, 0));

    for (int iY=0; iY < R; ++iY) {
        for (int iX=0; iX < C; ++iX) {
            // QUESTION HERE -- should we be storing redImage so that we can use red instead of isShadeOfBlack ?
            QRgb pix = image.pixel(iX, iY);
            if (!isShadeOfBlack(pix)) {
                matrix[iX][iY] = 1;
            }
        }
    }
    return matrix;
}

/*
Takes in a single puzzle piece matrix and an ideal corner and returns the best macth coordinates for that corner.
*/
pair<int, int> ImageProcess::findCorner(QVector<QVector<int>>& piece, QVector<QVector<int>>& idealCorner) {
    //XOR a corner matrix to all possible parts of piece matrix, return the best match
}

/*
Takes in a single puzzle piece matrix and returns an edge (a list of pair coordinates).
*/
void ImageProcess::findEdge(pair<int, int> corner1, pair<int, int> corner2, pair<int, int> direction) {

    vector<pair<int, int>> edge;
    edge.push_back(corner1); // start at corner 1

    // check every line until you get to a row of all 1's
    // top: x+corner1.x to x.corner2
    // right:
    // bottom:
    // left:
    // for each value, check if it is touching a 0 -- if yes, add to edge

    // determine direction: right(1,0) down(0,-1) left(-1, 0) up(0,1)
    pair<int, int> right(1,0); pair<int, int> down(0, -1); pair<int, int> left(-1,0); pair<int, int> up(0,1);
    if (direction == right) pair<int, int> checkdir = up;
    if (direction == down) pair<int, int> checkdir = right;
    if (direction == left) pair<int, int> checkdir = down;
    if (direction == up) pair<int, int> checkdir = left;

    // move in the direction as long as the value at curr=1 and current+checkdir=0

}

/*
Uses the helper methods to store edges in puzzle piece class for each piece.
*/
puzzlepiece ImageProcess::mapEdges(QVector<QVector<int>> piece) {
    //create 4 pre-set matricies for the corners
    //call method to find each of the corners
    //call method to find each of the edges
    //store edges in puzzle piece class for each piece

    QVector<QVector<int>> tl_corner, tr_corner, bl_corner, br_corner;

    tl_corner = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1}
    };

    tr_corner = {
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0}
    };

    br_corner = {
        {1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0}
    };

    bl_corner = {
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0},
    };


    pair<int, int> tl_coord = findCorner(piece, tl_corner);
    pair<int, int> tr_coord = findCorner(piece, tr_corner);
    pair<int, int> bl_coord = findCorner(piece, bl_corner);
    pair<int, int> br_coord = findCorner(piece, br_corner);

    puzzlepiece build_piece; // we also need to add piece id stuff
    build_piece.id = puzzlepiece_id;
    puzzlepiece_id++;
    build_piece.north = findEdge(tl_coord, tr_coord, {-1, 0});
    build_piece.east = findEdge(tr_coord, br_coord, {0, 1});
    build_piece.south = findEdge(br_coord, bl_coord, {1, 0});
    build_piece.west = findEdge(bl_coord, tl_coord, {0, -1});
    return build_piece;

}
