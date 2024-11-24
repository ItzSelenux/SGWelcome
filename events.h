gint checkname(const gchar *str)
{
	while (*str)
	{
		if (!isdigit(*str))
		{
			return 0;
		}
		str++;
	}
	return 1;
}

void on_item_activate(GtkWidget *input, gpointer command)
{
	gchar *toexec = g_strdup(gtk_widget_get_name(input));
	GError *error = NULL;
	GPid pid;
	gboolean success = g_spawn_async_with_pipes(NULL, (gchar *[]){"/bin/sh", "-c", toexec, NULL},
		NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &pid, NULL, NULL, NULL, &error);

	if (!success)
	{
		g_warning("Error running command: %s", error->message);
		g_error_free(error);
	}
	else
	{
		g_spawn_close_pid(pid);
	}

	g_free(toexec);
}
