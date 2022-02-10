#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

typedef struct s_zone
{
	int width;
	int height;
	char col;

} t_zone;

typedef struct s_circle
{
	char type;
	float x;
	float y;
	float radius;
	char col;

} t_circle;

void	ft_putstr(char *str)
{
	int i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

int get_data(FILE *file, t_zone *zone)
{
	int ret;
	ret = fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->col);
	if (ret != 3)
		return 0;
	if (zone->width <= 0 || zone->height <= 0 || zone->width > 300 || zone->height > 300)
		return 0;
	return 1;
}

char *matrix_generator(t_zone zone)
{
	char *str;
	str = malloc(sizeof(char) * (zone.width * zone.height + 1));
	if (str == NULL)
		return (NULL);
	int i = 0;
	while (i < zone.width * zone.height)
	{
		str[i] = zone.col;
		i++;
	}
	str[i] = '\0';
	return str;
}

void draw(char*str , t_zone zone)
{
	for (int i = 0; i < zone.height; i++)
	{
		write(1, &str[i * zone.width], zone.width);
		write(1, "\n", 1);
	}
}

int valid_circle(t_circle circle)
{
	if (circle.radius <= 0.00000000)
		return 0;
	if (circle.type != 'c' && circle.type != 'C')
		return 0;
	return 1;
}

void set_circle(char *matrix, t_circle circle, t_zone zone)
{
	int i = 0;
	int j;
	float dist;
	while (i < zone.height)
	{
		j = 0;
		while (j < zone.width)
		{
			dist  = sqrtf((circle.x - (float)j) * (circle.x - (float)j) + (circle.y - (float)i) * (circle.y - (float)i));
			if (circle.type == 'C')
			{
				if (dist <= circle.radius)
					matrix[(i*zone.width) + j] = circle.col;

			}
			else if (circle.type == 'c')
			{
				if (dist <= circle.radius && dist > circle.radius - 1.00000000)
					matrix[(i*zone.width) + j] = circle.col;
			}
			j++;
		}
		i++;
	}
}


int main(int argc, char **argv)
{
	FILE *file;

	if (argc != 2)
	{
		ft_putstr("Error: argument\n");
		return (1);
	}
	if(!(file = fopen(argv[1], "r")))
	{
		ft_putstr("Error: Operation file corrupted\n");
		return (1);
	}
	t_zone zone;
	if (get_data(file, &zone) == 0)
	{
		ft_putstr("Error: Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	char *matrix = NULL;
	matrix = matrix_generator(zone);
	if(matrix == NULL)
	{
		ft_putstr("Error: Operation file corrupted\n");
		fclose(file);
		return (1);
	}
	int ret;
	t_circle circle;
	while ((ret = fscanf(file, "%c %f %f %f %c\n", &circle.type, &circle.x, &circle.y, &circle.radius, &circle.col)) == 5)
	{
		if (!valid_circle(circle))
		{
			ft_putstr("Error: Operation file corrupted\n");
			free(matrix);
			fclose(file);
			return (1);
		}
		set_circle(matrix, circle, zone);
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
	return 0;
}

