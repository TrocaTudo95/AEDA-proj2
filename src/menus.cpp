﻿#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <map>
#include <queue>
#include <cmath>

#include "professor.h"
#include "uteis.h"
#include "aula.h"
#include "piscina.h"
#include "utente.h"
#include "data.h"


using namespace std;

// @@@ funções atualização
void atualizar_aulas_piscina(Piscina &p1)
{
	string dia_S;
	int dia = dia_do_sistema();
	int mes = mes_do_sistema();
	int ano = ano_do_sistema();
	dia_S = calculo_dia_da_semana(dia, mes, ano);

	for (size_t i{}; i < 7; i++) {
		dia_S = calculo_dia_da_semana(dia, mes, ano);

		map<string, string>::iterator it = p1.horariomap.find(dia_S);
		if (it != p1.horariomap.end())
		{
			string temp = it->second;
			int hora_i = stoi(temp.substr(0, 2));
			int min_i = stoi(temp.substr(3, 2));
			int hora_f = stoi(temp.substr(8, 2));
			int min_f = stoi(temp.substr(11, 2));
			int hora{ hora_i };
			int minutos{ min_i };


			do {
				// inicio aula

				Aula *a1 = new AulaPro();
				Data d_ini{};
				d_ini.setAno(ano);
				d_ini.setMes(mes);
				d_ini.setDia(dia);
				d_ini.setDiaSemana(dia_S);
				d_ini.setHoras(hora);
				d_ini.setMinutos(minutos);
				a1->setInicio(d_ini);

				//fim aula
				hora++;
				Data d_fim{};
				d_fim.setAno(ano);
				d_fim.setMes(mes);
				d_fim.setDia(dia);
				d_fim.setDiaSemana(dia_S);
				d_fim.setHoras(hora);
				d_fim.setMinutos(minutos);

				a1->setFim(d_fim);
				a1->setId();
				a1->setDuracao(60);


				a1->setNumMaximo(p1.getLotacao());

				if (!p1.aulaSobreposta(a1)) {
					//p1.atribui_profaula(a1);
					p1.adicionaAula(a1);
				}

			} while ((hora < (hora_f - 1)) || ((hora == (hora_f - 1) && (minutos <= min_f)))); // fim while

		}

		dia++;
		if (!dia_existe(dia, mes, ano)) {
			dia = 1;
			mes++;
		}
		if (!dia_existe(dia, mes, ano)) {
			dia = 1;
			mes = 1;
			ano++;
		}

	} // fim ciclo for

	p1.ordena_por_data();
	//	p1.atribuiprofs();
}

void apagar_aulas_old(Piscina &p1)
{
	string dia_S;
	int dia = dia_do_sistema();
	int mes = mes_do_sistema();
	int ano = ano_do_sistema();
	dia_S = calculo_dia_da_semana(dia, mes, ano);
	int horas = horas_do_sistema();
	int min = minutos_do_sistema();

	Data atual(dia, mes, ano, horas, min);
	atual.setDiaSemana(dia_S);

	for (size_t k = 0; k< p1.getHorario().size(); k++)
	{
		if (p1.getHorario()[k]->getFim() < atual)
		{
			p1.apaga_aula(p1.getHorario()[k]);
			k--;
		}
	}


}

void set_prioridades(vector<Piscina> &piscinas, Piscina &piscina_criada) {
	vector<Piscina > temp = piscinas;
	float distmin = 100000000;
	float dist = 0;
	unsigned int id = 0;
	unsigned int c = 0;
	for (size_t i = 0; i < temp.size(); i++) {


		for (size_t j = 0; j < temp.size(); i++) {
			dist = sqrt(pow(piscina_criada.getX() - temp[j].getX(), 2) + pow(piscina_criada.getY() - temp[j].getY(), 2));
			if (dist < distmin) {
				dist = distmin;
				id = j;
			}
		}
		piscinas[id].setProximidade(c++);
		temp.erase(temp.begin() + id);

	}

}

priority_queue<Piscina> cria_listapri(const vector<Piscina> &piscinas) {
	priority_queue<Piscina> piscinas_pri;

	for (size_t i = 0; i < piscinas.size(); i++) {
		piscinas_pri.push(piscinas[i]);
	}
	return piscinas_pri;
}


// 7.1.1 Detalhes cliente
void detalhes_cliente(Utente &ute)
{
	limparEcra();
	cabecalho();

	textcolor(LIGHT_GRAY);
	cout << "\t\t Nome: ";
	textcolor(WHITE);
	cout << ute.getNome();


	textcolor(LIGHT_GRAY);
	cout << "\t\t Mail: ";
	textcolor(WHITE);
	//cout << ute.getMail();

	textcolor(LIGHT_GRAY);
	cout << "\t\t Telefone: ";
	textcolor(WHITE);
	//cout << ute.getTelefone();




}


// 7.1 Gerir registros
void menu_registros_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 10 + opcao_b;
	int y1 = 10 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void menu_registros(Piscina &p1, Utente &ute)
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();

			cout << endl << endl;
			cout << "\t\t\t\t    Informacoes pessoais" << endl;
			cout << "\t\t\t\t    Aulas frequentadas" << endl;
			cout << "\t\t\t\t    (...) acrescentar" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << endl << endl;

		}

		imprimir = false;

		menu_registros_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 4);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				imprimir = true;
				break;

			case 2:
				imprimir = true;
				break;

			case 3:
				imprimir = true;
				break;

			case 4:
				break;

			}

	} while ((opcao != 4) || (tecla != ENTER));

	cout << endl << endl;

	return;

}


// 7 MENU UTENTE
void menu_utente_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 10+ opcao_b;
	int y1 = 10 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void menu_utente(Piscina &p1, Utente &ute)
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();
			cout << endl;
			cout << "\t\t\t Bem vindo, ";
			textcolor(CYAN);
			cout << ute.getNome();
			textcolor(WHITE);

			cout << endl << endl;
			cout << "\t\t\t\t    Ver registros" << endl;
			cout << "\t\t\t\t    Inscrever aulas" << endl;
			cout << "\t\t\t\t    Loja" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << "\t\t\t\t\t\t\t\t" << mostrar_mes(mes_do_sistema()) << " " << ano_do_sistema();
			cout << endl << endl;

		}

		imprimir = false;

		menu_utente_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 4);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				imprimir = true;
				break;

			case 2:
				imprimir = true;
				break;

			case 3:
				imprimir = true;
				break;

			case 4:
				break;

			}

	} while ((opcao != 4) || (tecla != ENTER));

	cout << endl << endl;
	
	return;

}


// 6.4 Menu gerir professor
void menu_gerir_professores_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 9 + opcao_b;
	int y1 = 9 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void menu_gerir_professores(Piscina &p1)
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();

			cout << endl << endl;
			cout << "\t\t\t\t    Listar professores" << endl;
			cout << "\t\t\t\t    Adicionar professor" << endl;
			cout << "\t\t\t\t    Remover professor" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << endl << endl;

		}

		imprimir = false;

		menu_gerir_professores_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 4);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				imprimir = true;
				break;

			case 2:
				imprimir = true;
				break;

			case 3:
				imprimir = true;
				break;

			case 4:
				break;

			}

	} while ((opcao != 4) || (tecla != ENTER));

	cout << endl << endl;

	return;

}


