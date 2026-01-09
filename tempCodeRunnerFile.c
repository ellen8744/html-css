// --- Sistema de gerenciamendo de biblioteca --- 

 

 

 

#include <stdio.h> // Para entrada e saida padrao 

#include <stdlib.h> // Para declarar as fucoes 

#include <string.h> // Para manipulacao das strings 

#include <time.h> //Para funcoes de data 

#include <stdbool.h> //Para dar suporte a dados do tipo booleano 

#define livrosmax 100 

#define usuariosmax 100 

#define emprestimomax 100 

 // Estruturas 

typedef struct { //Aqui coloquei para a funcao de data ser no formato DD/MM/AAAA 

int dia; 

int mes; 

int ano; 

} Data ; 

//Estruturas de struct de livro,usuario e emprestimo 

struct livro { 

int codigo; 

char titulo[100]; 

char autor[80]; 

char editora[60]; 

int ano; 

int exemplaresdisponiveis; // Numero de exemplares disponiveis 

}; 

struct usuario { 

int matricula; 

char nome[100]; 

char curso[50]; 

char telefone[15]; 

Data data_cadastro; 

};  

struct emprestimo { 

int codigo; // Codigo do emprestimo 

int matricula; 

int codigolivro; 

int dia,mes,ano;// Data do emprestimo 

int diaD,mesD,anoD; // Data da devolucao prevista(apos 7 dias) 

char status; // 1 = ativo, 0 = devolvido 

}; 

// Variaveis globais para contadores de itens 

int totallivros = 0; 

int totalusuarios = 0; 

int totalemprestimos = 0; 

// Arrays globais 

struct livro livros[livrosmax]; 

struct usuario usuarios[usuariosmax]; 

struct emprestimo emprestimos[emprestimomax]; 

// ==FunÃ§Ãµes== 

// Para remover dados que causem erros na leitura 

void limpar_buffer() { 

int c; 

while ((c = getchar()) != '\n' && c != EOF){} 

} 

// Funcao para ler numeros inteiros e limpar buffer 

int ler_inteiro() { 

int escolha; 

while (scanf("%d",&escolha)!=1) { 

printf("ERRO: Escolha invalida. Digite apenas numeros inteiros: "); 

limpar_buffer(); 

} 

limpar_buffer(); 

return escolha; 

} 

// Para ler strings de titulo, autor... 

void ler_string(char *str, int tamanho, const char *prompt) { 

// imprimir prompt 

printf("%s", prompt); 

if (fgets(str,tamanho,stdin)==NULL) { // para ler linha 

str[0]= '\0'; 

return; 

} 

size_t len = strlen(str); 

if (len > 0 && str[len-1] == '\n') { 

str[len-1] = '\0'; 

}else { 

limpar_buffer(); 

} 

} 

// PROTOTIPOS DE FUNCOES 

int menu_principal(void); 

void menu_livros(void); 

void menu_usuario(void); 

void menu_emprestimo(void); 

// Livros 

void cadastrarlivro(void); 

void mostrar_livros(void); 

void pesquisar_livro(void); 

// Usuarios 

void cadastrarusuario(void); 

void mostrarusuario(void); 

void pesquisar_usuario(void); 

// Emprestimos 

void cadastraremprestimo(void); 

// Datas 

bool ano_bissexto(int ano); 

int diasnoMes(int mes, int ano); 

Data adicionar_dias(Data data,int dias); 

void cadastrardata(Data *data,const char *prompt); 

// Arquivos 

void salvar_tudo(void); 

void carregar_tudo(void); // Implementada no final 

// IMPLEMENTACAO DAS FUNCOES DE DATA 

bool ano_bissexto(int ano) { //Fiz tambem para funcionar em ano bissexto,caso for 

return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0; 

} 

int diasnoMes(int mes ,int ano) { 

if (mes < 1 || mes > 12) return 0; 

if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30; 

if (mes == 2) return ano_bissexto(ano) ? 29 : 28; 

return 31; 

} 

Data adicionar_dias(Data data ,int dias) { 

data.dia += dias; 

while (data.dia > diasnoMes(data.mes,data.ano)){ 

data.dia -= diasnoMes(data.mes,data.ano); 

data.mes++; 

if (data.mes > 12) { 

data.mes = 1; 

data.ano++; 

} 

} 

return data; 

} 

void cadastrardata(Data *data,const char *prompt) { 

int dia, mes, ano; 

bool data_valida = false; 

do { 

printf("%s (DD/MM/AAAA): ",prompt); 

if (scanf("%d/%d/%d", &dia, &mes, &ano) == 3) { 

limpar_buffer(); 

if (mes >= 1 && mes <= 12 && dia >= 1 && dia <= 

diasnoMes(mes, ano) && ano >= 1900) { 

data->dia = dia; 

data->mes = mes; 

data->ano = ano; 

data_valida = true; 

} else {printf("ERRO: Data invalida. Verifique dia, mes e ano. \n"); 

} 

} else { 

printf("ERRO: Formato de data errada. Digite no formato DD/MM/AAAA. \n"); 

limpar_buffer(); 

} 

} while (!data_valida); 

} 

