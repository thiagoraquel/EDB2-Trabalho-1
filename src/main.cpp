#include <cstdlib>
#include <iostream>
#include <string>

#include "dados.h"

void printar_ajuda() {
  std::cout << "Digite um numero de 1 a 6 para indicar qual operacao deseja\n";
  std::cout << "1 - Inserir animal, 2 - Remover animal, 3 - Consultar id, 4 - "
               "Registrar novo monitoramento, 5 - Salvar arquivo, 6 - Imprimir "
               "todos os dados, 7 - Encerrar o programa\n";
}

void ignorar_caracteres_vazios() {
  std::string lixo;
  std::getline(std::cin, lixo);
}

int ler_operacao() {
  std::string entrada;
  std::getline(std::cin, entrada);
  try {
    return std::stoi(entrada);
  } catch (const std::exception &) {
    std::cout << "Operacao invalida\n";
    return -1;
  }
}

void leia_id_do_animal(std::string &id) {
  std::cout << "Digite o id do animal: ";
  std::cin >> id;
  ignorar_caracteres_vazios();
}

int main(int argc, char *argv[]) {
  std::string arquivo_de_entrada;
  if (argc > 1) {
    arquivo_de_entrada = argv[1];
  } else {
    arquivo_de_entrada = "fauna.txt";
  }
  Dados dados(arquivo_de_entrada);

  printar_ajuda();
  while (true) {
    std::cout << "Operação: ";
    int operacao = ler_operacao();
    std::string id;
    if (operacao == 1) {
      leia_id_do_animal(id);
      if (dados.id_valido(id)) {
        std::cout << "Já existe um animal com esse id.\n";
        continue;
      }
      Dados::DadosDoAnimal dados_do_animal;
      dados_do_animal.leia_valores();
      dados.inserir_animal(id, dados_do_animal);
    } else if (operacao == 2) {
      leia_id_do_animal(id);
      if (!dados.id_valido(id)) {
        std::cout << "não existe nenhum animal com esse id.\n";
        continue;
      }
      dados.remover_animal(id);
    } else if (operacao == 3) {
      leia_id_do_animal(id);
      if (!dados.id_valido(id)) {
        std::cout << "não existe nenhum animal com esse id.\n";
        continue;
      }
      dados.consultar_fauna(id).printar_valores();
    } else if (operacao == 4) {
      leia_id_do_animal(id);
      if (!dados.id_valido(id)) {
        std::cout << "não existe nenhum animal com esse id.\n";
        continue;
      }
      Dados::DadosDeMonitoramento dados_de_monitoramento;
      dados_de_monitoramento.leia_valores();
      dados.inserir_monitoramento_do_animal(id, dados_de_monitoramento);
    } else if (operacao == 5) {
      dados.salvar_dados();
    } else if (operacao == 6) {
      dados.imprima_todos_os_dados();
    } else if (operacao == 7) {
      return 0;
    } else {
      printar_ajuda();
    }
  }
}
