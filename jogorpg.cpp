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

// Criando struct para item, personagem, cenario, vilao, estado do jogo

struct Item {
    string nome;
    string tipo;
    int valor;
};

struct Personagem {
    string nome;
    string classe;
    int vida;
    int dano;
    vector<Item> inventario;
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

struct EstadoDoJogo {
    vector<Personagem> personagens;
    vector<Cenario> cenarios;
    vector<Vilao> viloes;
    int cenarioAtual;
    bool jogoFinalizado;
};

// Função para salvar o estado do jogo
void salvarJogo(const EstadoDoJogo &estadoDoJogo) {
    cout << "Salvando Progresso" << endl;

    ofstream arquivo("estado_do_jogo.txt");
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo para salvar o jogo!" << endl;
        return;
    }

    arquivo << "CenarioAtual: " << estadoDoJogo.cenarioAtual << endl;
    arquivo << "JogoFinalizado: " << (estadoDoJogo.jogoFinalizado ? "true" : "false") << endl;

    arquivo << "Personagens:" << endl;
    for (const auto &personagem: estadoDoJogo.personagens) {
        arquivo << personagem.nome << ","
                << personagem.classe << ","
                << personagem.vida << ","
                << personagem.dano << ";"
                << personagem.inventario.size();
        for (const auto &item: personagem.inventario) {
            arquivo << "," << item.nome << ","
                    << item.tipo << ","
                    << item.valor;
        }
        arquivo << endl;
    }

    arquivo << "Cenarios:" << endl;
    for (const auto &cenario: estadoDoJogo.cenarios) {
        arquivo << cenario.nome << ","
                << cenario.descricao << ","
                << cenario.dificuldade << endl;
    }

    arquivo << "Viloes:" << endl;
    for (const auto &vilao: estadoDoJogo.viloes) {
        arquivo << vilao.nomeVilao << ","
                << vilao.vidaVilao << ","
                << vilao.forcaVilao << endl;
    }

    arquivo.close();
    cout << "Progresso salvo com sucesso." << endl;
}

// Função para carregar o estado do jogo
bool carregarJogo(EstadoDoJogo &estadoDoJogo) {
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

    getline(arquivo, linha);
    estadoDoJogo.cenarioAtual = stoi(linha.substr(linha.find(":") + 1));

    getline(arquivo, linha);
    estadoDoJogo.jogoFinalizado = (linha.substr(linha.find(":") + 1) == "true");

    getline(arquivo, linha);
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

        pos = linha.find(";");
        p.dano = stoi(linha);
        linha = linha.substr(pos + 1);

        pos = linha.find(",");
        int numItens = pos == string::npos ? stoi(linha) : stoi(linha.substr(0, pos));
        if (pos != string::npos) {
            linha = linha.substr(pos + 1);

            for (int i = 0; i < numItens; i++) {
                Item item;

                pos = linha.find(",");
                item.nome = linha.substr(0, pos);
                linha = linha.substr(pos + 1);

                pos = linha.find(",");
                item.tipo = linha.substr(0, pos);
                linha = linha.substr(pos + 1);

                pos = (i == numItens - 1) ? string::npos : linha.find(",");
                item.valor = stoi(linha.substr(0, pos));
                if (pos != string::npos) {
                    linha = linha.substr(pos + 1);
                }

                p.inventario.push_back(item);
            }
        }

        estadoDoJogo.personagens.push_back(p);
    }

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

void adicionarItensAoPersonagem(Personagem &personagem) {
    // Criando itens
    Item pocao = {"Pocao de Cura", "cura", 50};
    Item espada = {"Espada Magica", "dano", 30};
    Item machado = {"Machado de Guerra", "dano", 40};
    Item varinha = {"Varinha Mistica", "dano", 50};
    Item livro = {"Livro de Feiticos", "cura", 25};
    Item punhal = {"Punhal Envenenado", "dano", 35};
    Item amuleto = {"Amuleto de Agilidade", "dano", 30};
    Item arco = {"Arco Longo", "dano", 45};
    Item capa = {"Capa da Luz", "cura", 25};
    Item aljava = {"Aljava Infinita", "dano", 20};

    // Adicionando os itens ao inventário do personagem
    if (personagem.nome == "Athos") {
        personagem.inventario.push_back(machado);
        personagem.inventario.push_back(pocao);
    } else if (personagem.nome == "Trixy") {
        personagem.inventario.push_back(varinha);
        personagem.inventario.push_back(livro);
    } else if (personagem.nome == "Kara") {
        personagem.inventario.push_back(punhal);
        personagem.inventario.push_back(amuleto);
    } else if (personagem.nome == "Finn") {
        personagem.inventario.push_back(arco);
        personagem.inventario.push_back(aljava);
    } else if (personagem.nome == "Lara") {
        personagem.inventario.push_back(espada);
        personagem.inventario.push_back(capa);
    }
}

