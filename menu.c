#ifndef GTK_H
	#include <gtk/gtk.h>
#endif
#ifndef GDKKEYSYMS_H
	#include <gdk/gdkkeysyms.h>
#endif
#ifndef MENU_H
	#include "Headers/menu.h"
#endif
#ifndef TREE_H
	#include "Headers/tree.h"
#endif
#ifndef STDINT_H
	#include <stdint.h> // uint8_t
#endif


void Init() {
//______________Window_____________________________

	gtk_init(NULL,NULL);		// init GTK

	InitTree ();			// tree init _TREE_C_

	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);  		 //main Window
	gtk_window_set_title(GTK_WINDOW(window),"ZVproject Tree Creator v.1.00"); //main window title
	gtk_window_set_default_size(GTK_WINDOW(window),800,600);     		  //main window size 800x600
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);		  //main window position
	gtk_container_set_border_width(GTK_CONTAINER(window), 10);       	  //main window border
	GdkPixbuf *icon =create_pixbuf("Img/icon.png");				  //icon
	gtk_window_set_icon(GTK_WINDOW(window),icon);				  //main window icon
	gtk_widget_show(window);						  //show main window



	GtkWidget *topwin = gtk_scrolled_window_new(NULL,NULL);   		 // Scrolled window for double menu
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(topwin),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC); //scrollbar V&H present only if needed
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(topwin),GTK_SHADOW_ETCHED_IN); // Scrolled window shadow

	
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);				//create new vertical container

//__________________________Menubar_________________________

	GtkWidget *menubar = menu_Init(window,topwin);	             //holds GtkMenuItem widgets			
	if (menubar == NULL)                           		     //check on Errors
		fprintf(stderr,"Error init toolbar\n");

	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0); // pack it to main vbox
//__________________________END_MENU___________________

//____________________________TOOLBAR____________________

	GtkWidget *toolbar= bar_Init(); 			     //holds toolbar widget
	if (toolbar == NULL) 					     //check on Errors
		fprintf(stderr,"Error init toolbar\n");

	gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 1); // pack it to main vbox

//__________________________END of TOLLBAR________________	

