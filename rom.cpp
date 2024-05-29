#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

#define alturaImagem 28
#define larguraImagem 28

#define casosTestes 80

using namespace std;
using namespace cv;

class Rede{
    public:
        vector<vector<bool>> tabelas;
        int tamanho;

        Rede(){
            tamanho = alturaImagem*larguraImagem/3;
            tabelas = vector<vector<bool>>(tamanho, vector<bool>(8, false));
        }

        void setUm(int chave, int x, int y, int z){
            tabelas[chave][x * 4 + y * 2 + z] = true;
        }

        int getRes(int chave, int x, int y, int z){
            return tabelas[chave][x * 4 + y * 2 + z] == 1 ? 1 : -1;
        }
};

void treinamento(Rede *rede, vector<bool> imagem, vector<vector<int>> chave){
    for(int i = 0;i< alturaImagem * larguraImagem / 3;i++){
        rede->setUm(i, imagem[chave[i][0]], imagem[chave[i][1]], imagem[chave[i][2]]);
    }
}

vector<bool> matParaVet(vector<vector<bool>> matriz){
    vector<bool> vetor(alturaImagem*larguraImagem);

    // cout << "Matriz binária:" << endl;
    // for (int i = 0; i < alturaImagem; ++i) {
    //     for (int j = 0; j < larguraImagem; ++j) {
    //         cout << (int) matriz[i][j] << " ";
    //     }
    //     cout << std::endl;
    // }

    for(int i = 0;i<alturaImagem;i++){
        for(int j = 0;j<larguraImagem;j++){
            vetor[i*larguraImagem + j] = matriz[i][j];
        }
    }

    return vetor;
}

void filtraTransparencia(Mat image, vector<vector<bool>> &matriz_binaria){
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            cv::Vec4b& pixel = image.at<cv::Vec4b>(y, x);
            if (pixel[3] < 100) {
                matriz_binaria[y][x] = false;
            } else {
                matriz_binaria[y][x] = true;
            }
        }
    }
}

vector<bool> leImagem(string nome){
    Mat img = imread(nome, IMREAD_UNCHANGED);
    vector<vector<bool>> matriz_binaria(alturaImagem, vector<bool>(larguraImagem, false));

    filtraTransparencia(img, matriz_binaria);

    return matParaVet(matriz_binaria);
}

int resultado(string arquivo, vector<Rede> redes, vector<vector<int>> chave){
    vector<bool> imagem = leImagem(arquivo);

    int maxVal1 = 0;
    int maxVal2 = 0;
    int max1 = 0;
    int max2 = 0;

    for(int i = 0;i<10;i++){
        int res = 0;
        for(int j = 0;j<alturaImagem*larguraImagem/3;j++){
            res += redes[i].getRes(j, imagem[chave[j][0]], imagem[chave[j][1]], imagem[chave[j][2]]);
        }

        if(maxVal1 < res){
            maxVal1 = res;
            max1 = i;
        }
        else if(maxVal2 < res){
            maxVal2 = res;
            max2 = i;
        }
    }

    // cout << "Resultado mais provavel: " << max1 << endl;
    // cout << "Segundo resultado mais provavel " << max2 << endl;

    return max1;
}

int main(){
    // - Chaves -
    random_device rd;
    mt19937 gen(rd());

    vector<vector<int>> chave(alturaImagem*larguraImagem/3, vector<int>(3));

    // Indices da matriz imagem
    vector<int> indices(alturaImagem*larguraImagem);

    for(int i = 0;i<alturaImagem*larguraImagem;i++){
        indices[i] = i;
    }

    shuffle(indices.begin(), indices.end(), gen);

    // Inicialização das chaves
    for(int i = 0;i<alturaImagem*larguraImagem/3;i++){
        chave[i][0] = indices[i*3];
        chave[i][1] = indices[i*3 + 1];
        chave[i][2] = indices[i*3 + 2];
    }

    
    // - Treinamento -
    // Rede bananas;
    // treinamento(&bananas, matriz_binaria, chave);

    // cout << "Resultado:" << endl;

    // int res = 0;
    // for(int i = 0;i<alturaImagem*larguraImagem/3;i++){
    //     res += bananas.getRes(i, matriz_binaria[chave[i][0]], matriz_binaria[chave[i][1]], matriz_binaria[chave[i][2]]);
    // }

    // cout << res << endl;
    
    vector<Rede> redes;

    for(int i = 0;i<10;i++){
        redes.push_back(Rede());
    }

    for(int i = 0;i<10;i++){
        for(int j = 0;j<casosTestes;j++){
            string nome = "dataset/" + to_string(i) + "/" + to_string(i) + "/" + to_string(j) + ".png";
            treinamento(&redes[i], leImagem(nome), chave);
        }
    }

    // - Resultado -
    // while(true){
    //     string nome;
    //     cout << "Digite o nome da imagem: ";
    //     cin >> nome;

    //     nome = "dataset/" + nome + ".png";

    //     if(nome == "sair.png"){
    //         break;
    //     }

    //     resultado(nome, redes, chave);
    // }
    for(int i = 0;i<10;i++){
        int acertos = 0;
        for(int j = 100;j<200;j++){
            string nome = "dataset/" + to_string(i) + "/" + to_string(i) + "/" + to_string(j) + ".png";
            if(resultado(nome, redes, chave) == i){
                acertos++;
            }
        }
        acertos = acertos * 100 / 100;

        cout << "Acertos da rede " << i << ": " << acertos << "%" << endl;
    }
}