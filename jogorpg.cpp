// Jogo RPG
// Alunos: Barbara Figueiredo (841308), João Pedro Leme Araújo(842123), Pedro Augusto Duarte Moreira (842580), Welton Wagner Dolce Junior (842698).
// Professor: Thiago Nicola Cajuela Garcia
// Matéria: Laboratório de Técnicas de Programação

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Criando struct para personagem, cenario e vilao
struct Personagem
{
    string nome;
    string classe;
    int vida;
    int dano;
};

struct Cenario
{
    string nome;
    string descricao;
    string dificuldade;
};

struct Vilao
{
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
bool todosPersonagensDerrotados(const vector<Personagem> &personagens)
{
    for (int i = 0; i < 5; i++)
    {
        if (personagens[i].vida > 0)
        {
            return false;
        }
    }
    return true;
}

// Funcao para batalha, recendo passagem por referencia
void batalhar(Vilao &vilao, vector<Personagem> &personagens, bool &jogoFinalizado)
{
    int turno = 0;
    int numeroDePersonagens = 5;
    int escolhaPersonagem;

    while (vilao.vidaVilao > 0)
    {
        // Exibindo as informacoes de vida do vilao
        cout << "\nVida do " << vilao.nomeVilao << ": " << vilao.vidaVilao << "\n";

        // Escolher personagem para o turno
        cout << "\nEscolha o personagem para atacar: \n";
        for (int i = 0; i < numeroDePersonagens; i++)
        {
            if (personagens[i].vida > 0)
            {
                cout << i + 1 << ". " << personagens[i].nome << " (" << personagens[i].classe << ") - Vida: " << personagens[i].vida << " - Dano: " << personagens[i].dano << endl;
            }
        }
        cout << "\nEscolha seu personagem: ";
        cin >> escolhaPersonagem;

        // Validacao da escolha
        if (escolhaPersonagem < 1 || escolhaPersonagem > numeroDePersonagens || personagens[escolhaPersonagem - 1].vida <= 0)
        {
            cout << "\nOpcao invalida ou personagem derrotado. Tente novamente.\n";
            continue;
        }

        // Escolhendo o indice correto correspondente ao personagem por referencia
        // Exemplo: Kara é a personagem 3, mas no vetor é o indice [2]
        Personagem &personagemEscolhido = personagens[escolhaPersonagem - 1];

        // Ataque do personagem ao vilao
        cout << personagemEscolhido.nome << " ataca " << vilao.nomeVilao << " causando " << personagemEscolhido.dano << " de dano!\n";
        vilao.vidaVilao -= personagemEscolhido.dano;

        // Se o vilao ainda estiver vivo, ele contra-ataca
        if (vilao.vidaVilao > 0)
        {
            cout << vilao.nomeVilao << " contra-ataca, causando " << vilao.forcaVilao << " de dano!\n";
            personagemEscolhido.vida -= vilao.forcaVilao;

            // Se o personagem morrer, ele nao pode mais atacar
            if (personagemEscolhido.vida <= 0)
            {
                cout << personagemEscolhido.nome << " foi derrotado!\n";
            }
        }

        // Verifica se o vilao foi derrotado
        if (vilao.vidaVilao <= 0)
        {
            cout << vilao.nomeVilao << " foi derrotado!\n";
            break;
        }

        // Verifica se todos os personagens foram derrotados
        if (todosPersonagensDerrotados(personagens))
        {
            cout << "\nGAME OVER!! Todos seus personagens foram derrotados!\n"
                 << endl;
            jogoFinalizado = true;
            break;
        }

        // Alterna o turno
        turno++;
    }
}

// Funcao de inicio de jogo
void comecarJogo(EstadoDoJogo &estadoDoJogo)
{
    vector<Personagem> &personagens = estadoDoJogo.personagens;
    vector<Cenario> &cenarios = estadoDoJogo.cenarios;
    vector<Vilao> &viloes = estadoDoJogo.viloes;
    int &cenarioAtual = estadoDoJogo.cenarioAtual;
    bool &jogoFinalizado = estadoDoJogo.jogoFinalizado;

    int numeroDeCenarios = 3;

    for (int i = cenarioAtual; i < numeroDeCenarios; i++)
    {
        // Exibe o cenrio e seu vilao correspondente
        cout << "\nCenario: " << cenarios[i].nome << " - Descricao: " << cenarios[i].descricao << " - Dificuldade: " << cenarios[i].dificuldade << endl;

        // Inicia a batalha contra o vilao do cenario atual
        batalhar(viloes[i], personagens, jogoFinalizado);
        cenarioAtual = i + 1;

        // O jogo é salvo a cada batalha finalizada
        salvarJogo(estadoDoJogo);

        if (jogoFinalizado)
        {
            break;
        }
    }
}

int main()
{
    cout << "Cinco reinos magicos coexistem em paz, cada um com um guardiao escolhido que protege seu povo e sua terra. Um antigo mal, no entanto, ameaca romper essa harmonia e subjugar todos os reinos. Os cinco guardioes precisam embarcar em uma jornada para encontrar os fragmentos de um artefato lendario, o qual permitira a destruicao do inimigo e a restauracao da paz.\n";

    EstadoDoJogo estadoDoJogo;

    // Criação dos personagens
    estadoDoJogo.personagens = {
        {"Thorin", "Guerreiro", 300, 25},
        {"Eldra", "Maga", 250, 30},
        {"Kara", "Ladina", 200, 35},
        {"Finn", "Arqueiro", 150, 30},
        {"Lara", "Sacerdotisa", 1000, 30}};

    // Criação dos cenários
    estadoDoJogo.cenarios = {
        {"Floresta das Sombras", "Uma floresta antiga e enigmatica, lar de criaturas magicas.", "Dificuldade Baixa"},
        {"Montanhas dos Ventos Gelados", "Passagens traicoeiras e picos cobertos de neve.", "Dificuldade Media"},
        {"Templo do Caos", "Local de origem do antigo mal, escondido em ruinas antigas.", "Dificuldade Alta"}};

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
    do
    {
        cout << "\n===== AVENTURA DOS 5 REINOS =====\n";
        cout << "1. Iniciar jogo\n";
        cout << "2. Carregar o progresso salvo\n";
        cout << "3. Sair\n";
        cout << "Digite a opcao: ";
        cin >> escolha;

        switch (escolha)
        {
        case 1:
            comecarJogo(estadoDoJogo);
            break;

        case 2:
            if (carregarJogo(estadoDoJogo))
            {
                comecarJogo(estadoDoJogo);
            }
            break;

        case 3:
            cout << "Saindo...\n";
            break;

        default:
            cout << "Opcao invalida! Tente novamente.\n";
            break;
        }

    } while (escolha != 3 && !estadoDoJogo.jogoFinalizado);

    return 0;
}