// IMPLEMENTACAO DAS FUNCOES DE ARQUIVO 

void salvar_tudo() { 

FILE *arquivo; 

// Para livros 

arquivo = fopen("livros.txt","w"); 

if (arquivo!=NULL) { 

fprintf(arquivo, "%d\n", totallivros); 

int i; 

for (i=0;i<totallivros;i++) { 

// Formato:codigo,titulo,autor,editora,ano,exemplaresdisponiveis 

fprintf(arquivo, "%d,%s,%s,%s,%d,%d\n", 

livros[i].codigo, 

livros[i].titulo[0]? livros[i].titulo: "" 

, 

livros[i].autor[0] ? livros[i].autor: "" 

, 

livros[i].editora[0] ? livros[i].editora: "" 

, 

livros[i].ano, 

livros[i].exemplaresdisponiveis); 

} 

fclose(arquivo); 

} else { 

printf("ERRO: Nao foi possivel abrir o arquivo de livros para salvar.\n"); 

} 

// Para usuarios 

arquivo = fopen("usuario.txt","w"); 

if (arquivo != NULL) { 

fprintf(arquivo, "%d\n", totalusuarios); 

int i; 

for (i=0;i<totalusuarios;i++) { 

// Formato: matricula,nome,curso,telefone,dia,mes,ano 

fprintf(arquivo, "%d,%s,%s,%s,%d,%d,%d\n" 

, 

usuarios[i].matricula, 

usuarios[i].nome , 

usuarios[i].curso, 

usuarios[i].telefone, 

usuarios[i].data_cadastro.dia, 

usuarios[i].data_cadastro.mes, 

usuarios[i].data_cadastro.ano); 

} 

fclose(arquivo); 

} else { 

printf("ERRO: Nao foi possivel abrir o arquivo de usuarios para salvar.\n"); 

} 

// Para emprestimos 

arquivo = fopen("emprestimo.txt","w"); 

if (arquivo != NULL) { 

fprintf(arquivo, "%d\n", totalemprestimos); 

int i; 

for (i=0;i<totalemprestimos;i++) { 

// Formato:codigo_e,matricula,codigo_l,dia_e,mes_e,ano_e,dia_d,mes_d,ano_d,status 

fprintf(arquivo, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%c\n", 

emprestimos[i].codigo, 

emprestimos[i].matricula , 

emprestimos[i].codigolivro, 

//Do emprestimo 

emprestimos[i].dia, 

emprestimos[i].mes, 

emprestimos[i].ano, 

//Parte de devolucao 

emprestimos[i].diaD, 

emprestimos[i].mesD, 

emprestimos[i].anoD, 

emprestimos[i].status); 

} 

fclose(arquivo); 

} else { 

printf("ERRO: Nao foi possivel abrir o arquivo de emprestimos para salvar.\n"); 

} 

} 

// DEFINICAO DA FUNCAO CARREGAR_TUDO 

void carregar_tudo() { 

FILE *arquivo; 

int i; 

// Carregar Livros 

arquivo = fopen("livros.txt", "r"); 

if (arquivo != NULL) { 

if (fscanf(arquivo, "%d\n", &totallivros) == 1) { 

for (i = 0; i < totallivros && i < livrosmax; i++) { 

// Usando %[^\n] ou outros delimitadores para ler strings com espacos, 

// mas vou usar o formato mais robusto para CSV(separado por virgula). 

if (fscanf(arquivo, "%d,%[^,],%[^,],%[^,],%d,%d\n" 

, 

&livros[i].codigo, 

livros[i].titulo, 

livros[i].autor, 

livros[i].editora, 

&livros[i].ano, 

&livros[i].exemplaresdisponiveis) != 6) { 

printf("AVISO: Erro ao ler dados de livro na linha %d. Carregamento interrompido.\n", i + 2); 

totallivros = i; 

break; 

} 

} 

} 

fclose(arquivo); 

} 

// Carregar Usuarios 

arquivo = fopen("usuario.txt", "r"); 

if (arquivo != NULL) { 

if (fscanf(arquivo, "%d\n", &totalusuarios) == 1) { 

for (i = 0; i < totalusuarios && i < usuariosmax; i++) { 

if (fscanf(arquivo, 

"%d,%[^,],%[^,],%[^,],%d,%d,%d\n" 

, 

&usuarios[i].matricula, 

usuarios[i].nome, 

usuarios[i].curso, 

usuarios[i].telefone, 

&usuarios[i].data_cadastro.dia, 

&usuarios[i].data_cadastro.mes, 

&usuarios[i].data_cadastro.ano) != 7) { 

printf("AVISO: Erro ao ler dados de usuario na linha %d. Carregamento interrompido.\n", i + 2); 

totalusuarios = i; 

break; 

} 

} 

} 

fclose(arquivo); 

} 

// Carregar Emprestimos 

arquivo = fopen("emprestimo.txt", "r"); 

if (arquivo != NULL) { 

if (fscanf(arquivo, "%d\n", &totalemprestimos) == 1) { 

for (i = 0; i < totalemprestimos && i < emprestimomax; 

i++) { 

if (fscanf(arquivo, 

"%d,%d,%d,%d,%d,%d,%d,%d,%d,%c\n", 

&emprestimos[i].codigo, 

&emprestimos[i].matricula, 

&emprestimos[i].codigolivro, 

&emprestimos[i].dia, 

&emprestimos[i].mes, 

&emprestimos[i].ano, 

&emprestimos[i].diaD, 

&emprestimos[i].mesD, 

&emprestimos[i].anoD, 

&emprestimos[i].status) != 10) { 

printf("AVISO: Erro ao ler dados de emprestimo na linha %d. Carregamento interrompido.\n", i + 2); 

totalemprestimos = i; 

break; 

} 

} 

} 

fclose(arquivo); 

} 

} 

