#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct	s_zone
{
	int width;
	int height;
	char col;
}				t_zone;

typedef struct	s_rec
{
	char type;
	float x;
	float y;
	float width;
	float height;
	char col;
}				t_rec;


void	ft_putstr(char *str)
{
	int i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}


int getinfo(FILE *file, t_zone *zone)
{
	int ret;
	ret = fscanf(file, "%d %d %c \n", &zone->width, &zone->height, &zone->col);
	if (ret != 3)
		return 0;
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return 0;
	return 1;
}

char *matrix_generator(t_zone zone)
{
	char *str;
	
	str = malloc(sizeof(char) * (zone.width * zone.height + 1));
	if (str == NULL)
		return NULL;
	int i = 0;
	while (i < zone.height * zone.width)
	{
		str[i]= zone.col;
		i++;
	}
	str[i] = '\0';
	return str;
}

void draw(char *str, t_zone zone)
{
	int i = 0;
	while (i < zone.height)
	{
		write(1, &str[i* zone.width], zone.width);
		write(1, "\n", 1);
		i++;
	}
}

int valid_rec(t_rec rec)
{
	if (rec.type != 'r' && rec.type != 'R')
		return 0;
	if (rec.width <= 0.00000000 || rec.height <= 0.00000000)
		return 0;
	return (1);
}

int is_in_rectangle(t_rec rec, float i, float j)
{
	if (j < rec.x || j > rec.x + rec.width || i < rec.y || i > rec.y + rec.height)
		return 0;
	if (j < 1.00000000 + rec.x  || j > rec.x + rec.width - 1.00000000 || i < rec.y + 1.000000 || i > rec.y + rec.height - 1.00000000)
		return 2;
	return 1;
}

void	set_rec(char *str, t_rec rec, t_zone zone)
{
	int i = 0;
	int j;
	int in;
	while (i < zone.height)
	{
		j = 0;
		while (j < zone.width)
		{

			in = is_in_rectangle(rec, (float)i, (float)j);
			if(rec.type == 'R')
			{
					if (in > 0)
						str[(i *zone.width) +j ] = rec.col;
			}
			else if (rec.type == 'r')
			{
					if (in ==2)
						str[(i *zone.width) +j ] = rec.col;
			}


			j++;
		}
		i++;
	}
}




int main(int argc, char **argv)
{
	if (argc != 2)
	{
		ft_putstr("Error: argument\n");
		return (1);
	}
	FILE *file;
	if (!(file = fopen(argv[1], "r")))
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	t_zone zone;
	if (getinfo(file, &zone) == 0)
	{
		ft_putstr("Error: Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	char *matrix;
	matrix = matrix_generator(zone);
	if (matrix == NULL)
	{
		ft_putstr("Error: Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	int ret;
	t_rec rec;
	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &rec.type, &rec.x, &rec.y, &rec.width, &rec.height, &rec.col )) == 6)
	{
		if (valid_rec(rec) == 0)
		{
			ft_putstr("Error: Operation file corrupted\n");
			free(matrix);
			fclose(file);
			return (1);
		}
		set_rec(matrix, rec, zone); 
	}
	if (ret != -1)
	{
		ft_putstr("Error: Operation file corrupted\n");
		free(matrix);
		fclose(file);
		return (1);
	}

	draw(matrix, zone);
	free(matrix);
	fclose(file);
	return (0);
}
