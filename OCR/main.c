#include <stdio.h>
#include <stdlib.h>
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "image_handle.h"
#include "neural_network.h"
#include <time.h>


void setInput(int a, int b, struct neural_network *network)
{
    network->neurons[0][0].input = a;
    network->neurons[0][1].input = b;
    network->neurons[0][2].input = 1;
}

int main (int argc, char* argv[])
{
    srand(time(NULL));

    struct neural_network *network = createNetwork(3, 3, 2);
    
    double out00, out01, out10, out11;
    unsigned count = 0;
    do
    {
        
        setInput(1,1, network);
        out11 = getOutput(network);
        learn(network, 0);
        setInput(0,0, network);
        out00 = getOutput(network);
        learn(network, 0);
        setInput(0,1,network);
        out01 = getOutput(network);
        learn(network, 1);
        setInput(1,0, network);
        out10 = getOutput(network);
        learn(network, 1);
        count++;
        printf("0xor0: %f 0xor1: %f 1xor0: %f 1xor1: %f\n", out00, out01, out10, out11);
    }while ((out00 > 0.05 || out01 < 0.95 || out10 < 0.95 || out11 > 0.05) && count < 200000);
    printf("%u iterations\n",count);
        if(argc < 2)
    {
        return EXIT_SUCCESS;
    }
    IplImage *img = load(argv[1]);
    if(img != NULL)
    {
        const char* window_title = "Perfect Image";
        cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
        cvShowImage (window_title, img);
        cvWaitKey(0);
        char path[] = "result.bmp";
        cvSaveImage(&path[0], img, NULL);
        cvDestroyAllWindows();
        cvReleaseImage(&img);
    }
    else
    {
        printf("Error loading image.\n");
    }
   freeNetwork(network);

  return EXIT_SUCCESS;
}
