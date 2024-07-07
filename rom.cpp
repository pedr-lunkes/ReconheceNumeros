#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

#define alturaImagem 28
#define larguraImagem 28

#define casosTestes 120
#define tamNeu 12

#define desafio 1000

using namespace std;
using namespace cv;

int pow(int a, int b){
    if(b == 0){
        return 1;
    }

    for(int i = 0;i<b;i++){
        a = a << 1;
    }
    return a;
}

class Rede{
    public:
        vector<vector<unsigned char>> tabelas;
        int tamanho;
        int esquecimento;

        Rede(){
            tamanho = alturaImagem*larguraImagem/tamNeu;
            tabelas = vector<vector<unsigned char>>(tamanho, vector<unsigned char>(pow(2, tamNeu), 0));
        }

        int indice(int *pesos){
            int ind = 0;
            for(int i = 0;i<tamNeu;i++){
                ind += pesos[i] * pow(2, i);
            }
            return ind;
        }

        void setUm(int chave, int *pesos){
            tabelas[chave][indice(pesos)] = 1;
        }

        char getRes(int chave, int *pesos){
            return tabelas[chave][indice(pesos)];
        }
};

int *pixels(int coordenada, vector<bool> imagem, vector<vector<int>> chave){
    int *pesos = (int *)malloc(tamNeu * sizeof(int));

    for(int i = 0;i<tamNeu;i++){
        pesos[i] = imagem[chave[coordenada][i]];
    }

    return pesos;
}

void treinamento(Rede *rede, vector<bool> imagem, vector<vector<int>> chave){
    for(int i = 0;i< alturaImagem * larguraImagem / tamNeu;i++){
        int *p = pixels(i, imagem, chave);
        rede->setUm(i, p);
        free(p);
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
    // int maxVal2 = 0;
    int max1 = 0;
    // int max2 = 0;

    for(int i = 0;i<10;i++){
        int res = 0;
        for(int j = 0;j<alturaImagem*larguraImagem/tamNeu;j++){
            int *p = pixels(j, imagem, chave);
            res += (int) redes[i].getRes(j, p);
            free(p);
        }

        if(maxVal1 < res){
            maxVal1 = res;
            max1 = i;
        }
    //     else if(maxVal2 < res){
    //         maxVal2 = res;
    //         max2 = i;
    //     }
    }

    // cout << "Resultado mais provavel: " << max1 << endl;
    // cout << "Segundo resultado mais provavel " << max2 << endl;

    return max1;
}

int main(){
    // - Chaves -
    random_device rd;
    mt19937 gen(rd());

    vector<vector<int>> chave(alturaImagem*larguraImagem/tamNeu, vector<int>(tamNeu));

    // Indices da matriz imagem
    vector<int> indices(alturaImagem*larguraImagem);

    for(int i = 0;i<alturaImagem*larguraImagem;i++){
        indices[i] = i;
    }

    shuffle(indices.begin(), indices.end(), gen);

    // Inicialização das chaves
    for(int i = 0;i<alturaImagem*larguraImagem/tamNeu;i++){
        for(int j = 0;j<tamNeu;j++){
            chave[i][j] = indices[i*tamNeu + j];
        }
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

    uniform_int_distribution<int> distrib(0, 10000);
    for(int i = 0;i<10;i++){
        for(int j = 0;j<casosTestes;j++){
            int ran = distrib(gen);
            string nome = "dataset/" + to_string(i) + "/" + to_string(i) + "/" + to_string(ran) + ".png";
            treinamento(&redes[i], leImagem(nome), chave);
        }
    }

    //- Resultado -
    // while(true){
    //     string nome;
    //     cout << "Digite o nome da imagem: ";
    //     cin >> nome;

    //     nome = "dataset/" + nome + ".png";

    //     if(nome == "dataset/sair.png"){
    //         break;
    //     }

    //     resultado(nome, redes, chave);
    // }

    // cout << resultado("dataset/1/1/100.png", redes, chave);

    for(int i = 0;i<10;i++){
        int acertos = 0;
        for(int j = casosTestes;j<casosTestes + desafio;j++){
            int ran = distrib(gen);
            string nome = "dataset/" + to_string(i) + "/" + to_string(i) + "/" + to_string(ran) + ".png";
            if(resultado(nome, redes, chave) == i){
                acertos++;
            }
        }
        acertos = acertos * 100 / desafio;

        cout << "Acertos da rede " << i << ": " << acertos << "%" << endl;
    }
}