//__________________________MAIN INIT_____________________________

	GtkWidget *hbox = gtk_hpaned_new();	// main box for program topwin data_view and main_view		
		//____________LEFT BOX____________________
		GtkWidget *v1box = gtk_vbox_new(FALSE, 0);		//container for topwin & data_view

			GtkWidget *node_type = gtk_scrolled_window_new(NULL,NULL);  	      //window for chosing node field's types
				gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(node_type),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
				gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(node_type),GTK_SHADOW_ETCHED_IN);

				GtkWidget *type_list = gtk_tree_view_new();
					gtk_tree_view_set_grid_lines(GTK_TREE_VIEW(type_list), GTK_TREE_VIEW_GRID_LINES_BOTH);
					gtk_tree_view_set_reorderable (GTK_TREE_VIEW(type_list),TRUE);

					int num = 1; // number of row
					GtkListStore *store = gtk_list_store_new(4,G_TYPE_UINT,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING);

					GtkCellRenderer *render;
					
					render = gtk_cell_renderer_text_new ();
					GtkTreeViewColumn* row_n = gtk_tree_view_column_new_with_attributes("#",render,"text",0, NULL);
					gtk_tree_view_append_column(GTK_TREE_VIEW(type_list), row_n);

					//________________________________________
					render = gtk_cell_renderer_combo_new ();
					//creating a list for combo box
					const gchar *type[] = {"int", "float", "string", "intA", "floatA"};
					GtkListStore *combo_list = gtk_list_store_new(1,G_TYPE_STRING);   // list for  combo box
					for (register uint8_t i=0;i++<=4; ){	
						gtk_list_store_insert_with_values(combo_list,NULL,-1, 0,type[i-1],-1);
					}
					//____returning list, free list at line 309
					g_object_set(render, "model", combo_list, "text-column", 0, "has-entry", FALSE, "editable", TRUE, (char *)NULL); 
					g_signal_connect(render, "changed", G_CALLBACK(on_tree_cell_combo_editing_start), store); 
					GtkTreeViewColumn* type_colomn = gtk_tree_view_column_new_with_attributes("Type",render, "text" , 1, NULL);
					gtk_tree_view_column_set_resizable(type_colomn, TRUE);
					gtk_tree_view_append_column(GTK_TREE_VIEW(type_list), type_colomn);
					
					//_______________________________________
					render = gtk_cell_renderer_text_new ();
					GtkTreeViewColumn* row_data = gtk_tree_view_column_new_with_attributes("Data",render,"text",2, NULL);
					gtk_tree_view_append_column(GTK_TREE_VIEW(type_list), row_data);
					g_object_set(render,"editable",TRUE,NULL);
					gtk_tree_view_column_set_resizable(row_data, TRUE);


					//_______________________________________
					GtkCellRenderer *render_pic = gtk_cell_renderer_pixbuf_new();
					GtkCellRenderer *render_button = gtk_cell_renderer_toggle_new();
					//GtkTreeViewColumn* delete_raw = gtk_tree_view_column_new_with_attributes("Delete",render_pic, "icon-name",render_text,"text", 2, NULL);
					g_object_set(render_pic, "icon-name", "edit-delete");
					GtkTreeViewColumn* delete_row = gtk_tree_view_column_new();

					gtk_tree_view_column_pack_start(delete_row,render_pic,FALSE);
					//g_object_set_data(G_OBJECT(render_pic), "win", window); 
					gtk_tree_view_column_pack_end(delete_row,render_button,FALSE);
							


					gtk_tree_view_append_column(GTK_TREE_VIEW(type_list), delete_row);		// insert delete cell into colomn
					g_signal_connect(G_OBJECT(render_button),"toggled",G_CALLBACK(remove_row_data_type),store); // add signal to delete
					g_signal_connect(G_OBJECT(render),"edited",G_CALLBACK(row_data_entry),type_list);
					GtkTreeIter itr;		// create new iter for the new row
					gtk_list_store_append(store,&itr);  // create new row for the list
					gtk_list_store_set(store, &itr, 0, num, 1,"char", -1); //add values to the new row

 					gtk_tree_view_set_model(GTK_TREE_VIEW(type_list),GTK_TREE_MODEL(store));

					g_object_unref (G_OBJECT (store));
					g_object_unref (G_OBJECT (combo_list));
				gtk_container_add(GTK_CONTAINER(node_type),type_list);
				//g_signal_connect(G_OBJECT(gtk_tree_view_get_model(GTK_TREE_VIEW(type_list))), "row-deleted", G_CALLBACK(drag_and_drop), FALSE);
				//g_signal_connect(G_OBJECT(gtk_tree_view_get_model(GTK_TREE_VIEW(type_list))), "row-inserted", G_CALLBACK(drag_and_drop), TRUE);

			gtk_box_pack_start(GTK_BOX(v1box), node_type, TRUE, TRUE, 3);

			GtkWidget *Type_Tool_box = gtk_hbox_new(FALSE, 5); //creating container for data tools

				GtkWidget *add_type = gtk_button_new_with_label("Add");		// create add button
				g_signal_connect(G_OBJECT(add_type),"clicked",G_CALLBACK(add_row_data_type),type_list);	// add signal on button and pass type_list widget

			
				GtkWidget *type_up = gtk_button_new_with_label("Up");
				g_signal_connect(G_OBJECT(type_up),"clicked",G_CALLBACK(row_data_type_up),type_list);	// add signal on button


				GtkWidget *type_down = gtk_button_new_with_label("Down");
				g_signal_connect(G_OBJECT(type_down),"clicked",G_CALLBACK(row_data_type_down),type_list);	// add signal on button
				gtk_box_pack_start(GTK_BOX(Type_Tool_box ), add_type, FALSE, TRUE, 3);   //add to the container
				gtk_box_pack_start(GTK_BOX(Type_Tool_box ), type_up, FALSE, TRUE, 3);   //add to the container
				gtk_box_pack_start(GTK_BOX(Type_Tool_box ), type_down, FALSE, TRUE, 3);   //add to the container
								

	
			gtk_box_pack_start(GTK_BOX(v1box), Type_Tool_box, FALSE, TRUE, 3);

		//______________________LEFT Bottom Part____________________________
	
			GtkWidget *node_data = gtk_scrolled_window_new(NULL,NULL);   		//data_view init
				gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(node_data),GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
				gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(node_data),GTK_SHADOW_ETCHED_IN);
			
				GtkWidget *data_list = gtk_tree_view_new();
					

				gtk_container_add(GTK_CONTAINER(node_data),data_list);

			gtk_box_pack_start(GTK_BOX(v1box), node_data, TRUE, TRUE, 2); 		//data_view container


		gtk_paned_pack1(GTK_PANED(hbox),v1box,TRUE,FALSE);                         // add container for tree and data into 
		//_____________END of LEFT BOX____________________

		//_____________RIGHT BOX__________________________
		GtkWidget *mainbox = gtk_vbox_new(FALSE,3);

			GtkWidget *tree_view = create_empty_tree_model();		//creating new tree
				gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(tree_view), FALSE);
				gtk_container_add(GTK_CONTAINER(topwin),tree_view);
			gtk_box_pack_start(GTK_BOX(mainbox),topwin,TRUE,TRUE,2);	  	        // topwin container init at 42 line

				
			GtkWidget *Tree_Tool_box = gtk_hbox_new(FALSE, 5); 			//creating container for tree tools

				GtkWidget *add = gtk_button_new_with_label("Add");		// create add button
				g_object_set_data(G_OBJECT(add),"Tree",tree_view);		// add Tree pointer to button

				GtkWidget *entry = gtk_entry_new();				// change name field
				gtk_entry_set_max_length (GTK_ENTRY (entry), 15); 		// max entry length
				gtk_entry_set_text( GTK_ENTRY(entry),"Child");			// set default text
				gtk_widget_set_size_request(entry, 120, -1);			// size for change field

				GtkWidget *remove = gtk_button_new_with_label("Remove");	// create remove button
				g_object_set_data(G_OBJECT(remove),"Tree",tree_view);		// add Tree pointer to button

				gtk_box_pack_start(GTK_BOX(Tree_Tool_box ), add, FALSE, TRUE, 3);   //add to the container
				gtk_box_pack_start(GTK_BOX(Tree_Tool_box ), entry, FALSE, TRUE, 3); //add to the conteiner
				gtk_box_pack_start(GTK_BOX(Tree_Tool_box ), remove, FALSE, TRUE, 3);//add to the container
			gtk_box_pack_start(GTK_BOX(mainbox), Tree_Tool_box, FALSE, TRUE, 3);
	
		gtk_paned_pack2(GTK_PANED(hbox), mainbox,TRUE,FALSE );
	gtk_container_add(GTK_CONTAINER(vbox),hbox);

	GtkWidget *statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

	gtk_container_add(GTK_CONTAINER(window),vbox);
  /*	
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
	gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
*/