// 6.3.1 Listar aulas
void listar_aulas(Piscina &p1)
{
	limparEcra();
	cabecalho();
	cout << endl << endl;

	for (size_t i{}; i < p1.getHorario().size(); i++)
	{
		textcolor(LIGHT_GRAY);
		cout << "\t\tId aula: ";
		textcolor(WHITE);

		cout << p1.getHorario()[i]->getId() << endl;

		textcolor(LIGHT_GRAY);
		cout << "\t\tData: ";
		textcolor(WHITE);
		
		cout << p1.getHorario()[i]->getInicio().getDiaSemana() << ", ";

		cout << p1.getHorario()[i]->getInicio().getDia() << " de ";
		cout << mostrar_mes(p1.getHorario()[i]->getInicio().getMes()) << " de ";
		cout << p1.getHorario()[i]->getInicio().getAno() << endl;
	
		textcolor(LIGHT_GRAY);
		cout << "\t\tHora: ";
		textcolor(WHITE);

		if (p1.getHorario()[i]->getInicio().getHoras() < 10)
			cout << "0" << p1.getHorario()[i]->getInicio().getHoras();
		else
			cout << p1.getHorario()[i]->getInicio().getHoras();

		cout << ":";
		if (p1.getHorario()[i]->getInicio().getMinutos() < 10)
			cout << "0" << p1.getHorario()[i]->getInicio().getMinutos();
		else
			cout << p1.getHorario()[i]->getInicio().getMinutos();


		cout << " - ";

		if (p1.getHorario()[i]->getFim().getHoras() < 10)
			cout << "0" << p1.getHorario()[i]->getFim().getHoras();
		else
			cout << p1.getHorario()[i]->getFim().getHoras();

		cout << ":";
		if (p1.getHorario()[i]->getFim().getMinutos() < 10)
			cout << "0" << p1.getHorario()[i]->getFim().getMinutos() << endl;
		else
			cout << p1.getHorario()[i]->getFim().getMinutos() << endl;

		textcolor(LIGHT_GRAY);
		cout << "\t\tTipo: " << endl << endl;
		textcolor(WHITE);


	}

	textcolor(CYAN);
	cout << "\n\n\n Prima 'ENTER' para sair \n";
	textcolor(WHITE);
	cin.ignore(256,'\n');

}

// 6.3 Menu gerir aulas
void menu_gerir_aulas_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 9 + opcao_b;
	int y1 = 9 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void menu_gerir_aulas(Piscina &p1)
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();

			cout << endl << endl;
			cout << "\t\t\t\t    Listar aulas" << endl;
			cout << "\t\t\t\t    Adicionar aula" << endl;
			cout << "\t\t\t\t    Remover aula" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << endl << endl;

		}

		imprimir = false;

		menu_gerir_aulas_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 4);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				listar_aulas(p1);
				imprimir = true;
				break;

			case 2:
				imprimir = true;
				break;

			case 3:
				imprimir = true;
				break;

			case 4:
				break;

			}

	} while ((opcao != 4) || (tecla != ENTER));

	cout << endl << endl;

	return;

}


// 6.2 Menu gerir utente
void menu_gerir_utentes_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 9 + opcao_b;
	int y1 = 9 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void menu_gerir_utentes(Piscina &p1)
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();

			cout << endl << endl;
			cout << "\t\t\t\t    Listar utentes" << endl;
			cout << "\t\t\t\t    Adicionar utente" << endl;
			cout << "\t\t\t\t    Remover utente" << endl;
			cout << "\t\t\t\t    Utentes inativos" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << endl << endl;

		}

		imprimir = false;

		menu_gerir_utentes_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 5);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				imprimir = true;
				break;

			case 2:
				imprimir = true;
				break;

			case 3:
				imprimir = true;
				break;

			case 4:
				imprimir = true;
				break;

			case 5:
				break;

			}

	} while ((opcao != 5) || (tecla != ENTER));

	cout << endl << endl;

	return;

}


// 6.1.2 Mostrar horario
void mostrar_horario(Piscina p1)
{
	int tecla;
	{
		limparEcra();
		cabecalho();

		int a = 186;
		char char1 = a;  // obter -> ║

		int b = 201;
		char char2 = b; // obter -> ╔

		int c = 205;
		char char3 = c; // obter -> ═

		int d = 187;
		char char4 = d; // obter -> ╗

		int e = 200;
		char char5 = e; // obter -> ╚

		int f = 188;
		char char6 = f; // obter -> ╝

		int g = 204;
		char char7 = g; // obter -> ╠

		int h = 185;
		char char8 = h; // obter -> ╣

		int i = 203;
		char char9 = i; // obter -> ╦

		int j = 206;
		char char10 = j; // obter -> ╬

		int k = 202;
		char char11 = k; // obter -> ╩

		cout << "\t      " << char2 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char9 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char4 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char5 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char11 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char6 << endl;

		textcolor(CYAN);
		gotoxy(22, 4);
		cout << "Dias";

		gotoxy(53, 4);
		cout << "Horario";
		textcolor(WHITE);



		for (size_t i = 1; i <= 7; i++)
		{
			map<string, string>::iterator it{};
			switch (i)
			{
			case 1:
				it = p1.horariomap.find("segunda");
				gotoxy(21, 6);
				cout << "Segunda";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 6);
					cout << it->second;
				}
				else {
					gotoxy(53, 6);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 2:
				it = p1.horariomap.find("terca");
				gotoxy(21, 8);
				cout << "Terca";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 8);
					cout << it->second;
				}
				else {
					gotoxy(53, 8);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 3:
				it = p1.horariomap.find("quarta");
				gotoxy(21, 10);
				cout << "Quarta";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 10);
					cout << it->second;
				}
				else {
					gotoxy(53, 10);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 4:
				it = p1.horariomap.find("quinta");
				gotoxy(21, 12);
				cout << "Quinta";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 12);
					cout << it->second;
				}
				else {
					gotoxy(53, 12);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 5:
				it = p1.horariomap.find("sexta");
				gotoxy(21, 14);
				cout << "Sexta";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 14);
					cout << it->second;
				}
				else {
					gotoxy(53, 14);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 6:
				it = p1.horariomap.find("sabado");
				gotoxy(21, 16);
				cout << "Sabado";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 16);
					cout << it->second;
				}
				else {
					gotoxy(53, 16);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 7:
				it = p1.horariomap.find("domingo");
				gotoxy(21, 18);
				cout << "Domingo";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 18);
					cout << it->second;
				}
				else {
					gotoxy(53, 18);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			}

		}

		textcolor(CYAN);
		cout << "\n\n\t Prima ENTER para continuar" << endl;
		textcolor(WHITE);
		int opcao;
		tecla = opcao_valida(opcao, 1, 1);
	} while (tecla != ENTER);
}


