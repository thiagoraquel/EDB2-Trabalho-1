#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "avl.h"

const static int NumeroDeDadosDeMonitoramento = 6;
const static std::string
    ordem_dos_dados_de_monitoramento[NumeroDeDadosDeMonitoramento] = {
        "Data da avaliacao",   "Teperatura",  "Peso", "Altura",
        "Sangue foi coletado", "Exame fisico"};

const static int NumeroDeDadosDoAnimal = 5;
const static std::string ordem_dos_dados_do_animal[NumeroDeDadosDoAnimal] = {
    "Apelido",
    "Primeiro dia de monitoramento",
    "Espécie",
    "Sexo",
    "Data de nascimento",
};

/**
 * Class that contains
*/
class Dados {
public:
  /**
   * IdType = string
  */
  using IdType = std::string;

  /**
   * Dados do monitoramento do animal
  */
  struct DadosDeMonitoramento {
    std::unordered_map<std::string, std::string> dados; // Armazena pares de chave-valor onde tanto a chave como o valor são strings

    /**
     * Leia os valores que o usuário der para cada dado de monitoramento
    */
    void leia_valores() {
      for (const std::string &dado : ordem_dos_dados_de_monitoramento) {
        std::cout << dado << ": ";
        std::string entrada;
        std::getline(std::cin, entrada);
        dados[dado] = entrada;
      }
    }

    /**
     * Printe os valores dos dados de monitoramento
    */
    void printar_valores() {
      for (const std::string &dado : ordem_dos_dados_de_monitoramento) {
        std::cout << "\t" << dado << ": " << dados[dado] << '\n';
      }
    }
  };

  /**
   * Dados do animal e do monitoramento do animal
  */
  struct DadosDoAnimal {
    std::unordered_map<std::string, std::string> dados; // Armazena pares de chave-valor onde tanto a chave como o valor são strings
    std::vector<DadosDeMonitoramento> monitoramento;  // Vetor de dados de monitorametno

    /**
     * Leia os valores que o usuário der para cada dado do animal
    */
    void leia_valores() {
      for (const std::string &dado : ordem_dos_dados_do_animal) {
        std::cout << dado << ": ";
        std::string entrada;
        std::getline(std::cin, entrada);
        dados[dado] = entrada;
      }
    }

    /**
     * Printar dados do animal e do monitoramento
    */
    void printar_valores() {
      for (const std::string &dado : ordem_dos_dados_do_animal) {
        std::cout << dado << ": " << dados[dado] << '\n';
      }
      for (int index = 0; index < monitoramento.size(); ++index) {
        std::cout << "dados do monitoramento " << index + 1 << ":\n";
        monitoramento[index].printar_valores();
      }
    }
  };

  /**
   * Constructor
  */
  Dados(const std::string &nome_do_arquivo) {

    // Atualize m_nome_do_arquivo
    m_nome_do_arquivo = nome_do_arquivo;

    // Abre o arquivo cujo nome é armazenado em m_nome_do_arquivo para leitura
    std::ifstream arquivo(m_nome_do_arquivo);

    // Verifica se arquivo vazio ou se atingiu o final (eof)
    if (!arquivo or arquivo.eof()) {
      return;
    }

    std::string line;
    getline(arquivo, line); // ignora a primeira linha 
    // Essa linha: " Apelido | Primeiro dia de monitoramento | Espécie | Sexo | Data de nascimento | "

    // Enquanto arquivo não chegou no final e há linhas para ler
    while (!arquivo.eof() && std::getline(arquivo, line)) {
      std::stringstream ss(line); // Passe line para ss
      std::string token;  // Tokens da linha

      IdType id;
      getline(ss, id, '|'); // Lê os dados em ss até encontrar '|' e passa para id

      DadosDoAnimal animal_data;
      // Repita 5 vezes (numero_dados_animal)

      // Comentar resto (TODO)

      for (int index = 0; index < NumeroDeDadosDoAnimal; ++index) { 
        getline(ss, token, '|');
        animal_data.dados[ordem_dos_dados_do_animal[index]] = token;
      }

      getline(ss, token);
      int numero_de_monitoramentos = std::stoi(token);

      for (int counter = 0; counter < numero_de_monitoramentos; ++counter) {
        getline(arquivo, line);
        std::stringstream ss2(line);
        DadosDeMonitoramento dados_de_monitoramento;
        for (int index = 0; index < NumeroDeDadosDeMonitoramento - 1; ++index) {
          getline(ss2, token, '|');
          dados_de_monitoramento
              .dados[ordem_dos_dados_de_monitoramento[index]] = token;
        }
        getline(ss2, token);
        dados_de_monitoramento.dados[ordem_dos_dados_de_monitoramento
                                         [NumeroDeDadosDeMonitoramento - 1]] =
            token;
        animal_data.monitoramento.push_back(dados_de_monitoramento);
      }
      m_dados.insert({id, animal_data});
    }
  }

