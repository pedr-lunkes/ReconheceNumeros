#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

#define alturaImagem 28
#define larguraImagem 28

#define casosTestes 120
#define tamNeu 12

#define desafio 10000 // Adjust this as needed

using namespace std;
using namespace cv;

vector<int> matParaVet(const vector<vector<int>>& matriz) {
    vector<int> vetor(alturaImagem * larguraImagem);

    for (int i = 0; i < alturaImagem; ++i) {
        for (int j = 0; j < larguraImagem; ++j) {
            vetor[i * larguraImagem + j] = matriz[i][j];
        }
    }

    return vetor;
}

void filtraTransparencia(const Mat& image, vector<vector<int>>& matriz_binaria) {
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec4b pixel = image.at<cv::Vec4b>(y, x);
            if (pixel[3] < 100) {
                matriz_binaria[y][x] = 0;
            } else {
                matriz_binaria[y][x] = 1;
            }
        }
    }
}

vector<int> leImagem(const string& nome) {
    Mat img = imread(nome, IMREAD_UNCHANGED);
    if (img.empty()) {
        cerr << "Could not open or find the image: " << nome << endl;
        return vector<int>(alturaImagem * larguraImagem, 0);
    }
    
    if (img.rows != alturaImagem || img.cols != larguraImagem) {
        resize(img, img, Size(larguraImagem, alturaImagem));
    }

    vector<vector<int>> matriz_binaria(alturaImagem, vector<int>(larguraImagem, 0));
    filtraTransparencia(img, matriz_binaria);

    return matParaVet(matriz_binaria);
}

Mat aumentaImagem(const Mat& img) {
    Mat resizedImg;
    resize(img, resizedImg, Size(1000, 1000), 0, 0, INTER_LINEAR);
    return resizedImg;
}

vector<int> somaVet(const vector<int>& a, const vector<int>& b) {
    vector<int> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

void printaImagem(const vector<int>& imagem) {
    for (int i = 0; i < alturaImagem; ++i) {
        for (int j = 0; j < larguraImagem; ++j) {
            printf("%d ", imagem[i * larguraImagem + j]);
        }
        printf("\n");
    }
}

void criaImagem(int a){
    vector<int> mascara(alturaImagem * larguraImagem, 0);

    for (int i = 0; i < desafio; ++i) {
        string filename = "dataset/" + to_string(a) + "/" + to_string(a) + "/" + to_string(i) + ".png";
        vector<int> img_vec = leImagem(filename);
        mascara = somaVet(mascara, img_vec);
    }

    vector<unsigned char> imagem(alturaImagem * larguraImagem, 0);

    for (int i = 0; i < alturaImagem * larguraImagem; ++i) {
        imagem[i] = static_cast<unsigned char>((mascara[i] * 255) / desafio);
    }

    Mat image(alturaImagem, larguraImagem, CV_8UC1, imagem.data());
    Mat imageGrande = aumentaImagem(image);

    imwrite(to_string(a) + "grande.png", image);
    imwrite(to_string(a) + ".png", imageGrande);

    char* arquivo = to_string(a) + ".txt";
    FILE *fp = fopen(arquivo, "w");
    for (int i = 0; i < alturaImagem * larguraImagem; ++i) {
        fprintf(fp, "%d ", mascara[i]);
    }
    fclose(fp);

    printaImagem(mascara);
}

int main() {
    for(int i = 0;i < 10;i++){
        criaImagem(i);
    }

    return 0;
}