// 6.1.1 Estado atual
void estado_atual(Piscina &p1)
{
	limparEcra();
	cabecalho();

	cout << endl << endl;

	Data atual{};
	int dia = dia_do_sistema();
	int mes = mes_do_sistema();
	int ano = ano_do_sistema();
	string dia_S = calculo_dia_da_semana(dia, mes, ano);
	int horas = horas_do_sistema();
	int minutos = minutos_do_sistema();

	atual.setAno(ano);
	atual.setMes(mes);
	atual.setDia(dia);
	atual.setHoras(horas);
	atual.setMinutos(minutos);
	atual.setDiaSemana(dia_S);

	unsigned int utentes_a{};
	bool existe{ false };
	unsigned int id{};

	for (size_t i{}; i < p1.getHorario().size(); i++) {
		if ((p1.getHorario()[i]->getInicio() < atual)
			&& (atual < p1.getHorario()[i]->getFim())) {
			existe = true;
			id = i;
			break;

		}

	}

	utentes_a = p1.getUtentesNaPisicina().size();

	if (!existe)
	{
		cout << "\n\t\t\t Atualmente a piscina encontra-se fechada!" << endl << endl;

	}
	else {
		cout << "\t\tId aula: " << p1.getHorario()[id]->getId() << endl;
		cout << "\t\tDuracao: " << p1.getHorario()[id]->getDuracao() << endl;
		cout << "\t\tHora inicio: ";

		if (p1.getHorario()[id]->getInicio().getHoras()<10)
			cout << "0" << p1.getHorario()[id]->getInicio().getHoras();
		else
			cout << p1.getHorario()[id]->getInicio().getHoras();

		cout << ":";
		if (p1.getHorario()[id]->getInicio().getMinutos()<10)
			cout << "0" << p1.getHorario()[id]->getInicio().getMinutos() << endl;
		else
			cout << p1.getHorario()[id]->getInicio().getMinutos() << endl;


		cout << "\t\tHora fim: ";

		if (p1.getHorario()[id]->getFim().getHoras() < 10)
			cout << "0" << p1.getHorario()[id]->getFim().getHoras();
		else
			cout << p1.getHorario()[id]->getFim().getHoras();

		cout << ":";
		if (p1.getHorario()[id]->getFim().getMinutos() < 10)
			cout << "0" << p1.getHorario()[id]->getFim().getMinutos() << endl;
		else
			cout << p1.getHorario()[id]->getFim().getMinutos() << endl;

		cout << "\t\tProfesor: " << p1.getHorario()[id]->getProfessor();
		cout << endl << endl;


		faz_grafico(utentes_a, "pessoas na piscina", p1.getLotacao(), "vagas", "lugares na piscina");



	}

	textcolor(CYAN);
	cout << "\n\n\t\tPrima 'Enter' para continuar " << endl;
	textcolor(WHITE);
	cin.ignore(256, '\n');

	return;

}


// 6.1 Menu gerir piscina
void menu_gerir_piscina_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 9 + opcao_b;
	int y1 = 9 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void menu_gerir_piscina(Piscina &p1)
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();

			cout << endl << endl;
			cout << "\t\t\t\t    Estado atual" << endl;
			cout << "\t\t\t\t    Horario de funcionamento" << endl;
			cout << "\t\t\t\t    Piscinas nas proximidades" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << endl << endl;

		}

		imprimir = false;

		menu_gerir_piscina_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 4);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				estado_atual(p1);
				imprimir = true;
				break;

			case 2:
				mostrar_horario(p1);
				imprimir = true;
				break;

			case 3:
				imprimir = true;
				break;

			case 4:
				break;

			}

	} while ((opcao != 4) || (tecla != ENTER));

	cout << endl << endl;

	return;

}


// 6 MENU ADMIN
void menu_admin_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 10 + opcao_b;
	int y1 = 10 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);

	return;
}

void menu_admin(Piscina &p1, string &fichPiscina, string &fichUtentes, string &fichAulas, string &fichProfessores)
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	atualizar_aulas_piscina(p1);

	do
	{
		apagar_aulas_old(p1);

		if (imprimir)
		{
			limparEcra();
			titulo();
			cout << endl;
			cout << "\t\t\t Bem vindo, ";
			textcolor(CYAN);
			cout << "admin";
			textcolor(WHITE);

			cout << endl << endl;
			cout << "\t\t\t\t    Gerir Piscina" << endl;
			cout << "\t\t\t\t    Gerir Utentes" << endl;
			cout << "\t\t\t\t    Gerir Aulas" << endl;
			cout << "\t\t\t\t    Gerir Professores" << endl;
			cout << "\t\t\t\t    Gerir Loja" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << "\t\t\t\t\t\t\t\t" << mostrar_mes(mes_do_sistema()) << " " << ano_do_sistema();
			cout << endl << endl;

		}

		imprimir = false;

		menu_admin_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 6);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				menu_gerir_piscina(p1);
				imprimir = true;
				break;

			case 2:
				menu_gerir_utentes(p1);
				imprimir = true;
				break;

			case 3:
				menu_gerir_aulas(p1);
				imprimir = true;
				break;

			case 4:
				menu_gerir_professores(p1);
				imprimir = true;
				break;

			case 5:
				break;

			case 6:
				break;

			}

	} while ((opcao != 6) || (tecla != ENTER));

	cout << endl << endl;

	return;

}


// 5.1 Entrar como utente
void encontra_parecido(Piscina p1, string nome, Utente &melhor)
{
	unsigned int melhor_count = 0;
	unsigned int atual=0;
	string nome_a;


	for (size_t i = 0; i < p1.getUtentes().size(); i++)
	{
		atual = 0;
		nome_a = p1.getUtentes()[i]->getNome();

		for (size_t j = 0; j < nome.size(); j++)
		{
			for (size_t k = 0; k <nome_a.size(); k++)
			{
				if (nome[j] == nome_a[k])
				{
					atual++;
					break;
				}
			}

		}

		if (atual > melhor_count)
		{
			melhor = *p1.getUtentes()[i];
			melhor_count = atual;
		}
		else if (atual == melhor_count)
		{
			int a1, a2;
			a1 = abs(nome[0] - melhor.getNome()[0]);
			a2 = abs(nome[0] - p1.getUtentes()[i]->getNome()[0]);

			if (a2 < a1)
			{
				melhor = *p1.getUtentes()[i];
				melhor_count = atual;
			}
		}
	}

	return;
}

