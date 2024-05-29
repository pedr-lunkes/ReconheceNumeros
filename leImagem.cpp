#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>

#define alturaImagem 16
#define larguraImagem 16

using namespace cv;
using namespace std;

vector<vector<bool>> leImagem(int num){
    Mat img = imread("Numeros.png", IMREAD_GRAYSCALE);

    Mat imagemBinaria;
    threshold(img, imagemBinaria, 127, 255, THRESH_BINARY);

    vector<vector<bool>> matriz_binaria(alturaImagem, vector<bool>(larguraImagem, false));
    for (int i = 0; i < alturaImagem; ++i) {
        for (int j = 0; j < larguraImagem; ++j) {
            matriz_binaria[i][j] = (imagemBinaria.at<uchar>(i, j) == 255) ? true : false;
        }
    }

    return matriz_binaria;
}

int main(){
    vector<vector<bool>> matriz_binaria = leImagem(0);

    cout << "Matriz binária:" << endl;
    for (int i = 0; i < alturaImagem; ++i) {
        for (int j = 0; j < larguraImagem; ++j) {
            cout << (int) matriz_binaria[i][j] << " ";
        }
        cout << std::endl;
    }
}