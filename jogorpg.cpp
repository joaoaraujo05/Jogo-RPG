// Jogo RPG
// Alunos: Barbara Figueiredo (841308), João Pedro Leme Araújo(842123), Pedro Augusto Duarte Moreira (842580), Welton Wagner Dolce Junior (842698).
// Professor: Thiago Nicola Cajuela Garcia
// Matéria: Laboratório de Técnicas de Programação

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

// Criando struct para personagem, cenario e vilao
struct Personagem {
    string nome;
    string classe;
    int vida;
    int dano;
};

struct Cenario {
    string nome;
    string descricao;
    string dificuldade;
};

struct Vilao {
    string nomeVilao;
    int vidaVilao;
    int forcaVilao;
};

struct EstadoDoJogo
{
    vector<Personagem> personagens;
    vector<Cenario> cenarios;
    vector<Vilao> viloes;
    int cenarioAtual;
    bool jogoFinalizado;
};

// Função para salvar o estado do jogo
void salvarJogo(const EstadoDoJogo &estadoDoJogo)
{
    cout << "Salvando Progresso" << endl;

    ofstream arquivo("estado_do_jogo.txt");
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para salvar o jogo!" << endl;
        return;
    }

    arquivo << "CenarioAtual: " << estadoDoJogo.cenarioAtual << endl;
    arquivo << "JogoFinalizado: " << (estadoDoJogo.jogoFinalizado ? "true" : "false") << endl;

    arquivo << "Personagens:" << endl;
    for (const auto &personagem : estadoDoJogo.personagens) {
        arquivo << personagem.nome << ","
                << personagem.classe << ","
                << personagem.vida << ","
                << personagem.dano << endl;
    }

    arquivo << "Cenarios:" << endl;
    for (const auto &cenario : estadoDoJogo.cenarios) {
        arquivo << cenario.nome << ","
                << cenario.descricao << ","
                << cenario.dificuldade << endl;
    }

    arquivo << "Viloes:" << endl;
    for (const auto &vilao : estadoDoJogo.viloes) {
        arquivo << vilao.nomeVilao << ","
                << vilao.vidaVilao << ","
                << vilao.forcaVilao << endl;
    }

    arquivo.close();
    cout << "Progresso salvo com sucesso." << endl;
}

// Função para carregar o estado do jogo
bool carregarJogo(EstadoDoJogo &estadoDoJogo)
{
    cout << "Carregando Progresso" << endl;

    ifstream arquivo("estado_do_jogo.txt");
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para carregar o jogo!" << endl;
        return false;
    }

    estadoDoJogo.personagens.clear();
    estadoDoJogo.cenarios.clear();
    estadoDoJogo.viloes.clear();

    string linha;

    // Lê CenarioAtual e JogoFinalizado
    getline(arquivo, linha);
    estadoDoJogo.cenarioAtual = stoi(linha.substr(linha.find(":") + 1));

    getline(arquivo, linha);
    estadoDoJogo.jogoFinalizado = (linha.substr(linha.find(":") + 1) == "true");

    // Lê Personagens
    getline(arquivo, linha); // "Personagens:"
    while (getline(arquivo, linha) && !linha.empty() && linha != "Cenarios:") {
        Personagem p;
        size_t pos = 0;
        pos = linha.find(",");
        p.nome = linha.substr(0, pos);
        linha = linha.substr(pos + 1);

        pos = linha.find(",");
        p.classe = linha.substr(0, pos);
        linha = linha.substr(pos + 1);

        pos = linha.find(",");
        p.vida = stoi(linha.substr(0, pos));
        linha = linha.substr(pos + 1);

        p.dano = stoi(linha);

        estadoDoJogo.personagens.push_back(p);
    }

    // Lê Cenarios
    while (getline(arquivo, linha) && !linha.empty() && linha != "Viloes:") {
        Cenario c;
        size_t pos = 0;
        pos = linha.find(",");
        c.nome = linha.substr(0, pos);
        linha = linha.substr(pos + 1);

        pos = linha.find(",");
        c.descricao = linha.substr(0, pos);
        linha = linha.substr(pos + 1);

        c.dificuldade = linha;

        estadoDoJogo.cenarios.push_back(c);
    }

    // Lê Viloes
    while (getline(arquivo, linha) && !linha.empty()) {
        Vilao v;
        size_t pos = 0;
        pos = linha.find(",");
        v.nomeVilao = linha.substr(0, pos);
        linha = linha.substr(pos + 1);

        pos = linha.find(",");
        v.vidaVilao = stoi(linha.substr(0, pos));
        linha = linha.substr(pos + 1);

        v.forcaVilao = stoi(linha);

        estadoDoJogo.viloes.push_back(v);
    }

    arquivo.close();
    cout << "Jogo carregado com sucesso!" << endl;
    return true;
}