// Implementacao das funcoes de livro 

void cadastrarlivro() { 

if (totallivros == livrosmax) { 

printf("ERRO: Max de livros atingidos (%d).\n", livrosmax); 

return ; 

} 

int tabela = totallivros; 

printf("\n--- NOVO LIVRO ---\n"); 

livros[tabela].codigo = tabela + 1; 

printf("ID do livro: %d\n", livros[tabela].codigo); 

ler_string(livros[tabela].titulo, sizeof(livros[tabela].titulo), 

"Titulo: "); 

ler_string(livros[tabela].autor, sizeof(livros[tabela].autor), 

"Autor: "); 

ler_string(livros[tabela].editora, 

sizeof(livros[tabela].editora), "Editora: "); 

printf("Ano de publicacao: "); 

livros[tabela].ano = ler_inteiro(); 

printf("Total de exemplares disponiveis: "); 

livros[tabela].exemplaresdisponiveis = ler_inteiro(); 

totallivros++; 

printf("\n\n\t\t\t \n Livro '%s' cadastrado com sucesso! Totallivros: %d\n\t\t\t\t", livros[tabela].titulo, totallivros); 

} 

void mostrar_livros() { 

printf("\n--- Lista de todos os livros (%d no total) ---\n"); 

    int i; 

    for (i=0;i<totallivros;i++) { 

        printf("%-5d | %-30.30s |%-20.20s | %5d | %-10d\n" 

        , 

        livros[i].codigo,livros[i].titulo,livros[i].autor, 

        livros[i].ano,livros[i].exemplaresdisponiveis); 

    } 

    printf("\n"); 

} 

void pesquisar_livro() { 

    char termo[100]; 

    printf("\n--- PESQUISAR LIVRO ---\n"); 

    limpar_buffer(); 

    ler_string(termo, sizeof(termo), "Digite o titulo: "); 

    printf("\nResultados:\n"); 

    printf("----------------------------------------------------------------------------------------\n"); 

    int encontrado = 0; 

    int i; 

    for (i = 0; i < totallivros; i++) { 

        // strstr retorna um ponteiro se encontrar a substring 

        // O termo de pesquisa ao case sensitive, pode ser melhorado com a funcao strcasestr 

        if (strstr(livros[i].titulo, termo) != NULL) { 

            printf("%d - %s (Autor: %s) | Disp: %d\n" 

            , 

            livros[i].codigo, livros[i].titulo, livros[i].autor, 

            livros[i].exemplaresdisponiveis); 

            encontrado = 1; 

        } 

    } 

if (!encontrado) { 

        printf("Nenhum livro encontrado com o termo '%s'.\n" 

        , 

        termo); 

    } 

} 

// Implementacao das funcoes de usuarios 

void cadastrarusuario() { 

    if (totalusuarios == usuariosmax) { 

        printf("ERRO: Max de usuarios atingido (%d).\n" 

        , 

        usuariosmax); 

        return; 

    } 

    int id = totalusuarios; 

    printf("\n--- CADASTRAR USUARIO ---\n"); 

    usuarios[id].matricula = id + 1; 

    printf("Matricula gerada: %d\n", usuarios[id].matricula); 

    limpar_buffer(); 

    // Usando a funcao correta para ler strings 

    ler_string(usuarios[id].nome, sizeof(usuarios[id].nome), "Nome completo: "); 

    ler_string(usuarios[id].curso, sizeof(usuarios[id].curso), 

    "Curso: "); 

    ler_string(usuarios[id].telefone, sizeof(usuarios[id].telefone), 

    "Telefone: "); 

    cadastrardata(&usuarios[id].data_cadastro,"Data de cadastro"); 

    totalusuarios++; 

    printf("\nUsuario '%s' cadastrado com sucesso! Total usuarios:%d\n", usuarios[id].nome, totalusuarios); 

} 