/*	

	g_signal_connect(selection, "changed",G_CALLBACK(on_changed), statusbar);

	g_signal_connect(G_OBJECT(add),"clicked",G_CALLBACK(fill_model),selection);*/
/*
	g_signal_connect(G_OBJECT(add),"clicked",G_CALLBACK(fill_model),selection);
*/
	
	g_signal_connect(G_OBJECT(add), "clicked", G_CALLBACK(add_child), entry);
  	g_signal_connect(G_OBJECT(remove), "clicked", G_CALLBACK(remove_child), NULL);

  	//g_signal_connect(G_OBJECT(add_data), "clicked", G_CALLBACK(add_field_data), NULL);
 // 	g_signal_connect(G_OBJECT(remove_data), "clicked", G_CALLBACK(remove_child), NULL);

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);  
	gtk_widget_show_all(window);
//	free(TreeArgs);
	g_object_unref(icon);
	gtk_main();
}

//_____________________________________MENU BAR__________________________________

GtkWidget* menu_Init(GtkWidget *window, GtkWidget *topwin) {
	GtkWidget *menubar = gtk_menu_bar_new();
	
	GtkWidget *fileMenu = gtk_menu_new();
	GtkWidget *treeMenu = gtk_menu_new();
		GtkWidget *infoMenu = gtk_menu_new();
		GtkWidget *editMenu = gtk_menu_new();
		GtkWidget *moveMenu = gtk_menu_new();
	GtkWidget *aboutMenu = gtk_menu_new();

	GtkAccelGroup * accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window),accel_group);

	GtkWidget *menu_main = gtk_menu_item_new_with_label("File");
		//GtkWidget *menu_new = gtk_menu_item_new_with_label("New tree");
		GtkWidget *menu_new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW,accel_group);
		//GtkWidget *menu_load = gtk_menu_item_new_with_label("Load tree");
		GtkWidget *menu_load = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN,accel_group);
		//GtkWidget *menu_save = gtk_menu_item_new_with_label("Save");
		GtkWidget *menu_save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE,accel_group);
		GtkWidget *menu_saveExit = gtk_menu_item_new_with_label("Save & Exit");
		GtkWidget *menu_exit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,accel_group);
			//gtk_widget_add_accelerator(menu_exit, "activate",accel_group, GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	GtkWidget *menu_tree = gtk_menu_item_new_with_label("Tree");
		GtkWidget *menu_info = gtk_menu_item_new_with_label("Info");
			GtkWidget *menu_info_print = gtk_menu_item_new_with_label("Print data of current node");
			GtkWidget *menu_info_printAll = gtk_menu_item_new_with_label("Print all tree");
			GtkWidget *menu_info_children = gtk_menu_item_new_with_label("Number of children of node");
		GtkWidget *menu_edit = gtk_menu_item_new_with_label("Edit");
			GtkWidget *menu_edit_data = gtk_menu_item_new_with_label("Change/Add data in node");
			GtkWidget *menu_edit_children = gtk_menu_item_new_with_label("Add children to node");
			GtkWidget *menu_edit_insert = gtk_menu_item_new_with_label("Insert child");
			GtkWidget *menu_edit_delete = gtk_menu_item_new_with_label("Delete child of node");
		GtkWidget *menu_move = gtk_menu_item_new_with_label("Move");
			GtkWidget *menu_move_up = gtk_menu_item_new_with_label("Move up");
			GtkWidget *menu_move_down = gtk_menu_item_new_with_label("Move to a child");

	GtkWidget *menu_about = gtk_menu_item_new_with_label("About");	
		GtkWidget *menu_readme = gtk_menu_item_new_with_label("About");
	
	GtkWidget *sep = gtk_separator_menu_item_new();
	//--------------	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_main),fileMenu);
			gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),menu_new);
			gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),menu_load);
			gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),menu_save);
			gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),sep);
			gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),menu_saveExit);
			gtk_menu_shell_append(GTK_MENU_SHELL(fileMenu),menu_exit);
		gtk_menu_shell_append(GTK_MENU_SHELL(menubar),menu_main);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_tree), treeMenu);
				gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_info),infoMenu);
					gtk_menu_shell_append(GTK_MENU_SHELL(infoMenu),menu_info_print);
					gtk_menu_shell_append(GTK_MENU_SHELL(infoMenu),menu_info_printAll);
					gtk_menu_shell_append(GTK_MENU_SHELL(infoMenu),menu_info_children);	
			gtk_menu_shell_append(GTK_MENU_SHELL(treeMenu),menu_info);
				gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_edit),editMenu);
					gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),menu_edit_data);
					gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),menu_edit_children);
					gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),menu_edit_insert);
					gtk_menu_shell_append(GTK_MENU_SHELL(editMenu),menu_edit_delete);	
			gtk_menu_shell_append(GTK_MENU_SHELL(treeMenu),menu_edit);
				gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_move),moveMenu);
					gtk_menu_shell_append(GTK_MENU_SHELL(moveMenu),menu_move_up);
					gtk_menu_shell_append(GTK_MENU_SHELL(moveMenu),menu_move_down);
			gtk_menu_shell_append(GTK_MENU_SHELL(treeMenu),menu_move);
		gtk_menu_shell_append(GTK_MENU_SHELL(menubar),menu_tree);

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_about),aboutMenu);
			gtk_menu_shell_append(GTK_MENU_SHELL(aboutMenu),menu_readme);
		gtk_menu_shell_append(GTK_MENU_SHELL(menubar),menu_about);

