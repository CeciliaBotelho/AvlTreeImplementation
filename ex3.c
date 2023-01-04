#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef int Chave;

typedef struct {
    Chave chave;
} Registro;

typedef struct Node * Ponteiro;

typedef struct Node {
    Registro reg;
    Ponteiro left, right;
} Node;

int count_nodes(Ponteiro ponteiro){
    if (!ponteiro) {
        return 0;
    }

    return 1 + count_nodes(ponteiro->left) + count_nodes(ponteiro->right);
}

int height(Ponteiro ponteiro){
    int left_height = 0, right_height = 0;
    if (!ponteiro){
        return 0;
    }
    left_height = height(ponteiro->left);
    right_height = height(ponteiro->right);
    
    if (left_height > right_height) {
        return 1 + left_height;
    } else {
        return 1 + right_height;
    }
}

int get_balancing_factor(Ponteiro *ponteiro){
    return height((*ponteiro)->left) - height((*ponteiro)->right);
}

void rotacao_esq(Ponteiro *ponteiro_to_root){
    Ponteiro aux;
    aux = (*ponteiro_to_root)->right;
    (*ponteiro_to_root)->right = aux->left;
    aux->left = (*ponteiro_to_root);
    (*ponteiro_to_root) = aux;   
}

void rotacao_dir(Ponteiro *ponteiro_to_root){
    Ponteiro aux;
    aux = (*ponteiro_to_root)->left;
    (*ponteiro_to_root)->left = aux->right;
    aux->right = (*ponteiro_to_root);
    (*ponteiro_to_root) = aux;
}

int balance_left(Ponteiro *ponteiro){
    int left_bf = get_balancing_factor(&(*ponteiro)->left);

    if (left_bf > 0) {
        rotacao_dir(ponteiro);
        return 1;
    } else if (left_bf < 0) {
        rotacao_esq(&((*ponteiro)->left));
        rotacao_dir(ponteiro);
        return 1;
    }
    return 0;
}

int balance_right(Ponteiro *ponteiro){
    int right_bf = get_balancing_factor(&(*ponteiro)->right);

    if (right_bf < 0) {
        rotacao_esq(ponteiro);
        return 1;
    } else if (right_bf > 0) {
        rotacao_dir(&((*ponteiro)->right));
        rotacao_esq(ponteiro);
        return 1;
    }
    return 0;
}

int balance(Ponteiro *ponteiro){
    int root_bf = get_balancing_factor(ponteiro);
    if (root_bf > 1) {
        return balance_left(ponteiro);
    }
    if (root_bf < -1) {
        return balance_right(ponteiro);
    }
    return 0;
}

void search(Registro *reg, Ponteiro ponteiro){
    if (!ponteiro) {
        printf("\nError: Registro not found.\n");
        return;
    }

    if (reg->chave < ponteiro->reg.chave) {
        search(reg, ponteiro->left);
        return;
    }

    if (reg->chave > ponteiro->reg.chave) {
        search(reg, ponteiro->right);
        return;
    }
    
    *reg = ponteiro->reg;
}

int inserir(Registro reg, Ponteiro *ponteiro) {
    if (!*ponteiro) {
        *ponteiro = (Ponteiro) malloc(sizeof(Node));
        (*ponteiro)->reg = reg;
        (*ponteiro)->left = NULL;
        (*ponteiro)->right = NULL;
        return 1;
    }

    if (reg.chave < (*ponteiro)->reg.chave) {
        if (inserir(reg, &(*ponteiro)->left)){
            if (balance(ponteiro)){
                return 0;
            } else {
                return 1;
            }
        } else {
            return 0;
        }
    }

    if (reg.chave > (*ponteiro)->reg.chave) {
        if (inserir(reg, &(*ponteiro)->right)) {
            if (balance(ponteiro)){
                return 0;
            } else {
                return 1;
            }
        } else {
            return 0;
        }
    }

    printf("\nError: Registro %d ja exite.\n", reg.chave);
    return 0;
}

void total_del(Ponteiro to_remove, Ponteiro *to_replace){
    Ponteiro aux;

    if ((*to_replace)->right) {
        total_del(to_remove, &(*to_replace)->right);
        return;
    }

    to_remove->reg = (*to_replace)->reg;
    aux = *to_replace;
    *to_replace = (*to_replace)->left;
    free(aux);
}

