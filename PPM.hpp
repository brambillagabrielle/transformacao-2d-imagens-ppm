#ifndef PPM_H_INCLUDED
#define PPM_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

// Dados da PPM
struct PPM
{
    string tipo;
    int largura;
    int altura;
    int vmax;
    unsigned char *pixels;

    PPM()
    {
        pixels = nullptr;
        largura = 0;
        altura = 0;
        vmax = 255;
        tipo = "";
    }
};

// Definidor de cor RGB
struct RGB
{
    unsigned char r, g, b;

    RGB()
    {
        r = 0;
        g = 0;
        b = 0;
    }

    RGB(unsigned char _r, unsigned char _g, unsigned char _b)
    {
        r = _r;
        g = _g;
        b = _b;
    }
};

// Imprime o RGB formatado
ostream &operator<<(ostream &os, const RGB rgb)
{
    return os << (int)rgb.r << ", " << (int)rgb.g << ", " << (int)rgb.b;
}

// Desaloca o objeto da imagem PPM
void destruir(PPM *ppm)
{
    if (ppm->pixels)
        delete ppm->pixels;

    ppm->pixels = nullptr;
    ppm->largura = 0;
    ppm->altura = 0;
    ppm->tipo = "";
    ppm->vmax = 255;
}

void resetaPixels(PPM *ppm)
{
    for (int i = 0; i < ppm->largura * ppm->altura * 3; i++)
        ppm->pixels[i] = 0;
}

// Imprime as informações sobre a imagem PPM
void imprimir(PPM *ppm)
{
    cout << "\n * PPM * \n";
    cout << "Tipo: " << ppm->tipo << endl;
    cout << "Dimensao: " << ppm->largura << "x" << ppm->altura << endl;
    cout << "vMax: " << ppm->vmax << endl;

    if (ppm->pixels)
        cout << "Vetor de pixels: " << &(ppm->pixels) << endl;
    else
        cout << "Vetor de pixels: NULL\n";
}

// Cria uma imagem PPM com o fundo preto RGB(0,0,0)
void criar(PPM *ppm, int largura, int altura)
{
    if (ppm->pixels)
        delete ppm->pixels;

    int tamanho = largura * altura * 3;

    ppm->tipo = "P3";
    ppm->largura = largura;
    ppm->altura = altura;
    ppm->vmax = 255;
    ppm->pixels = new unsigned char[tamanho];

    for (int i = 0; i < tamanho; i++)
        ppm->pixels[i] = 0;
}

// Grava uma imagem PPM em um arquivo da memória
bool gravar(PPM *ppm, string caminho)
{
    if (!ppm->pixels)
        return false;

    ofstream arq;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    arq << ppm->tipo << endl;
    arq << ppm->largura << " " << ppm->altura << endl;
    arq << 255 << endl;

    int tam = ppm->largura * ppm->altura * 3;
    for (int i = 0; i < tam; i++)
    {
        arq << (int)ppm->pixels[i] << endl;
        arq.flush();
    }

    arq.close();
    return true;
}

// Lê uma linha do arquivo PPM, ignorando comentários (#)
string lerLinhaArquivoPPM(std::ifstream &arq)
{
    string linha = "", dadoLido = "";
    while (!arq.eof())
    {
        linha = "";
        std::getline(arq, linha);
        if (linha.size() > 0 && linha[0] != '#')
            return linha;
    }
    return linha;
}

// Lê todo o arquivo PPM, usando como função auxiliar a lerLinhaArquivoPPM
bool ler(PPM *ppm, string caminho)
{
    ifstream arq;
    string dado;
    stringstream str;

    arq.open(caminho);
    if (!arq.is_open())
    {
        cout << "PPM: endereco do arquivo invalido\n";
        return false;
    }

    dado = lerLinhaArquivoPPM(arq);
    if (dado == "P3")
        ppm->tipo = dado;
    else
    {
        cout << "PPM: erro ao ler o tipo da imagem\n";
        return false;
    }

    dado = lerLinhaArquivoPPM(arq);
    str = stringstream(dado);
    if (!(str >> ppm->largura))
    {
        cout << "PPM: erro ao ler a largura\n";
        return false;
    }
    if (!(str >> ppm->altura))
    {
        cout << "PPM: erro ao ler a largura\n";
        return false;
    }

    dado = lerLinhaArquivoPPM(arq);
    str = stringstream(dado);
    if (!(str >> ppm->vmax))
    {
        cout << "PPM: erro ao ler vmax\n";
        return false;
    }

    if (ppm->pixels)
        delete ppm->pixels;

    int tam = ppm->largura * ppm->altura * 3;
    ppm->pixels = new unsigned char[tam];

    int i = 0, p;
    while (!arq.eof())
    {

        if (arq >> p)
        {
            if (i < tam)
                ppm->pixels[i] = (unsigned char)p;
            i++;
        }
        else
            break;
    }

    if (i != tam)
    {
        cout << "PPM: erro ao ler os pixels\n";
        return false;
    }
    arq.close();

    return true;
}

// Retorna a cor RGB de um determinado pixel de uma imagem
RGB getPixel(PPM *ppm, int x, int y)
{
    RGB rgb;
    if (!ppm->pixels)
        return rgb;

    rgb.r = ppm->pixels[y * ppm->largura * 3 + x * 3];
    rgb.g = ppm->pixels[y * ppm->largura * 3 + x * 3 + 1];
    rgb.b = ppm->pixels[y * ppm->largura * 3 + x * 3 + 2];

    return rgb;
}

// Atribui uma cor passada como parâmetro para um determinado pixel de uma imagem
void setPixel(PPM *ppm, int x, int y, RGB rgb)
{
    if (!ppm->pixels)
        return;

    ppm->pixels[y * ppm->largura * 3 + x * 3] = rgb.r;
    ppm->pixels[y * ppm->largura * 3 + x * 3 + 1] = rgb.g;
    ppm->pixels[y * ppm->largura * 3 + x * 3 + 2] = rgb.b;
}

// Verifica se uma determinada coordenada é válida
bool coordValida(PPM *ppm, int x, int y)
{
    if (!ppm->pixels)
        return false;

    if (x < ppm->largura && x >= 0 && y < ppm->altura && y >= 0)
        return true;
    else
        return false;
}

#endif