void mostrarusuario() { 

    printf("\n--- LISTA DE USUARIOS (%d cadastrados) ---\n" 

    , 

    totalusuarios); 

    if (totalusuarios == 0) { 

        printf("Nenhum usuario cadastrado.\n"); 

        return; 

    } 

    printf("%-5s | %-30s | %-15s | %-12s | %-10s\n" 

    , 

    "ID", "NOME", "CURSO", "TELEFONE", "DATA"); 

    printf("----------------------------------------------------------------------------------------\n"); 

    int i; 

    for (i = 0; i < totalusuarios; i++) { 

        printf("%-5d | %-30.30s | %-15.15s | %-12.12s |%02d/%02d/%d\n" 

        , 

        usuarios[i].matricula, 

        usuarios[i].nome, 

        usuarios[i].curso, 

        usuarios[i].telefone, 

        usuarios[i].data_cadastro.dia, 

        usuarios[i].data_cadastro.mes, 

        usuarios[i].data_cadastro.ano); 

    } 

} 

void pesquisar_usuario() 

{ 

    char termo[100]; 

    printf("\n--- PESQUISAR USUARIO ---\n"); 

    limpar_buffer(); 

    ler_string(termo, sizeof(termo), "Nome para pesquisar: "); 

    printf("\nResultados:\n"); 

    printf("----------------------------------------------------------------------------------------\n"); 

    int encontrado = 0; 

    int i; 

    for (i = 0; i < totalusuarios; i++) { 

        // strstr ÃƒÂ© case sensitive 

        if (strstr(usuarios[i].nome, termo) != NULL) { 

            printf("%d - %s (Curso: %s) | Tel: %s\n" 

            , 

            usuarios[i].matricula, usuarios[i].nome, usuarios[i].curso, 

            usuarios[i].telefone); 

            encontrado = 1; 

        } 

    } 

    if (!encontrado) { 

        printf("Nenhum usuario encontrado com o termo '%s'.\n" 

        , 

        termo); 

    } 

} 

int buscar_usuario_por_matricula(int matricula) { 

    int i; 

    for ( i = 0; i < totalusuarios; i++) { 

        if (usuarios[i].matricula == matricula) { 

            return i; 

        } 

    } 

    return -1; 

} 

int buscar_livro_por_codigo(int codigo) { 

    int i; 

    for ( i = 0; i < totallivros; i++) { 

        if (livros[i].codigo == codigo) { 

            return i; 

        } 

    } 

    return -1; 

} 

// Implementacao das funcoes de emprestimos 

//Funcao de emprestimo com validacao completa para nao aceitar livros e usuarios inexistentes 

void cadastraremprestimo() { 

    if (totalemprestimos == emprestimomax) { 

        printf("ERRO: Maximo de emprestimos atingido.\n"); 

        return; 

    } 

    int id = totalemprestimos; 

    printf("\n--- FAZER EMPRESTIMO ---\n"); 

    // Validacao do usuario 

    int matricula; 

    int idx_usuario; 

    do { 

        printf("Matricula do usuario: "); 

        matricula = ler_inteiro(); 

        idx_usuario = buscar_usuario_por_matricula(matricula); 

        if (idx_usuario == -1) { 

            printf("ERRO: Usuario com matricula %d nao encontrado!Cadastre primeiro.\n", matricula); 

        } 

} while (idx_usuario == -1); 

    // Validacao do livro e o estoque 

    int codigolivro; 

    int idx_livro; 

    do { 

        printf("Codigo do livro: "); 

        codigolivro = ler_inteiro(); 

        idx_livro = buscar_livro_por_codigo(codigolivro); 

        if (idx_livro == -1) { 

            printf("ERRO: Livro com codigo %d nao encontrado!\n" 

            , 

            codigolivro); 

        } else if (livros[idx_livro].exemplaresdisponiveis <= 0) { 

            printf("ERRO: Livro '%s' sem exemplares disponiveis no momento!\n", livros[idx_livro].titulo); 

        } 

    } while (idx_livro == -1 || 

    livros[idx_livro].exemplaresdisponiveis <= 0); 

    // Tudo validado ? prossegue 

    emprestimos[id].matricula = matricula; 

    emprestimos[id].codigolivro = codigolivro; 

    emprestimos[id].codigo = id + 1; 

    Data data_emprestimo; 

    cadastrardata(&data_emprestimo, "Data do emprestimo"); 

    emprestimos[id].dia = data_emprestimo.dia; 

    emprestimos[id].mes = data_emprestimo.mes; 

    emprestimos[id].ano = data_emprestimo.ano; 

    Data data_devolucao = adicionar_dias(data_emprestimo, 7); 

    emprestimos[id].diaD = data_devolucao.dia; 

    emprestimos[id].mesD = data_devolucao.mes; 

    emprestimos[id].anoD = data_devolucao.ano; 

    emprestimos[id].status = 1; 

    livros[idx_livro].exemplaresdisponiveis--; // ? diminui estoque 

    totalemprestimos++; 

    printf("\nSUCESSO! Emprestimo COD %d registrado!\n" 

    , 

    emprestimos[id].codigo); 

    printf("Livro: %s\n", livros[idx_livro].titulo); 

    printf("Usuario: %s\n", usuarios[idx_usuario].nome); 

    printf("Devolucao prevista: %02d/%02d/%d\n\n" 

    , 

    emprestimos[id].diaD, emprestimos[id].mesD, emprestimos[id].anoD); 

} 