//------------actions-------

	g_signal_connect(G_OBJECT(menu_exit), "activate", G_CALLBACK(gtk_main_quit),NULL);
	//GtkWidget *view = create_view_and_model(1);
//	gtk_container_add(GTK_CONTAINER(topwin), view);
	g_signal_connect(G_OBJECT(menu_new), "activate", G_CALLBACK(tree_init),topwin);
	g_signal_connect(G_OBJECT(menu_load), "activate", G_CALLBACK(open_tree),NULL);
	


	return menubar;
}



void open_tree (GtkWidget* widget, gpointer data) {
	GtkWidget *dialog;
	
	dialog = gtk_file_chooser_dialog_new ("Open File", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,NULL);

	if (gtk_dialog_run(GTK_DIALOG(dialog))==GTK_RESPONSE_ACCEPT) {
		char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		//LoadTree(filename);
		g_free(filename);
	}
	gtk_widget_destroy(dialog);
	
	GtkWidget *view = create_empty_tree_model();


		
}

//____________________________TOOLBAR_______________________________
GtkWidget* bar_Init () {

	GtkWidget* tmpbar = gtk_toolbar_new();

	gtk_toolbar_set_style(GTK_TOOLBAR(tmpbar),GTK_TOOLBAR_ICONS);

	GtkToolItem *tool_sep=gtk_separator_tool_item_new(); //tool separator
	
	//-------------Tool_info----
	GtkToolItem *tool_print = gtk_tool_button_new_from_stock(GTK_STOCK_DND);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_print,-1);
	gtk_tool_item_set_tooltip_text(tool_print,"Print Info of current node");

