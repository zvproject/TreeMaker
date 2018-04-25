#ifndef GTK_H
	#include <gtk/gtk.h>
#endif
#ifndef STDINT_H
	#include <stdint.h> // uint8_t
#endif
#ifndef STDLIB_H
	#include <stdlib.h>
#endif

//public

void Init() ; //init windows and menus

//private
enum {
	COLUMN= 0,
	NUM_COLS
	};

GtkWidget* menu_Init(GtkWidget *window, GtkWidget *topwin);
GtkWidget*  bar_Init ();

void tree_init(GtkWidget* widget, gpointer topwin);
void open_tree (GtkWidget* widget, gpointer data) ;

GtkWidget *create_empty_tree_model(/*uint8_t mode*/) ;

void add_child(GtkWidget* widget, gpointer entry);	//add child
void remove_child(GtkWidget *widget, gpointer entry);	//remove child

void add_field_data (GtkWidget* widget, gpointer topwin);	//add data field
//GtkWidget *create_view_and_model(uint8_t mode);
GtkTreeModel *create_and_fill_model(uint8_t mode);
void fill_model(GtkWidget* widget, gpointer selection);

void on_changed(GtkWidget* widget, gpointer statusbar);
// __________________UPPER LEFT CORNER WINDOW___________
void on_tree_cell_combo_editing_start(GtkCellRendererCombo *renderer, gchar *path, GtkTreeIter *c_iter , GtkListStore *list); //change type on the selected row 
void remove_row_data_type (GtkCellRendererToggle *renderer, gchar *path,  GtkListStore *list); // delete row by selectin delite cell
void add_row_data_type (GtkWidget* widget, GtkWidget* list); // add one row data type
void row_data_type_up (GtkWidget *button, GtkWidget *tree); // move row up
void row_data_type_down (GtkWidget *button, GtkWidget *tree); // move row down
void row_data_entry (GtkCellRendererText *cell, gchar * path, gchar *str,GtkWidget *list);

//_______________________________________________________

GdkPixbuf *create_pixbuf(const gchar *filename);
gint reorder_list_colomn_index(GtkTreeModel  *list, GtkTreeIter iter, int8_t t_action); // reindex list when row was added or removed 
GtkWidget *warning_message(GtkWindow* w_main, gchar *message) ; //get out warning message
//void drag_and_drop (GtkTreeModel *list, GtkTreePath* path, GtkTreeIter iter, gboolean flag) ;