void entrar_como__utente_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 7 + opcao_b;
	int y1 = 7 + opcao;

	gotoxy(22, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(22, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void entrar_como_utente(Piscina &p1, string &fichPiscina, string &fichUtentes, string &fichAulas, string &fichProfessores)
{
	limparEcra();
	cabecalho();
	bool encontrou = false;
	vector<Utente *> ute = p1.getUtentes();
	Utente *novo = new Utente();

	string nome;

	if (p1.getUtentes().size() >= 1)
	{
		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Nome utente: ";
		textcolor(WHITE);
		getline(cin, nome);
		nome = delete_space(nome);
		if (nome == "sair")
			return;

		for (size_t i = 0; i < ute.size(); i++)
		{
			if (ute[i]->getNome() == nome)
			{
				novo = ute[i];
				encontrou = true;
				break;
			}
		}
		if (!encontrou)
		{
			encontra_parecido(p1, nome, (*novo));
			cout << endl;
			textcolor(RED);
			cout << "\t\t Cliente nao encontrado.";
			cout << " Sera que quis dizer " << novo->getNome() << "?" << endl << endl;
			textcolor(WHITE);

			cout << "\t\t\tSim" << endl;
			cout << "\t\t\tNao" << endl;

			int opcao = 1, opcao_b = 1, tecla;

			while (1)
			{
				entrar_como__utente_ops(opcao, opcao_b);
				opcao_b = opcao;
				tecla = opcao_valida(opcao, 1, 2);
				Sleep(100);
				
				if (tecla == ENTER)
				{
					switch (opcao)
					{
					case 1: 
						menu_utente(p1,(*novo));
						break;
					case 2:
						break;
					}

					break;
				}


			}

		}
		else menu_utente(p1, (*novo));

	}
	else {

		textcolor(RED);
		cout << "\n\n\t\t A lista de utentes encontra-se vazia " << endl;
		textcolor(WHITE);
		cout << endl << endl;
		cout << "\t\t Prima ENTER para voltar" << endl;
		cin.ignore(256, '\n');
	}

}


// 5 MENU GERAL
void menu_geral_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;

	int y = 9 + opcao_b;
	int y1 = 9 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);



	gotoxy(0, 21);
}

void menu_geral(Piscina &p1, string &fichPiscina, string &fichUtentes, string &fichAulas, string &fichProfessores)
{

	////////////////////////////////////////////////////////////////////////// <- UTENTES PARA TESTE ---- APAGAR
 	Utente *novo = new Utente();
	string a = "David";
	novo->setNome(a);
	p1.addUtente(novo);
	
	Utente *novo1 = new Utente();
	string a1 = "Divad";
	novo1->setNome(a1);
	p1.addUtente(novo1);

	////////////////////////////////////////////////////////////////////////////

	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();
			cout << endl << endl;
			cout << "\t\t\t\t    Entrar como administrador" << endl;
			cout << "\t\t\t\t    Entrar como utente" << endl;
			cout << "\t\t\t\t    Voltar ao menu anterior" << endl;

			cout << endl << endl;

		}

		imprimir = false;

		menu_geral_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 3);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				menu_admin(p1, fichPiscina, fichUtentes, fichAulas, fichProfessores);
				imprimir = true;
				break;

			case 2:
				entrar_como_utente(p1, fichPiscina, fichUtentes, fichAulas, fichProfessores);
				imprimir = true;
				break;


			case 3:
				return;
				break;

			}

	} while ((opcao != 3) || (tecla != 13));

	cout << endl << endl;



}


// 4.4 Testar ficheiro professores
string testar_fich_professores()
{
	string opcao;
	ifstream in;
	bool ok;

	do
	{
		cout << "\n\t\t Professores: ";
		getline(cin, opcao);
		if (opcao == "sair")
			return opcao;

		opcao = opcao + ".txt";

		in.open(opcao);

		if (!in.is_open())
		{
			textcolor(RED);
			cout << "\t\tO ficheiro " << opcao << " nao existe! \n";
			textcolor(WHITE);

		}
		else {
			string tipo;
			getline(in, tipo, ';');
			tipo = delete_space(tipo);

			if (tipo == "Professor") {
				ok = true;
			}
			else {
				textcolor(RED);
				cout << "\t\tO ficheiro " << opcao
					<< " nao contem informacoes sobre Professores! \n";
				textcolor(WHITE);
				ok = false;
				in.close();
			}
		}

	} while (!in.is_open() || !ok);

	in.close();


	return opcao;

}


// 4.3 Testar ficheiro aulas
string testar_fich_aulas()
{
	string opcao;
	ifstream in{};
	bool ok{};

	do
	{
		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Aulas: ";
		textcolor(WHITE);
		getline(cin, opcao);
		if (opcao == "sair")
			return opcao;

		opcao = opcao + ".txt";

		in.open(opcao);

		if (!in.is_open())
		{
			textcolor(RED);
			cout << "\t\tO ficheiro " << opcao << " nao existe! \n";
			textcolor(WHITE);
		}
		else {
			string tipo;
			getline(in, tipo, ';');
			tipo = delete_space(tipo);

			if (tipo == "Aula") {
				ok = true;
			}
			else {
				textcolor(RED);
				cout << "\t\tO ficheiro " << opcao << " nao contem informacoes sobre a Aulas! \n";
				textcolor(WHITE);
				ok = false;
				in.close();
			}
		}

	} while (!in.is_open() || !ok);

	in.close();

	return opcao;

}


// 4.2 Testar ficheiro utentes
string testar_fich_utentes()
{
	string opcao;
	ifstream in{};
	bool ok{};

	do
	{
		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Utentes: ";
		textcolor(WHITE);
		getline(cin, opcao);
		if (opcao == "sair")
			return opcao;

		opcao = opcao + ".txt";

		in.open(opcao);

		if (!in.is_open())
		{ 
			textcolor(RED);
			cout << "\t\tO ficheiro " << opcao << " nao existe! \n";
			textcolor(WHITE);
		}
		else {
			string tipo;
			getline(in, tipo, ';');
			tipo = delete_space(tipo);

			if (tipo == "Utente") {
				ok = true;
			}
			else {
				textcolor(RED);
				cout << "\t\tO ficheiro " << opcao << " nao contem informacoes sobre a Utentes! \n";
				textcolor(WHITE);
				ok = false;
				in.close();
			}
		}


	} while (!in.is_open() || !ok);

	in.close();

	return opcao;


}


// 4.1 Testar ficheiro piscina
string testar_fich_piscina() 
{
	string opcao;
	ifstream in{};

	bool ok{};

	do {
		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Piscina: ";
		textcolor(WHITE);
		getline(cin, opcao);

		if (opcao == "sair")
			return opcao;

		opcao = opcao + ".txt";

		in.open(opcao);

		if (!in.is_open())
		{ 
			textcolor(RED);
			cout << "\t\tO ficheiro " << opcao << " nao existe! \n";
			textcolor(WHITE);
		}
		else {
			string tipo;
			getline(in, tipo, ';');
			tipo = delete_space(tipo);

			if (tipo == "Piscina") {
				ok = true;
			}
			else {
				textcolor(RED);
				cout << "\t\tO ficheiro " << opcao << " nao contem informacoes sobre a Piscina! \n";
				textcolor(WHITE);
				ok = false;
				in.close();
			}
		}

	} while (!in.is_open() || !ok);

	in.close();

	return opcao;

}


// 4 CARREGAR PISCINA
void importar_ficheiros(string &fichPiscina, string &fichUtentes, string &fichAulas, string &fichProfessores) /// importar conteudo dos ficheiros
{
	Piscina p1;
	p1.ler_FichPiscina(fichPiscina);
	p1.ler_FichAulas(fichAulas);
	p1.ler_FichProfessores(fichProfessores);
	p1.ler_FichUtente(fichUtentes);
	p1.utentes_aulas();
	p1.professores_aulas();
	menu_geral(p1, fichPiscina, fichUtentes, fichAulas, fichProfessores);

	return;
}