int delete(Registro reg, Ponteiro *ponteiro) {
    Ponteiro aux;

    if (!*ponteiro) {
        printf("Error: Registro %d not found.", reg.chave);
        return 0;
    }

    if (reg.chave < (*ponteiro)->reg.chave){
        if (delete(reg, &(*ponteiro)->left)){
            balance(ponteiro);
            return 1;
        } else {
            return 0;
        }
    }

    if (reg.chave > (*ponteiro)->reg.chave) {
        if (delete(reg, &(*ponteiro)->right)){
            balance(ponteiro);
            return 1;
        } else {
            return 0;
        }
    }

    if (!(*ponteiro)->right) {
        aux = *ponteiro;
        *ponteiro = (*ponteiro)->left;
        free(aux);
        return 1;
    }

    if (!(*ponteiro)->left) {
        aux = *ponteiro;
        *ponteiro = (*ponteiro)->right;
        free(aux);
        return 1;
    }

    total_del(*ponteiro, &(*ponteiro)->left);
    return 1;
}

void print_tree(Ponteiro *ponteiro) {
    if (*ponteiro){
        printf("(%d", (*ponteiro)->reg.chave);
        print_tree(&(*ponteiro)->left);
        print_tree(&(*ponteiro)->right);
        printf(")");
    }
}

void initialize(Ponteiro *dictionary){
    *dictionary = NULL;
}

void reset_tree(Ponteiro ponteiro){
    if (!ponteiro){
        return;
    }

    reset_tree(ponteiro->left);
    reset_tree(ponteiro->right);
    free(ponteiro);
}

int standard_avl_verification(Ponteiro ponteiro){
    int bf = 0;

    if (!ponteiro) {
        return 1;
    }

    if (!standard_avl_verification(ponteiro->left)){return 0;}
    if (!standard_avl_verification(ponteiro->right)){return 0;}

    bf = get_balancing_factor(&ponteiro);
    
    if ((bf > 1) || (bf < -1)){
        return 0;
    } else {
        return 1;
    }
}

int calc_avl_verification(int nodes_qty, int tree_height){
    if (((1.44 * log2f((float) nodes_qty + 2)) + 1.0) > (float) tree_height){
        return 1;
    } else {
        return 0;
    }
}

void print_tree_status(Ponteiro *tree){
    printf("\nArvore: ");
    print_tree(tree);
    printf("\n-> Verificacao da AVL:\t%d", standard_avl_verification(*tree));
    printf("\n-> Calculo verififcacao AVL:\t%d", calc_avl_verification(count_nodes(*tree), height(*tree)));
    printf("\nTudo certo = 1; Cuidado, algo de errado= 0\n");
}

void primeiro_cen(){
    int node_qty = 0;
    Registro reg;
    Ponteiro *tree = (Ponteiro *) malloc(sizeof(Ponteiro));
    srand((unsigned) time(0));
    
    initialize(tree);

    printf("\nPrimeiro cenario\n");

    printf("\nEntre com o numero de nos: ");
    scanf("%d", &node_qty);

    for (int i = 0; i < node_qty; i++) {
        reg.chave = rand() % 50;
        inserir(reg, tree);
    }

    print_tree_status(tree);

    reset_tree(*tree);
    free(tree);
}

void segundo_cen(){
    Registro reg;
    Ponteiro *tree = (Ponteiro *) malloc(sizeof(Ponteiro));

    initialize(tree);

    printf("Segundo cenario\n");

    reg.chave = 90;
    inserir(reg, tree);
    print_tree_status(tree);
    reg.chave = 55;
    inserir(reg, tree);
    print_tree_status(tree);
    printf("\nRotacao a direita\n");
    reg.chave = 42;
    inserir(reg, tree);
    print_tree_status(tree);
    reg.chave = 47;
    inserir(reg, tree);
    print_tree_status(tree);
    printf("\nRotacao a esquerda\n");
    reg.chave = 50;
    inserir(reg, tree);
    print_tree_status(tree);
    reg.chave = 42;
    delete(reg, tree);
    print_tree_status(tree);
    printf("\nRotacao dupla a direita\n");
    reg.chave = 90;
    delete(reg, tree);
    print_tree_status(tree);
    reg.chave = 52;
    inserir(reg, tree);
    print_tree_status(tree);
    printf("\nRotacao dupla a esquerda\n");
    reg.chave = 47;
    delete(reg, tree);
    print_tree_status(tree);
    
    reset_tree(*tree);
    free(tree);
}

int main(int argc, char const *argv[])
{

    int continuar=1;


    while(continuar != 0)
    {
        printf("1. primeiro caso\n");
        printf("2. segundo caso\n");;
        printf("0. Sair\n");

        scanf("%d", &continuar);
        getc(stdin);

    
        switch (continuar)
        {
            case 1:
                primeiro_cen();
                break;
            

            case 2:
                segundo_cen();
                break;

            case 0:
                printf ("saindo.....");
                break;
        }
    }
    return 0;
}