// FUNCAO DE DEVOLUCAO DE EMPReSTIMO COM VALIDACAO COMPLETA (NUNCA VAI ACEITAR LIVRO OU USUARIO INEXISTENTE) 

void armazenar_devolucao() { 

    if (totalemprestimos == 0) { 

        printf("\nNenhum emprestimo registrado.\n"); 

        return; 

    } 

    printf("\n--- REGISTRAR DEVOLUCAO ---\n"); 

    printf("Codigo do emprestimo: "); 

    int codigo = ler_inteiro(); 

    int i; 

    for (i = 0; i < totalemprestimos; i++) { 

        if (emprestimos[i].codigo == codigo && emprestimos[i].status 

        == 1) { 

            // Busca nome do usuario e livro 

            int idx_usuario = 

            buscar_usuario_por_matricula(emprestimos[i].matricula); 

            int idx_livro = 

            buscar_livro_por_codigo(emprestimos[i].codigolivro); 

            emprestimos[i].status = 0; 

            livros[idx_livro].exemplaresdisponiveis++; 

            printf("\nDEVOLUCAO REGISTRADA COM SUCESSO!\n"); 

printf("Emprestimo: %d\n", emprestimos[i].codigo); 

printf("Usuario: %s\n", idx_usuario != -1 ? 

usuarios[idx_usuario].nome : "Desconhecido"); 

printf("Livro: %s\n", livros[idx_livro].titulo); 

printf("Exemplares disponiveis agora: %d\n\n" 

, 

livros[idx_livro].exemplaresdisponiveis); 

return; 

} 

} 

printf("Emprestimo nao encontrado ou ja¡ devolvido.\n\n"); 

} 

void registrar_emprestimos_ativos() { 

printf("\n --- EMPRESTIMOS ATIVOS --- \n "); 

if (totalemprestimos == 0) { 

printf("Nenhum emprestimo registrado ainda. \n "); 

return; 

} 

int escolha = 0; 

int i; 

for (i = 0; i < totalemprestimos; i++) { 

if (emprestimos[i].status == 1) { 

printf("Codigo Emprestimo: %d | livro: %d | usuario: %d | Devolucao prevista: %02d/%02d/%d \n " 

, 

emprestimos[i].codigo, 

emprestimos[i].codigolivro, 

emprestimos[i].matricula, 

emprestimos[i].diaD,emprestimos[i].mesD,emprestimos[i].anoD); 

escolha = 1; 

} 

} 

if (!escolha) { 

printf("Nenhum emprestimo ativo no momento. \n "); 

} 

printf("\n"); 

} 

void pesq_livro_por_codigo() { //pesquisa dos livros disponiveis e registrados 

printf("Digite o codigo do livro: "); 

int codigo = ler_inteiro(); 

int i; 

for (i = 0; i < totallivros; i++) { 

if (livros[i].codigo == codigo) { 

printf("\n Livro encontrado \n"); 

printf("Titulo: %sAutor: %sEditora: %sAno: %d\n Disponiveis: %d\n" 

, 

livros[i].titulo, 

livros[i].autor,livros[i].editora,livros[i].ano,livros[i].exemplaresdisponiveis); 

return; 

} 

} 

printf("Livro com codigo %d nao encontrado! Registro primeiro.\n", codigo); 

} 

//Pesquisa detalhada do livro 