/*	GtkToolItem *tool_printAll = gtk_tool_button_new_from_stock(GTK_STOCK_DND_MULTIPLE);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_printAll,-1);
	gtk_tool_item_set_tooltip_text(tool_printAll,"Print All tree");*/

	GtkToolItem *tool_cNumber = gtk_tool_button_new_from_stock(GTK_STOCK_PAGE_SETUP);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_cNumber,-1);
	gtk_tool_item_set_tooltip_text(tool_cNumber,"Number of children");

	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_sep,-1);

	//---------------Toll_edit
	GtkToolItem *tool_data = gtk_tool_button_new_from_stock(GTK_STOCK_BOLD);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_data ,-1);
	gtk_tool_item_set_tooltip_text(tool_data , "Change/Add data to node");

	GtkToolItem *tool_childAdd = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_childAdd ,-1);
	gtk_tool_item_set_tooltip_text(tool_childAdd , "Add children to node");

	GtkToolItem *tool_childInsert = gtk_tool_button_new_from_stock(GTK_STOCK_CONVERT);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_childInsert ,-1);
	gtk_tool_item_set_tooltip_text(tool_childInsert , "Insert Child");

	GtkToolItem *tool_childDelete = gtk_tool_button_new_from_stock(GTK_STOCK_CLOSE);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_childDelete ,-1);
	gtk_tool_item_set_tooltip_text(tool_childDelete , "Delete child");

	//-------------Tool_move------
	GtkToolItem *tool_MUp = gtk_tool_button_new_from_stock(GTK_STOCK_GO_UP);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_MUp,-1);
	gtk_tool_item_set_tooltip_text(tool_MUp,"Move to parent");

	GtkToolItem *tool_MDown = gtk_tool_button_new_from_stock(GTK_STOCK_GO_DOWN);
	gtk_toolbar_insert(GTK_TOOLBAR(tmpbar),tool_MDown,-1);
	gtk_tool_item_set_tooltip_text(tool_MDown,"Move to child");

	return tmpbar;
}

//_____________________TREE_VIEW_MAIN________________________

//New Tree
GtkWidget *create_empty_tree_model(/*uint8_t mode*/) {            //delete if exist and create empty tree,
	
	GtkTreeViewColumn *col 	  = gtk_tree_view_column_new();
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
	GtkWidget *tree	          = gtk_tree_view_new();
	
	gtk_tree_view_column_set_title(col,"My Tree");
	gtk_tree_view_append_column(GTK_TREE_VIEW(tree),col);

	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);

	GtkTreeStore *treestore   = gtk_tree_store_new(NUM_COLS,G_TYPE_STRING); // creating tree

	GtkTreeIter toplevel;
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, COLUMN, "Root", -1);


	GtkTreeModel *model       = GTK_TREE_MODEL(treestore);
	gtk_tree_view_set_model(GTK_TREE_VIEW(tree),model);
	g_object_unref(model);

	return tree;
}