// Função para coleta de item
void pegarItem(Personagem &p, const Item &i) {
    cout << p.nome << "pegou o item: " << i.nome << endl;
    p.inventario.push_back(i);
}

// Função para uso do item
void usarItem(Personagem &p, int indiceItem) {
    // Validando o indice do item
    if (indiceItem < 0 || indiceItem >= p.inventario.size()) {
        cout << "ITEM INVALIDO!" << endl;
        return;
    }

    Item &item = p.inventario[indiceItem];

    // Saida para uso dos itens
    if (item.tipo == "cura") {
        cout << "\n" << p.nome << " usou: " << item.nome << " e ganhou " << item.valor << " de vida!" << endl;
        p.vida += item.valor;
    } else if (item.tipo == "dano") {
        cout << "\n" << p.nome << " usou: " << item.nome << " e ganhou " << item.valor << " de dano!" << endl;
        p.dano += item.valor;
    }

    // Funcao erase remove o item do inventario
    p.inventario.erase(p.inventario.begin() + indiceItem);
}

// Função para mostrar o inventario do personagem
void mostrarInventario(const Personagem &p) {
    if (p.inventario.empty()) {
        cout << "\n" << p.nome << " nao possui itens no inventario" << endl;
        return;
    }

    cout << "\nInventario do " << p.nome << ":\n";
    for (int i = 0; i < p.inventario.size(); i++) {
        cout << i + 1 << ". " << p.inventario[i].nome << " - Tipo: " << p.inventario[i].tipo << " - Valor: " << p.
                inventario[i].valor << endl;
    }
}

// Função para usar itens durante a batalha
void usarItemDuranteBatalha(Personagem &p) {
    // Validando se o personagem tem itens
    if (p.inventario.empty()) {
        cout << "\n" << p.nome << " nao possui itens no inventario para ser usados" << endl;
        return;
    }

    // Mostra o inventario
    mostrarInventario(p);

    // Escolhe o item
    cout << "Escolha um item para usar (digite um numero): ";
    int escolha;
    cin >> escolha;

    // Valida o item
    if (escolha < 1 || escolha > p.inventario.size()) {
        cout << "ESCOLHA INVALIDA!" << endl;
        return;
    }

    // Chama a funcao de usar o item
    usarItem(p, escolha - 1);
}

// Funcao para verificar se todos os personagens foram derrotados
bool todosPersonagensDerrotados(const vector<Personagem> &personagens) {
    for (int i = 0; i < 5; i++) {
        if (personagens[i].vida > 0) {
            return false;
        }
    }
    return true;
}

