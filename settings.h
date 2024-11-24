void readconf(void)
{
	//READ THE CONF
	snprintf(config_file_path, sizeof(config_file_path), "/etc/sgwelcome.conf");
	//config_file_path = g_build_filename("etc", "sgwelcome.conf", NULL);

	FILE *file = fopen(config_file_path, "r");
	if (file == NULL)
	{
		if (file == NULL)
		{
			FILE *default_conf = fopen("/etc/sgwelcome.conf", "r");
			if (default_conf == NULL)
			{
				fprintf(stderr, "Error: could not open default configuration file /etc/sgwelcome.conf, please reinstall the program or put a config file in /etc/sgwelcome.conf.\n");
				exit(1);
			}
		fclose(default_conf);
		}
		else
		{
			fclose(file);
			printf("%s exists and can be read.\n", config_file_path);
		}
	}

// Open the file for reading
	char line[ML];
		if (file != NULL)
		{
			while (fgets(line, ML, file) != NULL)
			{
				gchar *name = strtok(line, "=");
				gchar *value_str = strtok(NULL, "=");

				if (name != NULL && value_str != NULL)
				{
					if (strcmp(name, "distrotitle") == 0) 
					{
						strncpy(distrotitle, value_str, sizeof(distrotitle));
							char *newline = strchr(distrotitle, '\n');
							if (newline != NULL)
							{
								*newline = '\0'; 
							}
					} 
					else if (strcmp(name, "distrologo") == 0) 
					{
						strncpy(distrologo, value_str, sizeof(distrologo));
						char *newline = strchr(distrologo, '\n');
							if (newline != NULL)
							{
								*newline = '\0'; 
							}
					} 
					else if (strcmp(name, "mainfolder") == 0)
					{
						strncpy(mainfolder, value_str, sizeof(mainfolder));
						char *newline = strchr(mainfolder, '\n');
							if (newline != NULL)
							{
								*newline = '\0'; 
							}
					}
					else if (strcmp(name, "tabposition") == 0) 
					{
						tabposition = atoi(value_str);
					}
					else if (strcmp(name, "showcomment") == 0) 
					{
						showcomment = atoi(value_str);
					}
					else if (strcmp(name, "actiondefaultsize") == 0) 
					{
						actiondefaultsize = atoi(value_str);
					}
					else if (strcmp(name, "windowsizey") == 0) 
					{
						windowsizey = atoi(value_str);
					}
					else if (strcmp(name, "windowsizex") == 0) 
					{
						windowsizex = atoi(value_str);
					}
				}
			}

	fclose(file); // Close the file
	}
	else
	{
		printf("Error opening file");
	}
	printf("distrotitle: %s\ndistrologo: %s\nmainfolder: %s\ntabposition: %d\n",
			distrotitle,distrologo,mainfolder,tabposition);
}