void escolher_ficheiros() /// um para cada classe
{
	string fichPiscina, fichUtentes, fichAulas, fichProfessores;

	limparEcra();
	cabecalho();
	textcolor(LIGHT_GRAY);
	cout << "\n\n\t\t Defina os ficheiros que contem informacoes sobre: \n";
	textcolor(WHITE);
	fichPiscina = testar_fich_piscina();
	if (fichPiscina != "sair") {
		fichUtentes = testar_fich_utentes();
		if (fichUtentes != "sair") {
			fichAulas = testar_fich_aulas();
			if (fichAulas != "sair") {
				fichProfessores = testar_fich_professores();
				if (fichProfessores != "sair")
				{
					importar_ficheiros(fichPiscina, fichUtentes, fichAulas, fichProfessores);
					return;
				}
			}
		}
	}

	cout << " Importacao cancelada pelo utilizador! Redirecionando ...\n ";
	system("sleep 2");
}


// 3.6 Verificação antes de avançar
unsigned int check(Piscina &p1, string &fichPiscina, string &fichUtentes)
{
	limparEcra();
	cabecalho();

	bool falta = false;
	unsigned int nome = 0, lotacao = 0, horario = 0, base_dados = 0;
	//unsigned int localização = 0;
	int a = 254;
	char square = a;

	if (p1.getNome() == "")
	{
		falta = true;
	}
	else nome = 1;

	if (p1.getLotacao() == NULL)
	{
		falta = true;
	}
	else lotacao = 1;

	map<string, string>::iterator it = p1.horariomap.begin();

	if (it == p1.horariomap.end())
	{
		falta = true;
	}
	else horario = 1;

	/// fazer o mesmo para a localizacao

	if ((fichUtentes == "") && (fichPiscina == ""))
	{
		falta = true;
	}
	else base_dados = 1;


	if (falta)
	{
		cout << "\t\t Informacoes em falta: " << endl;
		textcolor(RED);
		if (!nome)
		{
			cout << "\t\t " << square << " nome" << endl;
		}
		if (!lotacao)
		{
			cout << "\t\t " << square << " lotacao" << endl;
		}
		if (!horario)
		{
			cout << "\t\t " << square << " horario" << endl;
		}

		// fazer o mesmo para a localizacao

		if (!base_dados)
		{
			cout << "\t\t " << square << " base de dados" << endl;
		}
		textcolor(WHITE);

		cout << "\n\n\t\t Prima enter para voltar";
		gotoxy(0, 21);
		cin.ignore(256, '\n');

		return 0;
	}

	return 1;
}


// 3.5 Definir base de dados
void definir_base_de_dados(string &fichPiscina, string &fichUtentes, string &fichAulas, string &fichProfessores)
{
	limparEcra();
	cabecalho();
	int a = 254;
	char square = a;


	if (fichPiscina != "")
	{
		textcolor(RED);
		cout << "\n\n\t\tPiscina: ";
		textcolor(WHITE);
		cout << fichPiscina << endl;
		textcolor(RED);
		cout << "\t\tUtentes: ";
		textcolor(WHITE);
		cout << fichUtentes << endl;
		textcolor(RED);
		cout << "\t\tAulas: ";
		textcolor(WHITE);
		cout << fichAulas << endl;
		textcolor(RED);
		cout << "\t\tProfessores: ";
		textcolor(WHITE);
		cout << fichProfessores << endl;

		string opcao;

		do {
			textcolor(LIGHT_GRAY);
			cout << "\n\t\t Deseja aterar? ";
			textcolor(WHITE);
			getline(cin, opcao);

		} while ((opcao != "nao") && (opcao != "n") && (opcao != "sim") && (opcao != "s"));


		if ((opcao == "nao") || (opcao == "n")) /// se nao deseja guardar
			return;


	}
	
		limparEcra();
		cabecalho();
		textcolor(LIGHT_GRAY);
		cout << "\n\n\t\t Escolha o ficheiro para guardar informacoes sobre: \n\n"; 
		textcolor(WHITE);


	ifstream in{};
	ofstream out{};

	 // escolher nome do ficheiro para guardar Piscina

	string piscina_temp;
	do {
		in.close();
		out.close();

		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Piscina: ";
		textcolor(WHITE);
		cin.clear();
		getline(cin, piscina_temp);
		if (piscina_temp == "sair")
			return; //sair sem modificacoes

		piscina_temp += ".txt";
		in.open(piscina_temp);

		if (!in.fail()) {
			textcolor(RED);
			cout << "\t\t" << square << piscina_temp << " ja existente!" << endl;
			textcolor(WHITE);
		}
		else {

			out.open(piscina_temp);
			if (out.fail()) {
				textcolor(RED);
				cout << piscina_temp << " : nome do ficheiro invalido" << endl;
				textcolor(WHITE);
			}
		}

	} while (!in.fail() || out.fail());

	in.close();
	out.close();

	// escolher nome do ficheiro para guardar Utentes

	string utentes_temp;

	do {
		in.close();
		out.close();

		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Utentes: ";
		textcolor(WHITE);
		cin.clear();
		getline(cin, utentes_temp);

		if (utentes_temp == "sair")
			return; //sair sem modificacoes

		utentes_temp += ".txt";
		in.open(utentes_temp);

		if (!in.fail()) {
			textcolor(RED);
			cout << "\t\t"<< square << utentes_temp << " ja existente!" << endl;
			textcolor(WHITE);
		}
		else {

			out.open(utentes_temp);
			if (out.fail()) {
				textcolor(RED);
				cout << utentes_temp << " : nome do ficheiro invalido" << endl;
				textcolor(WHITE);
			}
		}

	} while (!in.fail() || out.fail());

	in.close();
	out.close();


	// escolher nome do ficheiro para guardar Aulas

	string aulas_temp;

	do {
		in.close();
		out.close();

		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Aulas: ";
		textcolor(WHITE);
		cin.clear();
		getline(cin, aulas_temp);

		if (aulas_temp == "sair")
			return; //sair sem modificacoes

		aulas_temp += ".txt";
		in.open(aulas_temp);

		if (!in.fail()) {
			textcolor(RED);
			cout << "\t\t " << square <<aulas_temp << " ja existente!" << endl;
			textcolor(WHITE);
		}
		else {

			out.open(aulas_temp);
			if (out.fail()) {
				textcolor(RED);
				cout << aulas_temp << " : nome do ficheiro invalido" << endl;
				textcolor(WHITE);
			}
		}

	} while (!in.fail() || out.fail());

	in.close();
	out.close();


	// escolher nome do ficheiro para guardar Professores

	string profs_temp;

	do {
		in.close();
		out.close();

		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Professores: ";
		textcolor(WHITE);
		cin.clear();
		getline(cin, profs_temp);

		if (profs_temp == "sair")
			return; //sair sem modificacoes

		profs_temp += ".txt";
		in.open(profs_temp);

		if (!in.fail()) {
			textcolor(RED);
			cout << "\t\t" << square <<profs_temp << " ja existente!" << endl;
			textcolor(WHITE);
		}
		else {

			out.open(profs_temp);
			if (out.fail()) {
				textcolor(RED);
				cout << profs_temp << " : nome do ficheiro invalido" << endl;
				textcolor(WHITE);
			}
		}

	} while (!in.fail() || out.fail());

	in.close();
	out.close();

	const char *lixo = piscina_temp.c_str();  // apagar ficheiro temporario piscina
	remove(lixo);
	const char *lixo1 = utentes_temp.c_str();  // apagar ficheiro temporario utentes
	remove(lixo1);
	const char *lixo2 = aulas_temp.c_str();  // apagar ficheiro temporario aulas
	remove(lixo2);
	const char *lixo3 = profs_temp.c_str();  // apagar ficheiro temporario lixo
	remove(lixo3);

	/// Guardar ou não

	string opcao{};

	do {
		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Deseja guardar? ";
		textcolor(WHITE);
		getline(cin, opcao);

	} while ((opcao != "nao") && (opcao != "n") && (opcao != "sim")
		&& (opcao != "s"));

	if ((opcao == "nao") || (opcao == "n")) /// se nao deseja guardar
		return; /// retorna sem modificacoes

				  /// guardar nomes dos ficheiros

	fichPiscina = piscina_temp;
	fichUtentes = utentes_temp;
	fichAulas = aulas_temp;
	fichProfessores = profs_temp;

}