void pesquisa_detalhada_livro() { 

int opcao, codigo, encontrado, i = 0; 

char termo[100]; 

printf("\n--- PESQUISA DETALHADA DO LIVRO ---\n"); 

printf("1. Por codigo\n2. Por titulo\n3. Por autor\nEscolha: "); 

opcao = ler_inteiro(); 

switch (opcao) { 

case 1: 

printf("Digite o codigo: "); 

codigo = ler_inteiro(); 

int i; 

for (i = 0; i < totallivros; i++) { 

if (livros[i].codigo == codigo) { 

printf("\n>>> LIVRO ENCONTRADO <<<\n"); 

printf("Codigo: %d\nTitulo: %s\nAutor:%s\nEditora: %s\nAno: %d\nDisponiveis:" 

       " %d\n\n", 

       livros[i].codigo, livros[i].titulo, 

livros[i].autor, 

livros[i].editora, livros[i].ano, 

livros[i].exemplaresdisponiveis); 

    encontrado = 1; 

    break; 

} 

} 

        if (!encontrado) printf("Livro nao encontrado!\n"); 

        break; 

    case 2: 

    case 3: 

        limpar_buffer(); 

        ler_string(termo, sizeof(termo), opcao == 2 ? "Digite o titulo: " : "Digite o nome autor: "); 

        printf("\n>>> RESULTADOS <<<\n"); 

        for (i = 0; i < totallivros; i++) { 

            if ((opcao == 2 && strstr(livros[i].titulo, termo)) 

            || 

            (opcao == 3 && strstr(livros[i].autor, termo))) 

            { 

                printf("COD %d | %s - %s (%d) | Disp: %d\n" 

                , 

                livros[i].codigo, livros[i].titulo, 

                livros[i].autor, 

                livros[i].ano, 

                livros[i].exemplaresdisponiveis); 

                encontrado = 1; 

            } 

        } 

        if (!encontrado) printf("Nenhum livro encontrado com esse termo.\n"); 

        break; 

    default: 

        printf("Opcao invalida!\n"); 

} 

    printf("\n"); 

} 

// Emprestimos em atraso(detalhado) 

void emprestimos_em_atraso() { 

    printf("\n --- EMPRESTIMOS EM ATRASO --- \n"); 

    //Aqui vai pegar a data atual e mostrar os atrasados 

    time_t t = time(NULL); 

    struct tm *atual = localtime(&t); 

    int hoje_dia = atual->tm_mday; 

    int hoje_mes = atual->tm_mon + 1; // tm_mon vai de 0 a 11 

    int hoje_ano = atual->tm_year + 1900; // tm_year e anos desde 1900 

    printf("Data atual do sistema: %02d/%02d/%d\n", hoje_dia,hoje_mes, hoje_ano); 

int encontrado = 0; 

int i; 

for (i = 0; i < totalemprestimos; i++) { 

if (emprestimos[i].status == 1) { // ativo 

int atrasado = 0; 

if (emprestimos[i].anoD < hoje_ano) atrasado = 1; 

else if (emprestimos[i].anoD == hoje_ano) { 

if (emprestimos[i].mesD < hoje_mes) atrasado = 1; 

else if (emprestimos[i].mesD == hoje_mes && 

emprestimos[i].diaD < hoje_dia) atrasado = 1; 

} 

if (atrasado) { 

printf("ATRASADO! Cod: %d | Livro: %d | Usuario: %d| Previsto: %02d/%02d/%d\n" 

, 

emprestimos[i].codigo, 

emprestimos[i].codigolivro, emprestimos[i].matricula, 

emprestimos[i].diaD, emprestimos[i].mesD, 

emprestimos[i].anoD); 

encontrado = 1; 

} 

} 

} 

if (!encontrado) { 

printf("Nenhum emprestimo em atraso. Todos em dia!\n"); 

} 

printf("\n"); 

} 

//Parte do relatorio dos livros mais emprestados 

void livros_mais_emprestados() { 

printf("\n --- RELATORIO: LIVROS MAIS EMPRESTADOS ---\n"); 

if (totallivros == 0) { 

printf("Nenhum livro cadastrado ainda.\n"); 

return; 

} 

int contagem[100] = {0}; 

int indices[100]; 

// Inicializa os Ã­ndices 

for (int i = 0; i < totallivros; i++) { 

indices[i] = i; 

} 

// Conta quantas vezes cada livro foi emprestado 

for (int i = 0; i < totalemprestimos; i++) { 

for (int j = 0; j < totallivros; j++) { 

if (livros[j].codigo == emprestimos[i].codigolivro) { 

contagem[j]++; 

break; 

} 

} 

} 

// Ordena os Ã­ndices (NAO mexe no array de livros!) 

for (int i = 0; i < totallivros - 1; i++) { 

for (int j = i + 1; j < totallivros; j++) { 

if (contagem[indices[j]] > contagem[indices[i]]) { 

int temp = indices[i]; 

indices[i] = indices[j]; 

indices[j] = temp; 

} 

} 

} 

// Exibe o ranking 

printf("%-5s | %-30s | %-20s | %s\n" 

, "RANK", "TITULO", "AUTOR", 

"VEZES"); 

printf("-----------------------------------------------------------------\n"); 

int rank = 1; 

int mostrou_algum = 0; 

for (int i = 0; i < totallivros && rank <= 10; i++) { 

int idx = indices[i]; 

if (contagem[idx] > 0) { 

printf("%-5d | %-30.30s | %-20.20s | %d vez(es)\n" 

, 

rank++, livros[idx].titulo, livros[idx].autor, 

contagem[idx]); 

mostrou_algum = 1; 

} 

} 

if (!mostrou_algum) { 

printf("Nenhum livro foi emprestado ainda.\n"); 

} 

printf("\n"); 

} 

