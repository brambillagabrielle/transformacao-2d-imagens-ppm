#include <Eigen/Dense>
using namespace Eigen;

#include "PPM.hpp"

#define _PI 3.14159265358979323846

void transformacao2D(PPM *pgmEntrada, PPM *pgmSaida, Matrix3f M)
{
    for (size_t y = 0; y < pgmEntrada->altura; y++)
    {
        for (size_t x = 0; x < pgmEntrada->largura; x++)
        {
            Vector3f coordenadaEntrada(x, y, 1);
            Vector3f coordenadaSaida = M * coordenadaEntrada;

            coordenadaSaida(0) = round(coordenadaSaida(0));
            coordenadaSaida(1) = round(coordenadaSaida(1));

            if (coordValida(pgmSaida, coordenadaSaida.x(), coordenadaSaida.y()))
            {
                RGB cor = getPixel(pgmEntrada, coordenadaEntrada.x(), coordenadaEntrada.y());
                setPixel(pgmSaida, coordenadaSaida.x(), coordenadaSaida.y(), cor);
            }
        }
    }
}

void transformacao2DInversa(PPM *pgmEntrada, PPM *pgmSaida, Matrix3f M)
{
    for (size_t y = 0; y < pgmSaida->altura; y++)
    {
        for (size_t x = 0; x < pgmSaida->largura; x++)
        {
            Vector3f coordenadaSaida(x, y, 1);
            Vector3f coordenadaEntrada = M * coordenadaSaida;

            coordenadaEntrada(0) = round(coordenadaEntrada(0));
            coordenadaEntrada(1) = round(coordenadaEntrada(1));

            if (coordValida(pgmSaida, coordenadaEntrada.x(), coordenadaEntrada.y()))
            {
                RGB cor = getPixel(pgmEntrada, coordenadaEntrada.x(), coordenadaEntrada.y());
                setPixel(pgmSaida, coordenadaSaida.x(), coordenadaSaida.y(), cor);
            }
        }
    }
}

float degree2rad(float ang)
{
    return ang * (float)(_PI / 180.0);
}

Matrix3f getTranslacao(float tx, float ty)
{
    Matrix3f M = Matrix3f::Identity();

    M(0,2) = tx;
    M(1,2) = ty;

    return M;
}

Matrix3f getRotacao(float graus)
{
    Matrix3f M = Matrix3f::Identity();
    float cosO = cos(degree2rad(graus));
    float sinO = sin(degree2rad(graus));

    M(0,0) = cosO;
    M(0,1) = -sinO;
    M(1,0) = sinO;
    M(1,1) = cosO;

    return M;
}

Matrix3f getEscala(float sx, float sy)
{
    Matrix3f M = Matrix3f::Identity();

    M(0,0) = sx;
    M(1,1) = sy;

    return M;
}

Matrix3f getCisalhamentoHorizontal(float a)
{
    Matrix3f M = Matrix3f::Identity();

    M(0,1) = a;

    return M;
}

Matrix3f getCisalhamentoVertical(float b)
{
    Matrix3f M = Matrix3f::Identity();

    M(1,0) = b;

    return M;
}

Matrix3f getReflexaoEixoX()
{
    Matrix3f M = Matrix3f::Identity();

    M(1,1) = -1;

    return M;
}

Matrix3f getReflexaoEixoY()
{
    Matrix3f M = Matrix3f::Identity();

    M(0,0) = -1;

    return M;
}