// Funcao para verificar se todos os personagens foram derrotados
bool todosPersonagensDerrotados(const vector<Personagem>& personagens) {
    for (int i = 0; i < 5; i++) {
        if (personagens[i].vida > 0) {
            return false;
        }
    }
    return true;
}

// Funcao para batalha, recendo passagem por referencia
void batalhar(Vilao& vilao, vector<Personagem>& personagens, bool& jogoFinalizado, EstadoDoJogo& estadoDoJogo) {
    int turno = 0;
    int numeroDePersonagens = 5;
    int escolhaPersonagem;

    //Introdução do vilão antes da batalha começar
    cout << "\n" << vilao.nomeVilao << " diz: ";
    if (vilao.nomeVilao == "Guardiao das Sombras") {
        cout << "\"Eu fui traido, e agora voces pagarao com suas vidas!\"";
    } else if (vilao.nomeVilao == "Lider dos Trolls") {
        cout << "\"Os reinos serao meus! Ninguem pode me deter!\"";
    }
    else if (vilao.nomeVilao == "O Caos"){
        cout << "\"Eu trarei a verdadeira liberdade! O fim de toda ordem esta proximo!\"";
    }
    cout << endl;

    while (vilao.vidaVilao > 0) {
        // Exibindo as informacoes de vida do vilao
        cout << "\n============================================\n";
        cout << "VILAO\t\t\tVIDA" << endl;
        cout << vilao.nomeVilao << "\t" << vilao.vidaVilao << endl;
        cout << "\n============================================\n";
        
        // Escolher personagem para o turno
        cout << "\nEscolha o personagem para atacar: \n";
        for (int i = 0; i < numeroDePersonagens; i++) {
            if (personagens[i].vida > 0) {
                cout << i + 1 << ". " << personagens[i].nome << " (" << personagens[i].classe << ") - Vida: " << personagens[i].vida << " - Dano: " << personagens[i].dano << endl;
            }
        }
        cout << "\nEscolha seu personagem: ";
        cin >> escolhaPersonagem;

        // Validacao da escolha
        if (escolhaPersonagem < 1 || escolhaPersonagem > numeroDePersonagens || personagens[escolhaPersonagem - 1].vida <= 0) {
            cout << "\nOpcao invalida ou personagem derrotado. Tente novamente.\n";
            continue;
        }

        // Escolhendo o indice correto correspondente ao personagem por referencia
        // Exemplo: Kara é a personagem 3, mas no vetor é o indice [2]
        Personagem& personagemEscolhido = personagens[escolhaPersonagem - 1];

        // Ataque do personagem ao vilao
        cout << "\n" << personagemEscolhido.nome << " ataca " << vilao.nomeVilao << " causando " << personagemEscolhido.dano << " de dano!\n";
        vilao.vidaVilao -= personagemEscolhido.dano;

        // Se o vilao ainda estiver vivo, ele contra-ataca
        if (vilao.vidaVilao > 0) {
            cout << vilao.nomeVilao << " contra-ataca, causando " << vilao.forcaVilao << " de dano!\n";
            personagemEscolhido.vida -= vilao.forcaVilao;

            // Frase entre ataques dos vilões
            cout << "\n" << vilao.nomeVilao << " diz: ";
            if (vilao.nomeVilao == "Guardiao das Sombras") {
                cout << (rand() % 2 == 0 ? "\"Voces vao se perder para sempre, assim como todos os que me desafiaram!\"\n" :
                                           "\"Sombras eternas irao devorar todos voces!\"\n");
            } else if (vilao.nomeVilao == "Lider dos Trolls") {
                cout << (rand() % 2 == 0 ? "\"Esse ataque foi patetico! Eu sou indestrutivel!\"\n" :
                                           "\"Eu pisotearei todos que se atreverem a me enfrentar!\"\n");
            } else if (vilao.nomeVilao == "O Caos") {
                cout << (rand() % 2 == 0 ? "\"Nada pode conter o Caos! Em breve, tudo sera reduzido ao nada!\"\n" :
                                           "\"A ordem sera destruida, e o caos reinara para sempre!\"\n");
            }
            cout << endl;

            // Dentro da função batalhar, logo após o vilão atacar o personagem escolhido
            if (personagemEscolhido.vida > 0) {
                cout << personagemEscolhido.nome << " diz: ";

                if (personagemEscolhido.nome == "Thorin") {
                    cout << (rand() % 3 == 0 ? "\"Isso e tudo que voce tem? Eu sou mais forte do que isso!\"\n" :
                                               (rand() % 2 == 0 ? "\"Vai ser preciso mais do que isso para me derrubar!\"\n" :
                                                                  "\"Eu ja lutei contra piores do que voce!\"\n"));
                } else if (personagemEscolhido.nome == "Eldra") {
                    cout << (rand() % 3 == 0 ? "\"Voce ousa desafiar o poder da magia?\"\n" :
                                               (rand() % 2 == 0 ? "\"Minhas defesas ainda estao intactas. Prepare-se!\"\n" :
                                                                  "\"Essa dor so fortalece minha determinacao!\"\n"));
                } else if (personagemEscolhido.nome == "Kara") {
                    cout << (rand() % 3 == 0 ? "\"Ah, voce vai pagar caro por esse ataque!\"\n" :
                                               (rand() % 2 == 0 ? "\"Mal me acertou! Vai ter que ser mais rapido!\"\n" :
                                                                  "\"Voce nao tem chance contra minhas habilidades!\"\n"));
                } else if (personagemEscolhido.nome == "Finn") {
                    cout << (rand() % 3 == 0 ? "\"Nao pense que vou perder o foco tao facil!\"\n" :
                                               (rand() % 2 == 0 ? "\"Minha precisao continua impecavel!\"\n" :
                                                                  "\"Seus ataques sao fortes, mas eu sou agil!\"\n"));
                } else if (personagemEscolhido.nome == "Lara") {
                    cout << (rand() % 3 == 0 ? "\"A luz ainda me guia... eu continuarei lutando!\"\n" :
                                               (rand() % 2 == 0 ? "\"A forca dos reinos flui dentro de mim!\"\n" :
                                                                  "\"A escuridao nunca triunfara sobre a luz!\"\n"));
                }
            }

            // Se o personagem morrer, ele nao pode mais atacar
            if (personagemEscolhido.vida <= 0) {
                cout << personagemEscolhido.nome << " foi derrotado!\n";
            }
        }

        // Verifica se o vilao foi derrotado
        if (vilao.vidaVilao <= 0) {  
            if (vilao.vidaVilao <=0 && vilao.nomeVilao == "O Caos") {
                cout << "\n" << vilao.nomeVilao << " FOI DERROTADO!\n";
                cout << "\n*************************************************";
                cout << "\n********* PARABENS!! VOCE GANHOU O JOGO *********\n";
                cout << "*************************************************\n";
                jogoFinalizado = true;
                break;
            }

            cout << "\n" << vilao.nomeVilao << " FOI DERROTADO!\n";

            // Pergunta se quer continuar jogando
            char escolha;
            cout << "Deseja continuar jogando? (s/n): ";
            cin >> escolha;

            if (escolha == 'n' || escolha == 'N') {
                cout << "Encerrando o jogo..." << endl;
                jogoFinalizado = true;
            } else {
                // Introdução após a morte do vilão
                cout << "\nApos a morte de " << vilao.nomeVilao << ", os herois seguem seu destino rumo a novos desafios.\n";
                if (vilao.nomeVilao == "Guardiao das Sombras") {
                    cout << "Eles agora sabem que as sombras sao mais profundas e que o mal ainda persiste nas terras sombrias...\n";
                } else if (vilao.nomeVilao == "Lider dos Trolls") {
                    cout << "Com a derrota do lider, os reinos se preparam para a paz, mas os trolls ainda vagam pelas florestas...\n";
                } else if (vilao.nomeVilao == "O Caos") {
                    cout << "Embora o caos tenha sido derrotado, a ordem do mundo nunca mais sera a mesma...\n";
                }

                break;
            }
        }

        // Verifica se todos os personagens foram derrotados
        if (todosPersonagensDerrotados(personagens)) {
            cout << "\n======== GAME OVER!! Todos seus personagens foram derrotados! ========\n" << endl;
            jogoFinalizado = true;
            break;
        }

        // Alterna o turno
        turno++;
    }
}