//______Tree_Control_for_main_Right_box

// Add child to selected node
void add_child(GtkWidget* widget, gpointer entry){

	GtkTreeIter iter, child;  					//pointer for selection node and new child node
	GtkTreeModel* model;   						//nead for selection, model is empty
	GtkWidget* tree = g_object_get_data(G_OBJECT(widget),"Tree");   //get tree from button
	
	if (tree != NULL) {
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree)); //find what was selected

		if (gtk_tree_selection_get_selected(selection, &model, &iter)) { 
		

			GtkTreePath* path = gtk_tree_model_get_path(model,&iter); 	//get path of the iter
 			int depth = gtk_tree_path_get_depth(path); 		//get depth of iter of selected row
			//------------might not work
			if (current!=root) { 
				for (int i=0;++i<depth;){
					node_from_number(gtk_tree_path_get_indices(path)[i-1]);
				}
			}
			//------------
			AddChild(current,1); 
			gtk_tree_path_free(path); 

			GtkTreeStore *treestore = GTK_TREE_STORE(model);  //a model for tree widgets
			gtk_tree_store_append(treestore, &child, &iter);  //create a child for selected node
			gtk_tree_store_set(treestore, &child, COLUMN,gtk_entry_get_text(entry) , -1);  // get a name for child

		}
	}
}

//remove child to selected node
void remove_child(GtkWidget *widget, gpointer entry){

	GtkTreeIter iter, child;  					//pointer for selection node and new child node
	GtkTreeModel* model;   						//nead for selection, model is empty
	GtkWidget* tree = g_object_get_data(G_OBJECT(widget),"Tree");   //get tree from button
	
	if (tree != NULL) {		//check if NULL to avoid GTK errors
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree)); //find what was selected

		if (gtk_tree_selection_get_selected(selection, &model, &iter)) { 
			GtkTreeStore *treestore = GTK_TREE_STORE(model);  //a model for tree widgets
			gtk_tree_store_remove(treestore, &iter);  //create a child for selected node
		}
	}
}

//__________________________________________________________

//________________________TREE_TYPE_left_upper_________________________
// add another type row
void add_row_data_type (GtkWidget* widget, GtkWidget *list) {

	GtkListStore *store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));	//get store from tree widget
	gint num = gtk_tree_model_iter_n_children(gtk_tree_view_get_model(GTK_TREE_VIEW(list)),NULL) +1 ;	//get number of all rows to get an index of a new one
	GtkTreeIter iter;
	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(list));  // get selection
	if (!gtk_tree_selection_get_selected(selection, NULL, &iter))	{		// get iter from selection
		gtk_list_store_append(store,&iter);		// insert in the end of the list if ther is no selection
		
	}
	else {
		gtk_list_store_insert_before(store,&iter,&iter);	// insert new row before selected one
		gtk_tree_selection_unselect_all(selection);		// clear selection; othervise ther is no way to add new item on the bottom of the list, or will nbeed to creat another button

		num = reorder_list_colomn_index(gtk_tree_view_get_model(GTK_TREE_VIEW(list)),iter,0); 		// reindex all rows and get index for a new row will increment later	
	}	
	gtk_list_store_set(store, &iter, 0 , num, 1,"char",  -1); //set a default info for the new row
}


// remove selected row
void remove_row_data_type (GtkCellRendererToggle *renderer, gchar *path,  GtkListStore *list) {
	GtkTreeIter iter;
	gtk_tree_model_get_iter_from_string( GTK_TREE_MODEL(list), &iter, path );  	//get model
	if (gtk_tree_model_iter_n_children(GTK_TREE_MODEL(list),NULL) == 1) {	//can't delete last type, some data must be present

		warning_message(NULL,"Why do you need an empty tree?");	
	}
	else {	
		reorder_list_colomn_index(GTK_TREE_MODEL(list),iter,-1); 			//reorder list
		gtk_list_store_remove(list, &iter);						//remove selected element
	}
}