//3.4 Definir localizacao


//3.3 Definir horario
void mostrar_calendario(Piscina p1)
{
	int tecla;
	{
		limparEcra();
		cabecalho();

		int a = 186;
		char char1 = a;  // obter -> ║

		int b = 201;
		char char2 = b; // obter -> ╔

		int c = 205;
		char char3 = c; // obter -> ═

		int d = 187;
		char char4 = d; // obter -> ╗

		int e = 200;
		char char5 = e; // obter -> ╚

		int f = 188;
		char char6 = f; // obter -> ╝

		int g = 204;
		char char7 = g; // obter -> ╠

		int h = 185;
		char char8 = h; // obter -> ╣

		int i = 203;
		char char9 = i; // obter -> ╦

		int j = 206;
		char char10 = j; // obter -> ╬

		int k = 202;
		char char11 = k; // obter -> ╩

		cout << "\t      " << char2 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char9 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char4 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char7 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char10 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char8 << endl;
		cout << "\t      " << char1 << "                    " << char1 << "                                       " << char1 << endl;
		cout << "\t      " << char5 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char11 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char3 << char6 << endl;

		textcolor(CYAN);
		gotoxy(22, 4);
		cout << "Dias";

		gotoxy(53, 4);
		cout << "Horario";
		textcolor(WHITE);



		for (size_t i = 1; i <= 7; i++)
		{
			map<string, string>::iterator it{};
			switch (i)
			{
			case 1:
				it = p1.horariomap.find("segunda");
				gotoxy(21, 6);
				cout << "Segunda";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 6);
					cout << it->second;
				}
				else {
					gotoxy(53, 6);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 2:
				it = p1.horariomap.find("terca");
				gotoxy(21, 8);
				cout << "Terca";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 8);
					cout << it->second;
				}
				else {
					gotoxy(53, 8);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 3:
				it = p1.horariomap.find("quarta");
				gotoxy(21, 10);
				cout << "Quarta";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 10);
					cout << it->second;
				}
				else {
					gotoxy(53, 10);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 4:
				it = p1.horariomap.find("quinta");
				gotoxy(21, 12);
				cout << "Quinta";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 12);
					cout << it->second;
				}
				else {
					gotoxy(53, 12);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 5:
				it = p1.horariomap.find("sexta");
				gotoxy(21, 14);
				cout << "Sexta";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 14);
					cout << it->second;
				}
				else {
					gotoxy(53, 14);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 6:
				it = p1.horariomap.find("sabado");
				gotoxy(21, 16);
				cout << "Sabado";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 16);
					cout << it->second;
				}
				else {
					gotoxy(53, 16);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			case 7:
				it = p1.horariomap.find("domingo");
				gotoxy(21, 18);
				cout << "Domingo";
				if (it != p1.horariomap.end())
				{
					gotoxy(50, 18);
					cout << it->second;
				}
				else {
					gotoxy(53, 18);
					textcolor(RED);
					cout << "FECHADO";
					textcolor(WHITE);

				}
				break;

			}

		}

		textcolor(CYAN);
		cout << "\n\n\t Prima ENTER para continuar" << endl;
		textcolor(WHITE);
		int opcao;
		tecla = opcao_valida(opcao, 1, 1);
	} while (tecla != ENTER);
}

unsigned int definir_horas(Piscina &p1, vector<string> &dias)
{
	string hora_i, hora_f;
	int horas{}, minutos{};
	char car{};
	bool avancar{ false };
	int a = 254;
	char square = a; // obter  █

	cin.ignore(256, '\n');

	for (size_t i{}; i < dias.size(); i++) {
		limparEcra();
		cabecalho();
		cout << endl << endl;

		///hora de inicio
		do {
			textcolor(RED);
			cout << "\n\n\t\t " << dias[i] << ": " << endl;
			textcolor(LIGHT_GRAY);
			cout << "\t\tHora de abertura (hh:mm): ";
			textcolor(WHITE);
			getline(cin, hora_i);
			stringstream ss1(hora_i);
			ss1 >> horas >> car >> minutos;

			if (hora_i.size() == 4)
				hora_i = "0" + hora_i;

			if ((horas < 0) || (horas > 24) || ((horas == 24) && (minutos > 0)) || (minutos < 0) || (minutos >= 60) || (car != ':'))
			{
				avancar = false;
				textcolor(RED);
				cout << "\t\t"<< square <<"hora invalida";
				textcolor(WHITE);
			}
			else {
				avancar = true;
			}


		} while (!avancar);

		avancar = false;
		horas = {};
		minutos = {};

		// hora de encerramento

		do {
			textcolor(LIGHT_GRAY);
			cout << "\t\tHora de encerramento (hh:mm): ";
			textcolor(WHITE);
			getline(cin, hora_f);
			stringstream ss2(hora_f);
			ss2 >> horas >> car >> minutos;

			if (hora_f.size() == 4)
				hora_f = "0" + hora_f;

			if ((horas < 0) || (horas > 24) || ((horas == 24) && (minutos > 0)) || (minutos < 0) || (minutos >= 60) || (hora_f < hora_i) || (car != ':'))
			{
				avancar = false;
				textcolor(RED);
				cout << "\t\t" << square << "hora invalida";
				textcolor(WHITE);
			}
			else {
				avancar = true;
			}


		} while (!avancar);

		string h;
		h = hora_i + " - " + hora_f;
		p1.horariomap[dias[i]] = h;

	}
	mostrar_calendario(p1);

	return 1;
}

void editar_dias_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a; // obter  █

	int y = 0;
	int y1 = 0;

	if (opcao_b <= 7)
		y = 6 + opcao_b;
	else y = 7 + opcao_b;

	if (opcao <= 7)
		y1 = 6 + opcao;
	else y1 = 7 + opcao;

	gotoxy(25, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(25, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);

	gotoxy(0, 21);


}

