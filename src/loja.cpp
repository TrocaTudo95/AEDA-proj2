#include "loja.h"

using namespace std;

//Loja
BST<Produto> Loja::getProdutos()
{
	return produtos;
}

//confirmar se esta correto
void Loja::vende(Produto &prod)
{
	BSTItrIn<Produto> it(produtos);
	bool existe = false;
	while (!it.isAtEnd())
	{

		if(prod == it.retrieve())
		{
				if(prod.getStock > it.retrieve().getStock)
				{
					cout << "Nao ha itens suficientes em stock para concluir a sua compra" << endl;
					existe = true;
				}
				else
				{
					unsigned int st = it.retrieve().getStock();
					st = st - prod.getStock();
					string name = it.retrieve().getNome();
					string tm = it.retrieve().getTamanho();
					Produto px(name, tm, st);
					produtos.remove(it.retrieve());
					produtos.insert(px);
					existe = true;

				}
					
		}

		it.advance();

	
	}

	if(! existe)
	{
		cout << "o produto nao existe" << endl;
	
	}


}

//Produto

Produto::Produto(string &nome,string &tamanho,unsigned int &stock)
{
	this->nome = nome;
	this->tamanho = tamanho;
	this->stock = stock;
}

string Produto::getNome()
{
	return nome;
}
string Produto::getTamanho()
{
	return tamanho;
}
unsigned int Produto::getStock()
{
	return stock;
}
void Produto::setNome(string &nome)
{
	this->nome = nome;
}
void Produto::setTamanho(string &tamanho)
{
	this->tamanho = tamanho;
}
void Produto::setStock(unsigned int &stock)
{
	this->stock = stock;
}

bool Produto::operator==(const Produto &p) const
{
	if (p.getNome == nome && p.getTamanho == tamanho)
		return true;
	else
		return false;


}
