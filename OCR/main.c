#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "neural_network.h"
#include <time.h>
#include "image_treatment.h"
#include "fileio.h"
#include "image_detection.h"

void learnAlphabet(neural_network *network);


int main (int argc, char* argv[])

{

	if (argc == 1)
	{

        int layerCount[4] = {20*20, 20*20*2.5, 100, 128};
        neural_network *network = createNetwork(4,layerCount, 0.9, 0.3);
        learnAlphabet(network);
        saveNetwork(network);
        //freeNetwork(network);
        _Exit(0);
	}

	else if (argc == 2)
	{
		IplImage *img = load(argv[1]);
		if(img != NULL)
		{


			//img = treatment(img);
			int nb_char;
			struct rect_char *chars = detection(img, &nb_char);
            int nLayers[3] = {20*20, 20*20*1.5, 128};
            neural_network *network = loadNetwork();
            char *str = getString(chars, nb_char, img, network);
            printf("%s\n", str);
            saveToFile(str, "test OCR");
            freeNetwork(network);
            free(chars);
            free(str);
            return 0;
            printf("nb char %i", nb_char);

			const char* window_title = "Perfect Image";
			cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
			cvShowImage (window_title, img);
			cvWaitKey(0);
			char path[] = "result.bmp";
			cvSaveImage(&path[0], img, NULL);
			cvDestroyAllWindows();
			cvReleaseImage(&img);
			printf("Everything went fine. Exiting...\n");
			return 0;
		}
		else
		{
			printf("Error loading image.\n");
			return -1;
		}
	}
	else
	{
		printf("Argument issue. Exiting...\n");
		return -1;
	}
    return 0;
}


double *getCharOutput(char c)
{
    double *charArray = malloc(sizeof(double) * 128);
    for (int i = 0; i < c; i++) {
        charArray[i] = 0;
    }
    charArray[c] = 1;
    for (int i = c + 1; i < 128; i++) {
        charArray[i] = 0;
    }
    return charArray;
}

void learnAlphabet(neural_network *network)
{
    char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890";

    int nb_char;
    IplImage *img = load("images/alphabet.png");
    struct rect_char *rect_chars = learning_detection(img, &nb_char);


    if (!img) {
        printf("error loading image lawl\n");
        return;
    }
    double **input = malloc(62 * sizeof(double *));

    for (int i = 0; i < 62; i++) {
        input[i] = imageArray(create_char(img, rect_chars[i]));
    }
    double **output = malloc(sizeof(double *) * 128);

    int i = 0;
    for (char *c = alphabet; *c; c++, i++) {
        output[i] = getCharOutput(*c);
    }

    int num_iter = 30000;
    int report_per_iter = 1000;
    int dataRows = 62;

    for (int i = 0; i < num_iter; i++) {
        int row = i % dataRows;
        runBackward(network, input[row], output[row]);
        if (!(i % report_per_iter))
        {
            double err = evalError(network, output[row]);
            printf("iter = %i err = %f\n", i, err);
        }
    }
    
    double **out = malloc(sizeof(double *) * 62);
    
    for (int i = 0; i < 62; i++) {
        out[i] = malloc(sizeof(double) * 128);
        runForward(network, input[i]);
        for (int j = 0; j < 128; j++) {
            out[i][j] = getOutput(network, j);
        }
    }
    
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 62; j++) {
            printf("p(%c) = %c : %f ", (char)j, (char) i, out[j][i]);
        }
        printf("\n");
    }
    
    
/*
    for (int i = 0; i < 128; i++) {
        free(output[i]);
        if (i < 62) {
            free(input[i]);
        }
    }*/
    /*
    free(output);
    free(input);
    free(rect_chars);
    cvReleaseImage(&img);*/
}
