void editar_dias(Piscina &p1, vector<string>&dias)
{
	int opcao = 1, opcao_b = 1, tecla;
	vector<string> temp = dias;
	bool imprimir = true;

	do {
		if (imprimir)
		{
		limparEcra();
		cabecalho();

		textcolor(LIGHT_GRAY);
		cout << "\n\n\t\t Atualmente: ";
		textcolor(WHITE);
		if (temp.size() == 0)
		{
			textcolor(RED);
			cout << "sem dias de funcionamento ";
			textcolor(WHITE);

		}
		else
		{
			temp = ordena_dias_semana(temp);
			cout << temp[0];
			for (size_t i{ 1 }; i < temp.size(); i++) {
				cout << ", " << temp[i];
			}
		}

		cout << "\n\n\t\t\t  ";
		if (encontra_string_vetor("segunda", temp) != -1)
			cout << " Retirar Segunda" << endl;
		else
			cout << " Adicionar Segunda" << endl;

		cout << "\t\t\t  ";
		if (encontra_string_vetor("terca", temp) != -1)
			cout << " Retirar Terca" << endl;
		else
			cout << " Adicionar Terca" << endl;

		cout << "\t\t\t  ";
		if (encontra_string_vetor("quarta", temp) != -1)
			cout << " Retirar Quarta" << endl;
		else
			cout << " Adicionar Quarta" << endl;

		cout << "\t\t\t  ";
		if (encontra_string_vetor("quinta", temp) != -1)
			cout << " Retirar Quinta" << endl;
		else
			cout << " Adicionar Quinta" << endl;

		cout << "\t\t\t  ";
		if (encontra_string_vetor("sexta", temp) != -1)
			cout << " Retirar Sexta" << endl;
		else
			cout << " Adicionar Sexta" << endl;

		cout << "\t\t\t  ";
		if (encontra_string_vetor("sabado", temp) != -1)
			cout << " Retirar Sabado" << endl;
		else
			cout << " Adicionar Sabado" << endl;

		cout << "\t\t\t  ";
		if (encontra_string_vetor("domingo", temp) != -1)
			cout << " Retirar Domingo" << endl << endl;
		else
			cout << " Adicionar Domingo" << endl << endl;

		cout << "\t\t\t   Concluir" << endl;
		cout << "\t\t\t   Cancelar" << endl;
	}
		imprimir = false;

		editar_dias_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 9);
		Sleep(100);

		if (tecla == ENTER)
		{
			switch (opcao) {

			case 1:
				if (encontra_string_vetor("segunda", temp) != -1)
					temp.erase(temp.begin() + encontra_string_vetor("segunda", temp));
				else
					temp.push_back("segunda");

				imprimir = true;
				break;

			case 2:
				if (encontra_string_vetor("terca", temp) != -1)
					temp.erase(temp.begin() + encontra_string_vetor("terca", temp));
				else
					temp.push_back("terca");
				imprimir = true;
				break;

			case 3:
				if (encontra_string_vetor("quarta", temp) != -1)
					temp.erase(temp.begin() + encontra_string_vetor("quarta", temp));
				else
					temp.push_back("quarta");
				imprimir = true;
				break;

			case 4:
				if (encontra_string_vetor("quinta", temp) != -1)
					temp.erase(temp.begin() + encontra_string_vetor("quinta", temp));
				else
					temp.push_back("quinta");
				imprimir = true;
				break;

			case 5:
				if (encontra_string_vetor("sexta", temp) != -1)
					temp.erase(temp.begin() + encontra_string_vetor("sexta", temp));
				else
					temp.push_back("sexta");
				imprimir = true;
				break;

			case 6:
				if (encontra_string_vetor("sabado", temp) != -1)
					temp.erase(temp.begin() + encontra_string_vetor("sabado", temp));
				else
					temp.push_back("sabado");
				imprimir = true;
				break;

			case 7:
				if (encontra_string_vetor("domingo", temp) != -1)
					temp.erase(temp.begin() + encontra_string_vetor("domingo", temp));
				else
					temp.push_back("domingo");
				imprimir = true;
				break;

			case 8:
				dias = temp; // altera os dias
				break;

			case 9:
				break;

			}
		}


	} while (((opcao != 8) && (opcao != 9)) || (tecla != ENTER));

}

void definir_dias_funcionamento_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a; // obter  █

	int y = 0;
	int y1 = 0;

	if (opcao_b <= 3)
		y = 8 + opcao_b;
	else y = 9 + opcao_b;

	if (opcao <= 3)
		y1 = 8 + opcao;
	else y1 = 9 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);

	gotoxy(0, 21);

	return;

}

int definir_dias_funcionamento(Piscina &p1)
{
	unsigned int opcao1 = 0;
	int opcao = 1, opcao_b = 1, tecla;
	vector<string> dias;
	bool imprimir = true;
	
	do {
		tecla = 0;

		if (imprimir)
		{
			limparEcra();
			cabecalho();
			textcolor(LIGHT_GRAY);
			cout << "\n\n\t\t Atualmente: ";
			textcolor(WHITE);
			if (dias.size() == 0)
			{
				textcolor(RED);
				cout << "sem dias de funcionamento ";
				textcolor(WHITE);
			}
			else
			{
				dias = ordena_dias_semana(dias);
				cout << dias[0];
				for (size_t i{ 1 }; i < dias.size(); i++) {
					cout << ", " << dias[i];
				}
			}

			textcolor(LIGHT_GRAY);
			cout << "\n\n\t\t Dias de funcionamento:" << endl << endl;
			textcolor(WHITE);
			cout << "\t\t\t\t    Todos os dias" << endl;
			cout << "\t\t\t\t    Dias uteis" << endl;
			cout << "\t\t\t\t    Editar" << endl << endl;
			cout << "\t\t\t\t    Avancar" << endl;
			cout << "\t\t\t\t    Voltar atras" << endl << endl;
		}
		imprimir = false;

		definir_dias_funcionamento_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 5);

		if (tecla == ENTER)
		{
			switch (opcao) {

			case 1:
				dias = { "segunda", "terca", "quarta", "quinta", "sexta", "sabado", "domingo" };
				imprimir = true;
				break;

			case 2:
				dias = { "segunda", "terca", "quarta", "quinta", "sexta" };
				imprimir = true;
				break;

			case 3:
				editar_dias(p1, dias);
				imprimir = true;
				break;

			case 4:
				if (dias.size() == 0)
				{
					cout << "\n Defina os dias de funcionamento antes de avancar! 'Enter' para continuar";
					getchar();
					imprimir = true;
				}
				else {
					opcao1 = definir_horas(p1, dias);
					opcao = 5;

				}
				break;

			case 5:
				break;

			}
		}

	} while ((opcao != 5) || (tecla != ENTER));

	if (opcao1 == 0)
	{
		cout << "\nO horario continua por definir! 'Enter' para continuar" << endl;
		cin.ignore();
		getchar();
		return 0;
	}
	return 1;

}