// Funcao de inicio de jogo
void comecarJogo(EstadoDoJogo &estadoDoJogo) {
    vector<Personagem> &personagens = estadoDoJogo.personagens;
    vector<Cenario> &cenarios = estadoDoJogo.cenarios;
    vector<Vilao> &viloes = estadoDoJogo.viloes;
    int &cenarioAtual = estadoDoJogo.cenarioAtual;
    bool &jogoFinalizado = estadoDoJogo.jogoFinalizado;

    int numeroDeCenarios = 3;

    for (int i = cenarioAtual; i < numeroDeCenarios; i++) {
        // Exibe o cenrio e seu vilao correspondente
        cout << "\nCenario: " << cenarios[i].nome << " - Descricao: " << cenarios[i].descricao << " - Dificuldade: " << cenarios[i].dificuldade << endl;

        // Descrição da história e motivação do vilão
        if (i == 0) {
            cout << "\nUma presenca sombria... " << viloes[i].nomeVilao << " aparece! Ele, que um dia foi o protetor da Floresta das Sombras, agora busca vinganca, acreditando que a paz e uma ilusao.\n";
        } else if (i == 1) {
            cout << "\nO chao treme enquanto o " << viloes[i].nomeVilao << " se aproxima. Ambicioso e brutal, ele deseja tornar-se o senhor dos reinos, subjugando todas as criaturas sob seu poder.\n";
        } else if (i == 2) {
            cout << "\nUm silencio mortal toma conta do templo... " << viloes[i].nomeVilao << " emerge das trevas, determinado a destruir toda a ordem e lancar o mundo em caos eterno.\n";
        }

        // Inicia a batalha contra o vilao do cenario atual
        batalhar(viloes[i], personagens, jogoFinalizado, estadoDoJogo);
        cenarioAtual = i + 1;

        // O jogo é salvo a cada batalha finalizada
        salvarJogo(estadoDoJogo);

        if (jogoFinalizado) {
            break;
        }
    }
}