// change type of the row
void on_tree_cell_combo_editing_start(GtkCellRendererCombo *renderer, gchar *path, GtkTreeIter *c_iter , GtkListStore *list) 
// rendered - is combo box in the cell
// path - is path to the row of the parent list
// c_iter - internla iter; iter of combo box
// list - main list store 
{
	GtkTreeIter iter;								// init local iter of combo box
	gtk_tree_model_get_iter_from_string( GTK_TREE_MODEL( list ), &iter, path );	// get iter of parent list using string path
	GtkListStore *store = NULL;								
	gchar* new_string = NULL;							
	g_object_get(renderer,"model",&store,NULL);					// get liststore model of the combo box
   	gtk_tree_model_get (GTK_TREE_MODEL(store), c_iter, 0, &new_string, -1);		// get string of selected item in combo box
	
	gtk_list_store_set (list, &iter , 1, new_string, -1);				// insert selected string as default in main list
}

// move row type up
gboolean select_prev_iter (GtkTreeModel *tree_model, GtkTreeIter *iter,GtkTreeIter *main_iter) {  // only for GTK 2 on gtk3 this function is built in
//pass tree model
// iter where previous iter will be writen
// referance iter 
	g_return_val_if_fail (GTK_IS_TREE_MODEL (tree_model), FALSE);
 	g_return_val_if_fail (iter != NULL, FALSE);
  
	gtk_tree_model_get_iter_first (tree_model,iter);
	if ((gtk_tree_model_iter_n_children(tree_model,NULL)==1) || (iter == main_iter)) {
		iter = NULL;
		return FALSE;
	}
	else {
		gboolean check=TRUE;
		GtkTreeIter tmp_iter;
		while (iter->user_data != main_iter->user_data){
			tmp_iter= *iter;

			gtk_tree_model_iter_next(tree_model,iter);

		}
		*iter = tmp_iter;
		return TRUE;
	}
}


void row_data_type_up (GtkWidget *button, GtkWidget *tree) {
	GtkListStore *list = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tree)));
	if (gtk_tree_model_iter_n_children(GTK_TREE_MODEL(list),NULL) == 1) 
		warning_message(NULL,"Move?! WHERE? You are forever alone!");	
	else  {
		GtkTreeIter iter;
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));  // get selection
		if (!gtk_tree_selection_get_selected(selection, NULL, &iter))			// get iter from selection
			warning_message(NULL,"What to move? A node? A box? EARTH?");		// insert in the end of the list if ther is no selection
		else {		
			GtkTreeIter iter_below;
			if (!select_prev_iter(gtk_tree_view_get_model(GTK_TREE_VIEW(tree)),&iter_below,&iter)) 
				warning_message(NULL,"Up? Do you have material to fix the roof?");	
			else {
				gtk_list_store_swap(list,&iter,&iter_below);
				reorder_list_colomn_index(GTK_TREE_MODEL(list),iter, 0);
			}
		}
		gtk_tree_selection_unselect_all(selection);
	}	
}	

void row_data_type_down (GtkWidget *button, GtkWidget *tree) {
	GtkListStore *list = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(tree)));
	if (gtk_tree_model_iter_n_children(GTK_TREE_MODEL(list),NULL) == 1) 
		warning_message(NULL,"Move?! WHERE? You are forever alone!");	
	else  {
		GtkTreeIter iter;
		GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));  // get selection
		if (!gtk_tree_selection_get_selected(selection, NULL, &iter))			// get iter from selection
			warning_message(NULL,"What to move? A node? A box? EARTH?");		// insert in the end of the list if ther is no selection
		else {		
			GtkTreeIter iter_under=iter;	
			if (!gtk_tree_model_iter_next(GTK_TREE_MODEL(list),&iter_under)) 
				warning_message(NULL,"You are already down! Next stop is hell. Buckle up!");	
			else {
				gtk_list_store_swap(list,&iter,&iter_under);
				reorder_list_colomn_index(GTK_TREE_MODEL(list),iter_under, 0);
			}

		}
		gtk_tree_selection_unselect_all(selection);
	}	
}