// Funcao para batalha, recendo passagem por referencia
void batalhar(Vilao &vilao, vector<Personagem> &personagens, bool &jogoFinalizado, EstadoDoJogo &estadoDoJogo) {
    int turno = 0;
    int numeroDePersonagens = 5;
    int escolhaPersonagem;

    //Introdução do vilão antes da batalha começar
    cout << "\n" << vilao.nomeVilao << " diz: ";
    if (vilao.nomeVilao == "Guardiao das Sombras") {
        cout << "\"Voces destruiram tudo! Agora sentirao a verdadeira escuridao\"";
    } else if (vilao.nomeVilao == "Lider dos Trolls") {
        cout << "\"O mundo sera meu! Nenhum de voces sovrevivera\"";
    } else if (vilao.nomeVilao == "O Caos") {
        cout << "\"A ordem eh uma mentira! Apenas o caos eh eterno!\"";
    }
    cout << endl;

    while (vilao.vidaVilao > 0) {
        // Exibindo as informacoes de vida do vilao
        cout << "\n============================================\n";
        cout << "\n\tVILAO: " << vilao.nomeVilao << "\n" << "\tVIDA: " << vilao.vidaVilao << endl;
        cout << "\n============================================\n";

        // Escolher personagem para o turno
        cout << "\nEscolha o personagem para atacar: \n";
        for (int i = 0; i < numeroDePersonagens; i++) {
            if (personagens[i].vida > 0) {
                cout << i + 1 << ". " << personagens[i].nome << " (" << personagens[i].classe << ") - Vida: " <<
                        personagens[i].vida << " - Dano: " << personagens[i].dano << endl;
            }
        }
        cout << "\nEscolha seu personagem: ";
        cin >> escolhaPersonagem;

        // Validacao da escolha
        if (escolhaPersonagem < 1 || escolhaPersonagem > numeroDePersonagens || personagens[escolhaPersonagem - 1].vida
            <= 0) {
            cout << "\nOpcao invalida ou personagem derrotado. Tente novamente.\n";
            continue;
        }

        // Escolhendo o indice correto correspondente ao personagem por referencia
        // Exemplo: Kara é a personagem 3, mas no vetor é o indice [2]
        Personagem &personagemEscolhido = personagens[escolhaPersonagem - 1];

        cout << "\nUsar um item durante o turno? (s/n): ";
        char usarItemEscolha;
        cin >> usarItemEscolha;

        // Validando entrada
        if (usarItemEscolha == 's' || usarItemEscolha == 'S') {
            usarItemDuranteBatalha(personagemEscolhido); // Permite ao jogador usar um item
        }

        // Ataque do personagem ao vilao
        cout << "\n" << personagemEscolhido.nome << " ataca " << vilao.nomeVilao << " causando " << personagemEscolhido.
                dano << " de dano!\n";
        vilao.vidaVilao -= personagemEscolhido.dano;

        // Se o vilao ainda estiver vivo, ele contra-ataca
        if (vilao.vidaVilao > 0) {
            cout << vilao.nomeVilao << " contra-ataca, causando " << vilao.forcaVilao << " de dano!\n";
            personagemEscolhido.vida -= vilao.forcaVilao;

            // Frase entre ataques dos vilões
            cout << "\n" << vilao.nomeVilao << " diz: ";
            if (vilao.nomeVilao == "Guardiao das Sombras") {
                cout << (rand() % 2 == 0
                             ? "\"Voces vao se perder para sempre, assim como todos os que me desafiaram!\"\n"
                             : "\"Sombras eternas irao devorar todos voces!\"\n");
            } else if (vilao.nomeVilao == "Lider dos Trolls") {
                cout << (rand() % 2 == 0
                             ? "\"Esse ataque foi patetico! Eu sou indestrutivel!\"\n"
                             : "\"Eu pisotearei todos que se atreverem a me enfrentar!\"\n");
            } else if (vilao.nomeVilao == "O Caos") {
                cout << (rand() % 2 == 0
                             ? "\"Nada pode conter o Caos! Em breve, tudo sera reduzido ao nada!\"\n"
                             : "\"A ordem sera destruida, e o caos reinara para sempre!\"\n");
            }
            cout << endl;

            // Dentro da função batalhar, logo após o vilão atacar o personagem escolhido
            if (personagemEscolhido.vida > 0) {
                cout << personagemEscolhido.nome << " diz: ";

                if (personagemEscolhido.nome == "Athos") {
                    cout << (rand() % 3 == 0
                                 ? "\"Isso e tudo que voce tem? Eu sou mais forte do que isso!\"\n"
                                 : (rand() % 2 == 0
                                        ? "\"Vai ser preciso mais do que isso para me derrubar!\"\n"
                                        : "\"Eu ja lutei contra piores do que voce!\"\n"));
                } else if (personagemEscolhido.nome == "Trixy") {
                    cout << (rand() % 3 == 0
                                 ? "\"Voce ousa desafiar o poder da magia?\"\n"
                                 : (rand() % 2 == 0
                                        ? "\"Minhas defesas ainda estao intactas. Prepare-se!\"\n"
                                        : "\"Essa dor so fortalece minha determinacao!\"\n"));
                } else if (personagemEscolhido.nome == "Kara") {
                    cout << (rand() % 3 == 0
                                 ? "\"Ah, voce vai pagar caro por esse ataque!\"\n"
                                 : (rand() % 2 == 0
                                        ? "\"Mal me acertou! Vai ter que ser mais rapido!\"\n"
                                        : "\"Voce nao tem chance contra minhas habilidades!\"\n"));
                } else if (personagemEscolhido.nome == "Finn") {
                    cout << (rand() % 3 == 0
                                 ? "\"Nao pense que vou perder o foco tao facil!\"\n"
                                 : (rand() % 2 == 0
                                        ? "\"Minha precisao continua impecavel!\"\n"
                                        : "\"Seus ataques sao fortes, mas eu sou agil!\"\n"));
                } else if (personagemEscolhido.nome == "Lara") {
                    cout << (rand() % 3 == 0
                                 ? "\"A luz ainda me guia... eu continuarei lutando!\"\n"
                                 : (rand() % 2 == 0
                                        ? "\"A forca dos reinos flui dentro de mim!\"\n"
                                        : "\"A escuridao nunca triunfara sobre a luz!\"\n"));
                }
            }

            // Se o personagem morrer, ele nao pode mais atacar
            if (personagemEscolhido.vida <= 0) {
                cout << personagemEscolhido.nome << " foi derrotado!\n";
            }
        }

        // Verifica se o vilao foi derrotado
        if (vilao.vidaVilao <= 0) {
            if (vilao.vidaVilao <= 0 && vilao.nomeVilao == "O Caos") {
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
                cout << "\nEncerrando o jogo..." << endl;
                jogoFinalizado = true;
            } else {
                // Introdução após a morte do vilão
                cout << "\nApos a morte de " << vilao.nomeVilao <<
                        ", os herois seguem seu destino rumo a novos desafios.\n";
                if (vilao.nomeVilao == "Guardiao das Sombras") {
                    cout <<
                            "Eles agora sabem que as sombras sao mais profundas e que o mal ainda persiste nas terras sombrias...\n";
                } else if (vilao.nomeVilao == "Lider dos Trolls") {
                    cout <<
                            "Com a derrota do lider, os reinos se preparam para a paz, mas os trolls ainda vagam pelas florestas...\n";
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
        cout << "\n\tCENARIO: " << cenarios[i].nome << "\n" << "\tDESCRICAO: " << cenarios[i].descricao << "\n" <<
                "\tDIFICULDADE: " << cenarios[i].dificuldade << endl;

        // Descrição da história e motivação do vilão
        if (i == 0) {
            cout <<
                    "\n========================================================================================================================\n";
            cout <<
                    "A Floresta Sombria eh envolta de misterio. Possui arvores tao densas que a luz nunca penetra completamente. Na antiguidade, fora uma floresta "
                    "exuberante, governada por elfos e criaturas de luz. Cego pelo poder,o Rei Elfo usou um fragmento de poder para formar um exercito e conquistar novos "
                    "reinos. No entanto, o poder contido no fragmento era maior do que o imaginado e ao tentar conter o caos, o protetor da floresta acaba sendo transformado "
                    "no Guardiao das Sombras. Agora, cabe aos herois a missao de derrotar o mal e trazer de volta a luz a Floresta das Sombras."
                    << endl;
            cout << "\nUma presenca sombria... " << viloes[i].nomeVilao <<
                    " aparece! Ele, que um dia foi o protetor da Floresta das Sombras, agora busca vinganca, acreditando que a paz eh uma ilusao.\n";
        } else if (i == 1) {
            cout <<
                    "\n========================================================================================================================\n";
            cout <<
                    "Apos a derrota do Guardiao das Sombras, os viajantes seguem em busca dos pedacos restantes do Fragmento de Poder."
                    " Seguindo as pistas, eles vao parar nas Montanhas dos Ventos Gelados. Conhecida pelos seus picos nevados, cavernas "
                    "profundas e inexploradas e seus ventos cortantes, as montanhas um dia foram habitadas por mineradores que, apenas do "
                    "ambiente inospito, eram responsaveis pela extracao dos metais preciosos que abasteciam o reino. No entanto,"
                    "apos a ruptura da ordem, o frio se intensificou e os trolls, atraidos pelo ouro e pelas pedras preciosas tomaram "
                    "conta do lugar. Comandados pelo Lider dos Trolls, a legiao instaura o caos e o medo nas montanhas, fazendo com que "
                    "os seus antigos habitantes busquem refugio nas cavernas. Assim, e dever dos herois derrotar a ameaca e trazer a paz"
                    << endl;
            cout << "\nO chao treme enquanto o " << viloes[i].nomeVilao <<
                    " se aproxima. Ambicioso e brutal, ele deseja tornar-se o senhor dos reinos, subjugando todas as criaturas sob seu poder.\n";
        } else if (i == 2) {
            cout <<
                    "\n========================================================================================================================\n";
            cout <<
                    "Dois reinos ja podem celebrar a sua liberdade. No entanto, ainda resta o maior dos viloes, O Caos. A personificacao da destruicao e da desordem"
                    "que busca destruir o mundo, O Caos surgiu no momento em que os reinos, fomentados pela busca por poder, fragmentaram a Chama Primordial. Alimentado "
                    "pela corrupcao e pelo conflitos dos lideres que deveriam garantir a ordem e a felicidade do seu povo, O Caos nao acredita que a ordem possa ser "
                    "restaurada sem que o mundo seja completamente destruido. Ao adentrar no Templo do Caos, um labirinto testa as habilidades dos herois que, ao chegar no "
                    "coracao do Templo, encontram um aglomerado de escuridao sem forma definida. O vilao ja os aguardava para o embate final."
                    << endl;
            cout << "\nUm silencio mortal toma conta do templo... " << viloes[i].nomeVilao <<
                    " emerge das trevas, determinado a destruir toda a ordem e lancar o mundo em caos eterno.\n";
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

    cout << "===============================================================================================\n";
    cout << "\n";
    cout <<
            "Tres reinos magicos viveram em harmonia, protegidos pelos guardioes. No entanto, ao buscar poder e gloria, os reinos se corromperam, "
            "trazendo atona um desequilibrio na ordem e na magia. Por conta disso, uma ameaca, o mal conhecido como 'O Caos', despertou e seus seguidores "
            "espalham a escuridao pelos reinos. O 'Guardiao das Sombras' tomou a Floresta das Sombras, e o 'Lider dos Trolls' invadiu as "
            "Montanhas dos Ventos Gelados. Para restaurar a paz, os guardioes devem derrotar esses vilaos e, por fim, enfrentar 'O Caos', ou "
            "o equilibrio dos reinos sera perdido para sempre.\n";
    cout << "\n";
    cout << "===============================================================================================\n";

    EstadoDoJogo estadoDoJogo;

    // Criação dos personagens
    estadoDoJogo.personagens = {
        {"Athos", "Guerreiro", 300, 25},
        {"Trixy", "Maga", 250, 30},
        {"Kara", "Assassina", 200, 35},
        {"Finn", "Arqueiro", 150, 30},
        {"Lara", "Oraculo", 1000, 30}
    };

    // Adiciona itens ao inventário de cada personagem
    for (Personagem &personagem: estadoDoJogo.personagens) {
        adicionarItensAoPersonagem(personagem);
    }

    // Criação dos cenários
    estadoDoJogo.cenarios = {
        {"Floresta das Sombras", "Uma floresta antiga e enigmatica, lar de criaturas magicas.", "Dificuldade Baixa"},
        {"Montanhas dos Ventos Gelados", "Passagens traicoeiras e picos cobertos de neve.", "Dificuldade Media"},
        {"Templo do Caos", "Local de origem do antigo mal, escondido em ruinas antigas.", "Dificuldade Alta"}
    };

    // Criação dos vilões
    estadoDoJogo.viloes = {
        {"Guardiao das Sombras", 300, 40},
        {"Lider dos Trolls", 400, 60},
        {"O Caos", 950, 120} // vilão final
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

        switch (escolha) {
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