void busca_avancada_livros() { 

char titulo[100], autor[100], editora[100]; 

int ano, codigo, usar_codigo = 0, usar_ano = 0; 

int usar_titulo = 0, usar_autor = 0, usar_editora = 0, 

usar_status = 0; 

int status_desejado; // 1 = disponivel, 0 = emprestado 

printf("\n --- BUSCA AVANCADA ---\n"); 

printf("Digite os criterios desejados. Para ignorar um campo,deixe vazio.\n\n"); 

fflush(stdin); 

printf("Titulo: "); 

fgets(titulo, 100, stdin); strtok(titulo, "\n"); 

if (strlen(titulo) > 0) usar_titulo = 1; 

printf("Autor: "); 

fgets(autor, 100, stdin); strtok(autor, "\n"); 

if (strlen(autor) > 0) usar_autor = 1; 

printf("Editora: "); 

fgets(editora, 100, stdin); strtok(editora, "\n"); 

if (strlen(editora) > 0) usar_editora = 1; 

printf("Ano (0 para ignorar): "); 

scanf("%d", &ano); 

if (ano != 0) usar_ano = 1; 

printf("Codigo (0 para ignorar): "); 

scanf("%d", &codigo); 

if (codigo != 0) usar_codigo = 1; 

limpar_buffer(); 

printf("Status (D = disponivel, E = emprestado, I = ignorar):"); 

scanf("%c", &status_desejado); 

if (status_desejado == 'D' || status_desejado == 'E') 

usar_status = 1; 

else 

usar_status = 0; 

printf("\n --- RESULTADOS ---\n"); 

int encontrou = 0; 

int i; 

for ( i = 0; i < totallivros; i++) { 

// ---- FILTROS ---- 

if (usar_titulo && strstr(livros[i].titulo, titulo) == NULL) 

continue; 

if (usar_autor && strstr(livros[i].autor, autor) == NULL) 

continue; 

if (usar_editora && strstr(livros[i].editora, editora) == 

NULL) 

continue; 

if (usar_ano && livros[i].ano != ano) 

continue; 

if (usar_codigo && livros[i].codigo != codigo) 

continue; 

if (usar_status) { 

if (status_desejado == 'D' && 

livros[i].exemplaresdisponiveis <= 0) continue; 

if (status_desejado == 'E' && 

livros[i].exemplaresdisponiveis > 0) { 

continue; 

} 

} 

// SE PASSOU POR TODOS OS FILTROS, MOSTRA : 

printf("\nCodigo: %d\nTitulo: %s\nAutor: %s\nEditora:%s\nAno: %d\nStatus: %s\n" 

, 

livros[i].codigo, 

livros[i].titulo, 

livros[i].autor, 

livros[i].editora, 

livros[i].ano, 

livros[i].exemplaresdisponiveis >0 ? "Disponivel" : "Todos emprestados"); 

encontrou = 1; 

} 

if (!encontrou) { 

printf("\nNenhum livro encontrado com esses criterios.\n"); 

} 

} 

// RENOVACAO DE EMPRESTIMO 

void renovar_emprestimo() { 

if (totalemprestimos == 0) { 

printf("\nNenhum emprestimo registrado ainda.\n"); 

    return; 

} 

printf("\n--- RENOVAR EMPRESTIMO ---\n"); 

printf("Digite o codigo do emprestimo ativo que deseja renovar:"); 

int codigo = ler_inteiro(); 

int i; 

for (i = 0; i < totalemprestimos; i++) { 

if (emprestimos[i].codigo == codigo && emprestimos[i].status 

== 1) { // ativo 

// Busca dados do usuario e livro para exibir 

int idx_usuario = 

buscar_usuario_por_matricula(emprestimos[i].matricula); 

int idx_livro = 

buscar_livro_por_codigo(emprestimos[i].codigolivro); 

printf("\nEmprestimo encontrado!\n"); 

printf("Livro: %s\n", livros[idx_livro].titulo); 

printf("Usuario: %s\n", usuarios[idx_usuario].nome); 

printf("Devolucao atual: %02d/%02d/%d\n" 

, 

emprestimos[i].diaD, emprestimos[i].mesD, 

emprestimos[i].anoD); 

// Nova data de devolucao = +7 dias da data prevista atual 

Data nova_data; 

nova_data.dia = emprestimos[i].diaD; 

nova_data.mes = emprestimos[i].mesD; 

nova_data.ano = emprestimos[i].anoD; 

Data renovada = adicionar_dias(nova_data, 7); 

// Atualiza no vetor 

emprestimos[i].diaD = renovada.dia; 

emprestimos[i].mesD = renovada.mes; 

emprestimos[i].anoD = renovada.ano; 

printf("RENOVACAO REALIZADA COM SUCESSO!\n"); 

printf("Nova data de devolucao: %02d/%02d/%d\n\n" 

, 

renovada.dia, renovada.mes, renovada.ano); 

return; 

} 

} 

printf("Emprestimo nao encontrado ou ja devolvido.\n\n"); 

} 

