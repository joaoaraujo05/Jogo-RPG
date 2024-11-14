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
void batalhar(Vilao& vilao, vector<Personagem>& personagens, bool& jogoFinalizado) {
    int turno = 0;
    int numeroDePersonagens = 5;
    int escolhaPersonagem;

    //Introdução do vilão antes da batalha começar
    cout << "\n" << vilao.nomeVilao << "diz: ";
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
        cout << "\nVida do " << vilao.nomeVilao << ": " << vilao.vidaVilao << "\n";
        
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
                                           "\"A ordem sera destruída, e o caos reinara para sempre!\"\n");
            }
            cout << endl;

            // Dentro da função batalhar, logo após o vilão atacar o personagem escolhido
            if (personagemEscolhido.vida > 0) {
                cout << personagemEscolhido.nome << " diz: ";
                
                if (personagemEscolhido.nome == "Thorin") {
                    cout << (rand() % 3 == 0 ? "\"Isso e tudo que você tem? Eu sou mais forte do que isso!\"\n" :
                                               (rand() % 2 == 0 ? "\"Vai ser preciso mais do que isso para me derrubar!\"\n" :
                                                                  "\"Eu ja lutei contra piores do que você!\"\n"));
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
                cout << "\nPARABENS!! VOCE GANHOU O JOGO";
            }
            cout << vilao.nomeVilao << " foi derrotado!\n";

            // Introdução após a morte do vilão
            cout << "\nApós a morte de " << vilao.nomeVilao << ", os herois seguem seu destino rumo a novos desafios.\n";
            if (vilao.nomeVilao == "Guardiao das Sombras") {
                cout << "Eles agora sabem que as sombras sao mais profundas e que o mal ainda persiste nas terras sombrias...\n";
            } else if (vilao.nomeVilao == "Lider dos Trolls") {
                cout << "Com a derrota do lider, os reinos se preparam para a paz, mas os trolls ainda vagam pelas florestas...\n";
            } else if (vilao.nomeVilao == "O Caos") {
                cout << "Embora o caos tenha sido derrotado, a ordem do mundo nunca mais sera a mesma...\n";
            }

            break;
        }

        // Verifica se todos os personagens foram derrotados
        if (todosPersonagensDerrotados(personagens)) {
            cout << "\nGAME OVER!! Todos seus personagens foram derrotados!\n" << endl;
            jogoFinalizado = true;
            break;
        }

        // Alterna o turno
        turno++;
    }
}

// Funcao de inicio de jogo
void comecarJogo(vector<Personagem>& personagens, vector<Cenario>& cenarios, vector<Vilao>& viloes, bool& jogoFinalizado) {
    int numeroDeCenarios = 3;
    
    for (int i = 0; i < numeroDeCenarios; i++) {
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
        batalhar(viloes[i], personagens, jogoFinalizado);

        if (jogoFinalizado) {
            break;
        }
    }
}

int main() {
    // Contexto inicial
    cout << "Cinco reinos magicos vivem em harmonia, protegidos pelos guardioes. Mas uma ameaca, o mal conhecido como 'O Caos', despertou e seus seguidores espalham a escuridao pelos reinos. O 'Guardiao das Sombras' tomou a Floresta das Sombras, e o 'Lider dos Trolls' invadiu as Montanhas dos Ventos Gelados. Para restaurar a paz, os guardioes devem derrotar esses vilaos e, por fim, enfrentar 'O Caos', ou o equilibrio dos reinos sera perdido para sempre.\n";

    // Criação dos personagens
    vector<Personagem> personagens = {
    {"Thorin", "Guerreiro", 300, 25},  
    {"Eldra", "Maga", 250, 30},        
    {"Kara", "Ladina", 200, 35},       
    {"Finn", "Arqueiro", 150, 30},     
    {"Lara", "Sacerdotisa", 1000, 30}
    };

    // Criação dos cenários
    vector<Cenario> cenarios = {
        {"Floresta das Sombras", "Uma floresta antiga e enigmatica, lar de criaturas magicas.", "Dificuldade Baixa"},
        {"Montanhas dos Ventos Gelados", "Passagens traicoeiras e picos cobertos de neve.", "Dificuldade Media"},
        {"Templo do Caos", "Local de origem do antigo mal, escondido em ruinas antigas.", "Dificuldade Alta"}
    };

    // Criação dos vilões
    vector<Vilao> viloes = {
        {"Guardiao das Sombras", 200, 30},
        {"Lider dos Trolls", 300, 50},
        {"O Caos", 500, 70} // vilão final
    };

    int escolha;
    bool jogoFinalizado = false;

    // Menu inicial
    do {
        cout << "\n===== AVENTURA DOS 5 REINOS =====\n";
        cout << "1. Iniciar jogo\n";
        cout << "2. Salvar o progresso\n";
        cout << "3. Sair\n";
        cout << "Digite a opcao: ";
        cin >> escolha;

        switch (escolha)
        {
        case 1:
            comecarJogo(personagens, cenarios, viloes, jogoFinalizado);
            break;
        
        case 2:
            //salvarJogo();
            break;

        case 3:
            cout << "Saindo...\n";
            break;

        default:
            cout << "Opcao invalida! Tente novamente.\n";
            break;
        }

    } while (escolha != 3 && !jogoFinalizado);

    return 0;
}