//3.2 Definir lotacao
void lotacao_piscina(Piscina &p1)
{
	unsigned int lotacao;
	bool fim = false;
	int a = 254;
	char square = a; // obter  █

	if (p1.getLotacao() != NULL)
	{
		limparEcra();
		cabecalho();
		cout << endl;
		textcolor(LIGHT_GRAY);
		cout << "\t\t Lotacao atual: ";
		textcolor(WHITE);
		cout << p1.getLotacao() << endl << endl;
		
		cin.ignore();
		string opcao;
		do {
			textcolor(LIGHT_GRAY);
			cout << "\n\t\t Deseja aterar? ";
			textcolor(WHITE);
			getline(cin, opcao);

		} while ((opcao != "nao") && (opcao != "n") && (opcao != "sim") && (opcao != "s"));


		if ((opcao == "nao") || (opcao == "n")) /// se nao deseja guardar
			return;
	}

	limparEcra();
	cabecalho();

	do
	{
		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Lotacao: ";
		textcolor(WHITE);
		cin >> lotacao;

		if (cin.fail())
		{
			textcolor(RED);
			cout << "\t\t " << square << "Numero invalido \n\n ";
			textcolor(WHITE);
			cin.clear();
			cin.ignore(256, '\n');
		}
		else fim = true;

	} while (!fim);

	textcolor(LIGHT_GREEN);
	cout << "\t\t " << square << " aceite"<< endl;
	textcolor(WHITE);
	Sleep(500);

	p1.setLotacao(lotacao);

	return;

}


//3.1 Definir nome
void nome_piscina(Piscina &p1)
{
	string nome;
	bool fim = false;
	int a = 254;
	char square = a; // obter  █

	if (p1.getNome() != "")
	{
		limparEcra();
		cabecalho();
		textcolor(LIGHT_GRAY);
		cout << "\t\t Nome atual: ";
		textcolor(WHITE);
		cout << p1.getNome() << endl << endl;
		string opcao;

		//cin.ignore(256, '\n');
		do {
			textcolor(LIGHT_GRAY);
			cout << "\n\t\t Deseja aterar? ";
			textcolor(WHITE);
			getline(cin, opcao);

		} while ((opcao != "nao") && (opcao != "n") && (opcao != "sim") && (opcao != "s"));


		if ((opcao == "nao") || (opcao == "n")) /// se nao deseja guardar
			return;
	}

	limparEcra();
	cabecalho();
	do {
		textcolor(LIGHT_GRAY);
		cout << "\n\t\t Nome da piscina: ";
		textcolor(WHITE);
		getline(cin, nome);

		if (nome != "")
			fim = true;
		else {
			textcolor(RED);
			cout << "\t\t " << square << " Campo obrigatorio!" << endl;
			textcolor(WHITE);
		}


	} while (!fim);
	
	textcolor(LIGHT_GREEN);
	cout << "\t\t " << square << " aceite" << endl;
	textcolor(WHITE);
	Sleep(500);

	p1.setNome(nome);

	return;


}


//3. CRIAR PISCINA
void menu_criar_piscina_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a; // obter  █

	int y = 0;
	int y1 = 0;

	if (opcao_b <= 5)
		 y = 9 + opcao_b;
	else y = 10 + opcao_b;
	
	if(opcao <=5)
		y1 = 9 + opcao;
	else y1 = 10 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);

	gotoxy(0, 21);

}

void menu_criar_piscina()
{
	unsigned int result = 0;
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;
	string fichPiscina, fichUtentes, fichAulas, fichProfessores;
	Piscina p1;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();
			cout << endl << endl;
			cout << "\t\t\t\t    Definir nome" << endl;
			cout << "\t\t\t\t    Definir lotacao" << endl;
			cout << "\t\t\t\t    Definir horario" << endl;
			cout << "\t\t\t\t    Definir localizacao" << endl;
			cout << "\t\t\t\t    Definir base de dados" << endl << endl;
			cout << "\t\t\t\t    Concluir e avancar" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;

		}

		imprimir = false;

		menu_criar_piscina_ops(opcao, opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 7);
		Sleep(100);


		if (tecla == ENTER)
			switch (opcao)
			{
			case 1:
				nome_piscina(p1);
				imprimir = true;
				break;

			case 2:
				lotacao_piscina(p1);
				imprimir = true;
				break;

			case 3:
				if (result == 0)
					result  = definir_dias_funcionamento(p1);
				else mostrar_calendario(p1);
				imprimir = true;
				break;

			case 4:
				break;

			case 5:
				definir_base_de_dados(fichPiscina, fichUtentes, fichAulas, fichProfessores);
				imprimir = true;
				break;

			case 6:
				if (check(p1, fichPiscina, fichProfessores))
					menu_geral(p1, fichPiscina, fichUtentes, fichAulas, fichProfessores);
				imprimir = true;
				break;

			case 7:
				return;
				break;
			}

	} while ((opcao != 7) || (tecla != 13));

	cout << endl << endl;


}


//2. CREDITOS
void creditos()
{
	system("cls");
	titulo();
	cout << endl;
	cout << endl << "\t    Projeto final da cadeira 'Algoritmos e Estruturas de Dados' - AEDA " << endl << endl;
	cout << "\t    Desenvolvido por:" << endl << endl;

	cout << "\t\t\t David Falcao    -  up201506571";
	textcolor(YELLOW);
	cout << " @ ";
	textcolor(WHITE);
	cout <<"fe.up.pt" << endl;

	cout << "\t\t\t Jose Machado    -  up201504779";
	textcolor(YELLOW);
	cout << " @ ";
	textcolor(WHITE);
	cout << "fe.up.pt" << endl;

	cout << "\t\t\t Pedro Miranda   -  up201506574";
	textcolor(YELLOW);
	cout << " @ ";
	textcolor(WHITE);
	cout << "fe.up.pt" << endl;


	textcolor(YELLOW);
	cout << endl << endl << "\t    Prima ENTER para voltar" << endl;
	textcolor(WHITE);
	
	cin.ignore();
}


// 1.MENU INICIAL
void menu_inicial_ops(int opcao, int opcao_b)
{
	int a = 254;
	char square = a;
	
	int y = 9 + opcao_b;
	int y1 = 9 + opcao;

	gotoxy(34, y);
	textcolor(YELLOW);
	cout << " ";
	textcolor(WHITE);

	gotoxy(34, y1);
	textcolor(YELLOW);
	cout << square;
	textcolor(WHITE);

	

	gotoxy(0, 21);
}

void menu_inicial()
{
	int opcao = 1, opcao_b = 1, tecla;
	bool imprimir = true;

	do
	{
		if (imprimir)
		{
			limparEcra();
			titulo();
			cout << endl << endl;
			cout << "\t\t\t\t    Criar Piscina" << endl;
			cout << "\t\t\t\t    Carregar Piscina" << endl;
			cout << "\t\t\t\t    Sobre" << endl;
			cout << "\t\t\t\t    Sair" << endl;
			cout << endl << endl << endl << endl;
			cout << "\t\t\t\t\t\t\t\t" << mostrar_mes(mes_do_sistema()) << " " << ano_do_sistema();
			cout << endl << endl;
		
		}

		imprimir = false;

		menu_inicial_ops(opcao,opcao_b);
		opcao_b = opcao;
		tecla = opcao_valida(opcao, 1, 4);
		Sleep(100);


		if (tecla == ENTER)
		switch (opcao)
		{
		case 1:
			menu_criar_piscina();
			imprimir = true;
			break;

		case 2:
			escolher_ficheiros();
			imprimir = true;
			break;

		case 3:
			creditos();
			imprimir = true;
			break;

		case 4:
			return;
			break;

		}
	
	} while ((opcao != 4) || (tecla != 13));

	cout << endl << endl;
}

int main()
{
	configurar_terminal();
	menu_inicial();

	return 0;
}
