#include <iostream>
using namespace std;

#include "transformacoes.hpp"

int main(void)
{
	setlocale(LC_ALL, "Portuguese");

	string caminho;
	cout<< "Insira o caminho da imagem de entrada: ";
	cin>> caminho;

	PPM ppmEntrada;
	if (ler(&ppmEntrada, caminho))
	{
		float tx = (ppmEntrada.largura - 1) / 2.0, ty = (ppmEntrada.altura - 1) / 2.0;

		PPM ppmSaida;
		criar(&ppmSaida, ppmEntrada.largura, ppmEntrada.altura);

		bool sair = false;
		string acao, transformacao;
		int quantTransformacao = 0;

		Matrix3f T = getTranslacao(tx, ty), Tinverso = getTranslacao(-tx, -ty);
    	Matrix3f M = Matrix3f::Identity();

		while (!sair)
		{
			cout<< "\nInsira a transformação 2D desejada (R, T, S, RE ou CI): ";
			cin>> transformacao;
			quantTransformacao++;

			if (transformacao == "R") // Rotação
			{
				float graus; 
				cout<< "Insira o valor em graus da rotação: ";
				cin>> graus;

				M = getRotacao(graus) * M;
				transformacao2DInversa(&ppmEntrada, &ppmSaida, T * M.inverse() * Tinverso);
				string caminhoNovaImagem = "imagens/transformacao-" + to_string(quantTransformacao) + ".ppm";
				gravar(&ppmSaida, caminhoNovaImagem);
				cout<< "\nImagem salva: " << caminhoNovaImagem;

				resetaPixels(&ppmSaida);
			}
			else if (transformacao == "T") // Translação
			{
				float tx, ty;
				cout<< "Insira o valor de translação no eixo x (tx): ";
				cin>> tx;
				cout<< "Insira o valor de translação no eixo y (ty): ";
				cin>> ty;

				M = getTranslacao(tx, ty) * M;
				transformacao2DInversa(&ppmEntrada, &ppmSaida, T * M.inverse() * Tinverso);
				string caminhoNovaImagem = "imagens/transformacao-" + to_string(quantTransformacao) + ".ppm";
				gravar(&ppmSaida, caminhoNovaImagem);
				cout<< "\nImagem salva: " << caminhoNovaImagem;

				resetaPixels(&ppmSaida);
			}
			else if (transformacao == "S") // Escala
			{
				float sx, sy;
				cout<< "Insira o valor de escala no eixo x (sx): ";
				cin>> sx;
				cout<< "Insira o valor de escala no eixo y (sy): ";
				cin>> sy;

				M = getEscala(sx, sy) * M;
				transformacao2DInversa(&ppmEntrada, &ppmSaida, T * M.inverse() * Tinverso);
				string caminhoNovaImagem = "imagens/transformacao-" + to_string(quantTransformacao) + ".ppm";
				gravar(&ppmSaida, caminhoNovaImagem);
				cout<< "\nImagem salva: " << caminhoNovaImagem;

				resetaPixels(&ppmSaida);
			}
			else if (transformacao == "RE") // Reflexão
			{
				string eixo;
				cout<< "A reflexão deve ocorrer em torno do eixo x ou y? (x/y): ";
				cin>> eixo;
				
				if (eixo == "x")
				{
					M = getReflexaoEixoX() * M;
					transformacao2DInversa(&ppmEntrada, &ppmSaida, T * M.inverse() * Tinverso);
					string caminhoNovaImagem = "imagens/transformacao-" + to_string(quantTransformacao) + ".ppm";
					gravar(&ppmSaida, caminhoNovaImagem);
					cout<< "\nImagem salva: " << caminhoNovaImagem;

					resetaPixels(&ppmSaida);
				}
				else if (eixo == "y")
				{
					M = getReflexaoEixoY() * M;
					transformacao2DInversa(&ppmEntrada, &ppmSaida, T * M.inverse() * Tinverso);
					string caminhoNovaImagem = "imagens/transformacao-" + to_string(quantTransformacao) + ".ppm";
					gravar(&ppmSaida, caminhoNovaImagem);
					cout<< "\nImagem salva: " << caminhoNovaImagem;

					resetaPixels(&ppmSaida);
				}
			}
			else if (transformacao == "CI") // Cisalhamento
			{
				string orientacao;
				cout<< "O cisalhamento deve ocorrer horizontalmente ou verticalmente? (h/v): ";
				cin>> orientacao;

				if (orientacao == "a")
				{
					float a;
					cout<< "Insira o valor do cisalhamento horizontal (a): ";
					cin>> a;
					
					M = getCisalhamentoHorizontal(a) * M;
					transformacao2DInversa(&ppmEntrada, &ppmSaida, T * M.inverse() * Tinverso);
					string caminhoNovaImagem = "imagens/transformacao-" + to_string(quantTransformacao) + ".ppm";
					gravar(&ppmSaida, caminhoNovaImagem);
					cout<< "\nImagem salva: " << caminhoNovaImagem;

					resetaPixels(&ppmSaida);
				}
				else if (orientacao == "b")
				{
					float b;
					cout<< "Insira o valor do cisalhamento vertical (a): ";
					cin>> b;
					
					M = getCisalhamentoVertical(b) * M;
					transformacao2DInversa(&ppmEntrada, &ppmSaida, T * M.inverse() * Tinverso);
					string caminhoNovaImagem = "imagens/transformacao-" + to_string(quantTransformacao) + ".ppm";
					gravar(&ppmSaida, caminhoNovaImagem);
					cout<< "\nImagem salva: " << caminhoNovaImagem;

					resetaPixels(&ppmSaida);
				}
			}
			else
				cout<< "\nErro: transformação inserida é inválida!";

			cout<< "\n\nDeseja finalizar a execução? (S/N): ";
			cin>> acao;

			if (acao == "S" || acao == "s")
			{
				sair = true;
				cout<< "\n\nSaindo do programa...\n\n";
			}
		}
	}
	else
		cout<< "\nErro: o caminho inserido é inválido!";
	
	return EXIT_SUCCESS; 
}