  ~Dados() { salvar_dados(); }  // Deconstrutor

  /**
   * Inserir animal na árvore AVL
  */
  void inserir_animal(const IdType &id, const DadosDoAnimal &dados_do_animal) {
    m_dados.insert({id, dados_do_animal});
  }

  void remover_animal(const IdType &id) { m_dados.erase(id); }

  DadosDoAnimal consultar_fauna(const IdType &id) {
    return m_dados.find(id)->second;
  }

  void inserir_monitoramento_do_animal(
      const IdType &id, const DadosDeMonitoramento &dados_de_monitoramento) {
    m_dados.find(id)->second.monitoramento.push_back(dados_de_monitoramento);
  }

  void salvar_dados() {
    std::ofstream arquivo(m_nome_do_arquivo);
    for (const std::string &dado : ordem_dos_dados_do_animal) {
      arquivo << dado << " | ";
    }
    arquivo << "\n";

    for (auto it = m_dados.begin(); it != m_dados.end(); ++it) {
      arquivo << it->first << '|';
      for (const std::string &dados_do_animal : ordem_dos_dados_do_animal) {
        arquivo << it->second.dados[dados_do_animal] << '|';
      }
      arquivo << it->second.monitoramento.size() << "\n";
      for (DadosDeMonitoramento &monitoramento : it->second.monitoramento) {
        for (const std::string &dados_de_monitoramento :
             ordem_dos_dados_de_monitoramento) {
          arquivo << monitoramento.dados[dados_de_monitoramento];
          if (dados_de_monitoramento !=
              ordem_dos_dados_de_monitoramento[NumeroDeDadosDeMonitoramento -
                                               1]) {
            arquivo << '|';
          }
        }
        arquivo << "\n";
      }
    }
    auto it = m_dados.end();
    if (it != nullptr) {

      arquivo << it->first << '|';
      for (const std::string &dados_do_animal : ordem_dos_dados_do_animal) {
        arquivo << it->second.dados[dados_do_animal] << '|';
      }
      arquivo << it->second.monitoramento.size() << "\n";
      for (DadosDeMonitoramento &monitoramento : it->second.monitoramento) {
        for (const std::string &dados_de_monitoramento :
             ordem_dos_dados_de_monitoramento) {
          arquivo << monitoramento.dados[dados_de_monitoramento];
          if (dados_de_monitoramento !=
              ordem_dos_dados_de_monitoramento[NumeroDeDadosDeMonitoramento -
                                               1]) {
            arquivo << '|';
          }
        }
        arquivo << "\n";
      }
    }
  }

  /**
   * Verifica se id é válido
  */
  bool id_valido(const IdType &id) {
    if (m_dados.find(id) == nullptr) {
      return false;
    }
    return true;
  }

  void imprima_todos_os_dados() {
    for (auto it = m_dados.begin(); it != m_dados.end(); ++it) {
      std::cout << "id: " << it->first << "\n";
      it->second.printar_valores();
    }
    auto it = m_dados.end();
    if (it != nullptr) {
      std::cout << "id: " << it->first << "\n";
      it->second.printar_valores();
    }
  }

private:
  AVL<IdType, DadosDoAnimal> m_dados;
  /**
   * Name of the archive
  */
  std::string m_nome_do_arquivo;
};