int main() {
    // Contexto inicial

    cout << "================================================================================================================================================================================================================\n";
    cout << "\n";
    cout << "Cinco reinos magicos vivem em harmonia, protegidos pelos guardioes. Mas uma ameaca, o mal conhecido como 'O Caos', despertou e seus seguidores espalham a escuridao pelos reinos. O 'Guardiao das Sombras' tomou a Floresta das Sombras, e o 'Lider dos Trolls' invadiu as Montanhas dos Ventos Gelados. Para restaurar a paz, os guardioes devem derrotar esses vilaos e, por fim, enfrentar 'O Caos', ou o equilibrio dos reinos sera perdido para sempre.\n";
    cout << "\n";
    cout << "================================================================================================================================================================================================================\n";

    EstadoDoJogo estadoDoJogo;

    // Criação dos personagens
    estadoDoJogo.personagens = {
    {"Thorin", "Guerreiro", 300, 25},  
    {"Eldra", "Maga", 250, 30},        
    {"Kara", "Ladina", 200, 35},       
    {"Finn", "Arqueiro", 150, 30},     
    {"Lara", "Sacerdotisa", 1000, 30}
    };

    // Criação dos cenários
    estadoDoJogo.cenarios = {
        {"Floresta das Sombras", "Uma floresta antiga e enigmatica, lar de criaturas magicas.", "Dificuldade Baixa"},
        {"Montanhas dos Ventos Gelados", "Passagens traicoeiras e picos cobertos de neve.", "Dificuldade Media"},
        {"Templo do Caos", "Local de origem do antigo mal, escondido em ruinas antigas.", "Dificuldade Alta"}
    };

    // Criação dos vilões
    estadoDoJogo.viloes = {
        {"Guardiao das Sombras", 200, 30},
        {"Lider dos Trolls", 300, 50},
        {"O Caos", 500, 70} // vilão final
    };

    estadoDoJogo.cenarioAtual = 0;
    estadoDoJogo.jogoFinalizado = false;

    int escolha;

    // Menu inicial
    do {
        cout << "\n=====================================\n";
        cout << "|        AVENTURA DOS 5 REINOS       |\n";
        cout << "=====================================\n";
        cout << "|        Escolha uma opcao:          |\n";
        cout << "|        1. Iniciar Jogo             |\n";
        cout << "|        2. Carregar Progresso       |\n";
        cout << "|        3. Sair                     |\n";
        cout << "=====================================\n";
        cout << "Digite a opcao: ";
        cin >> escolha;

        switch (escolha)
        {
        case 1:
            comecarJogo(estadoDoJogo);
            break;

        case 2:
            if (carregarJogo(estadoDoJogo)) {
                comecarJogo(estadoDoJogo);
            }
            break;

        case 3:
            cout << "Saindo...\n";
            break;

        default:
            cout << "\nOPCAO INVALIDA!! DIGITE A OPCAO CORRETA.\n";
            break;
        }

    } while (escolha != 3 && !estadoDoJogo.jogoFinalizado);

    return 0;
}