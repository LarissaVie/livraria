#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Estrutura da lista encadeada dupla para armazenar os livros */
typedef struct Livro {
    char titulo[100];
    char autor[100];
    float preco;
    int quantidade;
    struct Livro* prox;
    struct Livro* ant;
} Livro;

Livro* head = NULL;

/* Inserir um livro na lista */
void inserirLivro(Livro** head, const char* titulo, const char* autor, float preco, int quantidade) {
    Livro* novo = (Livro*)malloc(sizeof(Livro));
    strcpy(novo->titulo, titulo);
    strcpy(novo->autor, autor);
    novo->preco = preco;
    novo->quantidade = quantidade;
    novo->prox = *head;
    novo->ant = NULL;

    if (*head != NULL) {
        (*head)->ant = novo;
    }
    *head = novo;
}

/* Remover um livro da lista */
void removerLivro(Livro** head, const char* titulo) {
    Livro* temp = *head;
    while (temp != NULL) {
        if (strcmp(temp->titulo, titulo) == 0) {
            if (temp->ant != NULL) {
                temp->ant->prox = temp->prox;
            } else {
                *head = temp->prox;
            }

            if (temp->prox != NULL) {
                temp->prox->ant = temp->ant;
            }
            free(temp);
            return;
        }
        temp = temp->prox;
    }
}

/* Popular o combo box com os livros */
void atualizarComboBox(GtkWidget *combo_box) {
    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(combo_box));  /* Limpar a lista anterior */

    Livro* temp = head;
    while (temp != NULL) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_box), temp->titulo);  /* Adicionar cada título ao combo box */
        temp = temp->prox;
    }
}

/* Função para exibir os livros como "cards" lado a lado no FlowBox */
void atualizarFlowBox(GtkWidget *flowbox) {
    /* Limpar o FlowBox antes de adicionar novos livros */
    GList *children, *iter;

    children = gtk_container_get_children(GTK_CONTAINER(flowbox));
    for (iter = children; iter != NULL; iter = g_list_next(iter)) {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }
    g_list_free(children);

    /* Adicionar os livros no FlowBox como "cards" */
    Livro* temp = head;
    while (temp != NULL) {
        char livro_info[256];
        snprintf(livro_info, sizeof(livro_info), 
                 "Título: %s\nAutor: %s\nPreço: %.2f\nQuantidade: %d", 
                 temp->titulo, temp->autor, temp->preco, temp->quantidade);

        GtkWidget *frame = gtk_frame_new(NULL);  /* Para simular o card */
        GtkWidget *label = gtk_label_new(livro_info);
        gtk_container_add(GTK_CONTAINER(frame), label);
        gtk_widget_set_margin_top(frame, 10);
        gtk_widget_set_margin_bottom(frame, 10);
        gtk_widget_set_margin_start(frame, 10);
        gtk_widget_set_margin_end(frame, 10);

        gtk_flow_box_insert(GTK_FLOW_BOX(flowbox), frame, -1);  /* Adicionar o "card" de livro no FlowBox */
        temp = temp->prox;
    }

    gtk_widget_show_all(flowbox);  /* Garantir que o FlowBox seja exibido corretamente */
}

/* Função para validar entradas numéricas */
static void validar_entrada_numerica(GtkEditable *editable, const gchar *text, gint length, gint *position, gpointer data) {
    for (int i = 0; i < length; i++) {
        if (!g_ascii_isdigit(text[i]) && text[i] != '.') {  /* Permitir apenas números e ponto para decimais */
            g_signal_stop_emission_by_name(editable, "insert-text");
            return;
        }
    }
}

/* Função para buscar livro pelo título */
Livro* buscarLivro(Livro* head, const char* titulo) {
    Livro* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->titulo, titulo) == 0) {
            return temp;
        }
        temp = temp->prox;
    }
    return NULL;
}

/* Função de clique para inserir o livro */
static void on_inserir_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;

    const gchar *titulo = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const gchar *autor = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const gchar *preco_str = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    const gchar *quantidade_str = gtk_entry_get_text(GTK_ENTRY(entries[3]));

    if (strlen(titulo) == 0 || strlen(autor) == 0 || strlen(preco_str) == 0 || strlen(quantidade_str) == 0) {
        gtk_label_set_text(GTK_LABEL(entries[4]), "Erro: Todos os campos devem ser preenchidos.");
        return;
    }

    float preco = atof(preco_str);
    int quantidade = atoi(quantidade_str);

    inserirLivro(&head, titulo, autor, preco, quantidade);

    gtk_label_set_text(GTK_LABEL(entries[4]), "Livro inserido com sucesso!");

    /* Atualizar o combo box e o flowbox após inserir o livro */
    atualizarComboBox(entries[5]);
    atualizarFlowBox(entries[6]);  /* Exibir os livros automaticamente */
}

