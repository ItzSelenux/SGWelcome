void load_folder_list(const gchar *notes_path, GtkWidget *parent_notebook, gint parent_page)
{
	DIR *dir;
	struct dirent *entry;
	//Stage 1: read the master dir to create tabs
	if ((dir = opendir(notes_path)) != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_DIR)
			{
				const char *filename = entry->d_name;

				if (filename[0] != '.')
				{
					// Remove numbers and dots from the filename
					char filtered_filename[256];
					int j = 0;
					for (int i = 0; i < strlen(filename); ++i)
					{
						if (!isdigit(filename[i]) && filename[i] != '.')
						{
							filtered_filename[j++] = filename[i];
						}
					}
					filtered_filename[j] = '\0';

					GtkWidget *notebook_grid = gtk_grid_new();

					gtk_container_set_border_width(GTK_CONTAINER(notebook_grid), 10);
					gtk_container_set_border_width(GTK_CONTAINER(window), 10);
					GtkWidget *list = gtk_list_box_new();
					GtkWidget *scrolled_list = gtk_scrolled_window_new(NULL, NULL);
					gtk_widget_set_vexpand(scrolled_list, TRUE);
					gtk_container_add(GTK_CONTAINER(scrolled_list), list);
					gtk_notebook_append_page(GTK_NOTEBOOK(parent_notebook), GTK_WIDGET(notebook_grid), gtk_label_new(filtered_filename));

					char subfolder_path[ML];
					snprintf(subfolder_path, sizeof(subfolder_path), "%s/%s", notes_path, filename);

					DIR *subdir;
					struct dirent *subentry;
					//Stage 2: read /$master_folder/$tab/* to create labels
					if ((subdir = opendir(subfolder_path)) != NULL)
					{
						while ((subentry = readdir(subdir)) != NULL)
						{
							if (subentry->d_type == DT_REG) // Disable showing directories
							{
								char cfile[9999];
								const char *infilename = subentry->d_name;

								snprintf(cfile, sizeof(cfile), "%s/%s", subfolder_path, infilename);
								//g_print(cfile);
								FILE *file = fopen(cfile, "r");

								char line[ML];
								char content[ML];
								content[0] = '\0';

								while (fgets(line, sizeof(line), file) != NULL)
								{
									strcat(content, line); // Concatenate lines to form content
								}

								GtkWidget *label_content = gtk_label_new(NULL);
								gtk_label_set_xalign(GTK_LABEL(label_content), 0.0);
								gtk_widget_set_hexpand(label_content, TRUE);
								gint position = atoi(infilename);
								gtk_label_set_markup(GTK_LABEL(label_content), content);
								gtk_grid_attach(GTK_GRID(notebook_grid), label_content, 0, position, 1, 1);
							}
							//Stage 3: read /$masterdata/$tab/data/* to create images and entries
							else if (subentry->d_type == DT_DIR && strcmp(subentry->d_name, "data") == 0)
							{
								// Handle "data" directory
								char data_folder_path[PATH_MAX*2];
								snprintf(data_folder_path, sizeof(data_folder_path), "%s/%s", subfolder_path, subentry->d_name);

								DIR *data_dir;
								struct dirent *data_entry;

								if ((data_dir = opendir(data_folder_path)) != NULL)
								{
									while ((data_entry = readdir(data_dir)) != NULL)
									{
										if (data_entry->d_type == DT_REG)
										{
											gchar cfile[PATH_MAX*3];
											const gchar *infilename = data_entry->d_name;

											snprintf(cfile, sizeof(cfile), "%s/%s", data_folder_path, infilename);

											GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(cfile, NULL);
											int position = atoi(infilename);
											if (pixbuf != NULL && checkname(infilename))
											{
												GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
												gtk_grid_attach(GTK_GRID(notebook_grid), image, 1, position, 1, 1);
												g_object_unref(pixbuf);
											}
										}
										//Stage 3.5: this is about the entries
										if (data_entry->d_type == DT_DIR)
										{
											DIR *indir;
											struct dirent *inentry;
											const gchar *infilename = data_entry->d_name;
											snprintf(current_folder, sizeof(current_folder), "%s/%s/data/%s", notes_path, filename, infilename);
											if (checkname(infilename) && (indir = opendir(current_folder)) != NULL) 
											{
												while ((inentry = readdir(indir)) != NULL)
												{
													if (inentry->d_type == DT_REG) // Disable showing directories
													{
														gchar cfile[9999];
														const gchar *rinfilename = inentry->d_name;

														snprintf(cfile, sizeof(cfile), "%s/%s", current_folder, rinfilename );

														GKeyFile *key_file = g_key_file_new();

														if (g_key_file_load_from_file(key_file, cfile, G_KEY_FILE_NONE, NULL))
														{
															gchar *ActName = NULL;
															gchar *ActNote = NULL;
															gchar *ActNotif = NULL;
															gchar *ActIcon = NULL;
															gint NoDisplay = 0;
															gchar *ActBtnLabel = NULL;

															gchar *only_show_in = g_key_file_get_string(key_file, "Desktop Entry", "OnlyShowIn", NULL);
															if (only_show_in != NULL)
															{
																if (strcmp(only_show_in, getenv("XDG_CURRENT_DESKTOP")) == 0)
																{
																	NoDisplay = 0;
																}
																else
																{
																	NoDisplay = 1;
																}
																g_free(only_show_in);
															}
															
															gchar *currentlang = getenv("LANG"), *lang_key;
															if (currentlang == NULL)
															{
																lang_key="Name";
															}
															else
															{
																gchar **lang_parts = g_strsplit(currentlang, "_", -1);
																lang_key = g_strdup_printf("Name[%s]", lang_parts[0]);
															}
															ActName = g_key_file_get_string(key_file, "Desktop Entry", lang_key, NULL);
															if (ActName == NULL)
															{
																ActName = g_key_file_get_string(key_file, "Desktop Entry", "Name", NULL);
															}

															//ActName = g_key_file_get_string(key_file, "Desktop Entry", "Name", NULL);
															ActNote = g_key_file_get_string(key_file, "Desktop Entry", "Comment", NULL);
															ActIcon = g_key_file_get_string(key_file, "Desktop Entry", "Icon", NULL);
															if (ActIcon)
															{
																gchar *newline = strchr(ActIcon, '\n');
																if (newline != NULL)
																{
																	*newline = '\0';
																}
															}
															ActNotif = g_key_file_get_string(key_file, "Desktop Entry", "Exec", NULL);
															ActBtnLabel = g_key_file_get_string(key_file, "Desktop Entry", "ButtonText", NULL);

															if (NoDisplay == 0)
															{
																GtkWidget *ingrid = gtk_grid_new();
																gtk_grid_set_column_homogeneous(GTK_GRID(ingrid), TRUE);
																gtk_grid_set_row_spacing(GTK_GRID(ingrid), 5);
																	GdkPixbuf *pixbuf = gtk_icon_theme_load_icon(theme, ActIcon, actiondefaultsize, GTK_ICON_LOOKUP_USE_BUILTIN, NULL);
																	GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, actiondefaultsize, actiondefaultsize, GDK_INTERP_BILINEAR);
																	GtkWidget *icon = gtk_image_new_from_pixbuf(scaled_pixbuf);
																//GtkWidget *icon = gtk_image_new_from_icon_name(ActIcon, GTK_ICON_SIZE_BUTTON);
																GtkWidget *label_filename = gtk_label_new(ActName);
																	gtk_widget_set_tooltip_text(ingrid, ActNote);
																GtkWidget *label_time = gtk_label_new(ActNote);
																GtkWidget *Installbutton = gtk_button_new_with_label(ActBtnLabel);
																	gtk_widget_set_name(Installbutton, ActNotif);
																	g_signal_connect(Installbutton, "clicked", G_CALLBACK(on_item_activate), NULL);

																gtk_grid_attach(GTK_GRID(ingrid), icon, 0, 0, 1, 1);
																gtk_grid_attach(GTK_GRID(ingrid), label_filename, 1, 0, 1, 1);
																if (showcomment)
																	gtk_grid_attach(GTK_GRID(ingrid), label_time, 2, 0, 1, 1);
																gtk_grid_attach(GTK_GRID(ingrid), Installbutton, 3, 0, 1, 1);

																gint position = atoi(infilename);
																gtk_list_box_insert(GTK_LIST_BOX(list), ingrid, -1);
																gtk_grid_attach(GTK_GRID(notebook_grid), scrolled_list, 0, position, 2, 1);
															}
														}
													}
												}
												closedir(indir);
											}
										}
									}
									closedir(data_dir);
								}
							}
						}
						closedir(subdir);
					}
				}
			}
		}
		closedir(dir);
	}
}