// FUNCOES DE MENU 

int menu_principal() { 

int escolha; 

printf("\n\n\t\t\t --- Sistema de Gerenciamento de Biblioteca --- \n\t\t\t\t"); 

printf("\n 1. Cadastrar Livro \n "); 

printf("\n 2. Cadastrar Usuario \n"); 

printf("\n 3. Cadastrar Emprestimo \n"); 

printf("\n 4. Restauracao automatica (Carregar dados) \n"); 

printf("\n 0. Sair \n "); 

printf("\n Escolha: "); 

escolha = ler_inteiro(); 

switch (escolha) { 

case 1: menu_livros(); break; 

case 2: menu_usuario(); break; 

case 3: menu_emprestimo(); break; 

case 4: 

carregar_tudo(); 

printf("\n Restauracao automatica de dados concluida.\n"); 

break; 

case 0: 

printf("\n Salvando dados e saindo...\n"); 

break; 

default : printf("\n Escolha invalida\n"); break; 

} 

return escolha; 

} 

void menu_livros() { 

int escolha; 

do { 

printf("\n\n\t\t\t --- CADASTRAR LIVRO ---\n\t\t\t\t"); 

printf("\n1. Cadastrar novo livro\n"); 

printf("\n2. Mostrar lista de todos os livros\n"); 

printf("\n3. Pesquisar livro (por titulo)\n"); 

printf("\n4. Pesquisa detalhada \n"); 

printf("\n5. Busca avancada \n"); 

printf("\n0. Voltar pro menu principal\n"); 

printf("\n Escolha: "); 

escolha = ler_inteiro(); 

switch (escolha) { 

case 1: cadastrarlivro(); break; 

case 2: mostrar_livros(); break; 

case 3: pesquisar_livro(); break; 

case 4:pesquisa_detalhada_livro(); break; 

case 5:busca_avancada_livros();break; 

case 0: printf("\n Voltando pro menu principal...\n"); break; 

default : printf("\n Escolha invalida \n"); break; 

} 

} while (escolha != 0); 

} 

void menu_usuario() { 

int escolha; 

do { 

printf("\n\n\t\t\t --- CADASTRAR USUARIO ---\n\t\t\t\t"); 

printf("\n1. Cadastrar novo usuario\n"); 

printf("\n2. Mostrar lista de usuarios\n"); 

printf("\n3. Pesquisar usuario por nome\n"); 

printf("\n4. Atualizar cadastro \n"); 

printf("\n0. Voltar \n"); 

printf("\n Escolha: "); 

escolha = ler_inteiro(); 

switch (escolha) { 

case 1: cadastrarusuario(); break; 

case 2: mostrarusuario(); break; 

case 3: pesquisar_usuario(); break; 

case 4: printf("\n Atualizacao de cadastro \n"); 

break; 

case 0 : printf("\n Voltando... \n"); break; 

default : printf("Escolha invalida \n"); break; 

} 

} while (escolha != 0); 

} 

void menu_emprestimo() { 

int escolha; 

do { 

printf("\n\n\t\t\t --- CADASTRAR EMPRESTIMO ---\n\t\t\t\t"); 

printf("\n1. Fazer novo emprestimo\n"); 

printf("\n2. Registrar devolucao \n"); 

printf("\n3. Ver emprestimos ativos \n "); 

printf("\n4. Opcoes de emprestimos disponiveis\n"); 

printf("\n5. Consultar emprestimo em atraso\n "); 

printf("\n6. Relatorio de livros mais emprestados \n"); 

printf("\n7.Renovar emprestimo \n"); 

printf("\n0. Voltar\n"); 

printf("\n Escolha: "); 

escolha = ler_inteiro(); 

switch (escolha) { 

case 1: cadastraremprestimo(); break; 

case 2: armazenar_devolucao(); break; 

case 3:registrar_emprestimos_ativos();break; 

case 4: pesq_livro_por_codigo(); break; 

case 5: emprestimos_em_atraso(); printf("\n Ver emprestimo atrasado \n");break; 

case 6: livros_mais_emprestados();break; 

case 7:renovar_emprestimo();break; 

case 0: printf("\n\n\t\t\t Voltando... \n\t\t\t\t"); 

break; 

default : printf("Escolha invalida \n"); break; 

} 

} while (escolha != 0); 

} 

// Funcao Principal para carregar e salvar os dados 

int main() { 

carregar_tudo(); // Carrega os dados salvos ao iniciar 

printf("Dados carregados. Livros: %d | Usuarios: %d |Emprestimos: %d\n", totallivros, totalusuarios, totalemprestimos); 

int escolha; 

do { 

escolha = menu_principal(); 

} while (escolha != 0); 

salvar_tudo(); // Salva no final antes de sair 

printf("Dados salvos com sucesso.\n "); 

return 0; 

} 

 

 

 

 