/* Função de clique para remover o livro */
static void on_remover_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;

    const gchar *titulo_selecionado = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(entries[5]));
    
    if (titulo_selecionado == NULL) {
        gtk_label_set_text(GTK_LABEL(entries[4]), "Erro: Nenhum livro selecionado.");
        return;
    }

    removerLivro(&head, titulo_selecionado);

    gtk_label_set_text(GTK_LABEL(entries[4]), "Livro removido com sucesso.");

    /* Atualizar o combo box e o flowbox após remover o livro */
    atualizarComboBox(entries[5]);
    atualizarFlowBox(entries[6]);  /* Exibir os livros automaticamente */
}

/* Função de clique para buscar o livro */
static void on_buscar_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;
    
    const gchar *titulo = gtk_entry_get_text(GTK_ENTRY(entries[0]));

    Livro* livro = buscarLivro(head, titulo);
    
    if (livro != NULL) {
        char info[256];
        snprintf(info, sizeof(info), "Livro encontrado:\nTítulo: %s\nAutor: %s\nPreço: %.2f\nQuantidade: %d",
                 livro->titulo, livro->autor, livro->preco, livro->quantidade);
        gtk_label_set_text(GTK_LABEL(entries[4]), info);
    } else {
        gtk_label_set_text(GTK_LABEL(entries[4]), "Livro não encontrado.");
    }
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid, *titulo_entry, *autor_entry, *preco_entry, *quantidade_entry;
    GtkWidget *inserir_button, *remover_button, *buscar_button, *mensagem_label;
    GtkWidget *combo_box, *scrolled_window, *flowbox;
    GtkWidget *logo_image;

    gtk_init(&argc, &argv);

    /* Carregar o CSS do arquivo externo */
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Livraria Elegante");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_container_set_border_width(GTK_CONTAINER(window), 20);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Criar uma GtkScrolledWindow para rolagem de toda a página */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_widget_set_size_request(scrolled_window, 800, 600);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(scrolled_window), grid);

    /* Adicionar logo e estilizar */
    logo_image = gtk_image_new_from_file("logo.png");
    gtk_widget_set_size_request(logo_image, 50, 50);  /* Redimensionar logo */
    gtk_grid_attach(GTK_GRID(grid), logo_image, 1, 0, 1, 1);

    /* Entradas de texto */
    titulo_entry = gtk_entry_new();
    autor_entry = gtk_entry_new();
    preco_entry = gtk_entry_new();
    quantidade_entry = gtk_entry_new();

    /* Definir os campos Preço e Quantidade para aceitar apenas números */
    g_signal_connect(preco_entry, "insert-text", G_CALLBACK(validar_entrada_numerica), NULL);
    g_signal_connect(quantidade_entry, "insert-text", G_CALLBACK(validar_entrada_numerica), NULL);

    /* Organizar os campos */
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Título:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), titulo_entry, 1, 1, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Autor:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), autor_entry, 1, 2, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Preço:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), preco_entry, 1, 3, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Quantidade:"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), quantidade_entry, 1, 4, 2, 1);

    /* ComboBox para selecionar livros */
    combo_box = gtk_combo_box_text_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Selecionar Livro para Remover:"), 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo_box, 1, 5, 2, 1);

    inserir_button = gtk_button_new_with_label("Inserir Livro");
    remover_button = gtk_button_new_with_label("Remover Livro");
    buscar_button = gtk_button_new_with_label("Buscar Livro");

    gtk_grid_attach(GTK_GRID(grid), inserir_button, 0, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), remover_button, 1, 6, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), buscar_button, 2, 6, 1, 1);  /* Botão de busca */

    /* FlowBox para exibir livros lado a lado */
    flowbox = gtk_flow_box_new();
    gtk_flow_box_set_max_children_per_line(GTK_FLOW_BOX(flowbox), 3);  /* Definir o máximo de livros por linha */
    gtk_flow_box_set_selection_mode(GTK_FLOW_BOX(flowbox), GTK_SELECTION_NONE);  /* Desativar seleção */
    gtk_grid_attach(GTK_GRID(grid), flowbox, 0, 7, 4, 1);  /* Expandir o FlowBox por 4 colunas */

    /* Mensagem de status */
    mensagem_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), mensagem_label, 0, 8, 4, 1);

    /* Adicionar o grid na scrolled_window */
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    GtkWidget* entries[] = {titulo_entry, autor_entry, preco_entry, quantidade_entry, mensagem_label, combo_box, flowbox};

    g_signal_connect(inserir_button, "clicked", G_CALLBACK(on_inserir_clicked), entries);
    g_signal_connect(remover_button, "clicked", G_CALLBACK(on_remover_clicked), entries);
    g_signal_connect(buscar_button, "clicked", G_CALLBACK(on_buscar_clicked), entries);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