void row_data_entry (GtkCellRendererText *cell, gchar * path, gchar *str,GtkWidget *list){

	GtkTreeIter iter;
	gtk_tree_model_get_iter_from_string (gtk_tree_view_get_model(GTK_TREE_VIEW(list)),&iter, path);
	gtk_list_store_set(GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list))), &iter, 2 , str,  -1); //set a new info for the selected row
	GtkTreeSelection *selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(list)); ;
	gtk_tree_selection_unselect_all(selection);
}

//__________________________________________________________


//
void tree_init(GtkWidget* widget, gpointer topwin) {

	GtkWidget *view = create_empty_tree_model();
	gtk_container_add(GTK_CONTAINER(topwin), view);
	//g_signal_connect(selection, "changed",G_CALLBACK(on_changed), statusbar);

}


//____________________Main Toolbar functions____________



//______________________OTHER_()___________________

//----------load icon
GdkPixbuf *create_pixbuf(const gchar *filename) {
	GError *error = NULL;
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(filename,&error);
	if (!pixbuf) {
		fprintf(stderr,"%s\n",error->message);
		g_error_free(error);
	}
	return pixbuf;
}

//------reorder List
/*


gint reorder_list_colomn_index(GtkTreeModel *list, GtkTreeIter iter, int8_t t_action) {
//t_action -1 for delete
//1 for insert
//0 for swap
	GtkTreePath* path = gtk_tree_model_get_path(list,&iter); 	//get path of the iter
	gint index = *gtk_tree_path_get_indices(path); 		//get index of iter of selected row
	gtk_tree_path_free(path); 				// free path we already got the index
	gint elements = gtk_tree_model_iter_n_children(list,NULL)+t_action; //get number of all elements inluding inserted
	gint reindex = index; 					//get index of iter of selected row
	if (t_action == -1)
		gtk_tree_model_iter_next(list,&iter); 			//select next iter
	for (register uint16_t i=reindex; ++i<=elements;) { 		//reindex all rows below

		gtk_list_store_set(GTK_LIST_STORE(list), &iter, 0 , ++reindex,-1); //change values
		gtk_tree_model_iter_next(list,&iter); 			//select next iter
	}
	return ++index;		// returning 0 if element is removed
}

*/
gint reorder_list_colomn_index(GtkTreeModel *list, GtkTreeIter iter, int8_t t_action) {
//t_action -1 for delete
//1 for insert
//0 for swap
	gint index = *gtk_tree_path_get_indices(gtk_tree_model_get_path(list,&iter)); 		//get index of iter of selected rows
	gint elements = gtk_tree_model_iter_n_children(list,NULL)+t_action; //get number of all elements inluding inserted
	if (t_action == -1)
		gtk_tree_model_iter_next(list,&iter); 			//select next iter
	for (register uint16_t i=index; ++i<=elements;) { 		//reindex all rows below

		gtk_list_store_set(GTK_LIST_STORE(list), &iter, 0 , ++(*gtk_tree_path_get_indices(gtk_tree_model_get_path(list,&iter)))+t_action,-1); //change values
		gtk_tree_model_iter_next(list,&iter); 			//select next iter
	}
	return ++index;		// returning 0 if element is removed
}

//-------get warning

GtkWidget *warning_message(GtkWindow* w_main, gchar *message) {

	GtkWidget *dialog=gtk_message_dialog_new (w_main,  GTK_RESPONSE_CLOSE, GTK_MESSAGE_WARNING,  GTK_BUTTONS_CLOSE,  message);
 	gtk_widget_show(dialog);
	gtk_dialog_run(GTK_DIALOG(dialog));
    	gtk_widget_destroy(dialog);
};

//---------listen drag and drop
/*
void drag_and_drop (GtkTreeModel *list, GtkTreePath* path, GtkTreeIter iter ,gboolean flag) {
	
	static int drag = 0;
	static int drop = 0;
	if (flag)
		++drag;
	else
		++drop;
	if ((drag!=0) && (drop!=0)) {
		reorder_list_colomn_index(list, path, iter,0);	
		--drag;
		--drop;
	}

}*/


